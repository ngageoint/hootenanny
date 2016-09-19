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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ConflictsNetworkMatcher.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/rnd/conflate/network/EdgeMatch.h>

#include "EdgeMatchSetFinder.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(NetworkMatcher, ConflictsNetworkMatcher)

const double ConflictsNetworkMatcher::EPSILON = 1e-6;

ConflictsNetworkMatcher::ConflictsNetworkMatcher()
{
  _edgeMatches.reset(new IndexedEdgeMatchSet());

  ConfigOptions conf;
  _partialHandicap = conf.getNetworkConflictsPartialHandicap();
  _stubHandicap = conf.getNetworkConflictsStubHandicap();
  _aggression = conf.getNetworkConflictsAggression();
  _weightInfluence = conf.getNetworkConflictsWeightInfluence();
  _outboundWeighting = conf.getNetworkConflictsOutboundWeighting();
  _stubThroughWeighting = conf.getNetworkConflictsStubThroughWeighting();
}

double ConflictsNetworkMatcher::_aggregateScores(QList<double> pairs)
{
  //qSort(pairs.begin(), pairs.end(), greaterThan);
  qSort(pairs);

  // this quick little method makes the scores decrease with each matching pair
  // the idea being that you get big boost for the highest score then the benefit goes down
  // quickly. So more intersections matched is better, but having one poorly matched or unmatched
  // intersection won't hurt. We'll see what happens.
  double result = EPSILON;
  double denominator = 1;
  double multiplier = 1;
  for (int i = 0; i < pairs.size(); i++)
  {
    result += pairs[i] * denominator;
    denominator *= multiplier;
  }

  return result;
}

shared_ptr<ConflictsNetworkMatcher> ConflictsNetworkMatcher::create()
{
  return shared_ptr<ConflictsNetworkMatcher>(new ConflictsNetworkMatcher());
}

void ConflictsNetworkMatcher::_createEmptyStubEdges(OsmNetworkPtr na, OsmNetworkPtr nb)
{
  if (na == _n1)
  {
    LOG_INFO("na is n1");
  }
  else
  {
    LOG_INFO("na is n2");
  }
  const OsmNetwork::VertexMap& vm = na->getVertexMap();

  // go through all the vertices in the network
  foreach (ConstNetworkVertexPtr va, vm.values())
  {
    QList<ConstNetworkVertexPtr> vCandidatesB = _details->getCandidateMatches(va);
    bool createStub = false;

    foreach (ConstNetworkVertexPtr vb, vCandidatesB)
    {
      QList<ConstNetworkEdgePtr> edges = nb->getEdgesFromVertex(vb);

      // go through all the edges that start/end at vb
      foreach (ConstNetworkEdgePtr e, edges)
      {
        // if both the from and to vertices are candidate matches with va
        if (e->getFrom() != e->getTo() && vCandidatesB.contains(e->getFrom()) &&
          vCandidatesB.contains(e->getTo()))
        {
          createStub = true;
        }
      }
    }

    if (createStub)
    {
      // Create stub
      NetworkEdgePtr newStub(new NetworkEdge(va, va, false));
      newStub->addMember(va->getElement());
      LOG_INFO("Adding new edge: " << newStub);
      na->addEdge(newStub);
    }
  }
}

void ConflictsNetworkMatcher::_createMatchRelationships()
{
  int count = 0;
  foreach (ConstEdgeMatchPtr em, _edgeMatches->getAllMatches().keys())
  {
    PROGRESS_INFO(count++ << " / " << _edgeMatches->getAllMatches().size());
    // if the two edges
    //  - overlap
    //  - contain non-extreme vertices that overlap with any vertex in the other edge (see white
    //    board example)
    // They're conflicts!
    QSet<ConstEdgeMatchPtr> conflict = _edgeMatches->getMatchesThatOverlap(em);
    QSet<ConstEdgeMatchPtr> touches;
    ConstNetworkVertexPtr from1, to1;
    ConstNetworkVertexPtr from2, to2;
    if (em->getString1()->getFrom()->isExtreme())
    {
      from1 = em->getString1()->getFromVertex();
      touches.unite(_edgeMatches->getMatchesThatTerminateAt(from1));
      conflict += _edgeMatches->getMatchesWithInteriorVertex(from1);
    }
    if (em->getString1()->getTo()->isExtreme())
    {
      to1 = em->getString1()->getToVertex();
      touches.unite(_edgeMatches->getMatchesThatTerminateAt(to1));
      conflict += _edgeMatches->getMatchesWithInteriorVertex(to1);
    }
    if (em->getString2()->getFrom()->isExtreme())
    {
      from2 = em->getString2()->getFromVertex();
      touches.unite(_edgeMatches->getMatchesThatTerminateAt(from2));
      conflict += _edgeMatches->getMatchesWithInteriorVertex(from2);
    }
    if (em->getString2()->getTo()->isExtreme())
    {
      to2 = em->getString2()->getToVertex();
      touches.unite(_edgeMatches->getMatchesThatTerminateAt(to2));
      conflict += _edgeMatches->getMatchesWithInteriorVertex(to2);
    }
    conflict -= em;

    // if the two edge matches end in the same vertices and there is no overlap these are
    // supporters!
    QSet<ConstEdgeMatchPtr> support;
    if (from1 && from2)
    {
      support += _getMatchesWithSharedTermination(from1, from2);
    }
    if (to1 && to2)
    {
      support += _getMatchesWithSharedTermination(to1, to2);
    }
    // remove any conflicts from the support list
    support -= conflict;
    // remove this match from support.
    support -= em;

    // any edge that touches, but isn't supporting is a conflict.
    touches -= em;
    touches -= support;
    conflict += touches;
    LOG_VART(em);
    LOG_VART(conflict);
    LOG_VART(touches);
    LOG_VART(support);

    foreach (ConstEdgeMatchPtr other, conflict)
    {
      _matchRelationships[em].append(ConstMatchRelationshipPtr(new MatchRelationship(other, true)));
    }
    foreach (ConstEdgeMatchPtr other, support)
    {
      MatchRelationshipPtr mr(new MatchRelationship(other, false));
      QSet<ConstEdgeMatchPtr> connectingStubs = _edgeMatches->getConnectingStubs(em, other);
      QSet<ConstEdgeMatchPtr> nonConflictingStubs = connectingStubs - conflict;
      if (connectingStubs.size() == 0 || nonConflictingStubs.size() >= 1)
      {
        mr->setThroughStubs(_edgeMatches->getConnectingStubs(em, other));
        // if this edge supports through a stub, then record the stub that is used to support it.
        _matchRelationships[em].append(mr);
      }
    }

//    foreach (ConstEdgeMatchPtr aSupport, support) LOG_VARD(aSupport);
//    foreach (ConstEdgeMatchPtr aConflict, conflict) LOG_VARD(aConflict);

    _scores[em] = 1.0;
    _weights[em] = 1.0;
  }
}

QList<NetworkEdgeScorePtr> ConflictsNetworkMatcher::getAllEdgeScores() const
{
  QList<NetworkEdgeScorePtr> result;

  foreach (ConstEdgeMatchPtr em, _scores.keys())
  {
    NetworkEdgeScorePtr p(new NetworkEdgeScore(em, _scores[em], _scores[em]));
    result.append(p);
  }

  return result;
}

QList<NetworkVertexScorePtr> ConflictsNetworkMatcher::getAllVertexScores() const
{
  QList<NetworkVertexScorePtr> result;

  return result;
}

QList<ConstNetworkEdgePtr> ConflictsNetworkMatcher::_getEdgesOnVertex(ConstNetworkVertexPtr v)
{
  QList<ConstNetworkEdgePtr> r1 = _n1->getEdgesFromVertex(v);
  QList<ConstNetworkEdgePtr> r2 = _n2->getEdgesFromVertex(v);

  QList<ConstNetworkEdgePtr> result = r1;
  result.append(r2);

  return result;
}

QSet<ConstEdgeMatchPtr> ConflictsNetworkMatcher::_getMatchesWithSharedTermination(
  ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const
{
  // This function is the result of stubs. In some cases two edges may not directly share a
  // termination, but they share a termination through a stub. First we find the obvious shared
  // terminations, then find the implied shared terminations.
  QSet<ConstEdgeMatchPtr> m = _edgeMatches->getMatchesWithTermination(v1, v2);
  QSet<ConstEdgeMatchPtr> result = m;

  foreach (ConstEdgeMatchPtr e, m)
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
    LOG_VAR(v1);
    LOG_VAR(v2);
    foreach (ConstEdgeMatchPtr e, result - m)
    {
      LOG_VAR(e);
    }
  }

  return result;
}

void ConflictsNetworkMatcher::iterate()
{
  //_iterateRank();
  _iterateSimple();
}

void ConflictsNetworkMatcher::_iterateRank()
{
  const double partialHandicap = 0.5;
  EdgeScoreMap newScores;
  LOG_VARW(_scores.size());
  foreach(ConstEdgeMatchPtr em, _scores.keys())
  {
    double numerator = em->containsPartial() || em->containsStub() ? _scores[em] * partialHandicap : _scores[em];
    double denominator = numerator;

    foreach(ConstMatchRelationshipPtr r, _matchRelationships[em])
    {
      double handicap = r->getEdge()->containsPartial() || em->containsStub() ? partialHandicap : 1.0;
      double s = _scores[r->getEdge()] * handicap;
      //s = pow(s, 2);
      LOG_VAR(s);

      int supportCount = 0;
      int relationCount = 0;
      foreach (ConstMatchRelationshipPtr sr, _matchRelationships[r->getEdge()])
      {
        if (sr->isConflict() == false)
        {
          supportCount++;
        }
        relationCount++;
      }
      supportCount = max(1, supportCount);
      relationCount = max(1, relationCount);

      //s = s / (double)supportCount;
      s = s / (double)relationCount;

      if (r->isConflict() == false)
      {
        numerator += s;
      }
      denominator += s;
    }

    newScores[em] = pow(numerator / denominator, 4);
    LOG_INFO(em << " " << numerator << "/" << denominator << " " << newScores[em]);
  }

  _scores = newScores;
}

void ConflictsNetworkMatcher::_iterateSimple()
{
  const double partialHandicap = _partialHandicap;
  const double stubHandicap = _stubHandicap;
  double aggression = _aggression;
  EdgeScoreMap newScores, newWeights;
  double weightSum = EPSILON;
  int count = 0;
  foreach(ConstEdgeMatchPtr em, _scores.keys())
  {
    PROGRESS_INFO(++count << "/" << _scores.size());

    double handicap = em->containsPartial() ? partialHandicap : 1.0;
    if (em->containsStub())
    {
      handicap = stubHandicap;
    }
//    if (em->containsPartial())
//    {
//      aggression += 8;
//    }
    double numerator = _scores[em] * handicap;
    double denominator = numerator;
    LOG_VAR(numerator);

    foreach(ConstMatchRelationshipPtr r, _matchRelationships[em])
    {
      double childHandicap = r->getEdge()->containsPartial() ? partialHandicap : 1.0;
      if (r->getEdge()->containsStub())
      {
        childHandicap = stubHandicap;
      }

//      if (r->getEdge()->containsStub() && r->isConflict())
//      {
//        aggression += 3.0;
//      }

//      if (r->getEdge()->containsStub() && em->containsStub() && r->isConflict())
//      {
//        handicap = 2.0;
//      }

      double s = _scores[r->getEdge()] * childHandicap;

      // in some cases a stub can implicitly connect two matches. If this occurs we shouldn't add
      // or subtract unless that stub is relatively high confidence. So, impact our score
      // accordingly.
      double stubWeight = -1;
      QSet<ConstEdgeMatchPtr> throughStubs = r->getThroughStubs();
      foreach (ConstEdgeMatchPtr stub, throughStubs)
      {
        stubWeight = max(stubWeight, _scores[stub]);
      }

      // based on testing through stubs connections shouldn't really count for or against matches.
      if (stubWeight != -1)
      {
        LOG_VAR(stubWeight);
        //s *= stubWeight / 10.0;
        s *= pow(stubWeight, _stubThroughWeighting);
        //s = 0.0;
      }

      int supportCount = 0;
      int relationCount = 0;
      foreach (ConstMatchRelationshipPtr sr, _matchRelationships[r->getEdge()])
      {
        if (sr->isConflict() == false)
        {
          supportCount++;
        }
        relationCount++;
      }
      supportCount = max(1, supportCount);
      relationCount = max(1, relationCount);

      s = s * _weights[r->getEdge()];

      //s = s / (double)relationCount;
      s = s * pow(1.0 / (double)relationCount, _outboundWeighting);

      if (r->isConflict() == false)
      {
        LOG_INFO("support:  " << s << "\t" << r->getEdge()->toString().left(80));
        //s = s / (double)supportCount;
        numerator += s;
      }
      else
      {
        LOG_INFO("conflict: " << s << "\t" << r->getEdge()->toString().left(80));
        //s = s / (double)relationCount;
      }

      denominator += s;
    }

    newScores[em] = pow(numerator / denominator, aggression);
    newWeights[em] = denominator;
    weightSum += denominator;
    LOG_INFO(em << " " << numerator << "/" << denominator << " " << newScores[em] << " " <<
      newWeights[em]);
  }

  foreach (ConstEdgeMatchPtr em, newWeights.keys())
  {
    newWeights[em] = pow(newWeights[em] * newWeights.size() / weightSum, _weightInfluence);
  }

  _scores = newScores;
  _weights = newWeights;
}

void ConflictsNetworkMatcher::matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2)
{
  _n1 = n1;
  _n2 = n2;
  _details.reset(new NetworkDetails(map, n1, n2));

  // Add stub edges to both networks.
  // if both vertices on an edge match a single vertex (v2)
    // create a new zero length edge that connects v2 to itself
  _createEmptyStubEdges(_n1, _n2);
  _createEmptyStubEdges(_n2, _n1);

  // create empty stub edges can change the map, recreate the details so the indexes are
  // reinitialized.
  _details.reset(new NetworkDetails(map, n1, n2));

  // create a spatial index of n2 vertices & edges.
  _createEdge2Index();

  // create an initial estimation of edge match based on typical similarity scores
  _seedEdgeScores();

  _createMatchRelationships();
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
  for (OsmNetwork::EdgeMap::const_iterator it = em.begin(); it != em.end(); ++it)
  {
    PROGRESS_INFO(count++ << " / " << em.size());
    ConstNetworkEdgePtr e1 = it.value();
    // find all the n2 edges that are in range of this one
    Envelope env = _details->getEnvelope(it.value());
    env.expandBy(_details->getSearchRadius(it.value()));
    IntersectionIterator iit = _createIterator(env, _edge2Index);

    while (iit.next())
    {
      ConstNetworkEdgePtr e2 = _index2Edge[iit.getId()];
      LOG_VAR(e1);
      LOG_VAR(e2);

      double score = _details->getPartialEdgeMatchScore(e1, e2);
      LOG_VAR(score);
      if (score > 0)
      {
        // add all the EdgeMatches that are seeded with this edge pair.
        finder.addEdgeMatches(e1, e2);
      }
    }
  }
}

}
