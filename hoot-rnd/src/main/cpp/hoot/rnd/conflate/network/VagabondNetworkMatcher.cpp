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
#include "VagabondNetworkMatcher.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/polygon/extractors/HausdorffDistanceExtractor.h>

#include "EdgeMatchSetFinder.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(NetworkMatcher, VagabondNetworkMatcher)

VagabondNetworkMatcher::VagabondNetworkMatcher() :
  _dampen(0.8)
{
  _pr.reset(new IndexedEdgeMatchSet());
}

shared_ptr<VagabondNetworkMatcher> VagabondNetworkMatcher::create()
{
  return shared_ptr<VagabondNetworkMatcher>(new VagabondNetworkMatcher());
}

QList<NetworkEdgeScorePtr> VagabondNetworkMatcher::getAllEdgeScores() const
{
  QList<NetworkEdgeScorePtr> result;

  for (IndexedEdgeMatchSet::MatchHash::const_iterator it = _pr->getAllMatches().begin();
    it != _pr->getAllMatches().end(); ++it)
  {
    NetworkEdgeScorePtr p(new NetworkEdgeScore(it.key(),
      it.value() * _pr->getAllMatches().size(), 1));
    p->setUid(it.key()->toString());

    result.append(p);
  }

  return result;
}

QList<NetworkVertexScorePtr> VagabondNetworkMatcher::getAllVertexScores() const
{
  QList<NetworkVertexScorePtr> result;
  return result;
}

void VagabondNetworkMatcher::iterate()
{
  iteratePageRank();
}

void VagabondNetworkMatcher::iteratePageRank()
{
  // assign a new PR to all edge pairs based on
  // PR(A) = (1 - _damping) / N + d * (PR(B) / L(B) + PR(C) / L(C) + ...)
  // where A is the edge being traversed into
  // B, C, ... are edges that traverse into A and L is the number of outbound edges for that edge
  // ###
  // ### And then a bit of experimentation that isn't documented. Read the code.
  // ###

  IndexedEdgeMatchSetPtr newHash = _pr->clone();

  double sum = 0.0;

  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    sum += it.value();
    it.value() = 0;
  }

  // add this weight to all edges at the end.
  double allWeight = (1 - _dampen) / _pr->getSize();

  /// @todo this is much less efficient then some iterator approaches, but it is easy to write &
  /// read. if this works and it is a bottle neck, please fix it.
  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    ConstEdgeMatchPtr from = it.key();
    QList<ConstEdgeMatchPtr> values = _links->values(from);

    if (values.size() != 0)
    {
      double contribution = (_pr->getScore(from) / (values.size())) * _dampen;

      foreach (ConstEdgeMatchPtr to, values)
      {
        newHash->setScore(to, newHash->getScore(to) + contribution);
      }
    }
    else
    {
      allWeight += _pr->getScore(from) / (double)_pr->getSize();
    }
  }

  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    it.value() = it.value() + allWeight;
  }

  _pr = newHash;
}

void VagabondNetworkMatcher::iteratePageRankBleeding()
{
  /// @todo
  throw NotImplementedException("@todo assign half the weight to one end of the way and half to "
    "the other. If an end connects to nothing then distribute the weight evenly across the graph."
    "Another possible option is to store weights in two directions (inbound & outbound). The "
    "weights can only be applied to one side or the other.");


  // assign a new PR to all edge pairs based on
  // PR(A) = (1 - _damping) / N + d * (PR(B) / L(B) + PR(C) / L(C) + ...)
  // where A is the edge being traversed into
  // B, C, ... are edges that traverse into A and L is the number of outbound edges for that edge
  // ###
  // ### And then a bit of experimentation that isn't documented. Read the code.
  // ###

  IndexedEdgeMatchSetPtr newHash = _pr->clone();

  double sum = 0.0;

  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    sum += it.value();
    it.value() = 0;
  }

  // add this weight to all edges at the end.
  double allWeight = (1 - _dampen) / _pr->getSize();

  /// @todo this is much less efficient then some iterator approaches, but it is easy to write &
  /// read. if this works and it is a bottle neck, please fix it.
  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    ConstEdgeMatchPtr from = it.key();
    QList<ConstEdgeMatchPtr> values = _links->values(from);

    if (values.size() != 0)
    {
      double contribution = (_pr->getScore(from) / (values.size())) * _dampen;

      //LOG_VAR(from);
      foreach (ConstEdgeMatchPtr to, values)
      {
        newHash->setScore(to, newHash->getScore(to) + contribution);
//        LOG_VAR(to);
//        LOG_VAR(newHash[to]);
//        LOG_VAR(contribution);
      }
    }
    else
    {
      allWeight += _pr->getScore(from) / (double)_pr->getSize();
    }
  }
  LOG_VAR(allWeight);

  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    it.value() = it.value() + allWeight;
  }

  _pr = newHash;
}

void VagabondNetworkMatcher::iterateVoting()
{
  // assign a new PR to all edge pairs based on
  // PR(A) = (1 - _damping) / N + d * (PR(B) / L(B) + PR(C) / L(C) + ...)
  // where A is the edge being traversed into
  // B, C, ... are edges that traverse into A and L is the number of outbound edges for that edge

  IndexedEdgeMatchSetPtr newHash = _pr->clone();

  double sum = 0.0;

  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    sum += it.value();
    it.value() = 0;
  }

  // add this weight to all edges at the end.
  double allWeight = (1 - _dampen) / _pr->getSize();

  // this is much less efficient then some iterator approaches, but it is easy to write & read.
  // if this works and it is a bottle neck, please fix it.
  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    ConstEdgeMatchPtr from = it.key();
    QList<ConstEdgeMatchPtr> values = _links->values(from);

    if (values.size() != 0)
    {
      double contribution = _pr->getScore(from) * _dampen;

      foreach (ConstEdgeMatchPtr to, values)
      {
        newHash->setScore(to, newHash->getScore(to) + contribution);
      }
    }
  }

  double finalSum = 0.0;
  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    it.value() = it.value() + allWeight;
    finalSum += it.value();
  }

  for (IndexedEdgeMatchSet::MatchHash::iterator it = newHash->getAllMatches().begin();
    it != newHash->getAllMatches().end(); ++it)
  {
    it.value() /= finalSum;
  }

  _pr = newHash;
}

void VagabondNetworkMatcher::matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2)
{
  _map = map;
  _n1 = n1;
  _n2 = n2;
  _details.reset(new NetworkDetails(map, n1, n2));

  // calculate all the candidate edge pairs.
  LOG_INFO("Calculating edge index...");
  _createEdge2Index();
  LOG_INFO("Calculating edge matches...");
  _calculateEdgeMatches();

  // calculate the edge pairs that can be accessed from each candidate edge pair
  LOG_INFO("Calculating edge links...");
  _calculateEdgeLinks();

  // for each link distribute a PR based on length and frequency an edge is used
  LOG_INFO("Distributing PR...");
  _distributePrLengthWeighted();
  // distribute an even PR to all edge pairs
  //_distributePrEvenly();
}

void VagabondNetworkMatcher::_calculateEdgeLinks()
{
  _links = _pr->calculateEdgeLinks();
}

void VagabondNetworkMatcher::_calculateEdgeMatches()
{
  EdgeMatchSetFinder finder(_details, _pr, _n1, _n2);

  // go through all the n1 edges
  const OsmNetwork::EdgeMap& em = _n1->getEdgeMap();
  int count = 0;
  //LOG_VAR(em);
  for (OsmNetwork::EdgeMap::const_iterator it = em.begin(); it != em.end(); ++it)
  {
    ConstNetworkEdgePtr e1 = it.value();
    // find all the n2 edges that are in range of this one
    Envelope env = _details->getEnvelope(it.value());
    env.expandBy(_details->getSearchRadius(it.value()));
    IntersectionIterator iit = _createIterator(env, _edge2Index);
    //LOG_VAR(e1);

    while (iit.next())
    {
      ConstNetworkEdgePtr e2 = _index2Edge[iit.getId()];

      if (_details->getPartialEdgeMatchScore(e1, e2) > 0)
      {
        // add all the EdgeMatches that are seeded with this edge pair.
        finder.addEdgeMatches(e1, e2);
      }
    }
    if (Log::getInstance().getLevel() <= Log::Info)
    {
      cout << "Calculating edge matches: " << count++ << " / " << em.size() << "\t\r" << std::flush;
    }
  }
  if (Log::getInstance().getLevel() <= Log::Info)
  {
    cout << endl;
  }
}

double VagabondNetworkMatcher::_calculateLinkWeight(QHash<ConstNetworkEdgePtr, int>& counts,
  ConstEdgeStringPtr str)
{
  double result = 0.0;
  for (int i = 0; i < str->getAllEdges().size(); ++i)
  {
    ConstNetworkEdgePtr e = str->getAllEdges()[i].getEdge();
    double w = 1.0 / counts[e];
    double l = _details->calculateLength(e);
    result += w * l;
  }

  return result;
}


void VagabondNetworkMatcher::_countEdgesUsed(QHash<ConstNetworkEdgePtr, int>& counts,
  ConstEdgeStringPtr str)
{
  for (int i = 0; i < str->getAllEdges().size(); ++i)
  {
    counts[str->getAllEdges()[i].getEdge()]++;
  }
}

void VagabondNetworkMatcher::_distributePrLengthWeighted()
{
  QHash<ConstNetworkEdgePtr, int> counts;

  // go through all the links
  for (IndexedEdgeMatchSet::MatchHash::iterator it = _pr->getAllMatches().begin();
    it != _pr->getAllMatches().end(); ++it)
  {
    // go through each edge and count the number of times it is used.
    _countEdgesUsed(counts, it.key()->getString1());
    _countEdgesUsed(counts, it.key()->getString2());
  }

  // distribute a weight equal to sum of v of all edges in a link.
  // where v is the length * (1 / number of times an edge is referenced).
  double sum = 0.0;
  for (IndexedEdgeMatchSet::MatchHash::iterator it = _pr->getAllMatches().begin();
    it != _pr->getAllMatches().end(); ++it)
  {
    double w = 0.0;
    w += _calculateLinkWeight(counts, it.key()->getString1());
    w += _calculateLinkWeight(counts, it.key()->getString2());
    it.value() = w;
    sum += w;
  }

  // normalize the values so they add to 1.
  for (IndexedEdgeMatchSet::MatchHash::iterator it = _pr->getAllMatches().begin();
    it != _pr->getAllMatches().end(); ++it)
  {
    it.value() = it.value() / sum;
  }
}

void VagabondNetworkMatcher::_distributePrEvenly()
{
  // distribute an even PR to all edge pairs
  double startPr = 1.0 / (double)_pr->getSize();
  LOG_VAR(startPr);
  for (IndexedEdgeMatchSet::MatchHash::iterator it = _pr->getAllMatches().begin();
    it != _pr->getAllMatches().end(); ++it)
  {
    it.value() = startPr;
  }
}

}
