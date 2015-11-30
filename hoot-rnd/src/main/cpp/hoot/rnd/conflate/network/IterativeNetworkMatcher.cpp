#include "IterativeNetworkMatcher.h"

// Tgs
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/MemoryPageStore.h>

namespace hoot
{

IterativeNetworkMatcher::IterativeNetworkMatcher()
{
}

void IterativeNetworkMatcher::_createEdge2Index()
{
  // No tuning was done, I just copied these settings from OsmMapIndex.
  // 10 children = 368 bytes
  shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
  _edge2Index.reset(new HilbertRTree(mps, 2));

  std::vector<Box> boxes;
  std::vector<int> fids;

  const OsmNetwork::EdgeMap& em = _n2->getEdgeMap();
  for (OsmNetwork::EdgeMap::const_iterator it = em.begin(); it != em.end(); ++it)
  {
    fids.push_back((int)_index2Edge.size());
    _index2Edge.push_back(it.value());

    Box b(2);
    Meters searchRadius = _details2->getSearchRadius(it.value());
    Envelope env(_details2->getEnvelope(it.value()));
    env.expandBy(searchRadius);

    b.setBounds(0, env.getMinX(), env.getMaxX());
    b.setBounds(1, env.getMinY(), env.getMaxY());

    boxes.push_back(b);
  }

  _edge2Index->bulkInsert(boxes, fids);
}

void IterativeNetworkMatcher::_createVertex2Index()
{
  // No tuning was done, I just copied these settings from OsmMapIndex.
  // 10 children = 368 bytes
  shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
  _vertex2Index.reset(new HilbertRTree(mps, 2));

  std::vector<Box> boxes;
  std::vector<int> fids;

  const OsmNetwork::VertexMap& vm = _n2->getVertexMap();
  for (OsmNetwork::VertexMap::const_iterator it = vm.begin(); it != vm.end(); ++it)
  {
    fids.push_back((int)_index2Vertex.size());
    _index2Vertex.push_back(it.value());

    Box b(2);
    Meters searchRadius = _details2->getSearchRadius(it.value());
    Envelope env(_details2->getEnvelope(it.value()));
    env.expandBy(searchRadius);

    b.setBounds(0, env.getMinX(), env.getMaxX());
    b.setBounds(1, env.getMinY(), env.getMaxY());

    boxes.push_back(b);
  }

  _vertex2Index->bulkInsert(boxes, fids);
}

void IterativeNetworkMatcher::matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2)
{
  _n1 = n1;
  _n2 = n2;
  _details1.reset(new NetworkDetails(map, n1));
  _details2.reset(new NetworkDetails(map, n2));

  // create a spatial index of n2 vertices & edges.
  _createEdge2Index();
  _createVertex2Index();

  // create an initial estimation of node matching based on proximity & way count
  _seedVertexScores();
  // create an initial estimation of edge match based on typical similarity scores
  _seedEdgeScores();

  // while criteria isn't met
  for (int i = 0; i < 10; ++i)
  {
    // create a more refined estimate of vertex match based on the proximity & way count as well
    // as the neighboring edge scores (product of neighboring scores?)
    _updateVertexScores();

    // create a more refined estimate of edge match based on the typical similarity scores and
    // the neighboring node scores (product of neighboring scores?)
    _updateEdgeScores();
  }
}

double IterativeNetworkMatcher::_scoreEdges(ConstNetworkEdgePtr /*e1*/,
  ConstNetworkEdgePtr /*e2*/) const
{
  return 1.0;
}

double IterativeNetworkMatcher::_scoreVertices(ConstNetworkVertexPtr /*e1*/,
  ConstNetworkVertexPtr /*e2*/) const
{
  return 1.0;
}

void IterativeNetworkMatcher::_seedEdgeScores()
{
  // go through all the n1 edges
  const OsmNetwork::EdgeMap& em = _n1->getEdgeMap();
  for (OsmNetwork::EdgeMap::const_iterator it = em.begin(); it != em.end(); ++it)
  {
    // find all the n2 edges that are in range of this one
    Envelope env = _details1->getEnvelope(it.value());
    env.expandBy(_details1->getSearchRadius(it.value()));
    IntersectionIterator iit = _createIterator(env, _vertex2Index);

    while (iit.next())
    {
      NetworkEdgePtr e2 = _index2Edge[iit.getId()];
      double score = _scoreEdges(it.value(), e2);

      if (score > 0)
      {
        _edgeScores[EdgePair(it.value(), e2)] = score;
      }
    }
  }
}

void IterativeNetworkMatcher::_seedVertexScores()
{
  // go through all the n1 vertices
  const OsmNetwork::VertexMap& vm = _n1->getVertexMap();
  for (OsmNetwork::VertexMap::const_iterator it = vm.begin(); it != vm.end(); ++it)
  {
    NetworkVertexPtr v1 = it.value();

    // find all the vertices that are in range of this one
    ConstElementPtr e1 = it.value()->getElement();
    ConstNodePtr n1 = dynamic_pointer_cast<const Node>(e1);
    auto_ptr<Envelope> env(e1->getEnvelope(_map));
    env->expandBy(e1->getCircularError());
    IntersectionIterator iit = _createIterator(*env, _vertex2Index);

    double s1 = n1->getCircularError() / 2.0;

    // set the initial match score to 1 for all candidate matches
    while (iit.next())
    {
      NetworkVertexPtr v2 = _index2Vertex[iit.getId()];
      ConstNodePtr n2 = dynamic_pointer_cast<const Node>(v2->getElement());
      Meters d = n2->toCoordinate().distance(n1->toCoordinate());
      double s2 = n2->getCircularError() / 2.0;

      // calculate the combined standard deviation
      double sigma = sqrt(s1 * s1 + s2 * s2);

      // threshold is standard deviation * 2
      if (d <= sigma * 2)
      {
        VertexPair p(v1, v2);
        _vertexScores[p] = _scoreVertices(v1, v2);
      }
    }
  }
}

void IterativeNetworkMatcher::_updateEdgeScores()
{
  // go through all edge matches

    // get the vertex match for the from node

    // get the vertex match for the to node

    // aggregate the scores of the vertex matches

    // set this edge score to the new aggregated score.

}

void IterativeNetworkMatcher::_updateVertexScores()
{
  // go through all vertex matches

    // get all the neighboring edges for the first vertex.

    // get all the neighboring edges for the second vertex.

    // find the best match of scores between the two sets of edges

    // aggregate the scores between the two sets of edges

    // set this vertexes score to the new aggregated score.
}

}
