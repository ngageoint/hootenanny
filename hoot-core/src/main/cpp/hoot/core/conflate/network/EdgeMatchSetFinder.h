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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef EDGEMATCHSETFINDER_H
#define EDGEMATCHSETFINDER_H

#include "IndexedEdgeMatchSet.h"
#include "NetworkDetails.h"

namespace hoot
{

class EdgeMatchSetFinder
{
public:
  EdgeMatchSetFinder(NetworkDetailsPtr details, IndexedEdgeMatchSetPtr matchSet,
    ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2);

  /**
   * Evaluate edges e1 and e2 for match. If the end vertices don't match then recursively search
   * for potential matches until an end is found.
   */
  void addEdgeMatches(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  void setAddStubsInBothDirections(bool bidirectionalStubs) { _bidirectionalStubs = bidirectionalStubs; }

  void setIncludePartialMatches(bool include) { _includePartialMatches = include; }

private:

  bool _bidirectionalStubs;
  NetworkDetailsPtr _details;
  bool _includePartialMatches;
  IndexedEdgeMatchSetPtr _matchSet;
  ConstOsmNetworkPtr _n1, _n2;
  int _steps;

  bool _addEdgeMatches(ConstEdgeMatchPtr em);

  bool _addEdgeNeighborsToEnd(ConstEdgeMatchPtr em, QSet<ConstNetworkEdgePtr> neighbors1,
    QSet<ConstNetworkEdgePtr> neighbors2);

  bool _addEdgeNeighborsToStart(ConstEdgeMatchPtr em, QSet<ConstNetworkEdgePtr> neighbors1Set,
    QSet<ConstNetworkEdgePtr> neighbors2Set);

  /// @todo delete me if I'm still not in use.
  bool _addPartialMatch(ConstEdgeMatchPtr em);

  /**
   * Finds which subline match between e1 and e2 and then appends those sublines to em as
   * appropriate.
   */
  void _appendMatch(EdgeMatchPtr em, ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  /**
   * Returns all the edges that intersect the given location. If the location isn't on a vertex
   * then an empty set is returned.
   */
  QSet<ConstNetworkEdgePtr> _getEdgesFromLocation(ConstEdgeLocationPtr l) const;

  bool _hasConfidentTiePoint(ConstEdgeLocationPtr l) const;

  bool _isCandidateMatch(ConstEdgeLocationPtr l1, ConstEdgeLocationPtr l2) const;

  /**
   * Finds which subline match between e1 and e2 and then prepends those sublines to em as
   * appropriate.
   */
  void _prependMatch(EdgeMatchPtr em, ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  /**
   * Record the match if the score threshold is met.
   */
  bool _recordMatch(ConstEdgeMatchPtr em);

  double _scoreMatch(ConstEdgeMatchPtr em) const;

  ConstEdgeSublinePtr _snapSublineToString(ConstEdgeStringPtr str, ConstEdgeSublinePtr sub) const;

  /// @todo delete me if I'm still not in use.
  EdgeMatchPtr _trimFromEdge(ConstEdgeMatchPtr em);

  /// @todo delete me if I'm still not in use.
  EdgeMatchPtr _trimToEdge(ConstEdgeMatchPtr em);
};

typedef boost::shared_ptr<EdgeMatchSetFinder> EdgeMatchSetFinderPtr;
typedef boost::shared_ptr<const EdgeMatchSetFinder> ConstEdgeMatchSetFinderPtr;

// not implemented
bool operator<(ConstEdgeMatchSetFinderPtr, ConstEdgeMatchSetFinderPtr);

}

#endif // EDGEMATCHSETFINDER_H
