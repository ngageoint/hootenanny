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
#include "IterativeEdgeMatcher.h"

namespace hoot
{

IterativeEdgeMatcher::IterativeEdgeMatcher() :
  _p(.5),
  _dampening(1)
{
}

double IterativeEdgeMatcher::_aggregateScores(QList<double> pairs)
{
  //qSort(pairs.begin(), pairs.end(), greaterThan);
  qSort(pairs);

  // this quick little method makes the scores decrease with each matching pair
  // the idea being that you get big boost for the highest score than then the benefit goes down
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

double IterativeEdgeMatcher::_calculateEdgeVertexScore(const VertexScoreMap& /*vm*/,
  ConstNetworkVertexPtr from1, ConstNetworkVertexPtr from2,
  ConstNetworkVertexPtr to1, ConstNetworkVertexPtr to2) const
{
  // aggregate the scores of the vertex matches
  double sFrom = std::max(_vertex12Scores[from1][from2], _vertex21Scores[from1][from2]) *
    std::max(_vertex12Scores[from2][from1], _vertex21Scores[from2][from1]);
  double sTo = std::max(_vertex12Scores[to1][to2], _vertex21Scores[to1][to2]) *
    std::max(_vertex12Scores[to2][to1], _vertex21Scores[to2][to1]);

  return pow(sFrom * sTo, _p);
}

QList<NetworkEdgeScorePtr> IterativeEdgeMatcher::getAllEdgeScores() const
{
  QList<NetworkEdgeScorePtr> result;

  for (EdgeScoreMap::const_iterator it = _edge12Scores.begin(); it != _edge12Scores.end();
    ++it)
  {
    ConstNetworkEdgePtr e1 = it.key();

    for (QHash<ConstNetworkEdgePtr, DirectedEdgeScore>::const_iterator jt = it.value().begin();
      jt != it.value().end(); ++jt)
    {
      ConstNetworkEdgePtr e2 = jt.key();

throw NotImplementedException();
//      NetworkEdgeScorePtr p(new NetworkEdgeScore(e1, e2, jt.value().score,
//        _edge21Scores[e2][e1].score));
//      result.append(p);
    }
  }

  return result;
}

QList<NetworkVertexScorePtr> IterativeEdgeMatcher::getAllVertexScores() const
{
  QList<NetworkVertexScorePtr> result;
  return result;
}

QList<ConstNetworkEdgePtr> IterativeEdgeMatcher::_getEdgesOnVertex(ConstNetworkVertexPtr v)
{
  QList<ConstNetworkEdgePtr> r1 = _n1->getEdgesFromVertex(v);
  QList<ConstNetworkEdgePtr> r2 = _n2->getEdgesFromVertex(v);

  QList<ConstNetworkEdgePtr> result = r1;
  result.append(r2);

  return result;
}

void IterativeEdgeMatcher::iterate()
{
  // create a more refined estimate of vertex match based on the proximity & way count as well
  // as the neighboring edge scores (product of neighboring scores?)
  _updateVertexScores(_vertex12Scores, _edge12Scores);
  _updateVertexScores(_vertex21Scores, _edge21Scores);
  _normalizeAllScores();

  // create a more refined estimate of edge match based on the typical similarity scores and
  // the neighboring node scores (product of neighboring scores?)
  LOG_INFO("1 to 2");
  _updateEdgeScores(_edge12Scores, _vertex12Scores);
  LOG_INFO("2 to 1");
  _updateEdgeScores(_edge21Scores, _vertex21Scores);
  _normalizeAllScores();
}

void IterativeEdgeMatcher::matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2)
{
  _n1 = n1;
  _n2 = n2;
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

void IterativeEdgeMatcher::_normalizeAllScores()
{
  _normalizeScores(_edge12Scores);
  _normalizeScores(_edge21Scores);
  _normalizeScores(_vertex12Scores);
  _normalizeScores(_vertex21Scores);
}

void IterativeEdgeMatcher::_normalizeGlobalScores(EdgeScoreMap& t)
{
  double sum = 0.0;
  for (EdgeScoreMap::iterator it = t.begin(); it != t.end(); ++it)
  {
    QHash<ConstNetworkEdgePtr, DirectedEdgeScore>& t2 = it.value();
    for (QHash<ConstNetworkEdgePtr, DirectedEdgeScore>::iterator jt = t2.begin(); jt != t2.end();
      ++jt)
    {
      sum += std::max(EPSILON, jt.value().score);
    }
  }
  double expected = std::max(_edge12Scores.size(), _edge21Scores.size());

  for (EdgeScoreMap::iterator it = t.begin(); it != t.end(); ++it)
  {
    QHash<ConstNetworkEdgePtr, DirectedEdgeScore>& t2 = it.value();
    for (QHash<ConstNetworkEdgePtr, DirectedEdgeScore>::iterator jt = t2.begin(); jt != t2.end();
      ++jt)
    {
      jt.value().score = std::max(EPSILON, jt.value().score) / sum * expected;
    }
  }
}

void IterativeEdgeMatcher::_normalizeGlobalScores(VertexScoreMap& t)
{
  double sum = 0.0;
  for (VertexScoreMap::iterator it = t.begin(); it != t.end(); ++it)
  {
    QHash<ConstNetworkVertexPtr, double>& t2 = it.value();
    for (QHash<ConstNetworkVertexPtr, double>::iterator jt = t2.begin(); jt != t2.end(); ++jt)
    {
      sum += std::max(EPSILON, jt.value());
    }
  }
  double expected = std::max(_vertex12Scores.size(), _vertex21Scores.size());

  for (VertexScoreMap::iterator it = t.begin(); it != t.end(); ++it)
  {
    QHash<ConstNetworkVertexPtr, double>& t2 = it.value();
    for (QHash<ConstNetworkVertexPtr, double>::iterator jt = t2.begin(); jt != t2.end();
      ++jt)
    {
      jt.value() = std::max(EPSILON, jt.value()) / sum * expected;
    }
  }
}

void IterativeEdgeMatcher::_normalizeScores(EdgeScoreMap& t)
{
  for (EdgeScoreMap::iterator it = t.begin(); it != t.end(); ++it)
  {
    double sum = 0.0;
    QHash<ConstNetworkEdgePtr, DirectedEdgeScore>& t2 = it.value();
    for (QHash<ConstNetworkEdgePtr, DirectedEdgeScore>::iterator jt = t2.begin(); jt != t2.end();
      ++jt)
    {
      sum += std::max(EPSILON, jt.value().score);
    }

    for (QHash<ConstNetworkEdgePtr, DirectedEdgeScore>::iterator jt = t2.begin(); jt != t2.end();
      ++jt)
    {
      jt.value().score = std::max(EPSILON, jt.value().score) / sum;
    }
  }
}

void IterativeEdgeMatcher::_normalizeScores(VertexScoreMap& t)
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

double IterativeEdgeMatcher::_scoreEdges(ConstNetworkEdgePtr e1,
  ConstNetworkEdgePtr e2) const
{
  return _details->getEdgeMatchScore(e1, e2);
}

double IterativeEdgeMatcher::_scoreVertices(ConstNetworkVertexPtr /*e1*/,
  ConstNetworkVertexPtr /*e2*/) const
{
  return 1.0;
}

void IterativeEdgeMatcher::_seedEdgeScores()
{
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
      double score = _scoreEdges(it.value(), e2);
      bool reversed = _details->isReversed(e1, e2);

      if (score > 0)
      {
        _edge12Scores[e1][e2] = DirectedEdgeScore(score, reversed);
        _edge21Scores[e2][e1] = DirectedEdgeScore(score, reversed);
      }
    }
  }
}

void IterativeEdgeMatcher::_seedVertexScores()
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

      double score = _scoreVertices(v1, v2);
      if (score > 0)
      {
        _vertex12Scores[v1][v2] = score;
        _vertex21Scores[v2][v1] = score;
      }
    }
  }
}

void IterativeEdgeMatcher::_updateEdgeScores(EdgeScoreMap &em, const VertexScoreMap& vm)
{
  // go through all edge matches
  for (EdgeScoreMap::iterator it = em.begin(); it != em.end(); ++it)
  {
    ConstNetworkEdgePtr e1 = it.key();

    // get the vertex match for the from node
    ConstNetworkVertexPtr from1 = e1->getFrom();
    // get the vertex match for the to node
    ConstNetworkVertexPtr to1 = e1->getTo();

    QHash<ConstNetworkEdgePtr, DirectedEdgeScore>& t2 = it.value();

    for (QHash<ConstNetworkEdgePtr, DirectedEdgeScore>::iterator jt = t2.begin(); jt != t2.end();
      ++jt)
    {
      ConstNetworkEdgePtr e2 = jt.key();
      ConstNetworkVertexPtr from2 = e2->getFrom();

      ConstNetworkVertexPtr to2 = e2->getTo();
      bool reversed = jt.value().reversed;

      // aggregate the scores of the vertex matches
      // try matching each of the vertices since we don't know if 1 matches 1 or 1 matches 2.
      double newScore, v, e;

      if (!reversed)
      {
        if (e1->getMembers()[0]->getId() == -1803179 || e2->getMembers()[0]->getId() == -1803179)
        {
          LOG_INFO(from1 << " => " << from2);
          LOG_INFO(to1 << " => " << to2);
        }
        v = _calculateEdgeVertexScore(vm, from1, from2, to1, to2);
        e = _scoreEdges(e1, e2);
        if (e1->getMembers()[0]->getId() == -1803179 || e2->getMembers()[0]->getId() == -1803179)
        {
          LOG_INFO(from1 << " => " << from2);
          LOG_INFO(to1 << " => " << to2);
          LOG_VAR(e1);
          LOG_VAR(e2);
          LOG_VAR(v);
          LOG_VAR(e);
        }
      }
      else
      {
        v = _calculateEdgeVertexScore(vm, to1, from2, from1, to2);
        e = _scoreEdges(e1, e2);
      }
      newScore = pow(v, _dampening) * e;

      jt.value() = DirectedEdgeScore(newScore, reversed);
    }
  }
}

void IterativeEdgeMatcher::_updateVertexScores(VertexScoreMap& vm, EdgeScoreMap &/*em*/)
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
    ConstNetworkVertexPtr v1 = it.key();
    cost.v1 = v1;

    QList<ConstNetworkEdgePtr> edges1 = _getEdgesOnVertex(v1);

    // Go through all the potential matching vertices
    QHash<ConstNetworkVertexPtr, double>& t2 = it.value();
    for (QHash<ConstNetworkVertexPtr, double>::iterator jt = t2.begin(); jt != t2.end(); ++jt)
    {
      Saps sap(cost);
      ConstNetworkVertexPtr v2 = jt.key();
      cost.v2 = v2;
      QList<ConstNetworkEdgePtr> edges2 = _getEdgesOnVertex(v2);

      // get all the neighboring edges for the first vertex.
      for (int i1 = 0; i1 < edges1.size(); ++i1)
      {
        sap.addActor(&(edges1[i1]));
      }

      // get all the neighboring edges for the second vertex.
      for (int i2 = 0; i2 < edges2.size(); ++i2)
      {
        sap.addTask(&(edges2[i2]));
      }

      // find the best match of scores between the two sets of edges
      vector<Saps::ResultPair> pairing = sap.calculatePairing();
      QList<double> scores;

      for (size_t i = 0; i < pairing.size(); ++i)
      {
        if (pairing[i].actor && pairing[i].task)
        {
          scores.append(cost.cost(pairing[i].actor, pairing[i].task));
        }
      }

      // aggregate the scores between the two sets of edges
      double edgeScore = pow(_aggregateScores(scores), _dampening) * _scoreVertices(v1, v2);

      // set this vertex pair's score to the new aggregated score.
      jt.value() = edgeScore;
    }
  }
}

}
