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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#include "UnlikelyRoadRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/criterion/WayLengthCriterion.h>
#include <hoot/core/criterion/WayHeadingVarianceCriterion.h>
#include <hoot/core/criterion/RoundaboutCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, UnlikelyRoadRemover)

UnlikelyRoadRemover::UnlikelyRoadRemover() :
_maxWayLength(25.0),
_numHistogramBins(16),
_headingDelta(5.0),
_sampleDistance(1.0),
_maxHeadingVariance(60.0)
{
}

void UnlikelyRoadRemover::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts(conf);
  _maxWayLength = confOpts.getUnlikelyRoadRemoverMaxLength();
  _numHistogramBins = confOpts.getUnlikelyRoadRemoverNumBins();
  _headingDelta = confOpts.getUnlikelyRoadRemoverHeadingDelta();
  _sampleDistance = confOpts.getUnlikelyRoadRemoverSampleDistance();
  _maxHeadingVariance = confOpts.getUnlikelyRoadRemoverMaxHeadingVariance();
}

ElementCriterionPtr UnlikelyRoadRemover::_getRemovalCrit(const ConstOsmMapPtr& map) const
{
  // remove ways at or below a certain size
  std::shared_ptr<WayLengthCriterion> lengthCrit =
    std::make_shared<WayLengthCriterion>(
      _maxWayLength, NumericComparisonType::LessThanOrEqualTo, map);
  // remove ways whose heading (in degrees) varies at or above a certain amount 
  std::shared_ptr<WayHeadingVarianceCriterion> headingCrit =
    std::make_shared<WayHeadingVarianceCriterion>(
      _maxHeadingVariance, NumericComparisonType::GreaterThanOrEqualTo, map);
  headingCrit->setNumHistogramBins(_numHistogramBins);
  headingCrit->setSampleDistance(_sampleDistance);
  headingCrit->setHeadingDelta(_headingDelta); 
  ChainCriterionPtr chainedWayCrit = std::make_shared<ChainCriterion>(lengthCrit, headingCrit);

  // make sure only roads are removed
  std::shared_ptr<HighwayCriterion> roadCrit =
    std::make_shared<HighwayCriterion>(HighwayCriterion(map));
  // Don't consider roundabouts, since their headings will obviously vary quite a bit as you go
  // around them.
  std::shared_ptr<NotCriterion> roundaboutCrit =
    std::make_shared<NotCriterion>(std::make_shared<RoundaboutCriterion>());
  ChainCriterionPtr chainedRoadCrit = std::make_shared<ChainCriterion>(roadCrit, roundaboutCrit);

  return std::make_shared<ChainCriterion>(chainedWayCrit, chainedRoadCrit);
}

void UnlikelyRoadRemover::apply(OsmMapPtr& map)
{
  _numAffected = 0;
  _numProcessed = 0;

  ElementCriterionPtr removalCrit = _getRemovalCrit(map);

  // Find the ways to remove.
  std::set<long> wayIdsToRemove;
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    ConstWayPtr way = waysItr->second;
    _numProcessed++;
    if (!way || !removalCrit->isSatisfied(way))
    {
      continue;
    }
    LOG_VART(ElementId::way(way->getId()));

    wayIdsToRemove.insert(way->getId());
  }

  // Remove the ways.
  for (std::set<long>::const_iterator wayIdsItr = wayIdsToRemove.begin();
       wayIdsItr != wayIdsToRemove.end(); ++wayIdsItr)
  {
    RecursiveElementRemover(ElementId::way(*wayIdsItr)).apply(map);
    _numAffected++;
  }
  LOG_VART(_numAffected);
}

QStringList UnlikelyRoadRemover::getCriteria() const
{
  QStringList criteria;
  criteria.append(HighwayCriterion::className());
  return criteria;
}

}
