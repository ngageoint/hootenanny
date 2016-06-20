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

namespace hoot
{

class IndexedEdgeMatchSet : public EdgeMatchSet
{
public:
  typedef QHash<EdgeMatchPtr, double> MatchHash;

  IndexedEdgeMatchSet();
  
  void addEdgeMatch(const EdgeMatchPtr& em, double score);

  QList<ConstEdgeMatchPtr> getIntersectingMatches(const ConstEdgeMatchPtr& em);

  shared_ptr<IndexedEdgeMatchSet> clone() const;

  /**
   * Returns true if the specified element (or the reversed equivalent) is contained in this set.
   */
  virtual bool contains(const EdgeMatchPtr &em) const;

  const MatchHash& getAllMatches() const { return _matches; }
  MatchHash& getAllMatches() { return _matches; }

  /**
   * Return all the edges that either start at v1/v2 or end at v1/v2.
   */
  QSet<EdgeMatchPtr> getMatchesWithTermination(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
    const;

  /**
   * This is handy if you want to de-duplicate edge matches. The equivalent stored pointer will be
   * returned if it exists.
   *
   * An exception is thrown if the match doesn't exist.
   */
  EdgeMatchPtr getMatch(const EdgeMatchPtr &em) const;

  /**
   * Return the score associated with an edge match.
   */
  double getScore(EdgeMatchPtr em) const { return _matches[em]; }

  int getSize() const { return _matches.size(); }

  void setScore(EdgeMatchPtr em, double score) { _matches[em] = score; }

  virtual QString toString() const;

private:
  MatchHash _matches;
};

typedef shared_ptr<IndexedEdgeMatchSet> IndexedEdgeMatchSetPtr;
typedef shared_ptr<const IndexedEdgeMatchSet> ConstIndexedEdgeMatchSetPtr;

}

#endif // __EDGE_MATCH_SET_INDEX_H__
