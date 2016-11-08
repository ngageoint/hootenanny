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
  _elementResultIterator.reset();
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
  LOG_DEBUG("IN OsmApiDbReader::read()...");

  if (_osmElemId > -1 && _osmElemType != ElementType::Unknown)
  {
    LOG_INFO("Executing OSM API read query against element type " << _osmElemType << "...");

    _read(map, _osmElemType);
  }
  else if(_bbox == "") // process SELECT ALL
  {
    LOG_INFO("Executing OSM API read query against all element types...");

    for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
    {
      ElementType::Type elementType = static_cast<ElementType::Type>(ctr);
      _read(map, elementType);
    }
  }
  else // process BOUNDED REGION
  {
    LOG_INFO(
      "Executing OSM API bounded read query against all element types with bounds " << _bbox << "...");

    Envelope env = GeometryUtils::envelopeFromConfigString(_bbox);

    //select all elements in the bounding box
    for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
    {
      LOG_TRACE("About to call bounded with element");
      LOG_TRACE(ctr);
      ElementType::Type elementType = static_cast<ElementType::Type>(ctr);
      _readBounded(map, elementType, env);
    }

    //crop the map
    MapCropper::crop(map, env);
  }
}

void OsmApiDbReader::_readBounded(shared_ptr<OsmMap> map, const ElementType& elementType, const Envelope& env)
{
  LOG_DEBUG("IN OsmApiDbReader::readBounded(,)...");
  long long lastId = LLONG_MIN;
  shared_ptr<Element> element;
  QStringList tags;
  bool firstElement = true;

  // contact the DB and select all
  shared_ptr<QSqlQuery> elementResultsIterator =
    _database.selectBoundedElements(_osmElemId, elementType, _bbox);

  // check if db active or not
  assert(elementResultsIterator->isActive());

  switch (elementType.getEnum())
  {
    ///////////////////////////////////////////////////////////////////
    // NODES
    ///////////////////////////////////////////////////////////////////
    case ElementType::Node:
      while (elementResultsIterator->next())
      {
        long long id = elementResultsIterator->value(0).toLongLong();
        if (lastId != id)
        {
          // process the complete element only after the first element created
          if (!firstElement)
          {
            if (tags.size()>0)
            {
              LOG_VART(tags);
              element->setTags(ApiDb::unescapeTags(tags.join(", ")));
              ApiDbReader::addTagsToElement(element);
            }

            if (_status != Status::Invalid)
            {
              element->setStatus(_status);
            }
            map->addElement(element);
            tags.clear();
          }

          // extract the node contents except for the tags
          element = _resultToNode(*elementResultsIterator, *map);

          lastId = id;
          firstElement = false;
        }

        // read the tag for as many rows as there are tags
        // need to get into form "key1"=>"val1", "key2"=>"val2", ...
        QString result = _database.extractTagFromRow(elementResultsIterator, elementType.getEnum());
        if (result != "")
        {
          tags << result;
        }
      }
      // process the last complete element only if an element has been created
      if (!firstElement)
      {
        if (tags.size() > 0)
        {
          LOG_VART(tags);
          element->setTags(ApiDb::unescapeTags(tags.join(", ")));
          ApiDbReader::addTagsToElement(element);
        }
        if (_status != Status::Invalid) { element->setStatus(_status); }
        map->addElement(element);
        tags.clear();
      }
      break;

    ///////////////////////////////////////////////////////////////////
    // WAYS
    ///////////////////////////////////////////////////////////////////
    case ElementType::Way:
      while( elementResultsIterator->next() )
      {
        long long wayId = elementResultsIterator->value(0).toLongLong();
        shared_ptr<QSqlQuery> nodeInfoIterator = _database.selectNodesForWay( wayId );
        bool foundOne = false;
        while( nodeInfoIterator->next() && !foundOne)
        {
          // do the bounds check
          double lat = nodeInfoIterator->value(ApiDb::NODES_LATITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;
          double lon = nodeInfoIterator->value(ApiDb::NODES_LONGITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;
          Coordinate c(lon, lat);
          if (env.contains(c))
          {
            foundOne = true;
          }
        }
        if( foundOne )
        {
          // we have a polygon, so now you have to do some work; else go on to the next way_id
          // process the way into a data structure
          shared_ptr<Element> element = _resultToWay(*elementResultsIterator, *map);

          // get the way tags
          shared_ptr<QSqlQuery> wayTagIterator = _database.selectTagsForWay( wayId );
          while( wayTagIterator->next() )
          {
            // test for blank tag
            QString val1 = wayTagIterator->value(1).toString();
            QString val2 = wayTagIterator->value(2).toString();
            QString tag = "";
            if(val1!="" || val2!="") tag = "\""+val1+"\"=>\""+val2+"\"";
            if(tag != "") tags << tag;
          }
          if(tags.size()>0)
          {
            LOG_VART(tags);
            element->setTags( ApiDb::unescapeTags(tags.join(", ")) );
            ApiDbReader::addTagsToElement(element);
          }

          if (_status != Status::Invalid)
          {
            element->setStatus(_status);
          }
          map->addElement(element);
          tags.clear();
        }
      }
      break;

    ///////////////////////////////////////////////////////////////////
    // RELATIONS
    ///////////////////////////////////////////////////////////////////
    case ElementType::Relation:
      while( elementResultsIterator->next() )
      {
        _processRelation(*elementResultsIterator, *map, env);
      }
      break;

    default:
      throw HootException(QString("Unexpected element type: %1").arg(elementType.toString()));
  }

  LOG_DEBUG("LEAVING OsmApiDbReader::_read...");
}

void OsmApiDbReader::_processRelation(const QSqlQuery& resultIterator, OsmMap& map, const Envelope& env)
{
  QStringList tags;
  long long relId = resultIterator.value(0).toLongLong();

  vector<RelationData::Entry> members = _database.selectMembersForRelation( relId );
  for(vector<RelationData::Entry>::iterator it = members.begin(); it != members.end(); ++it)
  {
    ElementId eid = (*it).getElementId();
    QString type = eid.getType().toString();
    long idFromRelation = eid.getId();

    if(type=="Node")
    {
      shared_ptr<QSqlQuery> nodeIterator = _database.selectBoundedElements(idFromRelation, ElementType::Node, _bbox);
      if( nodeIterator->next() ) // we found a relation in the bounds
      {
        // process the relation into a data structure
        shared_ptr<Element> element = _resultToRelation(resultIterator, map);

        // get the way tags
        shared_ptr<QSqlQuery> relationTagIterator = _database.selectTagsForRelation( relId );
        while( relationTagIterator->next() )
        {
          // test for blank tag
          QString val1 = relationTagIterator->value(1).toString();
          QString val2 = relationTagIterator->value(2).toString();
          QString tag = "";
          if(val1!="" || val2!="") tag = "\""+val1+"\"=>\""+val2+"\"";
          if(tag != "") tags << tag;
        }
        if(tags.size()>0)
        {
          LOG_VART(tags);
          element->setTags(ApiDb::unescapeTags(tags.join(", ")) );
          ApiDbReader::addTagsToElement( element );
        }

        if (_status != Status::Invalid) {element->setStatus(_status); }
        map.addElement(element);
        tags.clear();
      }
    }
    else if(type == "Way")
    {
      shared_ptr<QSqlQuery> nodeInfoIterator = _database.selectNodesForWay( idFromRelation );
      bool foundOne = false;
      while( nodeInfoIterator->next() && !foundOne)
      {
        // do the bounds check
        double lat = nodeInfoIterator->value(ApiDb::NODES_LATITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;
        double lon = nodeInfoIterator->value(ApiDb::NODES_LONGITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;
        Coordinate c(lon, lat);
        if (env.contains(c))
        {
          foundOne = true;
        }
      }
      if( foundOne ) // we found a relation in the bounds
      {
        // process the relation into a data structure
        shared_ptr<Element> element = _resultToRelation(resultIterator, map);

        // get the way tags
        shared_ptr<QSqlQuery> relationTagIterator = _database.selectTagsForRelation( relId );
        while( relationTagIterator->next() )
        {
          // test for blank tag
          QString val1 = relationTagIterator->value(1).toString();
          QString val2 = relationTagIterator->value(2).toString();
          QString tag = "";
          if(val1!="" || val2!="") tag = "\""+val1+"\"=>\""+val2+"\"";
          if(tag != "") tags << tag;
        }
        if(tags.size()>0)
        {
          LOG_VART(tags);
          element->setTags( ApiDb::unescapeTags(tags.join(", ")) );
          ApiDbReader::addTagsToElement( element );
        }

        if (_status != Status::Invalid) { element->setStatus(_status); }
        map.addElement(element);
        tags.clear();
      }
    }
    else if(type == "Relation")
    {
      shared_ptr<QSqlQuery> relIterator = _database.selectBoundedElements(idFromRelation, ElementType::Relation, _bbox);
      while(relIterator->next())
      {
        _processRelation(*relIterator, map, env);
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

  while( elementResultsIterator->next() )
  {
    long long id = elementResultsIterator->value(0).toLongLong();
    if( lastId != id )
    {
      // process the complete element only after the first element created
      if(!firstElement)
      {
        if(tags.size()>0)
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
    if(result != "") tags << result;
  }

  // process the last complete element only if an element has been created
  if(!firstElement)
  {
    if(tags.size()>0)
    {
      LOG_VART(tags);
      element->setTags(ApiDb::unescapeTags(tags.join(", ")) );
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
  double lat = resultIterator.value(ApiDb::NODES_LATITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;
  double lon = resultIterator.value(ApiDb::NODES_LONGITUDE).toLongLong()/(double)ApiDb::COORDINATE_SCALE;

  shared_ptr<Node> result(
    new Node(
      _status,
      nodeId,
      lon,
      lat,
      ConfigOptions().getCircularErrorDefaultValue(),
      resultIterator.value(ApiDb::NODES_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::NODES_VERSION).toLongLong(),
      resultIterator.value(ApiDb::NODES_TIMESTAMP).toUInt()));
  LOG_VART(result);
  return result;
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

  //add nodes to the map if the map does not contain the node for way. Since query nodes in bounding box
  //may not contain all nodes for the way which will cause problem when cropping map later
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
    LOG_VARD(newRelationId);
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
