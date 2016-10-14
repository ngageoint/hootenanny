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
#include "SingleSidedNetworkMatcher.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/rnd/conflate/network/EdgeMatch.h>

#include "EdgeMatchSetFinder.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(NetworkMatcher, SingleSidedNetworkMatcher)

const double SingleSidedNetworkMatcher::EPSILON = 1e-6;

SingleSidedNetworkMatcher::SingleSidedNetworkMatcher() :
  _dampening(.85),
  _w(1.5)
{
  _edgeMatches.reset(new IndexedEdgeMatchSet());
}

double SingleSidedNetworkMatcher::_calculateProbabilityOfMatch(ConstEdgeMatchPtr e) const
{
  // probability of this match is the product of the probability of each edge from n2 that
  // participates in this match.
  bool found = false;
  double score = 1.0;

  foreach (const EdgeString::EdgeEntry& ee, e->getString2()->getAllEdges())
  {
    foreach (const EdgeLinkScorePtr& els, _edge2Scores[ee.getEdge()])
    {
      if (els->match == e)
      {
        score *= els->score;
        found = true;
      }
    }
  }

  if (!found)
  {
    // note that this is 0, not EPSILON, if there are no match scores found this isn't a valid
    // match.
    score = 0.0;
  }

  return score;
}

double SingleSidedNetworkMatcher::_calculateProbabilityOfMatch(ConstNetworkVertexPtr v1,
  ConstNetworkVertexPtr v2) const
{
  QSet<ConstEdgeMatchPtr> m = _edgeMatches->getMatchesWithTermination(v1, v2);

  double pOfWrong = 1;

  // if we found some edges, use those to determine the probability that none of the matches are
  // correct.
  if (m.isEmpty() == false)
  {
    foreach (ConstEdgeMatchPtr e, m)
    {
      pOfWrong *= 1 - _calculateProbabilityOfMatch(e);
    }
  }
  // if we don't have any matches for v1/v2 then we've got very little information. Fall back on
  // our original vertex score estimates.
  else
  {
    if (_vertex21Scores[v2].contains(v2))
    {
      pOfWrong = 1 - _vertex21Scores[v2][v1];
    }
    // what!? We've got no information at all (happens at dead ends)
    else
    {
      // maybe this is legit, dunno. It may require tuning.
      pOfWrong = 0.0;
    }
  }

  return 1 - pOfWrong;
}

shared_ptr<SingleSidedNetworkMatcher> SingleSidedNetworkMatcher::create()
{
  return shared_ptr<SingleSidedNetworkMatcher>(new SingleSidedNetworkMatcher());
}

QList<NetworkEdgeScorePtr> SingleSidedNetworkMatcher::getAllEdgeScores() const
{
  QList<NetworkEdgeScorePtr> result;

  foreach (const ConstEdgeMatchPtr& match, _edgeMatches->getAllMatches().keys())
  {
    // game the system so the final score is what we want.
    double s = sqrt(_calculateProbabilityOfMatch(match));
    LOG_INFO("s: " << _calculateProbabilityOfMatch(match) << " " << match);
    NetworkEdgeScorePtr score(new NetworkEdgeScore(match, s, s));
    result.append(score);
  }

  return result;
}

QList<NetworkVertexScorePtr> SingleSidedNetworkMatcher::getAllVertexScores() const
{
  QList<NetworkVertexScorePtr> result;

  for (VertexScoreMap::const_iterator it = _vertex21Scores.begin(); it != _vertex21Scores.end();
    ++it)
  {
    ConstNetworkVertexPtr v2 = it.key();

    for (QHash<ConstNetworkVertexPtr, double>::const_iterator jt = it.value().begin();
      jt != it.value().end(); ++jt)
    {
      ConstNetworkVertexPtr v1 = jt.key();

      NetworkVertexScorePtr p(new NetworkVertexScore(v1, v2, jt.value(), jt.value()));
      result.append(p);
    }
  }

  return result;
}

double SingleSidedNetworkMatcher::_getScore(ConstNetworkEdgePtr e2, ConstEdgeMatchPtr em) const
{
  double result = 0.0;

  foreach (const EdgeLinkScorePtr& score, _edge2Scores[e2])
  {
    if (score->match == em)
    {
      result = score->score;
    }
  }

  return result;
}

void SingleSidedNetworkMatcher::iterate()
{
  //_updateEdgeScoresAdditive();
  _updateEdgeScores();

  _normalizeScores();
}

void SingleSidedNetworkMatcher::matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2)
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

  _normalizeScores();

  // calculate how one edge match can link to another.
  _links = _edgeMatches->calculateEdgeLinks();
}

/// @todo this method will need to be redefined to take advantage of the new match strings
void SingleSidedNetworkMatcher::_normalizeScores()
{
  // normalize the scores based on each edge in the second input. The value can be less than 1.
  // The difference between 1 is implicitly the P(no match).
  foreach(ConstNetworkEdgePtr e2, _edge2Scores.keys())
  {
    QList<EdgeLinkScorePtr> scores = _edge2Scores[e2];
    double sum = 0.0;

    LOG_INFO("e2: " << e2 << " score links: " << scores);
    foreach (const EdgeLinkScorePtr& s, scores)
    {
      sum += pow(s->score, _w);
    }

    if (sum > 1.0)
    {
      for (int i = 0; i < scores.size(); ++i)
      {
        // score should be at least EPSILON
        scores[i]->score = std::max(EPSILON, pow(scores[i]->score, _w) / sum);
      }
    }
    LOG_INFO("e2: " << e2 << " score links: " << scores);
  }
}

double SingleSidedNetworkMatcher::_scoreEdges(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const
{
  return _details->getEdgeMatchScore(e1, e2);
}

double SingleSidedNetworkMatcher::_scoreVertices(ConstNetworkVertexPtr /*e1*/,
  ConstNetworkVertexPtr /*e2*/) const
{
  return 1.0;
}

void SingleSidedNetworkMatcher::_seedEdgeScores()
{
  EdgeMatchSetFinder finder(_details, _edgeMatches, _n1, _n2);

  // go through all the n1 edges
  const OsmNetwork::EdgeMap& em = _n1->getEdgeMap();
  LOG_VAR(em);
  for (OsmNetwork::EdgeMap::const_iterator it = em.begin(); it != em.end(); ++it)
  {
    ConstNetworkEdgePtr e1 = it.value();
    // find all the n2 edges that are in range of this one
    Envelope env = _details->getEnvelope(it.value());
    env.expandBy(_details->getSearchRadius(it.value()));
    IntersectionIterator iit = _createIterator(env, _edge2Index);
    LOG_VAR(e1);

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

  // put the edges into a structure where we can score them.
  foreach (ConstEdgeMatchPtr em, _edgeMatches->getAllMatches().keys())
  {
    ConstEdgeStringPtr s2 = em->getString2();
    foreach (const EdgeString::EdgeEntry& ee, s2->getAllEdges())
    {
      // seed all scores w/ 1.0. This will be normalized later.
      _edge2Scores[ee.getEdge()].append(EdgeLinkScorePtr(new EdgeLinkScore(em, 1.0)));
    }
  }
}

void SingleSidedNetworkMatcher::_seedVertexScores()
{
  // go through all the n2 vertices
  const OsmNetwork::VertexMap& vm = _n2->getVertexMap();
  for (OsmNetwork::VertexMap::const_iterator it = vm.begin(); it != vm.end(); ++it)
  {
    ConstNetworkVertexPtr v2 = it.value();

    QList<ConstNetworkVertexPtr> matches = _details->getCandidateMatches(v2);
    foreach(ConstNetworkVertexPtr v1, matches)
    {
      _vertex21Scores[v2][v1] = 1.0 / matches.size();
    }
  }
}

/// @todo modify this method to take a EdgeMatchSet or just one half of the set. -- dunno how
/// that will work yet.
void SingleSidedNetworkMatcher::_updateEdgeScores()
{
  EdgeMatchScoreMap newScores;

  // for each edge (e2) in n2
  const OsmNetwork::EdgeMap& em2 = _n2->getEdgeMap();
  for (OsmNetwork::EdgeMap::const_iterator it = em2.begin(); it != em2.end(); ++it)
  {
    ConstNetworkEdgePtr e2 = it.value();

    // set the new score to
    //
    //  old score * 1 - product(1 - P(neighbor i) * _dampen))

    // for each match that contains e2
    foreach (ConstEdgeMatchPtr e2Match, _edgeMatches->getMatchesThatContain(e2))
    {
      double pOfNoMatchesFrom = 1.0;
      double pOfNoMatchesTo = 1.0;
      int count = 0;

      bool foundFromLink = false;
      bool foundToLink = false;

      QList<double> Ps, v;
      // for each match that links to e2Match
      foreach (ConstEdgeMatchPtr link, _links->values(e2Match))
      {
        double Pi = _calculateProbabilityOfMatch(link);
        ++count;

        if (link->contains(e2Match->getString1()->getFromVertex()))
        {
          foundFromLink = true;
          pOfNoMatchesFrom *= 1 - (Pi * _dampening);
        }
        else if (link->contains(e2Match->getString1()->getToVertex()))
        {
          foundToLink = true;
          pOfNoMatchesTo *= 1 - (Pi * _dampening);
        }
        else
        {
          LOG_VAR(e2);
          LOG_VAR(link);
          LOG_VAR(e2Match);
          LOG_VAR(_links->values(e2Match));
          throw InternalErrorException("Expected the link to contain either the from or to "
            "vertex.");
        }
      }

      if (!foundFromLink)
      {
        double p = _calculateProbabilityOfMatch(e2Match->getString1()->getFromVertex(),
          e2Match->getString2()->getFromVertex());
        pOfNoMatchesFrom *= 1 - (p * _dampening);
        LOG_VAR(e2Match);
        LOG_VAR(p);
      }
      if (!foundToLink)
      {
        double p = _calculateProbabilityOfMatch(e2Match->getString1()->getToVertex(),
          e2Match->getString2()->getToVertex());
        pOfNoMatchesTo *= 1 - (p * _dampening);
        LOG_VAR(e2Match);
        LOG_VAR(p);
      }

      double score = _getScore(e2, e2Match);
      // if there are no neighbors to give evidence don't modify the score.
      if (count >= 1)
      {
        score = (1 - pOfNoMatchesFrom * pOfNoMatchesTo);
      }

      if (e2->getMembers()[0]->getElementId().getId() == -19)
      {
        LOG_VAR(e2Match);
        LOG_VAR(pOfNoMatchesFrom);
        LOG_VAR(pOfNoMatchesTo);
        LOG_VAR(foundFromLink);
        LOG_VAR(foundToLink);
        LOG_VAR(score);
      }

      newScores[e2].append(EdgeLinkScorePtr(new EdgeLinkScore(e2Match, score)));
    }
  }

  _edge2Scores = newScores;
}

/// @todo modify this method to take a EdgeMatchSet or just one half of the set. -- dunno how
/// that will work yet.
void SingleSidedNetworkMatcher::_updateEdgeScoresAdditive()
{
  EdgeMatchScoreMap newScores;

  // for each edge (e2) in n2
  const OsmNetwork::EdgeMap& em2 = _n2->getEdgeMap();
  for (OsmNetwork::EdgeMap::const_iterator it = em2.begin(); it != em2.end(); ++it)
  {
    ConstNetworkEdgePtr e2 = it.value();

    // set the new score to
    //
    //  old score * 1 - product(1 - P(neighbor i) * _dampen))

    // for each match that contains e2
    foreach (ConstEdgeMatchPtr e2Match, _edgeMatches->getMatchesThatContain(e2))
    {
      double scoreFrom = 1.0;
      double scoreTo = 1.0;
      int count = 0;

      bool foundFromLink = false;
      bool foundToLink = false;

      QList<double> Ps, v;
      // for each match that links to e2Match
      foreach (ConstEdgeMatchPtr link, _links->values(e2Match))
      {
        double Pi = _calculateProbabilityOfMatch(link);
        ++count;

        if (link->contains(e2Match->getString1()->getFromVertex()))
        {
          foundFromLink = true;
          scoreFrom += (Pi * _dampening);
        }
        else if (link->contains(e2Match->getString1()->getToVertex()))
        {
          foundToLink = true;
          scoreTo += (Pi * _dampening);
        }
        else
        {
          LOG_VAR(e2);
          LOG_VAR(link);
          LOG_VAR(e2Match);
          throw InternalErrorException("Expected the link to contain either the from or to "
            "vertex.");
        }
      }

      if (!foundFromLink)
      {
        double p = _calculateProbabilityOfMatch(e2Match->getString1()->getFromVertex(),
          e2Match->getString2()->getFromVertex());
        scoreFrom += (p * _dampening);
        LOG_VAR(e2Match);
        LOG_VAR(p);
      }
      if (!foundToLink)
      {
        double p = _calculateProbabilityOfMatch(e2Match->getString1()->getToVertex(),
          e2Match->getString2()->getToVertex());
        scoreTo += (p * _dampening);
        LOG_VAR(e2Match);
        LOG_VAR(p);
      }

      double score = _getScore(e2, e2Match);
      // if there are no neighbors to give evidence don't modify the score.
      if (count >= 1)
      {
        score = scoreFrom + scoreTo;
      }

      if (e2->getMembers()[0]->getElementId().getId() == -19)
      {
        LOG_VAR(e2Match);
        LOG_VAR(scoreFrom);
        LOG_VAR(scoreTo);
        LOG_VAR(foundFromLink);
        LOG_VAR(foundToLink);
        LOG_VAR(score);
      }

      newScores[e2].append(EdgeLinkScorePtr(new EdgeLinkScore(e2Match, score)));
    }
  }

  _edge2Scores = newScores;
}

/// @todo modify this to use the EdgeMatchSet rather than the vertices
void SingleSidedNetworkMatcher::_updateVertexScores()
{
}

}
