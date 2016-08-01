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

#include "DanglerRemover.h"

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
#include <hoot/core/algorithms/MaximalNearestSubline.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/filters/UnknownCriterion.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
using namespace hoot::elements;

// Qt
#include <qdebug.h>

// Standard
#include <iostream>
using namespace std;

// Tgs
#include <tgs/StreamUtils.h>
using namespace Tgs;

#include "DanglerRemoverManipulation.h"

namespace hoot {

#define SQR(x) ((x) * (x))

DanglerRemover::DanglerRemover(Meters errorPlus)
{
  _errorPlus = errorPlus;
}

const vector< shared_ptr<Manipulation> >& DanglerRemover::findAllManipulations(
        shared_ptr<const OsmMap> map)
{
  LOG_INFO("Finding all dangle remover manipulations...");

  // Find all Unknown ways
  UnknownCriterion unknownCrit;
  vector<long> unknown = FindWaysVisitor::findWays(map, &unknownCrit);

  // return the result
  return findWayManipulations(map, unknown);
}

const vector< shared_ptr<Manipulation> >& DanglerRemover::findWayManipulations(
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

void DanglerRemover::_findMatches(long baseWayId)
{
  shared_ptr<DanglerRemoverManipulation> result(new DanglerRemoverManipulation(baseWayId, _map, _errorPlus));

  if (result->getScoreEstimate() > 0)
  {
    _result.push_back(result);
  }
}

}
