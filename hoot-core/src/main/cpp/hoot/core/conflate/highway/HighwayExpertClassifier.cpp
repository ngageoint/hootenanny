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
#include "HighwayExpertClassifier.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/algorithms/MaximalNearestSubline.h>
#include <hoot/core/algorithms/ProbabilityOfMatch.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/ops/CopySubsetOp.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(HighwayClassifier, HighwayExpertClassifier)

HighwayExpertClassifier::HighwayExpertClassifier()
{
}

MatchClassification HighwayExpertClassifier::classify(const ConstOsmMapPtr& map,
  ElementId /*eid1*/, ElementId /*eid2*/, const WaySublineMatchString &match)
{
  // calculate the average classification. Is there a better approach? Max, min, mean? Dunno.
  MatchClassification result;

  for (size_t i = 0; i < match.getMatches().size(); i++)
  {
    MatchClassification m = classify(map, match.getMatches()[i]);

    result.setMatchP(m.getMatchP() + result.getMatchP());
    result.setMissP(m.getMissP() + result.getMissP());
    result.setReviewP(m.getReviewP() + result.getReviewP());
  }

  if (result.getMatchP() + result.getMissP() + result.getReviewP() == 0.0)
  {
    result.setMiss();
  }

  result.normalize();

  return result;
}

MatchClassification HighwayExpertClassifier::classify(const ConstOsmMapPtr& map,
  const WaySublineMatch& match)
{
  MatchClassification result;

  OsmMapPtr mapCopy(new OsmMap());
  CopySubsetOp(map,
               match.getSubline1().getElementId(),
               match.getSubline2().getElementId()).apply(mapCopy);

  if (match.isValid() == false)
  {
    result.setMissP(1.0);
    result.setMatchP(0.0);
    result.setReviewP(0.0);
    return result;
  }

  WayPtr sl1 = match.getSubline1().toWay(mapCopy);
  WayPtr sl2 = match.getSubline2().toWay(mapCopy);

  ElementConverter ec(mapCopy);
  Meters l1 = ec.convertToLineString(match.getSubline1().getWay())->getLength();
  Meters l2 = ec.convertToLineString(match.getSubline2().getWay())->getLength();

  // what portion of the original lines is the MNS
  double po1 = ec.convertToLineString(sl1)->getLength() / l1;
  double po2 = ec.convertToLineString(sl2)->getLength() / l2;

  // give it a score
  double ps = std::min(po1, po2) / 2.0 + 0.5;

  double p;

  // if either of the lines are zero in length.
  if (po1 == 0 || po2 == 0)
  {
    p = 0.0;
  }
  else
  {
    p = ps * ProbabilityOfMatch::getInstance().expertProbability(mapCopy, sl1, sl2);
  }

  result.setMatchP(p);
  result.setMissP(1.0 - p);

  return result;
}

map<QString, double> HighwayExpertClassifier::getFeatures(const shared_ptr<const OsmMap>& /*m*/,
  ElementId /*eid1*/, ElementId /*eid2*/, const WaySublineMatchString& /*match*/) const
{
  return map<QString, double>();
}


}
