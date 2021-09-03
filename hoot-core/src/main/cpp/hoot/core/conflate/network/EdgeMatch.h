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
#ifndef EDGEMATCH_H
#define EDGEMATCH_H

// Tgs
#include <tgs/HashMap.h>

// Hoot
#include <hoot/core/conflate/network/EdgeString.h>

// Qt
#include <QRegExp>

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

  std::shared_ptr<EdgeMatch> clone() const;

  /**
   * Returns true if the specified edge is in either the first or second edge string.
   */
  bool contains(ConstNetworkEdgePtr e) const;
  /**
   * Returns true if string 1 & 2 in other are contained by string 1 & 2 of this.
   */
  bool contains(const std::shared_ptr<const EdgeMatch>& other) const;
  /**
   * Returns true if the specified vertex is in either the first or second edge string.
   */
  bool contains(ConstNetworkVertexPtr v) const;
  bool containsPartial() const { return getString1()->isPartial() || getString2()->isPartial(); }
  bool containsStub() const { return getString1()->isStub() || getString2()->isStub(); }

  /**
   * Returns the number of terminals that do not fall on a vertex. Possible count is 0 to 4.
   */
  int countPartialMatches() const;

  EdgeStringPtr getString1() { return _edges1; }
  EdgeStringPtr getString2() { return _edges2; }
  ConstEdgeStringPtr getString1() const { return _edges1; }
  ConstEdgeStringPtr getString2() const { return _edges2; }

  /**
   * Call this method if the underlying strings have changed.
   */
  void notifyStringChange() const { _resetHash(); }

  /**
   * Returns true if any of the edges in this edge match overlap with other. Overlapping vertices
   * are ignored.
   */
  bool overlaps(const std::shared_ptr<const EdgeMatch>& other) const;

  /**
   * Returns true if the matches have the same EdgeStrings, but possibly with different portions
   */
  bool isVerySimilarTo(const std::shared_ptr<const EdgeMatch>& other) const;

  /**
   * Reverse both edge strings that make up this EdgeMatch.
   */
  void reverse() const { _edges1->reverse(); _edges2->reverse(); _resetHash(); }

  QString getUid() const;

  QString toString() const;

  QString getSimilarityString() const;
  QString getFirstReversedSimilarityString() const;
  QString getSecondReversedSimilarityString() const;

private:

  mutable uint _hash;

  friend uint qHash(const std::shared_ptr<const EdgeMatch>& em);

  EdgeStringPtr _edges1, _edges2;

  static const QRegExp _portionReplaceRegEx;

  void _resetHash() const { _hash = 0; }
};

using EdgeMatchPtr = std::shared_ptr<EdgeMatch>;
using ConstEdgeMatchPtr = std::shared_ptr<const EdgeMatch>;

// not implemented
bool operator<(ConstEdgeMatchPtr, ConstEdgeMatchPtr);
bool operator<(EdgeMatchPtr, EdgeMatchPtr);

// needed for QSet
bool operator==(const ConstEdgeMatchPtr& em1, const ConstEdgeMatchPtr& em2);

inline uint qHash(const ConstEdgeMatchPtr& em)
{
  if (em->_hash == 0)
  {
    em->_hash = qHash(em->getString1()) ^ qHash(em->getString2());
  }
  return em->_hash;
  //return Tgs::cantorPairing(qHash(em->getString1()), qHash(em->getString2()));
  //return qHash(em->toString()) ^ 0xabcdefg;
}

inline uint qHash(const EdgeMatchPtr& em)
{
  return qHash((ConstEdgeMatchPtr)em);
}

}

#endif // EDGEMATCH_H
