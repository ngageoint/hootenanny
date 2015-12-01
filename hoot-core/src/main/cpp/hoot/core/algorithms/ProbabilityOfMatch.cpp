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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ProbabilityOfMatch.h"

// GEOS
#include <geos/geom/LineString.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/filters/ParallelWayFilter.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>

// Standard
#include <vector>
using namespace std;

// Tgs
#include <tgs/StreamUtils.h>
#include <tgs/Statistics/Normal.h>
using namespace Tgs;

#include "WayDiscretizer.h"

namespace hoot
{

ProbabilityOfMatch ProbabilityOfMatch::_default;
bool ProbabilityOfMatch::debug = false;

ProbabilityOfMatch::ProbabilityOfMatch()
{
  _useModel = Settings::getInstance().getBool("match.model.enabled", false);

  QString model = Settings::getInstance().getString("match.model.name", "dc-attributes");
  _parallelExp = Settings::getInstance().getDouble("match.parallel.exponent", 1);

  if (_useModel)
  {
    LOG_INFO("Using model: " << model);
  }

  if (model == "dc-attributes")
  {
    // using attributes on DC perm7-1to3.arff
    LogisticRegression::Coeff coeff;
    coeff["AttributeScore"] = 121.9717;
    coeff["DistanceScore"] = -2.9825;
    coeff["LengthScore"] = -8.254;
    coeff["ParallelScore"] = 35.6726;
    coeff["PortionScore"] = 18.4649;
    coeff["ProbabilityScore"] = 20.8245;
    _model.setIntercept(-166.5986);
    _model.setCoefficients(coeff);
  }
  else if (model == "dc-noattributes")
  {
    // no attributes on DC perm7-1to3-noattr.arff
    LogisticRegression::Coeff coeff;
    coeff["DistanceScore"] = 1.3947;
    coeff["LengthScore"] = -4.9846;
    coeff["ParallelScore"] = 56.557;
    coeff["PortionScore"] = 18.7814;
    coeff["ProbabilityScore"] = -0.1394;
    _model.setIntercept(-68.4266);
    _model.setCoefficients(coeff);
  }
  else if (model == "dc-attr-intersection")
  {
    LogisticRegression::Coeff coeff;
    coeff["AttributeScore"] = 173.5906;
    coeff["DistanceScore"] = -8.2995;
    coeff["DistanceScoreMax"] = -16.6474;
    coeff["IntersectionScore"] = 19.8346;
    coeff["LengthScore"] = -0.9609;
    coeff["ParallelScore"] = 11.6577;
    coeff["PortionScore"] = 16.146;
    coeff["ProbabilityScore"] = -4.949;
    _model.setIntercept(-205.7471);
    _model.setCoefficients(coeff);
  }
  else
  {
    throw HootException("Unrecognized model name.");
  }
}

double ProbabilityOfMatch::attributeScore(const ConstOsmMapPtr& map,
  const shared_ptr<const Way>& w1, const shared_ptr<const Way>& w2)
{
  double score = 1.0;

//  if (w1->getTags()["oneway"] != w2->getTags()["oneway"])
//  {
//    score *= .5;
//  }
//  if (w1->getTags()["tunnel"] != w2->getTags()["tunnel"])
//  {
//    score *= .5;
//  }
//  if (w1->getTags()["bridge"] != w2->getTags()["bridge"])
//  {
//    score *= .5;
//  }
//  if (w1->getTags()["highway"] != w2->getTags()["highway"])
//  {
//    score *= .5;
//  }
  score = TagComparator::getInstance().compareTags(w1->getTags(), w2->getTags());
  if (w1->isOneWay() && w2->isOneWay())
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

map<QString, double> ProbabilityOfMatch::calculateStats(const ConstOsmMapPtr& map,
  const shared_ptr<const Way>& w1, const shared_ptr<const Way>& w2, double portionScore)
{
  double ds = distanceScore(map, w1, w2);
  double dsMax = _dMax;
  // weight this more heavily.
  double ps = parallelScore(map, w1, w2);
  double as = (attributeScore(map, w1, w2) * .7 + .3);
  double zs = zipperScore(map, w1, w2);
  double ls = lengthScore(map, w1, w2);

  std::map<QString, double> sample;
  sample["AttributeScore"] = as;
  sample["DistanceScore"] = ds;
  sample["DistanceScoreMax"] = dsMax;
  sample["LengthScore"] = ls;
  sample["ParallelScore"] = ps;
  sample["PortionScore"] = portionScore;
  sample["ProbabilityScore"] = ds * ps * as * zs * ls;
  sample["ZipperScore"] = zs;

  return sample;
}

double ProbabilityOfMatch::distanceScore(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
                                         const shared_ptr<const Way>& w2)
{
  return distanceScore(map, w1, ElementConverter(map).convertToLineString(w2), w2->getCircularError());
}

double ProbabilityOfMatch::distanceScore(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
  const shared_ptr<const LineString>& ls2, Meters circularError)
{
  Meters distanceSum = 0.0;

  vector<Coordinate> v;
  WayDiscretizer wd(map, w1);
  wd.discretize(2.0, v);

  _dMax = 0.0;

  for (size_t i = 0; i < v.size(); i++)
  {
    Point* point(GeometryFactory::getDefaultInstance()->createPoint(v[i]));
    if (debug)
    {
      cout << "distance " << ls2->distance(point) << endl;
    }
    double d = ls2->distance(point);
    distanceSum += d;
    _dMax = max(d, _dMax);
    delete point;
  }

  _dMax /= (circularError + w1->getCircularError());

  Meters distanceMean = distanceSum / v.size();

  /// @todo Make me better.
  // E.g. if s1 = 50 & s2 = 50, then sigma = 70.
  // This is a placeholder for the probability. Mike Porter will help me out w/ a better
  // approximation later.
  double s1 = w1->getCircularError() / 2.0;
  double s2 = circularError / 2.0;
  double sigma = sqrt(s1 * s1 + s2 * s2);

  // The rational here is that we can calculate the
  double p = 1 - (Normal::phi(distanceMean, sigma) - 0.5) * 2.0;

  if (debug)
  {
    LOG_INFO("" << "distanceMean: " << distanceMean);
    LOG_INFO("" << "  s1: " << s1 << " s2: " << s2 << " sigma: " << sigma << " p: " << p);
  }

  return p;
}

double ProbabilityOfMatch::lengthScore(const ConstOsmMapPtr &map, const shared_ptr<const Way>& w1,
  const shared_ptr<const Way> &w2)
{
  Meters l1 = ElementConverter(map).convertToLineString(w1)->getLength();
  Meters l2 = ElementConverter(map).convertToLineString(w2)->getLength();

  // longer matches get a higher score.
  Meters mean = (l1 + l2) / 2.0;

  return 0.2 + ((mean / (mean + 20)) * 0.8);
}

double ProbabilityOfMatch::modelProbability(const ConstOsmMapPtr& map,
  const shared_ptr<const Way>& w1, const shared_ptr<const Way>& w2, double portionScore)
{
  std::map<QString, double> sample = calculateStats(map, w1, w2, portionScore);

  return _model.evaluate(sample);
}

double ProbabilityOfMatch::parallelScore(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
                                         const shared_ptr<const Way>& w2)
{
  ParallelWayFilter pwf(map, w1, true);

  double delta = pwf.calculateDifference(w2);
  return pow(cos(delta), _parallelExp);
}

double ProbabilityOfMatch::probability(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
                                       const shared_ptr<const Way>& w2,
  double portionScore)
{
  if (_useModel)
  {
    return modelProbability(map, w1, w2, portionScore);
  }
  else
  {
    return expertProbability(map, w1, w2);
  }
}

double ProbabilityOfMatch::expertProbability(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
                                             const shared_ptr<const Way>& w2)
{
  double ds = distanceScore(map, w1, w2);
  // weight this more heavily.
  double ps = parallelScore(map, w1, w2);
  double as = (attributeScore(map, w1, w2) * .7 + .3);
  double zs = zipperScore(map, w1, w2);
  double ls = lengthScore(map, w1, w2);

  if (debug)
  {
    LOG_INFO("  ds2 " << distanceScore(map, w2, w1));
    LOG_INFO("  l1 " << ElementConverter(map).convertToLineString(w1)->getLength());
    LOG_INFO("  l2 " << ElementConverter(map).convertToLineString(w2)->getLength());
    LOG_INFO("" << "probability of match " << ds << " " << ps << " " << as << " " << zs);
    LOG_INFO("" << "  " << ds * ps * as * zs);
  }

  return ds * ps * as * zs * ls;
}

double ProbabilityOfMatch::zipperScore(const ConstOsmMapPtr& /*map*/,
  const shared_ptr<const Way>& w1, const shared_ptr<const Way>& w2)
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
