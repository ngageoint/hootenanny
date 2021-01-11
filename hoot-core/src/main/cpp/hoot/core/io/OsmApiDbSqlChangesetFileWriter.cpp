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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmApiDbSqlChangesetFileWriter.h"

// hoot
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/util/ConfigUtils.h>

// Qt
#include <QSqlError>
#include <QSqlQuery>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmChangesetFileWriter, OsmApiDbSqlChangesetFileWriter)

OsmApiDbSqlChangesetFileWriter::OsmApiDbSqlChangesetFileWriter() :
_changesetId(0),
_includeDebugTags(false),
_includeCircularErrorTags(false),
_changesetIgnoreBounds(false)
{
}

OsmApiDbSqlChangesetFileWriter::OsmApiDbSqlChangesetFileWriter(const QUrl& url) :
_changesetId(0),
_includeDebugTags(false),
_includeCircularErrorTags(false),
_changesetIgnoreBounds(false)
{
  _db.open(url);
}

OsmApiDbSqlChangesetFileWriter::~OsmApiDbSqlChangesetFileWriter()
{
  _db.close();
}

void OsmApiDbSqlChangesetFileWriter::setConfiguration(const Settings &conf)
{
  ConfigOptions co(conf);
  _changesetUserId = co.getChangesetUserId();
  _includeDebugTags = co.getWriterIncludeDebugTags();
  _includeCircularErrorTags = co.getWriterIncludeCircularErrorTags();
  _metadataAllowKeys = co.getChangesetMetadataAllowedTagKeys();
  _changesetIgnoreBounds = co.getChangesetIgnoreBounds();
}

void OsmApiDbSqlChangesetFileWriter::write(
  const QString& path, const ChangesetProviderPtr& changesetProvider)
{
  QList<ChangesetProviderPtr> changesetProviders;
  changesetProviders.append(changesetProvider);
  write(path, changesetProviders);
}

void OsmApiDbSqlChangesetFileWriter::write(
  const QString& path, const QList<ChangesetProviderPtr>& changesetProviders)
{
  LOG_DEBUG("Writing changeset to: " << path << "...");

  LOG_VARD(_map1List.size());
  LOG_VARD(_map2List.size());
  assert(_map1List.size() == _map2List.size());

  _remappedIds.clear();
  _changesetBounds.init();
  _parsedChangeIds.clear();

  _outputSql.setFileName(path);
  if (_outputSql.open(QIODevice::WriteOnly | QIODevice::Text) == false)
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(path));
  }

  int changes = 0;
  _createChangeSet();

  for (int i = 0; i < changesetProviders.size(); i++)
  {
    LOG_DEBUG(
      "Deriving changes with changeset provider: " << i + 1 << " / " << changesetProviders.size() <<
      "...");

    // TODO
    ConstOsmMapPtr map1;
    ConstOsmMapPtr map2;
    std::shared_ptr<InBoundsCriterion> boundsCrit1;
    std::shared_ptr<InBoundsCriterion> boundsCrit2;
    if (ConfigUtils::boundsOptionEnabled())
    {
      map1 = _map1List.at(i);
      LOG_VART(map1.get());
      map2 = _map2List.at(i);
      LOG_VART(map2.get());

      if (map1)
      {
        boundsCrit1 = ConfigUtils::getBoundsCrit(map1);
      }
      if (map2)
      {
        boundsCrit2 = ConfigUtils::getBoundsCrit(map2);
      }
    }

    ChangesetProviderPtr changesetProvider = changesetProviders.at(i);
    LOG_VART(changesetProvider.get());
    LOG_VART(changesetProvider->hasMoreChanges());
    while (changesetProvider->hasMoreChanges())
    {
      LOG_TRACE("Reading next SQL change...");
      Change change = changesetProvider->readNextChange();
      LOG_VART(change.toString());

      if (!change.getElement())
      {
        continue;
      }
      // See related note in OsmXmlChangesetFileWriter::write.
      else if (_parsedChangeIds.contains(change.getElement()->getElementId()))
      {
        LOG_TRACE("Skipping change for element ID already having change: " << change << "...");
        continue;
      }
      // TODO
      else if (!_changesetIgnoreBounds && ConfigUtils::boundsOptionEnabled())
      {
        std::shared_ptr<InBoundsCriterion> boundsCrit;
        if (map2 && boundsCrit2 && map2->containsElement(change.getElement()))
        {
          boundsCrit = boundsCrit2;
        }
        else if (map1 && boundsCrit1 && map1->containsElement(change.getElement()))
        {
          boundsCrit = boundsCrit1;
        }
        else
        {
          //throw HootException(
          // change.getElement()->getElementId().toString() + " not found in map for bounds check.");
          LOG_DEBUG(change.getElement()->getElementId() << " not found in map for bounds check.");
        }
        LOG_VART(boundsCrit.get());
        if (boundsCrit)
        {
          LOG_VART(boundsCrit->isSatisfied(change.getElement()));
        }
        if (boundsCrit && !boundsCrit->isSatisfied(change.getElement()))
        {
          continue;
        }
      }

      LOG_VART(change.getType());
      switch (change.getType())
      {
        case Change::Create:
          _createNewElement(change.getElement());
          break;
        case Change::Modify:
          _updateExistingElement(change.getElement());
          break;
        case Change::Delete:
          _deleteExistingElement(change.getElement());
          break;
        case Change::Unknown:
          // see comment in ChangesetDeriver::_nextChange() when
          // _fromE->getElementId() < _toE->getElementId() as to why we do a no-op here.
          break;
        default:
          throw IllegalArgumentException("Unexpected change type.");
      }

      if (change.getType() != Change::Unknown)
      {
        if (change.getElement()->getElementType().getEnum() == ElementType::Node)
        {
          ConstNodePtr node = std::dynamic_pointer_cast<const Node>(change.getElement());
          _changesetBounds.expandToInclude(node->getX(), node->getY());
        }
        _parsedChangeIds.append(change.getElement()->getElementId());
        changes++;
      }
    }

    boundsCrit1.reset();
    boundsCrit2.reset();
  }

  _updateChangeset(changes);

  _outputSql.close();
}

void OsmApiDbSqlChangesetFileWriter::_updateChangeset(const int numChanges)
{
  //update the changeset's bounds
  LOG_DEBUG("Updating changeset's bounds: " << _changesetBounds.toString());
  LOG_VARD(numChanges);
  _outputSql.write(
    QString("UPDATE %1 SET min_lat=%2, max_lat=%3, min_lon=%4, max_lon=%5, num_changes=%6 WHERE id=%7;\n")
      .arg(ApiDb::getChangesetsTableName())
      .arg((qlonglong)OsmApiDb::toOsmApiDbCoord(_changesetBounds.getMinY()))
      .arg((qlonglong)OsmApiDb::toOsmApiDbCoord(_changesetBounds.getMaxY()))
      .arg((qlonglong)OsmApiDb::toOsmApiDbCoord(_changesetBounds.getMinX()))
      .arg((qlonglong)OsmApiDb::toOsmApiDbCoord(_changesetBounds.getMaxX()))
      .arg(numChanges)
      .arg(_changesetId)
    .toUtf8());

   _changesetBounds.init();
}

void OsmApiDbSqlChangesetFileWriter::_createChangeSet()
{
  if (_changesetUserId == -1)
  {
    throw HootException("Invalid changeset user ID: " + QString::number(_changesetUserId));
  }

  LOG_DEBUG("Getting changeset ID...");
  _changesetId = _db.getNextId(ApiDb::getChangesetsTableName());
  LOG_DEBUG("Creating changeset with ID: " << _changesetId);
  _outputSql.write(
    QString("INSERT INTO %1 (id, user_id, created_at, closed_at) VALUES "
            "(%2, %3, %4, %4);\n")
      .arg(ApiDb::getChangesetsTableName())
      .arg(_changesetId)
      .arg(_changesetUserId)
      .arg(OsmApiDb::TIMESTAMP_FUNCTION)
    .toUtf8());
  _outputSql.write(
    QString("INSERT INTO %1 (changeset_id, k, v) VALUES "
            "(%2, '%3', '%4');\n")
      .arg(ApiDb::getChangesetTagsTableName())
      .arg(_changesetId)
      .arg("written_by")
      .arg("Hootenanny")
    .toUtf8());
}

ElementPtr OsmApiDbSqlChangesetFileWriter::_getChangeElement(ConstElementPtr element)
{
  ElementPtr changeElement;
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      changeElement.reset(new Node(*std::dynamic_pointer_cast<const Node>(element)));
      break;
    case ElementType::Way:
      changeElement.reset(new Way(*std::dynamic_pointer_cast<const Way>(element)));
      break;
   case ElementType::Relation:
      changeElement.reset(new Relation(*std::dynamic_pointer_cast<const Relation>(element)));
      break;
    default:
      throw HootException("Unknown element type");
  }
  return changeElement;
}

void OsmApiDbSqlChangesetFileWriter::_createNewElement(ConstElementPtr element)
{
  const QString elementTypeStr = element->getElementType().toString().toLower();
  ElementPtr changeElement = _getChangeElement(element);

  // We always remap IDs for create statements to mimic the behavior of creation via public OSM API.
  // and avoid issuing a create statement for an existing element ID. We can't simply set negative
  // IDs here, as there's no remapping procedure in OsmApiDbSqlChangesetApplier to change them to
  // positive and negative IDs aren't allowed in the db. Arguably, we could move the remapping to
  // OsmApiDbSqlChangesetApplier, but that would make debugging more difficult when viewing a
  // changeset file with mismatching IDs compared to what was actually written to the db.
  LOG_TRACE("ID before: " << changeElement->getElementId());
  const long id = _db.getNextId(element->getElementType().getEnum());
  _remappedIds[changeElement->getElementId()] = ElementId(changeElement->getElementType(), id);
  LOG_TRACE("ID after: " << ElementId(changeElement->getElementType(), id));

  changeElement->setId(id);
  changeElement->setVersion(1);
  changeElement->setVisible(true);
  changeElement->setChangeset(_changesetId);
  LOG_TRACE("Creating: " << changeElement->getElementId());

  QString note = "";
  LOG_VART(changeElement->getId());
  LOG_VART(note);
  LOG_VART(changeElement->getVersion());
  QString commentStr = "/* create " + elementTypeStr + " " + QString::number(changeElement->getId());
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  const QString values = _getInsertValuesStr(changeElement);
  _outputSql.write(
    ("INSERT INTO " + elementTypeStr + "s (" + elementTypeStr + "_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_" + elementTypeStr + "s (id, " + values).toUtf8());

  _createTags(changeElement);

  switch (changeElement->getElementType().getEnum())
  {
    case ElementType::Way:
      _createWayNodes(std::dynamic_pointer_cast<const Way>(changeElement));
      break;
    case ElementType::Relation:
      _createRelationMembers(std::dynamic_pointer_cast<const Relation>(changeElement));
      break;
    default:
      // node
      break;
  }
}

QString OsmApiDbSqlChangesetFileWriter::_getUpdateValuesStr(ConstElementPtr element) const
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      return _getUpdateValuesNodeStr(std::dynamic_pointer_cast<const Node>(element));
    case ElementType::Way:
      return _getUpdateValuesWayOrRelationStr(element);
    case ElementType::Relation:
      return _getUpdateValuesWayOrRelationStr(element);
    default:
      throw HootException("Unknown element type");
  }
}

QString OsmApiDbSqlChangesetFileWriter::_getUpdateValuesNodeStr(ConstNodePtr node) const
{
  return
    QString("latitude=%2, longitude=%3, changeset_id=%4, visible=%5, \"timestamp\"=%8, tile=%6, version=%7 WHERE id=%1;\n")
      .arg(node->getId())
      .arg((qlonglong)OsmApiDb::toOsmApiDbCoord(node->getY()))
      .arg((qlonglong)OsmApiDb::toOsmApiDbCoord(node->getX()))
      .arg(node->getChangeset())
      .arg(_getVisibleStr(node->getVisible()))
      .arg(ApiDb::tileForPoint(node->getY(), node->getX()))
      .arg(node->getVersion())
      .arg(OsmApiDb::TIMESTAMP_FUNCTION);
}

QString OsmApiDbSqlChangesetFileWriter::_getUpdateValuesWayOrRelationStr(ConstElementPtr element) const
{
  return
    QString("changeset_id=%2, visible=%3, \"timestamp\"=%5, version=%4 WHERE id=%1;\n")
      .arg(element->getId())
      .arg(element->getChangeset())
      .arg(_getVisibleStr(element->getVisible()))
      .arg(element->getVersion())
      .arg(OsmApiDb::TIMESTAMP_FUNCTION);
}

void OsmApiDbSqlChangesetFileWriter::_updateExistingElement(ConstElementPtr element)
{
  LOG_TRACE("Writing update for: " << element->getElementId() << "...");
  const QString elementTypeStr = element->getElementType().toString().toLower();
  ElementPtr changeElement = _getChangeElement(element);

  // If another parsed change previously modified the element with this id, we want to get the
  // modified version.
  const long currentVersion = changeElement->getVersion();
  LOG_VART(currentVersion);
  if (currentVersion < 1)
  {
    throw HootException(
      "Elements being modified in an .osc.sql changeset must always have a version greater than one: " +
      element->getElementId().toString());
  }
  const long newVersion = currentVersion + 1;
  LOG_VART(newVersion);

  changeElement->setVersion(newVersion);
  LOG_VART(changeElement->getVersion());
  changeElement->setChangeset(_changesetId);
  changeElement->setVisible(true);
  LOG_TRACE("Updating: " << changeElement->getElementId());

  QString note = "";
  LOG_VART(changeElement->getId());
  LOG_VART(note);
  LOG_VART(changeElement->getVersion());
  QString commentStr = "/* modify " + elementTypeStr + " " + QString::number(changeElement->getId());
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  // <element-name> table contains all version of all elements of that type in a history, so insert
  // into that table.
  _outputSql.write(
    ("INSERT INTO " + elementTypeStr + "s (" + elementTypeStr + "_id, " +
     _getInsertValuesStr(changeElement)).toUtf8());
  // current_<element-name> contains the single latest version of the element, so update that record
  _outputSql.write(
    ("UPDATE current_" + elementTypeStr + "s SET " + _getUpdateValuesStr(changeElement)).toUtf8());

  _deleteCurrentTags(changeElement->getElementId());
  _createTags(changeElement);

  switch (changeElement->getElementType().getEnum())
  {
    case ElementType::Way:
      _deleteAll(ApiDb::getCurrentWayNodesTableName(), "way_id", changeElement->getId());
      _deleteAll(ApiDb::getWayNodesTableName(), "way_id", changeElement->getId());
      _createWayNodes(std::dynamic_pointer_cast<const Way>(changeElement));
      break;
    case ElementType::Relation:
      _deleteAll(ApiDb::getCurrentRelationMembersTableName(), "relation_id", changeElement->getId());
      _deleteAll(ApiDb::getRelationMembersTableName(), "relation_id", changeElement->getId());
      _createRelationMembers(std::dynamic_pointer_cast<const Relation>(changeElement));
      break;
    default:
      // node
      break;
  }
}

void OsmApiDbSqlChangesetFileWriter::_deleteExistingElement(ConstElementPtr element)
{
  const QString elementIdStr = QString::number(element->getId());
  const QString elementTypeStr = element->getElementType().toString().toLower();
  ElementPtr changeElement = _getChangeElement(element);

  const long currentVersion = changeElement->getVersion();
  if (currentVersion < 1)
  {
    throw HootException(
      "Elements being deleted in an .osc.sql changeset must always have a version greater than one: " +
      element->getElementId().toString());
  }
  const long newVersion = currentVersion + 1;

  changeElement->setVersion(newVersion);
  changeElement->setVisible(false);
  changeElement->setChangeset(_changesetId);
  LOG_TRACE("Deleting: " << changeElement->getElementId());

  QString note = "";
  LOG_VART(changeElement->getId());
  LOG_VART(note);
  LOG_VART(changeElement->getVersion());
  QString commentStr = "/* delete " + elementTypeStr + " " + QString::number(changeElement->getId());
  commentStr += "*/\n";
  _outputSql.write((commentStr).toUtf8());

  //OSM API DB keeps history for all elements, so the existing element in the master table is not
  //modified and a new record is added with the updated version and visible set to false
  _outputSql.write(("INSERT INTO " + elementTypeStr + "s (" + elementTypeStr + "_id, " +
                    _getInsertValuesStr(changeElement)).toUtf8());

  _deleteCurrentTags(changeElement->getElementId());

  switch (changeElement->getElementType().getEnum())
  {
    case ElementType::Node:

      _outputSql.write(
        ("DELETE FROM " + ApiDb::getCurrentWayNodesTableName() +
         " WHERE node_id=" + elementIdStr + ";\n").toUtf8());
      _outputSql.write(
        ("DELETE FROM " + ApiDb::getCurrentRelationMembersTableName() +
         " WHERE member_type = 'Node' AND member_id = " +
         elementIdStr + ";\n").toUtf8());

      break;

    case ElementType::Way:

      //all of its entries in current way nodes are removed
      _outputSql.write(
        ("DELETE FROM " + ApiDb::getCurrentWayNodesTableName() +
         " WHERE way_id=" + elementIdStr + ";\n").toUtf8());
      _outputSql.write(
        ("DELETE FROM " + ApiDb::getCurrentRelationMembersTableName() +
         " WHERE member_type = 'Way' AND member_id = " +
         elementIdStr + ";\n").toUtf8());

      break;

    case ElementType::Relation:

      _outputSql.write(
        ("DELETE FROM " + ApiDb::getCurrentRelationMembersTableName() +
         " WHERE relation_id=" + elementIdStr + ";\n").toUtf8());
      _outputSql.write(
        ("DELETE FROM " + ApiDb::getCurrentRelationMembersTableName() +
         " WHERE member_type = 'Relation' AND member_id = " +
         elementIdStr + ";\n").toUtf8());

      break;

    default:
      throw HootException("Unknown element type");
  }

  //in the current table, the element isn't deleted but set to be invisible
  const QString values =
    QString("changeset_id=%1, visible=%2, version=%3 WHERE id=%4;\n")
      .arg(changeElement->getChangeset())
      .arg(_getVisibleStr(changeElement->getVisible()))
      .arg(changeElement->getVersion())
      .arg(changeElement->getId());
  _outputSql.write(("UPDATE current_" + elementTypeStr + "s SET " + values).toUtf8());
}

QString OsmApiDbSqlChangesetFileWriter::_getInsertValuesStr(ConstElementPtr element) const
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      return _getInsertValuesNodeStr(std::dynamic_pointer_cast<const Node>(element));
    case ElementType::Way:
      return _getInsertValuesWayOrRelationStr(element);
    case ElementType::Relation:
      return _getInsertValuesWayOrRelationStr(element);
    default:
      throw HootException("Unknown element type");
  }
}

QString OsmApiDbSqlChangesetFileWriter::_getInsertValuesNodeStr(ConstNodePtr node) const
{
  return
    QString("latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile, version) VALUES (%1, %2, %3, %4, %5, %8, %6, %7);\n")
      .arg(node->getId())
      .arg((qlonglong)OsmApiDb::toOsmApiDbCoord(node->getY()))
      .arg((qlonglong)OsmApiDb::toOsmApiDbCoord(node->getX()))
      .arg(node->getChangeset())
      .arg(_getVisibleStr(node->getVisible()))
      .arg(ApiDb::tileForPoint(node->getY(), node->getX()))
      .arg(node->getVersion())
      .arg(OsmApiDb::TIMESTAMP_FUNCTION);
}

QString OsmApiDbSqlChangesetFileWriter::_getInsertValuesWayOrRelationStr(
  ConstElementPtr element) const
{
  return
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, %3, %5, %4);\n")
      .arg(element->getId())
      .arg(element->getChangeset())
      .arg(_getVisibleStr(element->getVisible()))
      .arg(element->getVersion())
      .arg(OsmApiDb::TIMESTAMP_FUNCTION);
}

void OsmApiDbSqlChangesetFileWriter::_createTags(ConstElementPtr element)
{
  LOG_TRACE("Creating tags for: " << element->getElementId());

  QStringList tableNames = _tagTableNamesForElement(element->getElementId());

  Tags tags = element->getTags();
  if (_includeDebugTags)
  {
    tags.set(MetadataTags::HootId(), QString::number(element->getId()));
    tags.set(MetadataTags::HootStatus(), QString::number(element->getStatus().getEnum()));
  }
  LOG_VART(tags);
  if (element->getElementType().getEnum() == ElementType::Relation && !tags.contains("type"))
  {
    ConstRelationPtr tmp = std::dynamic_pointer_cast<const Relation>(element);
    tags.appendValue("type", tmp->getType());
  }

  // These should never be written in a changeset, even when debug tags are enabled, and will cause
  // problems in ChangesetReplacementCreator if added to source data when read back out.
  QStringList metadataAlwaysIgnore;
  metadataAlwaysIgnore.append(MetadataTags::HootHash());
  metadataAlwaysIgnore.append(MetadataTags::HootChangeExcludeDelete());
  metadataAlwaysIgnore.append(MetadataTags::HootConnectedWayOutsideBounds());
  metadataAlwaysIgnore.append(MetadataTags::HootSnapped());

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString key = it.key();
    QString val = it.value();

    if (key.isEmpty() == false && val.isEmpty() == false)
    {
      if (metadataAlwaysIgnore.contains(key))
        continue;
      else if (!_includeDebugTags && key.toLower().startsWith("hoot:") &&
               // There are some instances where we want to explicitly allow some metadata tags.
               !_metadataAllowKeys.contains(key))
        continue;
      //  Don't include the hoot hash, also don't include the circular error tag
      //  unless circular error tags AND debug tags are turned on
      else if (key == MetadataTags::ErrorCircular() &&
               !(_includeCircularErrorTags || _includeDebugTags))
        continue;

      const QString currentTagValues =
      QString("(%1_id, k, v) VALUES (%2, '%3', '%4');\n")
        .arg(element->getElementId().getType().toString().toLower())
        .arg(element->getElementId().getId())
        .arg(key.replace('\'', "''"))
        .arg(val.replace('\'', "''"));

      const QString tagValues =
        QString("(%1_id, k, v, version) VALUES (%2, '%3', '%4', %5);\n")
          .arg(element->getElementId().getType().toString().toLower())
          .arg(element->getElementId().getId())
          .arg(key.replace('\'', "''"))
          .arg(val.replace('\'', "''"))
          .arg(element->getVersion());

      _outputSql.write(
        (QString("INSERT INTO %1 ").arg(tableNames.at(0)) + currentTagValues).toUtf8());
      _outputSql.write((QString("INSERT INTO %1 ").arg(tableNames.at(1)) + tagValues).toUtf8());
    }
  }
}

QStringList OsmApiDbSqlChangesetFileWriter::_tagTableNamesForElement(const ElementId& eid) const
{
  QStringList tableNames;
  const QString tableName1 = "current_" + eid.getType().toString().toLower() + "_tags";
  tableNames.append(tableName1);
  const QString tableName2 = eid.getType().toString().toLower() + "_tags";
  tableNames.append(tableName2);
  return tableNames;
}

void OsmApiDbSqlChangesetFileWriter::_createWayNodes(ConstWayPtr way)
{
  LOG_TRACE("Creating way nodes for: " << way->getElementId());

  const std::vector<long> nodeIds = way->getNodeIds();
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    // If this was a newly created node its id was remapped when it was created, but the way still
    // has the old way node id.
    ElementId nodeElementId = ElementId(ElementType::Node, nodeIds.at(i));
    LOG_VART(nodeElementId);
    if (_remappedIds.contains(nodeElementId))
    {
      nodeElementId = _remappedIds[nodeElementId];
    }
    assert(nodeElementId.getId() > 0);
    LOG_VART(nodeElementId);

    QString values =
      QString("(way_id, node_id, version, sequence_id) VALUES (%1, %2, 1, %3);\n")
        .arg(way->getId())
        .arg(nodeElementId.getId())
        .arg(i + 1);
    _outputSql.write(("INSERT INTO " + ApiDb::getWayNodesTableName() + " " + values).toUtf8());

    values =
      QString("(way_id, node_id, sequence_id) VALUES (%1, %2, %3);\n")
        .arg(way->getId())
        .arg(nodeElementId.getId())
        .arg(i + 1);
    _outputSql.write(("INSERT INTO " + ApiDb::getCurrentWayNodesTableName() + " " + values).toUtf8());
  }
}

void OsmApiDbSqlChangesetFileWriter::_createRelationMembers(ConstRelationPtr relation)
{
  LOG_TRACE("Creating relation members for: " << relation->getElementId());

  const vector<RelationData::Entry> members = relation->getMembers();
  for (size_t i = 0; i < members.size(); i++)
  {
    const RelationData::Entry member = members[i];
    LOG_VART(member.getElementId());
    // If the member was a newly created element its id was remapped when it was created, but the
    // relation still has the old element id.
    ElementId memberElementId = member.getElementId();
    LOG_VART(memberElementId);
    if (_remappedIds.contains(memberElementId))
    {
      memberElementId = _remappedIds[memberElementId];
    }
    assert(memberElementId.getId() > 0);
    LOG_VART(memberElementId);

    QString values =
      QString(
        "(relation_id, member_type, member_id, member_role, version, sequence_id) VALUES (%1, '%2', %3, '%4', 1, %5);\n")
        .arg(relation->getId())
        .arg(memberElementId.getType().toString())
        .arg(memberElementId.getId())
        .arg(member.getRole())
        .arg(i + 1);
    _outputSql.write(("INSERT INTO " + ApiDb::getRelationMembersTableName() + " " + values).toUtf8());

    values =
      QString(
        "(relation_id, member_type, member_id, member_role, sequence_id) VALUES (%1, '%2', %3, '%4', %5);\n")
        .arg(relation->getId())
        .arg(memberElementId.getType().toString())
        .arg(memberElementId.getId())
        .arg(member.getRole())
        .arg(i + 1);
    _outputSql.write(
      ("INSERT INTO " + ApiDb::getCurrentRelationMembersTableName() + " " + values).toUtf8());
  }
}

void OsmApiDbSqlChangesetFileWriter::_deleteCurrentTags(const ElementId& eid)
{
  LOG_TRACE("Deleting tags for: " << eid);
  QStringList tableNames = _tagTableNamesForElement(eid);
  foreach (QString tableName, tableNames)
  {
    QString idFieldName = tableName;
    idFieldName.replace("current_", "").replace("_tags", "");
    idFieldName += "_id";
    _deleteAll(tableName, idFieldName, eid.getId());
  }
}

void OsmApiDbSqlChangesetFileWriter::_deleteAll(const QString& tableName, const QString& idFieldName,
                                           const long id)
{
  LOG_TRACE("Deleting all from: " << tableName << "...");
  _outputSql.write(
    (QString("DELETE FROM %1 WHERE %2 = %3;\n")
      .arg(tableName)
      .arg(idFieldName)
      .arg(id))
    .toUtf8());
}

}
