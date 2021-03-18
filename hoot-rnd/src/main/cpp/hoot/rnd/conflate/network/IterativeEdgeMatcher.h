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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __ITERATIVE_EDGE_MATCHER_H__
#define __ITERATIVE_EDGE_MATCHER_H__

// hoot
#include <hoot/core/algorithms/optimizer/SingleAssignmentProblemSolver.h>
#include <hoot/core/conflate/network/NetworkEdgeScore.h>
#include <hoot/core/conflate/network/NetworkVertexScore.h>
#include <hoot/core/conflate/network/OsmNetwork.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/rnd/conflate/network/VagabondNetworkMatcher.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/IntersectionIterator.h>

namespace hoot
{

/**
 * This approach iteratively improves scores based on edge scores. It is more restrictive than
 * the IterativeNetworkMatcher by looking at possible edge paths not just vertex connectivity.
 */
class IterativeEdgeMatcher : public VagabondNetworkMatcher
{
public:

  IterativeEdgeMatcher();
  virtual ~IterativeEdgeMatcher() = default;

  void iterate();

  void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2);

  QList<NetworkEdgeScorePtr> getAllEdgeScores() const;

  QList<NetworkVertexScorePtr> getAllVertexScores() const;

protected:
  virtual double _scoreEdges(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  virtual double _scoreVertices(ConstNetworkVertexPtr e1, ConstNetworkVertexPtr e2) const;

private:

  class DirectedEdgeScore
  {
  public:

    DirectedEdgeScore() : score(0), reversed(false) {}
    DirectedEdgeScore(double s, bool r) :
      score(s),
      reversed(r)
    {}

    QString toString() const { return QString("score: %1, reversed: %2").arg(score).arg(reversed); }

    double score;
    bool reversed;
  };

  using Saps = SingleAssignmentProblemSolver<ConstNetworkEdgePtr, ConstNetworkEdgePtr>;

  /// [row][col]
  using EdgeScoreMap = QHash<ConstNetworkEdgePtr, QHash<ConstNetworkEdgePtr, DirectedEdgeScore>>;
  /// [row][col]
  using VertexScoreMap = QHash<ConstNetworkVertexPtr, QHash<ConstNetworkVertexPtr, double>>;

  /**
   * A cost function used to compare network edges. It is a simple lookup.
   */
  class CostFunction :
    public SingleAssignmentProblemSolver<ConstNetworkEdgePtr, ConstNetworkEdgePtr>::CostFunction
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

    virtual ~CostFunction() = default;

    /**
     * Returns the cost associated with assigning actor a to task t.
     */
    virtual double cost(const ConstNetworkEdgePtr* e1,
                        const ConstNetworkEdgePtr* e2) const
    {
      assert((*em1)[*e1][*e2].reversed == (*em2)[*e2][*e1].reversed);
      double result = 0.0;
      bool reversed = (*em1)[*e1][*e2].reversed;
      bool valid = false;
      if (reversed)
      {
        // if this edge is reversed, and the vertices are reversed
        if (((*e1)->getFrom() == v1 && (*e2)->getTo() == v2) ||
          ((*e1)->getTo() == v1 && (*e2)->getFrom() == v2))
        {
          valid = true;
        }
      }
      else
      {
        // if this is not reversed and the vertices aren't reversed
        if (((*e1)->getFrom() == v1 && (*e2)->getFrom() == v2) ||
          ((*e1)->getTo() == v1 && (*e2)->getTo() == v2))
        {
          valid = true;
        }
      }

      if (valid)
      {
        result = pow((*em1)[*e1][*e2].score * (*em2)[*e2][*e1].score, p);
      }

      return result;
    }
  };

  IndexedEdgeLinks _links;
  IndexedEdgeMatchSetPtr _pr;

  EdgeScoreMap _edge12Scores, _edge21Scores;
  VertexScoreMap _vertex12Scores, _vertex21Scores;
  /// P modifies the aggressiveness of the algorithm. Higher is more aggressive.
  double _p;
  double _dampening;

  double _aggregateScores(QList<double> pairs);

  double _calculateEdgeVertexScore(const VertexScoreMap& vm, ConstNetworkVertexPtr from1,
    ConstNetworkVertexPtr from2, ConstNetworkVertexPtr to1, ConstNetworkVertexPtr to2) const;

  QList<ConstNetworkEdgePtr> _getEdgesOnVertex(ConstNetworkVertexPtr v);

  void _normalizeAllScores();

  /**
   * Normalizes the scores in a table. All the weights will sum to a constant based on the network
   * size. All values will be treated as at least EPSILON for normalizing purposes.
   */
  void _normalizeGlobalScores(EdgeScoreMap& t);
  void _normalizeGlobalScores(VertexScoreMap& t);

  /**
   * Normalizes the scores in a table. All the columns in a given row will sum to 1. All values will
   * be treated as at least EPSILON for normalizing purposes.
   */
  void _normalizeScores(EdgeScoreMap& t);
  void _normalizeScores(VertexScoreMap& t);

  void _seedEdgeScores();

  void _seedVertexScores();

  void _updateEdgeScores(EdgeScoreMap &em, const VertexScoreMap &vm);

  void _updateVertexScores(VertexScoreMap& vm, EdgeScoreMap &em);

};

}

#endif // __ITERATIVE_EDGE_MATCHER_H__
