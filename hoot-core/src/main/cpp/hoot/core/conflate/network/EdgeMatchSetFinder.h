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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef EDGEMATCHSETFINDER_H
#define EDGEMATCHSETFINDER_H

#include <hoot/core/conflate/network/IndexedEdgeMatchSet.h>
#include <hoot/core/conflate/network/NetworkDetails.h>

namespace hoot
{

struct EdgeMatchScore
{
  ConstEdgeMatchPtr match;
  double score = -1.0; //default score to indicate invalid instance
};

// Stores a set of edge matches, keyed by a similarity string; see EdgeMatch::isVerySimilarTo
using EdgeMatchSimilarity = QHash<QString, EdgeMatchScore>;

class EdgeMatchSetFinder
{
public:

  static const QString EDGE_MATCH_SIMILAR_KEY;
  static const QString EDGE_MATCH_SIMILAR_FIRST_REVERSED_KEY;
  static const QString EDGE_MATCH_SIMILAR_SECOND_REVERSED_KEY;

  EdgeMatchSetFinder(NetworkDetailsPtr details, IndexedEdgeMatchSetPtr matchSet,
    ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2);

  /**
   * Evaluate edges e1 and e2 for match. If the end vertices don't match then recursively search
   * for potential matches until an end is found.
   */
  void addEdgeMatches(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  void setAddStubsInBothDirections(bool bidirectionalStubs)
  { _bidirectionalStubs = bidirectionalStubs; }

  void setIncludePartialMatches(bool include) { _includePartialMatches = include; }

  int getNumSimilarEdgeMatches() const { return _numSimilarEdgeMatches; }

private:

  bool _bidirectionalStubs;
  NetworkDetailsPtr _details;
  bool _includePartialMatches;
  IndexedEdgeMatchSetPtr _matchSet;
  ConstOsmNetworkPtr _n1, _n2;
  int _steps;
  int _maxSteps;

  int _numSimilarEdgeMatches;
  // stores three categories of EdgeMatchSimilarity; one with both edges in the regular direction,
  // one with the first edge in the regular direction and the second reversed, and one with the
  // second edge in the regular direction and the first reversed; see EdgeMatch::isVerySimilarTo
  QMap<QString, EdgeMatchSimilarity> _edgeMatchSimilarities;

  bool _addEdgeMatches(ConstEdgeMatchPtr em);

  bool _addEdgeNeighborsToEnd(ConstEdgeMatchPtr em, QSet<ConstNetworkEdgePtr> neighbors1,
    QSet<ConstNetworkEdgePtr> neighbors2);

  bool _addEdgeNeighborsToStart(ConstEdgeMatchPtr em, QSet<ConstNetworkEdgePtr> neighbors1Set,
    QSet<ConstNetworkEdgePtr> neighbors2Set);

  /**
   * Returns all the edges that intersect the given location. If the location isn't on a vertex
   * then an empty set is returned.
   */
  QSet<ConstNetworkEdgePtr> _getEdgesFromLocation(ConstEdgeLocationPtr l) const;

  bool _hasConfidentTiePoint(ConstEdgeLocationPtr l) const;

  bool _isCandidateMatch(ConstEdgeLocationPtr l1, ConstEdgeLocationPtr l2) const;

  /**
   * Record the match if the score threshold is met.
   */
  bool _recordMatch(ConstEdgeMatchPtr em);

  double _scoreMatch(ConstEdgeMatchPtr em) const;

  void _addReverseMatch(ConstEdgeMatchPtr edgeMatch, const double score) const;

  void _resetEdgeMatchSimilarities();
};

using EdgeMatchSetFinderPtr = std::shared_ptr<EdgeMatchSetFinder>;
using ConstEdgeMatchSetFinderPtr = std::shared_ptr<const EdgeMatchSetFinder>;

}

#endif // EDGEMATCHSETFINDER_H
