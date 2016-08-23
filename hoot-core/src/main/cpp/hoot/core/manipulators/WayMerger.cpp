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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/WorkingMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/MaximalNearestSubline.h>
#include <hoot/core/algorithms/ProbabilityOfMatch.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
using namespace hoot::elements;

// Qt
#include <qdebug.h>

// Standard
#include <iostream>
using namespace std;

// Tgs
#include <tgs/StreamUtils.h>
using namespace Tgs;

#include "WayMergeManipulation.h"

namespace hoot {

#define SQR(x) ((x) * (x))

HOOT_FACTORY_REGISTER(Manipulator, WayMerger)

WayMerger::WayMerger()
{
  _minSplitSize = ConfigOptions().getWayMergerMinSplitSize();
}

Manipulation* WayMerger::_createManipulation(long leftId, long rightId,
  shared_ptr<const OsmMap> map, Meters minSplitSize)
{
  return new WayMergeManipulation(leftId, rightId, map, minSplitSize);
}

const vector< shared_ptr<Manipulation> >& WayMerger::findAllManipulations(
        shared_ptr<const OsmMap> map)
{
  LOG_INFO("Finding all way merge manipulations...");

  // Find all Unknown1 ways
  StatusCriterion statusCrit(Status::Unknown1);
  vector<long> unknown1 = FindWaysVisitor::findWays(map, &statusCrit);

  // return the result
  return findWayManipulations(map, unknown1);
}

const vector< shared_ptr<Manipulation> >& WayMerger::findWayManipulations(
        shared_ptr<const OsmMap> map, const vector<long>& wids)
{
  _result.clear();
  _map = map;

  size_t i;
  for (i = 0; i < wids.size(); i++)
  {
    if (Log::getInstance().isInfoEnabled() && i >= 100 && i % 100 == 0)
    {
      cout << "  finding manipulations: " << i << " / " << wids.size() << "        \r";
      cout.flush();
    }
    if (_map->containsWay(wids[i]))
    {
      // evaluate the way to see if it has a matching candidate and add it to _result.
      _findMatches(wids[i]);
    }
  }

  if (Log::getInstance().isInfoEnabled() && i >= 100)
  {
    cout << endl;
  }

  return _result;
}

void WayMerger::_findMatches(long baseWayId)
{
  shared_ptr<const Way> baseWay = _map->getWay(baseWayId);

  // if it isn't a highway we can't merge it.
  if (OsmSchema::getInstance().isLinearHighway(baseWay->getTags(), ElementType::Way) == false)
  {
    return;
  }

  // find all the parallel ways that could be candidates.
  vector<long> otherWays = _findOtherWays(baseWay);

  // go through all the other ways
  for (size_t oi = 0; oi < otherWays.size(); oi++)
  {
    shared_ptr<const Way> otherWay = _map->getWay(otherWays[oi]);

    if (otherWay->isUnknown() && baseWay->isUnknown() &&
        OsmSchema::getInstance().isLinearHighway(otherWay->getTags(), ElementType::Way))
    {
      // create a new manipulation and add it onto the result.
      shared_ptr<Manipulation> m(_createManipulation(baseWay->getId(), otherWay->getId(),
                                                          _map, _minSplitSize));
      _result.push_back(m);
    }
  }
}

vector<long> WayMerger::_findOtherWays(shared_ptr<const Way> baseWayConst)
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
  CopySubsetOp(_map, allWays).apply(map);


  shared_ptr<Way> baseWay = map->getWay(baseWayConst->getId());

  vector<long> result;
  result.reserve(filtered.size());
  for (size_t i = 0; i < filtered.size(); i++)
  {
    shared_ptr<Way> otherWay = map->getWay(filtered[i]);

    Meters circularError = otherWay->getCircularError() + baseWay->getCircularError();
    // use the maximal nearest subline code to find the best subline
    shared_ptr<Way> mns1 = MaximalNearestSubline::getMaximalNearestSubline(map, baseWay,
      otherWay, _minSplitSize, circularError);
    if (mns1 != 0)
    {
      shared_ptr<Way> mns2 = MaximalNearestSubline::getMaximalNearestSubline(map,
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
