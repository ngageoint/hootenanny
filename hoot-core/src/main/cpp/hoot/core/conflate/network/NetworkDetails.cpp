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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "NetworkDetails.h"

// hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/ProbabilityOfMatch.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/algorithms/extractors/EuclideanDistanceExtractor.h>
#include <hoot/core/algorithms/extractors/HausdorffDistanceExtractor.h>
#include <hoot/core/algorithms/linearreference/NaiveWayMatchStringMapping.h>
#include <hoot/core/algorithms/linearreference/WayMatchStringMappingConverter.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcherFactory.h>
#include <hoot/core/conflate/highway/HighwayClassifier.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int NetworkDetails::logWarnCount = 0;

static double min(double a, double b, double c) { return std::min(a, std::min(b, c)); }

NetworkDetails::NetworkDetails(ConstOsmMapPtr map, ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2)
  : _map(map),
    _n1(n1),
    _n2(n2),
    _maxStubLength(ConfigOptions().getNetworkMaxStubLength())
{
  setConfiguration(conf());
}

void NetworkDetails::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _sublineMatcher = SublineStringMatcherFactory::getMatcher(CreatorDescription::BaseFeatureType::Highway);
  _classifier = Factory::getInstance().constructObject<HighwayClassifier>(opts.getConflateMatchHighwayClassifier());
}

Meters NetworkDetails::calculateDistance(ConstEdgeLocationPtr el) const
{
  return (el->getPortion()) * calculateLength(el->getEdge());
}

Radians NetworkDetails::calculateHeading(ConstEdgeLocationPtr el) const
{
  WayLocation wl(_map, toWay(el->getEdge()), calculateDistance(el));
  return WayHeading::calculateHeading(wl);
}

Radians NetworkDetails::calculateHeadingAtVertex(ConstNetworkEdgePtr e, ConstNetworkVertexPtr v) const
{  
  // Is it possible if e is a relation and has one way member, then we should use that for the way
  // here? Haven't seen evidence of that yet, so not implementing it.

  if (e->getMembers().size() != 1 || e->getMembers()[0]->getElementType() != ElementType::Way)
  {
    LOG_VART(e);
    LOG_VART(e->getMembers()[0]);
    throw IllegalArgumentException("The input edge must have exactly 1 way as its member.");
  }

  ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e->getMembers()[0]);
  Radians result;
  if (v == e->getFrom())
    result = WayHeading::calculateHeading(WayLocation(_map, w, 0.0), 5.0);
  else if (v == e->getTo())
    result = WayHeading::calculateHeading(WayLocation::createAtEndOfWay(_map, w), -5.0);
  else
    throw IllegalArgumentException("'v' must be either the from or to vertex on edge 'e'.");

  return result;
}

Meters NetworkDetails::calculateLength(ConstNetworkEdgePtr e) const
{
  assert(e->getMembers().size() == 1);
  return ElementGeometryUtils::calculateLength(e->getMembers()[0], _map);
}

Meters NetworkDetails::calculateLength(ConstEdgeSublinePtr e) const
{
  Meters l = calculateLength(e->getEdge());
  return (e->getLatter()->getPortion() - e->getFormer()->getPortion()) * l;
}

Meters NetworkDetails::calculateLength(ConstEdgeStringPtr e) const
{
  Meters l = 0.0;
  for (const auto& ee : e->getAllEdges())
    l += calculateLength(ee.getSubline());

  return l;
}

QList<EdgeSublineMatchPtr> NetworkDetails::calculateMatchingSublines(ConstNetworkEdgePtr e1,
                                                                     ConstNetworkEdgePtr e2)
{
  LOG_TRACE("Calculating matching sublines...");

  QList<EdgeSublineMatchPtr> result;

  if (e1->isStub() || e2->isStub())
    return result;

  const NetworkDetails::SublineCache& cache = _getSublineCache(toWay(e1), toWay(e2));

  const WaySublineMatchString::MatchCollection& matches = cache.getMatches()->getMatches();
  LOG_VART(matches);

  for (const auto& wsm : matches)
  {
    EdgeSublinePtr subline2 = _toEdgeSubline(wsm.getSubline2(), e2);
    if (wsm.isReverseMatch())
      subline2->reverse();
    result.append(std::make_shared<EdgeSublineMatch>(_toEdgeSubline(wsm.getSubline1(), e1), subline2));
  }

  return result;
}

void NetworkDetails::calculateNearestLocation(ConstEdgeStringPtr string, ConstEdgeSublinePtr subline,
                                              ConstEdgeLocationPtr& elString, ConstEdgeLocationPtr& elSubline) const
{
  LOG_DEBUG("Calculating nearest location...");

  elString.reset();
  elSubline.reset();

  // if the subline and string terminate on the from vertex.
  if (string->getFrom()->isExtreme() &&
    ((subline->getStart()->isExtreme() &&
      string->getFromVertex() == subline->getStart()->getVertex()) ||
     (subline->getEnd()->isExtreme() &&
      string->getFromVertex() == subline->getEnd()->getVertex())))
  {
    elString = string->getFrom();
    elSubline = elString;
  }
  // if the subline and string terminate on the to vertex.
  else if (string->getTo()->isExtreme() &&
           ((subline->getStart()->isExtreme() &&
             string->getToVertex() == subline->getStart()->getVertex()) ||
            (subline->getEnd()->isExtreme() &&
             string->getToVertex() == subline->getEnd()->getVertex())))
  {
    elString = string->getTo();
    elSubline = elString;
  }
  else if (string->contains(subline->getStart()))
  {
    elString = subline->getStart();
    elSubline = subline->getStart();
  }
  else if (string->contains(subline->getEnd()))
  {
    elString = subline->getEnd();
    elSubline = subline->getEnd();
  }
  else if (subline->contains(string->getFrom()))
  {
    elString = string->getFrom();
    elSubline = string->getFrom();
  }
  else if (subline->contains(string->getTo()))
  {
    elString = string->getTo();
    elSubline = string->getTo();
  }
  else
  {
    Meters stringDFrom = 0;
    Meters stringDTo = calculateLength(string);
    Meters sublineDFrom = calculateStringLocation(string, subline->getStart());
    Meters sublineDTo = calculateStringLocation(string, subline->getEnd());

    Meters fromFrom = fabs(stringDFrom - sublineDFrom);
    Meters fromTo = fabs(stringDFrom - sublineDTo);
    Meters toTo = fabs(stringDTo - sublineDTo);
    Meters toFrom = fabs(stringDTo - sublineDFrom);

    // if subline isn't on an edge that is part of string.
    if (sublineDFrom == numeric_limits<double>::max() ||
        sublineDTo == numeric_limits<double>::max())
    {
      elString.reset();
      elSubline.reset();
    }
    else if (fromFrom < min(fromTo, toTo, toFrom))
    {
      elString = string->getFrom();
      elSubline = subline->getStart();
    }
    else if (fromTo < min(fromFrom, toTo, toFrom))
    {
      elString = string->getFrom();
      elSubline = subline->getEnd();
    }
    else if (toTo < min(fromTo, fromFrom, toFrom))
    {
      elString = string->getTo();
      elSubline = subline->getEnd();
    }
    else
    {
      elString = string->getTo();
      elSubline = subline->getStart();
    }
  }
}

double NetworkDetails::calculateStringLocation(ConstEdgeStringPtr es, ConstEdgeLocationPtr el) const
{
  LOG_DEBUG("Calculating string location...");

  Meters beginningD = -calculateDistance(es->getFrom());
  ConstNetworkVertexPtr edgeStartVertex;

  if (es->getAllEdges()[0].isBackwards())
    edgeStartVertex = es->getFirstEdge()->getTo();
  else
    edgeStartVertex = es->getFirstEdge()->getFrom();

  // if el1 is not part of this string, but shares a vertex with the first edge then we can
  // still calculate and return the distance.
  if (el->isExtreme() && el->getVertex() == edgeStartVertex)
    return beginningD;

  for (const auto& ee : es->getAllEdges())
  {
    Meters l = calculateLength(ee.getEdge());
    if (el->getEdge() == ee.getEdge())
      return beginningD + calculateDistance(el);

    beginningD += l;
  }

  ConstNetworkVertexPtr edgeEndVertex;

  if (es->getAllEdges().back().isBackwards())
    edgeEndVertex = es->getLastEdge()->getFrom();
  else
    edgeEndVertex = es->getLastEdge()->getTo();

  // if el1 is not part of this string, but shares a vertex with the first edge then we can
  // still calculate and return the distance.
  if (el->isExtreme() && el->getVertex() == edgeEndVertex)
    return beginningD;

  return numeric_limits<double>::max();
}

NetworkDetails::SublineCache NetworkDetails::_calculateSublineScore(ConstOsmMapPtr map,
                                                                    ConstWayPtr w1,
                                                                    ConstWayPtr w2) const
{
  LOG_TRACE("Calculating subline score...");

  Meters searchRadiusHighway = ConfigOptions().getSearchRadiusHighway();
  double searchRadius = searchRadiusHighway <= 0.0 ? getSearchRadius(w1, w2) : searchRadiusHighway;

  // Gonna round this up to the nearest whole. If this doesn't work out, consider adding some
  // rounding or slop or whatever to WayLocation::move(Meters distance) [which is, like, 7 layers
  // deeper in the onion].
  searchRadius = ceil(searchRadius);
  LOG_VART(searchRadiusHighway);
  LOG_VART(searchRadius);

  // Calculate the shared sublines.
  WaySublineMatchString sublineMatch = _sublineMatcher->findMatch(map, w1, w2, searchRadius);
  LOG_VART(sublineMatch);

  MatchClassification c;
  // Calculate the match score.
  if (sublineMatch.isValid())
    c = _classifier->classify(map, w1->getElementId(), w2->getElementId(), sublineMatch);

  return SublineCache(c.getMatchP(), std::make_shared<WaySublineMatchString>(sublineMatch));
}

QList<ConstNetworkVertexPtr> NetworkDetails::getCandidateMatches(ConstNetworkVertexPtr v)
{
  return _getVertexMatcher()->getCandidateMatches(v);
}

double NetworkDetails::_getEdgeAngleScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2,
                                          ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const
{
  double score = 1.0;

  LOG_VART(e1->getMembers().size());
  LOG_VART(e1->getMembers()[0]->getElementId());
  LOG_VART(e2->getMembers().size());
  LOG_VART(e2->getMembers()[0]->getElementId());
  // if this edge is a simple way type.
  if (e1->getMembers().size() == 1 && e1->getMembers()[0]->getElementType() == ElementType::Way &&
      e2->getMembers().size() == 1 && e2->getMembers()[0]->getElementType() == ElementType::Way)
  {
    Radians theta1 = calculateHeadingAtVertex(e1, v1);
    Radians theta2 = calculateHeadingAtVertex(e2, v2);

    Radians diff = WayHeading::deltaMagnitude(theta1, theta2);

    // if the diff in angle is > 90deg then set score to 0
    if (diff > M_PI / 2.0)
      score = 0.0;
    else
      score = cos(diff);
  }
  return score;
}

EdgeMatchPtr NetworkDetails::extendEdgeMatch(ConstEdgeMatchPtr em,
                                             ConstNetworkEdgePtr e1,
                                             ConstNetworkEdgePtr e2) const
{
  LOG_TRACE("Extending edge match...");

  EdgeMatchPtr result;

  // Run an experiment to see if a valid match is created by adding esm onto em.

  if (e1->isStub() || e2->isStub())
    return result;

  // snap e1 & e2 onto the end of
  EdgeStringPtr es1 = em->getString1()->clone();
  EdgeStringPtr es2 = em->getString2()->clone();

  extendEdgeString(es1, e1);
  extendEdgeString(es2, e2);

  LOG_VART(es1);
  LOG_VART(es2);

  // calculate the subline match with the new em
  // - convert the edge strings into ways
  OsmMapPtr map = std::make_shared<OsmMap>(_map->getProjection());

  WayPtr w1 = toWayString(es1)->copySimplifiedWayIntoMap(*_map, map);
  WayPtr w2 = toWayString(es2)->copySimplifiedWayIntoMap(*_map, map);
  LOG_VART(ElementToGeometryConverter(map).convertToLineString(w1)->toString());
  LOG_VART(ElementToGeometryConverter(map).convertToLineString(w2)->toString());
  // - calculate the matching subline of the two ways
  SublineCache sc = _calculateSublineScore(map, w1, w2);
  LOG_VART(sc.getP());
  // - use the length along the way to convert back to the edge string and crop as needed.
  if (sc.getP() == 0)
    return result;

  LOG_VART(sc.getMatches());
  const WaySublineMatchString::MatchCollection& mc = sc.getMatches()->getMatches();
  for (const auto& wsm : mc)
  {
    EdgeStringPtr tmp1 = es1->clone();
    EdgeStringPtr tmp2 = es2->clone();

    // trim es1 and es2 based on the way matches found
    _trimEdgeString(map, tmp1, w1, wsm.getSublineString1());
    _trimEdgeString(map, tmp2, w2, wsm.getSublineString2());
    tmp1->snapExtremes(EdgeLocation::SLOPPY_EPSILON);
    tmp2->snapExtremes(EdgeLocation::SLOPPY_EPSILON);

    LOG_VART(tmp1);
    LOG_VART(tmp2);

    // if the subline match intersects e1 and e2 then this is a successful modification
    if (tmp1->isValid() && tmp2->isValid() &&
        tmp1->contains(e1) && tmp2->contains(e2) &&
        tmp1->touches(em->getString1()) && tmp2->touches(em->getString2()))
    {
      result = std::make_shared<EdgeMatch>(tmp1, tmp2);
      LOG_TRACE("Successful edge match extension: " << result);
      return result;
    }
  }

  // returns a null ptr
  LOG_TRACE("Unsuccessful edge match extension for " << em->toString());
  return result;
}

void NetworkDetails::extendEdgeString(EdgeStringPtr es, ConstNetworkEdgePtr e) const
{
  LOG_TRACE("Extending edge string...");

  bool foundEnd = false;
  // if e is the same as the last edge in the string
  if (es->getLastEdge() == e ||
      (es->getAllEdges().back().isBackwards() && e->contains(es->getLastEdge()->getFrom())) ||
      (es->getAllEdges().back().isBackwards() == false && e->contains(es->getLastEdge()->getTo())))
  {
    // extend the last edge in the string all the way to the end.
    ConstEdgeSublinePtr sub = es->getAllEdges().back().getSubline();
    ConstEdgeLocationPtr elStart = sub->getStart();
    ConstEdgeLocationPtr elEnd = sub->getEnd();

    if (sub->isBackwards())
      elEnd = std::make_shared<EdgeLocation>(sub->getEdge(), 0);
    else
      elEnd = std::make_shared<EdgeLocation>(sub->getEdge(), 1);

    es->removeLast();
    es->appendEdge(std::make_shared<const EdgeSubline>(elStart, elEnd));
    foundEnd = true;
  }

  if (es->getFirstEdge() == e ||
      (es->getAllEdges().front().isBackwards() && e->contains(es->getFirstEdge()->getTo())) ||
      (es->getAllEdges().front().isBackwards() == false &&
       e->contains(es->getFirstEdge()->getFrom())))
  {
    // extend the first edge in the string all the way to the beginning.
    ConstEdgeSublinePtr sub = es->getAllEdges().front().getSubline();
    ConstEdgeLocationPtr elStart = sub->getStart();
    ConstEdgeLocationPtr elEnd = sub->getEnd();

    if (sub->isBackwards())
      elStart = std::make_shared<EdgeLocation>(sub->getEdge(), 1);
    else
      elStart = std::make_shared<EdgeLocation>(sub->getEdge(), 0);

    es->removeFirst();
    es->prependEdge(std::make_shared<const EdgeSubline>(elStart, elEnd));
    foundEnd = true;
  }

  if (!foundEnd)
  {
    LOG_VART(es->validate());
    LOG_VART(es);
    LOG_VART(e);
    throw IllegalArgumentException("Could not extend specified string with the specified edge.");
  }

  if (!es->contains(e))
  {
    if (es->getFrom()->isExtreme() && (es->getFromVertex() == e->getTo() || es->getFromVertex() == e->getFrom()))
      es->prependEdge(std::make_shared<EdgeSubline>(e, 0, 1));
    else if (es->getTo()->isExtreme() && (es->getToVertex() == e->getTo() || es->getToVertex() == e->getFrom()))
      es->appendEdge(e);
    else
      throw IllegalArgumentException("Could not extend specified string with the specified edge.");
  }
}

double NetworkDetails::getEdgeStringMatchScore(ConstEdgeStringPtr e1, ConstEdgeStringPtr e2) const
{
  double result;

  // if they're both stubs we don't need to match them
  if (e1->isStub() && e2->isStub())
  {
    LOG_TRACE("Both edge strings are stubs.");
    result = 0.0;
  }
  // if either string is a stub, then return 1 on candidate.
  else if (e1->isStub() || e2->isStub())
  {
    LOG_TRACE("One edge string is a stub.");

    ConstEdgeStringPtr stub = e1->isStub() ? e1 : e2;
    ConstEdgeStringPtr notStub = e1->isStub() ? e2 : e1;

    bool candidate = true;

    if (notStub->calculateLength(_map) > _maxStubLength)
      candidate = false;

    for (const auto& ee : notStub->getAllEdges())
      candidate = candidate && isCandidateMatch(ee.getEdge(), stub->getFirstEdge());

    result = candidate ? 1.0 : 0.0;
  }
  else  // if these are typical way edges
  {
    LOG_TRACE("Neither edge strings are stubs.");

    WayStringPtr ws1 = toWayString(e1);
    WayStringPtr ws2 = toWayString(e2);

    Meters sr = getSearchRadius(ws1, ws2);

    // if this is a partial match in the middle of an edge then we want to be more strict about
    // accepting the match.
    LOG_VART(e1->isFullPartial());
    LOG_VART(e2->isFullPartial());
    if (e1->isFullPartial() || e2->isFullPartial())
      sr *= 2;

    // we won't even try to make partial matches smaller than the search radius. It just creates too
    // much noise.
    LOG_VART(e1->isPartial());
    LOG_VART(e2->isPartial());
    if (ws1->calculateLength() < sr && ws2->calculateLength() < sr && (e1->isPartial() || e2->isPartial()))
      result = 0.0;
    else
    {
      RelationPtr r1 = std::make_shared<Relation>(Status::Unknown1, _map->createNextRelationId(), 15);
      r1->setType(MetadataTags::RelationMultilineString());
      RelationPtr r2 = std::make_shared<Relation>(Status::Unknown1, _map->createNextRelationId(), 15);
      r2->setType(MetadataTags::RelationMultilineString());

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
      OsmMapPtr mapCopy = std::make_shared<OsmMap>(_map->getProjection());
      CopyMapSubsetOp(_map, wids).apply(mapCopy);
      // We don't want to impact the global IDs with our little test.
      mapCopy->setIdGenerator(_map->getIdGenerator().clone());
      mapCopy->addElement(r1);
      mapCopy->addElement(r2);

      WayMatchStringMappingPtr mapping = std::make_shared<NaiveWayMatchStringMapping>(ws1, ws2);
      // Convert from a mapping to a WaySublineMatchString.
      WaySublineMatchStringPtr matchString = WayMatchStringMappingConverter().toWaySublineMatchString(mapping);

      MatchClassification c;
      // calculate the match score
      c = _classifier->classify(mapCopy, r1->getElementId(), r2->getElementId(), *matchString);

      result = c.getMatchP();
    }
  }

  return result;
}

Envelope NetworkDetails::getEnvelope(ConstNetworkEdgePtr e) const
{
  std::shared_ptr<Envelope> env(e->getMembers()[0]->getEnvelope(_map));
  for (int i = 1; i < e->getMembers().size(); ++i)
  {
    std::shared_ptr<Envelope> env2(e->getMembers()[i]->getEnvelope(_map));
    env->expandToInclude(env2.get());
  }
  return *env;
}

Envelope NetworkDetails::getEnvelope(ConstNetworkVertexPtr v) const
{
  std::shared_ptr<Envelope> env(v->getElement()->getEnvelope(_map));
  return *env;
}

double NetworkDetails::getPartialEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  LOG_VART(e1->getMembers().size());
  LOG_VART(e2->getMembers().size());
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);
  double result;

  if (e1->isStub() || e2->isStub())
  {
    if (isCandidateMatch(e1, e2))
      result = 0.5;
    else
      result = 0.0;
  }
  else
  {
    double bestScore = -1.0;
    if (isCandidateMatch(e1->getFrom(), e2->getFrom()))
      bestScore = max(bestScore, _getEdgeAngleScore(e1->getFrom(), e2->getFrom(), e1, e2));
    if (isCandidateMatch(e1->getTo(), e2->getTo()))
      bestScore = max(bestScore, _getEdgeAngleScore(e1->getTo(), e2->getTo(), e1, e2));
    if (isCandidateMatch(e1->getFrom(), e2->getTo()))
      bestScore = max(bestScore, _getEdgeAngleScore(e1->getFrom(), e2->getTo(), e1, e2));
    if (isCandidateMatch(e1->getTo(), e2->getFrom()))
      bestScore = max(bestScore, _getEdgeAngleScore(e1->getTo(), e2->getFrom(), e1, e2));

    // this is a partial match
    if (bestScore == -1.0)
    {
      LOG_TRACE("Scoring partial match...");
      bestScore = 1.0;
    }

    ConstWayPtr w1 = std::dynamic_pointer_cast<const Way>(e1->getMembers()[0]);
    ConstWayPtr w2 = std::dynamic_pointer_cast<const Way>(e2->getMembers()[0]);
    LOG_VART(w1.get());
    LOG_VART(w2.get());

    // Not sure why this is happening...
    if (!w1.get() || !w2.get())
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Unable to retrieve partial match score. One or more ways is null.");
        if (!w1.get())
        {
          LOG_DEBUG("Way 1 is null.");
        }
        if (!w2.get())
        {
          LOG_DEBUG("Way 2 is null.");
        }
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;

      return 0.0;
    }

    const SublineCache& sc = _getSublineCache(w1, w2);
    LOG_VART(sc.getP());
    LOG_VART(bestScore);
    result = sc.getP() * bestScore;
  }

  return result;
}

Meters NetworkDetails::getSearchRadius(ConstNetworkEdgePtr e) const
{
  double ce = -1;

  if (e->isStub())
  {
    for (const auto& element : _n2->getEdgesFromVertex(e->getFrom()))
    {
      if (element->isStub() == false)
        ce = std::max(ce, getSearchRadius(element));
    }
    if (ce == -1)
      ce = e->getFrom()->getElement()->getCircularError();
  }
  else
  {
    for (const auto& member : e->getMembers())
    {
      if (ce < member->getCircularError())
        ce = member->getCircularError();
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
  for (const auto& e : _n1->getEdgesFromVertex(v))
    ce = std::max(ce, getSearchRadius(e));

  for (const auto& e : _n2->getEdgesFromVertex(v))
    ce = std::max(ce, getSearchRadius(e));

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

NetworkDetails::SublineCache NetworkDetails::_getSublineCache(ConstWayPtr w1, ConstWayPtr w2)
{
  ElementId e1 = w1->getElementId();
  ElementId e2 = w2->getElementId();

  LOG_VART(e1);
  LOG_VART(e2);
  LOG_VART(_sublineCache[e1]);
  LOG_VART(_sublineCache[e1].contains(e2));

  if (_sublineCache[e1].contains(e2))
    return _sublineCache[e1][e2];
  // This is just here to keep runaway caches sizes from happening during testing. It can be
  // converted over to an actual cache with QQache if needed.
  if (_sublineCache.size() < ConfigOptions().getNetworkSublineMaxCacheSize())
  {
    _sublineCache[e1][e2] = _calculateSublineScore(_map, w1, w2);
    return _sublineCache[e1][e2];
  }
  else
    return _calculateSublineScore(_map, w1, w2);
}

LegacyVertexMatcherPtr NetworkDetails::_getVertexMatcher()
{
  if (!_vertexMatcher)
  {
    LOG_TRACE("Creating vertex matcher...");
    _vertexMatcher = std::make_shared<LegacyVertexMatcher>(_map);
    _vertexMatcher->identifyVertexMatches(_n1, _n2, *this);
  }
  return _vertexMatcher;
}

bool NetworkDetails::hasConfidentTiePoint(ConstNetworkVertexPtr v)
{
  return _getVertexMatcher()->hasConfidentTiePoint(v);
}

bool NetworkDetails::isCandidateMatch(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const
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

bool NetworkDetails::isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  assert(e1->getMembers().size() == 1);
  assert(e2->getMembers().size() == 1);
  bool result;

  if (e1->isStub() || e2->isStub())
    result = false;
  else
  {
    ConstWayPtr w1 = toWay(e1);
    ConstWayPtr w2 = toWay(e2);

    const SublineCache& sc = _getSublineCache(w1, w2);

    // this will only make sense if there is a single match between two edges.
    if (sc.getMatches()->getReverseVector1().size() != 1)
      throw NotImplementedException("This method should be avoided. Please change upstream logic.");

    result = sc.getMatches()->getMatches()[0].isReverseMatch();
  }

  return result;
}

bool NetworkDetails::isStringCandidate(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const
{
  bool result = false;

  if (e1->isStub() || e2->isStub())
    return result;

  LOG_VART(e1->getMembers().size());
  LOG_VART(e1->getMembers()[0]->getElementId());
  LOG_VART(e2->getMembers().size());
  LOG_VART(e2->getMembers()[0]->getElementId());

  if (e1->getMembers().size() == 1 &&
      e2->getMembers().size() == 1 &&
      e1->getMembers()[0]->getElementType() == ElementType::Way &&
      e2->getMembers()[0]->getElementType() == ElementType::Way)
  {
    if (e2->contains(e1->getFrom()))
    {
      Radians r1, r2;
      if (e1->getFrom() == e2->getFrom())
      {
        r1 = calculateHeadingAtVertex(e1, e1->getFrom());
        r2 = calculateHeadingAtVertex(e2, e2->getFrom());
      }
      else
      {
        r1 = calculateHeadingAtVertex(e1, e1->getFrom());
        r2 = calculateHeadingAtVertex(e2, e2->getTo());
      }
      result = WayHeading::deltaMagnitude(r1, r2) > toRadians(45);
      LOG_VART(toDegrees(WayHeading::deltaMagnitude(r1, r2)));
    }
    else if (e2->contains(e1->getTo()))
    {
      Radians r1, r2;
      if (e1->getTo() == e2->getTo())
      {
        r1 = calculateHeadingAtVertex(e1, e1->getTo());
        r2 = calculateHeadingAtVertex(e2, e2->getTo());
      }
      else
      {
        r1 = calculateHeadingAtVertex(e1, e1->getTo());
        r2 = calculateHeadingAtVertex(e2, e2->getFrom());
      }
      result = WayHeading::deltaMagnitude(r1, r2) > toRadians(45);
      LOG_VART(toDegrees(WayHeading::deltaMagnitude(r1, r2)));
    }
  }

  return result;
}

EdgeSublinePtr NetworkDetails::_toEdgeSubline(const WaySubline& ws, ConstNetworkEdgePtr e) const
{
  EdgeSublinePtr result;
  Meters l = ElementGeometryUtils::calculateLength(ws.getWay(), _map);
  result =
    std::make_shared<EdgeSubline>(
      e, ws.getStart().calculateDistanceOnWay() / l, ws.getEnd().calculateDistanceOnWay() / l);
  return result;
}

ConstWayPtr NetworkDetails::toWay(ConstNetworkEdgePtr e) const
{
  if (e->getMembers().size() != 1)
    throw IllegalArgumentException("Expected e to contain a single way.");

  ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e->getMembers()[0]);

  if (!w)
    throw IllegalArgumentException("Expected e to contain a single way.");

  return w;
}

WayStringPtr NetworkDetails::toWayString(ConstEdgeStringPtr e, const EidMapper& mapper) const
{
  const QList<EdgeString::EdgeEntry>& edges = e->getAllEdges();
  WayStringPtr ws = std::make_shared<WayString>();

  for (const auto& entry : edges)
  {
    ConstNetworkEdgePtr edge = entry.getEdge();
    const ConstEdgeSublinePtr& subline = entry.getSubline();
    // ignore stubs while building way strings.
    if (edge->isStub() == false)
    {
      if (edge->getMembers().size() != 1 || edge->getMembers()[0]->getElementType() != ElementType::Way)
      {
        LOG_VART(edge);
        throw IllegalArgumentException("Expected a network edge with exactly 1 way.");
      }
      ElementId eid = mapper.mapEid(edge->getMembers()[0]->getElementId());

      ConstWayPtr w = std::dynamic_pointer_cast<const Way>(_map->getWay(eid));
      if (!w)
      {
        LOG_VART(eid);
        throw IllegalArgumentException("Way: " + eid.toString() + " does not exist in the map.");
      }

      Meters l = calculateLength(edge);
      double startP = subline->getStart()->getPortion();
      double endP = subline->getEnd()->getPortion();

      WaySubline s(WayLocation(_map, w, startP * l), WayLocation(_map, w, endP * l));

      ws->append(s);
    }
  }

  LOG_VART(ws);
  return ws;
}

void NetworkDetails::_trimEdgeString(ConstElementProviderPtr /*provider*/, EdgeStringPtr es,
                                     WayPtr /*w*/, const WaySublineCollection& ws) const
{
  LOG_TRACE("Trimming edge string...");

  // sanity check to make sure the lengths are about the same.
  assert(ws.getSublines().size() == 1);

  WaySubline s = ws.getSublines()[0];
  Meters dStart = s.getStart().calculateDistanceOnWay();
  Meters dEnd = s.getEnd().calculateDistanceOnWay();

  es->trim(_map, dStart, dEnd);
}

}
