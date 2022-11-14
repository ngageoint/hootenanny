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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "ConflictsNetworkMatcher.h"

// hoot
#include <hoot/core/algorithms/FrechetDistance.h>
#include <hoot/core/conflate/network/EdgeMatch.h>
#include <hoot/core/conflate/network/EdgeMatchSetFinder.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(NetworkMatcher, ConflictsNetworkMatcher)

const double ConflictsNetworkMatcher::EPSILON = 1e-6;

ConflictsNetworkMatcher::ConflictsNetworkMatcher()
  : _edgeMatches(std::make_shared<IndexedEdgeMatchSet>()),
    _partialHandicap(ConfigOptions().getNetworkConflictsPartialHandicap()),
    _stubHandicap(ConfigOptions().getNetworkConflictsStubHandicap()),
    _aggression(ConfigOptions().getNetworkConflictsAggression()),
    _stubThroughWeighting(ConfigOptions().getNetworkConflictsStubThroughWeighting()),
    _weightInfluence(ConfigOptions().getNetworkConflictsWeightInfluence()),
    _outboundWeighting(ConfigOptions().getNetworkConflictsOutboundWeighting()),
    _sanityCheckMinSeparationDistance(ConfigOptions().getNetworkConflictsSanityCheckMinSeparationDistance()),
    _sanityCheckSeparationDistanceMultiplier(ConfigOptions().getNetworkConflictsSanityCheckSeparationDistanceMultiplier()),
    _conflictingScoreThresholdModifier(ConfigOptions().getNetworkConflictsConflictingScoreThresholdModifier()),
    _matchThreshold(ConfigOptions().getNetworkConflictsMatcherThreshold())
{
  if (_matchThreshold <= 0.0 || _matchThreshold > 1.0)
  {
    throw IllegalArgumentException(
      QString("Invalid conflicts match threshold: %1. Must be greater than 0.0 and less than 1.0.").arg(QString::number(_matchThreshold)));
  }
}

double ConflictsNetworkMatcher::_aggregateScores(QList<double> pairs) const
{
  qSort(pairs);

  // this quick little method makes the scores decrease with each matching pair
  // the idea being that you get big boost for the highest score then the benefit goes down
  // quickly. So more intersections matched is better, but having one poorly matched or unmatched
  // intersection won't hurt. We'll see what happens.
  double result = EPSILON;
  double denominator = 1;
  double multiplier = 1;
  for (auto pair : qAsConst(pairs))
  {
    result += pair * denominator;
    denominator *= multiplier;
  }

  return result;
}

QList<NetworkVertexScorePtr> ConflictsNetworkMatcher::getAllVertexScores() const
{
  return QList<NetworkVertexScorePtr>();
}

std::shared_ptr<ConflictsNetworkMatcher> ConflictsNetworkMatcher::create()
{
  return std::make_shared<ConflictsNetworkMatcher>();
}

void ConflictsNetworkMatcher::_createEmptyStubEdges(OsmNetworkPtr na, OsmNetworkPtr nb) const
{
  LOG_TRACE("Creating stub edges...");

  if (na == _n1)
  {
    LOG_TRACE("na is n1");
  }
  else
  {
    LOG_TRACE("na is n2");
  }
  const OsmNetwork::VertexMap& vm = na->getVertexMap();

  // go through all the vertices in the network
  for (const auto& va : vm.values())
  {
    QList<ConstNetworkVertexPtr> vCandidatesB = _details->getCandidateMatches(va);
    bool createStub = false;

    for (const auto& vb : qAsConst(vCandidatesB))
    {
      QList<ConstNetworkEdgePtr> edges = nb->getEdgesFromVertex(vb);

      // go through all the edges that start/end at vb
      for (const auto& e : qAsConst(edges))
      {
        // if both the from and to vertices are candidate matches with va
        if (e->getFrom() != e->getTo() && vCandidatesB.contains(e->getFrom()) &&
            vCandidatesB.contains(e->getTo()))
        {
          createStub = true;
          break;
        }
      }
    }

    if (createStub)
    {
      // Create stub
      NetworkEdgePtr newStub = std::make_shared<NetworkEdge>(va, va, false);
      newStub->addMember(va->getElement());
      LOG_TRACE("Adding new edge: " << newStub);
      na->addEdge(newStub);
    }
  }
}

Meters ConflictsNetworkMatcher::_getMatchSeparation(ConstEdgeMatchPtr pMatch) const
{
  // convert the EdgeStrings into WaySublineStrings
  WayStringPtr str1 = _details->toWayString(pMatch->getString1());
  WayStringPtr str2 = _details->toWayString(pMatch->getString2());

  if (str1->getSize() > 0 && str2->getSize() > 0)
  {
    //  Create a temp map, and add the ways
    OsmMapPtr tempMap = std::make_shared<OsmMap>();
    tempMap->setProjection(_details->getMap()->getProjection());
    WayPtr pWay1 = str1->copySimplifiedWayIntoMap(*(_details->getMap()), tempMap);
    WayPtr pWay2 = str2->copySimplifiedWayIntoMap(*(_details->getMap()), tempMap);

    // Make our frechet object
    FrechetDistance distanceCalc(tempMap, pWay1, pWay2);
    Meters d = distanceCalc.distance();

    LOG_TRACE("Match (" << pMatch->getUid() << ") separation: " << d);

    return d;
  }
  else
    return 0.0;
}

void ConflictsNetworkMatcher::_sanityCheckRelationships()
{
  int ctr = 0;
  QList<ConstEdgeMatchPtr> keys = _scores.keys();
  const int total = keys.size();
  int matchesRemoved = 0;
  for (const auto& em : qAsConst(keys))
  {
    const double myDistance = _getMatchSeparation(em);
    for (const auto& r : qAsConst(_matchRelationships[em]))
    {
      // If it's a conflict, AND we are a lot closer, axe the other one
      if (r->isConflict())
      {
        const double theirDistance = _getMatchSeparation(r->getEdge());
        if (myDistance > _sanityCheckMinSeparationDistance &&
            ((myDistance * _sanityCheckSeparationDistanceMultiplier) < theirDistance))
        {
          LOG_TRACE(
            "Removing insane match: " << r->getEdge()->getUid() << " - " << theirDistance <<
            " keeping: " << em->getUid() << " - " << myDistance);

          // Remove match
          _edgeMatches->getAllMatches().remove(r->getEdge());

          // Remove from scores
          _scores.remove(r->getEdge());

          // Remove from relationships
          _matchRelationships.remove(r->getEdge());

          matchesRemoved++;
        }
      }
    }

    ctr++;
    if (ctr % 100 == 0)
    {
      PROGRESS_INFO(
        "Sanity checked " << StringUtils::formatLargeNumber(ctr) << " of " <<
        StringUtils::formatLargeNumber(total) << " relationships. " <<
        StringUtils::formatLargeNumber(matchesRemoved) << " matches removed.");
    }
  }
}

void ConflictsNetworkMatcher::_createMatchRelationships()
{
  int count = 0;
  for (const auto& em : _edgeMatches->getAllMatches().keys())
  {
    // if the two edges
    //  - overlap
    //  - contain non-extreme vertices that overlap with any vertex in the other edge (see white
    //    board example)
    // They're conflicts!
    QSet<ConstEdgeMatchPtr> conflict = _edgeMatches->getMatchesThatOverlap(em);
    LOG_TRACE("conflicting overlapping matches: " << conflict.size());
    QSet<ConstEdgeMatchPtr> touches;
    ConstNetworkVertexPtr from1, to1;
    ConstNetworkVertexPtr from2, to2;
    if (em->getString1()->getFrom()->isExtreme())
    {
      from1 = em->getString1()->getFromVertex();
      touches.unite(_edgeMatches->getMatchesThatTerminateAt(from1));
      LOG_VART(conflict.size());
    }
    if (em->getString1()->getTo()->isExtreme())
    {
      to1 = em->getString1()->getToVertex();
      touches.unite(_edgeMatches->getMatchesThatTerminateAt(to1));
      LOG_VART(conflict.size());
    }
    if (em->getString2()->getFrom()->isExtreme())
    {
      from2 = em->getString2()->getFromVertex();
      touches.unite(_edgeMatches->getMatchesThatTerminateAt(from2));
      LOG_VART(conflict.size());
    }
    if (em->getString2()->getTo()->isExtreme())
    {
      to2 = em->getString2()->getToVertex();
      touches.unite(_edgeMatches->getMatchesThatTerminateAt(to2));
      LOG_VART(conflict.size());
    }
    conflict -= em;
    LOG_VART(conflict.size());

    // if the two edge matches end in the same vertices and there is no overlap these are
    // supporters!
    QSet<ConstEdgeMatchPtr> support;
    if (from1 && from2)
    {
      support += _getMatchesWithSharedTermination(from1, from2);
      LOG_VART(support.size());
    }
    if (to1 && to2)
    {
      support += _getMatchesWithSharedTermination(to1, to2);
      LOG_VART(support.size());
    }
    // remove any conflicts from the support list
    support -= conflict;
    LOG_VART(support.size());
    // remove this match from support.
    support -= em;
    LOG_VART(support.size());

    // any edge that touches, but isn't supporting its a conflict.
    touches -= em;
    touches -= support;

    // TODO: Removing the non-supporting, touching edges from the conflicts was done to make a case
    // test pass at the expense of no other case tests failing and no decrease in regression
    // performance. However, this seems like an important piece of logic, so we need to keep in mind
    // that this change may have to be reverted if we encounter a situation where having it disabled
    // causes problems. - #3052
    //conflict += touches;

    LOG_VART(conflict.size());
    LOG_VART(em);
    LOG_VART(conflict);
    LOG_VART(touches);
    LOG_VART(support);

    // Conflicts
    for (const auto& other : qAsConst(conflict))
      _matchRelationships[em].append(std::make_shared<const MatchRelationship>(other, true));

    for (const auto& other : qAsConst(support))
    {
      MatchRelationshipPtr mr = std::make_shared<MatchRelationship>(other, false);
      QSet<ConstEdgeMatchPtr> connectingStubs = _edgeMatches->getConnectingStubs(em, other);
      QSet<ConstEdgeMatchPtr> nonConflictingStubs = connectingStubs - conflict;
      if (connectingStubs.empty() || nonConflictingStubs.size() >= 1)
      {
        mr->setThroughStubs(_edgeMatches->getConnectingStubs(em, other));
        // if this edge supports through a stub, then record the stub that is used to support it.
        _matchRelationships[em].append(mr);
      }
    }

    if (Log::getInstance().getLevel() <= Log::Trace)
    {
      for (const auto& aConflict : qAsConst(conflict)) LOG_VART(aConflict);
      for (const auto& aSupport : qAsConst(support)) LOG_VART(aSupport);
    }

    _scores[em] = 1.0;
    _weights[em] = 1.0;

    count++;
    if (count % 1000 == 0)
    {
      PROGRESS_INFO(
        StringUtils::formatLargeNumber(count) << " of " <<
        StringUtils::formatLargeNumber(_edgeMatches->getAllMatches().size()) <<
        " match relationships processed.");
    }
  }

  LOG_VART(_scores);
  LOG_VART(_weights);
}

QList<NetworkEdgeScorePtr> ConflictsNetworkMatcher::getAllEdgeScores() const
{
  QList<NetworkEdgeScorePtr> result;
  for (const auto& em : _scores.keys())
    result.append(std::make_shared<NetworkEdgeScore>(em, _scores[em], _scores[em]));

  return result;
}

QList<ConstNetworkEdgePtr> ConflictsNetworkMatcher::_getEdgesOnVertex(ConstNetworkVertexPtr v) const
{
  QList<ConstNetworkEdgePtr> r1 = _n1->getEdgesFromVertex(v);
  QList<ConstNetworkEdgePtr> r2 = _n2->getEdgesFromVertex(v);

  QList<ConstNetworkEdgePtr> result = r1;
  result.append(r2);

  return result;
}

QSet<ConstEdgeMatchPtr> ConflictsNetworkMatcher::_getMatchesWithSharedTermination(ConstNetworkVertexPtr v1,
                                                                                  ConstNetworkVertexPtr v2) const
{
  // This function is the result of stubs. In some cases two edges may not directly share a
  // termination, but they share a termination through a stub. First we find the obvious shared
  // terminations, then find the implied shared terminations.
  QSet<ConstEdgeMatchPtr> m = _edgeMatches->getMatchesWithTermination(v1, v2);
  QSet<ConstEdgeMatchPtr> result = m;

  for (const auto& e : qAsConst(m))
  {
    if (e->containsStub())
    {
      if (e->getString1()->isStub())
      {
        result += _edgeMatches->getMatchesWithTermination(v1, e->getString2()->getFromVertex());
        result += _edgeMatches->getMatchesWithTermination(v1, e->getString2()->getToVertex());
      }
      else
      {
        result += _edgeMatches->getMatchesWithTermination(e->getString1()->getFromVertex(), v2);
        result += _edgeMatches->getMatchesWithTermination(e->getString1()->getToVertex(), v2);
      }
    }
  }

  // any matches that contain v1/v2 as interior vertices isn't a candidate.
  result -= _edgeMatches->getMatchesWithInteriorVertex(v1);
  result -= _edgeMatches->getMatchesWithInteriorVertex(v2);

  if (m.size() != result.size())
  {
    LOG_VART(v1);
    LOG_VART(v2);
    for (const auto& e : result - m)
    {
      LOG_VART(e);
    }
  }

  return result;
}

void ConflictsNetworkMatcher::iterate()
{
  _iterateSimple();
}

void ConflictsNetworkMatcher::_iterateRank()
{
  const double partialHandicap = 0.5;
  EdgeScoreMap newScores;
  LOG_VART(_scores.size());
  for (const auto& em : _scores.keys())
  {
    LOG_VART(em->containsPartial());
    LOG_VART(em->containsStub());

    double numerator = em->containsPartial() || em->containsStub() ? _scores[em] * partialHandicap : _scores[em];
    double denominator = numerator;

    for (const auto& r : qAsConst(_matchRelationships[em]))
    {
      LOG_VART(r->getEdge()->containsPartial());

      double handicap =
        r->getEdge()->containsPartial() || em->containsStub() ? partialHandicap : 1.0;
      double s = _scores[r->getEdge()] * handicap;
      //s = pow(s, 2);
      LOG_VART(s);

      int supportCount = 0;
      int relationCount = 0;
      for (const auto& sr : qAsConst(_matchRelationships[r->getEdge()]))
      {
        if (sr->isConflict() == false)
          supportCount++;

        relationCount++;
      }
      supportCount = max(1, supportCount);
      relationCount = max(1, relationCount);

      LOG_VART(supportCount);
      s = s / (double)relationCount;

      if (r->isConflict() == false)
        numerator += s;

      denominator += s;
    }

    newScores[em] = pow(numerator / denominator, 4);
    LOG_TRACE(em << " " << numerator << "/" << denominator << " " << newScores[em]);
  }

  _scores = newScores;
}

void ConflictsNetworkMatcher::_iterateSimple()
{
  const double partialHandicap = _partialHandicap;
  const double stubHandicap = _stubHandicap;
  double aggression = _aggression;

  // we'll create a new copy of the scores in this function and assign to the authoritative copy at
  // the end.
  EdgeScoreMap newScores, newWeights;
  double weightSum = EPSILON;
  int count = 0;

  // go through all matches
  const QList<ConstEdgeMatchPtr>& scores = _scores.keys();
  const int total = scores.size();
  for (const auto& em : qAsConst(scores))
  {
    double handicap = pow(partialHandicap, em->countPartialMatches());
    LOG_VART(em);
    LOG_VART(_scores[em]);
    LOG_VART(handicap);
    LOG_VART(em->countPartialMatches());
    if (em->containsStub())
    {
      handicap = stubHandicap;
      LOG_VART(handicap);
    }

    double numerator = _scores[em] * handicap;
    double denominator = numerator;
    LOG_VART(numerator);

    for (const auto& r : qAsConst(_matchRelationships[em]))
    {
      double childHandicap = pow(partialHandicap, r->getEdge()->countPartialMatches());
      LOG_VART(r->getEdge());
      LOG_VART(r->getEdge()->countPartialMatches());
      LOG_VART(childHandicap);
      if (r->getEdge()->containsStub())
        childHandicap = stubHandicap;

      LOG_VART(r->getEdge()->getString1()->contains(em->getString1()));
      LOG_VART(r->getEdge()->getString2()->contains(em->getString2()));
      // if r contains at least one line in em and em doesn't contain an edge string in r
      // (overlapping, but not completely contained)
      if ((r->getEdge()->getString1()->contains(em->getString1()) ||
           r->getEdge()->getString2()->contains(em->getString2())) &&
          !(em->getString1()->contains(r->getEdge()->getString1()) ||
            em->getString2()->contains(r->getEdge()->getString2())))
      {
        childHandicap *= 1.5;
        LOG_VART(childHandicap);
      }

      // s is modified throughout the function to represent how important this edge match is to
      // em.
      double s = _scores[r->getEdge()] * childHandicap;
      LOG_VART(s);

      // in some cases a stub can implicitly connect two matches. If this occurs we shouldn't add
      // or subtract unless that stub is relatively high confidence. So, impact our score
      // accordingly.
      double stubWeight = -1;
      QSet<ConstEdgeMatchPtr> throughStubs = r->getThroughStubs();
      for (const auto& stub : qAsConst(throughStubs))
        stubWeight = max(stubWeight, _scores[stub]);

      // based on testing through stubs connections shouldn't really count for or against matches.
      if (stubWeight != -1)
      {
        LOG_VART(stubWeight);
        s *= pow(stubWeight, _stubThroughWeighting);
        LOG_VART(s);
      }

      // how many edges match relationships support this match
      int supportCount = 0;
      // Number of match relationships
      int relationCount = 0;
      for (const auto& sr : qAsConst(_matchRelationships[r->getEdge()]))
      {
        if (sr->isConflict() == false)
          supportCount++;

        relationCount++;
      }
      supportCount = max(1, supportCount);
      relationCount = max(1, relationCount);

      LOG_VART(supportCount);

      s = s * _weights[r->getEdge()];
      LOG_VART(s);

      // if network.conflicts.outbound.weighting is non-zero, use the number of neighboring matches
      // to weight the influence of this edge match.
      s = s * pow(1.0 / (double)relationCount, _outboundWeighting);
      LOG_VART(s);

      if (r->isConflict() == false)
      {
        LOG_TRACE("support:  " << s << "\t" << r->getEdge()->toString().left(80));
        numerator += s;
        LOG_VART(numerator);
      }
      else
      {
        LOG_TRACE("conflict: " << s << "\t" << r->getEdge()->toString().left(80));
      }

      denominator += s;
      LOG_VART(denominator);
    }

    // If the denominator trends to close to 0, we pollute the system with NaNs
    if (denominator > EPSILON)
      newScores[em] = pow(numerator / denominator, aggression);
    else
      newScores[em] = 0.0;

    newWeights[em] = denominator;
    weightSum += denominator;
    LOG_VART(weightSum);

    LOG_TRACE("\ns1: " << em->getString1() << "\ns2: " << em->getString2() << "\n"
             << numerator << "/" << denominator << " " << newScores[em]
             << " " << newWeights[em]);
    LOG_TRACE(em << " " << numerator << "/" << denominator << " " << newScores[em] << " " << newWeights[em]);

    count++;
    if (count % 1000 == 0)
    {
      PROGRESS_INFO(
        StringUtils::formatLargeNumber(count) << " of " << StringUtils::formatLargeNumber(total) <<
        " matches processed.");
    }
  }

  // Setting this really helps reduce scoring oscillation
  for (const auto& em : newWeights.keys())
  {
    newWeights[em] = pow(newWeights[em] * newWeights.size() / weightSum, _weightInfluence);
    LOG_VART(newWeights[em]);
  }

  _scores = newScores;
  _weights = newWeights;
}

void ConflictsNetworkMatcher::matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2)
{
  _n1 = n1;
  _n2 = n2;
  _details = std::make_shared<NetworkDetails>(map, n1, n2);

  // Add stub edges to both networks.
  // if both vertices on an edge match a single vertex (v2)
    // create a new zero length edge that connects v2 to itself
  _createEmptyStubEdges(_n1, _n2);
  _createEmptyStubEdges(_n2, _n1);

  // create empty stub edges can change the map, recreate the details so the indexes are
  // reinitialized.
  _details = std::make_shared<NetworkDetails>(map, n1, n2);

  // create a spatial index of n2 vertices & edges.
  _createEdge2Index();

  // create an initial estimation of edge match based on typical similarity scores
  _seedEdgeScores();

  _createMatchRelationships();

  _sanityCheckRelationships();
}

void ConflictsNetworkMatcher::finalize()
{
  // Check our relationships
  int count = 0;
  int total = _scores.size();
  for (const auto& em : _scores.keys())
  {
    for (const auto& r : qAsConst(_matchRelationships[em]))
    {
      const double myScore = _scores[em];
      const double theirScore = _scores[r->getEdge()];
      // If it's a conflict, AND we score a lot better, axe the other one
      if (r->isConflict() && myScore > (_conflictingScoreThresholdModifier + theirScore))
        _scores[r->getEdge()] = 1.0e-5;
    }
    count++;
    if (count % 100 == 0)
    {
      PROGRESS_INFO(
        StringUtils::formatLargeNumber(count) << " of " << StringUtils::formatLargeNumber(total) <<
        " edge matches finalized.");
    }
  }
}

void ConflictsNetworkMatcher::_seedEdgeScores()
{  
  EdgeMatchSetFinder finder(_details, _edgeMatches, _n1, _n2);

  // our stubs don't need to be bidirectional since they don't create new nodes.
  finder.setAddStubsInBothDirections(false);
  finder.setIncludePartialMatches(true);

  int count = 0;
  // go through all the n1 edges
  const OsmNetwork::EdgeMap& em = _n1->getEdgeMap();
  int totalNumIntersections = 0;
  for (auto it = em.begin(); it != em.end(); ++it)
  {
    ConstNetworkEdgePtr e1 = it.value();

    // find all the n2 edges that are in range of this one
    Envelope env = _details->getEnvelope(it.value());
    env.expandBy(_details->getSearchRadius(it.value()));
    LOG_TRACE("Search Radius: " << _details->getSearchRadius(it.value()));

    IntersectionIterator iit = _createIterator(env, _edge2Index);
    int numIntersections = 0;
    while (iit.next())
    {
      ConstNetworkEdgePtr e2 = _index2Edge[iit.getId()];
      LOG_VART(e1);
      LOG_VART(e2);

      const double score = _details->getPartialEdgeMatchScore(e1, e2);
      LOG_TRACE("partial edge match score:" << score);
      if (score > 0.0)
      {
        // Add all the EdgeMatches that are seeded with this edge pair.
        finder.addEdgeMatches(e1, e2);
      }

      numIntersections++;
      totalNumIntersections++;
    }

    count++;
    if (count % 10 == 0)
    {
      // Strangely, PROGRESS_INFO doesn't log one per line once StringUtils::formatLargeNumber gets
      // added here...why is that?; update 10/11/19: even with StringUtils removed here, I'm still
      // seeing multiple lines logged...why?
      PROGRESS_INFO(
        StringUtils::formatLargeNumber(count) << " of " <<
        StringUtils::formatLargeNumber(em.size()) << " edge match scores processed for " <<
        StringUtils::formatLargeNumber(totalNumIntersections) << " total intersections. " <<
        StringUtils::formatLargeNumber(finder.getNumSimilarEdgeMatches()) <<
        " duplicate edge matches removed.");
    }
  }

  _printEdgeMatches();
}

void ConflictsNetworkMatcher::_printEdgeMatches() const
{
  if (Log::getInstance().getLevel() <= Log::Trace)
  {
    stringstream ss;
    for (const auto& em : _edgeMatches->getAllMatches().keys())
    {
      for (const auto& edge : em->getString1()->getAllEdges())
      {
        for (const auto& elmnt : edge.getSubline()->getStart()->getEdge()->getMembers())
        {
          if (elmnt->getElementType() == ElementType::Way)
            ss << "(way:" << elmnt->getId() << ")";
        }
      }
      ss << " <<matches>> ";
      for (const auto& edge : em->getString2()->getAllEdges())
      {
        for (const auto& elmnt : edge.getSubline()->getStart()->getEdge()->getMembers())
        {
          if (elmnt->getElementType() == ElementType::Way)
            ss << "(way:" << elmnt->getId() << ")";
        }
      }
      ss << std::endl;
    }
    LOG_TRACE(ss.str());
  }
}

}
