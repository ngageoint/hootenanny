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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "FindStreetIntersectionsByName.h"

// hoot
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/StatusUpdateVisitor.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, FindStreetIntersectionsByName)

void FindStreetIntersectionsByName::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _nameCrit = std::make_shared<NameCriterion>();
  _nameCrit->setConfiguration(conf);
  const QStringList streetNames = opts.getNameCriterionNames();
  if (streetNames.size() != 2)
  {
    throw IllegalArgumentException(
      QString("The name.criterion.names configuration option for FindStreetIntersectionsByName must consist of exactly two names."));
  }
}

void FindStreetIntersectionsByName::apply(OsmMapPtr& map)
{
  if (!_nameCrit)
    throw HootException("Must call setConfiguration on FindStreetIntersectionsByName before running.");

  LOG_VARD(map->size());
  // Use the total number of roads in the map as the total feature being processed.
  _numProcessed = (int)FilteredVisitor::getStat(std::make_shared<HighwayCriterion>(map), std::make_shared<ElementCountVisitor>(), map);
  LOG_VARD(_numProcessed);
  _numAffected = 0;

  // Reduce the map down to just the streets that match the two input street names. Load the streets
  // found for each input into separate maps and give them statuses, so we don't get matches between
  // streets both with similar names as the first or second input (e.g. input="Fremont;Olive" and
  // we get back a partial match for "Strada Olivero" and "West Strada Olivero").
  const QStringList streetNames = _nameCrit->getNames();
  OsmMapPtr matchingRoads1Map = _filterRoadsByStreetName(streetNames[0], Status::Unknown1, map);
  LOG_VARD(matchingRoads1Map->size());
  OsmMapPtr matchingRoads2Map = _filterRoadsByStreetName(streetNames[1], Status::Unknown2, map);
  LOG_VARD(matchingRoads2Map->size());

  // Now, combine the maps back together to search over and use the assigned statuses to prevent
  // incorrect matches.
  OsmMapPtr combinedMatchingRoadsMap = std::make_shared<OsmMap>(matchingRoads1Map);
  combinedMatchingRoadsMap->append(matchingRoads2Map);
  matchingRoads1Map.reset();
  matchingRoads2Map.reset();

  OsmMapPtr intersectingWayNodesMap = std::make_shared<OsmMap>();
  // make a copy so we can iterate through even if there are changes
  const WayMap ways = combinedMatchingRoadsMap->getWays();
  LOG_VARD(ways.size());
  // go through all the name matched streets
  for (auto waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    WayPtr way = waysItr->second;
    if (!way)
      continue;
    LOG_VART(way);

    // find all streets that intersect the current one
    const std::vector<WayPtr> intersectingWays = WayUtils::getIntersectingWays(way->getId(), combinedMatchingRoadsMap);
    LOG_VART(intersectingWays.size());
    for (const auto& otherWay : intersectingWays)
    {
      // if the streets being compared aren't identical or have the same name (road split into
      // multiple parts)
      const Qt::CaseSensitivity caseSensitivity = _nameCrit->getCaseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive;
      if (otherWay && otherWay->getElementId() != way->getElementId() &&
          otherWay->getStatus() != way->getStatus() &&
          otherWay->getTags().getName().compare(way->getTags().getName(), caseSensitivity) != 0)
      {
        LOG_VART(otherWay);
        // find all way nodes they share
        const QSet<long> intersectingWayNodeIds = way->sharedNodeIds(*otherWay);
        LOG_VART(intersectingWayNodeIds.size());
        for (auto node_id : qAsConst(intersectingWayNodeIds))
        {
          NodePtr wayNode = combinedMatchingRoadsMap->getNode(node_id);
          if (wayNode)
          {
            LOG_VART(wayNode);
            // Add the intersection node to the output map.
            NodePtr intersectionNode = std::make_shared<Node>(*wayNode);
            intersectionNode->getTags().set(MetadataTags::HootIntersectionStreet1(), way->getTags().getName());
            intersectionNode->getTags().set(MetadataTags::HootIntersectionStreet2(), otherWay->getTags().getName());
            intersectingWayNodesMap->addNode(intersectionNode);
            _numAffected++;
          }
        }
      }
    }
  }
  map = intersectingWayNodesMap;
}

OsmMapPtr FindStreetIntersectionsByName::_filterRoadsByStreetName(const QString& name, const Status& status, const ConstOsmMapPtr& map) const
{
  const QStringList streetNames(name);
  _nameCrit->setNames(streetNames);
  ElementCriterionPtr crit = std::make_shared<ChainCriterion>(std::make_shared<HighwayCriterion>(map), _nameCrit);
  CopyMapSubsetOp mapCopier(map, crit);
  OsmMapPtr matchingRoadsMap = std::make_shared<OsmMap>();
  mapCopier.apply(matchingRoadsMap);
  StatusUpdateVisitor statusUpdater(status);
  matchingRoadsMap->visitWaysRw(statusUpdater);
  return matchingRoadsMap;
}

QString FindStreetIntersectionsByName::getCompletedStatusMessage() const
{
  if (!_nameCrit->getNames().isEmpty())
  {
    return QString("Located %1 street intersections for inputs: %2 and %3 out of %4 streets.")
            .arg(StringUtils::formatLargeNumber(_numAffected), _nameCrit->getNames()[0],
                 _nameCrit->getNames()[1], StringUtils::formatLargeNumber(_numProcessed));
  }
  else
    return "Located no street intersections.";
}

}
