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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef __SINGLE_SIDED_NETWORK_MATCHER_H__
#define __SINGLE_SIDED_NETWORK_MATCHER_H__

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

#include "IndexedEdgeLinks.h"

namespace hoot
{

using namespace Tgs;

class SingleSidedNetworkMatcherTest;

/**
 * Uses the second input to match against the first. Only the second input features are given
 * scores in their matches against the first input. Each edge in the second input is given a
 * probability of match score with all the EdgeMatches it can be part of. At each iteration these
 * probabilities are used to increase/decrease the scores of linked matches.
 *
 * Unfortunately this is quite brittle when attempting to multiply the probabilities. Sometimes
 * too much is conflated, at other times, too little. (_updateEdgeScores())
 *
 * By swapping from using the product to additive (_updateEdgeScoresAdditive()) the results look
 * a little better, but I think the iterative approach was giving a sligthly better result.
 */
class SingleSidedNetworkMatcher :
  public NetworkMatcher
{
public:
  static string className() { return "hoot::SingleSidedNetworkMatcher"; }

  const static double EPSILON;

  /**
   * Always construct with create() to make a shared pointer.
   */
  SingleSidedNetworkMatcher();

  /**
   * Use this instead of a constructor.
   */
  static shared_ptr<SingleSidedNetworkMatcher> create();

  void iterate();

  void matchNetworks(ConstOsmMapPtr map, OsmNetworkPtr n1, OsmNetworkPtr n2);

  QList<NetworkEdgeScorePtr> getAllEdgeScores() const;

  QList<NetworkVertexScorePtr> getAllVertexScores() const;

protected:
  virtual double _scoreEdges(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  virtual double _scoreVertices(ConstNetworkVertexPtr e1, ConstNetworkVertexPtr e2) const;

private:

  // for white box testing.
  friend class SingleSidedNetworkMatcherTest;

  class EdgeLinkScore
  {
  public:

    EdgeLinkScore() : score(0) {}
    EdgeLinkScore(ConstEdgeMatchPtr m, double s) :
      match(m),
      score(s)
    {}

    QString toString() const { return QString("match: %1 score: %2").arg(match->toString()).
      arg(score); }

    ConstEdgeMatchPtr match;
    double score;
  };

  typedef shared_ptr<EdgeLinkScore> EdgeLinkScorePtr;

  typedef SingleAssignmentProblemSolver<ConstNetworkEdgePtr, ConstNetworkEdgePtr> Saps;

  /// [v2]
  typedef QHash<ConstNetworkEdgePtr, QList<EdgeLinkScorePtr> > EdgeMatchScoreMap;
  /// [v2][v1]
  typedef QHash< ConstNetworkVertexPtr, QHash<ConstNetworkVertexPtr, double> > VertexScoreMap;

  IndexedEdgeMatchSetPtr _edgeMatches;
  EdgeMatchScoreMap _edge2Scores;
  ConstIndexedEdgeLinksPtr _links;
  VertexScoreMap _vertex21Scores;
  /// dampening modifies the aggressiveness of the algorithm. Higher is more aggressive.
  /// EPSILON <= _dampening <= 1.0
  double _dampening;
  double _w;

  double _calculateProbabilityOfMatch(ConstEdgeMatchPtr e) const;

  double _calculateProbabilityOfMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const;

  double _getScore(ConstNetworkEdgePtr e2, ConstEdgeMatchPtr em) const;

  void _normalizeScores();

  void _seedEdgeScores();

  void _seedVertexScores();

  void _updateEdgeScores();

  void _updateEdgeScoresAdditive();

  void _updateVertexScores();

};

typedef shared_ptr<SingleSidedNetworkMatcher> SingleSidedNetworkMatcherPtr;
typedef shared_ptr<const SingleSidedNetworkMatcher> ConstSingleSidedNetworkMatcherPtr;

// not implemented
bool operator<(ConstSingleSidedNetworkMatcherPtr, ConstSingleSidedNetworkMatcherPtr);

}

#endif // __SINGLE_SIDED_NETWORK_MATCHER_H__
