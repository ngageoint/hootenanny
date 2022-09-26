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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "AttributeComparator.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/StringUtils.h>

// TGS
#include <tgs/Statistics/Random.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

AttributeComparator::AttributeComparator(const std::shared_ptr<OsmMap>& map1,
                                         const std::shared_ptr<OsmMap>& map2)
  : BaseComparator(map1, map2),
    _iterations(10),
    _median(0.0),
    _mean(0.0),
    _ci(-1.0),
    _s(-1.0)
{
}

double AttributeComparator::compareMaps()
{
  LOG_STATUS("Comparing map attributes...");

  _updateBounds();
  double scoreSum = 0.0;

  double buffer = 10.0;

  double oldIsACost = OsmSchema::getInstance().getIsACost();
  OsmSchema::getInstance().setIsACost(0.5);

  vector<double> scores;
  // sampled standard deviation
  _s = -1;
  // 1.645 for 90% confidence, 1.96 for 95% confidence, and 2.58 for 99% confidence.
  double zalpha = 1.645;
  _ci = -1;

  std::shared_ptr<OsmMap> referenceMap, otherMap;

  // do this a bunch of times
  const int totalIterations = _iterations * 4;
  for (int i = 0; i < totalIterations && (int)scores.size() < _iterations; i++)
  {
    // generate a random source point
    _r.x = Random::instance()->generateUniform() * (_projectedBounds.MaxX - _projectedBounds.MinX) +
           _projectedBounds.MinX;
    _r.y = Random::instance()->generateUniform() * (_projectedBounds.MaxY - _projectedBounds.MinY) +
           _projectedBounds.MinY;

    // pick one map as the reference map
    if (Random::instance()->coinToss())
    {
      referenceMap = _mapP1;
      otherMap = _mapP2;
    }
    else
    {
      referenceMap = _mapP2;
      otherMap = _mapP1;
    }

    // find the nearest way on the reference map
    vector<long> wids1 = referenceMap->getIndex().findWayNeighbors(_r, buffer);
    vector<long> wids2 = otherMap->getIndex().findWayNeighbors(_r, buffer);

    double bestScore = -1.0;
    for (auto ref_way_id : wids1)
    {
      WayPtr w1 = referenceMap->getWay(ref_way_id);
      for (auto sec_way_id : wids2)
      {
        WayPtr w2 = otherMap->getWay(sec_way_id);
        double score = TagComparator::getInstance().compareTags(w1->getTags(), w2->getTags());
        if (score > bestScore)
          bestScore = score;
      }
    }

    if (bestScore >= 0.0)
    {
      scoreSum += bestScore;
      scores.push_back(bestScore);
      sort(scores.begin(), scores.end());
      _median = scores[scores.size() / 2];
      _mean = scoreSum / (double)scores.size();
    }

    if (scores.size() > 1)
    {
      double v = 0;
      for (auto score : scores)
        v += (score - _mean) * (score - _mean);

      _s = sqrt(v / (scores.size() - 1));
      _ci = zalpha * _s / sqrt(scores.size());
    }

    if (i % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_STATUS(
        "Completed " << StringUtils::formatLargeNumber(i) << " of " <<
        StringUtils::formatLargeNumber(totalIterations) << " attribute comparison iterations.");
    }
  }
  LOG_STATUS(
    "Completed " << StringUtils::formatLargeNumber(totalIterations) <<
    " attribute comparison iterations.");

  OsmSchema::getInstance().setIsACost(oldIsACost);

  return _mean;
}

}
