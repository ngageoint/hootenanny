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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
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
#include <hoot/core/util/Log.h>

// TGS
#include <tgs/Statistics/Random.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

AttributeComparator::AttributeComparator(const std::shared_ptr<OsmMap>& map1,
                                         const std::shared_ptr<OsmMap>& map2) :
  BaseComparator(map1, map2),
  _iterations(10),
  _median(0.0),
  _mean(0.0),
  _ci(-1.0),
  _s(-1.0)
{
}

double AttributeComparator::compareMaps()
{
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
  for (int i = 0; i < _iterations * 4 && (int)scores.size() < _iterations; i++)
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

    Tags t1, t2;
    double bestScore = -1.0;
    for (size_t j = 0; j < wids1.size(); j++)
    {
      WayPtr w1 = referenceMap->getWay(wids1[j]);

      for (size_t k = 0; k < wids2.size(); k++)
      {
        WayPtr w2 = otherMap->getWay(wids2[k]);
        double score = TagComparator::getInstance().compareTags(w1->getTags(), w2->getTags());
        if (score > bestScore)
        {
          bestScore = score;
          t1 = w1->getTags();
          t2 = w2->getTags();
        }
      }
    }

    if (bestScore >= 0.0)
    {
//        LOG_INFO("====");
//        LOG_INFO("score: " << bestScore);
//        LOG_INFO("t1: \n" << t1);
//        LOG_INFO("t2: \n" << t2);

      scoreSum += bestScore;
      scores.push_back(bestScore);
      sort(scores.begin(), scores.end());
      _median = scores[scores.size() / 2];
      _mean = scoreSum / (double)scores.size();
    }

    if (scores.size() > 1)
    {
      double v = 0;
      for (size_t i = 0; i < scores.size(); i++)
      {
        v += (scores[i] - _mean) * (scores[i] - _mean);
      }
      _s = sqrt(v / (scores.size() - 1));

      _ci = zalpha * _s / sqrt(scores.size());
    }

    PROGRESS_INFO(i << " / " << _iterations << " mean: " << _mean << "   ");
  }

  LOG_INFO(_iterations << " / " << _iterations << " mean: " << _mean << "   ");

  OsmSchema::getInstance().setIsACost(oldIsACost);

  return _mean;
}

}
