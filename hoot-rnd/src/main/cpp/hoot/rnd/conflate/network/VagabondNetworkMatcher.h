#ifndef __VAGABOND_NETWORK_MATCHER_H__
#define __VAGABOND_NETWORK_MATCHER_H__

// hoot
#include <hoot/rnd/conflate/network/NetworkMatcher.h>

#include "EdgeMatchSet.h"
#include "IndexedEdgeLinks.h"
#include "IndexedEdgeMatchSet.h"

namespace hoot
{

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

  /**
   * Use this instead of a constructor. To simplify life a shared pointer should always be used.
   */
  static shared_ptr<VagabondNetworkMatcher> create();

  virtual void iterate();

  virtual void iteratePageRank();

  virtual void iterateVoting();

  virtual void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2);

  virtual QList<NetworkEdgeScorePtr> getAllEdgeScores() const;

  virtual QList<NetworkVertexScorePtr> getAllVertexScores() const;

protected:
  /**
   * Use create instead.
   */
  VagabondNetworkMatcher();

private:

  IndexedEdgeLinks _links;
  IndexedEdgeMatchSetPtr _pr;
  double _dampen;

  void _calculateEdgeLinks();
  void _calculateEdgeMatches();
  QSet<EdgeMatchPtr> _getConnectedEdges(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);
};

typedef shared_ptr<VagabondNetworkMatcher> VagabondNetworkMatcherPtr;
typedef shared_ptr<const VagabondNetworkMatcher> ConstVagabondNetworkMatcherPtr;

}

#endif // __VAGABOND_NETWORK_MATCHER_H__
