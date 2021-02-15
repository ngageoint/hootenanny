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

#include "UnlikelyRoadRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/criterion/WayLengthCriterion.h>
#include <hoot/rnd/criterion/WayHeadingVarianceCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, UnlikelyRoadRemover)

UnlikelyRoadRemover::UnlikelyRoadRemover() :
_maxWayLength(25.0),
_numHistogramBins(16),
_headingDelta(20.0),
_sampleDistance(15.0),
_maxHeadingVariance(60.0)
{
}

void UnlikelyRoadRemover::setConfiguration(const Settings& /*conf*/)
{
  //ConfigOptions confOpts(conf);
  // TODO: set these from config
  _maxWayLength = 25.0;
  _numHistogramBins = 72;
  _headingDelta = 1.0;
  _sampleDistance = 1.0;
  _maxHeadingVariance = 60.0;
}

ElementCriterionPtr UnlikelyRoadRemover::_getRemovalCrit(const ConstOsmMapPtr& map)
{
  // remove ways at or below a certain size
  std::shared_ptr<WayLengthCriterion> lengthCrit(
    new WayLengthCriterion(_maxWayLength, NumericComparisonType::LessThanOrEqualTo, map));
  std::shared_ptr<WayHeadingVarianceCriterion> headingCrit(
    new WayHeadingVarianceCriterion(
      _maxHeadingVariance, NumericComparisonType::LessThanOrEqualTo, map));
  headingCrit->setNumHistogramBins(_numHistogramBins);
  headingCrit->setSampleDistance(_sampleDistance);
  headingCrit->setHeadingDelta(_headingDelta);
  ChainCriterionPtr wayCrit(new ChainCriterion(lengthCrit, headingCrit));
  // make sure only roads are removed
  std::shared_ptr<HighwayCriterion> roadCrit(new HighwayCriterion(map));
  ChainCriterionPtr crit(new ChainCriterion(wayCrit, roadCrit));
  return crit;
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
    LOG_VARD(way->getElementId());

    wayIdsToRemove.insert(way->getId());
  }

  // Remove the ways.
  for (std::set<long>::const_iterator wayIdsItr = wayIdsToRemove.begin();
       wayIdsItr != wayIdsToRemove.end(); ++wayIdsItr)
  {
    const long wayId = *wayIdsItr;
    LOG_VART(wayId);
    RecursiveElementRemover(ElementId::way(wayId)).apply(map);
    _numAffected++;
  }
}

QStringList UnlikelyRoadRemover::getCriteria() const
{
  QStringList criteria;
  criteria.append(HighwayCriterion::className());
  return criteria;
}

}
