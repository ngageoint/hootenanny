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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "PoiPolygonInvalidReviewNodeRemover.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, PoiPolygonInvalidReviewNodeRemover)

PoiPolygonInvalidReviewNodeRemover::PoiPolygonInvalidReviewNodeRemover() :
_numRelationsRemoved(0),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void PoiPolygonInvalidReviewNodeRemover::apply(const std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;     // _numAffected reflects the actual number of nodes removed
  _numProcessed = 0;    // _numProcessed reflects total elements processed
  _numRelationsRemoved = 0;
  _nodesToRemove.clear();
  _reviewRelationsToRemove.clear();

  // make a copy here, since we may be removing some relations later
  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ConstRelationPtr relation = it->second;
    LOG_VART(relation->getType());
    LOG_VART(relation->getTags()[MetadataTags::HootReviewType()]);
    // This problem only affects POIs being merged that are involved in either POI/Polygon or
    // POI/POI reviews
    if (relation->getType() == MetadataTags::RelationReview() &&
        (relation->getTags()[MetadataTags::HootReviewType()] == PoiPolygonMatch::MATCH_NAME ||
         // TODO: need a way to not hardcode this...get it from ScriptMatchCreator somehow?
         relation->getTags()[MetadataTags::HootReviewType()] == "POI"))
    {
      const std::vector<RelationData::Entry>& members = relation->getMembers();
      for (size_t i = 0; i < members.size(); i++)
      {
        const RelationData::Entry member = members[i];
        LOG_VART(member.getElementId());
        if (member.getElementId().getType() == ElementType::Node)
        {
          ConstElementPtr element = map->getElement(member.getElementId());
          if (element)
          {
            LOG_VART(map->getElement(member.getElementId())->getTags().size());
            if (map->getElement(member.getElementId())->getTags().empty())
            {
              LOG_TRACE("Marking " << member.getElementId() << " for removal...");
              _nodesToRemove.insert(member.getElementId().getId());
              if (members.size() <= 2)
              {
                LOG_TRACE("Marking " << relation->getElementId() << " for removal...");
                _reviewRelationsToRemove.insert(relation->getElementId().getId());
              }
            }
          }
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

  if (_nodesToRemove.empty())
  {
    return;
  }

  _numProcessed = 0;
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const Relation* relation = it->second.get();
    LOG_VART(relation->getElementId());
    const long relationId = relation->getId();
    if (_reviewRelationsToRemove.find(relationId) != _reviewRelationsToRemove.end())
    {
      LOG_TRACE("Removing relation: " << relation->getElementId() << "...");
      RemoveRelationByEid::removeRelation(map, relationId);
      _numRelationsRemoved++;
    }
    _numProcessed++;

    if (_numRelationsRemoved % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Removed " << StringUtils::formatLargeNumber(_numRelationsRemoved) <<
        " relation / " << StringUtils::formatLargeNumber(_reviewRelationsToRemove.size()) <<
        " total relations to remove.");
    }
    else if (_numProcessed % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(_numProcessed) <<
        " relations / " << StringUtils::formatLargeNumber(relations.size()) << " total relations.");
    }
  }

  // Make a copy here, since we may be removing some of these nodes.
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

QStringList PoiPolygonInvalidReviewNodeRemover::getCriteria() const
{
  QStringList criteria;
  criteria.append(PoiPolygonPoiCriterion::className());
  criteria.append(PoiPolygonPolyCriterion::className());
  return criteria;
}

}
