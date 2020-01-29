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

#include "ProbabilityOfMatch.h"

// GEOS
#include <geos/geom/LineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/WayDiscretizer.h>
#include <hoot/core/criterion/ParallelWayCriterion.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/criterion/OneWayCriterion.h>
// Standard
#include <vector>
using namespace std;

// Tgs
#include <tgs/StreamUtils.h>
#include <tgs/Statistics/Normal.h>
using namespace Tgs;

namespace hoot
{

std::shared_ptr<ProbabilityOfMatch> ProbabilityOfMatch::_theInstance;

ProbabilityOfMatch::ProbabilityOfMatch()
{
  _parallelExp = ConfigOptions().getMatchParallelExponent();
}

double ProbabilityOfMatch::attributeScore(const ConstOsmMapPtr& map,
  const ConstWayPtr& w1, const ConstWayPtr& w2)
{
  double score = 1.0;

  score = TagComparator::getInstance().compareTags(w1->getTags(), w2->getTags());
  OneWayCriterion oneWayCrit;
  if (oneWayCrit.isSatisfied(w1) && oneWayCrit.isSatisfied(w2))
  {
    if (DirectionFinder::isSimilarDirection(map, w1, w2) == false)
    {
      score *= .1;
    }
  }
  // if we can't compare the scores, then just give it a 1. Hrmph.
  if (score < 0.0)
  {
    score = 1.0;
  }

  return score;
}

double ProbabilityOfMatch::distanceScore(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
                                         const ConstWayPtr& w2)
{
  return distanceScore(map, w1, ElementConverter(map).convertToLineString(w2), w2->getCircularError());
}

double ProbabilityOfMatch::distanceScore(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
  const std::shared_ptr<const LineString>& ls2, Meters circularError)
{
  Meters distanceSum = 0.0;

  vector<Coordinate> v;
  WayDiscretizer wd(map, w1);
  wd.discretize(2.0, v);

  _dMax = 0.0;

  for (size_t i = 0; i < v.size(); i++)
  {
    Point* point(GeometryFactory::getDefaultInstance()->createPoint(v[i]));
    LOG_VART(ls2->distance(point));
    double d = ls2->distance(point);
    distanceSum += d;
    _dMax = max(d, _dMax);
    delete point;
  }

  _dMax /= (circularError + w1->getCircularError());

  Meters distanceMean = distanceSum / v.size();

  // TODO: Make me better.
  // E.g. if s1 = 50 & s2 = 50, then sigma = 70.
  // This is a placeholder for the probability. Mike Porter will help me out w/ a better
  // approximation later.
  double s1 = w1->getCircularError() / 2.0;
  double s2 = circularError / 2.0;
  double sigma = sqrt(s1 * s1 + s2 * s2);

  // The rational here is that we can calculate the
  double p = 1 - (Normal::phi(distanceMean, sigma) - 0.5) * 2.0;

  LOG_TRACE("distanceMean: " << distanceMean);
  LOG_TRACE("  s1: " << s1 << " s2: " << s2 << " sigma: " << sigma << " p: " << p);

  return p;
}

ProbabilityOfMatch& ProbabilityOfMatch::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new ProbabilityOfMatch());
  }
  return *_theInstance;
}

double ProbabilityOfMatch::lengthScore(const ConstOsmMapPtr &map, const ConstWayPtr& w1,
  const ConstWayPtr &w2)
{
  Meters l1 = ElementConverter(map).convertToLineString(w1)->getLength();
  Meters l2 = ElementConverter(map).convertToLineString(w2)->getLength();

  // longer matches get a higher score.
  Meters mean = (l1 + l2) / 2.0;

  return 0.2 + ((mean / (mean + 20)) * 0.8);
}

double ProbabilityOfMatch::parallelScore(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
                                         const ConstWayPtr& w2)
{
  ParallelWayCriterion pwf(map, w1, true);

  double delta = pwf.calculateDifference(w2);
  return pow(cos(delta), _parallelExp);
}

double ProbabilityOfMatch::expertProbability(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
                                             const ConstWayPtr& w2)
{
  double ds = distanceScore(map, w1, w2);
  // weight this more heavily.
  double ps = parallelScore(map, w1, w2);
  double as = (attributeScore(map, w1, w2) * .7 + .3);
  double zs = zipperScore(w1, w2);
  double ls = lengthScore(map, w1, w2);

  LOG_TRACE("  ds2 " << distanceScore(map, w2, w1));
  LOG_TRACE("  l1 " << ElementConverter(map).convertToLineString(w1)->getLength());
  LOG_TRACE("  l2 " << ElementConverter(map).convertToLineString(w2)->getLength());
  LOG_TRACE("probability of match " << ds << " " << ps << " " << as << " " << zs << " " << ls);
  LOG_TRACE("  " << ds * ps * as * zs * ls);

  return ds * ps * as * zs * ls;
}

double ProbabilityOfMatch::zipperScore(const ConstWayPtr& w1,
                                       const ConstWayPtr& w2)
{
  double result = 1.0;

  if (w1->getNodeId(0) != w2->getNodeId(0))
  {
    result *= 0.5;
  }
  if (w1->getLastNodeId() != w2->getLastNodeId())
  {
    result *= 0.5;
  }

  return result;
}

}
