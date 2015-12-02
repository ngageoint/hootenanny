#include "IterativeNetworkMatcher.h"

// Tgs
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/MemoryPageStore.h>

namespace hoot
{

const double IterativeNetworkMatcher::EPSILON = 1e-6;

IterativeNetworkMatcher::IterativeNetworkMatcher()
{
}

bool greaterThan(const double& v1, const double& v2)
{
  return v1 > v2;
}

double IterativeNetworkMatcher::_aggregateEdgeScores(QList<double> pairs)
{
  qSort(pairs.begin(), pairs.end(), greaterThan);

  // this quick little method makes the scores: score[0] / 2 + score[1] / 4 + ...
  // the idea being that you get big boost for the highest score than then the benefit goes down
  // quickly. So more intersections matched is better, but having one poorly matched or unmatched
  // intersection won't hurt. We'll see what happens.
  double result = EPSILON;
  double denominator = 0.5;
  for (int i = 0; i < pairs.size(); i++)
  {
    result += pairs[i] * denominator;
    denominator /= 2.0;
  }

  return result;
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
    _updateVertexScores(_vertex12Scores, _edge12Scores);
    _updateVertexScores(_vertex21Scores, _edge21Scores);

    // create a more refined estimate of edge match based on the typical similarity scores and
    // the neighboring node scores (product of neighboring scores?)
    _updateEdgeScores(_edge12Scores, _vertex12Scores);
    _updateEdgeScores(_edge21Scores, _vertex21Scores);
  }
}

void IterativeNetworkMatcher::_normalizeAllScores()
{
  _normalizeScores<ConstNetworkEdgePtr>(_edge12Scores);
  _normalizeScores<ConstNetworkEdgePtr>(_edge21Scores);
  _normalizeScores<ConstNetworkVertexPtr>(_vertex12Scores);
  _normalizeScores<ConstNetworkVertexPtr>(_vertex21Scores);
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
    NetworkEdgePtr e1 = it.value();
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
        _edge12Scores[e1][e2] = score;
        _edge12Scores[e2][e1] = score;
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
    Envelope env = _details1->getEnvelope(v1);
    env.expandBy(_details1->getSearchRadius(v1));
    IntersectionIterator iit = _createIterator(env, _vertex2Index);

    // set the initial match score to 1 for all candidate matches
    while (iit.next())
    {
      NetworkVertexPtr v2 = _index2Vertex[iit.getId()];

      double score = _scoreVertices(v1, v2);
      if (score > 0)
      {
        _vertex12Scores[v1][v2] = score;
        _vertex21Scores[v2][v1] = score;
      }
    }
  }
}

void IterativeNetworkMatcher::_updateEdgeScores(EdgeScoreMap &em, VertexScoreMap& vm)
{
  // go through all edge matches
  for (EdgeScoreMap::iterator it = em.begin(); it != em.end(); ++it)
  {
    ConstNetworkEdgePtr e1 = it.key();

    // get the vertex match for the from node
    ConstNetworkVertexPtr from1 = e1->getFrom();
    // get the vertex match for the to node
    ConstNetworkVertexPtr to1 = e1->getTo();

    QHash<ConstNetworkEdgePtr, double>& t2 = it.value();

    for (QHash<ConstNetworkEdgePtr, double>::iterator jt = t2.begin(); jt != t2.end(); ++jt)
    {
      ConstNetworkEdgePtr e2 = jt.key();
      ConstNetworkVertexPtr from2 = e2->getFrom();

      ConstNetworkVertexPtr to2 = e2->getTo();

      // aggregate the scores of the vertex matches
      double sFrom = vm[from1][from2];
      double sTo = vm[to1][to2];

      // set this edge score to the new aggregated score.
      double newScore = sFrom * sTo;
      jt.value() = newScore;
    }
  }
}

void IterativeNetworkMatcher::_updateVertexScores(VertexScoreMap& vm, EdgeScoreMap &em)
{
  CostFunction cost;
  cost.em = &em;

  // go through all vertex matches
  for (VertexScoreMap::iterator it = vm.begin(); it != vm.end(); ++it)
  {
    ConstNetworkVertexPtr v1 = it.key();

    QList<ConstNetworkEdgePtr> edges1 = _getEdgesOnVertex(v1);

    // Go through all the potential matching vertices
    QHash<ConstNetworkVertexPtr, double>& t2 = it.value();
    for (QHash<ConstNetworkVertexPtr, double>::iterator jt = t2.begin(); jt != t2.end(); ++jt)
    {
      Saps sap(cost);
      ConstNetworkVertexPtr v2 = jt.key();
      QList<ConstNetworkEdgePtr> edges2 = _getEdgesOnVertex(v2);

      // get all the neighboring edges for the first vertex.
      for (int i1 = 0; i1 < edges1.size(); ++i1)
      {
        sap.addActor(&(edges1[i1]));
      }

      // get all the neighboring edges for the second vertex.
      for (int i2 = 0; i2 < edges2.size(); ++i2)
      {
        sap.addActor(&(edges2[i2]));
      }

      // find the best match of scores between the two sets of edges
      vector<Saps::ResultPair> pairing = sap.calculatePairing();
      QList<double> scores;

      for (size_t i = 0; i < pairing.size(); ++i)
      {
        scores[i] = cost.cost(pairing[i].actor, pairing[i].task);
      }

      // aggregate the scores between the two sets of edges
      double edgeScore = _aggregateEdgeScores(scores);

      // set this vertex pair's score to the new aggregated score.
      jt.value() = edgeScore;
    }
  }
}

}
