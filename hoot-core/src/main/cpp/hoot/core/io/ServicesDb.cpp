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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ServicesDb.h"

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/db/SqlBulkInsert.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/ElementCacheLRU.h>

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

const Status ServicesDb::DEFAULT_ELEMENT_STATUS(Status::Invalid);

ServicesDb::ServicesDb()
{
  _init();
}

ServicesDb::~ServicesDb()
{
  LOG_VARD(_nodesInsertElapsed);
  LOG_VARD(_wayNodesInsertElapsed);
  LOG_VARD(_wayInsertElapsed);
  close();
}

Envelope ServicesDb::calculateEnvelope() const
{
  const long mapId = _currMapId;
  Envelope result;

  // if you're having performance issues read this:
  // http://www.postgresql.org/docs/8.0/static/functions-aggregate.html
  QSqlQuery boundsQuery = _exec("SELECT MIN(latitude) as minLat, MAX(latitude) AS maxLat "
                             ", MIN(longitude) as minLon, MAX(longitude) AS maxLon"
                             " FROM " + _getNodesTableName(mapId));

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

void ServicesDb::_checkLastMapId(long mapId)
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

void ServicesDb::close()
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

  _connectionType = DBTYPE_UNSUPPORTED;
}

void ServicesDb::endChangeset()
{
  // If we're already closed, nothing to do
  if ( _currChangesetId == -1 )
  {
    //LOG_DEBUG("Tried to end a changeset but there isn't an active changeset currently");
    return;
  }

  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    _endChangeset_Services(_currChangesetId, _changesetEnvelope, _changesetChangeCount);
    break;

  default:
    _endChangeset_OsmApi();
    break;
  }

  LOG_DEBUG("Successfully closed changeset " << QString::number(_currChangesetId));


  // NOTE: do *not* alter _currChangesetId or _changesetEnvelope yet.  We haven't written data to database yet!
  //    they will be refreshed upon opening a new database, so leave them alone!
  _changesetChangeCount = 0;
}

void ServicesDb::_endChangeset_Services(long changeSetId, Envelope env, int numChanges)
{
  const long mapId = _currMapId;

  if (!changesetExists(changeSetId))
  {
    throw HootException("No changeset exists with ID: " + changeSetId);
  }

  _checkLastMapId(mapId);
  if (_closeChangeSet == 0)
  {
    _closeChangeSet.reset(new QSqlQuery(_db));
    _closeChangeSet->prepare(
      QString("UPDATE %1 SET min_lat=:min_lat, max_lat=:max_lat, min_lon=:min_lon, "
        "max_lon=:max_lon, closed_at=NOW(), num_changes=:num_changes WHERE id=:id")
         .arg(_getChangesetsTableName(mapId)));
  }
  _closeChangeSet->bindValue(":min_lat", env.getMinY());
  _closeChangeSet->bindValue(":max_lat", env.getMaxY());
  _closeChangeSet->bindValue(":min_lon", env.getMinX());
  _closeChangeSet->bindValue(":max_lon", env.getMaxX());
  _closeChangeSet->bindValue(":num_changes", numChanges);
  _closeChangeSet->bindValue(":id", (qlonglong)changeSetId);

  if (_closeChangeSet->exec() == false)
  {
    LOG_ERROR("query bound values: ");
    LOG_ERROR(_closeChangeSet->boundValues());
    LOG_ERROR("\n");
    throw HootException("Error executing close changeset: " + _closeChangeSet->lastError().text() +
                        " (SQL: " + _closeChangeSet->executedQuery() + ")" + " with envelope: " +
                        QString::fromStdString(env.toString()));
  }
}

void ServicesDb::commit()
{
  if ( _db.isOpen() == false )
  {
    throw HootException("Tried to commit a transaction on a closed database");
  }

  if ( _inTransaction == false )
  {
    throw HootException("Tried to commit but weren't in a transaction");
  }


  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    createPendingMapIndexes();
    _flushBulkInserts();
    _resetQueries();
    if (!_db.commit())
    {
      LOG_WARN("Error committing transaction.");
      throw HootException("Error committing transaction: " + _db.lastError().text());
    }
    break;
  case DBTYPE_OSMAPI:
    LOG_DEBUG("Starting commit for OSM API");

    // If there are elements in the OSM cache, flush them
    _flushElementCacheToDb();

    _resetQueries();

    if ( _db.commit() == false )
    {
      LOG_WARN("Error committing transaction");
      throw HootException("Error committing transaction");
    }
    LOG_DEBUG("Commit was successful");

    break;

  default:
    throw HootException("Commit for unsupported database type");
    break;
  }
  _inTransaction = false;
}

void ServicesDb::_copyTableStructure(QString from, QString to)
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

void ServicesDb::createPendingMapIndexes()
{
  for (int i = 0; i < _pendingMapIndexes.size(); i++)
  {
    long mapId = _pendingMapIndexes[i];

    _execNoPrepare(QString("ALTER TABLE %1 "
      "ADD CONSTRAINT current_nodes_changeset_id_fkey_%2 FOREIGN KEY (changeset_id) "
        "REFERENCES %3 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION ")
        .arg(_getNodesTableName(mapId))
        .arg(_getMapIdString(mapId))
        .arg(_getChangesetsTableName(mapId)));

    _execNoPrepare(QString("CREATE INDEX %1_tile_idx ON %2 USING btree (tile)")
        .arg(_getNodesTableName(mapId))
        .arg(_getNodesTableName(mapId)));

    _execNoPrepare(QString("ALTER TABLE %1 "
      "ADD CONSTRAINT current_relations_changeset_id_fkey_%2 FOREIGN KEY (changeset_id) "
        "REFERENCES %3 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION ")
        .arg(_getRelationsTableName(mapId))
        .arg(_getMapIdString(mapId))
        .arg(_getChangesetsTableName(mapId)));

    _execNoPrepare(QString("ALTER TABLE %1 "
      "ADD CONSTRAINT current_way_nodes_node_id_fkey_%2 FOREIGN KEY (node_id) "
        "REFERENCES %3 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION, "
      "ADD CONSTRAINT current_way_nodes_way_id_fkey_%2 FOREIGN KEY (way_id) "
        "REFERENCES %4 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION")
        .arg(_getWayNodesTableName(mapId))
        .arg(_getMapIdString(mapId))
        .arg(_getNodesTableName(mapId))
        .arg(_getWaysTableName(mapId)));

    _execNoPrepare(QString("ALTER TABLE %1 "
      "ADD CONSTRAINT current_ways_changeset_id_fkey_%2 FOREIGN KEY (changeset_id) "
        "REFERENCES %3 (id) MATCH SIMPLE "
        "ON UPDATE NO ACTION ON DELETE NO ACTION ")
        .arg(_getWaysTableName(mapId))
        .arg(_getMapIdString(mapId))
        .arg(_getChangesetsTableName(mapId)));
  }

  _pendingMapIndexes.clear();
}

void ServicesDb::deleteMap(long mapId)
{
  _dropTable(_getRelationMembersTableName(mapId));
  _dropTable(_getRelationsTableName(mapId));
  _dropTable(_getWayNodesTableName(mapId));
  _dropTable(_getWaysTableName(mapId));
  _dropTable(_getNodesTableName(mapId));
  _dropTable(_getChangesetsTableName(mapId));

  _execNoPrepare("DROP SEQUENCE IF EXISTS " + _getNodeSequenceName(mapId) + " CASCADE");
  _execNoPrepare("DROP SEQUENCE IF EXISTS " + _getWaySequenceName(mapId) + " CASCADE");
  _execNoPrepare("DROP SEQUENCE IF EXISTS " + _getRelationSequenceName(mapId) + " CASCADE");

  _exec("DELETE FROM maps WHERE id=:id", (qlonglong)mapId);
}

void ServicesDb::_dropTable(QString tableName)
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

void ServicesDb::deleteUser(long userId)
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

QSqlQuery ServicesDb::_exec(QString sql, QVariant v1, QVariant v2, QVariant v3) const
{
  QSqlQuery q(_db);

 // LOG_VARD(sql);

  if (q.prepare(sql) == false)
  {
    throw HootException(QString("Error preparing query: %1 (%2)").arg(q.lastError().text()).
                        arg(sql));
  }

  if (v1.isValid())
  {
    q.bindValue(0, v1);
  }
  if (v2.isValid())
  {
    q.bindValue(1, v2);
  }
  if (v3.isValid())
  {
    q.bindValue(2, v3);
  }

  if (q.exec() == false)
  {
    throw HootException(QString("Error executing query: %1 (%2)").arg(q.lastError().text()).
                        arg(sql));
  }

  return q;
}

QSqlQuery ServicesDb::_execNoPrepare(QString sql) const
{
  // inserting strings in this fashion is safe b/c it is private and we closely control the table
  // names.
  QSqlQuery q(_db);

  //LOG_VARD(sql);

  if (q.exec(sql) == false)
  {
    QString error = QString("Error executing query: %1 (%2)").arg(q.lastError().text()).
        arg(sql);
    LOG_WARN(error);
    throw HootException(error);
  }

  return q;
}

QString ServicesDb::_escapeIds(const vector<long>& v) const
{
  QString str;
  str.reserve(v.size() * 6);
  QString comma(",");

  str.append("{");
  for (size_t i = 0; i < v.size(); i++)
  {
    if (i != 0)
    {
      str.append(comma);
    }
    str.append(QString::number(v[i]));
  }
  str.append("}");

  return str;
}

QString ServicesDb::_escapeTags(const Tags& tags) const
{
  QStringList l;
  static QChar f1('\\'), f2('"'), f3('\'');
  static QChar to('_');

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    // this doesn't appear to be working, but I think it is implementing the spec as described here:
    // http://www.postgresql.org/docs/9.0/static/hstore.html
    // The spec described above does seem to work on the psql command line. Curious.
    QString k = QString(it.key()).replace(f1, "\\\\").replace(f2, "\\\"");
    QString v = QString(it.value()).replace(f1, "\\\\").replace(f2, "\\\"");

    l << QString("\"%1\"=>\"%2\"").arg(k).arg(v);
  }
  return l.join(",");
}

QString ServicesDb::_execToString(QString sql, QVariant v1, QVariant v2, QVariant v3)
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

void ServicesDb::_flushBulkInserts()
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

QString ServicesDb::getDbVersion()
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

long ServicesDb::_getNextNodeId()
{
  long retVal = -1;
  switch ( _connectionType)
  {
  case DBTYPE_SERVICES:
    retVal = _getNextNodeId_Services(_currMapId);
    break;
  case DBTYPE_OSMAPI:
    retVal = _getNextNodeId_OsmApi();
    break;
  default:
    throw HootException("Get Next Node ID for unsupported database type");
    break;
  }

  return retVal;
}

long ServicesDb::_getNextNodeId_Services(long mapId)
{
  _checkLastMapId(mapId);
  if (_nodeIdReserver == 0)
  {
    _nodeIdReserver.reset(new InternalIdReserver(_db, _getNodeSequenceName(mapId)));
  }

  return _nodeIdReserver->getNextId();
}

long ServicesDb::_getNextNodeId_OsmApi()
{
  if (_osmApiNodeIdReserver == 0)
  {
    _osmApiNodeIdReserver.reset(new SequenceIdReserver(_db, "current_nodes_id_seq"));
  }

  return _osmApiNodeIdReserver->getNextId();
}

long ServicesDb::_getNextRelationId(long mapId)
{
  _checkLastMapId(mapId);
  if (_relationIdReserver == 0)
  {
    _relationIdReserver.reset(new InternalIdReserver(_db, _getRelationSequenceName(mapId)));
  }
  return _relationIdReserver->getNextId();
}

long ServicesDb::_getNextWayId()
{
  long retVal = -1;
  switch ( _connectionType)
  {
  case DBTYPE_SERVICES:
    retVal = _getNextWayId_Services(_currMapId);
    break;
  case DBTYPE_OSMAPI:
    retVal = _getNextWayId_OsmApi();
    break;
  default:
    throw HootException("Get Next Node ID for unsupported database type");
    break;
  }

  return retVal;

}

long ServicesDb::_getNextWayId_Services(const long mapId)
{
  _checkLastMapId(mapId);
  if (_wayIdReserver == 0)
  {
    _wayIdReserver.reset(new InternalIdReserver(_db, _getWaySequenceName(mapId)));
  }
  return _wayIdReserver->getNextId();
}

bool ServicesDb::_hasTable(QString tableName)
{
  QString sql = "SELECT 1 from pg_catalog.pg_class c JOIN pg_catalog.pg_namespace n ON "
      "n.oid = c.relnamespace WHERE c.relname = :name";
  QSqlQuery q = _exec(sql, tableName);

  return q.next();
}

void ServicesDb::_init()
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
  _connectionType = DBTYPE_UNSUPPORTED;
  _currChangesetId = -1;
  _changesetEnvelope.init();
  _changesetChangeCount = 0;

  // Value selected due to code comment: "500 found experimentally on my desktop [-initials]"
  _elementCacheCapacity = 500;
  _elementCache.reset(new ElementCacheLRU(_elementCacheCapacity));
}

void ServicesDb::beginChangeset()
{
  Tags emptyTags;
  beginChangeset(emptyTags);
}

void ServicesDb::beginChangeset(const Tags& tags)
{
  _changesetEnvelope.init();
  _changesetChangeCount = 0;

  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    _beginChangeset_Services(tags);
    break;

  case DBTYPE_OSMAPI:
    _beginChangeset_OsmApi();
    break;

  default:
    throw HootException("Begin changeset called for unsupported database type");

    break;
  }

  _changesetChangeCount = 0;

  LOG_DEBUG("Started new changeset " << QString::number(_currChangesetId));
}

void ServicesDb::_beginChangeset_Services(const Tags& tags)
{
  const long mapId = _currMapId;
  const long userId = _currUserId;

  _checkLastMapId(mapId);
  if (_insertChangeSet == 0)
  {
    _insertChangeSet.reset(new QSqlQuery(_db));
    _insertChangeSet->prepare(
      QString("INSERT INTO %1 (user_id, created_at, min_lat, max_lat, min_lon, max_lon, "
          "closed_at, tags) "
      "VALUES (:user_id, NOW(), :min_lat, :max_lat, :min_lon, :max_lon, NOW(), :tags) "
      "RETURNING id")
        .arg(_getChangesetsTableName(mapId)));
  }
  _insertChangeSet->bindValue(":user_id", (qlonglong)userId);
  _insertChangeSet->bindValue(":min_lat", _changesetEnvelope.getMinY());
  _insertChangeSet->bindValue(":max_lat", _changesetEnvelope.getMaxY());
  _insertChangeSet->bindValue(":min_lon", _changesetEnvelope.getMinX());
  _insertChangeSet->bindValue(":max_lon", _changesetEnvelope.getMaxX());
  _insertChangeSet->bindValue(":tags", _escapeTags(tags));

  _currChangesetId = _insertRecord(*_insertChangeSet);
}

long ServicesDb::insertMap(QString displayName, bool publicVisibility)
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

  QString mapIdStr = _getMapIdString(mapId);
  _copyTableStructure("changesets", _getChangesetsTableName(mapId));
  _copyTableStructure("current_nodes", "current_nodes" + mapIdStr);
  _copyTableStructure("current_relation_members", "current_relation_members" + mapIdStr);
  _copyTableStructure("current_relations", "current_relations" + mapIdStr);
  _copyTableStructure("current_way_nodes", "current_way_nodes" + mapIdStr);
  _copyTableStructure("current_ways", "current_ways" + mapIdStr);

  _execNoPrepare("CREATE SEQUENCE " + _getNodeSequenceName(mapId));
  _execNoPrepare("CREATE SEQUENCE " + _getRelationSequenceName(mapId));
  _execNoPrepare("CREATE SEQUENCE " + _getWaySequenceName(mapId));

  _execNoPrepare(QString("ALTER TABLE %1 "
    "ALTER COLUMN id SET DEFAULT NEXTVAL('%4'::regclass)")
      .arg(_getNodesTableName(mapId))
      .arg(_getNodeSequenceName(mapId)));

  _execNoPrepare(QString("ALTER TABLE %1 "
    "ALTER COLUMN id SET DEFAULT NEXTVAL('%4'::regclass)")
      .arg(_getRelationsTableName(mapId))
      .arg(_getRelationSequenceName(mapId)));

  _execNoPrepare(QString("ALTER TABLE %1 "
    "ALTER COLUMN id SET DEFAULT NEXTVAL('%4'::regclass)")
      .arg(_getWaysTableName(mapId))
      .arg(_getWaySequenceName(mapId)));

  // remove the index to speed up inserts. It'll be added back by createPendingMapIndexes
  _execNoPrepare(QString("DROP INDEX %1_tile_idx")
      .arg(_getNodesTableName(mapId)));

  _pendingMapIndexes.append(mapId);

  return mapId;
}

bool ServicesDb::insertNode(const double lat, const double lon,
  const Tags& tags, long& assignedId)
{
  assignedId = _getNextNodeId();

  LOG_DEBUG("Got ID " << QString::number(assignedId) << " for new node");

  return insertNode(assignedId, lat, lon, tags);
}

bool ServicesDb::insertNode(const long id, const double lat, const double lon, const Tags &tags)
{
  bool retVal = false;

  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    _insertNode_Services(id, lat, lon, tags);
    retVal = true;
    break;

  case DBTYPE_OSMAPI:
    _insertNode_OsmApi(id, lat, lon, tags);
    retVal = true;
    break;
  default:
    throw HootException("Insert node on unsupported database type");
    break;
  }

  if ( retVal == true )
  {
    ConstNodePtr envelopeNode(new Node(Status::Unknown1, id, lon, lat, 0.0));
    _updateChangesetEnvelope(envelopeNode);

    LOG_DEBUG("Inserted node " << QString::number(id));
  }

  return retVal;
}


void ServicesDb::_insertNode_Services(const long id, const double lat, const double lon,
  const Tags& tags)
{
  const long mapId = _currMapId;
  double start = Tgs::Time::getTime();

  _checkLastMapId(mapId);

  if (_nodeBulkInsert == 0)
  {
    QStringList columns;
    columns << "id" << "latitude" << "longitude" << "changeset_id" <<
               "tile" << "tags";

    _nodeBulkInsert.reset(new SqlBulkInsert(_db, _getNodesTableName(mapId), columns));
  }

  QList<QVariant> v;
  v.append((qlonglong)id);
  v.append((qlonglong)_round(lat * COORDINATE_SCALE, 7));
  v.append((qlonglong)_round(lon * COORDINATE_SCALE, 7));
  v.append((qlonglong)_currChangesetId);
  v.append(_tileForPoint(lat, lon));
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
}

bool ServicesDb::insertRelation(const Tags &tags, long& assignedId)
{
  assignedId = _getNextRelationId(_currMapId);

  return insertRelation(assignedId, tags);
}

bool ServicesDb::insertRelation(const long relationId, const Tags &tags)
{
  bool retVal = false;

  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    _insertRelation_Services(relationId, _currChangesetId, tags);
    retVal = true;
    break;
  default:
    throw HootException("insertRelation on unsupported database type");
    break;
  }

  if ( retVal == true )
  {
    LOG_DEBUG("Inserted relation " << QString::number(relationId));
  }
  return retVal;
}

void ServicesDb::insertRelationMembers(long relationId, ElementType type,
  long elementId, QString role, int sequenceId)
{
  const long mapId = _currMapId;

  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    _checkLastMapId(mapId);

    if (_insertRelationMembers == 0)
    {
      _insertRelationMembers.reset(new QSqlQuery(_db));
      _insertRelationMembers->prepare(
        "INSERT INTO " + _getRelationMembersTableName(mapId) +
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

    break;
  default:
    throw HootException("InsertRelationMembers called on unsupported database type");
    break;
  }

  LOG_DEBUG("Members added to relation " << QString::number(relationId));
}

long ServicesDb::insertUser(QString email, QString displayName)
{
  long retVal = -1;

  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    retVal = _insertUser_Services(email, displayName);
    break;
  case DBTYPE_OSMAPI:
    retVal = _insertUser_OsmApi(email);
    break;
  default:
    throw HootException("insertUser called with unsupported database");
    break;
  }

  return retVal;
}

long ServicesDb::_insertUser_Services(QString email, QString displayName)
{
  long id = -1;

  LOG_DEBUG("Inside insert user");
  if (_insertUser == 0)
  {
    _insertUser.reset(new QSqlQuery(_db));
    _insertUser->prepare("INSERT INTO users (email, display_name) "
                        "VALUES (:email, :display_name) "
                        "RETURNING id");
  }
  _insertUser->bindValue(":email", email);
  _insertUser->bindValue(":display_name", displayName);

  // if we failed to execute the query the first time
  if (_insertUser->exec() == false)
  {
    // it may be that another process beat us to it and the user was already inserted. This can
    // happen if a bunch of converts are run in parallel. See #3588
    id = getUserId(email, false);

    // nope, there is something else wrong. Report an error.
    if (id == -1)
    {
      QString err = QString("Error executing query: %1 (%2)").arg(_insertUser->executedQuery()).
          arg(_insertUser->lastError().text());
      LOG_WARN(err)
      throw HootException(err);
    }
    else
    {
      LOG_DEBUG("Did not insert user, queryied a previously created user.")
    }
  }
  // if the insert succeeded
  else
  {
    bool ok = false;
    if (_insertUser->next())
    {
      id = _insertUser->value(0).toLongLong(&ok);
    }

    if (!ok || id == -1)
    {
      LOG_ERROR("query bound values: ");
      LOG_ERROR(_insertUser->boundValues());
      LOG_ERROR("\n");
      throw HootException("Error retrieving new ID " + _insertUser->lastError().text() + " Query: " +
        _insertUser->executedQuery());
    }

    _insertUser->finish();
  }

  return id;
}

long ServicesDb::_insertUser_OsmApi(const QString& email)
{
  long id = -1;
  QSqlQuery insertUserStmt(_db);

  insertUserStmt.prepare(
    "INSERT INTO users (email, pass_crypt, creation_time) VALUES (:email, 'abcdefg', now()) RETURNING id;");
  insertUserStmt.bindValue(":email", email);

  // if we failed to execute the query the first time
  if (insertUserStmt.exec() == false)
  {
    // it may be that another process beat us to it and the user was already inserted. This can
    // happen if a bunch of converts are run in parallel. See #3588
    id = getUserId(email, false);

    // nope, there is something else wrong. Report an error.
    if (id == -1)
    {
      QString err = QString("Error executing query: %1 (%2)").arg(insertUserStmt.executedQuery()).
          arg(insertUserStmt.lastError().text());
      LOG_WARN(err)
      throw HootException(err);
    }
    else
    {
      LOG_DEBUG("Did not insert user, queried a previously created user.")
    }
  }
  // if the insert succeeded
  else
  {
    bool ok = false;
    if (insertUserStmt.next())
    {
      id = insertUserStmt.value(0).toLongLong(&ok);
    }

    if (!ok || id == -1)
    {
      LOG_ERROR("query bound values: ");
      LOG_ERROR(insertUserStmt.boundValues());
      LOG_ERROR("\n");
      throw HootException("Error retrieving new ID " + insertUserStmt.lastError().text() + " Query: " +
        insertUserStmt.executedQuery());
    }
  }

  return id;
}

long ServicesDb::getOrCreateUser(QString email, QString displayName)
{
  long result = getUserId(email, false);

  if (result == -1)
  {
    result = insertUser(email, displayName);
  }

  return result;
}

void ServicesDb::setUserId(const long sessionUserId)
{
  _currUserId = sessionUserId;

  LOG_DEBUG("User ID updated to " + QString::number(_currUserId));
}

void ServicesDb::setMapId(const long sessionMapId)
{
  _currMapId = sessionMapId;
  assert(_currMapId > 0);

  LOG_DEBUG("Map ID updated to " + QString::number(_currMapId));
}

long ServicesDb::getUserId(QString email, bool throwWhenMissing)
{
  if (_selectUserByEmail == 0)
  {
    _selectUserByEmail.reset(new QSqlQuery(_db));
    _selectUserByEmail->prepare("SELECT email, id, display_name FROM users WHERE email LIKE :email");
  }
  _selectUserByEmail->bindValue(":email", email);
  if (_selectUserByEmail->exec() == false)
  {
    throw HootException("Error finding user with the email: " + email + " " +
                        _selectUserByEmail->lastError().text());
  }

  long result = -1;
  if (_selectUserByEmail->next())
  {
    bool ok;
    result = _selectUserByEmail->value(1).toLongLong(&ok);
    if (!ok && throwWhenMissing)
    {
      throw HootException("Specified user was not found.");
    }
  }
  else if (throwWhenMissing)
  {
    QString error = QString("No user found with the email: %1 (maybe specify `%2=true`?)")
        .arg(email).arg(ConfigOptions::getServicesDbWriterCreateUserKey());
    LOG_WARN(error);
    throw HootException(error);
  }

  _selectUserByEmail->finish();

  return result;
}

long ServicesDb::_insertRecord(QSqlQuery& query)
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

bool ServicesDb::isSupported(QUrl url)
{
  bool valid = url.isValid();
  valid = valid && url.scheme() == "postgresql";

  if (valid)
  {
    QString path = url.path();
    QStringList plist = path.split("/");

    // Valid OSM API URL: postgresql://postgres@10.194.70.78:5432/terrytest
    // Valid Services DB: postgresql://myhost:5432/mydb/mylayer

    // OSM API will have plist.size of 2.
    if ( plist.size() == 2 )
    {
      ;
    }
    // 3 can be valid
    else if ( plist.size() == 3 )
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
  }

  return valid;
}

bool ServicesDb::isSupported(const QUrl& url, const DbType dbType)
{
  bool valid = url.isValid();
  valid = valid && url.scheme() == "postgresql";

  if (valid)
  {
    QString path = url.path();
    QStringList plist = path.split("/");

    // Valid OSM API URL: postgresql://postgres@10.194.70.78:5432/terrytest
    // Valid Services DB: postgresql://myhost:5432/mydb/mylayer

    // OSM API will have plist.size of 2.
    switch ( dbType )
    {
      case DBTYPE_OSMAPI:
        if ( plist.size() != 2 )
        {
          valid = false;
        }
        break;

      case DBTYPE_SERVICES:
        //LOG_DEBUG("Checking services URL: " << url.toString());

        // 3 can be valid
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
          // NO other list sizes are valid
          LOG_WARN("Looks like a DB path, but a DB name and layer name was expected. E.g. "
                   "postgresql://myhost:5432/mydb/mylayer");
          valid = false;
        }
        break;

      default:
        LOG_WARN("Invalid DB path");
        break;
    }
  }

  return valid;
}

void ServicesDb::_lazyFlushBulkInsert()
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

void ServicesDb::open(QUrl url)
{
  if (!isSupported(url))
  {
    throw HootException("An unsupported URL was passed in.");
  }

  QStringList pList = url.path().split("/");
  QString db = pList[1];

  QString connectionName = url.toString() + " 0x" + QString::number((qulonglong)this, 16);
  if (QSqlDatabase::contains(connectionName) == false)
  {
    _db = QSqlDatabase::addDatabase("QPSQL", connectionName);
  }
  else
  {
    _db = QSqlDatabase::database(connectionName);
  }

  if (_db.isOpen() == false)
  {
    _db.setDatabaseName(db);
    if (url.host() == "local")
    {
      _db.setHostName("/var/run/postgresql");
    }
    else
    {
      _db.setHostName(url.host());
    }
    _db.setPort(url.port());
    _db.setUserName(url.userName());
    _db.setPassword(url.password());

    if (_db.open() == false)
    {
      throw HootException("Error opening database: " + _db.lastError().text());
    }
  }

  if (_db.tables().size() == 0)
  {
    throw HootException("Attempting to open database " + url.toString() +
                        " but found zero tables. Does the DB exist? Has it been populated?");
  }

  // What kind of database is it
  _connectionType = _determineDbType();

  // Make sure URL still matches format we want once we've determined db type
  if ( isSupported(url, _connectionType ) == false )
  {
    throw HootException("An unsupported URL was passed in.");
  }

  _resetQueries();

  if ( (_connectionType == DBTYPE_SERVICES) && (isCorrectDbVersion() == false) )
  {
    LOG_WARN("Running against an unexpected DB version.");
    LOG_WARN("Expected: " << expectedDbVersion());
    LOG_WARN("Actual: " << getDbVersion());
  }

  QSqlQuery query("SET client_min_messages TO WARNING", _db);
  // if there was an error
  if (query.lastError().isValid())
  {
    LOG_WARN("Error disabling Postgresql INFO messages.");
  }

  LOG_DEBUG("Successfully opened database: " << url.toString());
}

void ServicesDb::_resetQueries()
{
  _closeChangeSet.reset();
  _insertChangeSet.reset();
  _insertChangeSetTag.reset();
  _insertMap.reset();
  _insertUser.reset();
  _insertWayNodes.reset();
  _insertRelationMembers.reset();
  _selectDbVersion.reset();
  _selectUserByEmail.reset();
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


  // OSM API
  _osmApiNodeIdReserver.reset();
  _osmApiWayIdReserver.reset();
}

void ServicesDb::rollback()
{
  _resetQueries();

  if (!_db.rollback())
  {
    LOG_WARN("Error rolling back transaction.");
    throw HootException("Error rolling back transaction: " + _db.lastError().text());
  }

  _inTransaction = false;
}

long ServicesDb::_round(double x)
{
  return (long)(x + 0.5);
}

long ServicesDb::_round(double x, int precision)
{
  return (long)(floor(x * (10 * (precision - 1)) + 0.5) / (10 * (precision - 1)));
  //return (long)(ceil(x * (10 * (precision - 1))) / (10 * (precision - 1)));
}

set<long> ServicesDb::selectMapIds(QString name)
{
  const long userId = _currUserId;

  if (_selectMapIds == 0)
  {
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

unsigned int ServicesDb::_tileForPoint(double lat, double lon)
{
  int lonInt = _round((lon + 180.0) * 65535.0 / 360.0);
  int latInt = _round((lat + 90.0) * 65535.0 / 180.0);

  unsigned int tile = 0;
  int          i;

  for (i = 15; i >= 0; i--)
  {
    tile = (tile << 1) | ((lonInt >> i) & 1);
    tile = (tile << 1) | ((latInt >> i) & 1);
  }

  return tile;
}

void ServicesDb::transaction()
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

QString ServicesDb::_elementTypeToElementTableName(long mapId, const ElementType& elementType) const
{
  if (elementType == ElementType::Node)
  {
    return _getNodesTableName(mapId);
  }
  else if (elementType == ElementType::Way)
  {
    return _getWaysTableName(mapId);
  }
  else if (elementType == ElementType::Relation)
  {
    return _getRelationsTableName(mapId);
  }
  else
  {
    throw HootException("Unsupported element type.");
  }
}

//TODO: consolidate these exists queries into a single method

bool ServicesDb::mapExists(const long id)
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

bool ServicesDb::changesetExists(const long id)
{
  const long mapId = _currMapId;

  _checkLastMapId(mapId);
  if (_changesetExists == 0)
  {
    _changesetExists.reset(new QSqlQuery(_db));
    _changesetExists->prepare(QString("SELECT num_changes FROM %1 WHERE id = :changesetId")
      .arg(_getChangesetsTableName(mapId)));
  }
  _changesetExists->bindValue(":changesetId", (qlonglong)id);
  if (_changesetExists->exec() == false)
  {
    throw HootException(_changesetExists->lastError().text());
  }

  return _changesetExists->next();
}

long ServicesDb::numElements(const ElementType& elementType)
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

shared_ptr<QSqlQuery> ServicesDb::selectAllElements(const long elementId, const ElementType& elementType)
{
  return selectElements(elementId, elementType, -1, 0);
}

shared_ptr<QSqlQuery> ServicesDb::selectAllElements(const ElementType& elementType)
{
  return selectElements(-1, elementType, -1, 0);
}



shared_ptr<QSqlQuery> ServicesDb::selectElements(const long elementId,
  const ElementType& elementType, const long limit, const long offset)
{
  const long mapId = _currMapId;
  _selectElementsForMap.reset(new QSqlQuery(_db));
  _selectElementsForMap->setForwardOnly(true);
  QString limitStr;
  if (limit == -1)
  {
    limitStr = "ALL";
  }
  else
  {
    limitStr = QString::number(limit);
  }

  QString sql =  "SELECT * FROM " + _elementTypeToElementTableName(mapId, elementType);

  if(elementId > -1)
  {
    sql += " WHERE id = :elementId ";
  }
  sql += " ORDER BY id LIMIT " + limitStr + " OFFSET " + QString::number(offset);
  _selectElementsForMap->prepare(sql);
  _selectElementsForMap->bindValue(":mapId", (qlonglong)mapId);
  if(elementId > -1)
  {
    _selectElementsForMap->bindValue(":elementId", (qlonglong)elementId);
  }

  if (_selectElementsForMap->exec() == false)
  {
    QString err = _selectElementsForMap->lastError().text();
    LOG_WARN(sql);
    throw HootException("Error selecting elements of type: " + elementType.toString() +
      " for map ID: " + QString::number(mapId) + " Error: " + err);
  }
  return _selectElementsForMap;
}

vector<long> ServicesDb::selectNodeIdsForWay(long wayId)
{
  const long mapId = _currMapId;

  vector<long> result;

  _checkLastMapId(mapId);

  if (!_selectNodeIdsForWay)
  {
    _selectNodeIdsForWay.reset(new QSqlQuery(_db));
    _selectNodeIdsForWay->setForwardOnly(true);
    _selectNodeIdsForWay->prepare(
      "SELECT node_id FROM " + _getWayNodesTableName(mapId) +
          " WHERE way_id = :wayId ORDER BY sequence_id");
  }
  _selectNodeIdsForWay->bindValue(":wayId", (qlonglong)wayId);
  if (_selectNodeIdsForWay->exec() == false)
  {
    throw HootException("Error selecting node ID's for way with ID: " + QString::number(wayId) +
      " Error: " + _selectNodeIdsForWay->lastError().text());
  }

  while (_selectNodeIdsForWay->next())
  {
    bool ok;
    result.push_back(_selectNodeIdsForWay->value(0).toLongLong(&ok));

    if (!ok)
    {
      QString err = QString("Error converting node ID to long. (%1)").
          arg(_selectNodeIdsForWay->value(0).toString());
      throw HootException(err);
    }
  }

  return result;
}

vector<RelationData::Entry> ServicesDb::selectMembersForRelation(long relationId)
{
  const long mapId = _currMapId;

  vector<RelationData::Entry> result;

  if (!_selectMembersForRelation)
  {
    _selectMembersForRelation.reset(new QSqlQuery(_db));
    _selectMembersForRelation->setForwardOnly(true);
#warning fix me.
    _selectMembersForRelation->prepare(
      "SELECT member_type, member_id, member_role FROM " + _getRelationMembersTableName(mapId) +
      " WHERE relation_id = :relationId ORDER BY sequence_id");
  }

  _selectMembersForRelation->bindValue(":mapId", (qlonglong)mapId);
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

void ServicesDb::_unescapeString(QString& s)
{
  if (s.startsWith("\""))
  {
    s.remove(0, 1);
  }
  if (s.endsWith("\""))
  {
    s.remove(s.size() - 1, 1);
  }

  s.replace("\\042", "\"");
  s.replace("\\\"", "\"");
  s.replace("\\\\", "\\");
  s.replace("\\134", "\\");
}

Tags ServicesDb::unescapeTags(const QVariant &v)
{
  assert(v.type() == QVariant::String);
  QString s = v.toString();
  // convert backslash and double quotes to their octal form so we can safely split on double quotes
  s.replace("\\\\", "\\134");
  s.replace("\\\"", "\\042");

  Tags result;
  QStringList l = s.split("\"");
  for (int i = 1; i < l.size(); i+=4)
  {
    _unescapeString(l[i]);
    _unescapeString(l[i + 2]);

    result.insert(l[i], l[i + 2]);
  }

  return result;
}

void ServicesDb::updateNode(const long id, const double lat, const double lon, const Tags& tags)
{
  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    _updateNode_Services(id, lat, lon, _currChangesetId, tags);
    break;
  default:
    throw HootException("UpdateNode on unsupported database type");
  }
}

void ServicesDb::updateRelation(const long id, const Tags& tags)
{
  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    _updateRelation_Services(id, _currChangesetId, tags);
    break;
  default:
    throw HootException("UpdateRelation on unsupported database type");
  }
}

void ServicesDb::updateWay(const long id, const Tags& tags)
{
  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    _updateWay_Services(id, _currChangesetId, tags);
    break;
  default:
    throw HootException("UpdateWay on unsupported database type");
  }

}

ServicesDb::DbType ServicesDb::_determineDbType()
{
  DbType retVal = DBTYPE_UNSUPPORTED;

  // OSM API has both nodes and current_nodes
  if ( (_hasTable("nodes") == true) && (_hasTable("current_nodes") == true ) )
  {
    retVal = DBTYPE_OSMAPI;
    LOG_DEBUG("Connection type set to OSM API DB");
  }

  // Services DB will always have tables for maps and review_items
  else if ( ( _hasTable("maps") == true ) && ( _hasTable("maps") == true ) )
  {
    retVal = DBTYPE_SERVICES;
    LOG_DEBUG("Connection type set to Services DB");
  }

  else
  {
    const QString err("Could not determine database type");
    LOG_ERROR(err);
    throw HootException(err);
  }

  return retVal;
}

bool ServicesDb::insertWay(const Tags &tags, long &assignedId)
{
  assignedId = _getNextWayId();

  return insertWay(assignedId, tags);
}

bool ServicesDb::insertWay(const long wayId, const Tags &tags)
{
  bool retVal = false;

  switch (_connectionType)
  {
  case DBTYPE_SERVICES:
    _insertWay_Services(wayId, _currChangesetId, tags);
    retVal = true;
    break;

  case DBTYPE_OSMAPI:
    _insertWay_OsmApi(wayId, tags);
    retVal = true;
    break;

  default:
    throw HootException("insertWay called on unsupported DB type");
    break;
  }

  return retVal;
}

void ServicesDb::_insertWay_Services(long wayId, long changeSetId, const Tags& tags)
{
  const long mapId = _currMapId;

  double start = Tgs::Time::getTime();

  _checkLastMapId(mapId);

  if (_wayBulkInsert == 0)
  {
    QStringList columns;
    columns << "id" << "changeset_id" << "tags";

    _wayBulkInsert.reset(new SqlBulkInsert(_db, _getWaysTableName(mapId), columns));
  }

  QList<QVariant> v;
  v.append((qlonglong)wayId);
  v.append((qlonglong)changeSetId);
  // escaping tags ensures that we won't introduce a SQL injection vulnerability, however, if a
  // bad tag is passed and it isn't escaped properly (shouldn't happen) it may result in a syntax
  // error.
  v.append(_escapeTags(tags));
  //v.append(_escapeIds(nids));

  _wayBulkInsert->insert(v);

  _wayNodesInsertElapsed += Tgs::Time::getTime() - start;

  _lazyFlushBulkInsert();

  LOG_DEBUG("Inserted way " << QString::number(wayId));
}

void ServicesDb::insertWayNodes(long wayId, const vector<long>& nodeIds)
{
  switch ( _connectionType )
  {
  case DBTYPE_SERVICES:
    _insertWayNodes_Services(wayId, nodeIds);
    break;
  case DBTYPE_OSMAPI:
    _insertWayNodes_OsmApi(wayId, nodeIds);
    break;
  default:
    throw HootException("InsertWayNodes called on unsupported database");
    break;
  }
}

void ServicesDb::_insertWayNodes_Services(long wayId, const vector<long>& nodeIds)
{
  const long mapId = _currMapId;
  double start = Tgs::Time::getTime();

  LOG_DEBUG("Inserting nodes into way " << QString::number(wayId));

  _checkLastMapId(mapId);

  if (_wayNodeBulkInsert == 0)
  {
    QStringList columns;
    columns << "way_id" << "node_id" << "sequence_id";

    _wayNodeBulkInsert.reset(new SqlBulkInsert(_db, _getWayNodesTableName(mapId), columns));
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

void ServicesDb::_insertRelation_Services(long relationId, long changeSetId, const Tags &tags)
{
  const long mapId = _currMapId;
  _checkLastMapId(mapId);

  if (_relationBulkInsert == 0)
  {
    QStringList columns;
    columns << "id" << "changeset_id" << "tags";

    _relationBulkInsert.reset(new SqlBulkInsert(_db, _getRelationsTableName(mapId), columns));
  }

  QList<QVariant> v;
  v.append((qlonglong)relationId);
  v.append((qlonglong)changeSetId);
  // escaping tags ensures that we won't introduce a SQL injection vulnerability, however, if a
  // bad tag is passed and it isn't escaped properly (shouldn't happen) it may result in a syntax
  // error.
  v.append(_escapeTags(tags));

  _relationBulkInsert->insert(v);

  _lazyFlushBulkInsert();
}


void ServicesDb::_updateNode_Services(long id, double lat, double lon, long changeSetId,
                            const Tags& tags)
{
  const long mapId = _currMapId;
  _flushBulkInserts();

  _checkLastMapId(mapId);

  if (_updateNode == 0)
  {
    _updateNode.reset(new QSqlQuery(_db));
    _updateNode->prepare(
      "UPDATE " + _getNodesTableName(mapId) +
      " SET latitude=:latitude, longitude=:longitude, changeset_id=:changeset_id, tile=:tile, "
      "    tags=:tags "
      "WHERE id=:id");
  }

  _updateNode->bindValue(":id", (qlonglong)id);
  _updateNode->bindValue(":latitude", (qlonglong)_round(lat * COORDINATE_SCALE, 7));
  _updateNode->bindValue(":longitude", (qlonglong)_round(lon * COORDINATE_SCALE, 7));
  _updateNode->bindValue(":changeset_id", (qlonglong)changeSetId);
  _updateNode->bindValue(":tile", (qlonglong)_tileForPoint(lat, lon));
  _updateNode->bindValue(":tags", _escapeTags(tags));

  if (_updateNode->exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_updateNode->executedQuery()).
        arg(_updateNode->lastError().text());
    LOG_WARN(err)
    throw HootException(err);
  }

  _updateNode->finish();
}

void ServicesDb::_updateRelation_Services(long id, long changeSetId, const Tags& tags)
{
  const long mapId = _currMapId;
  _flushBulkInserts();
  _checkLastMapId(mapId);

  if (_updateRelation == 0)
  {
    _updateRelation.reset(new QSqlQuery(_db));
    _updateRelation->prepare(
      "UPDATE " + _getRelationsTableName(mapId) +
      " SET changeset_id=:changeset_id, tags=:tags "
      "WHERE id=:id");
  }

  _updateRelation->bindValue(":id", (qlonglong)id);
  _updateRelation->bindValue(":changeset_id", (qlonglong)changeSetId);
  _updateRelation->bindValue(":tags", _escapeTags(tags));

  if (_updateRelation->exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_updateWay->executedQuery()).
        arg(_updateRelation->lastError().text());
    LOG_WARN(err)
    throw HootException(err);
  }

  _updateRelation->finish();
}

void ServicesDb::_updateWay_Services(long id, long changeSetId, const Tags& tags)
{
  const long mapId = _currMapId;
  _flushBulkInserts();
  _checkLastMapId(mapId);

  if (_updateWay == 0)
  {
    _updateWay.reset(new QSqlQuery(_db));
    _updateWay->prepare(
      "UPDATE " + _getWaysTableName(mapId) +
      " SET changeset_id=:changeset_id, tags=:tags "
      "WHERE id=:id");
  }

  _updateWay->bindValue(":id", (qlonglong)id);
  _updateWay->bindValue(":changeset_id", (qlonglong)changeSetId);
  _updateWay->bindValue(":tags", _escapeTags(tags));

  if (_updateWay->exec() == false)
  {
    QString err = QString("Error executing query: %1 (%2)").arg(_updateWay->executedQuery()).
        arg(_updateWay->lastError().text());
    LOG_WARN(err)
    throw HootException(err);
  }

  _updateWay->finish();
}

void ServicesDb::incrementChangesetChangeCount()
{
  _changesetChangeCount++;

  // If we've hit maximum count of changes for a changeset, close this one out and start a new one
  if ( _changesetChangeCount >= _maximumChangeSetEdits)
  {
    endChangeset();
    beginChangeset();
  }
}

void ServicesDb::_beginChangeset_OsmApi()
{
  // Insert the data we know now, have to fill in gaps when we close changeset
  QString changesetInsert("INSERT INTO changesets ( user_id, created_at, closed_at ) VALUES ( ");

  changesetInsert += QString::number(_currUserId) + ", now(), now() );";

  _execNoPrepare(changesetInsert);

  QSqlQuery getChangesetId(_db);
  getChangesetId.exec("SELECT currval('changesets_id_seq');");

  if (getChangesetId.next())
  {
    _currChangesetId = getChangesetId.value(0).toInt();
  }
}

void ServicesDb::_insertNode_OsmApi(const long id, const double lat, const double lon, const Tags &tags)
{
  /**
   * When inserting a new node into the OSM API DB, we need to add rows to the following
   * tables
   *    - nodes (full list of all nodes, including historical versions)
   *    - node_tags (tags associated with the new node)
   *    - current_nodes (subset of the nodes table, includes current version of all nodes)
   *    - current_nodes_tags (tags associated with current version of each node)
   */

  //LOG_DEBUG("Entering OSM node insert");

  double start = Tgs::Time::getTime();

  // Add to element cache
  ElementPtr newNode(new Node(Status::Unknown1, id, lon, lat, 0.0));
  newNode->setTags(tags);
  ConstElementPtr constNode(newNode);
  _elementCache->addElement(constNode);

  // See if node portion of cache is full and needs to be flushed
  if ( _elementCache->typeCount(ElementType::Node) == _elementCacheCapacity )
  {
    _flushElementCacheOsmApiNodes();
  }

  // Snag end time, update insert time
  _nodesInsertElapsed += Tgs::Time::getTime() - start;

  //LOG_DEBUG("Exiting OSM insert node with ID " << nodeId );
}

void ServicesDb::_flushElementCacheToDb()
{
  _flushElementCacheToDb(ElementType::Node);
  _flushElementCacheToDb(ElementType::Way);
  _flushElementCacheToDb(ElementType::Relation);
}

void ServicesDb::_flushElementCacheToDb(const ElementType::Type flushType )
{
  switch ( _connectionType )
  {
  case DBTYPE_OSMAPI:
    switch ( flushType )
    {
    case ElementType::Node:
      _flushElementCacheOsmApiNodes();
      break;
    case ElementType::Way:
      _flushElementCacheOsmApiWays();
      _flushElementCacheOsmApiWayNodes();
      break;
    case ElementType::Relation:
      _flushElementCacheOsmApiRelations();
      _flushElementCacheOsmApiRelationMembers();
      break;
    default:
      throw HootException("Invalid type passed to flush");
      break;
    }

    break;

  default:
    throw HootException("Unsupported DB type");
  }
}

void ServicesDb::_flushElementCacheOsmApiNodes()
{

  if ( _elementCache->typeCount(ElementType::Node) == 0 )
  {
     return;
  }

  //LOG_DEBUG("Starting flush of node cache")

  // already in transaction, no need to start new one

  try
  {
    // Start building string for command

    // Set up INSERT command for current_nodes.
    QString currentNodesInsertCmd = "INSERT INTO current_nodes ( id, latitude, longitude, changeset_id, visible, timestamp, tile, version ) VALUES ";

    // INSERT command for nodes
    QString nodesInsertCmd =        "INSERT INTO nodes         ( node_id, latitude, longitude, changeset_id, visible, timestamp, tile, version ) VALUES ";

    // INSERT command for current_node_tags
    QString currentNodeTagsInsertCmd = "INSERT INTO current_node_tags ( node_id, k, v ) VALUES ";

    // INSERT command for node_tags
    QString nodeTagsInsertCmd = "INSERT INTO node_tags ( node_id, version, k, v ) VALUES ";

    // Assume the insert tag commands don't need to be run until we see otherwise
    bool needInsertTags = false;

    // Iterate over all nodes in the cache, adding data to the current_nodes COPY command
    _elementCache->resetElementIterators();
    ConstNodePtr currNode = _elementCache->getNextNode();

    // Testing a smart pointer against zero is equivalent to checking against null
    while ( currNode != 0 )
    {
      // Populate variables to make string build a little bit cleaner
      const QString nodeIDString(QString::number(currNode->getId()));
      const QString latitudeString(QString::number(_round(currNode->getY() * COORDINATE_SCALE, 7)));
      const QString longitudeString(QString::number(_round(currNode->getX() * COORDINATE_SCALE, 7)));
      const QString changesetIDString(QString::number(_currChangesetId));
      const QString tileNumberString(QString::number(_tileForPoint(currNode->getY(), currNode->getX())));

      // nodes and current_nodes (identical data)
      QString nodesCurrentNodesRow =

        // Open paren
        "(" +

        // id
        nodeIDString + "," +

        // latitude
        latitudeString + "," +

        // longitude
        longitudeString + "," +

        // changeset_id
        changesetIDString + "," +

        // visible
        "'true'," +

        // timestamp
        "now()," +

        // tile
        tileNumberString + "," +

        // version
        "1" +

          // Closing paren
          ")";

        // current_node_tags and node_tags
        const Tags nodeTags = currNode->getTags();
        if ( nodeTags.size() > 0 )
        {
          // Mark that we've had to insert at least one tag
          needInsertTags = true;

          for ( Tags::const_iterator it = nodeTags.constBegin(); it != nodeTags.constEnd(); ++it )
          {
            // Do we need to prepend with comma?
            if ( currentNodeTagsInsertCmd.endsWith( ")" ) == true )
            {
              currentNodeTagsInsertCmd  += ", ";
              nodeTagsInsertCmd += ", ";
            }

            // String values containing single quotes need to be escaped (postgres uses double-single-quotes)
            QString escapedKey = it.key();
            QString escapedValue = it.value();
            escapedKey.replace("'", "''");
            escapedValue.replace("'", "''" );

            currentNodeTagsInsertCmd  +=
              // Opening paren
              "(" +

              // Id
              nodeIDString + "," +

              // Key (make sure to escape single quotes with double single quotes
              "'" + escapedKey + "'," +

              // Value
              "'" + escapedValue + "'" +

              // Closing paren
              ")";

            nodeTagsInsertCmd +=
              // Opening paren
              "(" +

              // Id
              nodeIDString + "," +

              // Version
              "1," +

              // Key (make sure to escape single quotes with double single quotes
              "'" + escapedKey + "'," +

              // Value
              "'" + escapedValue + "'" +

              // Closing paren
              ")";
          }
        }

        // If there are more nodes to include, add a comma to the node row statements
        currNode = _elementCache->getNextNode();
        if ( currNode != 0 )
        {
          nodesCurrentNodesRow += ", ";
        }

        // Both node tables get same chunk of parenthesized data
        currentNodesInsertCmd   += nodesCurrentNodesRow;
        nodesInsertCmd          += nodesCurrentNodesRow;
      }

      // Add final semicolons
      currentNodesInsertCmd   += ";";
      nodesInsertCmd          += ";";

      // Always execute inserts for both nodes tables
      //LOG_DEBUG("Current_nodes:\n\t" + currentNodesInsertCmd);
      _execNoPrepare(currentNodesInsertCmd);
      //LOG_DEBUG("nodes:\n\t" + nodesInsertCmd);
      _execNoPrepare(nodesInsertCmd);

      // Does node have tags?
      if ( needInsertTags == true )
      {
        // Do final semicolons
        currentNodeTagsInsertCmd += ";";
        nodeTagsInsertCmd += ";";

        //LOG_DEBUG("current_node_tags: \n\t" + currentNodeTagsInsertCmd);
        _execNoPrepare(currentNodeTagsInsertCmd);
        //LOG_DEBUG("node_tags: \n\t" + nodeTagsInsertCmd);
        _execNoPrepare(nodeTagsInsertCmd);
      }

  }
  catch ( ... )
  {
    throw HootException("Database error when inserting nodes");
  }

  // Remove all nodes now that they've been flushed
  _elementCache->removeElements(ElementType::Node);
}

void ServicesDb::_flushElementCacheOsmApiWays()
{
  if ( _elementCache->typeCount(ElementType::Way) == 0 )
  {
    //LOG_DEBUG("Bailing from flush of cached ways; nothing in cache!")
    return;
  }

  // First step is to flush any nodes to make sure we don't violate any foreign keys when inserting way
  _flushElementCacheOsmApiNodes();

  //LOG_DEBUG("Starting flush of way cache")

  // already in transaction, no need to start new one

  try
  {
    // Set up INSERT command for current_ways.
    QString currentWaysInsertCmd = "INSERT INTO current_ways ( id, changeset_id, timestamp, visible, version ) VALUES ";

    // INSERT command for ways
    QString waysInsertCmd =        "INSERT INTO ways         ( way_id, changeset_id, timestamp, visible, version ) VALUES ";

    // INSERT command for current_ways_tags
    QString currentWayTagsInsertCmd = "INSERT INTO current_way_tags ( way_id, k, v ) VALUES ";

    // INSERT command for way_tags
    QString wayTagsInsertCmd = "INSERT INTO way_tags ( way_id, version, k, v ) VALUES ";

    // Assume the insert tag commands don't need to be run until we see otherwise
    bool needInsertTags = false;

    // Iterate over all ways in the cache, adding data to the current_nodes INSERT command
    _elementCache->resetElementIterators();
    ConstWayPtr currWay = _elementCache->getNextWay();

    // Testing a smart pointer against zero is equivalent to checking against null
    while ( currWay != 0 )
    {
      // Populate variables to make string build a little bit cleaner
      const QString wayIDString(QString::number(currWay->getId()));
      const QString changesetIDString(QString::number(_currChangesetId));

      // waysand current_ways (identical data)
      QString waysCurrentWaysRow =

        // Open paren
        "(" +

        // id
        wayIDString + "," +

        // changeset_id
        changesetIDString + "," +

        // timestamp
        "now()," +

        // visible
        "'true'," +

        // version
        "1" +

        // Closing paren
        ")";

      // current_way_tags and way_tags
      const Tags wayTags = currWay->getTags();
      if ( wayTags.size() > 0 )
      {
        // Mark that we've had to insert at least one tag
        needInsertTags = true;
        for ( Tags::const_iterator it = wayTags.constBegin(); it != wayTags.constEnd(); ++it )
        {
          // Do we need to prepend with comma?
          if ( currentWayTagsInsertCmd.endsWith( ")" ) == true )
          {
            currentWayTagsInsertCmd  += ", ";
            wayTagsInsertCmd += ", ";
          }

          // String values containing single quotes need to be escaped (postgres uses double-single-quotes)
          QString escapedKey = it.key();
          QString escapedValue = it.value();
          escapedKey.replace("'", "''");
          escapedValue.replace("'", "''" );

          currentWayTagsInsertCmd  +=
            // Opening paren
            "(" +

            // Id
            wayIDString + "," +

            // Key (make sure to escape single quotes with double single quotes
            "'" + escapedKey + "'," +

            // Value
            "'" + escapedValue + "'" +

            // Closing paren
            ")";

          wayTagsInsertCmd +=
            // Opening paren
            "(" +

            // Id
            wayIDString + "," +

            // Version
            "1," +

            // Key (make sure to escape single quotes with double single quotes
            "'" + escapedKey + "'," +

            // Value
            "'" + escapedValue + "'" +

            // Closing paren
            ")";
        }
      }

      // If there are more ways to include, add a comma to the way row statements
      currWay = _elementCache->getNextWay();

      if ( currWay != 0 )
      {
        waysCurrentWaysRow += ", ";
      }

      // Both node tables get same chunk of parenthesized data
      currentWaysInsertCmd  += waysCurrentWaysRow;
      waysInsertCmd         += waysCurrentWaysRow;
    }

    // Add final semicolons
    currentWaysInsertCmd      += ";";
    waysInsertCmd             += ";";

    // Always execute inserts for all way and way node tables
    //LOG_DEBUG("Current_ways:\n\t" + currentWaysInsertCmd);
    _execNoPrepare(currentWaysInsertCmd);
    //LOG_DEBUG("ways:\n\t" + waysInsertCmd);
    _execNoPrepare(waysInsertCmd);

    // Does way have tags?
    if ( needInsertTags == true )
    {
      // Do final semicolons
      currentWayTagsInsertCmd += ";";
      wayTagsInsertCmd += ";";

      //LOG_DEBUG("current_node_tags: \n\t" + currentWayTagsInsertCmd);
      _execNoPrepare(currentWayTagsInsertCmd);
      //LOG_DEBUG("node_tags: \n\t" + wayTagsInsertCmd);
      _execNoPrepare(wayTagsInsertCmd);
    }

  }
  catch ( ... )
  {
    throw HootException("Database error when inserting ways");
  }

  // Remove all ways from cache as they've been written
  _elementCache->removeElements(ElementType::Way);
}

void ServicesDb::_flushElementCacheOsmApiWayNodes()
{
  if ( _wayNodesCache.size() == 0 )
  {
    //LOG_DEBUG("Bailing from flush of cached way nodes; nothing in cache!")
    return;
  }

  // First step is to flush any ways (which in turn flushes nodes) to make sure we don't violate any foreign keys when inserting way
  _flushElementCacheOsmApiWays();

  //LOG_DEBUG("Starting flush of way node cache");

  std::vector<long> wayIds;
  try
  {
    QString currentWayNodesInsertCmd = "INSERT INTO current_way_nodes (way_id, node_id, sequence_id) VALUES ";
    QString wayNodesInsertCmd = "INSERT INTO way_nodes (way_id, node_id, version, sequence_id) VALUES ";

    for ( std::map< long, std::vector<long> >::const_iterator wayNodeIter = _wayNodesCache.begin();
              wayNodeIter != _wayNodesCache.end(); ++wayNodeIter )
    {

      // Add way ID to list of way IDs (used for updating changeset envelope later on in function)
      wayIds.push_back(wayNodeIter->first);

      // Populate variables to make string build a little bit cleaner
      const QString wayIDString(QString::number(wayNodeIter->first));

      //LOG_DEBUG("Getting Node IDs");
      // way_nodes and current_way_nodes
      std::vector<long> wayNodeIds = wayNodeIter->second;

      //LOG_DEBUG("Got " + QString::number(wayNodeIds.size()) + " node IDs");

      unsigned int sequenceNumber = 1;
      for ( std::vector<long>::const_iterator nodeIter = wayNodeIds.begin(); nodeIter != wayNodeIds.end(); ++nodeIter, ++sequenceNumber)
      {
        // Do we need to prepend with comma?
        if ( currentWayNodesInsertCmd.endsWith( ")" ) == true )
        {
          currentWayNodesInsertCmd  += ", ";
          wayNodesInsertCmd += ", ";
        }

        currentWayNodesInsertCmd +=
            // Opening paren
            "(" +

            // Way Id
            wayIDString + ", " +

            // Node_Id
            QString::number(*nodeIter) + ", " +

            // Sequence ID
            QString::number(sequenceNumber) +

            // Closing paren
            ")";

        wayNodesInsertCmd +=
            // Opening paren
            "(" +

            // Way Id
            wayIDString + ", " +

            // Node_Id
            QString::number(*nodeIter) + ", " +

            // Version
            QString::number(1) +", " +

            // Sequence ID
            QString::number(sequenceNumber) +

            // Closing paren
            ")";
      }

    }

    // Add final semicolons
    currentWayNodesInsertCmd  += ";";
    wayNodesInsertCmd         += ";";

    _execNoPrepare(currentWayNodesInsertCmd);
    _execNoPrepare(wayNodesInsertCmd);

    _updateChangesetEnvelopeWayIds(wayIds);
  }
  catch ( ... )
  {
    throw HootException("Database error when inserting ways");
  }

  // Remove all way nodes from cache as they've been written
  _wayNodesCache.clear();
}




void ServicesDb::_updateChangesetEnvelope(const ConstNodePtr node)
{
  const double nodeX = node->getX();
  const double nodeY = node->getY();

  _changesetEnvelope.expandToInclude(nodeX, nodeY);
  //LOG_DEBUG("Changeset bounding box updated to include X=" + QString::number(nodeX) + ", Y=" + QString::number(nodeY));
}



void ServicesDb::_endChangeset_OsmApi()
{
  // Set lat/lon envelope and closing time for changset
  QSqlQuery closeChangeset(_db);

  closeChangeset.prepare(
    QString("UPDATE changesets SET min_lat=:min_lat, max_lat=:max_lat, min_lon=:min_lon, "
          "max_lon=:max_lon, closed_at=NOW(), num_changes=:num_changes WHERE id=:id;") );

  closeChangeset.bindValue(":min_lat", static_cast<qlonglong>(_round(_changesetEnvelope.getMinY() * COORDINATE_SCALE, 7)));
  closeChangeset.bindValue(":max_lat", static_cast<qlonglong>(_round(_changesetEnvelope.getMaxY() * COORDINATE_SCALE, 7)));
  closeChangeset.bindValue(":min_lon", static_cast<qlonglong>(_round(_changesetEnvelope.getMinX() * COORDINATE_SCALE, 7)));
  closeChangeset.bindValue(":max_lon", static_cast<qlonglong>(_round(_changesetEnvelope.getMaxX() * COORDINATE_SCALE, 7)));
  closeChangeset.bindValue(":num_changes", static_cast<qlonglong>(_changesetChangeCount) );
  closeChangeset.bindValue(":id", static_cast<qlonglong>(_currChangesetId));

  if (closeChangeset.exec() == false)
  {
    LOG_ERROR("query bound values: ");
    LOG_ERROR(closeChangeset.boundValues());
    LOG_ERROR("\n");
    throw HootException("Error executing close changeset: " + closeChangeset.lastError().text() +
                        " (SQL: " + closeChangeset.executedQuery() + ")");
  }
  else
  {
    //LOG_INFO("Successful changeset update with bound values: ");
    //LOG_VARI(closeChangeset.boundValues());
  }


  // Add tags to the changeset
  QSqlQuery addChangesetTags(_db);
  addChangesetTags.prepare(
    QString("INSERT INTO changeset_tags VALUES ( :changeset_id_1, 'created_by', 'Hootenanny' ), "
            "( :changeset_id_2, 'comment', 'Hootenanny data ingest' ), "
            "( :changeset_id_3, 'bot', 'yes' );") );

  addChangesetTags.bindValue(":changeset_id_1", static_cast<qlonglong>(_currChangesetId));
  addChangesetTags.bindValue(":changeset_id_2", static_cast<qlonglong>(_currChangesetId));
  addChangesetTags.bindValue(":changeset_id_3", static_cast<qlonglong>(_currChangesetId));

  if (addChangesetTags.exec() == false)
  {
    LOG_ERROR("query bound values: ");
    LOG_ERROR(addChangesetTags.boundValues());
    LOG_ERROR("\n");
    throw HootException("Error executing add changeset tags: " + addChangesetTags.lastError().text() +
                        " (SQL: " + addChangesetTags.executedQuery() + ")");
  }

  // Update users table as the number of changesets for this user has been incremented
  QSqlQuery getUserChangesetsCount(_db);
  getUserChangesetsCount.prepare(QString("SELECT changesets_count FROM users WHERE id=:id;"));
  getUserChangesetsCount.bindValue(":id", static_cast<qlonglong>(_currUserId));
  if ( getUserChangesetsCount.exec() == false )
  {
    LOG_ERROR("Could not get changesets_count from users");
    throw HootException("Could not get changesets_count");
  }

  int changesetsCount;
  if (getUserChangesetsCount.next())
  {
    changesetsCount = getUserChangesetsCount.value(0).toInt();
  }
  else
  {
    LOG_ERROR("Could not get changeset count data from result")
    throw HootException("Could not get changesets count data from result");
  }

  // Increment value of changesets_count and update database
   changesetsCount++;
   QSqlQuery updateUserChangesetsCount(_db);
   updateUserChangesetsCount.prepare(QString("UPDATE users SET changesets_count=:changeset_count WHERE id=:id;"));
   updateUserChangesetsCount.bindValue(":changeset_count", changesetsCount);
   updateUserChangesetsCount.bindValue(":id", static_cast<qlonglong>(_currUserId));
   if ( updateUserChangesetsCount.exec() == false )
   {
     LOG_ERROR("Could not update changesets_count for user");
     throw HootException("Could not get update changeset_counts for user");
   }

   // Add the user as a subscriber to the changeset they just added
   QSqlQuery addChangesetSubscriber(_db);
   addChangesetSubscriber.prepare(QString("INSERT INTO changesets_subscribers VALUES (:subscriber_id, :changeset_id);"));
   addChangesetSubscriber.bindValue(":subscriber_id", static_cast<qlonglong>(_currUserId));
   addChangesetSubscriber.bindValue(":changeset_id", static_cast<qlonglong>(_currChangesetId));
   if ( addChangesetSubscriber.exec() == false )
   {
     LOG_ERROR("Could not update changesets_count for user");
     throw HootException("Could not get update changeset_counts for user");
   }

   //LOG_DEBUG("Successfully closed changeset");
}

void ServicesDb::_updateChangesetEnvelopeWayIds(const std::vector<long>& wayIds)
{
  QString idListString;

  std::vector<long>::const_iterator idIter;

  for ( idIter = wayIds.begin(); idIter != wayIds.end(); ++idIter )
  {
    idListString += QString::number(*idIter) + ",";
  }

  // Remove last comma
  idListString.chop(1);

  // Get envelope for way from database, then update changeset envelope as needed
  QSqlQuery getWayEnvelopeCmd = _exec(QString(
        "SELECT way_id, MIN(latitude),MAX(latitude),MIN(longitude),MAX(longitude) "
        "FROM current_way_nodes JOIN current_nodes ON node_id = id "
        "WHERE way_id IN (%1) GROUP BY way_id;").arg(idListString));

  // NOTE: the result will return one row per way found in the list -- have to iterate until done!
  while (getWayEnvelopeCmd.next())
  {
    double minY = (double)getWayEnvelopeCmd.value(1).toLongLong() / (double)COORDINATE_SCALE;
    double maxY = (double)getWayEnvelopeCmd.value(2).toLongLong() / (double)COORDINATE_SCALE;
    double minX = (double)getWayEnvelopeCmd.value(3).toLongLong() / (double)COORDINATE_SCALE;
    double maxX = (double)getWayEnvelopeCmd.value(4).toLongLong() / (double)COORDINATE_SCALE;

    _changesetEnvelope.expandToInclude(minX, minY);
    _changesetEnvelope.expandToInclude(maxX, maxY);
  }
}

void ServicesDb::_flushElementCacheOsmApiRelations()
{
  if ( _elementCache->typeCount(ElementType::Relation) == 0 )
  {
    //LOG_DEBUG("Bailing from flush of cached relations; nothing in cache!")
    return;
  }

  // First step is to flush any ways and way nodes to make sure we don't violate foreign key integrity
  _flushElementCacheOsmApiWays();
  _flushElementCacheOsmApiWayNodes();

  //LOG_DEBUG("Starting flush of relation cache")

  // already in transaction, no need to start new one

  // Have to keep list of relation IDs we insert, so we can come back update changeset envelope
  std::vector<long> relationIds;

  try
  {
    // INSERT command for current_relations.
    QString currentRelationsInsertCmd( "INSERT INTO current_relations ( id, changeset_id, timestamp, visible, version ) VALUES " );

    // INSERT command for relations
    QString relationsInsertCmd(        "INSERT INTO relations         ( relation_id, changeset_id, timestamp, visible, version ) VALUES " );

    // INSERT command for current_relations_tags
    QString currentRelationTagsInsertCmd( "INSERT INTO current_relation_tags ( relation_id, k, v ) VALUES " );

    // INSERT command for relation_tags
    QString relationTagsInsertCmd( "INSERT INTO relation_tags ( relation_id, version, k, v ) VALUES " );

    // Assume the insert tag commands don't need to be run until we see otherwise
    bool needInsertTags = false;

    // Iterate over all ways in the cache, adding data to the current_nodes INSERT command
    _elementCache->resetElementIterators();
    ConstRelationPtr currRelation = _elementCache->getNextRelation();

    const QString changesetIDString(QString::number(_currChangesetId));

    // Testing a smart pointer against zero is equivalent to checking against null
    while ( currRelation != 0 )
    {
      // add to list of relation IDs that are being inserted, used later for changeset envelope update
      relationIds.push_back(currRelation->getId());

      // Populate variables to make string build a little bit cleaner
      const QString relationIDString(QString::number(currRelation->getId()));

      // relations and current_relations (identical data)
      QString relationsCurrentRelationsRow =

        // Open paren
        "(" +

        // id
        relationIDString + "," +

        // changeset_id
        changesetIDString + "," +

        // timestamp
        "now()," +

        // visible
        "'true'," +

        // version
        "1" +

        // Closing paren
        ")";

      // current_relation_tags and relation_tags
      const Tags relationTags = currRelation->getTags();
      if ( relationTags.size() > 0 )
      {
        // Mark that we've had to insert at least one tag
        needInsertTags = true;

        for ( Tags::const_iterator it = relationTags.constBegin(); it != relationTags.constEnd(); ++it )
        {
          // Do we need to prepend with comma?
          if ( currentRelationTagsInsertCmd.endsWith( ")" ) == true )
          {
            currentRelationTagsInsertCmd  += ", ";
            relationTagsInsertCmd += ", ";
          }

          // String values containing single quotes need to be escaped (postgres uses double-single-quotes)
          QString escapedKey = it.key();
          QString escapedValue = it.value();
          escapedKey.replace("'", "''");
          escapedValue.replace("'", "''" );

          currentRelationTagsInsertCmd  +=
            // Opening paren
            "(" +

            // Id
            relationIDString + "," +

            // Key (make sure to escape single quotes with double single quotes
            "'" + escapedKey + "'," +

            // Value
            "'" + escapedValue + "'" +

            // Closing paren
            ")";

          relationTagsInsertCmd +=
            // Opening paren
            "(" +

            // Id
            relationIDString + "," +

            // Version
            "1," +

            // Key (make sure to escape single quotes with double single quotes
            "'" + escapedKey + "'," +

            // Value
            "'" + escapedValue + "'" +

            // Closing paren
            ")";
        }
      }

      // Do we need to prepend comma?
      if ( currentRelationsInsertCmd.endsWith(")") == true )
      {
        currentRelationsInsertCmd  += ",";
        relationsInsertCmd        += ",";
      }


      // Both node tables get same chunk of parenthesized data
      currentRelationsInsertCmd += relationsCurrentRelationsRow;
      relationsInsertCmd        += relationsCurrentRelationsRow;

      currRelation = _elementCache->getNextRelation();
    }

    // Add final semicolons
    currentRelationsInsertCmd      += ";";
    relationsInsertCmd             += ";";

    // Always execute inserts for all way and way node tables
    //LOG_DEBUG("Current_ways:\n\t" + currentWaysInsertCmd);
    _execNoPrepare(currentRelationsInsertCmd);
    //LOG_DEBUG("ways:\n\t" + waysInsertCmd);
    _execNoPrepare(relationsInsertCmd);

    // Does way have tags?
    if ( needInsertTags == true )
    {
      // Do final semicolons
      currentRelationTagsInsertCmd += ";";
      relationTagsInsertCmd += ";";

      //LOG_DEBUG("current_node_tags: \n\t" + currentWayTagsInsertCmd);
      _execNoPrepare(currentRelationTagsInsertCmd);
      //LOG_DEBUG("node_tags: \n\t" + wayTagsInsertCmd);
      _execNoPrepare(relationTagsInsertCmd);
    }
  }
  catch ( ... )
  {
    throw HootException("Database error when inserting relations");
  }

  // Remove all relations from cache as they've been written
  _elementCache->removeElements(ElementType::Relation);
}

void ServicesDb::_flushElementCacheOsmApiRelationMembers()
{
  if ( _relationMembersCache.size() == 0 )
  {
    //LOG_DEBUG("Bailing from flush of relation members; nothing in cache!")
    return;
  }

  // First step is to flush any relations to make sure we don't violate any foreign keys when inserting relation member
  _flushElementCacheOsmApiRelations();

  //LOG_DEBUG("Starting flush of relation member cache");

  std::vector<long> relationIds;
  try
  {
    QString currentRelationMembersInsertCmd = "INSERT INTO current_relation_members (relation_id, member_type, member_id, member_role,          sequence_id) VALUES ";
    QString relationMembersInsertCmd =        "INSERT INTO         relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) VALUES ";

    for ( std::map< long, RelationMemberCacheEntry >::const_iterator relationMemberIter = _relationMembersCache.begin();
              relationMemberIter != _relationMembersCache.end(); ++relationMemberIter )
    {
      // Add relation ID to list of relation IDs (used for updating changeset envelope later on in function)
      relationIds.push_back(relationMemberIter->first);

      // Populate variables to make string build a little bit cleaner
      const QString relationIDString(QString::number(relationMemberIter->first));

      QString nwrType;
      switch ( relationMemberIter->second.elementId.getType().getEnum() )
      {
      case ElementType::Node:
        nwrType = "'Node'";
        break;
      case ElementType::Way:
        nwrType = "'Way'";
        break;

      case ElementType::Relation:
        nwrType = "'Relation'";
        break;


      default:
        LOG_DEBUG("Found unsupported member relation type");
        throw HootException("Unsupported relation member type");

        break;
      }

      RelationMemberCacheEntry currMember = relationMemberIter->second;
      QString sqlMemberRole = currMember.role;

      // Escape any single quotes
      sqlMemberRole.replace("'", "''");

      // Do we need to prepend with comma?
      if ( currentRelationMembersInsertCmd.endsWith( ")" ) == true )
      {
        currentRelationMembersInsertCmd  += ", ";
        relationMembersInsertCmd += ", ";
      }

      currentRelationMembersInsertCmd +=
          // Opening paren
          "(" +

          // Relation ID
          relationIDString + ", " +

          // Member type
          nwrType + ", " +

          // Member ID
          QString::number(currMember.elementId.getId()) + ", " +

          // Role
          "'" + sqlMemberRole + "', " +

          // Sequence ID
          QString::number(currMember.sequenceId) +

          // Closing paren
          ")";

      relationMembersInsertCmd +=
          // Opening paren
          "(" +

          // Relation ID
          relationIDString + ", " +

          // Member type
          nwrType + ", " +

          // Member ID
          QString::number(currMember.elementId.getId()) + ", " +

          // Role
          "'" + sqlMemberRole + "', " +

          // Version
          "1, " +

          // Sequence ID
          QString::number(currMember.sequenceId) +

          // Closing paren
          ")";
    }

    // Add final semicolons
    currentRelationMembersInsertCmd  += ";";
    relationMembersInsertCmd         += ";";

    /*
    _execNoPrepare(currentRelationMembersInsertCmd);
    _execNoPrepare(relationMembersInsertCmd);
    */
    //LOG_VARD(currentRelationMembersInsertCmd);
    //LOG_VARD(relationMembersInsertCmd);

    // TODO: Iterate over all the relations for the members we just inserted, updating changeset envelope
    //_updateChangesetEnvelopeRelationIds(relationIds);
  }
  catch ( ... )
  {
    throw HootException("Database error when inserting relation members");
  }

  // Remove all relation members from cache as they've been written
  _relationMembersCache.clear();
}

void ServicesDb::_insertWay_OsmApi(const long wayId, const Tags &tags)
{
  double start = Tgs::Time::getTime();

  // Add to element cache
  WayPtr newWay( new Way(Status::Unknown1, wayId, 0.0) );
  newWay->setTags(tags);
  ConstElementPtr constWay(newWay);
  _elementCache->addElement(constWay);

  // See if way portion of cache is full and needs to be flushed
  if ( _elementCache->typeCount(ElementType::Node) == _elementCacheCapacity )
  {
    _flushElementCacheOsmApiWays();
  }

  // Snag end time, update insert time
  _wayInsertElapsed += Tgs::Time::getTime() - start;

  // Note: changeset bounding box update is handled in flush, as it requires data to be in database
}

long ServicesDb::_getNextWayId_OsmApi()
{
  if ( _osmApiWayIdReserver == 0 )
  {
    _osmApiWayIdReserver.reset(new SequenceIdReserver(_db, "current_ways_id_seq"));
  }

  return _osmApiWayIdReserver->getNextId();
}

void ServicesDb::_insertWayNodes_OsmApi(const long wayId, const std::vector<long>& nodeIds)
{
  double start = Tgs::Time::getTime();

  // Add to cache of nodes for ways
  _wayNodesCache.insert( std::pair<long, std::vector<long> >(wayId, nodeIds) );

  // is way-node portion of cache full?
  if ( _wayNodesCache.size() == _elementCacheCapacity )
  {
    _flushElementCacheOsmApiWayNodes();
  }

  // Snag end time, update insert time
  _wayNodesInsertElapsed += Tgs::Time::getTime() - start;
}



}
