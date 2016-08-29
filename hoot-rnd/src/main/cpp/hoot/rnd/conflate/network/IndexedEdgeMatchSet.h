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
#ifndef __INDEXED_EDGE_MATCH_SET_H__
#define __INDEXED_EDGE_MATCH_SET_H__

#include "EdgeMatchSet.h"
#include "IndexedEdgeLinks.h"

namespace hoot
{

/**
 * Contains an indexed set of EdgeMatches. This allows efficient retrieval of specific matches based
 * on various criteria.
 */
class IndexedEdgeMatchSet : public EdgeMatchSet
{
public:
  typedef QHash<ConstEdgeMatchPtr, double> MatchHash;

  IndexedEdgeMatchSet();
  
  /**
   * The edge match should not be modified after it has been added to the index.
   */
  void addEdgeMatch(const ConstEdgeMatchPtr& em, double score);

  shared_ptr<IndexedEdgeLinks> calculateEdgeLinks();

  shared_ptr<IndexedEdgeMatchSet> clone() const;

  /**
   * Returns true if the specified element (or the reversed equivalent) is contained in this set.
   */
  virtual bool contains(const ConstEdgeMatchPtr &em) const;

  const MatchHash& getAllMatches() const { return _matches; }
  MatchHash& getAllMatches() { return _matches; }

  QSet<ConstEdgeMatchPtr> getMatchesThatContain(ConstNetworkEdgePtr e) const;

  QSet<ConstEdgeMatchPtr> getMatchesThatTerminateAt(ConstNetworkVertexPtr v) const;

  /**
   * Returns all edges that contain v as an interior vertex (not at an extreme)
   */
  QSet<ConstEdgeMatchPtr> getMatchesWithInteriorVertex(ConstNetworkVertexPtr v) const;

  /**
   * Return all matches that overlap with e. This may include e.
   */
  QSet<ConstEdgeMatchPtr> getMatchesThatOverlap(ConstEdgeMatchPtr e) const;

  /**
   * Return all matches that overlap with str.
   */
  QSet<ConstEdgeMatchPtr> getMatchesThatOverlap(ConstEdgeStringPtr str) const;

  /**
   * Return all the edges that either start at v1/v2 or end at v1/v2.
   */
  QSet<ConstEdgeMatchPtr> getMatchesWithTermination(ConstNetworkVertexPtr v1,
    ConstNetworkVertexPtr v2) const;

  /**
   * This is handy if you want to de-duplicate edge matches. The equivalent stored pointer will be
   * returned if it exists.
   *
   * An exception is thrown if the match doesn't exist.
   */
  ConstEdgeMatchPtr getMatch(const ConstEdgeMatchPtr &em) const;

  /**
   * Return the score associated with an edge match.
   */
  double getScore(ConstEdgeMatchPtr em) const { return _matches[em]; }

  int getSize() const { return _matches.size(); }

  /**
   * Returns a set of the connecting stubs if a and b are connected to the same stub, and that stub
   * allows the edges to be implicitly connected.
   *
   * E.g.
   *
   * *---v----*--w--*----x-----* network 1
   * *----u------*------z------* network 2
   *             y
   *
   * In this case match w/y is a stub so match u/v and match x/z are connected through a stub.
   */
  QSet<ConstEdgeMatchPtr> getConnectingStubs(ConstEdgeMatchPtr a, ConstEdgeMatchPtr b) const;

  QSet<ConstEdgeMatchPtr> getConnectingStubs(ConstEdgeLocationPtr ela1, ConstEdgeLocationPtr ela2,
    ConstEdgeLocationPtr elb1, ConstEdgeLocationPtr elb2) const;

  void setScore(ConstEdgeMatchPtr em, double score) { _matches[em] = score; }

  virtual QString toString() const;

private:
  typedef QHash<ConstNetworkEdgePtr, QSet<ConstEdgeMatchPtr> > EdgeToMatchMap;
  typedef QHash<ConstNetworkVertexPtr, QSet<ConstEdgeMatchPtr> > VertexToMatchMap;

  EdgeToMatchMap _edgeToMatch;
  /**
   * Maps from a vertex that touches or is contained by a match to the match.
   */
  VertexToMatchMap _vertexToMatch;
  MatchHash _matches;

  void _addEdgeToMatchMapping(ConstEdgeStringPtr str, const ConstEdgeMatchPtr &em);

  void _addVertexToMatchMapping(ConstEdgeStringPtr str, const ConstEdgeMatchPtr &em);
};

typedef shared_ptr<IndexedEdgeMatchSet> IndexedEdgeMatchSetPtr;
typedef shared_ptr<const IndexedEdgeMatchSet> ConstIndexedEdgeMatchSetPtr;

// not implemented
bool operator<(ConstIndexedEdgeMatchSetPtr, ConstIndexedEdgeMatchSetPtr);

}

#endif // __EDGE_MATCH_SET_INDEX_H__
