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

#include "DividedHighwayMerger.h"

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
#include <hoot/core/OsmMap.h>
#include <hoot/core/WorkingMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/LineStringAverager.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/filters/NotCriterion.h>
#include <hoot/core/filters/ParallelWayCriterion.h>
#include <hoot/core/filters/StatusFilter.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/filters/WayDirectionCriterion.h>
#include <hoot/core/filters/WayBufferCriterion.h>
#include <hoot/core/filters/OneWayCriterion.h>
#include <hoot/core/filters/UnknownCriterion.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Log.h>
using namespace hoot::elements;

// Qt
#include <qdebug.h>

// Standard
#include <iostream>
using namespace std;

// Tgs
#include <tgs/StreamUtils.h>
using namespace Tgs;

#include "DividedHighwayManipulation.h"

#define SQR(x) ((x) * (x))

namespace hoot
{

DividedHighwayMerger::DividedHighwayMerger(Meters minSeparation, Meters maxSeparation,
                                           Meters vectorError, double matchPercent)
{
  _minSeparation = minSeparation;
  _maxSeparation = maxSeparation;
  _vectorError = vectorError;
  _matchPercent = matchPercent;

  shared_ptr<OneWayCriterion> pOneWayCrit(new OneWayCriterion());
  shared_ptr<UnknownCriterion> pUnknownCrit(new UnknownCriterion());
  _oneWayUnknownCriterion.addCriterion(pOneWayCrit);
  _oneWayUnknownCriterion.addCriterion(pUnknownCrit);

}

const vector< shared_ptr<Manipulation> >& DividedHighwayMerger::findAllManipulations(
        shared_ptr<const OsmMap> map)
{
  // go through all the oneway, unknown ways
  vector<long> oneWays = FindWaysVisitor::findWays(map, &_oneWayUnknownCriterion);

  // return the result
  return findWayManipulations(map, oneWays);
}

const vector< shared_ptr<Manipulation> >& DividedHighwayMerger::findWayManipulations(
        shared_ptr<const OsmMap> map, const vector<long>& wids)
{
  _result.clear();
  _map = map;

  for (size_t i = 0; i < wids.size(); i++)
  {
    if (Log::getInstance().isInfoEnabled())
    {
      printf("DividedHighwayMerger::findAllManipulations() %d   \r", (int)_result.size());
      fflush(stdout);
    }
    // evaluate the way to see if it is a divided highway candidate and add it to _result.
    _findMatches(wids[i]);
  }
  if (Log::getInstance().isInfoEnabled())
  {
    printf("\n");
  }

  // return the result
  return _result;
}

vector<long> DividedHighwayMerger::_findCenterWays(shared_ptr<const Way> w1,
                                                   shared_ptr<const Way> w2)
{
  shared_ptr<OneWayCriterion> notOneWayCrit(new OneWayCriterion(false));

  Status s;
  if (w1->getStatus() == Status::Unknown1)
  {
    s = Status::Unknown2;
  }
  else
  {
    s = Status::Unknown1;
  }

  shared_ptr<StatusCriterion> statusCrit(new StatusCriterion(s));
  shared_ptr<ParallelWayCriterion> parallelCrit(new ParallelWayCriterion(_map, w1));

  ElementConverter ec(_map);
  shared_ptr<LineString> ls2 = ec.convertToLineString(w2);
  if (DirectionFinder::isSimilarDirection(_map, w1, w2) == false)
  {
    ls2.reset(dynamic_cast<LineString*>(ls2->reverse()));
  }

  // calculate the center line of two ways.
  shared_ptr<LineString> center = LineStringAverager::average(
    ec.convertToLineString(w1), ls2);
  shared_ptr<WayBufferCriterion> distanceCrit(new WayBufferCriterion(_map, center, 0.0,
    (w1->getCircularError() + w2->getCircularError()) / 2.0, _matchPercent));

  ChainCriterion crit;
  crit.addCriterion(notOneWayCrit);
  crit.addCriterion(statusCrit);
  crit.addCriterion(parallelCrit);
  crit.addCriterion(distanceCrit);

  return FindWaysVisitor::findWays(_map, &crit);
}

void DividedHighwayMerger::_findMatches(long baseWayId)
{
  shared_ptr<const Way> baseWay = _map->getWay(baseWayId);
  // find all the parallel and opposite ways that could be candidates.
  vector<long> otherWays = _findOtherWays(baseWay);

  //qDebug() << "otherWays.size()" << otherWays.size();

  // go through all the other ways
  for (size_t oi = 0; oi < otherWays.size(); oi++)
  {
    // this ensures that we'll only test a pair of ways once.
    if (baseWay->getId() < otherWays[oi])
    {
      shared_ptr<const Way> otherWay = _map->getWay(otherWays[oi]);

      // find all potential center lines
      vector<long> centerWays = _findCenterWays(baseWay, otherWay);

      // for each center line
      for (size_t ci = 0; ci < centerWays.size(); ci++)
      {
        // create a new manipulation and add it onto the result.
        shared_ptr<Manipulation> m(new DividedHighwayManipulation(baseWay->getId(),
          otherWay->getId(), centerWays[ci], _map, _vectorError));
        _result.push_back(m);
        //printf("DividedHighwayMerger::_findMatches() %d   \n", (int)_result.size());
        //fflush(stdout);
      }
    }
  }
}

vector<long> DividedHighwayMerger::_findOtherWays(boost::shared_ptr<const hoot::Way> baseWay)
{
  shared_ptr<OneWayCriterion> oneWayCrit(new OneWayCriterion());
  shared_ptr<StatusCriterion> statusCrit(new StatusCriterion(baseWay->getStatus()));
  shared_ptr<WayBufferCriterion> distanceCrit(new WayBufferCriterion(_map,
                                                                     baseWay,
                                                                     _maxSeparation,
                                                                     _matchPercent));
  shared_ptr<ParallelWayCriterion> parallelCrit(new ParallelWayCriterion(_map, baseWay));
  shared_ptr<WayDirectionCriterion> directionCrit(new WayDirectionCriterion(_map, baseWay, true));

  ChainCriterion chain;
  chain.addCriterion(oneWayCrit);
  chain.addCriterion(statusCrit);
  chain.addCriterion(distanceCrit);
  chain.addCriterion(parallelCrit);
  chain.addCriterion(directionCrit);

  return FindWaysVisitor::findWays(_map, &chain);
}

}



