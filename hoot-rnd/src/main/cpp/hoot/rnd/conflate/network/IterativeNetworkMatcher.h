#ifndef ITERATIVENETWORKMATCHER_H
#define ITERATIVENETWORKMATCHER_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/SingleAssignmentProblemSolver.h>
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
  const static double EPSILON;

  IterativeNetworkMatcher();

  void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2);

  QList<ConstNetworkEdgeMatchPtr> getEdgeMatches();

protected:
  virtual double _scoreEdges(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  virtual double _scoreVertices(ConstNetworkVertexPtr e1, ConstNetworkVertexPtr e2) const;

private:

  /// [row][col]
  typedef QHash< ConstNetworkEdgePtr, QHash<ConstNetworkEdgePtr, double> > EdgeScoreMap;
  /// [row][col]
  typedef QHash< ConstNetworkVertexPtr, QHash<ConstNetworkVertexPtr, double> > VertexScoreMap;
  typedef SingleAssignmentProblemSolver<ConstNetworkEdgePtr, ConstNetworkEdgePtr> Saps;

  /**
   * A cost function used to compare network edges. It is a simple lookup.
   */
  class CostFunction :
    public SingleAssignmentProblemSolver<ConstNetworkEdgePtr, ConstNetworkEdgePtr>::CostFunction
  {
  public:
    const EdgeScoreMap* em;

    /**
     * Returns the cost associated with assigning actor a to task t.
     */
    virtual double cost(const ConstNetworkEdgePtr* e1,
                        const ConstNetworkEdgePtr* e2) const
    {
      return (*em)[*e1][*e2];
    }
  };

  HilbertRTreePtr _edge2Index;
  deque<NetworkEdgePtr> _index2Edge;
  EdgeScoreMap _edge12Scores, _edge21Scores;
  ConstOsmMapPtr _map;
  OsmNetworkPtr _n1, _n2;
  HilbertRTreePtr _vertex2Index;
  deque<NetworkVertexPtr> _index2Vertex;
  VertexScoreMap _vertex12Scores, _vertex21Scores;
  NetworkDetailsPtr _details1, _details2;

  double _aggregateEdgeScores(QList<double> pairs);

  void _createEdge2Index();
  void _createVertex2Index();

  IntersectionIterator _createIterator(Envelope env, HilbertRTreePtr tree);

  QList<ConstNetworkEdgePtr> _getEdgesOnVertex(ConstNetworkVertexPtr v);

  void _normalizeAllScores();

  /**
   * Normalizes the scores in a table. All the columns in a given row will sum to 1. All values will
   * be treated as at least EPSILON for normalizing purposes.
   */
  template<typename T>
  void _normalizeScores(QHash< T, QHash<T, double> >& t)
  {
    typedef QHash< T, QHash<T, double> > Table;
    for (typename Table::iterator it = t.begin(); it != t.end(); ++it)
    {
      double sum = 0.0;
      QHash<T, double>& t2 = it.value();
      for (typename QHash<T, double>::iterator jt = t2.begin(); jt != t2.end(); ++jt)
      {
        sum += std::max(EPSILON, jt.value());
      }

      for (typename QHash<T, double>::iterator jt = t2.begin(); jt != t2.end(); ++jt)
      {
        jt.value() = std::max(EPSILON, jt.value()) / sum;
      }
    }
  }

  void _seedEdgeScores();

  void _seedVertexScores();

  void _updateEdgeScores(EdgeScoreMap &em, VertexScoreMap& vm);

  void _updateVertexScores(VertexScoreMap& vm, EdgeScoreMap &em);

};

}

#endif // ITERATIVENETWORKMATCHER_H
