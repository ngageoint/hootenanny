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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ApiDbReader.h"

// Hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/TableType.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// tgs
#include <tgs/System/Time.h>

// Qt
#include <QSet>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int ApiDbReader::logWarnCount = 0;

ApiDbReader::ApiDbReader() :
_useDataSourceIds(true),
_status(Status::Invalid),
_open(false),
_defaultCircularError(ConfigOptions().getCircularErrorDefaultValue()),
_readFullThenCropOnBounded(ConfigOptions().getApidbReaderReadFullThenCropOnBounded()),
_returnNodesOnly(false),
_keepStatusTag(ConfigOptions().getReaderKeepStatusTag()),
_circularErrorTagKeys(ConfigOptions().getCircularErrorTagKeys()),
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_totalNumMapNodes(0),
_totalNumMapWays(0),
_totalNumMapRelations(0),
_numNodesRead(0),
_numWaysRead(0),
_numRelationsRead(0),
_keepImmediatelyConnectedWaysOutsideBounds(
  ConfigOptions().getBoundsKeepImmediatelyConnectedWaysOutsideBounds())
{
}

bool ApiDbReader::isSupported(const QString& urlStr)
{
  QUrl url(urlStr);
  return _getDatabase()->isSupported(url);
}

void ApiDbReader::setBoundingBox(const QString& bbox)
{
  setBounds(GeometryUtils::boundsFromString(bbox));
}

void ApiDbReader::setOverrideBoundingBox(const QString& bbox)
{
  _overrideBounds = GeometryUtils::boundsFromString(bbox);
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
  _totalNumMapNodes = 0;
  _totalNumMapWays = 0;
  _totalNumMapRelations = 0;
  _numNodesRead = 0;
  _numWaysRead = 0;
  _numRelationsRead = 0;
}

bool ApiDbReader::_hasBounds()
{
  return _bounds.get() || _overrideBounds.get();
}

ElementId ApiDbReader::_mapElementId(const OsmMap& map, ElementId oldId)
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
    const int statusInt = statusStr.toInt(&ok);
    Status status = statusInt;
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
    // We don't need to carry this tag around once the value is set on the element...it will
    // be reinstated by some writers, though.
    if (!_keepStatusTag) { tags.remove(MetadataTags::HootStatus()); }
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

  // Arbitrarily pick the first error tag found. If the element has both, the last one parsed will
  // be used. We're not expecting elements to have more than one CE tag.
  const QString ceKey = tags.getFirstKey(_circularErrorTagKeys);
  if (!ceKey.isEmpty())
  {
    element->setCircularError(tags.get(ceKey).toDouble(&ok));
    if (!ok)
    {
      try
      {
        double tv = tags.getLength(ceKey).value();
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
          LOG_WARN("Error parsing " + ceKey + ".");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
    // Preserving original behavior of the reader here. Not all readers remove this key, so we may
    // want to unify their behavior. It may be ok, since its a hoot specific key. We wouldn't,
    // however, want to remove other error tags.
    if (ceKey == MetadataTags::ErrorCircular())
    {
      // We don't need to carry this tag around once the value is set on the element...it will
      // be reinstated by some writers, though.
      tags.remove(ceKey);
    }
  }
}

void ApiDbReader::_readWaysByNodeIds(OsmMapPtr map, const QSet<QString>& nodeIds,
                                     QSet<QString>& wayIds, QSet<QString>& additionalNodeIds,
                                     long& nodeCount, long& wayCount)
{
  LOG_DEBUG("Retrieving way IDs referenced by the selected nodes...");

  std::shared_ptr<QSqlQuery> wayIdItr = _getDatabase()->selectWayIdsByWayNodeIds(nodeIds);
  while (wayIdItr->next())
  {
    const long wayId = (*wayIdItr).value(0).toLongLong();
    LOG_VART(ElementId(ElementType::Way, wayId));
    wayIds.insert(QString::number(wayId));
  }
  LOG_VART(wayIds.size());

  if (!wayIds.empty())
  {
    // If the appropriate option is enabled, here we'll add the IDs of all ways that fall outside
    // of the requested bounds but are directly connected to a way that is being returned by the
    // query.
    QSet<QString> connectedWayIds;
    LOG_VART(_keepImmediatelyConnectedWaysOutsideBounds);
    if (_keepImmediatelyConnectedWaysOutsideBounds)
    {
      LOG_DEBUG("Retrieving way IDs immediately connected to the current ways...");
      connectedWayIds = _getDatabase()->selectConnectedWayIds(wayIds);
      LOG_VART(connectedWayIds);
      LOG_VART(connectedWayIds.size());
      wayIds.unite(connectedWayIds);
      LOG_VART(wayIds);
      LOG_VART(wayIds.size());
    }

    LOG_DEBUG("Retrieving ways by way ID...");
    std::shared_ptr<QSqlQuery> wayItr =
      _getDatabase()->selectElementsByElementIdList(wayIds, TableType::Way);
    const bool tagConnectedWays =
      ConfigOptions().getBoundsTagImmediatelyConnectedOutOfBoundsWays();
    LOG_VART(tagConnectedWays);
    while (wayItr->next())
    {
      WayPtr way = _resultToWay(*wayItr, *map);
      // If the appropriate option is enabled, we'll tag the connected out of bounds ways that we
      // just identified. This may be helpful later when creating replacement changesets.
      if (tagConnectedWays && connectedWayIds.contains(QString::number(way->getId())))
      {
        way->getTags().set(MetadataTags::HootConnectedWayOutsideBounds(), "yes");
      }
      map->addElement(way);
      LOG_VART(way);
      wayCount++;
    }

    LOG_DEBUG("Retrieving way node IDs referenced by the selected ways...");
    std::shared_ptr<QSqlQuery> additionalWayNodeIdItr =
      _getDatabase()->selectWayNodeIdsByWayIds(wayIds);
    while (additionalWayNodeIdItr->next())
    {
      const long nodeId = (*additionalWayNodeIdItr).value(0).toLongLong();
      LOG_VART(ElementId(ElementType::Node, nodeId));
      additionalNodeIds.insert(QString::number(nodeId));
    }

    // subtract nodeIds from additionalNodeIds, so no dupes get added
    LOG_VART(nodeIds.size());
    LOG_VART(additionalNodeIds.size());
    additionalNodeIds = additionalNodeIds.subtract(nodeIds);
    LOG_VART(additionalNodeIds.size());

    if (!additionalNodeIds.empty())
    {
      LOG_DEBUG(
        "Retrieving nodes falling outside of the query bounds but belonging to a selected way...");
      std::shared_ptr<QSqlQuery> additionalWayNodeItr =
        _getDatabase()->selectElementsByElementIdList(additionalNodeIds, TableType::Node);
      while (additionalWayNodeItr->next())
      {
        NodePtr node = _resultToNode(*additionalWayNodeItr, *map);
        map->addElement(node);
        LOG_VART(node);
        nodeCount++;
      }
    }
  }
}

void ApiDbReader::_fullRead(OsmMapPtr map)
{
  LOG_DEBUG("Executing API full read query...");
  for (int ctr = ElementType::Node; ctr != ElementType::Unknown; ctr++)
  {
    if (_returnNodesOnly && ctr != ElementType::Node)
    {
      break;
    }
    _read(map, static_cast<ElementType::Type>(ctr));
  }
}

void ApiDbReader::_readByBounds(OsmMapPtr map, const Envelope& bounds)
{
  long boundedNodeCount = 0;
  long boundedWayCount = 0;
  long boundedRelationCount = 0;

  LOG_DEBUG("Retrieving nodes within the query bounds...");
  std::shared_ptr<QSqlQuery> nodeItr = _getDatabase()->selectNodesByBounds(bounds);
  // Element IDs are stored as strings to allow inserting them directly into the select query
  // strings.
  QSet<QString> nodeIds;
  while (nodeItr->next())
  {
    const QSqlQuery resultIterator = *nodeItr;
    NodePtr node = _resultToNode(resultIterator, *map);
    LOG_VART(node);
    map->addElement(node);
    boundedNodeCount++;
    // Don't use the mapped id from the node object here, b/c we want don't want to use mapped ids
    // with any queries, because mapped ids may not exist yet.
    const long nodeId = resultIterator.value(0).toLongLong();
    LOG_VART(ElementId(ElementType::Node, nodeId));
    nodeIds.insert(QString::number(nodeId));
  }
  LOG_VART(nodeIds.size());

  if (!_returnNodesOnly)
  {
    if (!nodeIds.empty())
    {
      QSet<QString> wayIds;
      QSet<QString> additionalWayNodeIds;
      _readWaysByNodeIds(
        map, nodeIds, wayIds, additionalWayNodeIds, boundedNodeCount, boundedWayCount);
      nodeIds.unite(additionalWayNodeIds);
      LOG_VART(nodeIds.size());
      LOG_VART(wayIds.size());

      LOG_DEBUG("Retrieving relation IDs referenced by the selected ways and nodes...");
      QSet<QString> relationIds;
      assert(nodeIds.size() > 0);
      std::shared_ptr<QSqlQuery> relationIdItr =
        _getDatabase()->selectRelationIdsByMemberIds(nodeIds, ElementType::Node);
      while (relationIdItr->next())
      {
        const long relationId = (*relationIdItr).value(0).toLongLong();
        LOG_VART(ElementId(ElementType::Relation, relationId));
        relationIds.insert(QString::number(relationId));
      }
      if (!wayIds.empty())
      {
        relationIdItr = _getDatabase()->selectRelationIdsByMemberIds(wayIds, ElementType::Way);
        while (relationIdItr->next())
        {
          const long relationId = (*relationIdItr).value(0).toLongLong();
          LOG_VART(ElementId(ElementType::Relation, relationId));
          relationIds.insert(QString::number(relationId));
        }
      }
      LOG_VART(relationIds.size());

      //  Iterate all relations (and sub-relations) that are "within" the bounds
      QSet<QString> completedRelationIds;
      while (!relationIds.empty())
      {
        QSet<QString> newNodes;
        QSet<QString> newWays;
        QSet<QString> newRelations;

        LOG_DEBUG("Retrieving relations by relation ID...");
        std::shared_ptr<QSqlQuery> relationItr =
          _getDatabase()->selectElementsByElementIdList(relationIds, TableType::Relation);
        while (relationItr->next())
        {
          RelationPtr relation = _resultToRelation(*relationItr, *map);
          map->addElement(relation);
          const vector<RelationData::Entry>& members = relation->getMembers();
          //  Iterate all members so that they can be retrieved later
          for (vector<RelationData::Entry>::const_iterator it = members.begin();
               it != members.end(); ++it)
          {
            ElementType type = it->getElementId().getType();
            QString id = QString::number(it->getElementId().getId());
            if (type == ElementType::Node)
              newNodes.insert(id);
            else if (type == ElementType::Way)
              newWays.insert(id);
            else if (type == ElementType::Relation)
              newRelations.insert(id);
            else
            {
              LOG_WARN("Unknown relation member type");
            }
          }
          LOG_VART(relation);
          boundedRelationCount++;
          //  Keep track of all relations that have been added to the map so we don't try them again
          completedRelationIds.insert(QString::number(relation->getId()));
        }

        //  Clear the relations that we are iterating on, it is then filled with new relations later
        relationIds.clear();

        //  Iterate any new nodes that are members of relations that need to be queried
        newNodes = newNodes.subtract(nodeIds);
        if (!newNodes.empty())
        {
          std::shared_ptr<QSqlQuery> queryItr =
            _getDatabase()->selectElementsByElementIdList(newNodes, TableType::Node);
          while (queryItr->next())
          {
            const QSqlQuery resultIterator = *queryItr;
            NodePtr node = _resultToNode(resultIterator, *map);
            LOG_VART(node);
            map->addElement(node);
            boundedNodeCount++;
            const long nodeId = resultIterator.value(0).toLongLong();
            LOG_VART(ElementId(ElementType::Node, nodeId));
            nodeIds.insert(QString::number(nodeId));
          }
        }

        //  Iterate any new ways that are members of relations that need to be queried
        newWays = newWays.subtract(wayIds);
        if (!newWays.empty())
        {
          QSet<QString> additionalNodeIds;

          LOG_DEBUG("Retrieving ways by way ID...");
          std::shared_ptr<QSqlQuery> wayItr =
            _getDatabase()->selectElementsByElementIdList(newWays, TableType::Way);
          while (wayItr->next())
          {
            WayPtr way = _resultToWay(*wayItr, *map);
            map->addElement(way);
            LOG_VART(way);
            boundedWayCount++;
          }

          LOG_DEBUG("Retrieving way node IDs referenced by the selected ways...");
          std::shared_ptr<QSqlQuery> additionalWayNodeIdItr =
            _getDatabase()->selectWayNodeIdsByWayIds(newWays);
          while (additionalWayNodeIdItr->next())
          {
            const long nodeId = (*additionalWayNodeIdItr).value(0).toLongLong();
            LOG_VART(ElementId(ElementType::Node, nodeId));
            additionalNodeIds.insert(QString::number(nodeId));
          }

          // subtract nodeIds from additionalWayNodeIds, so no dupes get added
          LOG_VART(nodeIds.size());
          LOG_VART(additionalNodeIds.size());
          additionalNodeIds = additionalNodeIds.subtract(nodeIds);
          LOG_VART(additionalNodeIds.size());

          if (!additionalNodeIds.empty())
          {
            LOG_DEBUG(
              "Retrieving nodes falling outside of the query bounds but belonging to a selected " <<
              "way...");
            std::shared_ptr<QSqlQuery> additionalWayNodeItr =
              _getDatabase()->selectElementsByElementIdList(additionalNodeIds, TableType::Node);
            while (additionalWayNodeItr->next())
            {
              NodePtr node = _resultToNode(*additionalWayNodeItr, *map);
              map->addElement(node);
              LOG_VART(node);
              boundedNodeCount++;
            }
          }
        }

        //  Get the set of new relations found minus anything that has already been completed
        relationIds = newRelations.subtract(completedRelationIds);
      }
    }
  }

  LOG_DEBUG(
    "Bounded query read " << (boundedNodeCount + boundedWayCount + boundedRelationCount) <<
    " total elements.");
  LOG_VART(boundedNodeCount);
  LOG_VART(boundedWayCount);
  LOG_VART(boundedRelationCount);

  // The default behavior of the db bounded read is to return the entirety of features found
  // within the bounds, even if sections of those features exist outside the bounds. Only run the
  // crop operation if the crop related options used by the convert command are different than the
  // default behavior. Clearly, it would be more efficient to run a different set of initial queries
  // to pull the features back already cropped the way we want them from the start and skip this
  // step completely. That's possibly an optimization to look into doing in the future.
  ConfigOptions conf;
  if (!conf.getBoundsKeepEntireFeaturesCrossingBounds() ||
       conf.getBoundsKeepOnlyFeaturesInsideBounds())
  {
    // We've already handled keeping immediately connected oob ways during the query, so don't need
    // to worry about it here.
    IoUtils::cropToBounds(map, _bounds);
   }

  LOG_VARD(map->getNodes().size());
  LOG_VARD(map->getWays().size());
  LOG_VARD(map->getRelations().size());
}

void ApiDbReader::_readByBounds2(OsmMapPtr map, const Envelope& bounds)
{
  _fullRead(map);
  IoUtils::cropToBounds(map, bounds, _keepImmediatelyConnectedWaysOutsideBounds);
}

void ApiDbReader::read(const OsmMapPtr& map)
{
  //  Set the map source
  map->appendSource(_url);

  if (!_hasBounds())
  {
    _fullRead(map);
  }
  else
  {
    // api db bounds reading is based off of quad tiles which only operates on rectangular bounds,
    // so we have to pass an envelope here. The actual bounds is used to crop down the envelope
    // queried results later.
    geos::geom::Envelope bounds;
    LOG_VARD(_overrideBounds.get());
    LOG_VARD(_bounds.get());
    if (_overrideBounds.get())
    {
      bounds = *(_overrideBounds->getEnvelopeInternal());
    }
    else
    {
      bounds = *(_bounds->getEnvelopeInternal());
    }
    if (!_readFullThenCropOnBounded)
    {
      LOG_DEBUG(
        "Executing API bounded read query via SQL filtering at bounds: ..." <<
        GeometryUtils::envelopeToString(bounds).right(50) << "...");
      _readByBounds(map, bounds);
    }
    else
    {
      // The cropped query read from db inputs may get slower for very large datasets. The index
      // added as part of #4192 alleviated this quite a bit but there still can be a runtime
      // performance difference. Therefore, if we we need to perform a cropped query from a very
      // large dataset, we can allow the full dataset to be read in from the db input and then crop
      // it after the fact as a runtime performance optimization at the expense of extra memory
      //usage.
      LOG_DEBUG(
        "Executing API bounded read query via read all then crop at bounds: ..." <<
        GeometryUtils::envelopeToString(bounds).right(50) << "...");
      _readByBounds2(map, bounds);
    }
  }
}

void ApiDbReader::_read(OsmMapPtr map, const ElementType& elementType)
{
  // This method could possibly be replaced by the _readBounds method set to a global extent.

  long elementCount = 0;

  // contact the DB and select all
  std::shared_ptr<QSqlQuery> elementResultsIterator =
    _getDatabase()->selectAllElements(elementType);

  // Need to check isActive, rather than next() here b/c resultToElement actually calls next() and
  // it will always return an extra null node at the end (see comments in _resultToElement).
  while (elementResultsIterator->isActive())
  {
    std::shared_ptr<Element> element =
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
    LOG_DEBUG("Retrieving element counts and max IDs for: " << _url.right(25) << "...");

    _totalNumMapNodes = _getDatabase()->numEstimatedElements(ElementType::Node);
    _totalNumMapWays = 0;
    _totalNumMapRelations = 0;
    if (!_returnNodesOnly)
    {
      _totalNumMapWays = _getDatabase()->numEstimatedElements(ElementType::Way);
      _totalNumMapRelations = _getDatabase()->numEstimatedElements(ElementType::Relation);
    }

    _maxNodeId = _getDatabase()->maxId(ElementType::Node);
    LOG_VART(_maxNodeId);
    if (!_returnNodesOnly)
    {
      _maxWayId = _getDatabase()->maxId(ElementType::Way);
      LOG_VART(_maxWayId);
      _maxRelationId = _getDatabase()->maxId(ElementType::Relation);
      LOG_VART(_maxRelationId);
    }

    LOG_DEBUG("Queries took " << Tgs::Time::getTime() - start << " seconds.");

    LOG_INFO(
      "Reading dataset with approximately " <<
      StringUtils::formatLargeNumber(_totalNumMapNodes) << " nodes, " <<
      StringUtils::formatLargeNumber(_totalNumMapWays) << " ways, and " <<
      StringUtils::formatLargeNumber(_totalNumMapRelations) << " relations...");

    _firstPartialReadCompleted = true;
  }

  if (!_nextElement.get())
  {
    _nextElement = _getElementUsingIterator();
  }
  return _nextElement.get();
}

std::shared_ptr<Element> ApiDbReader::_getElementUsingIterator()
{
  _selectElementType = _getCurrentSelectElementType();
  //LOG_VART(_selectElementType);
  //After going through all the element types, the "unknown" element type will be returned, which
  //tells hasMoreElements that we don't have any more to return.
  if (_selectElementType == ElementType::Unknown)
  {
    return std::shared_ptr<Element>();
  }

  //see if another result is available
  if (!_elementResultIterator.get() || !_elementResultIterator->isActive())
  {
    //no results are available, so request some more results
    LOG_DEBUG(
      "Requesting more query results from: " << _url.right(50) << ", for element type: " <<
       _selectElementType.toString() << ", starting with ID: " << _lastId << "...");
    const double start = Tgs::Time::getTime();
    //NEVER remove the _lastId input from the call to selectElements here.  Doing that will
    //basically turn the query into a select all query (up to the max element limit).
    _elementResultIterator = _getDatabase()->selectElements(_selectElementType, _lastId);
    LOG_DEBUG("Query took " << Tgs::Time::getTime() - start << " seconds.");
  }

  //results are still available, so keep parsing through them
  std::shared_ptr<Element> element =
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

std::shared_ptr<Element> ApiDbReader::readNextElement()
{
  LOG_TRACE("Retrieving next element...");

  if (hasMoreElements())
  {
    std::shared_ptr<Element> result = _nextElement;
    _lastId = result->getId();
    //LOG_VART(_lastId);
    _nextElement.reset();
    _elementsRead++;
    //LOG_VART(_elementsRead);

    if (_selectElementType == ElementType::Node)
    {
      _numNodesRead++;
    }
    else if (_selectElementType == ElementType::Way)
    {
      _numWaysRead++;
    }
    if (_selectElementType == ElementType::Relation)
    {
      _numRelationsRead++;
    }

    if (_numNodesRead % _statusUpdateInterval == 0 && _selectElementType == ElementType::Node)
    {
      PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numNodesRead) << " nodes.");
    }
    if (_numWaysRead % _statusUpdateInterval == 0 && _selectElementType == ElementType::Way)
    {
      PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numWaysRead) << " ways.");
    }
    if (_numRelationsRead % _statusUpdateInterval == 0 &&
        _selectElementType == ElementType::Relation)
    {
      PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRelationsRead) << " relations.");
    }

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
  LOG_TRACE("Finalizing read operation...");

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

  if (_numNodesRead > 0 || _numWaysRead > 0 || _numRelationsRead > 0)
  {
    LOG_INFO(
      "Read " << StringUtils::formatLargeNumber(_numNodesRead) << " nodes, " <<
      StringUtils::formatLargeNumber(_numWaysRead) << " ways, and " <<
      StringUtils::formatLargeNumber(_numRelationsRead) << " relations.");
    _numNodesRead = 0;
    _numWaysRead = 0;
    _numRelationsRead = 0;
  }
}

void ApiDbReader::close()
{
  LOG_TRACE("Closing database reader...");
  finalizePartial();
}

std::shared_ptr<Element> ApiDbReader::_resultToElement(
  QSqlQuery& resultIterator, const ElementType& elementType, OsmMap& map)
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
    std::shared_ptr<Element> element;
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
    return std::shared_ptr<Element>();
  }
}

std::shared_ptr<OGRSpatialReference> ApiDbReader::getProjection() const
{
  std::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }
  return wgs84;
}

}
