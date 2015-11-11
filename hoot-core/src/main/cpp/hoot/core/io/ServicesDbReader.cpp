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
#include "ServicesDbReader.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/Settings.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, ServicesDbReader)

ServicesDbReader::ServicesDbReader() :
_status(Status::Invalid),
_useDataSourceIds(true),
_open(false),
_osmElemId(-1),
_osmElemType(ElementType::Unknown)
{
  setConfiguration(conf());
}

ServicesDbReader::~ServicesDbReader()
{
  close();
}

void ServicesDbReader::_addTagsToElement(shared_ptr<Element> element)
{
  bool ok;
  Tags& tags = element->getTags();

  if (tags.contains("hoot:status"))
  {
    QString statusStr = tags.get("hoot:status");
    bool ok;
    const int statusInt = statusStr.toInt(&ok);
    Status status = static_cast<Status::Type>(statusInt);
    if (ok && status.getEnum() >= Status::Invalid && status.getEnum() <= Status::Conflated)
    {
      element->setStatus(status);
    }
    else
    {
      LOG_WARN("Invalid status: " + statusStr + " for element with ID: " +
               QString::number(element->getId()));
    }
    tags.remove("hoot:status");
  }

  if (tags.contains("type"))
  {
    Relation* r = dynamic_cast<Relation*>(element.get());
    if (r)
    {
      r->setType(tags["type"]);
      tags.remove("type");
    }
  }

  if (tags.contains("error:circular"))
  {
    element->setCircularError(tags.get("error:circular").toDouble(&ok));
    if (!ok)
    {
      LOG_WARN("Error parsing error:circular.");
    }
    tags.remove("error:circular");
  }
  else if (tags.contains("accuracy"))
  {
    element->setCircularError(tags.get("accuracy").toDouble(&ok));

    if (!ok)
    {
      LOG_WARN("Error parsing accuracy.");
    }
    tags.remove("accuracy");
  }
}

Envelope ServicesDbReader::calculateEnvelope() const
{
  assert(_open);
  Envelope result = _database.calculateEnvelope();
  return result;
}

bool ServicesDbReader::isSupported(QString urlStr)
{
  QUrl url(urlStr);
  return _database.isSupported(url);
}

void ServicesDbReader::open(QString urlStr)
{
  if (!isSupported(urlStr))
  {
    throw HootException("An unsupported URL was passed in.");
  }

  QUrl url(urlStr);
  QString osmElemId = url.queryItemValue("osm-element-id");
  QString osmElemType = url.queryItemValue("osm-element-type");
  QStringList pList = url.path().split("/");
  LOG_DEBUG("url path = "+url.path());
  bool ok;
  bool ok2;
  QString mapName;
  _database.open(url);

  long requestedMapId = pList[pList.size() - 1].toLong(&ok);

  if(osmElemId.length() > 0 && osmElemType.length() > 0)
  {
    _osmElemId = osmElemId.toLong(&ok2);
    _osmElemType = ElementType::fromString(osmElemType);

  }

  if (!ok && _database.getDatabaseType() != ServicesDb::DBTYPE_OSMAPI)
  {
    if (_email == "")
    {
      throw HootException("If a map name is specified then the user email must also be specified "
                          "via: " + emailKey());
    }

    mapName = pList[pList.size() - 1];
    _database.setUserId(_database.getUserId(_email));
    set<long> mapIds = _database.selectMapIds(mapName);
    if (mapIds.size() != 1)
    {
      QString str = QString("Expected 1 map with the name '%1' but found %2 maps.").arg(mapName)
          .arg(mapIds.size());
      throw HootException(str);
    }
    requestedMapId = *mapIds.begin();
  }

  if( _database.getDatabaseType() != ServicesDb::DBTYPE_OSMAPI )
  {
    if (!_database.mapExists(requestedMapId))
    {
      _database.close();
      throw HootException("No map exists with ID: " + QString::number(requestedMapId));
    }
    _database.setMapId(requestedMapId);
  }

  //using a transaction seems to make sense here, b/c we don't want to read a map being modified
  //in the middle of its modification caused by a changeset upload, which could cause the map to
  //be invalid as a whole
  _database.transaction();
  _open = true;
}

bool ServicesDbReader::hasMoreElements()
{
  if (!_firstPartialReadCompleted)
  {
    //get the total element counts before beginning results parsing
    _totalNumMapNodes = _database.numElements(ElementType::Node);
    _totalNumMapWays = _database.numElements(ElementType::Way);
    _totalNumMapRelations = _database.numElements(ElementType::Relation);

    _firstPartialReadCompleted = true;
  }

  const long numElementsRead = _nodeIndex + _wayIndex + _relationIndex;
  const long numElementsTotal = _totalNumMapNodes + _totalNumMapWays + _totalNumMapRelations;
  assert(numElementsRead <= numElementsTotal);
  //each results index is 0 based, so as soon as the sum of indexes is equal to the total number of
  //elements, we're done iterating through them
  return (numElementsRead < numElementsTotal);
}

void  ServicesDbReader::initializePartial()
{
  _partialMap.reset(new OsmMap());

  _elementResultIterator.reset();
  _firstPartialReadCompleted = false;

  _elementsRead = 0;

  _nodeIndex = 0;
  _totalNumMapNodes = 0;

  _wayIndex = 0;
  _totalNumMapWays = 0;

  _relationIndex = 0;
  _totalNumMapRelations = 0;
}

void ServicesDbReader::read(shared_ptr<OsmMap> map)
{
  LOG_DEBUG("IN ServicesDbReader::read()...");

  if(_osmElemId > -1 && _osmElemType != ElementType::Unknown)
  {
    _read(map, _osmElemType);
  }
  else
  {
    for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
    {
      ElementType::Type elementType = static_cast<ElementType::Type>(ctr);
      _read(map, elementType);
    }
  }
}

void ServicesDbReader::_read(shared_ptr<OsmMap> map, const ElementType& elementType)
{
  LOG_DEBUG("IN ServicesDbReader::read(,)...");
  long long lastId = LLONG_MIN;
  shared_ptr<Element> element;
  QStringList tags;
  bool firstElement = true;

  // determine is Services or Osm Api DB
  ServicesDb::DbType connectionType = _database.getDatabaseType();

  // contact the DB and select all
  shared_ptr<QSqlQuery> elementResultsIterator = _database.selectAllElements(_osmElemId, elementType);

  // split the reading of Services and Osm Api DB upfront to avoid extra inefficiency of if-else calls
  //   inside the isActive loop
  switch ( connectionType )
  {
    case ServicesDb::DBTYPE_SERVICES:
      //need to check isActive, rather than next() here b/c resultToElement actually calls next() and
      //it will always return an extra null node at the end, unfortunately (see comments in
      //ServicesDb::resultToElement)
      while (elementResultsIterator->isActive())
      {
        shared_ptr<Element> element =
          _resultToElement(*elementResultsIterator, elementType, *map );
        //this check is necessary due to an inefficiency in ServicesDb::resultToElement
        if (element.get())
        {
          if (_status != Status::Invalid) { element->setStatus(_status); }
          map->addElement(element);
        }
      }
      break;

    case ServicesDb::DBTYPE_OSMAPI:
      // check if db active or not
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
              element->setTags( ServicesDb::unescapeTags(tags.join(", ")) );
              _addTagsToElement( element );
            }

            if (_status != Status::Invalid) { element->setStatus(_status); }
            map->addElement(element);
            tags.clear();
          }

          // extract the node contents except for the tags
          switch (elementType.getEnum())
          {
            case ElementType::Node:
              element = _resultToNode_OsmApi(*elementResultsIterator, *map);
              break;

            case ElementType::Way:
              element = _resultToWay_OsmApi(*elementResultsIterator, *map);
              break;

            case ElementType::Relation:
              element = _resultToRelation_OsmApi(*elementResultsIterator, *map);
              break;

            default:
              throw HootException(QString("Unexpected element type: %1").arg(elementType.toString()));
          }
          lastId = id;
          firstElement = false;
        }

        // read the tag for as many rows as there are tags
        // need to get into form "key1"=>"val1", "key2"=>"val2", ...

        QString result = _database.extractTagFromRow_OsmApi(elementResultsIterator, elementType.getEnum());
        if(result != "") tags << result;
      }

      // process the last complete element only if an element has been created
      if(!firstElement)
      {
        if(tags.size()>0)
        {
          element->setTags( ServicesDb::unescapeTags(tags.join(", ")) );
          _addTagsToElement( element );
        }
        if (_status != Status::Invalid) { element->setStatus(_status); }
        map->addElement(element);
        tags.clear();
      }
      break;

    default:
      throw HootException("_read cannot operate on unsupported database type");
      break;
  }
  LOG_DEBUG("LEAVING ServicesDbReader::_read...");
}

shared_ptr<Element> ServicesDbReader::readNextElement()
{
  if (!hasMoreElements())
  {
    throw HootException(
      "No more elements available to read from map");
  }

  ElementType selectElementType = _getCurrentSelectElementType();
  //see of another result is available
  if (!_elementResultIterator.get() || !_elementResultIterator->isActive())
  {
    //no results available, so request some more results
    _elementResultIterator =
      _database.selectElements(
        -1, selectElementType, _maxElementsPerMap, _getCurrentElementOffset(selectElementType));
  }

  //results still available, so keep parsing through them
  shared_ptr<Element> element = _resultToElement(*_elementResultIterator, selectElementType,
    *_partialMap);
  if (!element.get())
  {
    //exceptional case to deal with having to call QSqlQuery::next() inside of
    //ServicesDb::resultToElement rather than from inside this method
    _elementResultIterator.reset();
    return shared_ptr<Element>();
  }

  _incrementElementIndex(selectElementType);
  assert(selectElementType == element->getElementType());
  _elementsRead++;
  if (_elementsRead == _maxElementsPerMap)
  {
    _elementResultIterator->finish();
  }
  return element;
}

void ServicesDbReader::finalizePartial()
{
  _elementResultIterator.reset();
  _partialMap.reset();
  if (_open)
  {
    _database.commit();
    _database.close();
    _open = false;
  }
}

void ServicesDbReader::close()
{
  finalizePartial();
}

const ElementType ServicesDbReader::_getCurrentSelectElementType() const
{
  ElementType selectElementType;
  if (_nodeIndex < _totalNumMapNodes)
  {
    selectElementType = ElementType::Node;
  }
  else if (_wayIndex < _totalNumMapWays)
  {
    selectElementType = ElementType::Way;
  }
  else if (_relationIndex < _totalNumMapRelations)
  {
    selectElementType = ElementType::Relation;
  }
  else
  {
    assert(false);  //hasMoreElements should have kept us from being here
  }
  return selectElementType;
}

long ServicesDbReader::_getCurrentElementOffset(const ElementType& selectElementType) const
{
  long selectOffset = -1;
  if (selectElementType == ElementType::Node)
  {
    selectOffset = _nodeIndex;
  }
  else if (selectElementType == ElementType::Way)
  {
    selectOffset = _wayIndex;
  }
  else if (selectElementType == ElementType::Relation)
  {
    selectOffset = _relationIndex;
  }
  else
  {
    assert(false);  //hasMoreElements should have kept us from being here
  }
  return selectOffset;
}

void ServicesDbReader::_incrementElementIndex(const ElementType& selectElementType)
{
  if (selectElementType == ElementType::Node)
  {
    _nodeIndex++;
  }
  else if (selectElementType == ElementType::Way)
  {
    _wayIndex++;
  }
  else if (selectElementType == ElementType::Relation)
  {
    _relationIndex++;
  }
  else
  {
    assert(false);  //hasMoreElements should have kept us from being here
  }
}

ElementId ServicesDbReader::_mapElementId(const OsmMap& map, ElementId oldId)
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

shared_ptr<Element> ServicesDbReader::_resultToElement(QSqlQuery& resultIterator,
  const ElementType& elementType, OsmMap& map)
{
  assert(resultIterator.isActive());
  //It makes much more sense to have callers call next on the iterator before passing it into this
  //method.  However, I was getting some initialization errors with QSqlQuery when the
  //ServicesDbReader called it in that way during a partial map read.  So, calling it inside here
  //instead.  A side effect is that this method will return a NULL element during the last
  //iteration.  Therefore, callers should check resultIterator->isActive in a loop in place of
  //calling resultIterator->next() and also should check for the null element.
  if (resultIterator.next())
  {
    shared_ptr<Element> element;
    //TODO: this section could be simplified
    switch (elementType.getEnum())
    {
      case ElementType::Node:
        {
          element = _resultToNode(resultIterator, map);
        }
        break;

      case ElementType::Way:
        {
          element = _resultToWay(resultIterator, map);
        }
        break;

      case ElementType::Relation:
        {
          element = _resultToRelation(resultIterator, map);
        }
        break;

      default:
        throw HootException(QString("Unexpected element type: %1").arg(elementType.toString()));
    }

    _addTagsToElement(element);

    return element;
  }
  else
  {
    resultIterator.finish();
    return shared_ptr<Element>();
  }
}

shared_ptr<Node> ServicesDbReader::_resultToNode(const QSqlQuery& resultIterator, OsmMap& map)
{
  long nodeId = _mapElementId(map, ElementId::node(resultIterator.value(0).toLongLong())).getId();

  shared_ptr<Node> result(
    new Node(
      _status,
      nodeId,
      resultIterator.value(ServicesDb::NODES_LONGITUDE).toDouble(),
      resultIterator.value(ServicesDb::NODES_LATITUDE).toDouble(),
      ServicesDb::DEFAULT_ELEMENT_CIRCULAR_ERROR));

  result->setTags(ServicesDb::unescapeTags(resultIterator.value(ServicesDb::NODES_TAGS)));
  _addTagsToElement(result);

  return result;
}

shared_ptr<Node> ServicesDbReader::_resultToNode_OsmApi(const QSqlQuery& resultIterator, OsmMap& map)
{
  long nodeId = _mapElementId(map, ElementId::node(resultIterator.value(0).toLongLong())).getId();
  double lat = resultIterator.value(ServicesDb::NODES_LATITUDE).toLongLong()/(double)ServicesDb::COORDINATE_SCALE;
  double lon = resultIterator.value(ServicesDb::NODES_LONGITUDE).toLongLong()/(double)ServicesDb::COORDINATE_SCALE;
  shared_ptr<Node> result(
    new Node(
      _status,
      nodeId,
      lon,
      lat,
      ServicesDb::DEFAULT_ELEMENT_CIRCULAR_ERROR));

  return result;
}


shared_ptr<Way> ServicesDbReader::_resultToWay(const QSqlQuery& resultIterator, OsmMap& map)
{
  const long wayId = resultIterator.value(0).toLongLong();
  const long newWayId = _mapElementId(map, ElementId::way(wayId)).getId();
  shared_ptr<Way> way(
    new Way(
      _status,
      newWayId,
      ServicesDb::DEFAULT_ELEMENT_CIRCULAR_ERROR));

  way->setTags(ServicesDb::unescapeTags(resultIterator.value(ServicesDb::WAYS_TAGS)));
  _addTagsToElement(way);

  //TODO: read these out in batch at the same time the element results are read
  vector<long> nodeIds = _database.selectNodeIdsForWay(wayId);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    nodeIds[i] = _mapElementId(map, ElementId::node(nodeIds[i])).getId();
  }
  way->addNodes(nodeIds);

  return way;
}

shared_ptr<Way> ServicesDbReader::_resultToWay_OsmApi(const QSqlQuery& resultIterator, OsmMap& map)
{
  const long wayId = resultIterator.value(0).toLongLong();
  const long newWayId = _mapElementId(map, ElementId::way(wayId)).getId();
  shared_ptr<Way> way(
    new Way(
      _status,
      newWayId,
      ServicesDb::DEFAULT_ELEMENT_CIRCULAR_ERROR));

  //TODO: read these out in batch at the same time the element results are read
  vector<long> nodeIds = _database.selectNodeIdsForWay(wayId);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    nodeIds[i] = _mapElementId(map, ElementId::node(nodeIds[i])).getId();
  }
  way->addNodes(nodeIds);

  return way;
}

shared_ptr<Relation> ServicesDbReader::_resultToRelation(const QSqlQuery& resultIterator,
  const OsmMap& map)
{
  const long relationId = resultIterator.value(0).toLongLong();
  const long newRelationId = _mapElementId(map, ElementId::relation(relationId)).getId();

  shared_ptr<Relation> relation(
    new Relation(
      _status,
      newRelationId,
      ServicesDb::DEFAULT_ELEMENT_CIRCULAR_ERROR/*,
      "collection"*/));  //TODO: services db doesn't support relation "type" yet

  relation->setTags(ServicesDb::unescapeTags(resultIterator.value(ServicesDb::RELATIONS_TAGS)));
  _addTagsToElement(relation);

  //TODO: read these out in batch at the same time the element results are read
  vector<RelationData::Entry> members = _database.selectMembersForRelation(relationId);
  for (size_t i = 0; i < members.size(); ++i)
  {
    members[i].setElementId(_mapElementId(map, members[i].getElementId()));
  }
  relation->setMembers(members);

  return relation;
}

shared_ptr<Relation> ServicesDbReader::_resultToRelation_OsmApi(const QSqlQuery& resultIterator,
  const OsmMap& map)
{
  const long relationId = resultIterator.value(0).toLongLong();
  const long newRelationId = _mapElementId(map, ElementId::relation(relationId)).getId();

  shared_ptr<Relation> relation(
    new Relation(
      _status,
      newRelationId,
      ServicesDb::DEFAULT_ELEMENT_CIRCULAR_ERROR/*,
      "collection"*/));  //TODO: services db doesn't support relation "type" yet

  //TODO: read these out in batch at the same time the element results are read
  vector<RelationData::Entry> members = _database.selectMembersForRelation(relationId);
  for (size_t i = 0; i < members.size(); ++i)
  {
    members[i].setElementId(_mapElementId(map, members[i].getElementId()));
  }
  relation->setMembers(members);

  return relation;
}

void ServicesDbReader::setConfiguration(const Settings& conf)
{
  setMaxElementsPerMap(ConfigOptions(conf).getMaxElementsPerPartialMap());
  setUserEmail(conf.getString(emailKey(), ""));
}

boost::shared_ptr<OGRSpatialReference> ServicesDbReader::getProjection() const
{
  boost::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }

  return wgs84;
}


}
