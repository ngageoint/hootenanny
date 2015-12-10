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

  virtual ~NetworkMatcher();

  virtual void iterate() = 0;

  virtual void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2) = 0;

  virtual QList<NetworkEdgeScorePtr> getAllEdgeScores() const = 0;

  virtual QList<NetworkVertexScorePtr> getAllVertexScores() const = 0;

protected:
  HilbertRTreePtr _edge2Index;
  HilbertRTreePtr _vertex2Index;
  deque<NetworkEdgePtr> _index2Edge;
  deque<NetworkVertexPtr> _index2Vertex;
  ConstOsmMapPtr _map;
  OsmNetworkPtr _n1, _n2;
  NetworkDetailsPtr _details1, _details2;

  void _createEdge2Index();

  void _createVertex2Index();

  IntersectionIterator _createIterator(Envelope env, HilbertRTreePtr tree);

};

}

#endif // NETWORKMATCHER_H
