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

OsmChangesetSqlFileWriter::OsmChangesetSqlFileWriter(QUrl url) :
_changesetId(0)
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
    LOG_VARD(change.toString());
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
  _db.close();
}

void OsmChangesetSqlFileWriter::_createChangeSet()
{
  _changesetId = _db.getNextId("changesets");
  _outputSql.write(
    QString("INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES "
            "(%1, %2, now(), now());\n")
      .arg(_changesetId)
      .arg(ConfigOptions().getChangesetUserId())
    .toUtf8());
  //this will go away if user authentication is tied in at some point
  _outputSql.write(
    QString("INSERT INTO changeset_tags (changeset_id, k, v) VALUES "
            "(%1, '%2', '%3');\n")
      .arg(_changesetId)
      .arg("written_by")
      .arg("Hootenanny")
    .toUtf8());
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

  //API DB keeps history for all elements, so the element just gets set to invisible during a delete...
  //not actually deleted
  const QString values = QString("=%1;\n").arg(removedElement->getId());
  _outputSql.write(
    ("UPDATE " + elementName + "s SET visible=false WHERE " + elementName + "_id" + values).toUtf8());
  _outputSql.write(
    ("UPDATE current_" + elementName + "s SET visible=false WHERE id" + values).toUtf8());
}

// If osm.changeset.file.writer.generate.new.ids is false, then these create methods assume
// you've already set the ID correctly in terms of the OSM API target db for the element to be
// created.

void OsmChangesetSqlFileWriter::_create(const ConstNodePtr node)
{
  long id;
  if (ConfigOptions().getOsmChangesetSqlFileWriterGenerateNewIds())
  {
    id = _db.getNextId(ElementType::Node);
  }
  else
  {
    id = node->getId();
  }
  LOG_DEBUG("node create");
  LOG_VARD(id);

  QString values =
    QString("latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile, version) VALUES (%1, %2, %3, %4, true, now(), %5, 1);\n")
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
  long id;
  if (ConfigOptions().getOsmChangesetSqlFileWriterGenerateNewIds())
  {
    id = _db.getNextId(ElementType::Way);
  }
  else
  {
    id = way->getId();
  }
  LOG_DEBUG("way create");
  LOG_VARD(id);

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), 1);\n")
      .arg(id)
      .arg(_changesetId);
  _outputSql.write(("INSERT INTO ways (way_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_ways (id, " + values).toUtf8());

  _createWayNodes(id, way->getNodeIds());
  _createTags(way->getTags(), ElementId::way(id));
}

void OsmChangesetSqlFileWriter::_create(const ConstRelationPtr relation)
{
  long id;
  if (ConfigOptions().getOsmChangesetSqlFileWriterGenerateNewIds())
  {
    id = _db.getNextId(ElementType::Relation);
  }
  else
  {
    id = relation->getId();
  }
  LOG_DEBUG("relation create");
  LOG_VARD(id);

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), 1);\n")
      .arg(id)
      .arg(_changesetId);
  _outputSql.write(("INSERT INTO relations (relation_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_relations (id, " + values).toUtf8());

  _createRelationMembers(id, relation->getMembers());
  _createTags(relation->getTags(), ElementId::relation(id));
}

/*
 * For modify:
 *
   - <element-name> table contains all version of all elements of that type in a history, so insert
into that table.

   - current_<element-name> contains the single latest version of the element, so update that record.

  //The changeset deriver will start versions at 0 to keep the xml changeset writing happy, but
  //we always want 1 to be the starting point for sql writing.
*/

void OsmChangesetSqlFileWriter::_modify(const ConstNodePtr node)
{
  long version = -1;
  if (node->getVersion() == 0)
  {
    version = 2;
  }
  else
  {
    version = node->getVersion() + 1;
  }
  LOG_DEBUG("node modify");
  LOG_VARD(node->getId())
  LOG_VARD(version);

  QString values =
    QString("latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile, version) VALUES (%1, %2, %3, %4, true, now(), %5, %6);\n")
      .arg(node->getId())
      .arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7))
      .arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7))
      .arg(_changesetId)
      .arg(HootApiDb::tileForPoint(node->getY(), node->getX()))
      .arg(version);
  _outputSql.write(("INSERT INTO nodes (node_id, " + values).toUtf8());

  values =
    QString("latitude=%2, longitude=%3, changeset_id=%4, visible=true, \"timestamp\"=now(), tile=%5, version=%6 WHERE id=%1;\n")
      .arg(node->getId())
      .arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7))
      .arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7))
      .arg(_changesetId)
      .arg(HootApiDb::tileForPoint(node->getY(), node->getX()))
      .arg(version);
  _outputSql.write(("UPDATE current_nodes SET " + values).toUtf8());

  _deleteAllTags(ElementId::node(node->getId()));
  _createTags(node->getTags(), ElementId::node(node->getId()));
}

void OsmChangesetSqlFileWriter::_modify(const ConstWayPtr way)
{
  long version = -1;
  if (way->getVersion() == 0)
  {
    version = 2;
  }
  else
  {
    version = way->getVersion() + 1;
  }
  LOG_DEBUG("way modify");
  LOG_VARD(way->getId())
  LOG_VARD(version);

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), %3);\n")
      .arg(way->getId())
      .arg(_changesetId)
      .arg(version);
  _outputSql.write(("INSERT INTO ways (way_id, " + values).toUtf8());

  values =
    QString("changeset_id=%2, visible=true, \"timestamp\"=now(), version=%3 WHERE id=%1;\n")
      .arg(way->getId())
      .arg(_changesetId)
      .arg(version);
  _outputSql.write(("UPDATE current_ways SET " + values).toUtf8());

  _deleteAll("current_way_nodes", "way_id", way->getId());
  _deleteAll("way_nodes", "way_id", way->getId());
  _createWayNodes(way->getId(), way->getNodeIds());

  _deleteAllTags(ElementId::way(way->getId()));
  _createTags(way->getTags(), ElementId::way(way->getId()));
}

void OsmChangesetSqlFileWriter::_modify(const ConstRelationPtr relation)
{
  long version = -1;
  if (relation->getVersion() == 0)
  {
    version = 2;
  }
  else
  {
    version = relation->getVersion() + 1;
  }
  LOG_DEBUG("relation modify");
  LOG_VARD(relation->getId())
  LOG_VARD(version);

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), %3);\n")
      .arg(relation->getId())
      .arg(_changesetId)
      .arg(version);
  _outputSql.write(("INSERT INTO relations (relation_id, " + values).toUtf8());

  values =
    QString("changeset_id=%2, visible=true, \"timestamp\"=now(), version=%3 WHERE id=%1;\n")
      .arg(relation->getId())
      .arg(_changesetId)
      .arg(version);
  _outputSql.write(("UPDATE current_relations SET " + values).toUtf8());

  _deleteAll("current_relation_members", "relation_id", relation->getId());
  _deleteAll("relation_members", "relation_id", relation->getId());
  _createRelationMembers(relation->getId(), relation->getMembers());

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
  const QString tableName1 = "current_" + eid.getType().toString().toLower() + "_tags";
  tableNames.append(tableName1);
  const QString tableName2 = eid.getType().toString().toLower() + "_tags";
  tableNames.append(tableName2);
  return tableNames;
}

void OsmChangesetSqlFileWriter::_createWayNodes(const long wayId, const std::vector<long>& nodeIds)
{
  LOG_DEBUG("way nodes create");
  LOG_VARD(wayId);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    const long nodeId = nodeIds.at(i);
    LOG_VARD(nodeId);

    QString values =
      QString("(way_id, node_id, version, sequence_id) VALUES (%1, %2, 1, %3);\n")
        .arg(wayId)
        .arg(nodeId)
        .arg(i + 1);
    _outputSql.write(("INSERT INTO way_nodes " + values).toUtf8());

    values =
      QString("(way_id, node_id, sequence_id) VALUES (%1, %2, %3);\n")
        .arg(wayId)
        .arg(nodeId)
        .arg(i + 1);
    _outputSql.write(("INSERT INTO current_way_nodes " + values).toUtf8());
  }
}

void OsmChangesetSqlFileWriter::_createRelationMembers(const long relationId,
                                                       const vector<RelationData::Entry>& members)
{
  LOG_DEBUG("relation members create");
  LOG_VARD(relationId);
  for (size_t i = 0; i < members.size(); i++)
  {
    const RelationData::Entry member = members[i];
    LOG_VARD(member.toString());

    //TODO: is version right here?
    QString values =
      QString(
        "(relation_id, member_type, member_id, member_role, version, sequence_id) VALUES (%1, '%2', %3, '%4', 1, %5);\n")
        .arg(relationId)
        .arg(member.getElementId().getType().toString())
        .arg(member.getElementId().getId())
        .arg(member.getRole())
        .arg(i + 1);
    _outputSql.write(("INSERT INTO relation_members " + values).toUtf8());

    values =
      QString(
        "(relation_id, member_type, member_id, member_role, sequence_id) VALUES (%1, '%2', %3, '%4', %5);\n")
        .arg(relationId)
        .arg(member.getElementId().getType().toString())
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
    QString idFieldName = tableName;
    idFieldName.replace("current_", "").replace("_tags", "");
    idFieldName += "_id";
    _deleteAll(tableName, idFieldName, eid.getId());
  }
}

void OsmChangesetSqlFileWriter::_deleteAll(const QString tableName, const QString idFieldName,
                                           const long id)
{
  LOG_DEBUG("delete");
  LOG_VARD(tableName);
  _outputSql.write(
    (QString("DELETE FROM %1 WHERE %2 = %3;\n")
      .arg(tableName)
      .arg(idFieldName)
      .arg(id))
    .toUtf8());
}

}