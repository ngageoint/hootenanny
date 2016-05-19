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

  //_deletedElements[ElementType::Node] = QList<long>();
  //_deletedElements[ElementType::Way] = QList<long>();
  //_deletedElements[ElementType::Relation] = QList<long>();
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
  LOG_DEBUG("Creating changeset...");
  _changesetId = _db.getNextId("changesets");
  _outputSql.write(
    QString("INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES "
            "(%1, %2, now(), now());\n")
      .arg(_changesetId)
      .arg(ConfigOptions().getChangesetUserId())
    .toUtf8());
}

void OsmChangesetSqlFileWriter::_createNewElement(const ConstElementPtr element)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      _create(dynamic_pointer_cast<const Node>(element));
      break;
    case ElementType::Way:
      _create(dynamic_pointer_cast<const Way>(element));
      break;
    case ElementType::Relation:
      _create(dynamic_pointer_cast<const Relation>(element));
      break;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangesetSqlFileWriter::_updateExistingElement(const ConstElementPtr element)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      //if (!_deletedElements[ElementType::Node].contains(element->getId()))
      //{
        _modify(dynamic_pointer_cast<const Node>(element));
      //}
      break;
    case ElementType::Way:
      //if (!_deletedElements[ElementType::Way].contains(element->getId()))
      //{
        _modify(dynamic_pointer_cast<const Way>(element));
      //}
      break;
   case ElementType::Relation:
      //if (!_deletedElements[ElementType::Relation].contains(element->getId()))
      //{
        _modify(dynamic_pointer_cast<const Relation>(element));
      //}
      break;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangesetSqlFileWriter::_deleteExistingElement(const ConstElementPtr element)
{
  const ElementType::Type elementType = element->getElementType().getEnum();
  if (elementType != ElementType::Node && elementType != ElementType::Way &&
      elementType != ElementType::Relation)
  {
    throw HootException("Unknown element type");
  }

  const QString elementIdStr = QString::number(element->getId());
  const QString elementName = element->getElementType().toString().toLower();
  //const long version = element->getVersion() + 1;
  //LOG_VARD(version);
  //TODO: this needs to be handled better
  long version = -1;
  if (element->getVersion() < 1)
  {
    version = 2;
  }
  else
  {
    version = element->getVersion() + 1;
  }
  LOG_VARD(element->getId());
  if (element->getTags().contains("note"))
  {
    LOG_VARD(element->getTags().get("note"));
  }
  LOG_VARD(version);

  //OSM API DB keeps history for all elements, so the existing element in the master table is not
  //modified and a new record is added with the updated version and visible set to false
  _outputSql.write(("INSERT INTO " + elementName + "s (node_id, " +
                    _getInsertValuesStr(element->getId(), version, false, element)).toUtf8());

  _deleteCurrentTags(element->getElementId());

  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:

      _outputSql.write(
        ("DELETE FROM current_way_nodes WHERE node_id=" + elementIdStr + ";\n").toUtf8());
      _outputSql.write(
        ("DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = " +
         elementIdStr + ";\n").toUtf8());
      //_deletedElements[ElementType::Node].append(removedElement->getId());

      break;

    case ElementType::Way:

      //all of its entries in current way nodes are removed
      _outputSql.write(
        ("DELETE FROM current_way_nodes WHERE way_id=" + elementIdStr + ";\n").toUtf8());
      _outputSql.write(
        ("DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = " +
         elementIdStr + ";\n").toUtf8());
      //_deletedElements[ElementType::Way].append(removedElement->getId());

      break;

    case ElementType::Relation:

      _outputSql.write(
        ("DELETE FROM current_relation_members WHERE relation_id=" + elementIdStr + ";\n").toUtf8());
      _outputSql.write(
        ("DELETE FROM current_relation_members WHERE member_type = 'Relation' AND member_id = " +
         elementIdStr + ";\n").toUtf8());
      //_deletedElements[ElementType::Relation].append(removedElement->getId());

      break;

    default:
      throw HootException("Unknown element type");
  }

  //in the current table, the element isn't deleted but set to be invisible
  const QString values =
    QString("changeset_id=%1, visible=%2, version=%3 WHERE id=%4;\n")
      .arg(_changesetId)
      .arg(false)
      .arg(version)
      .arg(element->getId());
  _outputSql.write(("UPDATE current_" + elementName + "s SET " + values).toUtf8());
}

QString OsmChangesetSqlFileWriter::_getInsertValuesStr(const long id, const long version,
                                                       const bool visible,
                                                       const ConstElementPtr element) const
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      return _getInsertValuesStr(id, version, visible, dynamic_pointer_cast<const Node>(element));
    case ElementType::Way:
      return _getInsertValuesWayOrRelationStr(id, version, visible);
    case ElementType::Relation:
      return _getInsertValuesWayOrRelationStr(id, version, visible);
    default:
      throw HootException("Unknown element type");
  }
}

QString OsmChangesetSqlFileWriter::_getInsertValuesStr(const long id, const long version,
                                                       const bool visible,
                                                       const ConstNodePtr node) const
{
  const QString visibleStr = visible ? "true" : "false";
  return
    QString("latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile, version) VALUES (%1, %2, %3, %4, %5, now(), %6, %7);\n")
      .arg(id)
      .arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7))
      .arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7))
      .arg(_changesetId)
      .arg(visibleStr)
      .arg(HootApiDb::tileForPoint(node->getY(), node->getX()))
      .arg(version);
}

QString OsmChangesetSqlFileWriter::_getInsertValuesWayOrRelationStr(const long id,
                                                                    const long version,
                                                                    const bool visible) const
{
  const QString visibleStr = visible ? "true" : "false";
  return
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, %3, now(), %4);\n")
      .arg(id)
      .arg(_changesetId)
      .arg(visibleStr)
      .arg(version);
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
    LOG_VARD(id);
  }
  else
  {
    id = node->getId();
  }
  LOG_VARD(id);
  if (node->getTags().contains("note"))
  {
    LOG_VARD(node->getTags().get("note"));
  }

  const QString values = _getInsertValuesStr(id, 1, true, node);
  _outputSql.write(("INSERT INTO nodes (node_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_nodes (id, " + values).toUtf8());

  _createTags(node->getTags(), ElementId::node(id), 1);
}

void OsmChangesetSqlFileWriter::_create(const ConstWayPtr way)
{
  long id;
  if (ConfigOptions().getOsmChangesetSqlFileWriterGenerateNewIds())
  {
    id = _db.getNextId(ElementType::Way);
    LOG_VARD(id);
  }
  else
  {
    id = way->getId();
  }
  LOG_VARD(id);
  if (way->getTags().contains("note"))
  {
    LOG_VARD(way->getTags().get("note"));
  }

  const QString values = _getInsertValuesWayOrRelationStr(id, 1, true);
  _outputSql.write(("INSERT INTO ways (way_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_ways (id, " + values).toUtf8());

  /*const std::vector<long> nodeIds = way->getNodeIds();
  std::vector<long> verifiedNodeIds;
  for (unsigned int i = 0; i < nodeIds.size(); i++)
  {
    const long nodeId = nodeIds.at(i);
    if (!_deletedElements[ElementType::Node].contains(nodeId))
    {
      verifiedNodeIds.push_back(nodeId);
    }
  }*/
  _createWayNodes(id, /*verifiedNodeIds*/way->getNodeIds());
  _createTags(way->getTags(), ElementId::way(id), 1);
}

void OsmChangesetSqlFileWriter::_create(const ConstRelationPtr relation)
{
  long id;
  if (ConfigOptions().getOsmChangesetSqlFileWriterGenerateNewIds())
  {
    id = _db.getNextId(ElementType::Relation);
    LOG_VARD(id);
  }
  else
  {
    id = relation->getId();
  }
  LOG_VARD(id);
  if (relation->getTags().contains("note"))
  {
    LOG_VARD(relation->getTags().get("note"));
  }

  const QString values = _getInsertValuesWayOrRelationStr(id, 1, true);
  _outputSql.write(("INSERT INTO relations (relation_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_relations (id, " + values).toUtf8());

  /*const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
  std::vector<RelationData::Entry> verifiedRelationMembers;
  for (unsigned int i = 0; i < relationMembers.size(); i++)
  {
    const RelationData::Entry relationMember = relationMembers.at(i);
    if (!_deletedElements[relationMember.getElementId().getType().getEnum()].contains(
           relationMember.getElementId().getId()))
    {
      verifiedRelationMembers.push_back(relationMember);
    }
  }*/
  _createRelationMembers(id, /*verifiedRelationMembers*/relation->getMembers());
  _createTags(relation->getTags(), ElementId::relation(id), 1);
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
  LOG_VARD(node->getId());
  if (node->getTags().contains("note"))
  {
    LOG_VARD(node->getTags().get("note"));
  }
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

  _deleteCurrentTags(ElementId::node(node->getId()));
  _createTags(node->getTags(), ElementId::node(node->getId()), version);
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
  LOG_VARD(way->getId());
  if (way->getTags().contains("note"))
  {
    LOG_VARD(way->getTags().get("note"));
  }
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
  /*const std::vector<long> nodeIds = way->getNodeIds();
  std::vector<long> verifiedNodeIds;
  for (unsigned int i = 0; i < nodeIds.size(); i++)
  {
    const long nodeId = nodeIds.at(i);
    if (!_deletedElements[ElementType::Node].contains(nodeId))
    {
      verifiedNodeIds.push_back(nodeId);
    }
  }*/
  _createWayNodes(way->getId(), /*verifiedNodeIds*/way->getNodeIds());

  _deleteCurrentTags(ElementId::way(way->getId()));
  _createTags(way->getTags(), ElementId::way(way->getId()), version);
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
  LOG_VARD(relation->getId());
  if (relation->getTags().contains("note"))
  {
    LOG_VARD(relation->getTags().get("note"));
  }
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
  /*const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
  std::vector<RelationData::Entry> verifiedRelationMembers;
  for (unsigned int i = 0; i < relationMembers.size(); i++)
  {
    const RelationData::Entry relationMember = relationMembers.at(i);
    if (!_deletedElements[relationMember.getElementId().getType().getEnum()].contains(
           relationMember.getElementId().getId()))
    {
      verifiedRelationMembers.push_back(relationMember);
    }
  }*/
  _createRelationMembers(relation->getId(), /*verifiedRelationMembers*/relation->getMembers());

  _deleteCurrentTags(ElementId::relation(relation->getId()));
  _createTags(relation->getTags(), ElementId::relation(relation->getId()), version);
}

void OsmChangesetSqlFileWriter::_createTags(const Tags& tags, ElementId eid, const long version)
{
  QStringList tableNames = _tagTableNamesForElement(eid);

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString k = it.key();
    QString v = it.value();

    QString currentTagValues =
      QString("(%1_id, k, v) VALUES (%2, '%3', '%4');\n")
        .arg(eid.getType().toString().toLower())
        .arg(eid.getId())
        .arg(k.replace('\'', "''"))
        .arg(v.replace('\'', "''"));

    QString tagValues =
      QString("(%1_id, k, v, version) VALUES (%2, '%3', '%4', %5);\n")
        .arg(eid.getType().toString().toLower())
        .arg(eid.getId())
        .arg(k.replace('\'', "''"))
        .arg(v.replace('\'', "''"))
        .arg(version);

    _outputSql.write((QString("INSERT INTO %1 ").arg(tableNames.at(0)) + currentTagValues).toUtf8());
    _outputSql.write((QString("INSERT INTO %1 ").arg(tableNames.at(1)) + tagValues).toUtf8());
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
  //LOG_DEBUG("way nodes create");
  //LOG_VARD(wayId);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    const long nodeId = nodeIds.at(i);
    //LOG_VARD(nodeId);

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
  //LOG_DEBUG("relation members create");
  //LOG_VARD(relationId);
  for (size_t i = 0; i < members.size(); i++)
  {
    const RelationData::Entry member = members[i];
    //LOG_VARD(member.toString());

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

void OsmChangesetSqlFileWriter::_deleteCurrentTags(ElementId eid)
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