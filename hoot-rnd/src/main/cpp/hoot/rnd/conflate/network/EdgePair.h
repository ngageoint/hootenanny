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
#ifndef __EDGE_PAIR_H__
#define __EDGE_PAIR_H__

#include "NetworkEdge.h"
#include "NetworkVertex.h"

namespace hoot
{

/**
 * This class is not re-entrant or thread safe.
 */
class EdgePair
{
public:
  EdgePair(ConstNetworkEdgePtr ep1, ConstNetworkEdgePtr ep2);
  ConstNetworkVertexPtr getFrom1() const { return e1->getFrom(); }
  ConstNetworkVertexPtr getTo1() const { return e1->getTo(); }

  /**
   * Returns from for the second edge accounting for the reversal. E.g. getFrom1() will always
   * match to getFrom2().
   */
  ConstNetworkVertexPtr getFrom2() const;

  /**
   * Returns from for the second edge accounting for the reversal. E.g. getTo1() will always
   * match to getTo2().
   */
  ConstNetworkVertexPtr getTo2() const;

  QString toString() const;

  /// @todo make these private
  ConstNetworkEdgePtr e1;
  ConstNetworkEdgePtr e2;
  bool reversed;
  static int count;
  int id;
};

typedef shared_ptr<EdgePair> EdgePairPtr;
typedef shared_ptr<const EdgePair> ConstEdgePairPtr;

inline uint qHash(const EdgePairPtr& v) { return qHash(v->e1) ^ qHash(v->e2); }
inline bool operator==(const EdgePairPtr& v1, const EdgePairPtr& v2)
{
  return *(v1->e1) == *(v2->e1) && *(v1->e2) == *(v2->e2);
}

}

#endif // __EDGE_PAIR_H__
