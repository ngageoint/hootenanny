#ifndef __VAGABOND_NETWORK_MATCHER_H__
#define __VAGABOND_NETWORK_MATCHER_H__

// hoot
#include <hoot/rnd/conflate/network/NetworkMatcher.h>

namespace hoot
{

class NetworkEdgePair
{
public:
  ConstNetworkEdgePtr e1;
  ConstNetworkEdgePtr e2;
  bool reversed;
  static int count;
  int id;

  NetworkEdgePair(ConstNetworkEdgePtr ep1, ConstNetworkEdgePtr ep2) : e1(ep1), e2(ep2)
  {
    count++;
    id = count;
  }
  ConstNetworkVertexPtr getFrom1() { return e1->getFrom(); }
  ConstNetworkVertexPtr getTo1() { return e1->getTo(); }
  /**
   * Returns from for the second edge accounting for the reversal. E.g. getFrom1() will always
   * match to getFrom2().
   */
  ConstNetworkVertexPtr getFrom2()
  {
    if (reversed)
    {
      return e2->getTo();
    }
    else
    {
      return e2->getFrom();
    }
  }

  /**
   * Returns from for the second edge accounting for the reversal. E.g. getTo1() will always
   * match to getTo2().
   */
  ConstNetworkVertexPtr getTo2()
  {
    if (reversed)
    {
      return e2->getFrom();
    }
    else
    {
      return e2->getTo();
    }
  }

  QString toString() const
  {
    return QString::number(id) + " " + e1->toString() + ", " + e2->toString() +
      (reversed ? "(rev)" : "");
  }
};

typedef shared_ptr<NetworkEdgePair> NetworkEdgePairPtr;

inline uint qHash(const NetworkEdgePairPtr& v) { return qHash(v->e1) ^ qHash(v->e2); }
inline bool operator==(const NetworkEdgePairPtr& v1, const NetworkEdgePairPtr& v2)
{
  return *(v1->e1) == *(v2->e1) && *(v1->e2) == *(v2->e2);
}

/**
 * Start a random location in one of the networks, then recursively traverse the graph starting at
 * that location. At each hop in the graph, determine which edges could have been used in the
 * corresponding network and where they would lead. If there is no way to get there while
 * maintaining the Hausdorff distance, then it is a dead end. Otherwise, assign it a portion of the
 * total weight that the vagrant has and repeat.
 *
 * Backtracking is allowed.
 *
 * To make the terminology a little more precise, the term link will refer to a link from one
 * pair of edges to another pair of edges.
 *
 * This has many parallels to PageRank -- a partial inspiration.
 *
 * This approach has some neat benefits. It will find multiple possible solutions at one time at
 * which point an optimization approach such as Linear Programming could be used to create a final
 * solution. It also appears as though it isn't really suffering from a local maxima, but that
 * also leads to a bit of indecision.
 *
 * One notable benefit to this approach is that it is highly scalable. A hadoop implemenation
 * is very feasible, but would be outside the existing parallel conflation code.
 */
class VagabondNetworkMatcher : public NetworkMatcher
{
public:
  VagabondNetworkMatcher();

  virtual void iterate();

  virtual void iteratePageRank();

  virtual void iterateVoting();

  virtual void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2);

  virtual QList<NetworkEdgeScorePtr> getAllEdgeScores() const;

  virtual QList<NetworkVertexScorePtr> getAllVertexScores() const;

private:
  // links in the form {from, [to]}
  typedef QMultiHash<NetworkEdgePairPtr, NetworkEdgePairPtr> LinkHash;
  typedef QHash<NetworkEdgePairPtr, double> PrHash;
  LinkHash _links;
  QHash<NetworkEdgePairPtr, double> _pr;
  double _dampen;

  void _calculateEdgeLinks();
  void _calculateEdgePairs();
  QSet<NetworkEdgePairPtr> _getConnectedEdges(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);
};

}

#endif // __VAGABOND_NETWORK_MATCHER_H__
