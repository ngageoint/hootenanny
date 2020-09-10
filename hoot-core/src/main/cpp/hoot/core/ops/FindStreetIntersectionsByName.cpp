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
 * @copyright Copyright (C)  2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FindStreetIntersectionsByName.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/visitors/StatusUpdateVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, FindStreetIntersectionsByName)

void FindStreetIntersectionsByName::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  const QStringList streetNames = opts.getNameCriterionNames();
  if (streetNames.size() != 2)
  {
    throw IllegalArgumentException(
      QString("The name.criterion.names configuration option for FindStreetIntersectionsByName") +
      QString("must consist of exactly two names."));
  }
  _nameCrit.reset(new NameCriterion());
  _nameCrit->setConfiguration(conf);
}

void FindStreetIntersectionsByName::apply(OsmMapPtr& map)
{
  if (!_nameCrit)
  {
    throw HootException(
      "Must call setConfiguration on FindStreetIntersectionsByName before running.");
  }

  LOG_VARD(map->size());
  // Use the total number of roads in the map as the total feature being processed.
  _numProcessed =
    (int)FilteredVisitor::getStat(
      std::shared_ptr<HighwayCriterion>(new HighwayCriterion(map)),
      std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()),
      map);
  LOG_VARD(_numProcessed);
  _numAffected = 0;

  // Reduce the map down to just the streets that match the two input street names.
  ElementCriterionPtr crit(
    new ChainCriterion(std::shared_ptr<HighwayCriterion>(new HighwayCriterion(map)), _nameCrit));
  CopyMapSubsetOp mapCopier(map, crit);
  OsmMapPtr matchingRoadsMap(new OsmMap());
  mapCopier.apply(matchingRoadsMap);
  LOG_VARD(matchingRoadsMap->size());

  OsmMapPtr intersectingWayNodesMap(new OsmMap());
  // make a copy so we can iterate through even if there are changes
  const WayMap ways = matchingRoadsMap->getWays();
  LOG_VARD(ways.size());
  // go through all the name matched streets
  for (WayMap::const_iterator waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    WayPtr way = waysItr->second;
    if (!way)
    {
      continue;
    }
    LOG_VART(way);

    // find all streets that intersect the current one
    const std::vector<WayPtr> intersectingWays =
      WayUtils::getIntersectingWays(way->getId(), matchingRoadsMap);
    LOG_VART(intersectingWays.size());
    for (std::vector<WayPtr>::const_iterator otherWaysItr = intersectingWays.begin();
         otherWaysItr != intersectingWays.end(); ++otherWaysItr)
    {
      WayPtr otherWay = *otherWaysItr;
      // if the streets being compared aren't identical
      if (otherWay && otherWay->getElementId() != way->getElementId())
      {
        LOG_VART(otherWay);
        // find all way nodes they share
        const QSet<long> intersectingWayNodeIds = way->sharedNodeIds(*otherWay);
        LOG_VART(intersectingWayNodeIds.size());
        for (QSet<long>::const_iterator wayNodeIdsItr = intersectingWayNodeIds.begin();
             wayNodeIdsItr != intersectingWayNodeIds.end(); ++wayNodeIdsItr)
        {
          NodePtr wayNode = matchingRoadsMap->getNode(*wayNodeIdsItr);
          if (wayNode)
          {
            LOG_VART(wayNode);
            // add the node to the output map
            NodePtr intersectionNode(new Node(*wayNode));
            intersectionNode->getTags().set(
              MetadataTags::HootIntersectionStreet1(), way->getTags().getName());
            intersectionNode->getTags().set(
              MetadataTags::HootIntersectionStreet2(), otherWay->getTags().getName());
            intersectingWayNodesMap->addNode(intersectionNode);
            _numAffected++;
          }
        }
      }
    }
  }
  map = intersectingWayNodesMap;
}

}
