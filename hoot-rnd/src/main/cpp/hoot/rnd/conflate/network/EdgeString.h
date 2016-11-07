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

#include "EdgeSubline.h"
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
  /// @todo this structure can likely go away in the near future.
  struct EdgeEntry {
    EdgeEntry(ConstEdgeSublinePtr subline) : _subline(subline)
    {
    }

    const ConstNetworkEdgePtr& getEdge() const { return _subline->getEdge(); }

    const ConstEdgeSublinePtr& getSubline() const { return _subline; }

    bool isBackwards() const
    {
      return _subline->isBackwards();
    }

    void reverse()
    {
      EdgeSublinePtr tmp = _subline->clone();
      tmp->reverse();
      _subline = tmp;
    }

    QString toString() const
    {
      return _subline->toString();
    }

  private:
    /// The portion of the edge. The portion is always measured starting at "from" regardless of
    /// whether or not reversed is set. It is always a value in [0, 1]. If toPortion < fromPortion
    /// the edge is reversed.
    ConstEdgeSublinePtr _subline;
  };

  EdgeString();

  void addFirstEdge(ConstNetworkEdgePtr e);

  void addFirstEdge(ConstEdgeSublinePtr subline);

  void appendEdge(ConstNetworkEdgePtr e);

  void appendEdge(ConstEdgeSublinePtr subline);

  Meters calculateLength(const ConstElementProviderPtr& provider) const;

  shared_ptr<EdgeString> clone() const;

  /**
   * Returns true if the entire string in other is contained by this.
   */
  bool contains(const shared_ptr<const EdgeString> other) const;

  /**
   * Returns true if the specified edge is in this string.
   */
  bool contains(ConstNetworkEdgePtr e) const;

  /**
   * Returns true if the specified vertex is in this string.
   */
  bool contains(ConstNetworkVertexPtr e) const;

  bool containsInteriorVertex(ConstNetworkVertexPtr v) const;

  const QList<EdgeEntry>& getAllEdges() const { return _edges; }

  int getCount() const { return _edges.size(); }

  ConstNetworkEdgePtr getEdge(int i) const { return _edges[i].getEdge(); }

  ConstEdgeSublinePtr getEdgeSubline(int i) const { return _edges[i].getSubline(); }

  QSet<ConstNetworkEdgePtr> getEdgeSet() const;

  /**
   * Returns the edge that is d meters into the string. The Edge definition of length
   * (calculateLength) is used to determine offset. A value less than 0 will return the first
   * edge. A value greater than the string length (calculateLength()) will return the last edge.
   */
  ConstNetworkEdgePtr getEdgeAtOffset(ConstOsmMapPtr map, Meters offset) const;

  ConstEdgeLocationPtr getFrom() const;

  ConstNetworkVertexPtr getFromVertex() const;

  ConstNetworkEdgePtr getFirstEdge() const { return _edges.front().getEdge(); }

  ConstNetworkEdgePtr getLastEdge() const { return _edges.back().getEdge(); }

  QList<ConstElementPtr> getMembers() const;

  ConstEdgeLocationPtr getTo() const;

  ConstNetworkVertexPtr getToVertex() const;

  bool isEdgeClosed() const;

  /**
   * Returns true if v is at the beginning or end of the string.
   */
  bool isAtExtreme(ConstNetworkVertexPtr v) const;

  bool isFromOnVertex() const { return getFrom()->isExtreme(EdgeLocation::SLOPPY_EPSILON); }

  /**
   * If neither terminal is on a vertex return true.
   */
  bool isFullPartial() const { return isFromOnVertex() == false && isToOnVertex() == false; }

  /**
   * If at least one terminal is not on a vertex.
   */
  bool isPartial() const { return isFromOnVertex() == false || isToOnVertex() == false; }

  bool isStub() const { return _edges.size() == 1 && _edges[0].getEdge()->isStub(); }

  bool isToOnVertex() const { return getTo()->isExtreme(EdgeLocation::SLOPPY_EPSILON); }

  bool overlaps(shared_ptr<const EdgeString> other) const;

  bool overlaps(const ConstEdgeSublinePtr& es) const;

  bool overlaps(const ConstNetworkEdgePtr& es) const;

  void prependEdge(ConstNetworkEdgePtr e);

  /**
   * Reverse the order of the edges in this string. The "reversed" flag on each edge is also
   * inverted.
   */
  void reverse();

  QString toString() const;

private:

  friend class EdgeStringTest;

  QList<EdgeEntry> _edges;

};

typedef shared_ptr<EdgeString> EdgeStringPtr;
typedef shared_ptr<const EdgeString> ConstEdgeStringPtr;

// not implemented
bool operator<(ConstEdgeStringPtr, ConstEdgeStringPtr);

// needed for QSet
bool operator==(const ConstEdgeStringPtr& es1, const ConstEdgeStringPtr& es2);

inline uint qHash(const hoot::ConstEdgeStringPtr& es)
{
  uint result = 0;
  foreach (const EdgeString::EdgeEntry& e, es->getAllEdges())
  {
    result ^= qHash(e.getSubline());
  }

  return result;
}

inline uint qHash(const hoot::EdgeStringPtr& es)
{
  return qHash((ConstEdgeStringPtr)es);
}

}

#endif // EDGESTRING_H
