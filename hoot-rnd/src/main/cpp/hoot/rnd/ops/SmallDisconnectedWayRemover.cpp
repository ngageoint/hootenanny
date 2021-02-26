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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#include "SmallDisconnectedWayRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/DisconnectedWayCriterion.h>
#include <hoot/core/criterion/WayLengthCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/criterion/WayNodeCountCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/conflate/ConflateUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SmallDisconnectedWayRemover)

SmallDisconnectedWayRemover::SmallDisconnectedWayRemover() :
_maxWayLength(2),
_maxWayNodeCount(2)
{
}

SmallDisconnectedWayRemover::SmallDisconnectedWayRemover(
  const int maxWayLength, const int maxWayNodeCount) :
_maxWayLength(maxWayLength),
_maxWayNodeCount(maxWayNodeCount)
{
}

void SmallDisconnectedWayRemover::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts(conf);
  _maxWayLength = confOpts.getSmallDisconnectedWayRemoverMaxLength();
  _maxWayNodeCount = confOpts.getSmallDisconnectedWayRemoverMaxNodeCount();
}

void SmallDisconnectedWayRemover::apply(OsmMapPtr& map)
{
  _numAffected = 0;
  _numProcessed = 0;
  _map = map;
  LOG_VART(_maxWayLength);

  // length calc requires planar
  MapProjector::projectToPlanar(_map);

  // remove ways at or below a certain size
  std::shared_ptr<WayLengthCriterion> lengthCrit(
    new WayLengthCriterion(_maxWayLength, NumericComparisonType::LessThanOrEqualTo, _map));
  // remove ways having a node count at or below a certain amount
  std::shared_ptr<WayNodeCountCriterion> nodeCountCrit(
    new WayNodeCountCriterion(_maxWayNodeCount, NumericComparisonType::LessThanOrEqualTo));
  ChainCriterionPtr wayCrit(new ChainCriterion(lengthCrit, nodeCountCrit));
  // require that ways be disconnected from other ways to be removed
  std::shared_ptr<DisconnectedWayCriterion> disconnectedCrit(new DisconnectedWayCriterion(_map));
  ChainCriterionPtr crit(new ChainCriterion(wayCrit, disconnectedCrit));

  // Find the ways to remove.
  std::set<long> wayIdsToRemove;
  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    ConstWayPtr way = waysItr->second;
    _numProcessed++;
    if (!way)
    {
      continue;
    }
//    else if (_checkConflatable && !ConflateUtils::elementCanBeConflatedByActiveMatcher(way, _map))
//    {
//      LOG_TRACE(
//        "Skipping processing of " << way->getElementId() << " as it cannot be conflated by any " <<
//        "actively configured conflate matcher...");
//      continue;
//    }
    LOG_VART(way->getElementId());

    LOG_VART(lengthCrit->isSatisfied(way));
    LOG_VART(nodeCountCrit->isSatisfied(way));
    LOG_VART(disconnectedCrit->isSatisfied(way));
    if (crit->isSatisfied(way))
    {
      wayIdsToRemove.insert(way->getId());
    }
  }

  // Remove the ways.
  for (std::set<long>::const_iterator wayIdsItr = wayIdsToRemove.begin();
       wayIdsItr != wayIdsToRemove.end(); ++wayIdsItr)
  {
    const long wayId = *wayIdsItr;
    LOG_VART(wayId);
    RecursiveElementRemover(ElementId::way(wayId)).apply(_map);
    _numAffected++;
  }
}

QStringList SmallDisconnectedWayRemover::getCriteria() const
{
  QStringList criteria;
  criteria.append(LinearCriterion::className());
  return criteria;
}

}
