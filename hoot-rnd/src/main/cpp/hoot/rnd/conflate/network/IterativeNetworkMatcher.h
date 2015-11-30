#ifndef ITERATIVENETWORKMATCHER_H
#define ITERATIVENETWORKMATCHER_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/rnd/conflate/network/NetworkDetails.h>
#include <hoot/rnd/conflate/network/NetworkEdgeMatch.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/IntersectionIterator.h>

namespace hoot
{

using namespace Tgs;

class IterativeNetworkMatcher
{
public:
  IterativeNetworkMatcher();

  void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2);

  QList<ConstNetworkEdgeMatchPtr> getEdgeMatches();

protected:
  virtual double _scoreEdges(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  virtual double _scoreVertices(ConstNetworkVertexPtr e1, ConstNetworkVertexPtr e2) const;

private:

  typedef QPair<NetworkEdgePtr, NetworkEdgePtr> EdgePair;
  typedef QPair<NetworkVertexPtr, NetworkVertexPtr> VertexPair;
  typedef QHash<EdgePair, double> EdgeScoreMap;
  typedef QHash<VertexPair, double> VertexScoreMap;

  HilbertRTreePtr _edge2Index;
  deque<NetworkEdgePtr> _index2Edge;
  EdgeScoreMap _edgeScores;
  ConstOsmMapPtr _map;
  OsmNetworkPtr _n1, _n2;
  HilbertRTreePtr _vertex2Index;
  deque<NetworkVertexPtr> _index2Vertex;
  VertexScoreMap _vertexScores;
  NetworkDetailsPtr _details1, _details2;

  void _createEdge2Index();
  void _createVertex2Index();

  IntersectionIterator _createIterator(Envelope env, HilbertRTreePtr tree);

  void _seedEdgeScores();

  void _seedVertexScores();

  void _updateEdgeScores();

  void _updateVertexScores();

};

}

#endif // ITERATIVENETWORKMATCHER_H
