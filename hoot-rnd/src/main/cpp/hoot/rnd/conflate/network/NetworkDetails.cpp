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
#include <hoot/core/algorithms/WayMatchStringMerger.h>
#include <hoot/core/algorithms/linearreference/NaiveWayMatchStringMapping.h>
#include <hoot/core/algorithms/linearreference/WayMatchStringMappingConverter.h>
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

Meters NetworkDetails::calculateLength(ConstNetworkEdgePtr e)
{
  assert(e->getMembers().size() == 1);

  return ElementConverter(_map).calculateLength(e->getMembers()[0]);
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

double NetworkDetails::getEdgeStringMatchScore(ConstEdgeStringPtr e1, ConstEdgeStringPtr e2)
{
  WayStringPtr ws1 = toWayString(e1);
  WayStringPtr ws2 = toWayString(e2);
  RelationPtr r1(new Relation(Status::Unknown1, _map->createNextRelationId(), 15));
  r1->setType("multilinestring");
  RelationPtr r2(new Relation(Status::Unknown1, _map->createNextRelationId(), 15));
  r2->setType("multilinestring");

  // create a set of all the way IDs
  set<long> widSet;
  for (int i = 0; i < ws1->getSize(); ++i)
  {
    widSet.insert(ws1->at(i).getWay()->getId());
    r1->addElement("", ws1->at(i).getWay());
  }
  for (int i = 0; i < ws2->getSize(); ++i)
  {
    widSet.insert(ws2->at(i).getWay()->getId());
    r2->addElement("", ws2->at(i).getWay());
  }
  vector<long> wids;
  wids.insert(wids.begin(), widSet.begin(), widSet.end());

  // create a copy of the map to experiment with
  OsmMapPtr mapCopy = _map->copyWays(wids);
  mapCopy->addElement(r1);
  mapCopy->addElement(r2);

  WayMatchStringMappingPtr mapping(new NaiveWayMatchStringMapping(ws1, ws2));

  // convert from a mapping to a WaySublineMatchString
  WaySublineMatchStringPtr matchString = WayMatchStringMappingConverter().toWaySublineMatchString(
    mapping);

  MatchClassification c;
  // calculate the match score
  c = _classifier->classify(mapCopy, r1->getElementId(), r2->getElementId(), *matchString);
  LOG_VARD(ws1->calculateLength());
  LOG_VARD(ws2->calculateLength());
  LOG_VARD(c);

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

double NetworkDetails::getPartialEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);

  ConstWayPtr w1 = dynamic_pointer_cast<const Way>(e1->getMembers()[0]);
  ConstWayPtr w2 = dynamic_pointer_cast<const Way>(e2->getMembers()[0]);

  const SublineCache& sc = _getSublineCache(w1, w2);

  return sc.p;
}

Meters NetworkDetails::getSearchRadius(ConstNetworkEdgePtr e) const
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

Meters NetworkDetails::getSearchRadius(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const
{
  Meters ce1 = getSearchRadius(e1);
  Meters ce2 = getSearchRadius(e2);

  return sqrt(ce1 * ce1 + ce2 * ce2);
}

Meters NetworkDetails::getSearchRadius(ConstNetworkVertexPtr v) const
{
  return v->getElement()->getCircularError();
}

Meters NetworkDetails::getSearchRadius(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const
{
  Meters ce1 = getSearchRadius(v1);
  Meters ce2 = getSearchRadius(v2);

  return sqrt(ce1 * ce1 + ce2 * ce2);
}

const NetworkDetails::SublineCache& NetworkDetails::_getSublineCache(ConstWayPtr w1, ConstWayPtr w2)
{
  ElementId e1 = w1->getElementId();
  ElementId e2 = w2->getElementId();
  if (_sublineCache[e1].contains(e2))
  {
    return _sublineCache[e1][e2];
  }

  // calculated the shared sublines
  WaySublineMatchString sublineMatch = _sublineMatcher->findMatch(_map, w1, w2,
    ConfigOptions().getSearchRadiusHighway());

  MatchClassification c;
  bool reversed = false;
  if (sublineMatch.isValid())
  {
    // calculate the match score
    c = _classifier->classify(_map, w1->getElementId(), w2->getElementId(), sublineMatch);

    if (sublineMatch.getReverseVector2().size() != 1)
    {
      LOG_VAR(sublineMatch.getReverseVector2());
      throw NotImplementedException();
    }

    reversed = sublineMatch.getReverseVector2()[0];
  }

  _sublineCache[e1][e2].p = c.getMatchP();
  _sublineCache[e1][e2].reversed = reversed;

  return _sublineCache[e1][e2];
}

bool NetworkDetails::isCandidateMatch(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  Meters ce = getSearchRadius(e1, e2);

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
  Meters ce = getSearchRadius(v1, v2);

  double d = EuclideanDistanceExtractor().distance(*_map, v1->getElement(), v2->getElement());
  return d <= ce;
}

bool NetworkDetails::isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);

  ConstWayPtr w1 = dynamic_pointer_cast<const Way>(e1->getMembers()[0]);
  ConstWayPtr w2 = dynamic_pointer_cast<const Way>(e2->getMembers()[0]);

  const SublineCache& sc = _getSublineCache(w1, w2);

  return sc.reversed;
}

WayStringPtr NetworkDetails::toWayString(ConstEdgeStringPtr e) const
{
  QList<EdgeString::EdgeEntry> edges = e->getAllEdges();
  WayStringPtr ws(new WayString());

  for (int i = 0; i < edges.size(); ++i)
  {
    ConstNetworkEdgePtr e = edges[i].e;
    if (e->getMembers().size() != 1 || e->getMembers()[0]->getElementType() != ElementType::Way)
    {
      throw IllegalArgumentException("Expected a network edge with exactly 1 way.");
    }
    ConstWayPtr w = dynamic_pointer_cast<const Way>(e->getMembers()[0]);

    WaySubline s(WayLocation(_map, w, 0), WayLocation::createAtEndOfWay(_map, w));
    if (edges[i].reversed)
    {
      s = WaySubline(s.getEnd(), s.getStart());
    }

    ws->append(s);
  }

  return ws;
}

}
