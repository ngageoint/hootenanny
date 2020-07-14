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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "SuperfluousNodeRemover.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/RemoveNodeByEid.h>

// Standard
#include <iostream>

// TGS
#include <tgs/StreamUtils.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SuperfluousNodeRemover)

SuperfluousNodeRemover::SuperfluousNodeRemover() :
_ignoreInformationTags(false),
_unallowedOrphanKvps(ConfigOptions().getSuperfluousNodeRemoverUnallowedOrphanKvps()),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void SuperfluousNodeRemover::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;     // _numAffected reflects the actual number of nodes removed
  _numProcessed = 0;    // _numProcessed reflects total elements processed
  _usedNodes.clear();

  // Let's collect the IDs of all the nodes we can't remove first.

  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ConstRelationPtr relation = it->second;
    const vector<RelationData::Entry>& members = relation->getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
      const RelationData::Entry member = members[i];
      if (member.getElementId().getType() == ElementType::Node)
      {
        _usedNodes.insert(member.getElementId().getId());
      }
    }

    _numProcessed++;
    if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Exempted " << StringUtils::formatLargeNumber(_usedNodes.size()) <<
        " nodes from removal after processing " << StringUtils::formatLargeNumber(_numProcessed) <<
        " total elements.");
    }
  }

  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const ConstWayPtr& w = it->second;
    const vector<long>& nodeIds = w->getNodeIds();
    LOG_VART(nodeIds);
    _usedNodes.insert(nodeIds.begin(), nodeIds.end());
    _numProcessed += nodeIds.size();

    _numProcessed++;
    if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Exempted " << StringUtils::formatLargeNumber(_usedNodes.size()) <<
        " nodes from removal after processing " << StringUtils::formatLargeNumber(_numProcessed) <<
        " total elements.");
    }
  }

  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const Node* n = it->second.get();
    LOG_VART(n->getElementId());
    LOG_VART(n->getTags().getNonDebugCount());

    // There original reason behind adding this is that there a potential bug in
    // HighwaySnapMerger::_snapEnds that will leave turning circle nodes orphaned from roads.
    // Turning circles are always expected to be a road way node. If its an actual bug, it should
    // eventually be fixed, but this logic will clean them up for the time being. The types we allow
    // to be orphaned are configurable in case we ever need to add others.
    if (_usedNodes.find(n->getId()) == _usedNodes.end() &&
        n->getTags().hasAnyKvp(_unallowedOrphanKvps))
    {
      LOG_TRACE("Skipping " << n->getElementId() << " with unallowed orphan kvp...");
    }
    else if (!_ignoreInformationTags && n->getTags().getNonDebugCount() != 0)
    {
      LOG_TRACE(
        "Not marking " << n->getElementId() << " for removal due to having non-metadata tags...");
      _usedNodes.insert(n->getId());
    }
    else
    {
      LOG_TRACE("Marking " << n->getElementId() << " for removal...");
    }
    _numProcessed++;

    if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Exempted " << StringUtils::formatLargeNumber(_usedNodes.size()) <<
        " nodes from removal after processing " << StringUtils::formatLargeNumber(_numProcessed) <<
        " total elements.");
    }
  }
  LOG_VART(_usedNodes.size());
  //LOG_VART(_usedNodes);

  std::shared_ptr<OsmMap> reprojected;
  const NodeMap* nodesWgs84 = &nodes;
  // if the map is not in WGS84
  if (MapProjector::isGeographic(map) == false)
  {
    // create a new copy of the map and reproject it. This way we can be sure we do the bounds
    // calculation correctly.
    reprojected.reset(new OsmMap(map));
    MapProjector::projectToWgs84(reprojected);
    nodesWgs84 = &reprojected->getNodes();
    LOG_VART(nodesWgs84->size());
  }

  // Now, let's remove any that aren't in our do not remove list.

  _numProcessed = 0;
  for (NodeMap::const_iterator it = nodesWgs84->begin(); it != nodesWgs84->end(); ++it)
  {
    const Node* n = it->second.get();
    LOG_VART(n->getElementId());
    const long nodeId = n->getId();
    LOG_VART(_usedNodes.find(nodeId) == _usedNodes.end());
    if (_usedNodes.find(nodeId) == _usedNodes.end())
    {
      if (_bounds.isNull() || _bounds.contains(n->getX(), n->getY()))
      {
        LOG_TRACE("Removing node: " << n->getElementId() << "...");
        RemoveNodeByEid::removeNodeNoCheck(map, nodeId);
        _numAffected++;
      }
      else
      {
        LOG_TRACE("node not in bounds. " << n->getElementId());
        LOG_VART(_bounds);
      }
    }
    else
    {
      LOG_TRACE("Not removing node: " << n->getElementId() << "...");
    }
    _numProcessed++;

    if (_numAffected > 0 && _numAffected % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Removed " << StringUtils::formatLargeNumber(_numAffected) <<
        " nodes / " << StringUtils::formatLargeNumber(_usedNodes.size()) << " total nodes.");
    }
    else if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(_numProcessed) <<
        " nodes / " << StringUtils::formatLargeNumber(nodesWgs84->size()) << " total nodes.");
    }
  }
}

long SuperfluousNodeRemover::removeNodes(std::shared_ptr<OsmMap>& map,
                                         const bool ignoreInformationTags,
                                         const geos::geom::Envelope& e)
{
  SuperfluousNodeRemover nodeRemover;
  nodeRemover.setIgnoreInformationTags(ignoreInformationTags);
  if (!e.isNull())
  {
    nodeRemover.setBounds(e);
  }
  LOG_INFO(nodeRemover.getInitStatusMessage());
  nodeRemover.apply(map);
  LOG_INFO(nodeRemover.getCompletedStatusMessage());
  return nodeRemover.getNumFeaturesAffected();
}

void SuperfluousNodeRemover::setBounds(const Envelope &bounds)
{
  _bounds = bounds;
}

void SuperfluousNodeRemover::readObject(QDataStream& is)
{
  bool hasBounds;
  is >> hasBounds;
  if (hasBounds)
  {
    double minx, miny, maxx, maxy;
    is >> minx >> miny >> maxx >> maxy;
    _bounds = Envelope(minx, maxx, miny, maxy);
  }
}

void SuperfluousNodeRemover::writeObject(QDataStream& os) const
{
  if (_bounds.isNull())
  {
    os << false;
  }
  else
  {
    os << true;
    os << _bounds.getMinX() << _bounds.getMinY() << _bounds.getMaxX() << _bounds.getMaxY();
  }
}

}
