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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "IntersectionSplitter.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/OsmMap.h>

// Qt
#include <QDebug>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, IntersectionSplitter)

IntersectionSplitter::IntersectionSplitter()
{

}

IntersectionSplitter::IntersectionSplitter(boost::shared_ptr<OsmMap> map)
{
  _map = map;
}

void IntersectionSplitter::_mapNodesToWay(boost::shared_ptr<Way> way)
{
  long wId = way->getId();

  const std::vector<long>& nodes = way->getNodeIds();
  for (size_t i = 0; i < nodes.size(); i++)
  {
    _nodeToWays.insert(nodes[i], wId);

    if (_nodeToWays.count(nodes[i]) > 1)
    {
      _todoNodes.insert(nodes[i]);
    }
  }
}

void IntersectionSplitter::_mapNodesToWays()
{
  _nodeToWays.clear();

  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    boost::shared_ptr<Way> w = it->second;

    bool isNetworkType = false;

    if (OsmSchema::getInstance().isLinearHighway(w->getTags(), w->getElementType()) ||
      OsmSchema::getInstance().isLinearWaterway(*w))
    {
      isNetworkType  = true;
    }
    // if the way isn't a network type, maybe it is part of a relation that is a network type.
    else
    {
      const set<long>& relations = _map->getIndex().getElementToRelationMap()->getRelationByElement(
        w->getElementId());

      foreach (long rid, relations)
      {
        ElementPtr r = _map->getRelation(rid);
        const Tags& tags = r->getTags();
        if (OsmSchema::getInstance().isLinearHighway(tags, ElementType::Relation) ||
          OsmSchema::getInstance().isLinearWaterway(*r))
        {
          isNetworkType  = true;
        }
      }
    }

    if (isNetworkType)
    {
      _mapNodesToWay(w);
    }
  }
}

void IntersectionSplitter::_removeWayFromMap(boost::shared_ptr<Way> way)
{
  long wId = way->getId();

  const std::vector<long>& nodes = way->getNodeIds();
  for (size_t i = 0; i < nodes.size(); i++)
  {
    _nodeToWays.remove(nodes[i], wId);
  }
}

void IntersectionSplitter::splitIntersections(boost::shared_ptr<OsmMap> map)
{
  IntersectionSplitter is(map);
  return is.splitIntersections();
}

void IntersectionSplitter::splitIntersections()
{
  // make a map of nodes to ways.
  _mapNodesToWays();
  _wayReplacements.clear();

  // go through all the nodes
  bool todoLogged = false;
  while (_todoNodes.isEmpty() == false)
  {
    long nodeId = *_todoNodes.begin();
    //  Remove the node first in case it needs to be reprocessed later
    _todoNodes.remove(nodeId);

    if (Log::getInstance().isInfoEnabled() && _todoNodes.size() % 1000 == 0 && _todoNodes.size() > 0)
    {
      cout << "  Intersection splitter todo: " << _todoNodes.size() << "       \r";
      cout.flush();
      todoLogged = true;
    }
    // if the node is part of two or more ways
    if (_nodeToWays.count(nodeId) >= 2)
    {
      // evaluate each way for splitting
      QList<long> ways = _nodeToWays.values(nodeId);
      for (QList<long>::const_iterator way = ways.begin(); way != ways.end(); ++way)
      {
        _splitWay(*way, nodeId);
      }
    }
  }

  if (Log::getInstance().isInfoEnabled() && todoLogged)
  {
    cout << endl;
  }
}

void IntersectionSplitter::_splitWay(long wayId, long nodeId)
{
  boost::shared_ptr<Way> way = _map->getWay(wayId);
  if (way == 0)
  {
    LOG_TRACE("way at " << wayId << " does not exist.");
    return;
  }

  LOG_TRACE(
    "Splitting way: " << way->getElementId() << " at node: " <<
    ElementId(ElementType::Node, nodeId));

  // find the first index of the split node that isn't an endpoint.
  int firstIndex = -1;
  const std::vector<long>& nodeIds = way->getNodeIds();
  for (size_t i = 1; i < nodeIds.size() - 1; i++)
  {
    if (nodeIds[i] == nodeId)
    {
      firstIndex = i;
      break;
    }
  }
  LOG_VART(firstIndex);

  // if the first index wasn't an endpoint.
  if (firstIndex != -1)
  {
    QList<long> ways = _nodeToWays.values(nodeId);
    LOG_VART(ways);
    int concurrent_count = 0;
    int otherWays_count = ways.count() - 1;
    for (QList<long>::const_iterator it = ways.begin(); it != ways.end(); ++it)
    {
      long compWayId = *it;
      LOG_VART(compWayId);
      //  Don't compare it against itself
      if (wayId == compWayId)
        continue;

      //  Get the way info to make the comparison
      if (_wayReplacements.contains(compWayId))
      {
        compWayId = _wayReplacements[compWayId];
      }
      boost::shared_ptr<Way> comp = _map->getWay(compWayId);
      LOG_VART(comp.get());
      const std::vector<long>& compIds = comp->getNodeIds();
      long idx = comp->getNodeIndex(nodeId);

      //  Endpoints of the other way should be split
      if (idx < 1 || idx > (long)compIds.size() - 1)
        continue;

      //  Check both in forward and reverse for shared nodes in the way
      if ((nodeIds[firstIndex - 1] == compIds[idx - 1] && nodeIds[firstIndex + 1] == compIds[idx + 1]) ||
          (nodeIds[firstIndex - 1] == compIds[idx + 1] && nodeIds[firstIndex + 1] == compIds[idx - 1]))
        concurrent_count++;
    }

    // TODO: Need to figure out why this doesn't play nice with network conflation
    //  A split point is found when there is at least one non-concurrent way at this node
    if (concurrent_count < otherWays_count)
    {
      // split the way and remove it from the map
      WayLocation wl(_map, way, firstIndex, 0.0);
      vector< boost::shared_ptr<Way> > splits = WaySplitter::split(_map, way, wl);

      // if a split occurred.
      if (splits.size() > 1)
      {
        LOG_VART(way->getElementId());
        LOG_VART(way->getStatus());
        LOG_VART(splits[0]->getElementId());

        const ElementId splitWayId = way->getElementId();

        QList<ElementPtr> newWays;
        foreach (const boost::shared_ptr<Way>& w, splits)
        {
          newWays.append(w);
        }

        // make sure any ways that are part of relations continue to be part of those relations after
        // they're split.
        _map->replace(way, newWays);

        // see comments for similar functionality in HighwaySnapMerger::_mergePair
        if (ConfigOptions().getPreserveUnknown1ElementIdWhenModifyingFeatures() &&
            way->getStatus() == Status::Unknown1)
        {
          LOG_TRACE(
            "Setting unknown1 " << way->getElementId().getId() << " on " <<
            splits[0]->getElementId() << "...");
          ElementPtr newWaySegment(_map->getElement(splits[0]->getElementId())->clone());
          newWaySegment->setId(way->getElementId().getId());
          _map->replace(_map->getElement(splits[0]->getElementId()), newWaySegment);
          _wayReplacements[splits[0]->getElementId().getId()] = way->getElementId().getId();
        }

        _removeWayFromMap(way);

        // go through all the resulting splits
        for (size_t i = 0; i < splits.size(); i++)
        {
          // add the new ways nodes just in case the way was self intersecting.
          _mapNodesToWay(splits[i]);
        }

        LOG_VART(_map->containsElement(splitWayId));
      }
    }
  }
}

void IntersectionSplitter::apply(boost::shared_ptr<OsmMap> &map)
{
  splitIntersections(map);
}

}
