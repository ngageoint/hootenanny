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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmApiDbReader.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/util/GeometryUtils.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, OsmApiDbReader)

OsmApiDbReader::OsmApiDbReader() :
_database(new OsmApiDb()),
_osmElemId(-1),
_osmElemType(ElementType::Unknown)
{
  setConfiguration(conf());
}

OsmApiDbReader::~OsmApiDbReader()
{
  close();
}

bool OsmApiDbReader::isSupported(QString urlStr)
{
  QUrl url(urlStr);
  return _database->isSupported(url);
}

void OsmApiDbReader::open(QString urlStr)
{
  if (!isSupported(urlStr))
  {
    throw HootException("An unsupported URL was passed into OsmApiDbReader: " + urlStr);
  }
  _elementResultIterator.reset();
  _selectElementType = ElementType::Node;

  QUrl url(urlStr);
  QString osmElemId = url.queryItemValue("osm-element-id");
  QString osmElemType = url.queryItemValue("osm-element-type");
  LOG_DEBUG("url path = "+url.path());
  bool ok;

  _database->open(url);

  if (osmElemId.length() > 0 && osmElemType.length() > 0)
  {
    _osmElemId = osmElemId.toLong(&ok);
    _osmElemType = ElementType::fromString(osmElemType);
  }

  //using a transaction seems to make sense here, b/c we don't want to read a map being modified
  //in the middle of its modification caused by a changeset upload, which could cause the map to
  //be invalid as a whole
  _database->transaction();
  _open = true;
}

void OsmApiDbReader::read(boost::shared_ptr<OsmMap> map)
{
  if (_osmElemId > -1 && _osmElemType != ElementType::Unknown)
  {
    LOG_DEBUG("Executing OSM API read query against element type " << _osmElemType << "...");
    _read(map, _osmElemType);
  }
  else if (!_hasBounds())
  {
    LOG_INFO("Executing OSM API read query...");
    for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
    {
      _read(map, static_cast<ElementType::Type>(ctr));
    }
  }
  else
  {
    Envelope bounds;
    if (!_overrideBounds.isNull())
    {
      bounds = _overrideBounds;
    }
    else
    {
      bounds = _bounds;
    }
    LOG_DEBUG("Executing OSM API bounded read query with bounds " << bounds.toString() << "...");
    _readByBounds(map, bounds);
  }
}

void OsmApiDbReader::_parseAndSetTagsOnElement(ElementPtr element)
{
  QStringList tags;
  boost::shared_ptr<QSqlQuery> tagItr;
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      tagItr = _database->selectTagsForNode(element->getId());
      break;
    case ElementType::Way:
      tagItr = _database->selectTagsForWay(element->getId());
      break;
    case ElementType::Relation:
      tagItr = _database->selectTagsForRelation(element->getId());
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
    element->setTags(ApiDb::unescapeTags(tags.join(", ")));
  }
}

//TODO: _read could possibly be placed by the bounded read method set to a global extent...unless
//this read performs better for some reason
void OsmApiDbReader::_read(boost::shared_ptr<OsmMap> map, const ElementType& elementType)
{
  long elementCount = 0; //TODO: break this out by element type
  long long lastId = LLONG_MIN;
  boost::shared_ptr<Element> element;
  QStringList tags;
  bool firstElement = true;

  // select all
  boost::shared_ptr<QSqlQuery> elementResultsIterator = _database->selectElements(elementType);

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
          //LOG_VART(tags);
          element->setTags(ApiDb::unescapeTags(tags.join(", ")));
          _updateMetadataOnElement(element);
        }
        if (_status != Status::Invalid) { element->setStatus(_status); }
        LOG_VART(element);
        map->addElement(element);
        elementCount++;
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

    const QString result =
      _database->extractTagFromRow(elementResultsIterator, elementType.getEnum());
    if (result != "") tags << result;
  }

  // process the last complete element only if an element has been created
  if (!firstElement)
  {
    if (tags.size() > 0)
    {
      //LOG_VART(tags);
      element->setTags(ApiDb::unescapeTags(tags.join(", ")));
      _updateMetadataOnElement(element);
    }
    if (_status != Status::Invalid) { element->setStatus(_status); }
    LOG_VART(element);
    map->addElement(element);
    elementCount++;
    tags.clear();
  }

  LOG_DEBUG("Select all query read " << elementCount << " " << elementType.toString() <<
            " elements.");
  LOG_DEBUG("Current map:");
  LOG_VARD(map->getNodes().size());
  LOG_VARD(map->getWays().size());
  LOG_VARD(map->getRelations().size());
}

void OsmApiDbReader::close()
{
  if (_open)
  {
    //See comment in HootApiDbReader::finalizePartial.
    _database->commit();
    _database->close();
    _open = false;
  }
}

NodePtr OsmApiDbReader::_resultToNode(const QSqlQuery& resultIterator, OsmMap& map)
{
  const long rawId = resultIterator.value(0).toLongLong();
  LOG_TRACE("raw ID: " << rawId);
  const long nodeId = _mapElementId(map, ElementId::node(rawId)).getId();
  LOG_VART(nodeId);
  const double lat =
    resultIterator.value(ApiDb::NODES_LATITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;
  const double lon =
    resultIterator.value(ApiDb::NODES_LONGITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;

  boost::shared_ptr<Node> node(
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
  _updateMetadataOnElement(node);
  //we want the reader's status to always override any existing status
  if (_status != Status::Invalid) { node->setStatus(_status); }

  return node;
}

WayPtr OsmApiDbReader::_resultToWay(const QSqlQuery& resultIterator, OsmMap& map)
{
  const long wayId = resultIterator.value(0).toLongLong();
  LOG_TRACE("raw ID: " << wayId);
  const long newWayId = _mapElementId(map, ElementId::way(wayId)).getId();
  LOG_TRACE("Reading way with ID: " << wayId);
  if (newWayId != wayId)
  {
    LOG_VARD(newWayId);
  }

  boost::shared_ptr<Way> way(
    new Way(
      _status,
      newWayId,
      ConfigOptions().getCircularErrorDefaultValue(),
      resultIterator.value(ApiDb::WAYS_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::WAYS_VERSION).toLongLong(),
      resultIterator.value(ApiDb::WAYS_TIMESTAMP).toUInt()));

  // if performance becomes an issue, try reading these out in batch at the same time
  // the element results are read
  vector<long> nodeIds = _database->selectNodeIdsForWay(wayId);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    nodeIds[i] = _mapElementId(map, ElementId::node(nodeIds[i])).getId();
  }
  way->addNodes(nodeIds);

  _parseAndSetTagsOnElement(way);
  _updateMetadataOnElement(way);
  //we want the reader's status to always override any existing status
  if (_status != Status::Invalid) { way->setStatus(_status); }

  return way;
}

void OsmApiDbReader::_addNodesForWay(vector<long> nodeIds, OsmMap& map)
{
  for (unsigned int i = 0; i < nodeIds.size(); i++)
  {
    LOG_VART(nodeIds[i]);
    QStringList tags;
    if (map.containsNode(nodeIds[i]) == false)
    {
      boost::shared_ptr<QSqlQuery> queryIterator = _database->selectNodeById(nodeIds[i]);
      while (queryIterator->next())
      {
        boost::shared_ptr<Node> node = _resultToNode(*queryIterator.get(), map);
        QString result = _database->extractTagFromRow(queryIterator, ElementType::Node);
        if (result != "")
        {
          tags << result;
        }

        if (tags.size() > 0)
        {
          node->setTags(ApiDb::unescapeTags(tags.join(", ")));
          _updateMetadataOnElement(node);
        }
        //we want the reader's status to always override any existing status
        if (_status != Status::Invalid) { node->setStatus(_status); }
        LOG_VART(node);
        map.addElement(node);
      }
    }
  }
}

RelationPtr OsmApiDbReader::_resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map)
{
  const long relationId = resultIterator.value(0).toLongLong();
  LOG_TRACE("raw ID: " << relationId);
  const long newRelationId = _mapElementId(map, ElementId::relation(relationId)).getId();
  LOG_TRACE("Reading relation with ID: " << relationId);
  if (newRelationId != relationId)
  {
    LOG_VART(newRelationId);
  }

  boost::shared_ptr<Relation> relation(
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
  vector<RelationData::Entry> members = _database->selectMembersForRelation(relationId);
  for (size_t i = 0; i < members.size(); ++i)
  {
    members[i].setElementId(_mapElementId(map, members[i].getElementId()));
  }
  relation->setMembers(members);

  _parseAndSetTagsOnElement(relation);
  _updateMetadataOnElement(relation);
  //we want the reader's status to always override any existing status
  if (_status != Status::Invalid) { relation->setStatus(_status); }

  return relation;
}

void OsmApiDbReader::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setUserEmail(configOptions.getApiDbEmail());
  setBoundingBox(configOptions.getConvertBoundingBox());
  setOverrideBoundingBox(configOptions.getConvertBoundingBoxOsmApiDatabase());
}

}
