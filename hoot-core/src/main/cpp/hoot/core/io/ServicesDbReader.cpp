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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ServicesDbReader.h"

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
      try
      {
        double tv = tags.getLength("error:circular").value();
        element->setCircularError(tv);
        ok = true;
      }
      catch (const HootException& e)
      {
        ok = false;
      }

      if (!ok)
      {
        LOG_WARN("Error parsing error:circular.");
      }
    }
    tags.remove("error:circular");
  }
  else if (tags.contains("accuracy"))
  {
    element->setCircularError(tags.get("accuracy").toDouble(&ok));

    if (!ok)
    {
      try
      {
        double tv = tags.getLength("accuracy").value();
        element->setCircularError(tv);
        ok = true;
      }
      catch (const HootException& e)
      {
        ok = false;
      }

      if (!ok)
      {
        LOG_WARN("Error parsing accuracy.");
      }
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
  initializePartial();

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
  if (_nextElement == 0)
  {
    //populate next element.
    _nextElement = _getElementUsingIterator();
  }

  return _nextElement != NULL;
}

void  ServicesDbReader::initializePartial()
{
  _partialMap.reset(new OsmMap());

  _elementResultIterator.reset();
  _selectElementType = ElementType::Node;

  _elementsRead = 0;
}

void ServicesDbReader::read(shared_ptr<OsmMap> map)
{
  LOG_DEBUG("IN ServicesDbReader::read()...");
  //LOG_DEBUG("bounding box key=");
  //LOG_DEBUG(_bbox);

  if(_osmElemId > -1 && _osmElemType != ElementType::Unknown)
  {
    _read(map, _osmElemType);
  }
  else if(_bbox == "") // process SELECT ALL
  {
    for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
    {
      ElementType::Type elementType = static_cast<ElementType::Type>(ctr);
      _read(map, elementType);
    }
  }
  else // process BOUNDED REGION
  {
    for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
    {
        LOG_DEBUG("About to call bounded with element");
        LOG_DEBUG(ctr);
      ElementType::Type elementType = static_cast<ElementType::Type>(ctr);
      _readBounded(map, elementType);
    }
  }
}

void ServicesDbReader::_readBounded(shared_ptr<OsmMap> map, const ElementType& elementType)
{
  LOG_DEBUG("IN ServicesDbReader::readBounded(,)...");
  long long lastId = LLONG_MIN;
  shared_ptr<Element> element;
  QStringList tags;
  bool firstElement = true;
  QStringList bboxParts = _bbox.split(",");

  double minLat = bboxParts[1].toDouble();
  double minLon = bboxParts[0].toDouble();
  double maxLat = bboxParts[3].toDouble();
  double maxLon = bboxParts[2].toDouble();

  // determine is Services or Osm Api DB
  ServicesDb::DbType connectionType = _database.getDatabaseType();

  // contact the DB and select all
  shared_ptr<QSqlQuery> elementResultsIterator = _database.selectBoundedElements(_osmElemId, elementType, _bbox);

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

      switch (elementType.getEnum())
      {
        ///////////////////////////////////////////////////////////////////
        // NODES
        ///////////////////////////////////////////////////////////////////
        case ElementType::Node:
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
              element = _resultToNode_OsmApi(*elementResultsIterator, *map);

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
              double lat = nodeInfoIterator->value(ServicesDb::NODES_LATITUDE).toLongLong()/(double)ServicesDb::COORDINATE_SCALE;
              double lon = nodeInfoIterator->value(ServicesDb::NODES_LONGITUDE).toLongLong()/(double)ServicesDb::COORDINATE_SCALE;
              if(lat >= minLat && lat <= maxLat && lon >= minLon && lon <= maxLon) foundOne = true; // ToDo: process boundary condition
            }
            if( foundOne )
            {
              // we have a polygon, so now you have to do some work; else go on to the next way_id

              // process the way into a data structure
              shared_ptr<Element> element = _resultToWay_OsmApi(*elementResultsIterator, *map);

              // get the way tags
              shared_ptr<QSqlQuery> wayTagIterator = _database.selectTagsForWay_OsmApi( wayId );
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
                element->setTags( ServicesDb::unescapeTags(tags.join(", ")) );
                _addTagsToElement( element );
              }

              if (_status != Status::Invalid) { element->setStatus(_status); }
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
            _processRelation(*elementResultsIterator, *map);
          }
          break;

        default:
          throw HootException(QString("Unexpected element type: %1").arg(elementType.toString()));
      }
      break;

    default:
      throw HootException("_read cannot operate on unsupported database type");
      break;
  }
  LOG_DEBUG("LEAVING ServicesDbReader::_read...");
}

void ServicesDbReader::_processRelation(const QSqlQuery& resultIterator, OsmMap& map)
{
  QStringList tags;
  long long relId = resultIterator.value(0).toLongLong();
  QStringList bboxParts = _bbox.split(",");
  double minLat = bboxParts[1].toDouble();
  double minLon = bboxParts[0].toDouble();
  double maxLat = bboxParts[3].toDouble();
  double maxLon = bboxParts[2].toDouble();

  vector<RelationData::Entry> members = _database.selectMembersForRelation( relId );
  for(vector<RelationData::Entry>::iterator it = members.begin(); it != members.end(); ++it)
  {
    ElementId eid = (*it).getElementId();
    QString type = eid.getType().toString();
    long idFromRelation = eid.getId();

    if(type=="Node")
    {
      shared_ptr<QSqlQuery> nodeIterator = _database.selectBoundedElements(
                idFromRelation, ElementType::Node, _bbox);
      if( nodeIterator->next() ) // we found a relation in the bounds
      {
        // process the relation into a data structure
        shared_ptr<Element> element = _resultToRelation_OsmApi(resultIterator, map);

        // get the way tags
        shared_ptr<QSqlQuery> relationTagIterator = _database.selectTagsForRelation_OsmApi( relId );
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
          element->setTags( ServicesDb::unescapeTags(tags.join(", ")) );
          _addTagsToElement( element );
        }

        if (_status != Status::Invalid) { element->setStatus(_status); }
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
        double lat = nodeInfoIterator->value(ServicesDb::NODES_LATITUDE).toLongLong()/(double)ServicesDb::COORDINATE_SCALE;
        double lon = nodeInfoIterator->value(ServicesDb::NODES_LONGITUDE).toLongLong()/(double)ServicesDb::COORDINATE_SCALE;
        if(lat >= minLat && lat <= maxLat && lon >= minLon && lon <= maxLon) foundOne = true; // ToDo: process boundary condition
      }
      if( foundOne ) // we found a relation in the bounds
      {
        // process the relation into a data structure
        shared_ptr<Element> element = _resultToRelation_OsmApi(resultIterator, map);

        // get the way tags
        shared_ptr<QSqlQuery> relationTagIterator = _database.selectTagsForRelation_OsmApi( relId );
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
          element->setTags( ServicesDb::unescapeTags(tags.join(", ")) );
          _addTagsToElement( element );
        }

        if (_status != Status::Invalid) { element->setStatus(_status); }
        map.addElement(element);
        tags.clear();
      }
    }
    else if(type == "Relation")
    {
      shared_ptr<QSqlQuery> relIterator = _database.selectBoundedElements(
                idFromRelation, ElementType::Relation, _bbox);
      while(relIterator->next()) {
        _processRelation(*relIterator, map);
      }
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

shared_ptr<Element> ServicesDbReader::_getElementUsingIterator()
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

  //QSqlQuery::next() in ServicesDbReader::_resultToElement will return null
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

shared_ptr<Element> ServicesDbReader::readNextElement()
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
      resultIterator.value(ServicesDb::NODES_CHANGESET).toLongLong(),
      resultIterator.value(ServicesDb::NODES_VERSION).toLongLong(),
      OsmUtils::fromTimeString(
        resultIterator.value(ServicesDb::NODES_TIMESTAMP).toDateTime().toString("yyyy-MM-ddThh:mm:ssZ")),
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
      resultIterator.value(ServicesDb::WAYS_CHANGESET).toLongLong(),
      resultIterator.value(ServicesDb::WAYS_VERSION).toLongLong(),
      OsmUtils::fromTimeString(
        resultIterator.value(ServicesDb::WAYS_TIMESTAMP).toDateTime().toString("yyyy-MM-ddThh:mm:ssZ")),
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
      resultIterator.value(ServicesDb::RELATIONS_CHANGESET).toLongLong(),
      resultIterator.value(ServicesDb::RELATIONS_VERSION).toLongLong(),
      OsmUtils::fromTimeString(
        resultIterator.value(ServicesDb::RELATIONS_TIMESTAMP).toDateTime().toString("yyyy-MM-ddThh:mm:ssZ")),
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
  ConfigOptions configOptions(conf);
  setMaxElementsPerMap(configOptions.getMaxElementsPerPartialMap());
  setUserEmail(configOptions.getServicesDbReaderEmail());
  setBoundingBox(configOptions.getConvertBoundingBox());
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
