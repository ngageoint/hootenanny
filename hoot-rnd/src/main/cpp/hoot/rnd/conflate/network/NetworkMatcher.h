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
#ifndef NETWORKMATCHER_H
#define NETWORKMATCHER_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/rnd/conflate/network/NetworkDetails.h>
#include <hoot/rnd/conflate/network/NetworkEdgeScore.h>
#include <hoot/rnd/conflate/network/NetworkVertexScore.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/IntersectionIterator.h>

namespace hoot
{

using namespace Tgs;

/**
 * Not to be confused with a NetworkMatch, the NetworkMatcher matches up a whole network.
 */
class NetworkMatcher
{
public:
  const static double EPSILON;

  static string className() { return "hoot::NetworkMatcher"; }

  virtual ~NetworkMatcher();

  virtual double getMatchThreshold() const { return 0.15; }

  /**
   * Iterate torwards a solution. Each iteration should slowly move towards a better result, but
   * there will likely be a diminishing return for each call.
   */
  virtual void iterate() = 0;

  virtual void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2) = 0;

  virtual QList<NetworkEdgeScorePtr> getAllEdgeScores() const = 0;

  virtual QList<NetworkVertexScorePtr> getAllVertexScores() const = 0;

protected:
  HilbertRTreePtr _edge2Index;
  HilbertRTreePtr _vertex2Index;
  deque<ConstNetworkEdgePtr> _index2Edge;
  deque<ConstNetworkVertexPtr> _index2Vertex;
  ConstOsmMapPtr _map;
  OsmNetworkPtr _n1, _n2;
  NetworkDetailsPtr _details;

  void _createEdge2Index();

  void _createVertex2Index();

  IntersectionIterator _createIterator(Envelope env, HilbertRTreePtr tree);

};

typedef shared_ptr<NetworkMatcher> NetworkMatcherPtr;
typedef shared_ptr<const NetworkMatcher> ConstNetworkMatcherPtr;

// not implemented
bool operator<(ConstNetworkMatcherPtr, ConstNetworkMatcherPtr);

}

#endif // NETWORKMATCHER_H
