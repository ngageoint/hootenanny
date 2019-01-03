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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "WayMerger.h"

// Boost
using namespace boost;

// GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/operation/distance/DistanceOp.h>
using namespace geos::geom;
using namespace geos::operation::distance;

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/subline-matching/MaximalNearestSubline.h>
#include <hoot/core/algorithms/ProbabilityOfMatch.h>
#include <hoot/core/conflate/WorkingMap.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/manipulators/WayMergeManipulation.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/criterion/HighwayCriterion.h>
using namespace hoot::elements;

// Qt
#include <qdebug.h>

// Standard
#include <iostream>
using namespace std;

// Tgs
#include <tgs/StreamUtils.h>
using namespace Tgs;


namespace hoot
{

HOOT_FACTORY_REGISTER(Manipulator, WayMerger)

WayMerger::WayMerger()
{
  _minSplitSize = ConfigOptions().getWayMergerMinSplitSize();
}

Manipulation* WayMerger::_createManipulation(long leftId, long rightId,
  ConstOsmMapPtr map, Meters minSplitSize)
{
  return new WayMergeManipulation(leftId, rightId, map, minSplitSize);
}

const vector< boost::shared_ptr<Manipulation> >& WayMerger::findAllManipulations(
        ConstOsmMapPtr map)
{
  LOG_INFO("Finding all way merge manipulations...");

  // Find all Unknown1 ways
  StatusCriterion statusCrit(Status::Unknown1);
  vector<long> unknown1 = FindWaysVisitor::findWays(map, &statusCrit);

  // return the result
  return findWayManipulations(map, unknown1);
}

const vector< boost::shared_ptr<Manipulation> >& WayMerger::findWayManipulations(
        ConstOsmMapPtr map, const vector<long>& wids)
{
  _result.clear();
  _map = map;

  size_t i;
  for (i = 0; i < wids.size(); i++)
  {
    if (i >= 100 && i % 100 == 0)
    {
      PROGRESS_INFO("  finding manipulations: " << i << " / " << wids.size() << "        ");
    }
    if (_map->containsWay(wids[i]))
    {
      // evaluate the way to see if it has a matching candidate and add it to _result.
      _findMatches(wids[i]);
    }
  }

  if (i >= 100)
  {
    LOG_INFO("  finding manipulations: " << wids.size() << " / " << wids.size() << "        ");
  }

  return _result;
}

void WayMerger::_findMatches(long baseWayId)
{
  ConstWayPtr baseWay = _map->getWay(baseWayId);

  HighwayCriterion highwayCrit;

  // if it isn't a highway we can't merge it.
  if (highwayCrit.isSatisfied(baseWay) == false)
  {
    return;
  }

  // find all the parallel ways that could be candidates.
  vector<long> otherWays = _findOtherWays(baseWay);

  // go through all the other ways
  for (size_t oi = 0; oi < otherWays.size(); oi++)
  {
    ConstWayPtr otherWay = _map->getWay(otherWays[oi]);

    if (otherWay->isUnknown() && baseWay->isUnknown() && highwayCrit.isSatisfied(otherWay))
    {
      // create a new manipulation and add it onto the result.
      boost::shared_ptr<Manipulation> m(_createManipulation(baseWay->getId(), otherWay->getId(),
                                                          _map, _minSplitSize));
      _result.push_back(m);
    }
  }
}

vector<long> WayMerger::_findOtherWays(ConstWayPtr baseWayConst)
{
  Status otherStatus = Status::Unknown2;
  if (baseWayConst->getStatus() == Status::Unknown2)
  {
    otherStatus = Status::Unknown1;
  }

  StatusCriterion statusCrit(otherStatus);
  vector<long> filtered = FindWaysVisitor::findWays(_map,
                                                    &statusCrit,
                                                    baseWayConst,
                                                    0.0,
                                                    true);

  vector<long> allWays = filtered;
  allWays.push_back(baseWayConst->getId());

  OsmMapPtr map(new OsmMap());
  CopyMapSubsetOp(_map, allWays).apply(map);


  WayPtr baseWay = map->getWay(baseWayConst->getId());

  vector<long> result;
  result.reserve(filtered.size());
  for (size_t i = 0; i < filtered.size(); i++)
  {
    WayPtr otherWay = map->getWay(filtered[i]);

    Meters circularError = otherWay->getCircularError() + baseWay->getCircularError();
    // use the maximal nearest subline code to find the best subline
    WayPtr mns1 = MaximalNearestSubline::getMaximalNearestSubline(map, baseWay,
      otherWay, _minSplitSize, circularError);
    if (mns1 != 0)
    {
      WayPtr mns2 = MaximalNearestSubline::getMaximalNearestSubline(map,
        otherWay, mns1, _minSplitSize, circularError);

      if (mns2 != 0)
      {
        double mns1Length = ElementConverter(map).convertToLineString(mns1)->getLength();
        double mns2Length = ElementConverter(map).convertToLineString(mns2)->getLength();

        // if the maximal nearest subline is a good fit (e.g. similar attributes,
        // about the right angle, etc.)
        if (mns1Length > _minSplitSize && mns2Length > _minSplitSize &&
            ProbabilityOfMatch::getInstance().expertProbability(map, mns1, mns2) > 0.01)
        {
          // push the way onto the result.
          result.push_back(filtered[i]);
        }
      }
    }
  }

  return result;
}

}
