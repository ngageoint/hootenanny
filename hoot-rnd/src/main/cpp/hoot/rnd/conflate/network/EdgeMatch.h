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
#ifndef EDGEMATCH_H
#define EDGEMATCH_H

#include <tgs/HashMap.h>

#include "EdgeString.h"

namespace hoot
{

/**
 * Represents the match between two sets of edges. This could represent:
 *
 * - a 1:1 match where all of edge1 matches all of edge2
 * - a n:m match where all the edges in n combine to match all the edges in m
 * - a partial match where all or some of edge1 matches all or some of edge2
 * - This will always be a contiguous match between edges1 and edges2
 */
class EdgeMatch
{
public:
  EdgeMatch();

  EdgeMatch(ConstEdgeStringPtr es1, ConstEdgeStringPtr es2);

  shared_ptr<EdgeMatch> clone() const;

  /**
   * Returns true if the specified edge is in either the first or second edge string.
   */
  bool contains(ConstNetworkEdgePtr e) const;

  /**
   * Returns true if string 1 & 2 in other are contained by string 1 & 2 of this.
   */
  bool contains(const shared_ptr<const EdgeMatch>& other) const;

  /**
   * Returns true if the specified vertex is in either the first or second edge string.
   */
  bool contains(ConstNetworkVertexPtr v) const;

  bool containsPartial() const { return getString1()->isPartial() || getString2()->isPartial(); }

  bool containsStub() const { return getString1()->isStub() || getString2()->isStub(); }

  EdgeStringPtr getString1() { return _edges1; _resetHash(); }

  EdgeStringPtr getString2() { return _edges2; _resetHash(); }

  ConstEdgeStringPtr getString1() const { return _edges1; }

  ConstEdgeStringPtr getString2() const { return _edges2; }

  /**
   * Call this method if the underlying strings have changed.
   */
  void notifyStringChange() { _resetHash(); }

  /**
   * Returns true if any of the edges in this edge match overlap with other. Overlapping vertices
   * are ignored.
   */
  bool overlaps(const shared_ptr<const EdgeMatch>& other) const;

  /**
   * Reverse both edge strings that make up this EdgeMatch.
   */
  void reverse() const { _edges1->reverse(); _edges2->reverse(); _resetHash(); }

  QString toString() const;

private:
  mutable uint _hash;

  friend uint qHash(const shared_ptr<const EdgeMatch>& em);

  EdgeStringPtr _edges1, _edges2;

  void _resetHash() const { _hash = 0; }
};

typedef shared_ptr<EdgeMatch> EdgeMatchPtr;
typedef shared_ptr<const EdgeMatch> ConstEdgeMatchPtr;

// not implemented
bool operator<(ConstEdgeMatchPtr, ConstEdgeMatchPtr);
bool operator<(EdgeMatchPtr, EdgeMatchPtr);

// needed for QSet
bool operator==(const hoot::ConstEdgeMatchPtr& em1, const hoot::ConstEdgeMatchPtr& em2);

inline uint qHash(const hoot::ConstEdgeMatchPtr& em)
{
  if (em->_hash == 0)
  {
    em->_hash = qHash(em->getString1()) ^ qHash(em->getString2());
  }
  return em->_hash;
  //return Tgs::cantorPairing(qHash(em->getString1()), qHash(em->getString2()));
  //return qHash(em->toString()) ^ 0xabcdefg;
}

inline uint qHash(const hoot::EdgeMatchPtr& em)
{
  return qHash((hoot::ConstEdgeMatchPtr)em);
}

}

#endif // EDGEMATCH_H
