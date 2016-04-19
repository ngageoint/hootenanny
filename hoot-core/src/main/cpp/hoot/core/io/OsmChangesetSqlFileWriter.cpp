#include "OsmChangesetSqlFileWriter.h"

// hoot
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QSqlError>
#include <QSqlQuery>

namespace hoot
{

OsmChangesetSqlFileWriter::OsmChangesetSqlFileWriter(QUrl url, bool useInternalIds) :
_useInternalIds(useInternalIds),
_changesetId(0),
_nodeId(0),
_wayId(0),
_relationId(0)
{
  _db.open(url);
}

void OsmChangesetSqlFileWriter::write(const QString path, const ChangeSetProviderPtr changesetProvider)
{
  LOG_INFO("Writing changeset to " << path);

  _outputSql.setFileName(path);
  if (_outputSql.open(QIODevice::WriteOnly | QIODevice::Text) == false)
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(path));
  }

  int changes = 0;
  _createChangeSet();

  while (changesetProvider->hasMoreChanges())
  {
    Change change = changesetProvider->readNextChange();
    switch (change.type)
    {
      case Change::Create:
        _createNewElement(change.e);
        break;
      case Change::Modify:
        _updateExistingElement(change.e);
        break;
      case Change::Delete:
        _deleteExistingElement(change.e);
        break;
      default:
        throw IllegalArgumentException("Unexpected change type.");
    }
    changes++;

    if (changes > ConfigOptions().getChangesetMaxSize())
    {
      _createChangeSet();
      changes = 0;
    }
  }

  _outputSql.close();
}

void OsmChangesetSqlFileWriter::_createChangeSet()
{
  if (!_useInternalIds)
  {
    _changesetId = _db.getNextId("changesets");
  }
  else
  {
    _changesetId++;
  }
  _outputSql.write(
    QString("INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES "
            "(%1, %2, now(), now());\n")
      .arg(_changesetId)
      .arg(ConfigOptions().getChangesetUserId())
    .toUtf8());
}

long OsmChangesetSqlFileWriter::_getNextId(const ElementType type)
{
  switch (type.getEnum())
  {
    case ElementType::Node:
      if (!_useInternalIds)
      {
        _nodeId = _db.getNextId("current_" + type.toString().toLower() + "s");
      }
      else
      {
        _nodeId++;
      }
      return _nodeId;
    case ElementType::Way:
      if (!_useInternalIds)
      {
        _wayId = _db.getNextId("current_" + type.toString().toLower() + "s");
      }
      else
      {
        _wayId++;
      }
      return _wayId;
    case ElementType::Relation:
      if (!_useInternalIds)
      {
        _relationId = _db.getNextId("current_" + type.toString().toLower() + "s");
      }
      else
      {
        _relationId++;
      }
      return _relationId;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangesetSqlFileWriter::_createNewElement(const ConstElementPtr newElement)
{
  switch (newElement->getElementType().getEnum())
  {
    case ElementType::Node:
      _create(dynamic_pointer_cast<const Node>(newElement));
      break;
    case ElementType::Way:
      _create(dynamic_pointer_cast<const Way>(newElement));
      break;
    case ElementType::Relation:
      _create(dynamic_pointer_cast<const Relation>(newElement));
      break;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangesetSqlFileWriter::_updateExistingElement(const ConstElementPtr updatedElement)
{
  switch (updatedElement->getElementType().getEnum())
  {
    case ElementType::Node:
      _modify(dynamic_pointer_cast<const Node>(updatedElement));
      break;
    case ElementType::Way:
      _modify(dynamic_pointer_cast<const Way>(updatedElement));
      break;
   case ElementType::Relation:
      _modify(dynamic_pointer_cast<const Relation>(updatedElement));
      break;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangesetSqlFileWriter::_deleteExistingElement(const ConstElementPtr removedElement)
{
  ElementType::Type elementType = removedElement->getElementType().getEnum();
  if (elementType != ElementType::Node && elementType != ElementType::Way &&
      elementType != ElementType::Relation)
  {
    throw HootException("Unknown element type");
  }
  const QString elementName = removedElement->getElementType().toString().toLower();

  _throwErrorIfElementAtVersionDoesntExist(
    "current_" + elementName + "s", "id", removedElement->getId(), removedElement->getVersion());

  //API DB keeps history for all elements, so the element just gets set to invisible during a delete...
  //not actually deleted
  const QString values = QString("=%1;\n").arg(removedElement->getId());
  _outputSql.write(
    ("UPDATE " + elementName + "s SET visible=false WHERE " + elementName + "_id" + values).toUtf8());
  _outputSql.write(
    ("UPDATE current_" + elementName + "s SET visible=false WHERE id" + values).toUtf8());
}

void OsmChangesetSqlFileWriter::_create(const ConstNodePtr node)
{
  const long id = _getNextId(ElementType::Node);

  QString values =
    QString("latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile,  version) VALUES (%1, %2, %3, %4, true, now(), %5, 1);\n")
      .arg(id)
      .arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7))
      .arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7))
      .arg(_changesetId)
      .arg(HootApiDb::tileForPoint(node->getY(), node->getX()));
  _outputSql.write(("INSERT INTO nodes (node_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_nodes (id, " + values).toUtf8());

  _createTags(node->getTags(), ElementId::node(id));
}

void OsmChangesetSqlFileWriter::_create(const ConstWayPtr way)
{
  const long id = _getNextId(ElementType::Way);

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), 1);\n")
      .arg(id)
      .arg(_changesetId);
  _outputSql.write(("INSERT INTO way (way_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_ways (id, " + values).toUtf8());

  _createWayNodes(id, way->getNodeIds());

  _createTags(way->getTags(), ElementId::way(id));
}

void OsmChangesetSqlFileWriter::_create(const ConstRelationPtr relation)
{
  const long id = _getNextId(ElementType::Relation);

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), 1);\n")
      .arg(id)
      .arg(_changesetId);
  _outputSql.write(("INSERT INTO relations (relation_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_relations (id, " + values).toUtf8());

  _createRelationMembers(id, relation->getType(), relation->getMembers());

  _createTags(relation->getTags(), ElementId::relation(id));
}

void OsmChangesetSqlFileWriter::_modify(const ConstNodePtr node)
{
  _throwErrorIfElementAtVersionDoesntExist("current_nodes", "id", node->getId(), node->getVersion());

  QString values =
    QString("=%1, latitude=%2, longitude=%3, changeset_id=%4, visible=true, \"timestamp\"=now(), tile=%5, version=version+1 WHERE version=%6;\n")
      .arg(node->getId())
      .arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7))
      .arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7))
      .arg(_changesetId)
      .arg(HootApiDb::tileForPoint(node->getY(), node->getX()))
      .arg(node->getVersion());
  _outputSql.write(("UPDATE nodes SET node_id" + values).toUtf8());
  _outputSql.write(("UPDATE current_nodes SET id" + values).toUtf8());

  _deleteAllTags(ElementId::node(node->getId()));
  _createTags(node->getTags(), ElementId::node(node->getId()));
}

void OsmChangesetSqlFileWriter::_modify(const ConstWayPtr way)
{
  _throwErrorIfElementAtVersionDoesntExist("current_ways", "id", way->getId(), way->getVersion());

  QString values =
    QString("=%1, changeset_id=%2, visible=true, \"timestamp\"=now(), version=version+1 WHERE version=%3;\n")
      .arg(way->getId())
      .arg(_changesetId)
      .arg(way->getVersion());
  _outputSql.write(("UPDATE ways SET way_id" + values).toUtf8());
  _outputSql.write(("UPDATE current_ways SET id" + values).toUtf8());

  _deleteAll("current_way_nodes", "way_id", way->getId());
  _deleteAll("way_nodes", "way_id", way->getId());
  _createWayNodes(way->getId(), way->getNodeIds());

  _deleteAllTags(ElementId::way(way->getId()));
  _createTags(way->getTags(), ElementId::way(way->getId()));
}

void OsmChangesetSqlFileWriter::_modify(const ConstRelationPtr relation)
{
  _throwErrorIfElementAtVersionDoesntExist(
    "current_relations", "id", relation->getId(), relation->getVersion());

  QString values =
    QString("=%1, changeset_id=%2, visible=true, \"timestamp\"=now(), version=version+1 WHERE version=%3;\n")
      .arg(relation->getId())
      .arg(_changesetId)
      .arg(relation->getVersion());
  _outputSql.write(("UPDATE relations SET relation_id" + values).toUtf8());
  _outputSql.write(("UPDATE current_relations SET relation_id" + values).toUtf8());

  _deleteAll("current_relation_members", "relation_id", relation->getId());
  _deleteAll("relation_members", "relation_id", relation->getId());
  _createRelationMembers(relation->getId(), relation->getType(), relation->getMembers());

  _deleteAllTags(ElementId::relation(relation->getId()));
  _createTags(relation->getTags(), ElementId::relation(relation->getId()));
}

void OsmChangesetSqlFileWriter::_createTags(const Tags& tags, ElementId eid)
{
  QStringList tableNames = _tagTableNamesForElement(eid);

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString k = it.key();
    QString v = it.value();

    QString values1 =
      QString("(%1_id, k, v) VALUES (%2, '%3', '%4');\n").
        arg(eid.getType().toString().toLower()).
        arg(eid.getId()).arg(k.replace('\'', "''")).
        arg(v.replace('\'', "''"));

    QString values2 =
      QString("(%1_id, k, v, version) VALUES (%2, '%3', '%4', 1);\n").
        arg(eid.getType().toString().toLower()).
        arg(eid.getId()).arg(k.replace('\'', "''")).
        arg(v.replace('\'', "''"));

    _outputSql.write((QString("INSERT INTO %1 ").arg(tableNames.at(0)) + values1).toUtf8());
    _outputSql.write((QString("INSERT INTO %1 ").arg(tableNames.at(1)) + values2).toUtf8());
  }
}

QStringList OsmChangesetSqlFileWriter::_tagTableNamesForElement(ElementId eid) const
{
  QStringList tableNames;
  QString tableName1 = "current_" + eid.getType().toString().toLower() + "_tags";
  tableNames.append(tableName1);
  QString tableName2 = eid.getType().toString().toLower() + "_tags";
  tableNames.append(tableName2);
  return tableNames;
}

void OsmChangesetSqlFileWriter::_createWayNodes(const long wayId, const std::vector<long>& nodeIds)
{
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    const long nodeId = nodeIds.at(i);

    QString values =
      QString("(way_id, node_id, version sequence_id) VALUES (%1, %2, 1, %3);\n")
        .arg(wayId)
        .arg(nodeId)
        .arg(i + 1);
    _outputSql.write(("INSERT INTO way_nodes (" + values).toUtf8());

    values =
      QString("(way_id, node_id, sequence_id) VALUES (%1, %2, %3);\n")
        .arg(wayId)
        .arg(nodeId)
        .arg(i + 1);
    _outputSql.write(("INSERT INTO current_way_nodes " + values).toUtf8());
  }
}

void OsmChangesetSqlFileWriter::_createRelationMembers(const long relationId, const QString type,
                                                       const vector<RelationData::Entry>& members)
{
  for (size_t i = 0; i < members.size(); i++)
  {
    const RelationData::Entry member = members[i];

    QString values =
      QString(
        "(relation_id, member_type, member_id, member_role, version, sequence_id) VALUES (%1, %2, %3, %4, 1, %5);\n")
        .arg(relationId)
        .arg(type)
        .arg(member.getElementId().getId())
        .arg(member.getRole())
        .arg(i + 1);
    _outputSql.write(("INSERT INTO relation_members " + values).toUtf8());

    values =
      QString(
        "(relation_id, member_type, member_id, member_role, sequence_id) VALUES (%1, %2, %3, %4, %5);\n")
        .arg(relationId)
        .arg(type)
        .arg(member.getElementId().getId())
        .arg(member.getRole())
        .arg(i + 1);
    _outputSql.write(("INSERT INTO current_relation_members " + values).toUtf8());
  }
}

void OsmChangesetSqlFileWriter::_deleteAllTags(ElementId eid)
{
  QStringList tableNames = _tagTableNamesForElement(eid);
  foreach (QString tableName, tableNames)
  {
    _deleteAll(tableName, tableName + "_id", eid.getId());
  }
}

void OsmChangesetSqlFileWriter::_deleteAll(const QString tableName, const QString idFieldName,
                                           const long id)
{
  _outputSql.write(
    (QString("DELETE FROM %1 WHERE %2 = %3;\n")
      .arg(tableName)
      .arg(idFieldName)
      .arg(id))
    .toUtf8());
}

void OsmChangesetSqlFileWriter::_throwErrorIfElementAtVersionDoesntExist(const QString tableName,
                                                                         const QString idFieldName,
                                                                         const long id,
                                                                         const long version)
{
  const QString ifStmt =
    QString("IF (SELECT count(*) FROM %1 WHERE %2=%3 AND version=%4) == 0 THEN\n")
      .arg(tableName)
      .arg(idFieldName)
      .arg(id)
      .arg(version);
  const QString error =
    QString("  RAISE EXCEPTION '%1 to be modified/deleted with ID: %2 and version: %3 does not exist';\n")
      .arg(tableName)
      .arg(id)
      .arg(version);
  _outputSql.write(QString(ifStmt + error + "END IF;\n").toUtf8());
}

}