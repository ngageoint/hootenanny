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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HootApiDb.h"

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/db/SqlBulkInsert.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/ElementCacheLRU.h>
#include <hoot/core/util/OsmUtils.h>

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

#include "db/InternalIdReserver.h"

namespace hoot
{

HootApiDb::HootApiDb()
{
  _init();
}

HootApiDb::~HootApiDb()
{
  LOG_VARD(_nodesInsertElapsed);
  LOG_VARD(_wayNodesInsertElapsed);
  LOG_VARD(_wayInsertElapsed);
  close();
}

Envelope HootApiDb::calculateEnvelope() const
{
  const long mapId = _currMapId;
  Envelope result;

  // if you're having performance issues read this:
  // http://www.postgresql.org/docs/8.0/static/functions-aggregate.html
  QSqlQuery boundsQuery = _exec("SELECT MIN(latitude) as minLat, MAX(latitude) AS maxLat "
                             ", MIN(longitude) as minLon, MAX(longitude) AS maxLon"
                             " FROM " + getNodesTableName(mapId));

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
    //QString error = "Error retrieving bounds of map with ID: " + mapId;
    QString error = QString("Error calculating bounds: %1").arg(boundsQuery.lastError().text());
    LOG_WARN(error);
    throw HootException(error);
  }

  return result;
}

void HootApiDb::_checkLastMapId(long mapId)
{
  if (_lastMapId != mapId)
  {
    _flushBulkInserts();
    _resetQueries();
    _nodeIdReserver.reset();
    _wayIdReserver.reset();
    _relationIdReserver.reset();
    _lastMapId = mapId;
  }
}

void HootApiDb::close()
{
  createPendingMapIndexes();
  _flushBulkInserts();

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

void HootApiDb::endChangeset()
{
  // If we're already closed, nothing to do
  if ( _currChangesetId == -1 )
  {
    //LOG_DEBUG("Tried to end a changeset but there isn't an active changeset currently");
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

  // NOTE: do *not* alter _currChangesetId or _changesetEnvelope yet.  We haven't written data to database yet!
  //    they will be refreshed upon opening a new database, so leave them alone!
  _changesetChangeCount = 0;
}

void HootApiDb::commit()
{
  if ( _db.isOpen() == false )
  {
    throw HootException("Tried to commit a transaction on a closed database");
  }

  if ( _inTransaction == false )
  {
    throw HootException("Tried to commit but weren't in a transaction");
  }

  createPendingMapIndexes();
  _flushBulkInserts();
  _resetQueries();
  if (!_db.commit())
  {
    LOG_WARN("Error committing transaction.");
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
    _execNoPrepare(task);
  }
}

void HootApiDb::_copyTableStructure(QString from, QString to)
{
  // inserting strings in this fashion is safe b/c it is private and we closely control the table
  // names.
  QString sql = QString("CREATE TABLE %1 (LIKE %2 INCLUDING DEFAULTS INCLUDING CONSTRAINTS "
      "INCLUDING INDEXES)").arg(to).arg(from);
  QSqlQuery q(_db);

  //LOG_VARD(sql);

  if (q.exec(sql) == false)
  {
    QString error = QString("Error executing query: %1 (%2)").arg(q.lastError().text()).
        arg(sql);
    LOG_WARN(error);
    throw HootException(error);
  }
}

void HootApiDb::createPendingMapIndexes()
{
  for (int i = 0; i < _pendingMapIndexes.size(); i++)
  {
    long mapId = _pendingMapIndexes[i];

    _execNoPrepare(QString("ALTER TABLE %1 "
      "ADD CONSTRAINT current_nodes_changeset_id_fkey_%2 FOREIGN KEY (changeset_id) "
        "REFERENCES %3 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION ")
        .arg(getNodesTableName(mapId))
        .arg(getMapIdString(mapId))
        .arg(getChangesetsTableName(mapId)));

    _execNoPrepare(QString("CREATE INDEX %1_tile_idx ON %2 USING btree (tile)")
        .arg(getNodesTableName(mapId))
        .arg(getNodesTableName(mapId)));

    _execNoPrepare(QString("ALTER TABLE %1 "
      "ADD CONSTRAINT current_relations_changeset_id_fkey_%2 FOREIGN KEY (changeset_id) "
        "REFERENCES %3 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION ")
        .arg(getRelationsTableName(mapId))
        .arg(getMapIdString(mapId))
        .arg(getChangesetsTableName(mapId)));

    _execNoPrepare(QString("ALTER TABLE %1 "
      "ADD CONSTRAINT current_way_nodes_node_id_fkey_%2 FOREIGN KEY (node_id) "
        "REFERENCES %3 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION, "
      "ADD CONSTRAINT current_way_nodes_way_id_fkey_%2 FOREIGN KEY (way_id) "
        "REFERENCES %4 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION")
        .arg(getWayNodesTableName(mapId))
        .arg(getMapIdString(mapId))
        .arg(getNodesTableName(mapId))
        .arg(getWaysTableName(mapId)));

    _execNoPrepare(QString("ALTER TABLE %1 "
      "ADD CONSTRAINT current_ways_changeset_id_fkey_%2 FOREIGN KEY (changeset_id) "
        "REFERENCES %3 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION ")
        .arg(getWaysTableName(mapId))
        .arg(getMapIdString(mapId))
        .arg(getChangesetsTableName(mapId)));
  }

  _pendingMapIndexes.clear();
}

void HootApiDb::deleteMap(long mapId)
{
  // Drop related renderDB First
  dropDatabase(_getRenderDBName(mapId));

  // Drop related tables
  dropTable(getRelationMembersTableName(mapId));
  dropTable(getRelationsTableName(mapId));
  dropTable(getWayNodesTableName(mapId));
  dropTable(getWaysTableName(mapId));
  dropTable(getNodesTableName(mapId));
  dropTable(getChangesetsTableName(mapId));

  // Drop related sequences
  _execNoPrepare("DROP SEQUENCE IF EXISTS " + getNodeSequenceName(mapId) + " CASCADE");
  _execNoPrepare("DROP SEQUENCE IF EXISTS " + getWaySequenceName(mapId) + " CASCADE");
  _execNoPrepare("DROP SEQUENCE IF EXISTS " + getRelationSequenceName(mapId) + " CASCADE");

  // Delete map last
  _exec("DELETE FROM maps WHERE id=:id", (qlonglong)mapId);
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
  QString sql = QString("DROP DATABASE IF EXISTS \"%1\"").arg(databaseName);

  // TRICKY: You are not allowed to drop a database within a transaction.
  // If this is the case, we store the statement & execute it right after
  // the current trans is successfully committed.
  if (_inTransaction)
  { // Store for later
    _postTransactionStatements.push_back(sql);
  }
  else
  { // Execute now
    _execNoPrepare(sql);
  }
}

void HootApiDb::dropTable(const QString& tableName)
{
  // inserting strings in this fashion is safe b/c it is private and we closely control the table
  // names.
  QString sql = QString("DROP TABLE IF EXISTS %1").arg(tableName);
  QSqlQuery q(_db);

  if (q.exec(sql) == false)
  {
    QString error = QString("Error executing query: %1 (%2)").arg(q.lastError().text()).
        arg(sql);
    LOG_WARN(error);
    throw HootException(error);
  }
}

void HootApiDb::deleteUser(long userId)
{
  QSqlQuery maps = _exec("SELECT id FROM maps WHERE user_id=:user_id", (qlonglong)userId);

  // delete all the maps owned by this user
  while (maps.next())
  {
    long mapId = maps.value(0).toLongLong();
    deleteMap(mapId);
  }

  _exec("DELETE FROM users WHERE id=:id", (qlonglong)userId);
}

QString HootApiDb::_escapeTags(const Tags& tags) const
{
  QStringList l;
  static QChar f1('\\'), f2('"'), f3('\'');
  static QChar to('_');

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    if (it.value().isEmpty() == false && it.key().isEmpty() == false)
    {
      // this doesn't appear to be working, but I think it is implementing the spec as described here:
      // http://www.postgresql.org/docs/9.0/static/hstore.html
      // The spec described above does seem to work on the psql command line. Curious.
      QString k = QString(it.key()).replace(f1, "\\\\").replace(f2, "\\\"");
      QString v = QString(it.value()).replace(f1, "\\\\").replace(f2, "\\\"");
      k.replace("'", "''");
      v.replace("'", "''");

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
  if (_nodeBulkInsert != 0)
  {
    _nodeBulkInsert->flush();
  }
  if (_wayBulkInsert != 0)
  {
    _wayBulkInsert->flush();
  }
  if (_wayNodeBulkInsert != 0)
  {
    _wayNodeBulkInsert->flush();
  }
  if (_relationBulkInsert != 0)
  {
    _relationBulkInsert->flush();
  }
}

bool HootApiDb::isCorrectDbVersion()
{
  return getDbVersion() == ApiDb::expectedDbVersion();
}

QString HootApiDb::getDbVersion()
{
  if (_selectDbVersion == 0)
  {
    _selectDbVersion.reset(new QSqlQuery(_db));
    _selectDbVersion->prepare("SELECT id || ':' || author AS version_id FROM databasechangelog "
                             "ORDER BY dateexecuted DESC LIMIT 1");
  }

  if (_selectDbVersion->exec() == false)
  {
    throw HootException(_selectDbVersion->lastError().text());
  }

  QString result;
  if (_selectDbVersion->next())
  {
    result = _selectDbVersion->value(0).toString();
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
    _nodeIdReserver.reset(new InternalIdReserver(_db, getNodeSequenceName(mapId)));
  }

  return _nodeIdReserver->getNextId();
}

long HootApiDb::_getNextRelationId()
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);
  if (_relationIdReserver == 0)
  {
    _relationIdReserver.reset(new InternalIdReserver(_db, getRelationSequenceName(mapId)));
  }
  return _relationIdReserver->getNextId();
}

long HootApiDb::_getNextWayId()
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);
  if (_wayIdReserver == 0)
  {
    _wayIdReserver.reset(new InternalIdReserver(_db, getWaySequenceName(mapId)));
  }
  return _wayIdReserver->getNextId();
}

void HootApiDb::_init()
{
  _inTransaction = false;

  int recordsPerBulkInsert = 500;

  // set it to something obsurd.
  _lastMapId = -numeric_limits<long>::max();

  _nodesInsertElapsed = 0;
  // 500 found experimentally on my desktop -JRS
  _nodesPerBulkInsert = recordsPerBulkInsert;

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

void HootApiDb::beginChangeset()
{
  Tags emptyTags;
  beginChangeset(emptyTags);
}

void HootApiDb::beginChangeset(const Tags& tags)
{
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
      "VALUES (:user_id, NOW(), :min_lat, :max_lat, :min_lon, :max_lon, NOW(), " + _escapeTags(tags) + ") "
      "RETURNING id")
        .arg(getChangesetsTableName(mapId)));
  }
  _insertChangeSet->bindValue(":user_id", (qlonglong)userId);
  _insertChangeSet->bindValue(":min_lat", _changesetEnvelope.getMinY());
  _insertChangeSet->bindValue(":max_lat", _changesetEnvelope.getMaxY());
  _insertChangeSet->bindValue(":min_lon", _changesetEnvelope.getMinX());
  _insertChangeSet->bindValue(":max_lon", _changesetEnvelope.getMaxX());

  _currChangesetId = _insertRecord(*_insertChangeSet);

  _changesetChangeCount = 0;
  LOG_DEBUG("Started new changeset " << QString::number(_currChangesetId));
}

long HootApiDb::insertMap(QString displayName, bool publicVisibility)
{
  const int userId = _currUserId;

  if (_insertMap == 0)
  {
    _insertMap.reset(new QSqlQuery(_db));
    _insertMap->prepare("INSERT INTO maps (display_name, user_id, public, created_at) "
                        "VALUES (:display_name, :user_id, :public, NOW()) "
                        "RETURNING id");
  }
  _insertMap->bindValue(":display_name", displayName);
  _insertMap->bindValue(":user_id", userId);
  _insertMap->bindValue(":public", publicVisibility);

  long mapId = _insertRecord(*_insertMap);

  QString mapIdStr = getMapIdString(mapId);
  _copyTableStructure("changesets", getChangesetsTableName(mapId));
  _copyTableStructure("current_nodes", "current_nodes" + mapIdStr);
  _copyTableStructure("current_relation_members", "current_relation_members" + mapIdStr);
  _copyTableStructure("current_relations", "current_relations" + mapIdStr);
  _copyTableStructure("current_way_nodes", "current_way_nodes" + mapIdStr);
  _copyTableStructure("current_ways", "current_ways" + mapIdStr);

  _execNoPrepare("CREATE SEQUENCE " + getNodeSequenceName(mapId));
  _execNoPrepare("CREATE SEQUENCE " + getRelationSequenceName(mapId));
  _execNoPrepare("CREATE SEQUENCE " + getWaySequenceName(mapId));

  _execNoPrepare(QString("ALTER TABLE %1 "
    "ALTER COLUMN id SET DEFAULT NEXTVAL('%4'::regclass)")
      .arg(getNodesTableName(mapId))
      .arg(getNodeSequenceName(mapId)));

  _execNoPrepare(QString("ALTER TABLE %1 "
    "ALTER COLUMN id SET DEFAULT NEXTVAL('%4'::regclass)")
      .arg(getRelationsTableName(mapId))
      .arg(getRelationSequenceName(mapId)));

  _execNoPrepare(QString("ALTER TABLE %1 "
    "ALTER COLUMN id SET DEFAULT NEXTVAL('%4'::regclass)")
      .arg(getWaysTableName(mapId))
      .arg(getWaySequenceName(mapId)));

  // remove the index to speed up inserts. It'll be added back by createPendingMapIndexes
  _execNoPrepare(QString("DROP INDEX %1_tile_idx")
      .arg(getNodesTableName(mapId)));

  _pendingMapIndexes.append(mapId);

  return mapId;
}

bool HootApiDb::insertNode(const double lat, const double lon,
  const Tags& tags, long& assignedId)
{
  assignedId = _getNextNodeId();

  return insertNode(assignedId, lat, lon, tags);
}

bool HootApiDb::insertNode(const long id, const double lat, const double lon, const Tags &tags)
{
  const long mapId = _currMapId;
  double start = Tgs::Time::getTime();

  _checkLastMapId(mapId);

  if (_nodeBulkInsert == 0)
  {
    QStringList columns;
    columns << "id" << "latitude" << "longitude" << "changeset_id" << "timestamp" <<
               "tile" << "version" << "tags";

    _nodeBulkInsert.reset(new SqlBulkInsert(_db, getNodesTableName(mapId), columns));
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

  //LOG_DEBUG("Inserted node with ID: " << QString::number(id));

  return true;
}

bool HootApiDb::insertRelation(const Tags &tags, long& assignedId)
{
  assignedId = _getNextRelationId();

  return insertRelation(assignedId, tags);
}

bool HootApiDb::insertRelation(const long relationId, const Tags &tags)
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);

  if (_relationBulkInsert == 0)
  {
    QStringList columns;
    columns << "id" << "changeset_id" << "timestamp" << "version" << "tags";

    _relationBulkInsert.reset(new SqlBulkInsert(_db, getRelationsTableName(mapId), columns));
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

  LOG_DEBUG("Inserted relation with ID: " << QString::number(relationId));

  return true;
}

bool HootApiDb::insertRelationMember(const long relationId, const ElementType& type,
  const long elementId, const QString& role, const int sequenceId)

{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);

  if (_insertRelationMembers == 0)
  {
    _insertRelationMembers.reset(new QSqlQuery(_db));
    _insertRelationMembers->prepare(
      "INSERT INTO " + getRelationMembersTableName(mapId) +
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
    LOG_WARN(err)
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

bool HootApiDb::isSupported(QUrl url)
{
  bool valid = ApiDb::isSupported(url);

  if (url.scheme() != "hootapidb" && url.scheme() != "postgresql") //postgresql is deprecated but still support
  {
    valid = false;
  }

  if (valid)
  {
    QString path = url.path();
    QStringList plist = path.split("/");

    if ( plist.size() == 3 )
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
    else if ( (plist.size() == 4) && ((plist[1] == "") || (plist[2 ] == "") || (plist[3] == "")) )
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
  if (!isSupported(url))
  {
    throw HootException("An unsupported URL was passed into HootApiDb: " + url.toString());
  }

  _resetQueries();

  ApiDb::open(url);

  if (isCorrectDbVersion() == false)
  {
    LOG_WARN("Running against an unexpected DB version.");
    LOG_WARN("Expected: " << expectedDbVersion());
    LOG_WARN("Actual: " << getDbVersion());
  }
}

void HootApiDb::_resetQueries()
{
  _closeChangeSet.reset();
  _insertChangeSet.reset();
  _insertChangeSetTag.reset();
  _insertMap.reset();
  _insertWayNodes.reset();
  _insertRelationMembers.reset();
  _selectDbVersion.reset();
  _selectUserByEmail.reset();
  _insertUser.reset();
  _mapExists.reset();
  _changesetExists.reset();
  _numTypeElementsForMap.reset();
  _selectElementsForMap.reset();
  _selectReserveNodeIds.reset();
  _selectNodeIdsForWay.reset();
  _selectMapIds.reset();
  _selectMembersForRelation.reset();
  _updateNode.reset();
  _updateRelation.reset();
  _updateWay.reset();

  // bulk insert objects.
  _nodeBulkInsert.reset();
  _nodeIdReserver.reset();
  _relationBulkInsert.reset();
  _relationIdReserver.reset();
  _wayNodeBulkInsert.reset();
  _wayBulkInsert.reset();
  _wayIdReserver.reset();
}

void HootApiDb::rollback()
{
  _resetQueries();

  if (!_db.rollback())
  {
    LOG_WARN("Error rolling back transaction.");
    throw HootException("Error rolling back transaction: " + _db.lastError().text());
  }

  _inTransaction = false;
}

set<long> HootApiDb::selectMapIds(QString name)
{
  const long userId = _currUserId;
  LOG_DEBUG("selectMapIds name = "+name);
  LOG_DEBUG("userId = "+QString::number(userId));
  if (_selectMapIds == 0)
  {
      LOG_DEBUG("inside first test inside selectMapIds");
    _selectMapIds.reset(new QSqlQuery(_db));
    _selectMapIds->prepare("SELECT id FROM maps WHERE display_name LIKE :name AND user_id=:userId");
  }

  _selectMapIds->bindValue(":name", name);
  _selectMapIds->bindValue(":user_id", (qlonglong)userId);

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

void HootApiDb::transaction()
{
  // Queries must be created from within the current transaction.
  _resetQueries();
  if (!_db.transaction())
  {
    throw HootException(_db.lastError().text());
  }
  _inTransaction = true;
}

//using some kind of SQL generator (if one exists for C++ would prevent us from having to do
//hardcoded table and column references and keep this code less brittle...

QString HootApiDb::_elementTypeToElementTableName(long mapId, const ElementType& elementType) const
{
  if (elementType == ElementType::Node)
  {
    return getNodesTableName(mapId);
  }
  else if (elementType == ElementType::Way)
  {
    return getWaysTableName(mapId);
  }
  else if (elementType == ElementType::Relation)
  {
    return getRelationsTableName(mapId);
  }
  else
  {
    throw HootException("Unsupported element type.");
  }
}

bool HootApiDb::mapExists(const long id)
{
  if (_mapExists == 0)
  {
    _mapExists.reset(new QSqlQuery(_db));
    _mapExists->prepare("SELECT display_name FROM maps WHERE id = :mapId");
  }
  _mapExists->bindValue(":mapId", (qlonglong)id);
  if (_mapExists->exec() == false)
  {
    throw HootException(_mapExists->lastError().text());
  }

  return _mapExists->next();
}

bool HootApiDb::changesetExists(const long id)
{
  const long mapId = _currMapId;

  _checkLastMapId(mapId);
  if (_changesetExists == 0)
  {
    _changesetExists.reset(new QSqlQuery(_db));
    _changesetExists->prepare(QString("SELECT num_changes FROM %1 WHERE id = :changesetId")
      .arg(getChangesetsTableName(mapId)));
  }
  _changesetExists->bindValue(":changesetId", (qlonglong)id);
  if (_changesetExists->exec() == false)
  {
    throw HootException(_changesetExists->lastError().text());
  }

  return _changesetExists->next();
}

long HootApiDb::numElements(const ElementType& elementType)
{
  const long mapId = _currMapId;

  _numTypeElementsForMap.reset(new QSqlQuery(_db));
  _numTypeElementsForMap->prepare(
    "SELECT COUNT(*) FROM " + _elementTypeToElementTableName(mapId, elementType));
  if (_numTypeElementsForMap->exec() == false)
  {
    LOG_ERROR(_numTypeElementsForMap->executedQuery());
    LOG_ERROR(_numTypeElementsForMap->lastError().text());
    throw HootException(_numTypeElementsForMap->lastError().text());
  }

  long result = -1;
  if (_numTypeElementsForMap->next())
  {
    bool ok;
    result = _numTypeElementsForMap->value(0).toLongLong(&ok);
    if (!ok)
    {
      throw HootException("Count not retrieve count for element type: " + elementType.toString());
    }
  }
  _numTypeElementsForMap->finish();
  return result;
}

shared_ptr<QSqlQuery> HootApiDb::selectElements(const ElementType& elementType)
{
  const long mapId = _currMapId;
  _selectElementsForMap.reset(new QSqlQuery(_db));
  _selectElementsForMap->setForwardOnly(true);

  QString sql =  "SELECT * FROM " + _elementTypeToElementTableName(mapId, elementType);
  LOG_DEBUG(QString("SERVICES: Result sql query= "+sql));

  _selectElementsForMap->prepare(sql);
  _selectElementsForMap->bindValue(":mapId", (qlonglong)mapId);

  if (_selectElementsForMap->exec() == false)
  {
    QString err = _selectElementsForMap->lastError().text();
    LOG_WARN(sql);
    throw HootException("Error selecting elements of type: " + elementType.toString() +
      " for map ID: " + QString::number(mapId) + " Error: " + err);
  }
  return _selectElementsForMap;
}

vector<long> HootApiDb::selectNodeIdsForWay(long wayId)
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);
  QString sql = "SELECT node_id FROM " + getWayNodesTableName(mapId) +
      " WHERE way_id = :wayId ORDER BY sequence_id";

  return ApiDb::selectNodeIdsForWay(wayId, sql);
}

shared_ptr<QSqlQuery> HootApiDb::selectNodesForWay(long wayId)
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);
  QString sql = "SELECT node_id FROM " + getWayNodesTableName(mapId) +
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
      "SELECT member_type, member_id, member_role FROM " + getRelationMembersTableName(mapId) +
      " WHERE relation_id = :relationId ORDER BY sequence_id");
    _selectMembersForRelation->bindValue(":mapId", (qlonglong)mapId);
  }

  _selectMembersForRelation->bindValue(":relationId", (qlonglong)relationId);
  if (_selectMembersForRelation->exec() == false)
  {
    throw HootException("Error selecting members for relation with ID: " +
      QString::number(relationId) + " Error: " + _selectMembersForRelation->lastError().text());
  }

  while (_selectMembersForRelation->next())
  {
    const QString memberType = _selectMembersForRelation->value(0).toString();
    if (ElementType::isValidTypeString(memberType))
    {
      result.push_back(
        RelationData::Entry(
          _selectMembersForRelation->value(2).toString(),
          ElementId(ElementType::fromString(memberType),
          _selectMembersForRelation->value(1).toLongLong())));
    }
    else
    {
        LOG_WARN("Invalid relation member type: " + memberType + ".  Skipping relation member.");
    }
  }

  return result;
}

void HootApiDb::updateNode(const long id, const double lat, const double lon, const long version,
                            const Tags& tags)
{
  const long mapId = _currMapId;
  _flushBulkInserts();

  _checkLastMapId(mapId);

  if (_updateNode == 0)
  {
    _updateNode.reset(new QSqlQuery(_db));
    _updateNode->prepare(
      "UPDATE " + getNodesTableName(mapId) +
      " SET latitude=:latitude, longitude=:longitude, changeset_id=:changeset_id, "
      " timestamp=:timestamp, tile=:tile, version=:version, tags=" + _escapeTags(tags) + " WHERE id=:id");
  }

  _updateNode->bindValue(":id", (qlonglong)id);
  //_updateNode->bindValue(":latitude", (qlonglong)_round(lat * COORDINATE_SCALE, 7));
  _updateNode->bindValue(":latitude", lat);
  //_updateNode->bindValue(":longitude", (qlonglong)_round(lon * COORDINATE_SCALE, 7));
  _updateNode->bindValue(":longitude", lon);
  _updateNode->bindValue(":changeset_id", (qlonglong)_currChangesetId);
  _updateNode->bindValue(":timestamp", OsmUtils::currentTimeAsString());
  _updateNode->bindValue(":tile", (qlonglong)tileForPoint(lat, lon));
  _updateNode->bindValue(":version", (qlonglong)version);

  if (_updateNode->exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_updateNode->executedQuery()).
        arg(_updateNode->lastError().text());
    LOG_WARN(err)
    throw HootException(err);
  }

  _updateNode->finish();

  LOG_DEBUG("Updated node with ID: " << QString::number(id));
}

void HootApiDb::updateRelation(const long id, const long version, const Tags& tags)
{
  const long mapId = _currMapId;
  _flushBulkInserts();
  _checkLastMapId(mapId);

  if (_updateRelation == 0)
  {
    _updateRelation.reset(new QSqlQuery(_db));
    _updateRelation->prepare(
      "UPDATE " + getRelationsTableName(mapId) +
      " SET changeset_id=:changeset_id, timestamp=:timestamp, version=:version, tags=" + _escapeTags(tags) + " WHERE id=:id");
  }

  _updateRelation->bindValue(":id", (qlonglong)id);
  _updateRelation->bindValue(":changeset_id", (qlonglong)_currChangesetId);
  _updateRelation->bindValue(":timestamp", OsmUtils::currentTimeAsString());
  _updateRelation->bindValue(":version", (qlonglong)version);

  if (_updateRelation->exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_updateWay->executedQuery()).
        arg(_updateRelation->lastError().text());
    LOG_WARN(err)
    throw HootException(err);
  }

  _updateRelation->finish();

  LOG_DEBUG("Updated relation with ID: " << QString::number(id));
}

void HootApiDb::updateWay(const long id, const long version, const Tags& tags)
{
  const long mapId = _currMapId;
  _flushBulkInserts();
  _checkLastMapId(mapId);

  if (_updateWay == 0)
  {
    _updateWay.reset(new QSqlQuery(_db));
    _updateWay->prepare(
      "UPDATE " + getWaysTableName(mapId) +
      " SET changeset_id=:changeset_id, timestamp=:timestamp, version=:version, tags=" + _escapeTags(tags) + " WHERE id=:id");
  }

  _updateWay->bindValue(":id", (qlonglong)id);
  _updateWay->bindValue(":changeset_id", (qlonglong)_currChangesetId);
  _updateWay->bindValue(":timestamp", OsmUtils::currentTimeAsString());
  _updateWay->bindValue(":version", (qlonglong)version);

  if (_updateWay->exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_updateWay->executedQuery()).
        arg(_updateWay->lastError().text());
    LOG_WARN(err)
    throw HootException(err);
  }

  _updateWay->finish();

  LOG_DEBUG("Updated way with ID: " << QString::number(id));
}

bool HootApiDb::insertWay(const Tags &tags, long &assignedId)
{
  assignedId = _getNextWayId();

  return insertWay(assignedId, tags);
}

bool HootApiDb::insertWay(const long wayId, const Tags &tags)
{
  const long mapId = _currMapId;

  double start = Tgs::Time::getTime();

  _checkLastMapId(mapId);

  if (_wayBulkInsert == 0)
  {
    QStringList columns;
    columns << "id" << "changeset_id" << "timestamp" << "version" << "tags";

    _wayBulkInsert.reset(new SqlBulkInsert(_db, getWaysTableName(mapId), columns));
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

  LOG_DEBUG("Inserted way with ID: " << QString::number(wayId));

  return true;
}

void HootApiDb::insertWayNodes(long wayId, const vector<long>& nodeIds)
{
  const long mapId = _currMapId;
  double start = Tgs::Time::getTime();

  //LOG_DEBUG("Inserting nodes into way " << QString::number(wayId));

  _checkLastMapId(mapId);

  if (_wayNodeBulkInsert == 0)
  {
    QStringList columns;
    columns << "way_id" << "node_id" << "sequence_id";

    _wayNodeBulkInsert.reset(new SqlBulkInsert(_db, getWayNodesTableName(mapId), columns));
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
  //LOG_DEBUG("Changeset bounding box updated to include X=" + QString::number(nodeX) + ", Y=" + QString::number(nodeY));
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
  QString dbName = "";
  QString mapDisplayName = "";
  QString mapIdNumber = QString::number(mapId);
  QString sql = "SELECT current_database(), maps.display_name "
                "FROM maps "
                "WHERE maps.id=" + mapIdNumber;
  QSqlQuery q = _exec(sql);

  if (q.next())
  {
    dbName = q.value(0).toString();
    mapDisplayName = q.value(1).toString();
  }

  return (dbName + "_renderdb_" + mapIdNumber);
}

}
