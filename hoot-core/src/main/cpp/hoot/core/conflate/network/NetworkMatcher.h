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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef NETWORKMATCHER_H
#define NETWORKMATCHER_H

// hoot
#include <hoot/core/conflate/network/IndexedEdgeMatchSet.h>
#include <hoot/core/conflate/network/NetworkDetails.h>
#include <hoot/core/conflate/network/NetworkEdgeScore.h>
#include <hoot/core/conflate/network/NetworkVertexScore.h>
#include <hoot/core/conflate/network/OsmNetwork.h>
#include <hoot/core/elements/OsmMap.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/IntersectionIterator.h>

namespace hoot
{

/**
 * Not to be confused with a NetworkMatch, the NetworkMatcher matches up a whole network.
 */
class NetworkMatcher
{
public:

  const static double EPSILON;

  static QString className() { return "NetworkMatcher"; }

  NetworkMatcher() = default;
  virtual ~NetworkMatcher() = default;

  virtual double getMatchThreshold() const = 0;

  /**
   * Iterate torwards a solution. Each iteration should slowly move towards a better result, but
   * there will likely be a diminishing return for each call.
   */
  virtual void iterate() = 0;

  virtual void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2) = 0;

  virtual void finalize() { }

  virtual QList<NetworkEdgeScorePtr> getAllEdgeScores() const = 0;
  virtual QList<NetworkVertexScorePtr> getAllVertexScores() const = 0;

protected:

  Tgs::HilbertRTreePtr _edge2Index;
  Tgs::HilbertRTreePtr _vertex2Index;
  std::deque<ConstNetworkEdgePtr> _index2Edge;
  std::deque<ConstNetworkVertexPtr> _index2Vertex;
  ConstOsmMapPtr _map;
  OsmNetworkPtr _n1, _n2;
  NetworkDetailsPtr _details;

  void _createEdge2Index();

  Tgs::IntersectionIterator _createIterator(const geos::geom::Envelope& env, Tgs::HilbertRTreePtr tree) const;
};

using NetworkMatcherPtr = std::shared_ptr<NetworkMatcher>;
using ConstNetworkMatcherPtr = std::shared_ptr<const NetworkMatcher>;

// not implemented
bool operator<(ConstNetworkMatcherPtr, ConstNetworkMatcherPtr);

}

#endif // NETWORKMATCHER_H
