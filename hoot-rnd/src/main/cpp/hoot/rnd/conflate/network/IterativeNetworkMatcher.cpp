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
#include "IterativeNetworkMatcher.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/rnd/conflate/network/EdgeMatch.h>

#include "EdgeMatchSetFinder.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(NetworkMatcher, IterativeNetworkMatcher)

const double IterativeNetworkMatcher::EPSILON = 1e-6;

IterativeNetworkMatcher::IterativeNetworkMatcher() :
  _p(.1),
  _dampening(1.5)
{
  _edgeMatches.reset(new IndexedEdgeMatchSet());
}

double IterativeNetworkMatcher::_aggregateScores(QList<double> pairs)
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

double IterativeNetworkMatcher::_calculateEdgeVertexScore(const VertexScoreMap& /*vm*/,
  ConstEdgeLocationPtr from1, ConstEdgeLocationPtr from2,
  ConstEdgeLocationPtr to1, ConstEdgeLocationPtr to2) const
{
  // These scores initialize as low values which only really get used if there is a partial match.
  // The idea being that low values here will dissuade using partial matches if there is a
  // competing full match.
  double sFrom = 0.1;
  double sTo = 0.1;

  // aggregate the scores of the vertex matches
  if (from1->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
    from2->isExtreme(EdgeLocation::SLOPPY_EPSILON))
  {
    sFrom = _scoreVertices(from1->getVertex(EdgeLocation::SLOPPY_EPSILON),
      from2->getVertex(EdgeLocation::SLOPPY_EPSILON));
  }
  if (to1->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
    to2->isExtreme(EdgeLocation::SLOPPY_EPSILON))
  {
    sTo = _scoreVertices(to1->getVertex(EdgeLocation::SLOPPY_EPSILON),
      to2->getVertex(EdgeLocation::SLOPPY_EPSILON));
  }

  return sqrt(sFrom * sTo);
}

shared_ptr<IterativeNetworkMatcher> IterativeNetworkMatcher::create()
{
  return shared_ptr<IterativeNetworkMatcher>(new IterativeNetworkMatcher());
}

void IterativeNetworkMatcher::_createEmptyStubEdges(OsmNetworkPtr na, OsmNetworkPtr nb)
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
    QSet<ConstNetworkEdgePtr> edgeStubsB;

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
          edgeStubsB.insert(e);
        }
      }
    }

    if (createStub)
    {
      foreach (ConstNetworkEdgePtr eb, edgeStubsB)
      {
        _createStubIntersection(na, nb, va, eb);
      }

//      //foreach (ConstNetworkEdgePtr e, na->getEdgesFromVertex(va))
//      QList<ConstNetworkEdgePtr> edges = na->getEdgesFromVertex(va);
//      if (edges.size() >= 1)
//      {
//        ConstNetworkEdgePtr e = edges[0];
//        // create a new vertex that points to the same element
//        NetworkVertexPtr newV(new NetworkVertex(va->getElement()));
//        LOG_INFO("Adding vertex: " << newV);
//        na->addVertex(newV);

//        LOG_INFO("Removing edge: " << e);
//        na->removeEdge(e);
//        ConstNetworkVertexPtr newFrom = e->getFrom();
//        ConstNetworkVertexPtr newTo = e->getTo();
//        if (e->getFrom() == va)
//        {
//          newFrom = newV;
//        }
//        if (e->getTo() == va)
//        {
//          newTo = newV;
//        }

//        // connect the new vertex to va with a new stub edge
//        NetworkEdgePtr newEdge(new NetworkEdge(newFrom, newTo, e->isDirected()));
//        newEdge->setMembers(e->getMembers());
//        LOG_INFO("Adding new edge: " << newEdge);
//        na->addEdge(newEdge);

//        // connect the new vertex to va with a new stub edge
//        NetworkEdgePtr newStub(new NetworkEdge(newV, va, false));
//        newStub->addMember(va->getElement());
//        na->addEdge(newStub);
//        LOG_INFO("Adding stub edge: " << newStub);
//      }
    }
  }
}

void IterativeNetworkMatcher::_createStubIntersection(OsmNetworkPtr na, OsmNetworkPtr nb,
  ConstNetworkVertexPtr va, ConstNetworkEdgePtr smallWayB)
{

  /// @todo break this into smaller functions
  // Find all the edges that connect to eb
  QSet<ConstNetworkEdgePtr> ebNeighborhood = QSet<ConstNetworkEdgePtr>::fromList(
    nb->getEdgesFromVertex(smallWayB->getFrom()));
  ebNeighborhood.unite(QSet<ConstNetworkEdgePtr>::fromList(nb->getEdgesFromVertex(smallWayB->getTo())));
  ebNeighborhood.remove(smallWayB);

  // represents which vertices an edge may match [vertex in nb][edge in na]
  QHash<ConstNetworkVertexPtr, QList<ConstNetworkEdgePtr> > edgeVMatch;
  QList<ConstNetworkEdgePtr> unmatched;

  // for each edge that intersects with va
  foreach (ConstNetworkEdgePtr ea, na->getEdgesFromVertex(va))
  {
    bool fromMatch = false, toMatch = false;

    // go through all the eb neighbors
    foreach (ConstNetworkEdgePtr ebNeighbor, ebNeighborhood)
    {
      if (ebNeighbor->contains(smallWayB->getFrom()))
      {
        if (_details->isPartialCandidateMatch(va, smallWayB->getFrom(), ea, ebNeighbor))
        {
          fromMatch = true;
        }
      }
      if (ebNeighbor->contains(smallWayB->getTo()))
      {
        if (_details->isPartialCandidateMatch(va, smallWayB->getTo(), ea, ebNeighbor))
        {
          toMatch = true;
        }
      }
    }

    // if the edge only matches one vertex, put it in edgeVMatch
    if (fromMatch != toMatch)
    {
      if (fromMatch)
      {
        edgeVMatch[smallWayB->getFrom()].append(ea);
      }
      else
      {
        edgeVMatch[smallWayB->getTo()].append(ea);
      }
    }
    // if the edge doesn't match any other edges put it in unmatched
    // or, if the edge matches multiple vertices put it in unmatched.
    else
    {
      unmatched.append(ea);
    }
  }

  // if this is relatively simple then modify the graph to add a 0:1 match
  // (AKA intersection:way match)
  if (unmatched.size() == 0)
  {
    QList<ConstNetworkVertexPtr> involvedVertices;

    // for each vertex in nb with clear matches
    foreach (ConstNetworkVertexPtr vb, edgeVMatch.keys())
    {
      // create a new stub vertex in na
      NetworkVertexPtr newV(new NetworkVertex(va->getElement()));
      LOG_INFO("Adding vertex: " << newV);
      na->addVertex(newV);
      involvedVertices.append(newV);

      // for each edge that clearly matches vb
      foreach (ConstNetworkEdgePtr ea, edgeVMatch[vb])
      {
        LOG_INFO("Removing edge: " << ea);
        na->removeEdge(ea);
        ConstNetworkVertexPtr newFrom = ea->getFrom();
        ConstNetworkVertexPtr newTo = ea->getTo();
        if (ea->getFrom() == va)
        {
          newFrom = newV;
        }
        if (ea->getTo() == va)
        {
          newTo = newV;
        }

        // relink each edge to this vertex by recreating the edge
        NetworkEdgePtr newEdge(new NetworkEdge(newFrom, newTo, ea->isDirected()));
        newEdge->setMembers(ea->getMembers());
        LOG_INFO("Adding new edge: " << newEdge);
        na->addEdge(newEdge);
      }
    }

    // if there are no unmatched edges, delete va
    if (edgeVMatch.size() > 0 && unmatched.size() == 0)
    {
      LOG_INFO("Removing vertex: " << va);
      na->removeVertex(va);
    }
    else
    {
      involvedVertices.append(va);
    }

    // make stub edges between all the vertices.
    for (int i = 0; i < involvedVertices.size(); ++i)
    {
      for (int j = i + 1; j < involvedVertices.size(); ++j)
      {
        ConstNetworkVertexPtr vi = involvedVertices[i];
        ConstNetworkVertexPtr vj = involvedVertices[j];

        // Create stub
        NetworkEdgePtr newStub(new NetworkEdge(vi, vj, false));
        newStub->addMember(va->getElement());
        LOG_INFO("Adding new edge: " << newStub);
        na->addEdge(newStub);
      }
    }
  }
}

QList<NetworkEdgeScorePtr> IterativeNetworkMatcher::getAllEdgeScores() const
{
  QList<NetworkEdgeScorePtr> result;

  for (EdgeScoreMap::const_iterator it = _edge12Scores.begin(); it != _edge12Scores.end();
    ++it)
  {
    NetworkEdgeScorePtr p(new NetworkEdgeScore(it.key(), it.value(),
      _edge21Scores[it.key()]));
    result.append(p);
  }

  return result;
}

QList<NetworkVertexScorePtr> IterativeNetworkMatcher::getAllVertexScores() const
{
  QList<NetworkVertexScorePtr> result;

  for (VertexScoreMap::const_iterator it = _vertex12Scores.begin(); it != _vertex12Scores.end();
    ++it)
  {
    ConstNetworkVertexPtr v1 = it.key();

    for (QHash<ConstNetworkVertexPtr, double>::const_iterator jt = it.value().begin();
      jt != it.value().end(); ++jt)
    {
      ConstNetworkVertexPtr v2 = jt.key();

      NetworkVertexScorePtr p(new NetworkVertexScore(v1, v2, jt.value(), _vertex21Scores[v2][v1]));
      result.append(p);
    }
  }

  return result;
}

QList<ConstNetworkEdgePtr> IterativeNetworkMatcher::_getEdgesOnVertex(ConstNetworkVertexPtr v)
{
  QList<ConstNetworkEdgePtr> r1 = _n1->getEdgesFromVertex(v);
  QList<ConstNetworkEdgePtr> r2 = _n2->getEdgesFromVertex(v);

  QList<ConstNetworkEdgePtr> result = r1;
  result.append(r2);

  return result;
}

void IterativeNetworkMatcher::iterate()
{
  // create a more refined estimate of vertex match based on the proximity & way count as well
  // as the neighboring edge scores (product of neighboring scores?)
  _updateVertexScores(_vertex12Scores, _edge12Scores);
  _updateVertexScores(_vertex21Scores, _edge21Scores);
  _normalizeAllScores();

  // create a more refined estimate of edge match based on the typical similarity scores and
  // the neighboring node scores (product of neighboring scores?)
  LOG_INFO("these should use a copy of _edge**Scores, then update the original");
  LOG_INFO("1 to 2");
  _updateEdgeScores(_edge12Scores, _vertex12Scores);
  LOG_INFO("2 to 1");
  _updateEdgeScores(_edge21Scores, _vertex21Scores);

  _normalizeAllScores();
}

void IterativeNetworkMatcher::matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2)
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
  _createVertex2Index();

  // create an initial estimation of node matching based on proximity & way count
  _seedVertexScores();
  // create an initial estimation of edge match based on typical similarity scores
  _seedEdgeScores();

  _normalizeAllScores();
}

void IterativeNetworkMatcher::_normalizeAllScores()
{
  // tends to push a big score to a single edge pair.
//  _normalizeScoresGlobal(_edge12Scores);
//  _normalizeScoresGlobal(_edge21Scores);
//  _normalizeScoresGlobal(_vertex12Scores);
//  _normalizeScoresGlobal(_vertex21Scores);

  _normalizeScoresLocal(_edge12Scores);
  _normalizeScoresLocal(_edge21Scores);
  _normalizeScoresLocal(_vertex12Scores);
  _normalizeScoresLocal(_vertex21Scores);
}

/// @todo this method will need to be redefined to take advantage of the new match strings
void IterativeNetworkMatcher::_normalizeScoresGlobal(EdgeScoreMap& t)
{
  double sum = 0.0;
  for (EdgeScoreMap::iterator it = t.begin(); it != t.end(); ++it)
  {
    sum += std::max(EPSILON, it.value());
  }

  sum /= t.size();

  for (EdgeScoreMap::iterator it = t.begin(); it != t.end(); ++it)
  {
    it.value() = std::max(EPSILON, it.value()) / sum;
  }
}

void IterativeNetworkMatcher::_normalizeScoresGlobal(VertexScoreMap& t)
{
  for (VertexScoreMap::iterator it = t.begin(); it != t.end(); ++it)
  {
    double sum = 0.0;
    QHash<ConstNetworkVertexPtr, double>& t2 = it.value();
    for (QHash<ConstNetworkVertexPtr, double>::iterator jt = t2.begin(); jt != t2.end(); ++jt)
    {
      sum += std::max(EPSILON, jt.value());
    }

    for (QHash<ConstNetworkVertexPtr, double>::iterator jt = t2.begin(); jt != t2.end();
      ++jt)
    {
      jt.value() = std::max(EPSILON, jt.value()) / sum;
    }
  }
}

/// @todo this method will need to be redefined to take advantage of the new match strings
void IterativeNetworkMatcher::_normalizeScoresLocal(EdgeScoreMap& t)
{
  EdgeScoreMap oldScores = t;

  foreach (ConstEdgeMatchPtr em, oldScores.keys())
  {
    QSet<ConstEdgeMatchPtr> overlap = _edgeMatches->getMatchesThatOverlap(em);

    double sum = 0;

    foreach (ConstEdgeMatchPtr e, overlap)
    {
      sum += oldScores[e];
    }

    // don't increase the score above the original score, only decrease.
    //t[em] = std::min(oldScores[em], oldScores[em] / sum);

    // let it max out at 1.0 if there are no conflicts
    t[em] = oldScores[em] / sum;
  }
}

void IterativeNetworkMatcher::_normalizeScoresLocal(VertexScoreMap& t)
{
  for (VertexScoreMap::iterator it = t.begin(); it != t.end(); ++it)
  {
    double sum = 0.0;
    QHash<ConstNetworkVertexPtr, double>& t2 = it.value();
    for (QHash<ConstNetworkVertexPtr, double>::iterator jt = t2.begin(); jt != t2.end(); ++jt)
    {
      sum += std::max(EPSILON, jt.value());
    }

    for (QHash<ConstNetworkVertexPtr, double>::iterator jt = t2.begin(); jt != t2.end();
      ++jt)
    {
      jt.value() = std::max(EPSILON, jt.value()) / sum;
    }
  }
}

double IterativeNetworkMatcher::_scoreEdges(ConstEdgeMatchPtr em) const
{
  // return the raw edge score (e.g. from classifier)
  //return _details->getEdgeStringMatchScore(es1, es2);

  // return the latest score for this edge, tends to push all the scores to one edge pair.
  return sqrt(_edge21Scores[em] * _edge12Scores[em]);
}

double IterativeNetworkMatcher::_scoreVertices(ConstNetworkVertexPtr v1,
  ConstNetworkVertexPtr v2) const
{
  // return the raw vertex score.
  //double result = _details->getVertexMatchScore(v1, v2);

  // use the latest derived vertex score.
  double result = sqrt(max(_vertex12Scores[v1][v2] * _vertex21Scores[v2][v1],
    _vertex12Scores[v1][v2] * _vertex21Scores[v2][v1]));

  return result;
}

void IterativeNetworkMatcher::_seedEdgeScores()
{
  EdgeMatchSetFinder finder(_details, _edgeMatches, _n1, _n2);
  finder.setIncludePartialMatches(true);

  // modify details so that zero length edges don't give a score of zero
  // modify finder so that it doesn't add zero length edges to edge strings

  // go through all the n1 edges
  const OsmNetwork::EdgeMap& em = _n1->getEdgeMap();
  for (OsmNetwork::EdgeMap::const_iterator it = em.begin(); it != em.end(); ++it)
  {
    ConstNetworkEdgePtr e1 = it.value();
    // find all the n2 edges that are in range of this one
    Envelope env = _details->getEnvelope(it.value());
    env.expandBy(_details->getSearchRadius(it.value()));
    IntersectionIterator iit = _createIterator(env, _edge2Index);

    while (iit.next())
    {
      ConstNetworkEdgePtr e2 = _index2Edge[iit.getId()];

      if (_details->getPartialEdgeMatchScore(e1, e2) > 0)
      {
        // add all the EdgeMatches that are seeded with this edge pair.
        finder.addEdgeMatches(e1, e2);
      }
    }
  }

  foreach (ConstEdgeMatchPtr em, _edgeMatches->getAllMatches().keys())
  {
    _edge12Scores[em] = _edgeMatches->getScore(em);
    _edge21Scores[em] = _edgeMatches->getScore(em);
  }
}

void IterativeNetworkMatcher::_seedVertexScores()
{
  // go through all the n1 vertices
  const OsmNetwork::VertexMap& vm = _n1->getVertexMap();
  for (OsmNetwork::VertexMap::const_iterator it = vm.begin(); it != vm.end(); ++it)
  {
    ConstNetworkVertexPtr v1 = it.value();

    // find all the vertices that are in range of this one
    ConstElementPtr e1 = it.value()->getElement();
    Envelope env = _details->getEnvelope(v1);
    env.expandBy(_details->getSearchRadius(v1));
    IntersectionIterator iit = _createIterator(env, _vertex2Index);

    // set the initial match score to 1 for all candidate matches
    while (iit.next())
    {
      ConstNetworkVertexPtr v2 = _index2Vertex[iit.getId()];

      double score = _details->getVertexMatchScore(v1, v2);
      if (score > 0)
      {
        _vertex12Scores[v1][v2] = score;
        _vertex21Scores[v2][v1] = score;
      }
    }
  }
}

void IterativeNetworkMatcher::_updateEdgeScores(EdgeScoreMap &em, const VertexScoreMap& vm)
{
  // go through all edge matches
  for (EdgeScoreMap::iterator it = em.begin(); it != em.end(); ++it)
  {
    ConstEdgeMatchPtr em1 = it.key();
    ConstEdgeStringPtr es1 = em1->getString1();
    ConstEdgeStringPtr es2 = em1->getString2();

    // aggregate the scores of the vertex matches
    // try matching each of the vertices since we don't know if 1 matches 1 or 1 matches 2.
    double newScore, v, e;
    v = _calculateEdgeVertexScore(vm, es1->getFrom(), es2->getFrom(),
      es1->getTo(), es2->getTo());

    e = _scoreEdges(em1);
    newScore = pow(v, _dampening) * pow(e, _p);

    it.value() = newScore;
  }
}

/// @todo modify this to use the EdgeMatchSet rather than the vertices
void IterativeNetworkMatcher::_updateVertexScores(VertexScoreMap& vm, EdgeScoreMap &/*em*/)
{
  CostFunction cost;
  if (&vm == &_vertex12Scores)
  {
    cost.em1 = &_edge12Scores;
    cost.em2 = &_edge21Scores;
  }
  else
  {
    cost.em2 = &_edge12Scores;
    cost.em1 = &_edge21Scores;
  }
  cost.p = _p;

  // go through all vertex matches
  for (VertexScoreMap::iterator it = vm.begin(); it != vm.end(); ++it)
  {
    ConstNetworkVertexPtr va = it.key();
    cost.v1 = va;

    // Go through all the potential matching vertices
    QHash<ConstNetworkVertexPtr, double>& t2 = it.value();
    for (QHash<ConstNetworkVertexPtr, double>::iterator jt = t2.begin(); jt != t2.end(); ++jt)
    {
      Saps sap(cost);
      ConstNetworkVertexPtr vb = jt.key();

      QSet<ConstEdgeMatchPtr> matches = _edgeMatches->getMatchesWithTermination(va, vb);

      QList<double> scores;
      foreach (ConstEdgeMatchPtr m, matches)
      {
        scores.append(_scoreEdges(m));
      }

      double edgeScore = std::max(EPSILON,
        pow(_aggregateScores(scores), _dampening) * pow(_scoreVertices(va, vb), _p));

/// @todo only use the best matches, don't use them all.
//      cost.v2 = v2;

//      QSet<EdgeStringPtr> strings1, strings2;

//      // get all the neighboring edges for the first vertex.
//      foreach (EdgeMatchPtr m, matches)
//      {
//        strings1.insert(m->getString1());
//        strings2.insert(m->getString2());
//      }

//      double edgeScore = EPSILON;
//      if (strings1.size() > 0 && strings2.size() > 0)
//      {
//        foreach (const ConstEdgeStringPtr& s1, strings1)
//        {
//          sap.addActor(s1.get());
//        }
//        foreach (const ConstEdgeStringPtr& s2, strings2)
//        {
//          sap.addTask(s2.get());
//        }

//        // find the best match of scores between the two sets of edges
//        vector<Saps::ResultPair> pairing = sap.calculatePairing();
//        QList<double> scores;
//        LOG_VAR(pairing);

//        for (size_t i = 0; i < pairing.size(); ++i)
//        {
//          if (pairing[i].actor && pairing[i].task)
//          {
//            scores.append(cost.cost(pairing[i].actor, pairing[i].task));
//          }
//        }

//        LOG_VAR(scores);
//        LOG_VAR(_scoreVertices(v1, v2));
//        // aggregate the scores between the two sets of edges
//        edgeScore = pow(_aggregateScores(scores), _dampening) * _scoreVertices(v1, v2);
//      }

      // set this vertex pair's score to the new aggregated score.
      jt.value() = edgeScore;
    }
  }
}

}
