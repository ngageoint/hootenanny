/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HootApiDb.h"

// hoot
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/SqlBulkInsert.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/ElementCacheLRU.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/io/TableType.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/io/SqlBulkDelete.h>

// qt
#include <QStringList>
#include <QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

// Standard
#include <math.h>
#include <cmath>
#include <fstream>

// tgs
#include <tgs/System/Time.h>

#include "InternalIdReserver.h"

using namespace geos::geom;
using namespace std;

namespace hoot
{

unsigned int HootApiDb::logWarnCount = 0;

HootApiDb::HootApiDb() :
_precision(ConfigOptions().getWriterPrecision()),
_createIndexesOnClose(true),
_flushOnClose(true)
{
  _init();
}

HootApiDb::~HootApiDb()
{
  close();
}

void HootApiDb::_init()
{
  _floatingPointCoords = true;
  _capitalizeRelationMemberType = false;
  _inTransaction = false;

  int recordsPerBulkInsert = 500;

  // set it to something obsurd.
  _lastMapId = -numeric_limits<long>::max();

  _nodesInsertElapsed = 0;
  // 500 found experimentally on my desktop -JRS
  _nodesPerBulkInsert = recordsPerBulkInsert;
  _nodesPerBulkDelete = recordsPerBulkInsert; //TODO: figure out what this should be

  _wayNodesInsertElapsed = 0;
  // arbitrary, needs benchmarking
  _wayNodesPerBulkInsert = recordsPerBulkInsert;

  _wayInsertElapsed = 0;
  // arbitrary, needs benchmarking
  _waysPerBulkInsert = recordsPerBulkInsert;

  // arbitrary, needs benchmarking
  _relationsPerBulkInsert = recordsPerBulkInsert;

  _currUserId = -1;
  _currMapId = -1;
  _currChangesetId = -1;
  _changesetEnvelope.init();
  _changesetChangeCount = 0;
}

Envelope HootApiDb::calculateEnvelope() const
{
  LOG_TRACE("Calculating envelope...");

  const long mapId = _currMapId;
  Envelope result;

  // if you're having performance issues read this:
  // http://www.postgresql.org/docs/8.0/static/functions-aggregate.html
  QSqlQuery boundsQuery = _exec("SELECT MIN(latitude) as minLat, MAX(latitude) AS maxLat "
                             ", MIN(longitude) as minLon, MAX(longitude) AS maxLon"
                             " FROM " + getCurrentNodesTableName(mapId));

  if (boundsQuery.next())
  {
    double minY = boundsQuery.value(0).toDouble();
    double maxY = boundsQuery.value(1).toDouble();
    double minX = boundsQuery.value(2).toDouble();
    double maxX = boundsQuery.value(3).toDouble();
    result = Envelope(minX, maxX, minY, maxY);
  }
  else
  {
    QString error = QString("Error calculating bounds: %1").arg(boundsQuery.lastError().text());
    throw HootException(error);
  }

  return result;
}

void HootApiDb::_checkLastMapId(long mapId)
{
  LOG_TRACE("Checking last map ID: " << mapId << "...");
  LOG_VART(_lastMapId);
  if (_lastMapId != mapId)
  {
    _flushBulkInserts();
    _flushBulkDeletes();
    _resetQueries();
    _nodeIdReserver.reset();
    _wayIdReserver.reset();
    _relationIdReserver.reset();
    _lastMapId = mapId;
    LOG_VART(_lastMapId);
  }
}

void HootApiDb::close()
{
  LOG_TRACE("Closing database connection...");

  if (_createIndexesOnClose)
  {
    createPendingMapIndexes();
  }
  if (_flushOnClose)
  {
    _flushBulkInserts();
    _flushBulkDeletes();
  }

  _resetQueries();

  if (_inTransaction)
  {
    LOG_WARN("Closing database before transaction is committed. Rolling back transaction.");
    rollback();
  }

  // Seeing this? "Unable to free statement: connection pointer is NULL"
  // Make sure all queries are listed in _resetQueries.
  _db.close();
}

void HootApiDb::commit()
{
  LOG_TRACE("Committing transaction...");

  if (_db.isOpen() == false)
  {
    throw HootException("Tried to commit a transaction on a closed database.");
  }

  if (_inTransaction == false)
  {
    throw HootException(QString("Tried to commit but weren't in a transaction.  You may ") +
                        QString("need to set hootapi.db.writer.create.user=true."));
  }

  createPendingMapIndexes();
  _flushBulkInserts();
  _flushBulkDeletes();
  _resetQueries();
  if (!_db.commit())
  {
    throw HootException("Error committing transaction: " + _db.lastError().text());
  }

  _inTransaction = false;

  // If we get this far, transaction commit was successful. Now execute
  // all of our post-transaction tasks. (like drop database statements)
  // A task that throws an error will prevent subsequent tasks from executing.
  QVectorIterator<QString> taskIt(_postTransactionStatements);
  while (taskIt.hasNext())
  {
    QString task = taskIt.next();
    DbUtils::execNoPrepare(_db, task);
  }
}

void HootApiDb::_copyTableStructure(QString from, QString to)
{
  // inserting strings in this fashion is safe b/c it is private and we closely control the table
  // names.
  QString sql = QString("CREATE TABLE %1 (LIKE %2 INCLUDING DEFAULTS INCLUDING CONSTRAINTS "
      "INCLUDING INDEXES)").arg(to).arg(from);
  QSqlQuery q(_db);

  LOG_VART(sql);
  if (q.exec(sql) == false)
  {
    QString error = QString("Error executing query: %1 (%2)").arg(q.lastError().text()).
        arg(sql);
    throw HootException(error);
  }
}

void HootApiDb::createPendingMapIndexes()
{
  if (_pendingMapIndexes.size() > 0)
  {
    LOG_DEBUG("Creating " << _pendingMapIndexes.size() << " map indexes...");
  }

  for (int i = 0; i < _pendingMapIndexes.size(); i++)
  {
    long mapId = _pendingMapIndexes[i];

    DbUtils::execNoPrepare(
      _db,
      QString("ALTER TABLE %1 "
        "ADD CONSTRAINT current_nodes_changeset_id_fkey_%2 FOREIGN KEY (changeset_id) "
          "REFERENCES %3 (id) MATCH SIMPLE "
          "ON UPDATE NO ACTION ON DELETE NO ACTION ")
          .arg(getCurrentNodesTableName(mapId))
          .arg(getMapIdString(mapId))
          .arg(getChangesetsTableName(mapId)));

    DbUtils::execNoPrepare(
      _db,
      QString("CREATE INDEX %1_tile_idx ON %1 USING btree (tile)")
        .arg(getCurrentNodesTableName(mapId)));

    DbUtils::execNoPrepare(
      _db,
      QString("ALTER TABLE %1 "
        "ADD CONSTRAINT current_relations_changeset_id_fkey_%2 FOREIGN KEY (changeset_id) "
          "REFERENCES %3 (id) MATCH SIMPLE "
          "ON UPDATE NO ACTION ON DELETE NO ACTION ")
          .arg(getCurrentRelationsTableName(mapId))
          .arg(getMapIdString(mapId))
          .arg(getChangesetsTableName(mapId)));

    DbUtils::execNoPrepare(
      _db,
      QString("ALTER TABLE %1 "
        "ADD CONSTRAINT current_way_nodes_node_id_fkey_%2 FOREIGN KEY (node_id) "
          "REFERENCES %3 (id) MATCH SIMPLE "
          "ON UPDATE NO ACTION ON DELETE NO ACTION, "
        "ADD CONSTRAINT current_way_nodes_way_id_fkey_%2 FOREIGN KEY (way_id) "
          "REFERENCES %4 (id) MATCH SIMPLE "
          "ON UPDATE NO ACTION ON DELETE NO ACTION")
          .arg(getCurrentWayNodesTableName(mapId))
          .arg(getMapIdString(mapId))
          .arg(getCurrentNodesTableName(mapId))
          .arg(getCurrentWaysTableName(mapId)));

    DbUtils::execNoPrepare(
      _db,
      QString("ALTER TABLE %1 "
        "ADD CONSTRAINT current_ways_changeset_id_fkey_%2 FOREIGN KEY (changeset_id) "
          "REFERENCES %3 (id) MATCH SIMPLE "
          "ON UPDATE NO ACTION ON DELETE NO ACTION ")
          .arg(getCurrentWaysTableName(mapId))
          .arg(getMapIdString(mapId))
          .arg(getChangesetsTableName(mapId)));
  }

  _pendingMapIndexes.clear();
}

void HootApiDb::deleteMap(long mapId)
{
  LOG_TRACE("Deleting map: " << mapId << "...");

  // Drop related renderDB First
  dropDatabase(_getRenderDBName(mapId));

  // Drop related sequences
  dropSequence(getCurrentRelationMembersSequenceName(mapId));
  dropSequence(getCurrentRelationsSequenceName(mapId));
  dropSequence(getCurrentWayNodesSequenceName(mapId));
  dropSequence(getCurrentWaysSequenceName(mapId));
  dropSequence(getCurrentNodesSequenceName(mapId));
  dropSequence(getChangesetsSequenceName(mapId));

  // Drop related tables
  dropTable(getCurrentRelationMembersTableName(mapId));
  dropTable(getCurrentRelationsTableName(mapId));
  dropTable(getCurrentWayNodesTableName(mapId));
  dropTable(getCurrentWaysTableName(mapId));
  dropTable(getCurrentNodesTableName(mapId));
  dropTable(getChangesetsTableName(mapId));

  // Delete map last
  _exec("DELETE FROM " + ApiDb::getMapsTableName() + " WHERE id=:id", (qlonglong)mapId);
}

bool HootApiDb::hasTable(const QString& tableName)
{
  QString sql = "SELECT 1 from pg_catalog.pg_class c JOIN pg_catalog.pg_namespace n ON "
      "n.oid = c.relnamespace WHERE c.relname = :name";
  QSqlQuery q = _exec(sql, tableName);

  return q.next();
}

void HootApiDb::dropDatabase(const QString& databaseName)
{
  LOG_TRACE("Dropping database: " << databaseName << "...");

  QString sql = QString("DROP DATABASE IF EXISTS \"%1\"").arg(databaseName);

  // TRICKY: You are not allowed to drop a database within a transaction.
  // If this is the case, we store the statement & execute it right after
  // the current trans is successfully committed.
  if (_inTransaction)
  {
    // Store for later
    _postTransactionStatements.push_back(sql);
  }
  else
  {
    // Execute now
    DbUtils::execNoPrepare(_db, sql);
  }
}

void HootApiDb::dropTable(const QString& tableName)
{
  LOG_TRACE("Dropping table: " << tableName << "...");

  // inserting strings in this fashion is safe b/c it is private and we closely control the table
  // names.
  QString sql = QString("DROP TABLE IF EXISTS %1 CASCADE;").arg(tableName);
  QSqlQuery q(_db);

  if (q.exec(sql) == false)
  {
    QString error = QString("Error executing query: %1 (%2)").arg(q.lastError().text()).
        arg(sql);
    throw HootException(error);
  }
}

void HootApiDb::dropSequence(const QString& sequenceName)
{
  LOG_TRACE("Dropping sequence: " << sequenceName << "...");

  // inserting strings in this fashion is safe b/c it is private and we closely control the sequence
  // names.
  QString sql = QString("DROP SEQUENCE IF EXISTS %1 CASCADE;").arg(sequenceName);
  QSqlQuery q(_db);

  if (q.exec(sql) == false)
  {
    QString error = QString("Error executing query: %1 (%2)").arg(q.lastError().text()).
        arg(sql);
    throw HootException(error);
  }
}

void HootApiDb::deleteUser(long userId)
{
  LOG_TRACE("Deleting user: " << userId << "...");

  QSqlQuery maps = _exec("SELECT id FROM " + ApiDb::getMapsTableName() +
                         " WHERE user_id=:user_id", (qlonglong)userId);

  // delete all the maps owned by this user
  while (maps.next())
  {
    long mapId = maps.value(0).toLongLong();
    deleteMap(mapId);
  }

  _exec("DELETE FROM " + ApiDb::getUsersTableName() + " WHERE id=:id", (qlonglong)userId);
}

QString HootApiDb::_escapeTags(const Tags& tags)
{
  //TODO: this is likely redundant with other code

  QStringList l;
  static QChar f1('\\'), f2('"');

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString key = it.key();
    QString val = it.value().trimmed();
    if (val.isEmpty() == false)
    {
      // this doesn't appear to be working, but I think it is implementing the spec as described here:
      // http://www.postgresql.org/docs/9.0/static/hstore.html
      // The spec described above does seem to work on the psql command line. Curious.
      QString k = QString(key).replace(f1, "\\\\").replace(f2, "\\\"").replace("'", "''");
      QString v = QString(val).replace(f1, "\\\\").replace(f2, "\\\"").replace("'", "''");

      l << QString("'%1'").arg(k);
      l << QString("'%1'").arg(v);
    }
  }

  QString hstoreStr = l.join(",");
  if (!hstoreStr.isEmpty())
  {
     hstoreStr = "hstore(ARRAY[" + hstoreStr + "])";
  }
  else
  {
     hstoreStr = "''";
  }
  return hstoreStr;
}

QString HootApiDb::execToString(QString sql, QVariant v1, QVariant v2, QVariant v3)
{
  QSqlQuery q = _exec(sql, v1, v2, v3);

  QStringList l;
  while (q.next())
  {
    QStringList row;
    for (int i = 0; i < q.record().count(); ++i)
    {
      row.append(q.value(i).toString());
    }
    l.append(row.join(";"));
  }

  q.finish();

  return l.join("\n");
}

void HootApiDb::_flushBulkInserts()
{
  LOG_TRACE("Flushing bulk inserts...");

  if (_nodeBulkInsert != 0)
  {
    LOG_VARD(_nodeBulkInsert->getPendingCount());
    _nodeBulkInsert->flush();
  }
  if (_wayBulkInsert != 0)
  {
    LOG_VARD(_wayBulkInsert->getPendingCount());
    _wayBulkInsert->flush();
  }
  if (_wayNodeBulkInsert != 0)
  {
    LOG_VARD(_wayNodeBulkInsert->getPendingCount());
    _wayNodeBulkInsert->flush();
  }
  if (_relationBulkInsert != 0)
  {
    LOG_VARD(_relationBulkInsert->getPendingCount());
    _relationBulkInsert->flush();
  }
}

void HootApiDb::_flushBulkDeletes()
{
  LOG_TRACE("Flushing bulk deletes...");

  if (_nodeBulkDelete != 0)
  {
    LOG_VARD(_nodeBulkDelete->getPendingCount());
    _nodeBulkDelete->flush();
  }
}

bool HootApiDb::isCorrectHootDbVersion()
{
  return getHootDbVersion() == ApiDb::expectedHootDbVersion();
}

QString HootApiDb::getHootDbVersion()
{
  if (_selectHootDbVersion == 0)
  {
    _selectHootDbVersion.reset(new QSqlQuery(_db));
    _selectHootDbVersion->prepare("SELECT id || ':' || author AS version_id FROM databasechangelog "
                                  "ORDER BY dateexecuted DESC LIMIT 1");
  }

  if (_selectHootDbVersion->exec() == false)
  {
    throw HootException(_selectHootDbVersion->lastError().text());
  }

  QString result;
  if (_selectHootDbVersion->next())
  {
    result = _selectHootDbVersion->value(0).toString();
  }
  else
  {
    throw HootException("Unable to retrieve the DB version.");
  }

  return result;
}

long HootApiDb::_getNextNodeId()
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);
  if (_nodeIdReserver == 0)
  {
    _nodeIdReserver.reset(new InternalIdReserver(_db, getCurrentNodesSequenceName(mapId)));
  }
  return _nodeIdReserver->getNextId();
}

long HootApiDb::_getNextRelationId()
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);
  if (_relationIdReserver == 0)
  {
    _relationIdReserver.reset(new InternalIdReserver(_db, getCurrentRelationsSequenceName(mapId)));
  }
  return _relationIdReserver->getNextId();
}

long HootApiDb::_getNextWayId()
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);
  if (_wayIdReserver == 0)
  {
    _wayIdReserver.reset(new InternalIdReserver(_db, getCurrentWaysSequenceName(mapId)));
  }
  return _wayIdReserver->getNextId();
}

long HootApiDb::getNextId(const ElementType& elementType)
{
  switch (elementType.getEnum())
  {
    case ElementType::Node:
      return _getNextNodeId();

    case ElementType::Way:
      return _getNextWayId();

    case ElementType::Relation:
      return _getNextRelationId();

    default:
      throw HootException(QString("Unexpected element type: %1").arg(elementType.toString()));
  }
}

void HootApiDb::beginChangeset()
{
  Tags emptyTags;
  return beginChangeset(emptyTags);
}

void HootApiDb::beginChangeset(const Tags& tags)
{
  LOG_DEBUG("Starting changeset...");

  _changesetEnvelope.init();
  _changesetChangeCount = 0;
  const long mapId = _currMapId;
  const long userId = _currUserId;

  _checkLastMapId(mapId);
  if (_insertChangeSet == 0)
  {
    _insertChangeSet.reset(new QSqlQuery(_db));
    _insertChangeSet->prepare(
      QString("INSERT INTO %1 (user_id, created_at, min_lat, max_lat, min_lon, max_lon, "
        "closed_at, tags) "
        "VALUES (:user_id, NOW(), :min_lat, :max_lat, :min_lon, :max_lon, NOW(), " +
        _escapeTags(tags) + ") "
        "RETURNING id")
        .arg(getChangesetsTableName(mapId)));
  }
  _insertChangeSet->bindValue(":user_id", (qlonglong)userId);
  _insertChangeSet->bindValue(":min_lat", _changesetEnvelope.getMinY());
  _insertChangeSet->bindValue(":max_lat", _changesetEnvelope.getMaxY());
  _insertChangeSet->bindValue(":min_lon", _changesetEnvelope.getMinX());
  _insertChangeSet->bindValue(":max_lon", _changesetEnvelope.getMaxX());
  LOG_VARD(_insertChangeSet->lastQuery());

  _currChangesetId = _insertRecord(*_insertChangeSet);
  LOG_VARD(_currChangesetId);

  _changesetChangeCount = 0;
  LOG_DEBUG("Started new changeset " << QString::number(_currChangesetId));
}

long HootApiDb::insertChangeset(const geos::geom::Envelope& bounds, const Tags& tags,
                                const long numChanges)
{
  LOG_DEBUG("Inserting and closing changeset...");

  const long mapId = _currMapId;
  const long userId = _currUserId;

  _checkLastMapId(mapId);
  if (_insertChangeSet2 == 0)
  {
    _insertChangeSet2.reset(new QSqlQuery(_db));
    _insertChangeSet2->prepare(
      QString("INSERT INTO %1 (user_id, created_at, min_lat, max_lat, min_lon, max_lon, "
        "closed_at, num_changes, tags) "
        "VALUES (:user_id, NOW(), :min_lat, :max_lat, :min_lon, :max_lon, NOW(), :num_changes, " +
        _escapeTags(tags) + ") "
        "RETURNING id")
        .arg(getChangesetsTableName(mapId)));
  }
  _insertChangeSet2->bindValue(":user_id", (qlonglong)userId);
  _insertChangeSet2->bindValue(":min_lat", bounds.getMinY());
  _insertChangeSet2->bindValue(":max_lat", bounds.getMaxY());
  _insertChangeSet2->bindValue(":min_lon", bounds.getMinX());
  _insertChangeSet2->bindValue(":max_lon", bounds.getMaxX());
  _insertChangeSet2->bindValue(":num_changes", (int)numChanges);
  LOG_VARD(_insertChangeSet2->lastQuery());

  _currChangesetId = _insertRecord(*_insertChangeSet2);
  LOG_VARD(_currChangesetId);

  _changesetChangeCount = 0;
  LOG_DEBUG("Inserted and closed changeset " << QString::number(_currChangesetId));

  return _currChangesetId;
}

void HootApiDb::endChangeset()
{
  LOG_DEBUG("Ending changeset...");

  // If we're already closed, nothing to do
  if (_currChangesetId == -1)
  {
    LOG_DEBUG("Tried to end a changeset but there isn't an active changeset currently.");
    return;
  }

  const long mapId = _currMapId;
  if (!changesetExists(_currChangesetId))
  {
    throw HootException("No changeset exists with ID: " + _currChangesetId);
  }

  _checkLastMapId(mapId);
  if (_closeChangeSet == 0)
  {
    _closeChangeSet.reset(new QSqlQuery(_db));
    _closeChangeSet->prepare(
      QString("UPDATE %1 SET min_lat=:min_lat, max_lat=:max_lat, min_lon=:min_lon, "
        "max_lon=:max_lon, closed_at=NOW(), num_changes=:num_changes WHERE id=:id")
         .arg(getChangesetsTableName(mapId)));
  }
  _closeChangeSet->bindValue(":min_lat", _changesetEnvelope.getMinY());
  _closeChangeSet->bindValue(":max_lat", _changesetEnvelope.getMaxY());
  _closeChangeSet->bindValue(":min_lon", _changesetEnvelope.getMinX());
  _closeChangeSet->bindValue(":max_lon", _changesetEnvelope.getMaxX());
  _closeChangeSet->bindValue(":num_changes", (int)_changesetChangeCount);
  _closeChangeSet->bindValue(":id", (qlonglong)_currChangesetId);
  LOG_VARD(_closeChangeSet->lastQuery());

  if (_closeChangeSet->exec() == false)
  {
    LOG_ERROR("query bound values: ");
    LOG_ERROR(_closeChangeSet->boundValues());
    LOG_ERROR("\n");
    throw HootException("Error executing close changeset: " + _closeChangeSet->lastError().text() +
                        " (SQL: " + _closeChangeSet->executedQuery() + ")" + " with envelope: " +
                        QString::fromStdString(_changesetEnvelope.toString()));
  }

  LOG_DEBUG("Successfully closed changeset " << QString::number(_currChangesetId));

  // NOTE: do *not* alter _currChangesetId or _changesetEnvelope yet.  We haven't written data to
  //database yet!   they will be refreshed upon opening a new database, so leave them alone!
  _changesetChangeCount = 0;
}

long HootApiDb::insertMap(QString displayName, bool publicVisibility)
{
  LOG_TRACE("Inserting map...");

  const int userId = _currUserId;

  if (_insertMap == 0)
  {
    _insertMap.reset(new QSqlQuery(_db));
    _insertMap->prepare("INSERT INTO " + ApiDb::getMapsTableName() +
                        " (display_name, user_id, public, created_at) "
                        "VALUES (:display_name, :user_id, :public, NOW()) "
                        "RETURNING id");
  }
  _insertMap->bindValue(":display_name", displayName);
  _insertMap->bindValue(":user_id", userId);
  _insertMap->bindValue(":public", publicVisibility);

  long mapId = _insertRecord(*_insertMap);

  _copyTableStructure(ApiDb::getChangesetsTableName(), getChangesetsTableName(mapId));
  _copyTableStructure(ApiDb::getCurrentNodesTableName(), getCurrentNodesTableName(mapId));
  _copyTableStructure(
    ApiDb::getCurrentRelationMembersTableName(), getCurrentRelationMembersTableName(mapId));
  _copyTableStructure(ApiDb::getCurrentRelationsTableName(), getCurrentRelationsTableName(mapId));
  _copyTableStructure(ApiDb::getCurrentWayNodesTableName(), getCurrentWayNodesTableName(mapId));
  _copyTableStructure(ApiDb::getCurrentWaysTableName(), getCurrentWaysTableName(mapId));

  DbUtils::execNoPrepare(_db, "CREATE SEQUENCE " + getChangesetsSequenceName(mapId));
  DbUtils::execNoPrepare(_db, "CREATE SEQUENCE " + getCurrentNodesSequenceName(mapId));
  DbUtils::execNoPrepare(_db, "CREATE SEQUENCE " + getCurrentRelationsSequenceName(mapId));
  DbUtils::execNoPrepare(_db, "CREATE SEQUENCE " + getCurrentWaysSequenceName(mapId));

  DbUtils::execNoPrepare(
    _db,
    QString("ALTER TABLE %1 "
      "ALTER COLUMN id SET DEFAULT NEXTVAL('%4'::regclass)")
        .arg(getCurrentNodesTableName(mapId))
        .arg(getCurrentNodesSequenceName(mapId)));

  DbUtils::execNoPrepare(
    _db,
    QString("ALTER TABLE %1 "
      "ALTER COLUMN id SET DEFAULT NEXTVAL('%4'::regclass)")
        .arg(getCurrentRelationsTableName(mapId))
        .arg(getCurrentRelationsSequenceName(mapId)));

  DbUtils::execNoPrepare(
    _db,
    QString("ALTER TABLE %1 "
      "ALTER COLUMN id SET DEFAULT NEXTVAL('%4'::regclass)")
        .arg(getCurrentWaysTableName(mapId))
        .arg(getCurrentWaysSequenceName(mapId)));

  // remove the index to speed up inserts. It'll be added back by createPendingMapIndexes
  DbUtils::execNoPrepare(
    _db,
    QString("DROP INDEX %1_tile_idx").arg(getCurrentNodesTableName(mapId)));

  _pendingMapIndexes.append(mapId);

  return mapId;
}

bool HootApiDb::insertNode(const double lat, const double lon, const Tags& tags, long& assignedId)
{
  assignedId = _getNextNodeId();

  return insertNode(assignedId, lat, lon, tags);
}

bool HootApiDb::insertNode(const long id, const double lat, const double lon, const Tags &tags)
{
  LOG_TRACE("Inserting node: " << id << "...");

  const long mapId = _currMapId;
  double start = Tgs::Time::getTime();

  _checkLastMapId(mapId);

  if (_nodeBulkInsert == 0)
  {
    QStringList columns;
    columns << "id" << "latitude" << "longitude" << "changeset_id" << "timestamp" <<
               "tile" << "version" << "tags";

    _nodeBulkInsert.reset(new SqlBulkInsert(_db, getCurrentNodesTableName(mapId), columns));
  }

  QList<QVariant> v;
  v.append((qlonglong)id);
  v.append(lat);
  v.append(lon);
  v.append((qlonglong)_currChangesetId);
  v.append(OsmUtils::currentTimeAsString());
  v.append(tileForPoint(lat, lon));
  v.append((qlonglong)1);
  // escaping tags ensures that we won't introduce a SQL injection vulnerability, however, if a
  // bad tag is passed and it isn't escaped properly (shouldn't happen) it may result in a syntax
  // error.
  v.append(_escapeTags(tags));

  _nodeBulkInsert->insert(v);

  _nodesInsertElapsed += Tgs::Time::getTime() - start;

  if (_nodeBulkInsert->getPendingCount() >= _nodesPerBulkInsert)
  {
    _nodeBulkInsert->flush();
  }

  ConstNodePtr envelopeNode(new Node(Status::Unknown1, id, lon, lat, 0.0));
  _updateChangesetEnvelope(envelopeNode);

  LOG_TRACE("Inserted node: " << ElementId(ElementType::Node, id));
  LOG_VART(QString::number(lat, 'g', _precision))
  LOG_VART(QString::number(lon, 'g', _precision));

  return true;
}

bool HootApiDb::insertNode(ConstNodePtr node)
{
  return insertNode(node->getId(), node->getY(), node->getX(), node->getTags());
}

void HootApiDb::updateNode(ConstNodePtr node)
{
  return updateNode(node->getId(), node->getY(), node->getX(), node->getVersion(), node->getTags());
}

void HootApiDb::deleteNode(ConstNodePtr node)
{
  LOG_TRACE("Deleting node: " << node->getId() << "...");

  const long mapId = _currMapId;
  double start = Tgs::Time::getTime();

  _checkLastMapId(mapId);

  if (_nodeBulkDelete == 0)
  {
    _nodeBulkDelete.reset(new SqlBulkDelete(_db, getCurrentNodesTableName(mapId)));
  }
  _nodeBulkDelete->deleteElement(node->getId());

  _nodesDeleteElapsed += Tgs::Time::getTime() - start;

  if (_nodeBulkDelete->getPendingCount() >= _nodesPerBulkDelete)
  {
    _nodeBulkDelete->flush();
  }

  //TODO: multiary-ingest: how to update envelope here?
  //ConstNodePtr envelopeNode(new Node(Status::Unknown1, id, lon, lat, 0.0));
  //_updateChangesetEnvelope(envelopeNode);

  LOG_TRACE("Deleted node: " << ElementId(ElementType::Node, node->getId()));
}

bool HootApiDb::insertRelation(const Tags &tags, long& assignedId)
{
  assignedId = _getNextRelationId();

  return insertRelation(assignedId, tags);
}

bool HootApiDb::insertRelation(const long relationId, const Tags &tags)
{
  LOG_TRACE("Inserting relation: " << relationId << "...");

  const long mapId = _currMapId;
  _checkLastMapId(mapId);

  if (_relationBulkInsert == 0)
  {
    QStringList columns;
    columns << "id" << "changeset_id" << "timestamp" << "version" << "tags";

    _relationBulkInsert.reset(new SqlBulkInsert(_db, getCurrentRelationsTableName(mapId), columns));
  }

  QList<QVariant> v;
  v.append((qlonglong)relationId);
  v.append((qlonglong)_currChangesetId);
  v.append(OsmUtils::currentTimeAsString());
  v.append((qlonglong)1);
  // escaping tags ensures that we won't introduce a SQL injection vulnerability, however, if a
  // bad tag is passed and it isn't escaped properly (shouldn't happen) it may result in a syntax
  // error.
  v.append(_escapeTags(tags));

  _relationBulkInsert->insert(v);

  _lazyFlushBulkInsert();

  LOG_TRACE("Inserted relation: " << ElementId(ElementType::Relation, relationId));

  return true;
}

bool HootApiDb::insertRelationMember(const long relationId, const ElementType& type,
  const long elementId, const QString& role, const int sequenceId)
{
  LOG_TRACE("Inserting relation member for relation: " << relationId << "...");

  const long mapId = _currMapId;
  _checkLastMapId(mapId);

  if (_insertRelationMembers == 0)
  {
    _insertRelationMembers.reset(new QSqlQuery(_db));
    _insertRelationMembers->prepare(
      "INSERT INTO " + getCurrentRelationMembersTableName(mapId) +
        " (relation_id, member_type, member_id, member_role, sequence_id) "
      "VALUES (:relation_id, :member_type, :member_id, :member_role, :sequence_id)");
  }

  _insertRelationMembers->bindValue(":relation_id", (qlonglong)relationId);
  _insertRelationMembers->bindValue(":member_type", type.toString().toLower());
  _insertRelationMembers->bindValue(":member_id", (qlonglong)elementId);
  _insertRelationMembers->bindValue(":member_role", role);
  _insertRelationMembers->bindValue(":sequence_id", sequenceId);

  if (!_insertRelationMembers->exec())
  {
    throw HootException("Error inserting relation memeber: " +
      _insertRelationMembers->lastError().text());
  }
  return true;
}

long HootApiDb::getOrCreateUser(QString email, QString displayName)
{
  long result = getUserId(email, false);

  if (result == -1)
  {
    result = insertUser(email, displayName);
  }

  return result;
}

void HootApiDb::setUserId(const long sessionUserId)
{
  _currUserId = sessionUserId;

  LOG_DEBUG("User ID updated to " + QString::number(_currUserId));
}

void HootApiDb::setMapId(const long sessionMapId)
{
  _currMapId = sessionMapId;
  assert(_currMapId > 0);

  LOG_DEBUG("Map ID updated to " + QString::number(_currMapId));
}

long HootApiDb::_insertRecord(QSqlQuery& query)
{
  if (query.exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(query.executedQuery()).
        arg(query.lastError().text());
    LOG_ERROR(err);
    throw HootException(err);
  }
  bool ok = false;
  long id = -1;
  if (query.next())
  {
    id = query.value(0).toLongLong(&ok);
  }

  if (!ok || id == -1)
  {
    LOG_ERROR("query bound values: ");
    LOG_ERROR(query.boundValues());
    LOG_ERROR("\n");
    throw HootException("Error retrieving new ID " + query.lastError().text() + " Query: " +
      query.executedQuery());
  }

  query.finish();

  return id;
}

bool HootApiDb::isSupported(const QUrl& url)
{
  bool valid = ApiDb::isSupported(url);

  //postgresql is deprecated but still supported
  if (url.scheme() != "hootapidb" && url.scheme() != "postgresql")
  {
    valid = false;
  }

  if (valid)
  {
    QString path = url.path();
    QStringList plist = path.split("/");

    if (plist.size() == 3)
    {
      if (plist[1] == "")
      {
        LOG_WARN("Looks like a DB path, but a DB name was expected. E.g. "
                 "postgresql://myhost:5432/mydb/mylayer");
        valid = false;
      }
      else if (plist[2] == "")
      {
        LOG_WARN("Looks like a DB path, but a layer name was expected. E.g. "
                 "postgresql://myhost:5432/mydb/mylayer");
        valid = false;
      }
    }
    else if ((plist.size() == 4) && ((plist[1] == "") || (plist[2 ] == "") || (plist[3] == "")))
    {
      LOG_WARN("Looks like a DB path, but a valid DB name, layer, and element was expected. E.g. "
               "postgresql://myhost:5432/mydb/mylayer/1");
      valid = false;
    }
    else
    {
      //It might be OsmApiDb url. postgresql://myhost:5432/osmapi_test
      valid = false;
    }
  }

  return valid;
}

void HootApiDb::_lazyFlushBulkInsert()
{
  bool flush = false;

  if (_nodeBulkInsert && _nodeBulkInsert->getPendingCount() > _nodesPerBulkInsert)
  {
    flush = true;
  }
  if (_wayNodeBulkInsert && _wayNodeBulkInsert->getPendingCount() > _wayNodesPerBulkInsert)
  {
    flush = true;
  }
  if (_wayBulkInsert && _wayBulkInsert->getPendingCount() > _waysPerBulkInsert)
  {
    flush = true;
  }
  if (_relationBulkInsert && _relationBulkInsert->getPendingCount() > _relationsPerBulkInsert)
  {
    flush = true;
  }

  if (flush)
  {
    _flushBulkInserts();
  }
}

void HootApiDb::open(const QUrl& url)
{
  LOG_DEBUG("Opening database connection: " << url.toString() << "...");

  if (!isSupported(url))
  {
    throw HootException("An unsupported URL was passed into HootApiDb: " + url.toString());
  }

  _resetQueries();

  ApiDb::open(url);

  if (isCorrectHootDbVersion() == false)
  {
    const QString msg = "Running against an unexpected Hootenanny DB version.";
    LOG_DEBUG("Expected: " << expectedHootDbVersion());
    LOG_DEBUG("Actual: " << getHootDbVersion());
    throw HootException(msg);
  }
}

void HootApiDb::_resetQueries()
{
  LOG_TRACE("Resetting queries...");

  ApiDb::_resetQueries();

  _closeChangeSet.reset();
  _insertChangeSet.reset();
  _insertChangeSetTag.reset();
  _insertMap.reset();
  _insertWayNodes.reset();
  _insertRelationMembers.reset();
  _selectHootDbVersion.reset();
  _insertUser.reset();
  _mapExistsById.reset();
  _changesetExists.reset();
  _selectReserveNodeIds.reset();
  _selectNodeIdsForWay.reset();
  _selectMapIds.reset();
  _selectMembersForRelation.reset();
  _updateNode.reset();
  _updateRelation.reset();
  _updateWay.reset();
  _updateJobStatus.reset();
  _insertJobStatus.reset();
  _jobStatusExists.reset();
  _mapExistsByName.reset();
  _getMapIdByName.reset();
  _insertChangeSet2.reset();
  _numChangesets.reset();

  // bulk insert objects.
  _nodeBulkInsert.reset();
  _nodeIdReserver.reset();
  _relationBulkInsert.reset();
  _relationIdReserver.reset();
  _wayNodeBulkInsert.reset();
  _wayBulkInsert.reset();
  _wayIdReserver.reset();
}

set<long> HootApiDb::selectMapIds(QString name)
{
  const long userId = _currUserId;
  LOG_DEBUG("selectMapIds name = "+name);
  LOG_DEBUG("userId = "+QString::number(userId));
  if (_selectMapIds == 0)
  {
    _selectMapIds.reset(new QSqlQuery(_db));
    _selectMapIds->prepare("SELECT id FROM " + ApiDb::getMapsTableName() +
                           " WHERE display_name LIKE :name AND user_id=:userId");
  }
  _selectMapIds->bindValue(":name", name);
  _selectMapIds->bindValue(":user_id", (qlonglong)userId);
  LOG_VARD(_selectMapIds->lastQuery());

  if (_selectMapIds->exec() == false)
  {
    throw HootException(_selectMapIds->lastError().text());
  }

  set<long> result;
  while (_selectMapIds->next())
  {
    bool ok;
    long id = _selectMapIds->value(0).toLongLong(&ok);
    if (!ok)
    {
      throw HootException("Error parsing map ID.");
    }
    result.insert(id);
  }

  return result;
}

QString HootApiDb::tableTypeToTableName(const TableType& tableType) const
{
  if (tableType == TableType::Node)
  {
    return getCurrentNodesTableName(_currMapId);
  }
  else if (tableType == TableType::Way)
  {
    return getCurrentWaysTableName(_currMapId);
  }
  else if (tableType == TableType::Relation)
  {
    return getCurrentRelationsTableName(_currMapId);
  }
  else if (tableType == TableType::WayNode)
  {
    return getCurrentWayNodesTableName(_currMapId);
  }
  else if (tableType == TableType::RelationMember)
  {
    return getCurrentRelationMembersTableName(_currMapId);
  }
  else
  {
    throw HootException("Unsupported table type.");
  }
}

bool HootApiDb::mapExists(const long id)
{
  if (_mapExistsById == 0)
  {
    _mapExistsById.reset(new QSqlQuery(_db));
    _mapExistsById->prepare("SELECT display_name FROM " + ApiDb::getMapsTableName() +
                        " WHERE id = :mapId");
  }
  _mapExistsById->bindValue(":mapId", (qlonglong)id);
  if (_mapExistsById->exec() == false)
  {
    throw HootException(_mapExistsById->lastError().text());
  }

  return _mapExistsById->next();
}

bool HootApiDb::mapExists(const QString name)
{
  if (_mapExistsByName == 0)
  {
    _mapExistsByName.reset(new QSqlQuery(_db));
    _mapExistsByName->prepare("SELECT id FROM " + ApiDb::getMapsTableName() +
                              " WHERE display_name = :mapName");
  }
  _mapExistsByName->bindValue(":mapName", name);
  if (_mapExistsByName->exec() == false)
  {
    throw HootException(_mapExistsByName->lastError().text());
  }

  return _mapExistsByName->next();
}

long HootApiDb::getMapIdByName(const QString name)
{
  //assuming unique name here
  if (_getMapIdByName == 0)
  {
    _getMapIdByName.reset(new QSqlQuery(_db));
    _getMapIdByName->prepare("SELECT id FROM " + ApiDb::getMapsTableName() +
                             " WHERE display_name = :mapName");
  }
  _getMapIdByName->bindValue(":mapName", name);
  if (_getMapIdByName->exec() == false)
  {
    throw HootException(_getMapIdByName->lastError().text());
  }

  long result = -1;
  if (_getMapIdByName->next())
  {
    bool ok;
    result = _getMapIdByName->value(0).toLongLong(&ok);
    if (!ok)
    {
      throw HootException(_getMapIdByName->lastError().text());
    }
  }
  _getMapIdByName->finish();
  return result;
}

long HootApiDb::numChangesets()
{
  if (!_numChangesets)
  {
    _numChangesets.reset(new QSqlQuery(_db));
    _numChangesets->prepare("SELECT COUNT(*) FROM " + getChangesetsTableName(_currMapId));
  }
  LOG_VARD(_numChangesets->lastQuery());

  if (!_numChangesets->exec())
  {
    LOG_ERROR(_numChangesets->executedQuery());
    LOG_ERROR(_numChangesets->lastError().text());
    throw HootException(_numChangesets->lastError().text());
  }

  long result = -1;
  if (_numChangesets->next())
  {
    bool ok;
    result = _numChangesets->value(0).toLongLong(&ok);
    if (!ok)
    {
      throw HootException(
        "Count not changeset count for map with ID: " + QString::number(_currMapId));
    }
  }
  _numChangesets->finish();
  return result;
}

bool HootApiDb::changesetExists(const long id)
{
  LOG_DEBUG("Checking changeset exists...");

  const long mapId = _currMapId;

  _checkLastMapId(mapId);
  if (_changesetExists == 0)
  {
    _changesetExists.reset(new QSqlQuery(_db));
    _changesetExists->prepare(QString("SELECT num_changes FROM %1 WHERE id = :changesetId")
      .arg(getChangesetsTableName(mapId)));
  }
  _changesetExists->bindValue(":changesetId", (qlonglong)id);
  LOG_VARD(_changesetExists->lastQuery());
  if (_changesetExists->exec() == false)
  {
    throw HootException(_changesetExists->lastError().text());
  }

  return _changesetExists->next();
}

QString HootApiDb::elementTypeToElementTableName(const ElementType& elementType) const
{
  return tableTypeToTableName(TableType::fromElementType(elementType));
}

vector<long> HootApiDb::selectNodeIdsForWay(long wayId)
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);
  QString sql = "SELECT node_id FROM " + getCurrentWayNodesTableName(mapId) +
      " WHERE way_id = :wayId ORDER BY sequence_id";
  return ApiDb::selectNodeIdsForWay(wayId, sql);
}

boost::shared_ptr<QSqlQuery> HootApiDb::selectNodesForWay(long wayId)
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);
  QString sql = "SELECT node_id FROM " + getCurrentWayNodesTableName(mapId) +
      " WHERE way_id = :wayId ORDER BY sequence_id";
  return ApiDb::selectNodesForWay(wayId, sql);
}

vector<RelationData::Entry> HootApiDb::selectMembersForRelation(long relationId)
{
  const long mapId = _currMapId;
  vector<RelationData::Entry> result;

  if (!_selectMembersForRelation)
  {
    _selectMembersForRelation.reset(new QSqlQuery(_db));
    _selectMembersForRelation->setForwardOnly(true);
    _selectMembersForRelation->prepare(
      "SELECT member_type, member_id, member_role FROM " +
      getCurrentRelationMembersTableName(mapId) +
      " WHERE relation_id = :relationId ORDER BY sequence_id");
    _selectMembersForRelation->bindValue(":mapId", (qlonglong)mapId);
  }

  _selectMembersForRelation->bindValue(":relationId", (qlonglong)relationId);
  if (_selectMembersForRelation->exec() == false)
  {
    throw HootException("Error selecting members for relation with ID: " +
      QString::number(relationId) + " Error: " + _selectMembersForRelation->lastError().text());
  }
  LOG_VART(_selectMembersForRelation->numRowsAffected());
  LOG_VART(_selectMembersForRelation->executedQuery());

  while (_selectMembersForRelation->next())
  {
    const QString memberType = _selectMembersForRelation->value(0).toString();
    LOG_VART(memberType);
    if (ElementType::isValidTypeString(memberType))
    {
      RelationData::Entry member =
        RelationData::Entry(
          _selectMembersForRelation->value(2).toString(),
          ElementId(ElementType::fromString(memberType),
          _selectMembersForRelation->value(1).toLongLong()));
      LOG_VART(member);
      result.push_back(member);
    }
    else
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Invalid relation member type: " + memberType + ".  Skipping relation member.");
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }

  return result;
}

void HootApiDb::updateNode(const long id, const double lat, const double lon, const long version,
                           const Tags& tags)
{
  LOG_TRACE("Updating node: " << id << "...");

  const long mapId = _currMapId;
  _flushBulkInserts();

  _checkLastMapId(mapId);

  if (_updateNode == 0)
  {
    _updateNode.reset(new QSqlQuery(_db));
    _updateNode->prepare(
      "UPDATE " + getCurrentNodesTableName(mapId) +
      " SET latitude=:latitude, longitude=:longitude, changeset_id=:changeset_id, "
      " timestamp=:timestamp, tile=:tile, version=:version, tags=" + _escapeTags(tags) +
      " WHERE id=:id");
  }

  _updateNode->bindValue(":id", (qlonglong)id);
  _updateNode->bindValue(":latitude", lat);
  _updateNode->bindValue(":longitude", lon);
  _updateNode->bindValue(":changeset_id", (qlonglong)_currChangesetId);
  _updateNode->bindValue(":timestamp", OsmUtils::currentTimeAsString());
  _updateNode->bindValue(":tile", (qlonglong)tileForPoint(lat, lon));
  _updateNode->bindValue(":version", (qlonglong)version);

  if (_updateNode->exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_updateNode->executedQuery()).
        arg(_updateNode->lastError().text());
    throw HootException(err);
  }

  _updateNode->finish();

  LOG_TRACE("Updated node: " << ElementId(ElementType::Node, id));
}

void HootApiDb::updateRelation(const long id, const long version, const Tags& tags)
{
  LOG_TRACE("Updating relation: " << id << "...");

  const long mapId = _currMapId;
  _flushBulkInserts();
  _checkLastMapId(mapId);

  if (_updateRelation == 0)
  {
    _updateRelation.reset(new QSqlQuery(_db));
    _updateRelation->prepare(
      "UPDATE " + getCurrentRelationsTableName(mapId) +
      " SET changeset_id=:changeset_id, timestamp=:timestamp, version=:version, tags=" +
      _escapeTags(tags) + " WHERE id=:id");
  }

  _updateRelation->bindValue(":id", (qlonglong)id);
  _updateRelation->bindValue(":changeset_id", (qlonglong)_currChangesetId);
  _updateRelation->bindValue(":timestamp", OsmUtils::currentTimeAsString());
  _updateRelation->bindValue(":version", (qlonglong)version);

  if (_updateRelation->exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_updateWay->executedQuery()).
        arg(_updateRelation->lastError().text());
    throw HootException(err);
  }

  _updateRelation->finish();

  LOG_TRACE("Updated relation: " << ElementId(ElementType::Relation, id));
}

void HootApiDb::updateWay(const long id, const long version, const Tags& tags)
{
  LOG_TRACE("Updating way: " << id << "...");

  const long mapId = _currMapId;
  _flushBulkInserts();
  _checkLastMapId(mapId);

  if (_updateWay == 0)
  {
    _updateWay.reset(new QSqlQuery(_db));
    _updateWay->prepare(
      "UPDATE " + getCurrentWaysTableName(mapId) +
      " SET changeset_id=:changeset_id, timestamp=:timestamp, version=:version, tags=" +
      _escapeTags(tags) + " WHERE id=:id");
  }

  _updateWay->bindValue(":id", (qlonglong)id);
  _updateWay->bindValue(":changeset_id", (qlonglong)_currChangesetId);
  _updateWay->bindValue(":timestamp", OsmUtils::currentTimeAsString());
  _updateWay->bindValue(":version", (qlonglong)version);

  if (_updateWay->exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_updateWay->executedQuery()).
        arg(_updateWay->lastError().text());
    throw HootException(err);
  }

  _updateWay->finish();

  LOG_TRACE("Updated way: " << ElementId(ElementType::Way, id));
}

bool HootApiDb::insertWay(const Tags &tags, long &assignedId)
{
  assignedId = _getNextWayId();

  return insertWay(assignedId, tags);
}

bool HootApiDb::insertWay(const long wayId, const Tags &tags)
{
  LOG_TRACE("Inserting way: " << wayId << "...");

  const long mapId = _currMapId;

  double start = Tgs::Time::getTime();

  _checkLastMapId(mapId);

  if (_wayBulkInsert == 0)
  {
    QStringList columns;
    columns << "id" << "changeset_id" << "timestamp" << "version" << "tags";

    _wayBulkInsert.reset(new SqlBulkInsert(_db, getCurrentWaysTableName(mapId), columns));
  }

  QList<QVariant> v;
  v.append((qlonglong)wayId);
  v.append((qlonglong)_currChangesetId);
  v.append(OsmUtils::currentTimeAsString());
  v.append((qlonglong)1);
  // escaping tags ensures that we won't introduce a SQL injection vulnerability, however, if a
  // bad tag is passed and it isn't escaped properly (shouldn't happen) it may result in a syntax
  // error.
  v.append(_escapeTags(tags));

  _wayBulkInsert->insert(v);

  _wayNodesInsertElapsed += Tgs::Time::getTime() - start;

  _lazyFlushBulkInsert();

  LOG_TRACE("Inserted way: " << ElementId(ElementType::Way, wayId));
  LOG_TRACE(tags);

  return true;
}

void HootApiDb::insertWayNodes(long wayId, const vector<long>& nodeIds)
{
  LOG_TRACE("Inserting way nodes for way: " << wayId << "...");

  const long mapId = _currMapId;
  double start = Tgs::Time::getTime();

  LOG_TRACE("Inserting nodes into way " << QString::number(wayId));

  _checkLastMapId(mapId);

  if (_wayNodeBulkInsert == 0)
  {
    QStringList columns;
    columns << "way_id" << "node_id" << "sequence_id";

    _wayNodeBulkInsert.reset(new SqlBulkInsert(_db, getCurrentWayNodesTableName(mapId), columns));
  }

  QList<QVariant> v;
  v.append((qlonglong)wayId);
  v.append((qlonglong)0);
  v.append((qlonglong)0);

  for (size_t i = 0; i < nodeIds.size(); ++i)
  {
    v[1] = (qlonglong)nodeIds[i];
    v[2] = (qlonglong)i;
    _wayNodeBulkInsert->insert(v);
  }

  _wayNodesInsertElapsed += Tgs::Time::getTime() - start;

  _lazyFlushBulkInsert();
}

void HootApiDb::incrementChangesetChangeCount()
{
  _changesetChangeCount++;

  // If we've hit maximum count of changes for a changeset, close this one out and start a new one
  if ( _changesetChangeCount >= maximumChangeSetEdits())
  {
    endChangeset();
    beginChangeset();
  }
}

void HootApiDb::_updateChangesetEnvelope(const ConstNodePtr node)
{
  const double nodeX = node->getX();
  const double nodeY = node->getY();

  _changesetEnvelope.expandToInclude(nodeX, nodeY);
  LOG_TRACE(
    "Changeset bounding box updated to include X=" + QString::number(nodeX) + ", Y=" +
    QString::number(nodeY));
}

long HootApiDb::reserveElementId(const ElementType::Type type)
{
  long retVal = -1;

  switch ( type )
  {
  case ElementType::Node:
    retVal = _getNextNodeId();
    break;

  case ElementType::Way:
    retVal = _getNextWayId();
    break;

  case ElementType::Relation:
    retVal = _getNextRelationId();
    break;

  default:
    LOG_ERROR("Requested element ID for unknown element type");
    throw HootException("reserveElementId called with unknown type");
    break;
  }

  return retVal;
}

QString HootApiDb::_getRenderDBName(long mapId)
{
  // Get current database & maps.display_name
  QString table = ApiDb::getMapsTableName();
  QString dbName = "";
  QString mapDisplayName = "";
  QString mapIdNumber = QString::number(mapId);
  QString sql = "SELECT current_database(), " + table + ".display_name "
                "FROM " + table + " WHERE " + table + ".id=" + mapIdNumber;
  QSqlQuery q = _exec(sql);

  if (q.next())
  {
    dbName = q.value(0).toString();
    mapDisplayName = q.value(1).toString();
  }

  return (dbName + "_renderdb_" + mapIdNumber);
}

QUrl HootApiDb::getBaseUrl()
{
  // read the DB values from the DB config file.
  Settings s = readDbConfig();
  QUrl result;
  result.setScheme("hootapidb");
  result.setHost(s.get("DB_HOST").toString());
  result.setPort(s.get("DB_PORT").toInt());
  result.setUserName(s.get("DB_USER").toString());
  result.setPassword(s.get("DB_PASSWORD").toString());
  result.setPath("/" + s.get("DB_NAME").toString());
  return result;
}

QString HootApiDb::removeLayerName(const QString url)
{
  QStringList urlParts =  url.split("/");
  QString modifiedUrl;
  for (int i = 0; i < urlParts.size() - 1; i++)
  {
    modifiedUrl += urlParts[i] + "/";
  }
  modifiedUrl.chop(1);
  return modifiedUrl;
}

}
