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
#include "NetworkDetails.h"

// hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/ProbabilityOfMatch.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/polygon/extractors/EuclideanDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/HausdorffDistanceExtractor.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/Factory.h>

namespace hoot
{

NetworkDetails::NetworkDetails(ConstOsmMapPtr map, ConstOsmNetworkPtr network) :
  _map(map),
  _network(network)
{
  _sublineMatcher.reset(
    Factory::getInstance().constructObject<SublineStringMatcher>(
      ConfigOptions().getHighwaySublineStringMatcher()));
  _classifier.reset(
    Factory::getInstance().constructObject<HighwayClassifier>(
      ConfigOptions().getConflateMatchHighwayClassifier()));

}

double NetworkDetails::getEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);

  ConstWayPtr w1 = dynamic_pointer_cast<const Way>(e1->getMembers()[0]);
  ConstWayPtr w2 = dynamic_pointer_cast<const Way>(e2->getMembers()[0]);

  double result;

  if (isCandidateMatch(e1, e2) == false)
  {
    result = 0.0;
  }
  else
  {
    result = ProbabilityOfMatch::getInstance().expertProbability(_map, w1, w2);
  }

  return result;
}

double NetworkDetails::getPartialEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);

  ConstWayPtr w1 = dynamic_pointer_cast<const Way>(e1->getMembers()[0]);
  ConstWayPtr w2 = dynamic_pointer_cast<const Way>(e2->getMembers()[0]);

  //LOG_VAR(ElementConverter(_map).convertToGeometry(w1)->toString());
  //LOG_VAR(ElementConverter(_map).convertToGeometry(w2)->toString());

  // calculated the shared sublines
  WaySublineMatchString sublineMatch = _sublineMatcher->findMatch(_map, w1, w2,
    ConfigOptions().getSearchRadiusHighway());

  MatchClassification c;
  if (sublineMatch.isValid())
  {
    // calculate the match score
    c = _classifier->classify(_map, w1->getElementId(), w2->getElementId(), sublineMatch);
  }

  return c.getMatchP();
}

Envelope NetworkDetails::getEnvelope(ConstNetworkEdgePtr e)
{
  auto_ptr<Envelope> env(e->getMembers()[0]->getEnvelope(_map));
  for (int i = 1; i < e->getMembers().size(); ++i)
  {
    auto_ptr<Envelope> env2(e->getMembers()[i]->getEnvelope(_map));
    env->expandToInclude(env2.get());
  }

  return *env;
}

Envelope NetworkDetails::getEnvelope(ConstNetworkVertexPtr v)
{
  auto_ptr<Envelope> env(v->getElement()->getEnvelope(_map));

  return *env;
}

Meters NetworkDetails::getSearchRadius(ConstNetworkEdgePtr e)
{
  double ce = -1;

  for (int i = 0; i < e->getMembers().size(); ++i)
  {
    if (ce < e->getMembers()[0]->getCircularError())
    {
      ce = e->getMembers()[0]->getCircularError();
    }
  }

  return ce;
}

Meters NetworkDetails::getSearchRadius(ConstNetworkVertexPtr v)
{
  return v->getElement()->getCircularError();
}

bool NetworkDetails::isCandidateMatch(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  Meters ce1 = getSearchRadius(e1);
  Meters ce2 = getSearchRadius(e2);
  Meters ce = sqrt(ce1 * ce1 + ce2 * ce2);

  assert(e1->getMembers().size() == 1 && e2->getMembers().size() == 1);

  ConstElementPtr ep1 = e1->getMembers()[0];
  ConstElementPtr ep2 = e2->getMembers()[0];

  double d = EuclideanDistanceExtractor().distance(*_map, ep1, ep2);
  bool result = d <= ce;

  double hd = HausdorffDistanceExtractor().distance(*_map, ep1, ep2);
  result = result && hd <= ce;

  AngleHistogramExtractor ahe;
  ahe.setSmoothing(toRadians(20.0));
  double ah = ahe.extract(*_map, ep1, ep2);
  result = result && ah >= 0.5;

  return result;
}

bool NetworkDetails::isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  Meters ce = max(getSearchRadius(v1), getSearchRadius(v2));

  double d = EuclideanDistanceExtractor().distance(*_map, v1->getElement(), v2->getElement());
  return d <= ce;
}

bool NetworkDetails::isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);

  ConstWayPtr w1 = dynamic_pointer_cast<const Way>(e1->getMembers()[0]);
  ConstWayPtr w2 = dynamic_pointer_cast<const Way>(e2->getMembers()[0]);

  // calculated the shared sublines
  WaySublineMatchString sublineMatch = _sublineMatcher->findMatch(_map, w1, w2,
    ConfigOptions().getSearchRadiusHighway());

  if (sublineMatch.getReverseVector2().size() != 1)
  {
    throw NotImplementedException();
  }

  return sublineMatch.getReverseVector2()[0];
}

}
