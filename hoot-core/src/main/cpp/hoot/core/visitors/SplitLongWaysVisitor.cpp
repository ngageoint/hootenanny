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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "SplitLongWaysVisitor.h"

#include <cstddef>

#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

int SplitLongWaysVisitor::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ElementVisitor, SplitLongWaysVisitor)

SplitLongWaysVisitor::SplitLongWaysVisitor()
  : _maxNodesPerWay(0)
{
  // Find out if user set our configuration value at cmdline or if we should use default
  ConfigOptions configOptions;
  setMaxNumberOfNodes(configOptions.getWayMaxNodesPerWay());
}

void SplitLongWaysVisitor::setMaxNumberOfNodes(unsigned int maxNodesPerWay)
{
  _maxNodesPerWay = maxNodesPerWay;
  if (_maxNodesPerWay < 2)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      ConfigOptions configOptions;
      LOG_WARN("Invalid value for config value " << configOptions.getWayMaxNodesPerWayKey() <<
               ": " << _maxNodesPerWay << ", ignoring...");
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    _maxNodesPerWay = _defaultMaxNodesPerWay;
  }

  LOG_DEBUG("Max nodes per way set to " << getMaxNumberOfNodes() );
}


void SplitLongWaysVisitor::visit(const std::shared_ptr<Element>& element)
{
  // If not a way, ignore
  if (element->getElementType() != ElementType::Way)
    return;

  // Convert to a Way
  WayPtr way = std::dynamic_pointer_cast<Way>(element);
  WayPtr emptyWay;

  if (way == emptyWay)
  {
    LOG_TRACE("SplitLongWaysVisitor::visit: element is not a way, ignoring");
    return;
  }

  // Does way exceed max number of nodes?
  if (way->getNodeCount() <= getMaxNumberOfNodes())
  {
    LOG_TRACE("SplitLongWaysVisitor::visit: way " << way->getId() <<
      " has " << way->getNodeCount() << " nodes which is <= than"
      " max of " << getMaxNumberOfNodes() << ", ignoring");
    return;
  }
  LOG_TRACE("Found way " << way->getId() << " with " << way->getNodeCount() << " nodes");

  LOG_TRACE("SplitLongWaysVisitor::visit: way " << way->getId() <<
    " has " << way->getNodeCount() << " nodes which is greater thank"
    " max of " << getMaxNumberOfNodes() << ", splitting this way!");

  unsigned int nodesRemaining = way->getNodeCount();

  QList<ElementPtr> replacements;

  unsigned int masterNodeIndex = 0;
  while (nodesRemaining > 0)
  {
    unsigned int nodesThisTime = std::min(nodesRemaining, getMaxNumberOfNodes());

    // If there's only one node, this is a no-op as it's last node in previous way, don't add
    if (nodesThisTime == 1)
    {
      LOG_TRACE(
        "No need to add new way, there's only one node left over and it's stored in previous way");
      break;
    }

    //  Create a new way
    long way_id = way->getId();
    if (masterNodeIndex > 0)
      way_id = _map->createNextWayId();
    WayPtr newWay = std::make_shared<Way>(Status::Unknown1, way_id, way->getRawCircularError());
    newWay->setPid(way->getPid());
    newWay->setTags(way->getTags());
    replacements.append(newWay);
    LOG_TRACE("Created new way w/ ID " << newWay->getId() << " that is going to hold " << nodesThisTime <<
              " nodes");
    for (unsigned int i = 0; i < nodesThisTime; ++i)
    {
      if (i == 0)
      {
        LOG_TRACE(
          "Adding first node to way " << newWay->getId() << " with master index " <<
          (masterNodeIndex + i));
      }
      else if (i == nodesThisTime - 1)
      {
        LOG_TRACE(
          "Adding last node to way " << newWay->getId() << " with master index " <<
          (masterNodeIndex + i));
      }
      newWay->addNode(way->getNodeId(masterNodeIndex + i));
    }

    //  Add new way to the map
    _map->addWay(newWay);

    //  If we copied less than max nodes, that means we're done
    if (nodesThisTime < getMaxNumberOfNodes())
      break;
    else
    {
      //  Figure out where to start next time -- note the minus one as last node in current way
      //  has to be first node in next way to ensure continuity from the original way
      nodesRemaining -= (nodesThisTime - 1);
      masterNodeIndex += (nodesThisTime - 1);
    }
  }

  //  If the way is a 'loose' way create a relation, if not just replace it with all of the old ways as members of the relation
  const std::set<long> rids = _map->getIndex().getElementToRelationMap()->getRelationByElement(way->getElementId());
  if (rids.empty())
  {
    //  Create a new relation for all of the split ways
    long relation_id = _map->createNextRelationId();
    RelationPtr relation = std::make_shared<Relation>(Status::Unknown1, relation_id, way->getRawCircularError(), "");
    QString role = "";
    if (AreaCriterion().isSatisfied(way))
    {
      relation->setType("multipolygon");
      role = "outer";
    }
    else
      relation->setType("multilinestring");

    //  Add all of the replacement elements to the relation
    for (const auto& element : replacements)
      relation->addElement(role, element);

    //  Update any parent relations with the new relation
    _map->replace(way, relation, false);
  }
  else
  {
    //  Replace the way with all other ways in the parent relations
    _map->replace(way, replacements, false);
  }

  _numAffected++;
}

}
