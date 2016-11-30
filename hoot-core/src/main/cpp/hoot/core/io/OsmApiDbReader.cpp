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
#include "OsmApiDbReader.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/GeometryUtils.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>
#include <QDateTime>
#include <QSet>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, OsmApiDbReader)

OsmApiDbReader::OsmApiDbReader() :
_status(Status::Invalid),
_useDataSourceIds(true),
_open(false),
_osmElemId(-1),
_osmElemType(ElementType::Unknown)
{
  setConfiguration(conf());
}

OsmApiDbReader::~OsmApiDbReader()
{
  close();
}

void OsmApiDbReader::setBoundingBox(const QString bbox)
{
  if (!bbox.trimmed().isEmpty())
  {
    _bounds = GeometryUtils::envelopeFromConfigString(bbox);
  }
}

bool OsmApiDbReader::isSupported(QString urlStr)
{
  QUrl url(urlStr);
  return _database.isSupported(url);
}

void OsmApiDbReader::open(QString urlStr)
{
  if (!isSupported(urlStr))
  {
    throw HootException("An unsupported URL was passed into OsmApiDbReader: " + urlStr);
  }
  _elementResultIterator.reset(); //??
  _selectElementType = ElementType::Node;

  QUrl url(urlStr);
  QString osmElemId = url.queryItemValue("osm-element-id");
  QString osmElemType = url.queryItemValue("osm-element-type");
  LOG_DEBUG("url path = "+url.path());
  bool ok;

  _database.open(url);

  if(osmElemId.length() > 0 && osmElemType.length() > 0)
  {
    _osmElemId = osmElemId.toLong(&ok);
    _osmElemType = ElementType::fromString(osmElemType);
  }

  //using a transaction seems to make sense here, b/c we don't want to read a map being modified
  //in the middle of its modification caused by a changeset upload, which could cause the map to
  //be invalid as a whole
  _database.transaction();
  _open = true;
}

void OsmApiDbReader::read(shared_ptr<OsmMap> map)
{
  if (_osmElemId > -1 && _osmElemType != ElementType::Unknown)
  {
    LOG_INFO("Executing OSM API read query against element type " << _osmElemType << "...");
    _read(map, _osmElemType);
  }
  //TODO: this check is strange
  else if(_bounds.isNull() || _bounds.toString() == "-180,-90,180,90" ||
          _bounds.toString() == "-180.0,-90.0,180.0,90.0")
  {
    LOG_INFO("Executing OSM API read query...");
    for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
    {
      _read(map, static_cast<ElementType::Type>(ctr));
    }
  }
  else
  {
    LOG_INFO("Executing OSM API bounded read query with bounds " << _bounds.toString() << "...");
    _read(map, _bounds);
  }
}

void OsmApiDbReader::_parseAndSetTagsOnElement(ElementPtr element)
{
  QStringList tags;
  shared_ptr<QSqlQuery> tagItr;
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      tagItr = _database.selectTagsForNode(element->getId());
      break;
    case ElementType::Way:
      tagItr = _database.selectTagsForWay(element->getId());
      break;
    case ElementType::Relation:
      tagItr = _database.selectTagsForRelation(element->getId());
      break;
    default:
      throw HootException("Invalid element type.");
  }
  while (tagItr->next())
  {
    // test for blank tag
    QString val1 = tagItr->value(1).toString();
    QString val2 = tagItr->value(2).toString();
    QString tag = "";
    if (val1!="" || val2!="")
    {
      tag = "\""+val1+"\"=>\""+val2+"\"";
    }
    if (tag != "")
    {
      tags << tag;
    }
  }
  if (tags.size() > 0)
  {
    LOG_VART(tags);
    element->setTags(ApiDb::unescapeTags(tags.join(", ")));
    ApiDbReader::addTagsToElement(element);
  }
}

void OsmApiDbReader::_read(shared_ptr<OsmMap> map, const Envelope& bounds)
{
  LOG_DEBUG("Retrieving node records within the query bounds...");
  shared_ptr<QSqlQuery> nodeItr = _database.selectNodesByBounds(bounds);
  QStringList nodeIds;
  while (nodeItr->next())
  {
    NodePtr element = _resultToNode(*nodeItr, *map);
    LOG_VART(element->toString());
    map->addElement(element);
    nodeIds.append(QString::number(element->getId()));
  }
  LOG_VARD(nodeIds.size());

  if (nodeIds.size() > 0)
  {
    LOG_DEBUG("Retrieving way IDs referenced by the selected nodes...");
    QStringList wayIds;
    shared_ptr<QSqlQuery> wayIdItr = _database.selectWayIdsByWayNodeIds(nodeIds);
    while (wayIdItr->next())
    {
      const QString wayId = QString::number((*wayIdItr).value(0).toLongLong());
      LOG_VART(wayId);
      wayIds.append(wayId);
    }
    LOG_VARD(wayIds.size());

    if (wayIds.size() > 0)
    {
      LOG_DEBUG("Retrieving ways by way ID...");
      shared_ptr<QSqlQuery> wayItr =
        _database.selectElementsByElementIdList(wayIds, TableType::Way);
      while (wayItr->next())
      {
        WayPtr element = _resultToWay(*wayItr, *map);
        LOG_VART(element->toString());
        //I'm a little confused why this wouldn't cause a problem in that you could be writing ways
        //to the map here whose nodes haven't yet been written to the map yet.  Haven't encountered
        //he problem yet with test data, but will continue to keep an eye on it.
        map->addElement(element);
      }

      LOG_DEBUG("Retrieving way node IDs referenced by the selected ways...");
      QStringList additionalWayNodeIds;
      shared_ptr<QSqlQuery> additionalWayNodeIdItr = _database.selectWayNodeIdsByWayIds(wayIds);
      while (additionalWayNodeIdItr->next())
      {
        const QString nodeId = QString::number((*additionalWayNodeIdItr).value(0).toLongLong());
        LOG_VART(nodeId);
        additionalWayNodeIds.append(nodeId);
      }

      //subtract nodeIds from additionalWayNodeIds so no dupes get added
      const QSet<QString> nodeIdsAsSet = nodeIds.toSet();
      LOG_VARD(nodeIdsAsSet.size());
      QSet<QString> additionalWayNodeIdsAsSet = additionalWayNodeIds.toSet();
      LOG_VARD(additionalWayNodeIdsAsSet.size());
      additionalWayNodeIdsAsSet = additionalWayNodeIdsAsSet.subtract(nodeIdsAsSet);
      LOG_VARD(additionalWayNodeIdsAsSet.size());
      additionalWayNodeIds = additionalWayNodeIdsAsSet.toList();

      if (additionalWayNodeIds.size() > 0)
      {
        nodeIds.append(additionalWayNodeIds);
        LOG_DEBUG(
          "Retrieving nodes falling outside of the query bounds but belonging to a selected way...");
        shared_ptr<QSqlQuery> additionalWayNodeItr =
          _database.selectElementsByElementIdList(additionalWayNodeIds, TableType::Node);
        while (additionalWayNodeItr->next())
        {
          NodePtr element = _resultToNode(*additionalWayNodeItr, *map);
          LOG_VART(element->toString());
          map->addElement(element);
        }
      }
    }

    LOG_DEBUG("Retrieving relation IDs referenced by the selected ways and nodes...");
    QStringList relationIds;
    shared_ptr<QSqlQuery> relationIdItr =
      _database.selectRelationIdsByMemberIds(nodeIds, ElementType::Node);
    while (relationIdItr->next())
    {
      const QString relationId = QString::number((*relationIdItr).value(0).toLongLong());
      LOG_VART(relationId);
      relationIds.append(relationId);
    }
    relationIdItr = _database.selectRelationIdsByMemberIds(wayIds, ElementType::Way);
    while (relationIdItr->next())
    {
      const QString relationId = QString::number((*relationIdItr).value(0).toLongLong());
      LOG_VART(relationId);
      relationIds.append(relationId);
    }
    LOG_VARD(relationIds.size());

    if (relationIds.size() > 0)
    {
      LOG_DEBUG("Retrieving relations by relation ID...");
      shared_ptr<QSqlQuery> relationItr =
        _database.selectElementsByElementIdList(relationIds, TableType::Relation);
      while (relationItr->next())
      {
        RelationPtr element = _resultToRelation(*relationItr, *map);
        LOG_VART(element->toString());
        map->addElement(element);
      }
    }
  }
}

void OsmApiDbReader::_read(shared_ptr<OsmMap> map, const ElementType& elementType)
{
  LOG_DEBUG("IN OsmApiDbReader::read(,)...");
  long long lastId = LLONG_MIN;
  shared_ptr<Element> element;
  QStringList tags;
  bool firstElement = true;

  // contact the DB and select all
  shared_ptr<QSqlQuery> elementResultsIterator = _database.selectElements(elementType);

  assert(elementResultsIterator->isActive());

  while (elementResultsIterator->next())
  {
    long long id = elementResultsIterator->value(0).toLongLong();
    if (lastId != id)
    {
      // process the complete element only after the first element created
      if (!firstElement)
      {
        if (tags.size() > 0)
        {
          LOG_VART(tags);
          element->setTags(ApiDb::unescapeTags(tags.join(", ")) );
          ApiDbReader::addTagsToElement( element );
        }

        if (_status != Status::Invalid) { element->setStatus(_status); }
        map->addElement(element);
        tags.clear();
      }

      // extract the node contents except for the tags
      switch (elementType.getEnum())
      {
        case ElementType::Node:
          element = _resultToNode(*elementResultsIterator, *map);
          break;

        case ElementType::Way:
          element = _resultToWay(*elementResultsIterator, *map);
          break;

        case ElementType::Relation:
          element = _resultToRelation(*elementResultsIterator, *map);
          break;

        default:
          throw HootException(QString("Unexpected element type: %1").arg(elementType.toString()));
      }
      lastId = id;
      firstElement = false;
    }

    // read the tag for as many rows as there are tags
    // need to get into form "key1"=>"val1", "key2"=>"val2", ...

    QString result = _database.extractTagFromRow(elementResultsIterator, elementType.getEnum());
    if (result != "") tags << result;
  }

  // process the last complete element only if an element has been created
  if (!firstElement)
  {
    if (tags.size() > 0)
    {
      LOG_VART(tags);
      element->setTags(ApiDb::unescapeTags(tags.join(", ")));
      ApiDbReader::addTagsToElement( element );
    }
    if (_status != Status::Invalid) { element->setStatus(_status); }
    map->addElement(element);
    tags.clear();
  }

  LOG_DEBUG("LEAVING OsmApiDbReader::_read...");
}

void OsmApiDbReader::close()
{
  if (_open)
  {
    _database.commit();
    _database.close();
    _open = false;
  }
}

ElementId OsmApiDbReader::_mapElementId(const OsmMap& map, ElementId oldId)
{
  ElementId result;
  if (_useDataSourceIds)
  {
    result = oldId;
  }
  else
  {
    long id = oldId.getId();
    switch (oldId.getType().getEnum())
    {
    case ElementType::Node:
      if (_nodeIdMap.count(id) > 0)
      {
        result = ElementId::node(_nodeIdMap.at(id));
      }
      else
      {
        long newId = map.createNextNodeId();
        _nodeIdMap[id] = newId;
        result = ElementId::node(newId);
      }
      break;
    case ElementType::Way:
      if (_wayIdMap.count(id) > 0)
      {
        result = ElementId::way(_wayIdMap.at(id));
      }
      else
      {
        long newId = map.createNextWayId();
        _wayIdMap[id] = newId;
        result = ElementId::way(newId);
      }
      break;
    case ElementType::Relation:
      if (_relationIdMap.count(id) > 0)
      {
        result = ElementId::relation(_relationIdMap.at(id));
      }
      else
      {
        long newId = map.createNextRelationId();
        _relationIdMap[id] = newId;
        result = ElementId::relation(newId);
      }
      break;
    default:
      throw IllegalArgumentException("Expected a valid element type, but got: " +
        QString::number(oldId.getType().getEnum()));
    }
  }

  return result;
}

shared_ptr<Node> OsmApiDbReader::_resultToNode(const QSqlQuery& resultIterator, OsmMap& map)
{
  const long rawId = resultIterator.value(0).toLongLong();
  LOG_TRACE("raw ID: " << rawId);
  long nodeId = _mapElementId(map, ElementId::node(rawId)).getId();
  LOG_VART(nodeId);
  double lat =
    resultIterator.value(ApiDb::NODES_LATITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;
  double lon =
    resultIterator.value(ApiDb::NODES_LONGITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;

  shared_ptr<Node> node(
    new Node(
      _status,
      nodeId,
      lon,
      lat,
      ConfigOptions().getCircularErrorDefaultValue(),
      resultIterator.value(ApiDb::NODES_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::NODES_VERSION).toLongLong(),
      resultIterator.value(ApiDb::NODES_TIMESTAMP).toUInt()));

  _parseAndSetTagsOnElement(node);
//  if (_status != Status::Invalid)
//  {
//    node->setStatus(_status);
//  }

  LOG_VART(node);
  return node;
}

shared_ptr<Way> OsmApiDbReader::_resultToWay(const QSqlQuery& resultIterator, OsmMap& map)
{
  const long wayId = resultIterator.value(0).toLongLong();
  LOG_TRACE("raw ID: " << wayId);
  const long newWayId = _mapElementId(map, ElementId::way(wayId)).getId();
  LOG_TRACE("Reading way with ID: " << wayId);
  if (newWayId != wayId)
  {
    LOG_VARD(newWayId);
  }

  shared_ptr<Way> way(
    new Way(
      _status,
      newWayId,
      ConfigOptions().getCircularErrorDefaultValue(),
      resultIterator.value(ApiDb::WAYS_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::WAYS_VERSION).toLongLong(),
      resultIterator.value(ApiDb::WAYS_TIMESTAMP).toUInt()));

  // if performance becomes an issue, try reading these out in batch at the same time
  // the element results are read
  vector<long> nodeIds = _database.selectNodeIdsForWay(wayId);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    nodeIds[i] = _mapElementId(map, ElementId::node(nodeIds[i])).getId();
  }
  way->addNodes(nodeIds);

  _parseAndSetTagsOnElement(way);

  _addNodesForWay(nodeIds, map);

  LOG_VART(way);
  return way;
}

void OsmApiDbReader::_addNodesForWay(vector<long> nodeIds, OsmMap& map)
{
  for (unsigned int i = 0; i < nodeIds.size(); i++)
  {
    QStringList tags;
    if (map.containsNode(nodeIds[i]) == false)
    {
      shared_ptr<QSqlQuery> queryIterator = _database.selectNodeById(nodeIds[i]);
      while (queryIterator->next())
      {
        shared_ptr<Node> node = _resultToNode(*queryIterator.get(), map);
        QString result = _database.extractTagFromRow(queryIterator, ElementType::Node);
        if(result != "")
        {
          tags << result;
        }

        if(tags.size()>0)
        {
          LOG_VART(tags);
          node->setTags(ApiDb::unescapeTags(tags.join(", ")));
          ApiDbReader::addTagsToElement(node);
        }
        if (_status != Status::Invalid)
        {
          node->setStatus(_status);
        }
        map.addElement(node);
      }
    }
  }
}

shared_ptr<Relation> OsmApiDbReader::_resultToRelation(const QSqlQuery& resultIterator,
  const OsmMap& map)
{
  const long relationId = resultIterator.value(0).toLongLong();
  LOG_TRACE("raw ID: " << relationId);
  const long newRelationId = _mapElementId(map, ElementId::relation(relationId)).getId();
  LOG_TRACE("Reading relation with ID: " << relationId);
  if (newRelationId != relationId)
  {
    LOG_VART(newRelationId);
  }

  shared_ptr<Relation> relation(
    new Relation(
      _status,
      newRelationId,
      ConfigOptions().getCircularErrorDefaultValue(),
      "",
      resultIterator.value(ApiDb::RELATIONS_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::RELATIONS_VERSION).toLongLong(),
      resultIterator.value(ApiDb::RELATIONS_TIMESTAMP).toUInt()));

  _parseAndSetTagsOnElement(relation);

  // These could be read these out in batch at the same time the element results are read.
  vector<RelationData::Entry> members = _database.selectMembersForRelation(relationId);
  for (size_t i = 0; i < members.size(); ++i)
  {
    members[i].setElementId(_mapElementId(map, members[i].getElementId()));
  }
  relation->setMembers(members);

  LOG_VART(relation);
  return relation;
}

void OsmApiDbReader::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setUserEmail(configOptions.getOsmapiDbReaderEmail());
  setBoundingBox(configOptions.getConvertBoundingBox());
}

boost::shared_ptr<OGRSpatialReference> OsmApiDbReader::getProjection() const
{
  boost::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }

  return wgs84;
}

}
