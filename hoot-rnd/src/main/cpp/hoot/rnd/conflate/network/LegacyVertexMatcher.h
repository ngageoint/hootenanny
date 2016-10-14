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
#ifndef LEGACYVERTEXMATCHER_H
#define LEGACYVERTEXMATCHER_H

// hoot
#include <hoot/core/conflate/NodeMatcher.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>
#include <hoot/rnd/conflate/network/SearchRadiusProvider.h>

// Qt
#include <QList>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/IntersectionIterator.h>

namespace hoot
{

/**
 * Identifies both candidate and confident tie points. Tie points are intersections that are joined
 * together.
 *
 * A confident tie point is a point that we are confident has only one possible match and that match
 * is solid.
 *
 * A candidate tie point is simply that, a candidate. It will have a score, but it may or may not
 * be a tie point.
 */
class LegacyVertexMatcher
{
public:
  class TiePointScore
  {
  public:
    ConstNetworkVertexPtr v1, v2;
    double rawScore;

    TiePointScore(ConstNetworkVertexPtr iv1, ConstNetworkVertexPtr iv2, double score) :
      v1(iv1),
      v2(iv2),
      rawScore(score)
    {
    }

    QString toString() const
    {
      return QString("{ v1: \"%1\", v2: \"%2\", rawScore: %3)").arg(v1->toString())
        .arg(v2->toString()).arg(rawScore);
    }
  };

  typedef shared_ptr<TiePointScore> TiePointScorePtr;

  LegacyVertexMatcher(ConstOsmMapPtr map);

  QList<ConstNetworkVertexPtr> getCandidateMatches(ConstNetworkVertexPtr v) const;

  bool hasConfidentTiePoint(ConstNetworkVertexPtr v) const { return _hasConfidentTie.contains(v); }

  /**
   * Returns false if we're confident this is not a tie point, otherwise returns true.
   */
  bool isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2,
    SearchRadiusProvider& srp);

  /**
   * Process n1 and n2 to identify candidate and confident tie points.
   */
  void identifyVertexMatches(ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2,
    SearchRadiusProvider& srp);

  /**
   * Returns true if this is a confident tie point. If this is true then scoreTiePoint should return
   * 0 for every other combination that includes either v1 or v2.
   */
  bool isConfidentTiePoint(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);

  /**
   * Returns a score for v1 and v2. Bigger is better, but you can't really derive any other meaning
   * from the score (e.g. 2 isn't necessarily twice as good as 1).
   */
  double scoreMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);
private:

  ConstOsmMapPtr _map;
  OsmNetworkPtr _n1;
  OsmNetworkPtr _n2;
  Tgs::HilbertRTreePtr _vertex2Index;
  QList<ConstNetworkVertexPtr> _vertex2Fid;
  NodeMatcherPtr _nodeMatcher;
  // This approach is a little data structure heavy and there is certainly a more clever, faster
  // and more memory efficient approach, but I'm not sure it is warranted. This would actually
  // be kinda fun to optimize if it every becomes a bottle neck (doubtful).
  QMap< ConstNetworkVertexPtr, QList<TiePointScorePtr> > _scores1, _scores2;
  QMap< ConstNetworkVertexPtr, QMap<ConstNetworkVertexPtr, double> > _finalScores;
  // Reverse and forward ordering of _finalScores to make getCandidateMatches faster.
  // QMap< v, QList<v> >
  QMap< ConstNetworkVertexPtr, QList<ConstNetworkVertexPtr> > _finalCandidates;
  QSet<ConstNetworkVertexPtr> _hasConfidentTie;

  double _confidentThreshold;

  Tgs::IntersectionIterator _createIterator(Envelope env);

  void _createVertexIndex(const OsmNetwork::VertexMap& vm, SearchRadiusProvider &srp);

  /**
   * Returns the sum of all scores that involve either of the vertices in tie.
   */
  double _denominatorForTie(TiePointScorePtr tie);

  NodeMatcherPtr _getNodeMatcher();

  /**
   * Scores a single pair of vertices (doesn't consider neighbors)
   */
  double _scoreSinglePair(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);

  void _balanceVertexScores();
};

/**
 * Uses v1/v2 for comparison. This avoids using memory location for comparison which can cause
 * inconsistent output.
 */
inline bool operator<(const LegacyVertexMatcher::TiePointScorePtr& t1,
  const LegacyVertexMatcher::TiePointScorePtr& t2)
{
  if (t1->v1->getElementId() < t2->v1->getElementId())
  {
    return true;
  }
  else if (t1->v1->getElementId() == t2->v1->getElementId())
  {
    return t1->v2->getElementId() < t2->v2->getElementId();
  }

  return false;
}

inline uint qHash(const LegacyVertexMatcher::TiePointScorePtr& t)
{
  return qHash(std::pair<ElementId, ElementId>(t->v1->getElementId(), t->v2->getElementId()));
}

typedef shared_ptr<LegacyVertexMatcher> LegacyVertexMatcherPtr;
typedef shared_ptr<const LegacyVertexMatcher> ConstLegacyVertexMatcherPtr;

// not implemented
bool operator<(ConstLegacyVertexMatcherPtr, ConstLegacyVertexMatcherPtr);

}

#endif // LEGACYVERTEXMATCHER_H
