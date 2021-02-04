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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "SmallDisconnectedWayRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/DisconnectedWayCriterion.h>
#include <hoot/core/criterion/WaySizeCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/ops/RecursiveElementRemover.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SmallDisconnectedWayRemover)

SmallDisconnectedWayRemover::SmallDisconnectedWayRemover() :
_maxWaySize(2)
{
}

SmallDisconnectedWayRemover::SmallDisconnectedWayRemover(const int maxWaySize) :
_maxWaySize(maxWaySize)
{
}

void SmallDisconnectedWayRemover::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts(conf);
  _maxWaySize = confOpts.getSmallDisconnectedWayRemoverMaxSize();
}

void SmallDisconnectedWayRemover::apply(OsmMapPtr& map)
{
  _numAffected = 0;
  _numProcessed = 0;
  _map = map;
  LOG_VART(_maxWaySize);

  std::shared_ptr<WaySizeCriterion> sizeCrit(
    new WaySizeCriterion(_maxWaySize, NumericComparisonType::LessThanOrEqualTo));
  std::shared_ptr<DisconnectedWayCriterion> disconnectedCrit(new DisconnectedWayCriterion(_map));
  ChainCriterionPtr crit(new ChainCriterion(sizeCrit, disconnectedCrit));

  // Find the ways to remove.
  std::set<long> wayIdsToRemove;
  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    WayPtr way = waysItr->second;
    _numProcessed++;
    if (!way)
    {
      continue;
    }
    LOG_VART(way->getElementId());

    LOG_VART(sizeCrit->isSatisfied(way));
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
