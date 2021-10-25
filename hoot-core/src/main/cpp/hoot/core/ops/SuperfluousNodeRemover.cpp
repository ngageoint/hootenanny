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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "SuperfluousNodeRemover.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/conflate/ConflateUtils.h>

// TGS
#include <tgs/StreamUtils.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SuperfluousNodeRemover)

SuperfluousNodeRemover::SuperfluousNodeRemover() :
_removeNodes(true),
_numExplicitlyExcluded(0),
_ignoreInformationTags(false),
_taskStatusUpdateInterval(1000)
{
  setConfiguration(conf());
}

void SuperfluousNodeRemover::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _unallowedOrphanKvps = opts.getSuperfluousNodeRemoverUnallowedOrphanKvps();
  _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();

  const QSet<QString> excludeIdsStrs = opts.getSuperfluousNodeRemoverExcludeIds().toSet();
  for (QSet<QString>::const_iterator it = excludeIdsStrs.begin(); it != excludeIdsStrs.end(); ++it)
  {
    bool ok = false;
    _excludeIds.insert((*it).toLong(&ok));
    if (!ok)
    {
      throw IllegalArgumentException(
        QString("Invalid element exclude ID passed to ") + className());
    }
  }
  LOG_VARD(_excludeIds.size());
}

void SuperfluousNodeRemover::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;     // _numAffected reflects the actual number of nodes removed
  _numProcessed = 0;    // _numProcessed reflects total elements processed
  _usedNodeIds.clear();
  _superfluousNodeIds.clear();
  _numExplicitlyExcluded = 0;

  // Let's collect the IDs of all the nodes we can't remove first.

  LOG_VARD(map->getRelationCount());
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ConstRelationPtr relation = it->second;
    if (!relation)
    {
      continue;
    }
    const vector<RelationData::Entry>& members = relation->getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
      const RelationData::Entry member = members[i];
      if (member.getElementId().getType() == ElementType::Node)
      {
        _usedNodeIds.insert(member.getElementId().getId());
      }
    }

    _numProcessed++;
    if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Exempted " << StringUtils::formatLargeNumber(_usedNodeIds.size()) <<
        " nodes from removal after processing " << StringUtils::formatLargeNumber(_numProcessed) <<
        " total elements.");
    }
  }
  LOG_DEBUG(
    "Exempted " << StringUtils::formatLargeNumber(_usedNodeIds.size()) <<
    " nodes from removal that belong to relations after processing " <<
    StringUtils::formatLargeNumber(_numProcessed) << " total elements.");

  LOG_VARD(map->getWayCount());
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const ConstWayPtr& w = it->second;
    if (!w)
    {
      continue;
    }
    const vector<long>& nodeIds = w->getNodeIds();
    LOG_TRACE("Nodes belonging to " << w->getElementId() << ": " << nodeIds);
    _usedNodeIds.insert(nodeIds.begin(), nodeIds.end());
    _numProcessed += nodeIds.size();

    _numProcessed++;
    if ((_numProcessed % (_taskStatusUpdateInterval * 100) == 0) && _numProcessed != 0)
    {
      PROGRESS_INFO(
        "Exempted " << StringUtils::formatLargeNumber(_usedNodeIds.size()) <<
        " nodes from removal after processing " << StringUtils::formatLargeNumber(_numProcessed) <<
        " total elements.");
    }
  }
  LOG_DEBUG(
    "Exempted " << StringUtils::formatLargeNumber(_usedNodeIds.size()) <<
    " nodes from removal that belong to ways after processing " <<
    StringUtils::formatLargeNumber(_numProcessed) << " total elements.");

  LOG_VARD(map->getNodeCount());
  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const Node* n = it->second.get();
    if (!n)
    {
      continue;
    }
    LOG_VART(n->getElementId());
    LOG_VART(n->getTags().getNonDebugCount());

    if (_excludeIds.contains(n->getId()))
    {
      LOG_TRACE(
        "Not marking " << n->getElementId() << " for removal, as its explicitly excluded...");
      _usedNodeIds.insert(n->getId());
      _numExplicitlyExcluded++;
    }
    // There original reason behind adding this is that there a potential bug in
    // LinearSnapMerger::_snapEnds that will leave turning circle nodes orphaned from roads.
    // Turning circles are always expected to be a road way node. If its an actual bug, it should
    // eventually be fixed, but this logic will clean them up for the time being. The types we allow
    // to be orphaned are configurable in case we ever need to add others.
    else if (_usedNodeIds.find(n->getId()) == _usedNodeIds.end() &&
             n->getTags().hasAnyKvp(_unallowedOrphanKvps))
    {
      LOG_TRACE(
        "Marking " << n->getElementId() << " for removal with unallowed orphan kvp: " <<
        n->getTags().getFirstMatchingKvp(_unallowedOrphanKvps) << "...");
    }
    else if (!_ignoreInformationTags && n->getTags().getNonDebugCount() != 0)
    {
      LOG_TRACE(
        "Not marking " << n->getElementId() << " for removal due to having " <<
        n->getTags().getNonDebugCount() << " non-metadata tags...");
      _usedNodeIds.insert(n->getId());
    }
    // Since this class operates on elements with generic types, an additional check must be
    // performed here during conflation to enure we don't modify any element not associated with
    // an active conflate matcher in the current conflation configuration.
    else if (_conflateInfoCache && _ignoreInformationTags &&
             !_conflateInfoCache->elementCanBeConflatedByActiveMatcher(n->cloneSp(), className()))
    {
      LOG_TRACE(
        "Skipping processing of " << n->getElementId() << ", as it cannot be conflated by any " <<
        "actively configured conflate matcher...");
      _usedNodeIds.insert(n->getId());
    }
    else
    {
      LOG_TRACE("Marking " << n->getElementId() << " for removal...");
    }
    _numProcessed++;

    if ((_numProcessed % (_taskStatusUpdateInterval * 100) == 0) && _numProcessed != 0)
    {
      PROGRESS_INFO(
        "Exempted " << StringUtils::formatLargeNumber(_usedNodeIds.size()) <<
        " nodes from removal after processing " << StringUtils::formatLargeNumber(_numProcessed) <<
        " total elements.");
    }
  }
  LOG_DEBUG(
    "Exempted " << StringUtils::formatLargeNumber(_usedNodeIds.size()) <<
    " nodes from removal after processing " << StringUtils::formatLargeNumber(_numProcessed) <<
    " total elements.");

  LOG_VARD(_usedNodeIds.size());

  std::shared_ptr<OsmMap> reprojected;
  const NodeMap* nodesWgs84 = &nodes;
  // if the map is not in WGS84
  if (MapProjector::isGeographic(map) == false)
  {
    // Create a new copy of the map and reproject it. This way we can be sure we do the bounds
    // calculation correctly.
    reprojected = std::make_shared<OsmMap>(map);
    MapProjector::projectToWgs84(reprojected);
    nodesWgs84 = &reprojected->getNodes();
    LOG_VART(nodesWgs84->size());
  }

  // Now, let's remove any that aren't in our do not remove list.

  _numProcessed = 0;
  ElementToGeometryConverter elementConverter(map);
  for (NodeMap::const_iterator it = nodesWgs84->begin(); it != nodesWgs84->end(); ++it)
  {
    ConstNodePtr n = it->second;
    LOG_VART(n->getElementId());
    const long nodeId = n->getId();
    LOG_VART(_usedNodeIds.find(nodeId) == _usedNodeIds.end());
    if (_usedNodeIds.find(nodeId) == _usedNodeIds.end())
    {
      if (!_bounds.get() || _bounds->contains(elementConverter.convertToGeometry(n).get()))
      {
        _superfluousNodeIds.insert(nodeId);
        if (_removeNodes)
        {
          LOG_TRACE("Removing superfluous node: " << n->getElementId() << "...");
          RemoveNodeByEid::removeNodeNoCheck(map, nodeId);
        }
        else
        {
          LOG_TRACE("Counting superfluous node: " << n->getElementId() << "...");
        }
        _numAffected++;
      }
      else
      {
        LOG_TRACE("Node not in bounds: " << n->getElementId());
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
        " nodes / " << StringUtils::formatLargeNumber(_usedNodeIds.size()) << " total nodes.");
    }
    else if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(_numProcessed) <<
        " nodes / " << StringUtils::formatLargeNumber(nodesWgs84->size()) << " total nodes.");
    }
  }

  LOG_VARD(_numExplicitlyExcluded);
}

long SuperfluousNodeRemover::removeNodes(std::shared_ptr<OsmMap>& map,
                                         const bool ignoreInformationTags,
                                         const std::shared_ptr<geos::geom::Geometry>& bounds)
{
  SuperfluousNodeRemover nodeRemover;
  nodeRemover.setIgnoreInformationTags(ignoreInformationTags);
  if (bounds.get())
  {
    nodeRemover.setBounds(bounds);
  }
  LOG_INFO(nodeRemover.getInitStatusMessage());
  nodeRemover.apply(map);
  LOG_DEBUG(nodeRemover.getCompletedStatusMessage());
  return nodeRemover.getNumFeaturesAffected();
}

std::set<long> SuperfluousNodeRemover::collectSuperfluousNodeIds(
  std::shared_ptr<OsmMap>& map, const bool ignoreInformationTags,
  const std::shared_ptr<geos::geom::Geometry>& bounds)
{
  SuperfluousNodeRemover nodeIdCollector;
  nodeIdCollector.setIgnoreInformationTags(ignoreInformationTags);
  nodeIdCollector.setRemoveNodes(false);
  if (bounds.get())
  {
    nodeIdCollector.setBounds(bounds);
  }
  QString msg = nodeIdCollector.getInitStatusMessage().replace("Removing", "Collecting");
  LOG_STATUS(msg);
  nodeIdCollector.apply(map);
  msg = nodeIdCollector.getCompletedStatusMessage().replace("Removed", "Collected");
  LOG_DEBUG(msg);
  return nodeIdCollector.getSuperfluousNodeIds();
}

}
