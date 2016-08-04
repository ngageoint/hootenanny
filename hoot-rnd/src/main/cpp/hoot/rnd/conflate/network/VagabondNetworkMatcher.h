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
 *
 * The biggest disadvantage as this implementation stands now is that supporting subgraphs can be
 * created where the model will stagnate with each subgraph coming to its own internal solution, but
 * the subgraph's do not interact and so the solution never converges to a meaningful solution.
 * I've been calling these subgraphs "ghost graphs".
 */
class VagabondNetworkMatcher : public NetworkMatcher
{
public:

  static string className() { return "hoot::VagabondNetworkMatcher"; }

  /**
   * Use create instead.
   */
  VagabondNetworkMatcher();

  /**
   * Use this instead of a constructor. To simplify life a shared pointer should always be used.
   */
  static shared_ptr<VagabondNetworkMatcher> create();

  virtual void iterate();

  virtual void iteratePageRank();

  virtual void iteratePageRankBleeding();

  virtual void iterateVoting();

  virtual void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2);

  virtual QList<NetworkEdgeScorePtr> getAllEdgeScores() const;

  virtual QList<NetworkVertexScorePtr> getAllVertexScores() const;

private:

  IndexedEdgeLinksPtr _links;
  IndexedEdgeMatchSetPtr _pr;
  double _dampen;

  void _calculateEdgeLinks();
  void _calculateEdgeMatches();
  double _calculateLinkWeight(QHash<ConstNetworkEdgePtr, int>& counts, ConstEdgeStringPtr str);

  void _countEdgesUsed(QHash<ConstNetworkEdgePtr, int>& counts, ConstEdgeStringPtr str);

  void _distributePrEvenly();
  void _distributePrLengthWeighted();

  QSet<EdgeMatchPtr> _getConnectedEdges(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);
};

typedef shared_ptr<VagabondNetworkMatcher> VagabondNetworkMatcherPtr;
typedef shared_ptr<const VagabondNetworkMatcher> ConstVagabondNetworkMatcherPtr;

// not implemented
bool operator<(ConstVagabondNetworkMatcherPtr, ConstVagabondNetworkMatcherPtr);

}

#endif // __VAGABOND_NETWORK_MATCHER_H__
