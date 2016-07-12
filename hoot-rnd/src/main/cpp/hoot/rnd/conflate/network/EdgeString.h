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
#ifndef EDGESTRING_H
#define EDGESTRING_H

// hoot
#include <hoot/core/algorithms/linearreference/WayString.h>

#include "NetworkEdge.h"
#include "NetworkVertex.h"

namespace hoot
{

/**
 * Contains one or more ordered edges that are connected together into a string. Each edge may
 * contain just a partial or an entire edge as long as the pieces all connect.
 *
 * If the EdgeString is a stub it may only contain one edge and that edge must be a stub. An Edge
 * string cannot contain a stub and other Edges.
 */
class EdgeString
{
public:
  struct EdgeEntry {
    ConstNetworkEdgePtr e;
    bool reversed;

    EdgeEntry(ConstNetworkEdgePtr e, bool reversed)
    {
      this->e = e;
      this->reversed = reversed;
    }

    QString toString() const
    {
      return e->toString() + (reversed ? " (reverse)" : "");
    }
  };

  EdgeString();

  void addFirstEdge(ConstNetworkEdgePtr e, bool reverse);

  void appendEdge(ConstNetworkEdgePtr e);

  Meters calculateLength(const ConstElementProviderPtr& provider) const;

  shared_ptr<EdgeString> clone() const;

  /**
   * Returns true if the specified edge is in this string.
   */
  bool contains(ConstNetworkEdgePtr e) const;

  /**
   * Returns true if the specified vertex is in this string.
   */
  bool contains(ConstNetworkVertexPtr e) const;

  QList<EdgeEntry> getAllEdges() const { return _edges; }

  ConstNetworkEdgePtr getEdge(int i) const { return _edges[i].e; }

  /**
   * Returns the edge that is d meters into the string. The Edge definition of length
   * (calculateLength) is used to determine offset. A value less than 0 will return the first
   * edge. A value greater than the string length (calculateLength()) will return the last edge.
   */
  ConstNetworkEdgePtr getEdgeAtOffset(ConstOsmMapPtr map, Meters offset) const;

  ConstNetworkVertexPtr getFrom() const;

  ConstNetworkEdgePtr getFirstEdge() const { return _edges.back().e; }

  ConstNetworkEdgePtr getLastEdge() const { return _edges.back().e; }

  QList<ConstElementPtr> getMembers() const;

  ConstNetworkVertexPtr getTo() const;

  bool isStub() const { return _edges.size() == 1 && _edges[0].e->isStub(); }

  bool overlaps(shared_ptr<const EdgeString> other) const;

  bool overlaps(ConstNetworkEdgePtr e) const;

  void prependEdge(ConstNetworkEdgePtr e);

  /**
   * Reverse the order of the edges in this string. The "reversed" flag on each edge is also
   * inverted.
   */
  void reverse();

  QString toString() const;

private:

  QList<EdgeEntry> _edges;

};

typedef shared_ptr<EdgeString> EdgeStringPtr;
typedef shared_ptr<const EdgeString> ConstEdgeStringPtr;

// needed for QSet
inline bool operator==(const ConstEdgeStringPtr& es1, const ConstEdgeStringPtr& es2)
{
  return es1->toString() == es2->toString();
}

inline uint qHash(const hoot::EdgeStringPtr& es)
{
  return qHash(es->toString());
}

inline uint qHash(const hoot::ConstEdgeStringPtr& es)
{
  return qHash(es->toString());
}

}

#endif // EDGESTRING_H
