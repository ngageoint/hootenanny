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
#ifndef ITERATIVENETWORKMATCHER_H
#define ITERATIVENETWORKMATCHER_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/optimizer/SingleAssignmentProblemSolver.h>
#include <hoot/rnd/conflate/network/IndexedEdgeMatchSet.h>
#include <hoot/rnd/conflate/network/NetworkMatcher.h>
#include <hoot/rnd/conflate/network/NetworkEdgeScore.h>
#include <hoot/rnd/conflate/network/NetworkVertexScore.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/IntersectionIterator.h>

namespace hoot
{

using namespace Tgs;

class IterativeNetworkMatcherTest;

/**
 * This approach iteratively improves scores based on neighboring vertex and edge scores.
 *
 * The approach seems to work well most of the time, but suffers from a greedy mentality.
 */
class IterativeNetworkMatcher :
  public NetworkMatcher
{
public:
  static string className() { return "hoot::IterativeNetworkMatcher"; }

  const static double EPSILON;

  /**
   * Always construct with create() to make a shared pointer.
   */
  IterativeNetworkMatcher();

  /**
   * Use this instead of a constructor.
   */
  static shared_ptr<IterativeNetworkMatcher> create();

  void iterate();

  void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2);

  QList<NetworkEdgeScorePtr> getAllEdgeScores() const;

  QList<NetworkVertexScorePtr> getAllVertexScores() const;

protected:
  virtual double _scoreEdges(ConstEdgeMatchPtr em) const;

  virtual double _scoreVertices(ConstNetworkVertexPtr e1, ConstNetworkVertexPtr e2) const;

private:

  // for white box testing.
  friend class IterativeNetworkMatcherTest;

  typedef SingleAssignmentProblemSolver<EdgeString, EdgeString> Saps;

  typedef QHash<ConstEdgeMatchPtr, double> EdgeScoreMap;
  /// [row][col]
  typedef QHash< ConstNetworkVertexPtr, QHash<ConstNetworkVertexPtr, double> > VertexScoreMap;

  /**
   * A cost function used to compare network edges. It is a simple lookup.
   */
  class CostFunction :
    public SingleAssignmentProblemSolver<EdgeString, EdgeString>::CostFunction
  {
  public:
    const EdgeScoreMap* em1;
    const EdgeScoreMap* em2;
    ConstNetworkVertexPtr v1, v2;
    double p;

    CostFunction()
    {
      p = 1.0;
      em1 = 0;
      em2 = 0;
    }

    /**
     * Returns the cost associated with assigning actor a to task t.
     */
    virtual double cost(const EdgeString* e1,
                        const EdgeString* e2) const
    {
      bool valid = (e1->getFromVertex() == v1 && e2->getFromVertex() == v2) ||
        (e1->getToVertex() == v1 && e2->getToVertex() == v2);

      LOG_VAR(valid);
      LOG_VAR(v1);
      LOG_VAR(v2);
      double result = 0.0;

      if (valid)
      {
        ConstEdgeMatchPtr em(new EdgeMatch(e1->clone(), e2->clone()));
        result = pow((*em1)[em] * (*em2)[em], p);
      }

      LOG_VAR(result);

      return result;
    }
  };

  IndexedEdgeMatchSetPtr _edgeMatches;
  EdgeScoreMap _edge12Scores, _edge21Scores;
  VertexScoreMap _vertex12Scores, _vertex21Scores;
  /// The smaller P the less impact the features previous score has on the result. 0 means it has
  /// no impact.
  double _p;
  /// The higher this value the faster the algorithm will converge
  double _dampening;

  double _aggregateScores(QList<double> pairs);

  void _createEmptyStubEdges(OsmNetworkPtr na, OsmNetworkPtr nb);

  void _createStubIntersection(OsmNetworkPtr na, OsmNetworkPtr nb, ConstNetworkVertexPtr va,
    ConstNetworkEdgePtr eb);

  double _calculateEdgeVertexScore(const VertexScoreMap& vm, ConstEdgeLocationPtr from1,
    ConstEdgeLocationPtr from2, ConstEdgeLocationPtr to1, ConstEdgeLocationPtr to2) const;

  QList<ConstNetworkEdgePtr> _getEdgesOnVertex(ConstNetworkVertexPtr v);

  void _normalizeAllScores();

  /**
   * Normalizes the scores in a table. All the weights will sum to a constant based on the network
   * size. All values will be treated as at least EPSILON for normalizing purposes.
   */
  void _normalizeScoresGlobal(EdgeScoreMap& t);
  void _normalizeScoresGlobal(VertexScoreMap& t);

  /**
   * Normalizes the scores in a table. All the columns in a given row will sum to 1. All values will
   * be treated as at least EPSILON for normalizing purposes.
   */
  void _normalizeScoresLocal(EdgeScoreMap& t);
  void _normalizeScoresLocal(VertexScoreMap& t);

  void _seedEdgeScores();

  void _seedVertexScores();

  void _updateEdgeScores(EdgeScoreMap &em, const VertexScoreMap &vm);

  void _updateVertexScores(VertexScoreMap& vm, EdgeScoreMap &em);

};

typedef shared_ptr<IterativeNetworkMatcher> IterativeNetworkMatcherPtr;
typedef shared_ptr<const IterativeNetworkMatcher> ConstIterativeNetworkMatcherPtr;

// not implemented
bool operator<(ConstIterativeNetworkMatcherPtr, ConstIterativeNetworkMatcherPtr);

}

#endif // ITERATIVENETWORKMATCHER_H
