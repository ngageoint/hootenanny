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
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/WayMatchStringMerger.h>
#include <hoot/core/algorithms/linearreference/NaiveWayMatchStringMapping.h>
#include <hoot/core/algorithms/linearreference/WayMatchStringMappingConverter.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/polygon/extractors/EuclideanDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/HausdorffDistanceExtractor.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/Factory.h>

namespace hoot
{

NetworkDetails::NetworkDetails(ConstOsmMapPtr map, ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2) :
  _map(map),
  _n1(n1),
  _n2(n2)
{
  _sublineMatcher.reset(
    Factory::getInstance().constructObject<SublineStringMatcher>(
      ConfigOptions().getHighwaySublineStringMatcher()));
//  _classifier.reset(
//    Factory::getInstance().constructObject<HighwayClassifier>(
//      ConfigOptions().getConflateMatchHighwayClassifier()));

  _classifier.reset(
    Factory::getInstance().constructObject<HighwayClassifier>(
      QString::fromAscii("hoot::HighwayExpertClassifier")));

}

Radians NetworkDetails::calculateHeadingAtVertex(ConstNetworkEdgePtr e, ConstNetworkVertexPtr v)
{
  if (e->getMembers().size() != 1 || e->getMembers()[0]->getElementType() != ElementType::Way)
  {
    LOG_VAR(e);
    throw IllegalArgumentException("The input edge must have exactly 1 way as its member.");
  }

  ConstWayPtr w = dynamic_pointer_cast<const Way>(e->getMembers()[0]);
  Radians result;
  if (v == e->getFrom())
  {
    result = WayHeading::calculateHeading(WayLocation(_map, w, 0.0), 5.0);
  }
  else if (v == e->getTo())
  {
    result = WayHeading::calculateHeading(WayLocation::createAtEndOfWay(_map, w), -5.0);
  }
  else
  {
    throw IllegalArgumentException("'v' must be either the from or to vertex on edge 'e'.");
  }

  return result;
}

Meters NetworkDetails::calculateLength(ConstNetworkEdgePtr e) const
{
  assert(e->getMembers().size() == 1);

  return ElementConverter(_map).calculateLength(e->getMembers()[0]);
}

Meters NetworkDetails::calculateLength(ConstEdgeSublinePtr e) const
{
  Meters l = calculateLength(e->getEdge());

  return (e->getLatter()->getPortion() - e->getFormer()->getPortion()) * l;
}

QList<EdgeSublineMatchPtr> NetworkDetails::calculateMatchingSublines(ConstNetworkEdgePtr e1,
  ConstNetworkEdgePtr e2)
{
  QList<EdgeSublineMatchPtr> result;

  if (e1->isStub() || e2->isStub())
  {
    return result;
  }

  const NetworkDetails::SublineCache& cache = _getSublineCache(toWay(e1), toWay(e2));

  const WaySublineMatchString::MatchCollection& matches = cache.matches->getMatches();

  foreach (const WaySublineMatch& wsm, matches)
  {
    EdgeSublinePtr subline2 = _toEdgeSubline(wsm.getSubline2(), e2);
    if (wsm.isReverseMatch())
    {
      subline2->reverse();
    }
    EdgeSublineMatchPtr m(new EdgeSublineMatch(
      _toEdgeSubline(wsm.getSubline1(), e1),
      subline2));
    result.append(m);
  }

  return result;
}

QList<ConstNetworkVertexPtr> NetworkDetails::getCandidateMatches(ConstNetworkVertexPtr v)
{
  return _getVertexMatcher()->getCandidateMatches(v);
}

double NetworkDetails::_getEdgeAngleScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2,
  ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  double score = 1.0;

  // if this edge is a simple way type.
  if (e1->getMembers().size() == 1 && e1->getMembers()[0]->getElementType() == ElementType::Way &&
      e2->getMembers().size() == 1 && e2->getMembers()[0]->getElementType() == ElementType::Way)
  {
    Radians theta1 = calculateHeadingAtVertex(e1, v1);
    Radians theta2 = calculateHeadingAtVertex(e2, v2);

    Radians diff = WayHeading::deltaMagnitude(theta1, theta2);

    // if the diff in angle is > 90deg then set score to 0
    if (diff > M_PI / 2.0)
    {
      score = 0.0;
    }
    else
    {
      score = cos(diff);
    }
  }

  return score;
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
  double result;

  // if they're both stubs we don't need to match them
  if (e1->isStub() && e2->isStub())
  {
    result = 0.0;
  }
  // if either string is a stub, then return 1 on candidate.
  else if (e1->isStub() || e2->isStub())
  {
    ConstEdgeStringPtr stub = e1->isStub() ? e1 : e2;
    ConstEdgeStringPtr notStub = e1->isStub() ? e2 : e1;

    bool candidate = true;

    if (notStub->calculateLength(_map) > ConfigOptions().getNetworkMaxStubLength())
    {
      candidate = false;
    }

    foreach (EdgeString::EdgeEntry ee, notStub->getAllEdges())
    {
      candidate = candidate && isCandidateMatch(ee.getEdge(), stub->getFirstEdge());
    }

    result = candidate ? 1.0 : 0.0;
  }
  // if these are typical way edges
  else
  {
    WayStringPtr ws1 = toWayString(e1);
    WayStringPtr ws2 = toWayString(e2);

    Meters sr = getSearchRadius(ws1, ws2);

    // if this is a partial match in the middle of an edge then we want to be more strict about
    // accepting the match.
    if (e1->isFullPartial() || e2->isFullPartial())
    {
      sr *= 2;
    }

    // we won't even try to make partial matches smaller than the search radius. It just creates too
    // much noise.
    if (ws1->calculateLength() < sr && ws2->calculateLength() < sr &&
      (e1->isPartial() || e2->isPartial()))
    {
      result = 0.0;
    }
    else
    {
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

      // create a copy of the map for experimentation
      OsmMapPtr mapCopy(new OsmMap(_map->getProjection()));
      CopySubsetOp(_map, wids).apply(mapCopy);
      // we don't want to impact the global IDs with our little test.
      mapCopy->setIdGenerator(_map->getIdGenerator().clone());
      mapCopy->addElement(r1);
      mapCopy->addElement(r2);

//      WayPtr w1 = ws1->copySimplifiedWayIntoMap(*_map, mapCopy);
//      WayPtr w2 = ws2->copySimplifiedWayIntoMap(*_map, mapCopy);

//      double hd = HausdorffDistanceExtractor().distance(*mapCopy, w1, w2);
//      if (hd > sr)
//      {
//        LOG_VAR(hd);
//        LOG_VAR(sr);
//        result = 0.0;
//      }
//      else
      {
        WayMatchStringMappingPtr mapping(new NaiveWayMatchStringMapping(ws1, ws2));

        // convert from a mapping to a WaySublineMatchString
        WaySublineMatchStringPtr matchString = WayMatchStringMappingConverter().toWaySublineMatchString(
          mapping);

        LOG_VAR(matchString);

        MatchClassification c;
        // calculate the match score
        c = _classifier->classify(mapCopy, r1->getElementId(), r2->getElementId(), *matchString);

        result = c.getMatchP();
      }
    }
  }

  return result;
}

Envelope NetworkDetails::getEnvelope(ConstNetworkEdgePtr e) const
{
  auto_ptr<Envelope> env(e->getMembers()[0]->getEnvelope(_map));
  for (int i = 1; i < e->getMembers().size(); ++i)
  {
    auto_ptr<Envelope> env2(e->getMembers()[i]->getEnvelope(_map));
    env->expandToInclude(env2.get());
  }

  return *env;
}

Envelope NetworkDetails::getEnvelope(ConstNetworkVertexPtr v) const
{
  auto_ptr<Envelope> env(v->getElement()->getEnvelope(_map));

  return *env;
}

double NetworkDetails::getPartialEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);
  double result;

  if (e1->isStub() || e2->isStub())
  {
    if (isCandidateMatch(e1, e2))
    {
      result = 0.5;
    }
    else
    {
      result = 0.0;
    }
  }
  else
  {
    double bestScore = -1.0;
    if (isCandidateMatch(e1->getFrom(), e2->getFrom()))
    {
      bestScore = max(bestScore, _getEdgeAngleScore(e1->getFrom(), e2->getFrom(), e1, e2));
    }
    if (isCandidateMatch(e1->getTo(), e2->getTo()))
    {
      bestScore = max(bestScore, _getEdgeAngleScore(e1->getTo(), e2->getTo(), e1, e2));
    }
    if (isCandidateMatch(e1->getFrom(), e2->getTo()))
    {
      bestScore = max(bestScore, _getEdgeAngleScore(e1->getFrom(), e2->getTo(), e1, e2));
    }
    if (isCandidateMatch(e1->getTo(), e2->getFrom()))
    {
      bestScore = max(bestScore, _getEdgeAngleScore(e1->getTo(), e2->getFrom(), e1, e2));
    }

    // this is a partial match
    if (bestScore == -1.0)
    {
      bestScore = 1.0;
    }

    ConstWayPtr w1 = dynamic_pointer_cast<const Way>(e1->getMembers()[0]);
    ConstWayPtr w2 = dynamic_pointer_cast<const Way>(e2->getMembers()[0]);

    const SublineCache& sc = _getSublineCache(w1, w2);
    LOG_VAR(sc.p);
    LOG_VAR(bestScore);
    result = sc.p * bestScore;
  }

  return result;
}

Meters NetworkDetails::getSearchRadius(ConstNetworkEdgePtr e) const
{
  double ce = -1;

  if (e->isStub())
  {
    foreach (ConstNetworkEdgePtr e, _n2->getEdgesFromVertex(e->getFrom()))
    {
      if (e->isStub() == false)
      {
        ce = std::max(ce, getSearchRadius(e));
      }
    }

    if (ce == -1)
    {
      ce = e->getFrom()->getElement()->getCircularError();
    }
  }
  else
  {
    for (int i = 0; i < e->getMembers().size(); ++i)
    {
      if (ce < e->getMembers()[0]->getCircularError())
      {
        ce = e->getMembers()[0]->getCircularError();
      }
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
  // use the ce of the containing edge, not the vertex.
  Meters ce = -1;
  foreach (ConstNetworkEdgePtr e, _n1->getEdgesFromVertex(v))
  {
    ce = std::max(ce, getSearchRadius(e));
  }
  foreach (ConstNetworkEdgePtr e, _n2->getEdgesFromVertex(v))
  {
    ce = std::max(ce, getSearchRadius(e));
  }

  return ce;
}

Meters NetworkDetails::getSearchRadius(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const
{
  Meters ce1 = getSearchRadius(v1);
  Meters ce2 = getSearchRadius(v2);

  Meters result = sqrt(ce1 * ce1 + ce2 * ce2);
  return result;
}

Meters NetworkDetails::getSearchRadius(ConstWayStringPtr ws1, ConstWayStringPtr ws2) const
{
  Meters ce1 = ws1->getMaxCircularError();
  Meters ce2 = ws2->getMaxCircularError();

  Meters result = sqrt(ce1 * ce1 + ce2 * ce2);
  return result;
}

Meters NetworkDetails::getSearchRadius(ConstWayPtr w1, ConstWayPtr w2) const
{
  Meters ce1 = w1->getCircularError();
  Meters ce2 = w2->getCircularError();

  Meters result = sqrt(ce1 * ce1 + ce2 * ce2);
  return result;
}

const NetworkDetails::SublineCache& NetworkDetails::_getSublineCache(ConstWayPtr w1, ConstWayPtr w2)
{
  ElementId e1 = w1->getElementId();
  ElementId e2 = w2->getElementId();
  if (_sublineCache[e1].contains(e2))
  {
    return _sublineCache[e1][e2];
  }

  Meters srh = ConfigOptions().getSearchRadiusHighway();
  double sr = srh <= 0.0 ? getSearchRadius(w1, w2) : srh;
  LOG_VART(srh);
  LOG_VART(sr);
  //_sublineMatcher->setMinSplitSize(sr / 2.0);
  // calculated the shared sublines
  WaySublineMatchString sublineMatch = _sublineMatcher->findMatch(_map, w1, w2, sr);

  MatchClassification c;
  bool reversed = false;
  LOG_VART(sr);
  LOG_VART(sublineMatch);
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
  _sublineCache[e1][e2].matches = WaySublineMatchStringPtr(new WaySublineMatchString(sublineMatch));

  return _sublineCache[e1][e2];
}

double NetworkDetails::getVertexMatchScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  double score = _getVertexMatcher()->scoreMatch(v1, v2);

  if (score == 0.0 && isCandidateMatch(v1, v2))
  {
    score = 1.0;
  }

  return score;
}

LegacyVertexMatcherPtr NetworkDetails::_getVertexMatcher()
{
  if (!_vertexMatcher)
  {
    LOG_INFO("Creating vertex matcher");
    _vertexMatcher.reset(new LegacyVertexMatcher(_map));
    _vertexMatcher->identifyVertexMatches(_n1, _n2, *this);
  }

  return _vertexMatcher;
}

bool NetworkDetails::hasConfidentTiePoint(ConstNetworkVertexPtr v)
{
  return _getVertexMatcher()->hasConfidentTiePoint(v);
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

  if (ep1->getElementType() == ElementType::Way && ep2->getElementType() == ElementType::Way)
  {
    AngleHistogramExtractor ahe;
    ahe.setSmoothing(toRadians(20.0));
    double ah = ahe.extract(*_map, ep1, ep2);
    result = result && ah >= 0.5;
  }

  return result;
}

bool NetworkDetails::isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  return _getVertexMatcher()->isCandidateMatch(v1, v2, *this);
}

bool NetworkDetails::isPartialCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2,
  ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  double score = getPartialEdgeMatchScore(e1, e2);
  score *= _getEdgeAngleScore(v1, v2, e1, e2);

  return score >= ConfigOptions().getConflatorMinValidScore();
}

bool NetworkDetails::isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);
  bool result;

  if (e1->isStub() || e2->isStub())
  {
    result = false;
  }
  else
  {
    ConstWayPtr w1 = toWay(e1);
    ConstWayPtr w2 = toWay(e2);

    const SublineCache& sc = _getSublineCache(w1, w2);

    result = sc.reversed;
  }

  return result;
}

EdgeSublinePtr NetworkDetails::_toEdgeSubline(const WaySubline& ws, ConstNetworkEdgePtr e)
{
  EdgeSublinePtr result;

  Meters l = ElementConverter(_map).calculateLength(ws.getWay());
  result.reset(new EdgeSubline(e, ws.getStart().calculateDistanceOnWay() / l,
    ws.getEnd().calculateDistanceOnWay() / l));

  return result;
}

ConstWayPtr NetworkDetails::toWay(ConstNetworkEdgePtr e) const
{
  if (e->getMembers().size() != 1)
  {
    throw IllegalArgumentException("Expected e to contain a single way.");
  }

  ConstWayPtr w = dynamic_pointer_cast<const Way>(e->getMembers()[0]);

  if (!w)
  {
    throw IllegalArgumentException("Expected e to contain a single way.");
  }

  return w;
}

WayStringPtr NetworkDetails::toWayString(ConstEdgeStringPtr e, const EidMapper& mapper) const
{
  QList<EdgeString::EdgeEntry> edges = e->getAllEdges();
  WayStringPtr ws(new WayString());

  for (int i = 0; i < edges.size(); ++i)
  {
    ConstNetworkEdgePtr e = edges[i].getEdge();
    const ConstEdgeSublinePtr& subline = edges[i].getSubline();
    LOG_VAR(subline);
    // ignore stubs while building way strings.
    if (e->isStub() == false)
    {
      if (e->getMembers().size() != 1 || e->getMembers()[0]->getElementType() != ElementType::Way)
      {
        LOG_VARW(e);
        throw IllegalArgumentException("Expected a network edge with exactly 1 way.");
      }
      ElementId eid = mapper.mapEid(e->getMembers()[0]->getElementId());
      ConstWayPtr w = dynamic_pointer_cast<const Way>(_map->getWay(eid));

      Meters l = calculateLength(e);
      double startP = subline->getStart()->getPortion();
      double endP = subline->getEnd()->getPortion();

      WaySubline s(WayLocation(_map, w, startP * l), WayLocation(_map, w, endP * l));
      LOG_VAR(s);
      LOG_VAR(w);

      ws->append(s);
    }
  }

  return ws;
}

}
