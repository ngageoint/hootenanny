#include "VagabondNetworkMatcher.h"

#include <hoot/core/conflate/polygon/extractors/HausdorffDistanceExtractor.h>

namespace hoot
{

int NetworkEdgePair::count = 0;

VagabondNetworkMatcher::VagabondNetworkMatcher() :
  _dampen(0.95)
{
}

QList<NetworkEdgeScorePtr> VagabondNetworkMatcher::getAllEdgeScores() const
{
  QList<NetworkEdgeScorePtr> result;

  for (PrHash::const_iterator it = _pr.begin(); it != _pr.end(); ++it)
  {
    NetworkEdgeScorePtr p(new NetworkEdgeScore(it.key()->e1, it.key()->e2, it.value() * _pr.size(),
      1));
    p->setUid(QString("%1").arg(it.key()->id));
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
  // assign a new PR to all edge pairs based on
  // PR(A) = (1 - _damping) / N + d * (PR(B) / L(B) + PR(C) / L(C) + ...)
  // where A is the edge being traversed into
  // B, C, ... are edges that traverse into A and L is the number of outbound edges for that edge

  PrHash newHash = _pr;

  double sum = 0.0;

  for (PrHash::iterator it = newHash.begin(); it != newHash.end(); ++it)
  {
    sum += it.value();
    it.value() = 0;
  }

  LOG_VAR(sum);
  LOG_VAR(_links.size());

  // add this weight to all edges at the end.
  double allWeight = (1 - _dampen) / _pr.size();

  // this is much less efficient then some iterator approaches, but it is easy to write & read.
  // if this works and it is a bottle neck, please fix it.
  for (PrHash::iterator it = newHash.begin(); it != newHash.end(); ++it)
  {
    NetworkEdgePairPtr from = it.key();
    QList<NetworkEdgePairPtr> values = _links.values(from);

    if (values.size() != 0)
    {
      double contribution = (_pr[from] / values.size()) * _dampen;

      foreach (NetworkEdgePairPtr to, values)
      {
        newHash[to] = newHash[to] + contribution;
      }
    }
    else
    {
      allWeight += _pr[from] / (double)_pr.size();
    }
  }

  for (PrHash::iterator it = newHash.begin(); it != newHash.end(); ++it)
  {
    it.value() = it.value() + allWeight;
  }

  _pr = newHash;
}

void VagabondNetworkMatcher::matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2)
{
  _map = map;
  _n1 = n1;
  _n2 = n2;
  _details1.reset(new NetworkDetails(map, n1));
  _details2.reset(new NetworkDetails(map, n2));

  // calculate all the candidate edge pairs.
  _createEdge2Index();
  _calculateEdgePairs();

  // calculate the edge pairs that can be accessed from each candidate edge pair
  _calculateEdgeLinks();

  // (not yet) for each edge in base, distribute an even PR to all its edge pairs.
  // -- or --
  // (now) distribute an even PR to all edge pairs
  double startPr = 1.0 / (double)_pr.size();
  LOG_VAR(startPr);
  for (PrHash::iterator it = _pr.begin(); it != _pr.end(); ++it)
  {
    it.value() = startPr;
  }
}

void VagabondNetworkMatcher::_calculateEdgeLinks()
{
  for (QHash<NetworkEdgePairPtr, double>::const_iterator it = _pr.begin(); it != _pr.end();
    ++it)
  {
    NetworkEdgePairPtr ep = it.key();
    ConstNetworkVertexPtr from1, from2, to1, to2;
    from1 = ep->getFrom1();
    to1 = ep->getTo1();
    from2 = ep->getFrom2();
    to2 = ep->getTo2();

    // get all the edges that connect to from1/from2 and are also a proper pair.
    QSet<NetworkEdgePairPtr> fromLinks = _getConnectedEdges(from1, from2);

    // get all the edges that connect to to1/to2 and are also a proper pair.
    QSet<NetworkEdgePairPtr> toLinks = _getConnectedEdges(to1, to2);

    QSet<NetworkEdgePairPtr> links = fromLinks | toLinks;

    foreach(NetworkEdgePairPtr other, links)
    {
      // if the other edge isn't part of this edge.
      if (other->e1 != ep->e1 && other->e2 != ep->e2)
      {
        _links.insertMulti(ep, other);
      }
    }
  }

  LOG_VAR(_links.size());
}

void VagabondNetworkMatcher::_calculateEdgePairs()
{
  HausdorffDistanceExtractor hde;

  // go through all the n1 edges
  const OsmNetwork::EdgeMap& em = _n1->getEdgeMap();
  for (OsmNetwork::EdgeMap::const_iterator it = em.begin(); it != em.end(); ++it)
  {
    ConstNetworkEdgePtr e1 = it.value();
    // find all the n2 edges that are in range of this one
    Envelope env = _details1->getEnvelope(it.value());
    env.expandBy(_details1->getSearchRadius(it.value()));
    IntersectionIterator iit = _createIterator(env, _edge2Index);

    while (iit.next())
    {
      ConstNetworkEdgePtr e2 = _index2Edge[iit.getId()];
      /// @todo an assumption for now that may not work in the future.
      assert(e1->getMembers().size() == 1 && e2->getMembers().size() == 1);
      double radius = std::max(_details1->getSearchRadius(it.value()),
        _details2->getSearchRadius(e2));
      double d = hde.distance(*_map, e1->getMembers()[0], e2->getMembers()[0]);
      LOG_VAR(d);

      if (d < radius)
      {
        NetworkEdgePairPtr nep(new NetworkEdgePair(e1, e2));
        nep->reversed = _details1->isReversed(e1, e2);
        if (_details1->isCandidateMatch(nep->getFrom1(), nep->getFrom2()) &&
          _details1->isCandidateMatch(nep->getTo1(), nep->getTo2()))
        {
          _pr[nep] = 0;
        }
      }
    }
  }
}

QSet<NetworkEdgePairPtr> VagabondNetworkMatcher::_getConnectedEdges(ConstNetworkVertexPtr v1,
  ConstNetworkVertexPtr v2)
{
  QSet<NetworkEdgePairPtr> result;

  // get all edges connected to v1
  QList<ConstNetworkEdgePtr> el1 = _n1->getEdgesFromVertex(v1);
  // get all edges connected to v2
  QList<ConstNetworkEdgePtr> el2 = _n2->getEdgesFromVertex(v2);

  // look for all edge pairs
  foreach (ConstNetworkEdgePtr e1, el1)
  {
    foreach (ConstNetworkEdgePtr e2, el2)
    {
      // do a search radius sanity check and make sure it is a valid edge pair..
      ConstNetworkVertexPtr vother1 = e1->getFrom() == v1 ? e1->getTo() : e1->getFrom();
      ConstNetworkVertexPtr vother2 = e2->getFrom() == v2 ? e2->getTo() : e2->getFrom();

      NetworkEdgePairPtr ep(new NetworkEdgePair(e1, e2));
      if (_details1->isCandidateMatch(vother1, vother2) &&
        _pr.contains(ep))
      {
        result.insert(_pr.find(ep).key());
      }
    }
  }

  return result;
}

}
