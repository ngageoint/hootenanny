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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "PoiPolygonInvalidReviewNodeRemover.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, PoiPolygonInvalidReviewNodeRemover)

PoiPolygonInvalidReviewNodeRemover::PoiPolygonInvalidReviewNodeRemover() :
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void PoiPolygonInvalidReviewNodeRemover::apply(const std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;     // _numAffected reflects the actual number of nodes removed
  _numProcessed = 0;    // _numProcessed reflects total elements processed
  _nodesToRemove.clear();

  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ConstRelationPtr relation = it->second;
    LOG_VART(relation->getType());
    LOG_VART(relation->getTags()[MetadataTags::HootReviewType()]);
    if (relation->getType() == MetadataTags::RelationReview() &&
        relation->getTags()[MetadataTags::HootReviewType()] == PoiPolygonMatch::MATCH_NAME)
    {
      const std::vector<RelationData::Entry>& members = relation->getMembers();
      for (size_t i = 0; i < members.size(); i++)
      {
        const RelationData::Entry member = members[i];
        LOG_VART(member.getElementId().getType());
        LOG_VART(map->getElement(member.getElementId())->getTags().size());
        if (member.getElementId().getType() == ElementType::Node &&
            map->getElement(member.getElementId())->getTags().size() == 0)
        {
          LOG_TRACE("Adding " << member.getElementId() << "...");
          _nodesToRemove.insert(member.getElementId().getId());
        }
      }
    }

    _numProcessed++;
    if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Marked " << StringUtils::formatLargeNumber(_nodesToRemove.size()) <<
        " nodes for removal after processing " << StringUtils::formatLargeNumber(_numProcessed) <<
        " total elements.");
    }
  }
  LOG_VART(_nodesToRemove.size());

  if (_nodesToRemove.size() == 0)
  {
    return;
  }

  const NodeMap nodes = map->getNodes();
  _numProcessed = 0;
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const Node* n = it->second.get();
    LOG_VART(n->getElementId());
    const long nodeId = n->getId();
    if (_nodesToRemove.find(nodeId) != _nodesToRemove.end())
    {
      LOG_TRACE("Removing node: " << n->getElementId() << "...");
      RemoveNodeByEid::removeNodeNoCheck(map, nodeId);
      _numAffected++;
    }
    _numProcessed++;

    if (_numAffected % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Removed " << StringUtils::formatLargeNumber(_numAffected) <<
        " nodes / " << StringUtils::formatLargeNumber(_nodesToRemove.size()) <<
        " total nodes to remove.");
    }
    else if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(_numProcessed) <<
        " nodes / " << StringUtils::formatLargeNumber(nodes.size()) << " total nodes.");
    }
  }
}

}
