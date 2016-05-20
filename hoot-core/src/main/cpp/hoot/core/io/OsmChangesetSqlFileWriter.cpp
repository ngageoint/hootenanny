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

  _changeElementIdsToVersionsByElementType[ElementType::Node] = QMap<long, long>();
  _changeElementIdsToVersionsByElementType[ElementType::Way] = QMap<long, long>();
  _changeElementIdsToVersionsByElementType[ElementType::Relation] = QMap<long, long>();
}

void OsmChangesetSqlFileWriter::write(const QString path, ChangeSetProviderPtr changesetProvider)
{
  LOG_DEBUG("Writing changeset to " << path);

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

void OsmChangesetSqlFileWriter::_createNewElement(ElementPtr element)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      _create(dynamic_pointer_cast<Node>(element));
      break;
    case ElementType::Way:
      _create(dynamic_pointer_cast<Way>(element));
      break;
    case ElementType::Relation:
      _create(dynamic_pointer_cast<Relation>(element));
      break;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangesetSqlFileWriter::_updateExistingElement(ElementPtr element)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      _modify(dynamic_pointer_cast<Node>(element));
      break;
    case ElementType::Way:
      _modify(dynamic_pointer_cast<Way>(element));
      break;
   case ElementType::Relation:
      _modify(dynamic_pointer_cast<Relation>(element));
      break;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangesetSqlFileWriter::_deleteExistingElement(ElementPtr element)
{
  const ElementType::Type elementType = element->getElementType().getEnum();
  if (elementType != ElementType::Node && elementType != ElementType::Way &&
      elementType != ElementType::Relation)
  {
    throw HootException("Unknown element type");
  }

  const QString elementIdStr = QString::number(element->getId());
  const QString elementTypeStr = element->getElementType().toString().toLower();
  long currentVersion = -1;
  if (_changeElementIdsToVersionsByElementType[elementType].contains(element->getId()))
  {
    currentVersion = _changeElementIdsToVersionsByElementType[elementType].value(element->getId());
  }
  else
  {
    currentVersion = element->getVersion();
  }
  //TODO: hack - not sure why the version would ever be less than 1 by this point
  if (currentVersion < 1)
  {
    currentVersion = 2;
  }
  const long newVersion = currentVersion + 1;
  QString note = "";
  if (element->getTags().contains("note"))
  {
    note = element->getTags().get("note");
  }
  element->setVersion(newVersion);
  _changeElementIdsToVersionsByElementType[elementType][element->getId()] = newVersion;
  element->setVisible(false);
  element->setChangeset(_changesetId);
  LOG_VARD(element->getId());
  LOG_VARD(note);
  LOG_VARD(element->getVersion());
  QString commentStr = "/* delete " + elementTypeStr;
  if (!note.isEmpty())
  {
    commentStr += " - note: " + note;
  }
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  //OSM API DB keeps history for all elements, so the existing element in the master table is not
  //modified and a new record is added with the updated version and visible set to false
  _outputSql.write(("INSERT INTO " + elementTypeStr + "s (" + elementTypeStr + "_id, " +
                    _getInsertValuesStr(element)).toUtf8());

  _deleteCurrentTags(element->getElementId());

  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:

      _outputSql.write(
        ("DELETE FROM current_way_nodes WHERE node_id=" + elementIdStr + ";\n").toUtf8());
      _outputSql.write(
        ("DELETE FROM current_relation_members WHERE member_type = 'Node' AND member_id = " +
         elementIdStr + ";\n").toUtf8());

      break;

    case ElementType::Way:

      //all of its entries in current way nodes are removed
      _outputSql.write(
        ("DELETE FROM current_way_nodes WHERE way_id=" + elementIdStr + ";\n").toUtf8());
      _outputSql.write(
        ("DELETE FROM current_relation_members WHERE member_type = 'Way' AND member_id = " +
         elementIdStr + ";\n").toUtf8());

      break;

    case ElementType::Relation:

      _outputSql.write(
        ("DELETE FROM current_relation_members WHERE relation_id=" + elementIdStr + ";\n").toUtf8());
      _outputSql.write(
        ("DELETE FROM current_relation_members WHERE member_type = 'Relation' AND member_id = " +
         elementIdStr + ";\n").toUtf8());

      break;

    default:
      throw HootException("Unknown element type");
  }

  //in the current table, the element isn't deleted but set to be invisible
  const QString values =
    QString("changeset_id=%1, visible=%2, version=%3 WHERE id=%4;\n")
      .arg(element->getChangeset())
      .arg(_getVisibleStr(element->getVisible()))
      .arg(element->getVersion())
      .arg(element->getId());
  _outputSql.write(("UPDATE current_" + elementTypeStr + "s SET " + values).toUtf8());
}

QString OsmChangesetSqlFileWriter::_getInsertValuesStr(const ConstElementPtr element) const
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      return _getInsertValuesStr(dynamic_pointer_cast<const Node>(element));
    case ElementType::Way:
      return _getInsertValuesWayOrRelationStr(element);
    case ElementType::Relation:
      return _getInsertValuesWayOrRelationStr(element);
    default:
      throw HootException("Unknown element type");
  }
}

QString OsmChangesetSqlFileWriter::_getInsertValuesStr(const ConstNodePtr node) const
{
  return
    QString("latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile, version) VALUES (%1, %2, %3, %4, %5, now(), %6, %7);\n")
      .arg(node->getId())
      .arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7))
      .arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7))
      .arg(node->getChangeset())
      .arg(_getVisibleStr(node->getVisible()))
      .arg(HootApiDb::tileForPoint(node->getY(), node->getX()))
      .arg(node->getVersion());
}

QString OsmChangesetSqlFileWriter::_getInsertValuesWayOrRelationStr(const ConstElementPtr element) const
{
  return
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, %3, now(), %4);\n")
      .arg(element->getId())
      .arg(element->getChangeset())
      .arg(_getVisibleStr(element->getVisible()))
      .arg(element->getVersion());
}

// If osm.changeset.file.writer.generate.new.ids is false, then these create methods assume
// you've already set the ID correctly in terms of the OSM API target db for the element to be
// created.

void OsmChangesetSqlFileWriter::_create(NodePtr node)
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
  node->setId(id);
  node->setVersion(1);
  node->setVisible(true);
  node->setChangeset(_changesetId);
  //if an element is being created, we're starting fresh so overwriting the map entry is ok
  _changeElementIdsToVersionsByElementType[ElementType::Node].insert(
    node->getId(), node->getVersion());
  QString note = "";
  if (node->getTags().contains("note"))
  {
    note = node->getTags().get("note");
  }
  LOG_VARD(node->getId());
  LOG_VARD(note);
  LOG_VARD(node->getVersion());
  QString commentStr = "/* create node ";
  if (!note.isEmpty())
  {
    commentStr += " - note: " + note;
  }
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  const QString values = _getInsertValuesStr(dynamic_pointer_cast<const Node>(node));
  _outputSql.write(("INSERT INTO nodes (node_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_nodes (id, " + values).toUtf8());

  _createTags(node);
}

void OsmChangesetSqlFileWriter::_create(WayPtr way)
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
  QString note = "";
  if (way->getTags().contains("note"))
  {
    note = way->getTags().get("note");
  }
  way->setId(id);
  way->setVersion(1);
  way->setVisible(true);
  way->setChangeset(_changesetId);
  //if an element is being created, we're starting fresh so overwriting the map entry is ok
  _changeElementIdsToVersionsByElementType[ElementType::Way].insert(
    way->getId(), way->getVersion());
  LOG_VARD(way->getId());
  LOG_VARD(note);
  LOG_VARD(way->getVersion());
  QString commentStr = "/* create way ";
  if (!note.isEmpty())
  {
    commentStr += " - note: " + note;
  }
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  const QString values = _getInsertValuesWayOrRelationStr(way);
  _outputSql.write(("INSERT INTO ways (way_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_ways (id, " + values).toUtf8());

  _createWayNodes(way);
  _createTags(way);
}

void OsmChangesetSqlFileWriter::_create(RelationPtr relation)
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
  QString note = "";
  if (relation->getTags().contains("note"))
  {
    note = relation->getTags().get("note");
  }
  relation->setId(id);
  relation->setVersion(1);
  relation->setVisible(true);
  relation->setChangeset(_changesetId);
  //if an element is being created, we're starting fresh so overwriting the map entry is ok
  _changeElementIdsToVersionsByElementType[ElementType::Relation].insert(
    relation->getId(), relation->getVersion());
  LOG_VARD(relation->getId());
  LOG_VARD(note);
  LOG_VARD(relation->getVersion());
  QString commentStr = "/* create relation ";
  if (!note.isEmpty())
  {
    commentStr += " - note: " + note;
  }
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  const QString values = _getInsertValuesWayOrRelationStr(relation);
  _outputSql.write(("INSERT INTO relations (relation_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_relations (id, " + values).toUtf8());

  _createRelationMembers(relation);
  _createTags(relation);
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

void OsmChangesetSqlFileWriter::_modify(NodePtr node)
{
  //if another parsed change previously modified the element with this id, we want to get the
  //modified version
  long currentVersion = -1;
  if (_changeElementIdsToVersionsByElementType[ElementType::Node].contains(node->getId()))
  {
    currentVersion = _changeElementIdsToVersionsByElementType[ElementType::Node].value(node->getId());
  }
  else
  {
    currentVersion = node->getVersion();
  }
  const long newVersion = currentVersion + 1;
  QString note = "";
  if (node->getTags().contains("note"))
  {
    note = node->getTags().get("note");
  }
  node->setVersion(newVersion);
  _changeElementIdsToVersionsByElementType[ElementType::Node][node->getId()] = newVersion;
  node->setChangeset(_changesetId);
  LOG_VARD(node->getId());
  LOG_VARD(note);
  LOG_VARD(node->getVersion());
  QString commentStr = "/* modify node ";
  if (!note.isEmpty())
  {
    commentStr += " - note: " + note;
  }
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  QString values =
    QString("latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile, version) VALUES (%1, %2, %3, %4, true, now(), %5, %6);\n")
      .arg(node->getId())
      .arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7))
      .arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7))
      .arg(node->getChangeset())
      .arg(HootApiDb::tileForPoint(node->getY(), node->getX()))
      .arg(node->getVersion());
  _outputSql.write(("INSERT INTO nodes (node_id, " + values).toUtf8());

  values =
    QString("latitude=%2, longitude=%3, changeset_id=%4, visible=true, \"timestamp\"=now(), tile=%5, version=%6 WHERE id=%1;\n")
      .arg(node->getId())
      .arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7))
      .arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7))
      .arg(node->getChangeset())
      .arg(HootApiDb::tileForPoint(node->getY(), node->getX()))
      .arg(node->getVersion());
  _outputSql.write(("UPDATE current_nodes SET " + values).toUtf8());

  _deleteCurrentTags(ElementId::node(node->getId()));
  _createTags(node);
}

void OsmChangesetSqlFileWriter::_modify(WayPtr way)
{
  //if another parsed change previously modified the element with this id, we want to get the
  //modified version
  long currentVersion = -1;
  if (_changeElementIdsToVersionsByElementType[ElementType::Way].contains(way->getId()))
  {
    currentVersion = _changeElementIdsToVersionsByElementType[ElementType::Way].value(way->getId());
  }
  else
  {
    currentVersion = way->getVersion();
  }
  const long newVersion = currentVersion + 1;
  QString note = "";
  if (way->getTags().contains("note"))
  {
    note = way->getTags().get("note");
  }
  way->setVersion(newVersion);
  _changeElementIdsToVersionsByElementType[ElementType::Way][way->getId()] = newVersion;
  way->setChangeset(_changesetId);
  LOG_VARD(way->getId());
  LOG_VARD(note);
  LOG_VARD(way->getVersion());
  QString commentStr = "/* modify way ";
  if (!note.isEmpty())
  {
    commentStr += " - note: " + note;
  }
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), %3);\n")
      .arg(way->getId())
      .arg(way->getChangeset())
      .arg(way->getVersion());
  _outputSql.write(("INSERT INTO ways (way_id, " + values).toUtf8());

  values =
    QString("changeset_id=%2, visible=true, \"timestamp\"=now(), version=%3 WHERE id=%1;\n")
      .arg(way->getId())
      .arg(way->getChangeset())
      .arg(way->getVersion());
  _outputSql.write(("UPDATE current_ways SET " + values).toUtf8());

  _deleteAll("current_way_nodes", "way_id", way->getId());
  _deleteAll("way_nodes", "way_id", way->getId());
  _createWayNodes(way);

  _deleteCurrentTags(ElementId::way(way->getId()));
  _createTags(way);
}

void OsmChangesetSqlFileWriter::_modify(RelationPtr relation)
{
  //if another parsed change previously modified the element with this id, we want to get the
  //modified version
  long currentVersion = -1;
  if (_changeElementIdsToVersionsByElementType[ElementType::Relation].contains(relation->getId()))
  {
    currentVersion =
      _changeElementIdsToVersionsByElementType[ElementType::Relation].value(relation->getId());
  }
  else
  {
    currentVersion = relation->getVersion();
  }
  const long newVersion = currentVersion + 1;
  QString note = "";
  if (relation->getTags().contains("note"))
  {
    note = relation->getTags().get("note");
  }
  relation->setVersion(newVersion);
  _changeElementIdsToVersionsByElementType[ElementType::Relation][relation->getId()] = newVersion;
  relation->setChangeset(_changesetId);
  LOG_VARD(relation->getId());
  LOG_VARD(note);
  LOG_VARD(relation->getVersion());
  QString commentStr = "/* modify relation ";
  if (!note.isEmpty())
  {
    commentStr += " - note: " + note;
  }
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), %3);\n")
      .arg(relation->getId())
      .arg(relation->getChangeset())
      .arg(relation->getVersion());
  _outputSql.write(("INSERT INTO relations (relation_id, " + values).toUtf8());

  values =
    QString("changeset_id=%2, visible=true, \"timestamp\"=now(), version=%3 WHERE id=%1;\n")
      .arg(relation->getId())
      .arg(relation->getChangeset())
      .arg(relation->getVersion());
  _outputSql.write(("UPDATE current_relations SET " + values).toUtf8());

  _deleteAll("current_relation_members", "relation_id", relation->getId());
  _deleteAll("relation_members", "relation_id", relation->getId());
  _createRelationMembers(relation);

  _deleteCurrentTags(ElementId::relation(relation->getId()));
  _createTags(relation);
}

void OsmChangesetSqlFileWriter::_createTags(const ConstElementPtr element)
{
  QStringList tableNames = _tagTableNamesForElement(element->getElementId());
  const Tags tags = element->getTags();

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString k = it.key();
    QString v = it.value();

    const QString currentTagValues =
      QString("(%1_id, k, v) VALUES (%2, '%3', '%4');\n")
        .arg(element->getElementId().getType().toString().toLower())
        .arg(element->getElementId().getId())
        .arg(k.replace('\'', "''"))
        .arg(v.replace('\'', "''"));

    const QString tagValues =
      QString("(%1_id, k, v, version) VALUES (%2, '%3', '%4', %5);\n")
        .arg(element->getElementId().getType().toString().toLower())
        .arg(element->getElementId().getId())
        .arg(k.replace('\'', "''"))
        .arg(v.replace('\'', "''"))
        .arg(element->getVersion());

    _outputSql.write((QString("INSERT INTO %1 ").arg(tableNames.at(0)) + currentTagValues).toUtf8());
    _outputSql.write((QString("INSERT INTO %1 ").arg(tableNames.at(1)) + tagValues).toUtf8());
  }
}

QStringList OsmChangesetSqlFileWriter::_tagTableNamesForElement(const ElementId& eid) const
{
  QStringList tableNames;
  const QString tableName1 = "current_" + eid.getType().toString().toLower() + "_tags";
  tableNames.append(tableName1);
  const QString tableName2 = eid.getType().toString().toLower() + "_tags";
  tableNames.append(tableName2);
  return tableNames;
}

void OsmChangesetSqlFileWriter::_createWayNodes(const ConstWayPtr way)
{
  //LOG_DEBUG("way nodes create");
  //LOG_VARD(way->getId());
  const std::vector<long> nodeIds = way->getNodeIds();
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    const long nodeId = nodeIds.at(i);
    //LOG_VARD(nodeId);

    QString values =
      QString("(way_id, node_id, version, sequence_id) VALUES (%1, %2, 1, %3);\n")
        .arg(way->getId())
        .arg(nodeId)
        .arg(i + 1);
    _outputSql.write(("INSERT INTO way_nodes " + values).toUtf8());

    values =
      QString("(way_id, node_id, sequence_id) VALUES (%1, %2, %3);\n")
        .arg(way->getId())
        .arg(nodeId)
        .arg(i + 1);
    _outputSql.write(("INSERT INTO current_way_nodes " + values).toUtf8());
  }
}

void OsmChangesetSqlFileWriter::_createRelationMembers(const RelationPtr relation)
{
  //LOG_DEBUG("relation members create");
  //LOG_VARD(relation->getId());
  const vector<RelationData::Entry> members = relation->getMembers();
  for (size_t i = 0; i < members.size(); i++)
  {
    const RelationData::Entry member = members[i];
    //LOG_VARD(member.toString());

    QString values =
      QString(
        "(relation_id, member_type, member_id, member_role, version, sequence_id) VALUES (%1, '%2', %3, '%4', 1, %5);\n")
        .arg(relation->getId())
        .arg(member.getElementId().getType().toString())
        .arg(member.getElementId().getId())
        .arg(member.getRole())
        .arg(i + 1);
    _outputSql.write(("INSERT INTO relation_members " + values).toUtf8());

    values =
      QString(
        "(relation_id, member_type, member_id, member_role, sequence_id) VALUES (%1, '%2', %3, '%4', %5);\n")
        .arg(relation->getId())
        .arg(member.getElementId().getType().toString())
        .arg(member.getElementId().getId())
        .arg(member.getRole())
        .arg(i + 1);
    _outputSql.write(("INSERT INTO current_relation_members " + values).toUtf8());
  }
}

void OsmChangesetSqlFileWriter::_deleteCurrentTags(const ElementId& eid)
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
  //LOG_DEBUG("delete all" << tableName);
  _outputSql.write(
    (QString("DELETE FROM %1 WHERE %2 = %3;\n")
      .arg(tableName)
      .arg(idFieldName)
      .arg(id))
    .toUtf8());
}

}