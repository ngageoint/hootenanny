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
#include "ApiDbReader.h"

// Hoot
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/io/TableType.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// tgs
#include <tgs/System/Time.h>

// Qt
#include <QSet>

using namespace geos::geom;

namespace hoot
{

unsigned int ApiDbReader::logWarnCount = 0;

ApiDbReader::ApiDbReader() :
_useDataSourceIds(true),
_status(Status::Invalid),
_open(false),
_returnNodesOnly(false),
_keepFileStatus(ConfigOptions().getReaderKeepFileStatus())
{
}

bool ApiDbReader::isSupported(QString urlStr)
{
  QUrl url(urlStr);
  return _getDatabase()->isSupported(url);
}

void ApiDbReader::setBoundingBox(const QString bbox)
{
  if (!bbox.trimmed().isEmpty())
  {
    setBounds(GeometryUtils::envelopeFromConfigString(bbox));
  }
}

void ApiDbReader::setOverrideBoundingBox(const QString bbox)
{
  if (!bbox.trimmed().isEmpty())
  {
    _overrideBounds = GeometryUtils::envelopeFromConfigString(bbox);
  }
}

bool ApiDbReader::_hasBounds()
{
  return _isValidBounds(_bounds) || _isValidBounds(_overrideBounds);
}

ElementId ApiDbReader::_mapElementId(const OsmMap& map, ElementId oldId)
{
  LOG_TRACE("Mapping element ID: " << oldId << "...");

  ElementId result;
  LOG_VART(oldId);
  LOG_VART(_useDataSourceIds);
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
        LOG_VART(result);
      }
      else
      {
        long newId = map.createNextNodeId();
        _nodeIdMap[id] = newId;
        result = ElementId::node(newId);
        LOG_VART(result);
      }
      break;
    case ElementType::Way:
      if (_wayIdMap.count(id) > 0)
      {
        result = ElementId::way(_wayIdMap.at(id));
        LOG_VART(result);
      }
      else
      {
        long newId = map.createNextWayId();
        _wayIdMap[id] = newId;
        result = ElementId::way(newId);
        LOG_VART(result);
      }
      break;
    case ElementType::Relation:
      if (_relationIdMap.count(id) > 0)
      {
        result = ElementId::relation(_relationIdMap.at(id));
        LOG_VART(result);
      }
      else
      {
        long newId = map.createNextRelationId();
        _relationIdMap[id] = newId;
        result = ElementId::relation(newId);
        LOG_VART(result);
      }
      break;
    default:
      throw IllegalArgumentException("Expected a valid element type, but got: " +
        QString::number(oldId.getType().getEnum()));
    }
  }

  LOG_TRACE("Mapped element ID: " << oldId << " to ID: " << result << "...");

  return result;
}

void ApiDbReader::_updateMetadataOnElement(ElementPtr element)
{
  LOG_TRACE("Updating metadata on element " << element->getElementId() << "...");

  bool ok;
  Tags& tags = element->getTags();

  if (tags.contains(MetadataTags::HootStatus()))
  {
    QString statusStr = tags.get(MetadataTags::HootStatus());
    bool ok;
    const int statusInt = statusStr.toInt(&ok);
    Status status = static_cast<Status::Type>(statusInt);
    if (ok && status.getEnum() >= Status::Invalid && status.getEnum() <= Status::Conflated)
    {
      element->setStatus(status);
    }
    else
    {
      try
      {
        //  Try parsing the status in text form
        element->setStatus(Status::fromString(statusStr));
      }
      catch (const HootException&)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Invalid status: " + statusStr + " for element with ID: " +
                   QString::number(element->getId()));
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
    //We don't need to carry this tag around once the value is set on the element...it will
    //be reinstated by some writers, though.
    if (!_keepFileStatus) { tags.remove(MetadataTags::HootStatus()); }
  }

  if (tags.contains("type"))
  {
    Relation* r = dynamic_cast<Relation*>(element.get());
    if (r)
    {
      r->setType(tags["type"]);
      //I don't think OSM non-hoot metadata tags should be removed here...
      //tags.remove("type");
    }
  }

  if (tags.contains(MetadataTags::ErrorCircular()))
  {
    element->setCircularError(tags.get(MetadataTags::ErrorCircular()).toDouble(&ok));
    if (!ok)
    {
      try
      {
        double tv = tags.getLength(MetadataTags::ErrorCircular()).value();
        element->setCircularError(tv);
        ok = true;
      }
      catch (const HootException&)
      {
        ok = false;
      }

      if (!ok)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Error parsing " + MetadataTags::ErrorCircular() + ".");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
    //We don't need to carry this tag around once the value is set on the element...it will
    //be reinstated by some writers, though.
    tags.remove(MetadataTags::ErrorCircular());
  }
  else if (tags.contains(MetadataTags::Accuracy()))
  {
    element->setCircularError(tags.get(MetadataTags::Accuracy()).toDouble(&ok));

    if (!ok)
    {
      try
      {
        double tv = tags.getLength(MetadataTags::Accuracy()).value();
        element->setCircularError(tv);
        ok = true;
      }
      catch (const HootException&)
      {
        ok = false;
      }

      if (!ok)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Error parsing " + MetadataTags::Accuracy() + ".");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
    //I don't think OSM non-hoot metadata tags should be removed here...
    //tags.remove(MetadataTags::Accuracy());
  }
}

bool ApiDbReader::_isValidBounds(const Envelope& bounds)
{
  if (bounds.isNull() ||
      (bounds.getMinX() == -180.0 && bounds.getMinY() == -90.0 && bounds.getMaxX() == 180.0 &&
       bounds.getMaxY() == 90.0))
  {
    return false;
  }
  return true;
}

void ApiDbReader::_readByBounds(OsmMapPtr map, const Envelope& bounds)
{
  long boundedNodeCount = 0;
  long boundedWayCount = 0;
  long boundedRelationCount = 0;

  LOG_DEBUG("Retrieving node records within the query bounds...");
  boost::shared_ptr<QSqlQuery> nodeItr = _getDatabase()->selectNodesByBounds(bounds);
  QSet<QString> nodeIds;
  while (nodeItr->next())
  {
    const QSqlQuery resultIterator = *nodeItr;
    NodePtr node = _resultToNode(resultIterator, *map);
    LOG_VART(node);
    map->addElement(node);
    boundedNodeCount++;
    //Don't use the mapped id from the node object here, b/c we want don't want to use mapped ids
    //with any queries.  Mapped ids may not exist yet.
    const long nodeId = resultIterator.value(0).toLongLong();
    LOG_VART(ElementId(ElementType::Node, nodeId));
    nodeIds.insert( QString::number(nodeId));
  }
  LOG_VARD(nodeIds.size());

  if (!_returnNodesOnly)
  {
    if (nodeIds.size() > 0)
    {
      LOG_DEBUG("Retrieving way IDs referenced by the selected nodes...");
      QSet<QString> wayIds;
      boost::shared_ptr<QSqlQuery> wayIdItr = _getDatabase()->selectWayIdsByWayNodeIds(nodeIds);
      while (wayIdItr->next())
      {
        const long wayId = (*wayIdItr).value(0).toLongLong();
        LOG_VART(ElementId(ElementType::Way, wayId));
        wayIds.insert(QString::number(wayId));
      }
      LOG_VARD(wayIds.size());

      if (wayIds.size() > 0)
      {
        LOG_DEBUG("Retrieving ways by way ID...");
        boost::shared_ptr<QSqlQuery> wayItr =
          _getDatabase()->selectElementsByElementIdList(wayIds, TableType::Way);
        while (wayItr->next())
        {
          //I'm a little confused why this wouldn't cause a problem in that you could be writing ways
          //to the map here whose nodes haven't yet been written to the map yet.  Haven't encountered
          //the problem yet with test data, but will continue to keep an eye on it.
          WayPtr way = _resultToWay(*wayItr, *map);
          map->addElement(way);
          LOG_VART(way);
          boundedWayCount++;
        }

        LOG_DEBUG("Retrieving way node IDs referenced by the selected ways...");
        QSet<QString> additionalWayNodeIds;
        boost::shared_ptr<QSqlQuery> additionalWayNodeIdItr =
          _getDatabase()->selectWayNodeIdsByWayIds(wayIds);
        while (additionalWayNodeIdItr->next())
        {
          const long nodeId = (*additionalWayNodeIdItr).value(0).toLongLong();
          LOG_VART(ElementId(ElementType::Node, nodeId));
          additionalWayNodeIds.insert(QString::number(nodeId));
        }

        //subtract nodeIds from additionalWayNodeIds so no dupes get added
        LOG_VARD(nodeIds.size());
        LOG_VARD(additionalWayNodeIds.size());
        additionalWayNodeIds = additionalWayNodeIds.subtract(nodeIds);
        LOG_VARD(additionalWayNodeIds.size());

        if (additionalWayNodeIds.size() > 0)
        {
          nodeIds.unite(additionalWayNodeIds);
          LOG_DEBUG(
            "Retrieving nodes falling outside of the query bounds but belonging to a selected way...");
          boost::shared_ptr<QSqlQuery> additionalWayNodeItr =
            _getDatabase()->selectElementsByElementIdList(additionalWayNodeIds, TableType::Node);
          while (additionalWayNodeItr->next())
          {
            NodePtr node = _resultToNode(*additionalWayNodeItr, *map);
            map->addElement(node);
            LOG_VART(node);
            boundedNodeCount++;
          }
        }
      }

      LOG_DEBUG("Retrieving relation IDs referenced by the selected ways and nodes...");
      QSet<QString> relationIds;
      assert(nodeIds.size() > 0);
      boost::shared_ptr<QSqlQuery> relationIdItr =
        _getDatabase()->selectRelationIdsByMemberIds(nodeIds, ElementType::Node);
      while (relationIdItr->next())
      {
        const long relationId = (*relationIdItr).value(0).toLongLong();
        LOG_VART(ElementId(ElementType::Relation, relationId));
        relationIds.insert(QString::number(relationId));
      }
      if (wayIds.size() > 0)
      {
        relationIdItr = _getDatabase()->selectRelationIdsByMemberIds(wayIds, ElementType::Way);
        while (relationIdItr->next())
        {
          const long relationId = (*relationIdItr).value(0).toLongLong();
          LOG_VART(ElementId(ElementType::Relation, relationId));
          relationIds.insert(QString::number(relationId));
        }
      }
      LOG_VARD(relationIds.size());

      if (relationIds.size() > 0)
      {
        LOG_DEBUG("Retrieving relations by relation ID...");
        boost::shared_ptr<QSqlQuery> relationItr =
          _getDatabase()->selectElementsByElementIdList(relationIds, TableType::Relation);
        while (relationItr->next())
        {
          RelationPtr relation = _resultToRelation(*relationItr, *map);
          map->addElement(relation);
          LOG_VART(relation);
          boundedRelationCount++;
        }
      }
    }
  }

  LOG_DEBUG(
    "Bounded query read " << (boundedNodeCount + boundedWayCount + boundedRelationCount) <<
    " total elements.");
  LOG_VARD(boundedNodeCount);
  LOG_VARD(boundedWayCount);
  LOG_VARD(boundedRelationCount);
  LOG_DEBUG("Current map:");
  LOG_VARD(map->getNodes().size());
  LOG_VARD(map->getWays().size());
  LOG_VARD(map->getRelations().size());
}

void ApiDbReader::initializePartial()
{
  _partialMap.reset(new OsmMap());

  _firstPartialReadCompleted = false;
  _elementsRead = 0;
  _selectElementType = ElementType::Node;
  _lastId = 0;
  _maxNodeId = 0;
  _maxWayId = 0;
  _maxRelationId = 0;
}

void ApiDbReader::read(OsmMapPtr map)
{
  if (!_hasBounds())
  {
    LOG_DEBUG("Executing API read query...");
    for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
    {
      if (_returnNodesOnly && ctr != ElementType::Node)
      {
        break;
      }
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
    LOG_DEBUG("Executing API bounded read query with bounds " << bounds.toString() << "...");
    _readByBounds(map, bounds);
  }
}

void ApiDbReader::_read(OsmMapPtr map, const ElementType& elementType)
{
  //This method could possibly be replaced by the _readBounds method set to a global extent.

  long elementCount = 0;

  // contact the DB and select all
  boost::shared_ptr<QSqlQuery> elementResultsIterator =
    _getDatabase()->selectAllElements(elementType);

  //need to check isActive, rather than next() here b/c resultToElement actually calls next() and
  //it will always return an extra null node at the end (see comments in _resultToElement)
  while (elementResultsIterator->isActive())
  {
    boost::shared_ptr<Element> element =
      _resultToElement(*elementResultsIterator, elementType, *map );
    //this check is necessary due to the way _resultToElement behaves
    if (element.get())
    {
      map->addElement(element);
      LOG_VART(element);
      elementCount++;
    }
  }

  LOG_DEBUG(
    "Select all query read " << elementCount << " " << elementType.toString() << " elements.");
  LOG_VARD(map->getNodes().size());
  LOG_VARD(map->getWays().size());
  LOG_VARD(map->getRelations().size());
}

bool ApiDbReader::hasMoreElements()
{
  if (!_firstPartialReadCompleted)
  {
    //The estimated counts will only be accurate if the table has recently been analyzed.  The
    //non-estimated counts are always accurate, but can be expensive for large tables that haven't
    //recently been analyzed.  Since the count obtained here is for informational purposes only,
    //we'll just grab the estimated count and expect it to be wrong from time to time.

    const double start = Tgs::Time::getTime();
    LOG_DEBUG("Retrieving element counts and max IDs...");

    const long totalNumMapNodes = _getDatabase()->numEstimatedElements(ElementType::Node);
    long totalNumMapWays = 0;
    long totalNumMapRelations = 0;
    if (!_returnNodesOnly)
    {
      totalNumMapWays = _getDatabase()->numEstimatedElements(ElementType::Way);
      totalNumMapRelations = _getDatabase()->numEstimatedElements(ElementType::Relation);
    }

    _maxNodeId = _getDatabase()->maxId(ElementType::Node);
    LOG_VARD(_maxNodeId);
    if (!_returnNodesOnly)
    {
      _maxWayId = _getDatabase()->maxId(ElementType::Way);
      LOG_VARD(_maxWayId);
      _maxRelationId = _getDatabase()->maxId(ElementType::Relation);
      LOG_VARD(_maxRelationId);
    }

    LOG_DEBUG("Queries took " << Tgs::Time::getTime() - start << " seconds.");

    LOG_INFO(
      "Reading dataset with approximately " <<
      StringUtils::formatLargeNumber(totalNumMapNodes) << " nodes, " <<
      StringUtils::formatLargeNumber(totalNumMapWays) << " ways, and " <<
      StringUtils::formatLargeNumber(totalNumMapRelations) << " relations...");

    _firstPartialReadCompleted = true;
  }

  if (!_nextElement.get())
  {
    _nextElement = _getElementUsingIterator();
  }
  return _nextElement.get();
}

boost::shared_ptr<Element> ApiDbReader::_getElementUsingIterator()
{
  _selectElementType = _getCurrentSelectElementType();
  //LOG_VART(_selectElementType);
  //After going through all the element types, the "unknown" element type will be returned, which
  //tells hasMoreElements that we don't have any more to return.
  if (_selectElementType == ElementType::Unknown)
  {
    return boost::shared_ptr<Element>();
  }

  //see if another result is available
  if (!_elementResultIterator.get() || !_elementResultIterator->isActive())
  {
    //no results are available, so request some more results
    LOG_DEBUG("Requesting more query results...");
    //LOG_VART(_lastId);
    const double start = Tgs::Time::getTime();
    //Never ever remove the _lastId input from this call.  Doing that will turn the query into a
    //select all query.
    _elementResultIterator = _getDatabase()->selectElements(_selectElementType, _lastId);
    LOG_DEBUG("Query took " << Tgs::Time::getTime() - start << " seconds.");
  }

  //results are still available, so keep parsing through them
  boost::shared_ptr<Element> element =
    _resultToElement(*_elementResultIterator, _selectElementType, *_partialMap);

  if (!element.get())
  {
    //QSqlQuery::next() in _resultToElement will return null when at the end of records collection
    //for a given element type.  We don't want to ever return a null element as the last record to
    //clients, so here we'll just swallow it.

    LOG_TRACE("Received null element.");
    return _getElementUsingIterator();
  }

  assert(_selectElementType == element->getElementType());

  return element;
}

boost::shared_ptr<Element> ApiDbReader::readNextElement()
{
  LOG_TRACE("Retrieving next element...");

  if (hasMoreElements())
  {
    boost::shared_ptr<Element> result = _nextElement;
    _lastId = result->getId();
    //LOG_VART(_lastId);
    _nextElement.reset();
    _elementsRead++;
    //LOG_VART(_elementsRead);
    return result;
  }
  else
  {
    throw HootException("readNextElement should not be called if hasMoreElements returns false.");
  }
}

ElementType ApiDbReader::_getCurrentSelectElementType()
{
  if (_selectElementType == ElementType::Node && _lastId < _maxNodeId)
  {
    return ElementType::Node;
  }
  else if (_selectElementType == ElementType::Node && _lastId == _maxNodeId)
  {
    _lastId = 0;
    //calling finish/clear as the underlying query would have changed here to the query for the
    //next element type; this may not be necessary
    _elementResultIterator->finish();
    _elementResultIterator->clear();
    return ElementType::Way;
  }
  else if (_selectElementType == ElementType::Way && _lastId < _maxWayId)
  {
    return ElementType::Way;
  }
  else if (_selectElementType == ElementType::Way && _lastId == _maxWayId)
  {
    _lastId = 0;
    //see comment above
    _elementResultIterator->finish();
    _elementResultIterator->clear();
    return ElementType::Relation;
  }
  else if (_selectElementType == ElementType::Relation && _lastId < _maxRelationId)
  {
    return ElementType::Relation;
  }
  return ElementType::Unknown;
}

void ApiDbReader::finalizePartial()
{
  LOG_DEBUG("Finalizing read operation...");

  _partialMap.reset();

  //the query has to be freed before the database is closed
  if (_elementResultIterator)
  {
    _elementResultIterator->finish();
    _elementResultIterator->clear();
  }
  _elementResultIterator.reset();

  if (_open)
  {
    //The exception thrown by this commit will mask exception text coming from failed queries.  Not
    //sure yet how to prevent that from happening, so you may have to temporarily comment out the
    //commit statement to see query error detail when debugging.
    _getDatabase()->commit();
    _getDatabase()->close();
    _open = false;
  }
}

void ApiDbReader::close()
{
  LOG_DEBUG("Closing database reader...");
  finalizePartial();
}

boost::shared_ptr<Element> ApiDbReader::_resultToElement(QSqlQuery& resultIterator,
                                                         const ElementType& elementType,
                                                         OsmMap& map)
{
  assert(resultIterator.isActive());
  //It makes much more sense to have callers call next on the iterator before passing it into this
  //method.  However, I was getting some initialization errors with QSqlQuery when the
  //reader called it in that way during a partial map read.  So, calling next inside of this method
  //instead.  A side effect of this is that this method will always return a null element during the
  //last iteration.  Therefore, callers of this method should check resultIterator->isActive in a
  //loop in place of calling resultIterator->next() and also should check to see if the element is
  //null.
  if (resultIterator.next())
  {
    boost::shared_ptr<Element> element;
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

    return element;
  }
  else
  {
    //don't call clear here, as the prepared query may be executed again in a following iteration
    resultIterator.finish();
    return boost::shared_ptr<Element>();
  }
}

boost::shared_ptr<OGRSpatialReference> ApiDbReader::getProjection() const
{
  boost::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }
  return wgs84;
}

}
