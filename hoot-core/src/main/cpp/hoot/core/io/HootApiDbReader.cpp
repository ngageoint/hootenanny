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
#include "HootApiDbReader.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementType.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>
#include <QDateTime>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, HootApiDbReader)

HootApiDbReader::HootApiDbReader() :
_status(Status::Invalid),
_useDataSourceIds(true),
_open(false)
{
  LOG_VARD(_useDataSourceIds);
  setConfiguration(conf());
}

HootApiDbReader::~HootApiDbReader()
{
  close();
}

Envelope HootApiDbReader::calculateEnvelope() const
{
  assert(_open);
  Envelope result = _database.calculateEnvelope();
  return result;
}

bool HootApiDbReader::isSupported(QString urlStr)
{
  QUrl url(urlStr);
  return _database.isSupported(url);
}

void HootApiDbReader::open(QString urlStr)
{
  if (!isSupported(urlStr))
  {
    throw HootException("An unsupported URL was passed in to HootApiDbReader: " + urlStr);
  }
  initializePartial();

  QUrl url(urlStr);
  QStringList pList = url.path().split("/");
  LOG_DEBUG("url path = "+url.path());
  bool ok;
  _database.open(url);

  long requestedMapId = pList[pList.size() - 1].toLong(&ok);

  if (!ok)
  {
    if (_email == "")
    {
      throw HootException("If a map name is specified then the user email must also be specified "
                          "via: " + emailKey());
    }

    QString mapName = pList[pList.size() - 1];
    _database.setUserId(_database.getUserId(_email, false));
    set<long> mapIds = _database.selectMapIds(mapName);
    if (mapIds.size() != 1)
    {
      QString str = QString("Expected 1 map with the name '%1' but found %2 maps.").arg(mapName)
          .arg(mapIds.size());
      throw HootException(str);
    }
    requestedMapId = *mapIds.begin();
  }

  if (!_database.mapExists(requestedMapId))
  {
    _database.close();
    throw HootException("No map exists with ID: " + QString::number(requestedMapId));
  }
  _database.setMapId(requestedMapId);

  //using a transaction seems to make sense here, b/c we don't want to read a map being modified
  //in the middle of its modification caused by a changeset upload, which could cause the map to
  //be invalid as a whole
  _database.transaction();
  _open = true;
}

bool HootApiDbReader::hasMoreElements()
{
  if (_nextElement == 0)
  {
    //populate next element.
    _nextElement = _getElementUsingIterator();
  }

  return _nextElement != NULL;
}

void  HootApiDbReader::initializePartial()
{
  _partialMap.reset(new OsmMap());

  _elementResultIterator.reset();
  _selectElementType = ElementType::Node;

  _elementsRead = 0;
}

void HootApiDbReader::read(shared_ptr<OsmMap> map)
{
  LOG_DEBUG("IN HootApiDbReader::read()...");

  for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
  {
    ElementType::Type elementType = static_cast<ElementType::Type>(ctr);
    _read(map, elementType);
  }
}

void HootApiDbReader::_read(shared_ptr<OsmMap> map, const ElementType& elementType)
{
  LOG_DEBUG("IN HootApiDbReader::read(,)...");

  // contact the DB and select all
  shared_ptr<QSqlQuery> elementResultsIterator = _database.selectElements(elementType);

  //need to check isActive, rather than next() here b/c resultToElement actually calls next() and
  //it will always return an extra null node at the end, unfortunately (see comments in
  //HootApiDb::resultToElement)
  while (elementResultsIterator->isActive())
  {
    shared_ptr<Element> element =
      _resultToElement(*elementResultsIterator, elementType, *map );
    //this check is necessary due to an inefficiency in HootApiDb::resultToElement
    if (element.get())
    {
      if (_status != Status::Invalid) { element->setStatus(_status); }
      map->addElement(element);
    }
  }

  LOG_DEBUG("LEAVING HootApiDbReader::_read...");
}

shared_ptr<Element> HootApiDbReader::_getElementUsingIterator()
{
  if (_selectElementType == ElementType::Unknown)
  {
    return shared_ptr<Element>();
  }

  //see of another result is available
  if (!_elementResultIterator.get() || !_elementResultIterator->isActive())
  {
    //no results available, so request some more results
    _elementResultIterator = _database.selectElements(_selectElementType);
  }

  //results still available, so keep parsing through them
  shared_ptr<Element> element = _resultToElement(*_elementResultIterator, _selectElementType,
    *_partialMap);

  //QSqlQuery::next() in HootApiDbReader::_resultToElement will return null
  //when end of records loop. The iterator will be reset and go to next element type
  if (!element.get())
  {
    _elementResultIterator.reset();
    const int currentTypeIndex = static_cast<int>(_selectElementType.getEnum());
    ElementType::Type nextType = static_cast<ElementType::Type>((currentTypeIndex + 1));
    ElementType t(nextType);
    _selectElementType = t;
    return _getElementUsingIterator();
  }

  assert(_selectElementType == element->getElementType());

  return element;
}

shared_ptr<Element> HootApiDbReader::readNextElement()
{
  if (hasMoreElements())
  {
    shared_ptr<Element> result = _nextElement;
    _nextElement.reset();
    _elementsRead++;
    return result;
  }
  else
  {
    throw HootException("readNextElement should not called if hasMoreElements return false.");
  }
}

void HootApiDbReader::finalizePartial()
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

void HootApiDbReader::close()
{
  finalizePartial();
}

ElementId HootApiDbReader::_mapElementId(const OsmMap& map, ElementId oldId)
{
  ElementId result;
  //LOG_VARD(oldId);
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
  //LOG_VARD(result);

  return result;
}

shared_ptr<Element> HootApiDbReader::_resultToElement(QSqlQuery& resultIterator,
  const ElementType& elementType, OsmMap& map)
{
  assert(resultIterator.isActive());
  //It makes much more sense to have callers call next on the iterator before passing it into this
  //method.  However, I was getting some initialization errors with QSqlQuery when the
  //HootApiDbReader called it in that way during a partial map read.  So, calling it inside here
  //instead.  A side effect is that this method will return a NULL element during the last
  //iteration.  Therefore, callers should check resultIterator->isActive in a loop in place of
  //calling resultIterator->next() and also should check for the null element.
  if (resultIterator.next())
  {
    shared_ptr<Element> element;
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

    ApiDbReader::addTagsToElement(element);

    return element;
  }
  else
  {
    resultIterator.finish();
    return shared_ptr<Element>();
  }
}

shared_ptr<Node> HootApiDbReader::_resultToNode(const QSqlQuery& resultIterator, OsmMap& map)
{
  long nodeId = _mapElementId(map, ElementId::node(resultIterator.value(0).toLongLong())).getId();
  //LOG_DEBUG("Reading node with ID: " << nodeId);
  shared_ptr<Node> result(
    new Node(
      _status,
      nodeId,
      resultIterator.value(ApiDb::NODES_LONGITUDE).toDouble(),
      resultIterator.value(ApiDb::NODES_LATITUDE).toDouble(),
      ConfigOptions().getCircularErrorDefaultValue(),
      resultIterator.value(ApiDb::NODES_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::NODES_VERSION).toLongLong(),
      OsmUtils::fromTimeString(
        resultIterator.value(ApiDb::NODES_TIMESTAMP).toDateTime().toString("yyyy-MM-ddThh:mm:ssZ"))));

  result->setTags(ApiDb::unescapeTags(resultIterator.value(ApiDb::NODES_TAGS)));
  ApiDbReader::addTagsToElement(result);
  //LOG_VARD(result);
  return result;
}

shared_ptr<Way> HootApiDbReader::_resultToWay(const QSqlQuery& resultIterator, OsmMap& map)
{
  const long wayId = resultIterator.value(0).toLongLong();
  const long newWayId = _mapElementId(map, ElementId::way(wayId)).getId();
  //LOG_DEBUG("Reading way with ID: " << wayId);
  /*if (newWayId != wayId)
  {
    LOG_VARD(newWayId);
  }*/
  shared_ptr<Way> way(
    new Way(
      _status,
      newWayId,
      ConfigOptions().getCircularErrorDefaultValue(),
      resultIterator.value(ApiDb::WAYS_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::WAYS_VERSION).toLongLong(),
      OsmUtils::fromTimeString(
        resultIterator.value(ApiDb::WAYS_TIMESTAMP).toDateTime().toString("yyyy-MM-ddThh:mm:ssZ"))
      ));

  way->setTags(ApiDb::unescapeTags(resultIterator.value(ApiDb::WAYS_TAGS)));
  ApiDbReader::addTagsToElement(way);

  // These could be read out in batch at the same time the element results are read...
  vector<long> nodeIds = _database.selectNodeIdsForWay(wayId);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    nodeIds[i] = _mapElementId(map, ElementId::node(nodeIds[i])).getId();
  }
  way->addNodes(nodeIds);
  //LOG_VARD(way);
  return way;
}

shared_ptr<Relation> HootApiDbReader::_resultToRelation(const QSqlQuery& resultIterator,
  const OsmMap& map)
{
  const long relationId = resultIterator.value(0).toLongLong();
  const long newRelationId = _mapElementId(map, ElementId::relation(relationId)).getId();
  //LOG_DEBUG("Reading relation with ID: " << relationId);
  /*if (newRelationId != relationId)
  {
    LOG_VARD(newRelationId);
  }*/
  shared_ptr<Relation> relation(
    new Relation(
      _status,
      newRelationId,
      ConfigOptions().getCircularErrorDefaultValue(),
      "",/*"collection"*/ //services db doesn't support relation "type" yet
      resultIterator.value(ApiDb::RELATIONS_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::RELATIONS_VERSION).toLongLong(),
      OsmUtils::fromTimeString(
        resultIterator.value(ApiDb::RELATIONS_TIMESTAMP).toDateTime().toString("yyyy-MM-ddThh:mm:ssZ"))));

  relation->setTags(ApiDb::unescapeTags(resultIterator.value(ApiDb::RELATIONS_TAGS)));
  ApiDbReader::addTagsToElement(relation);

  // These could be read out in batch at the same time the element results are read...
  vector<RelationData::Entry> members = _database.selectMembersForRelation(relationId);
  for (size_t i = 0; i < members.size(); ++i)
  {
    members[i].setElementId(_mapElementId(map, members[i].getElementId()));
  }
  relation->setMembers(members);
  //LOG_VARD(relation);
  return relation;
}

void HootApiDbReader::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setMaxElementsPerMap(configOptions.getMaxElementsPerPartialMap());
  setUserEmail(configOptions.getHootapiDbReaderEmail());
}

boost::shared_ptr<OGRSpatialReference> HootApiDbReader::getProjection() const
{
  boost::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }

  return wgs84;
}


}
