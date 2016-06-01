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

  EdgeStringPtr getString1() { return _edges1; }

  EdgeStringPtr getString2() { return _edges2; }

  ConstEdgeStringPtr getString1() const { return _edges1; }

  ConstEdgeStringPtr getString2() const { return _edges2; }

  shared_ptr<EdgeMatch> clone() const;

  /**
   * Returns true if the specified edge is in either the first or second edge string.
   */
  bool contains(ConstNetworkEdgePtr e) const;

  /**
   * Returns true if any of the edges in this edge match overlap with other. Overlapping vertexes
   * are ignored.
   */
  bool overlaps(const shared_ptr<const EdgeMatch>& other) const;

  /**
   * Reverse both edge strings that make up this EdgeMatch.
   */
  void reverse() const { _edges1->reverse(); _edges2->reverse(); }

  QString toString() const;

private:

  EdgeStringPtr _edges1, _edges2;
};

typedef shared_ptr<EdgeMatch> EdgeMatchPtr;
typedef shared_ptr<const EdgeMatch> ConstEdgeMatchPtr;

// needed for QSet
bool operator==(const hoot::EdgeMatchPtr& em1, const hoot::EdgeMatchPtr& em2);
uint qHash(const hoot::EdgeMatchPtr& em);

}

#endif // EDGEMATCH_H
