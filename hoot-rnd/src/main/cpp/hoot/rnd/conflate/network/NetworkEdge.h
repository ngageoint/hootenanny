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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NETWORKEDGE_H
#define NETWORKEDGE_H

// hoot
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/OsmMap.h>
#include <hoot/rnd/conflate/network/NetworkVertex.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * An edge doesn't necessarily represent a single way. For instance an edge may represent both
 * roads that make up a divided highway. It is up to the network extractor to determine how this
 * is represented.
 */
class NetworkEdge
{
public:
  NetworkEdge();

  NetworkEdge(ConstNetworkVertexPtr from, ConstNetworkVertexPtr to, bool directed);

  void addMember(ConstElementPtr e) { _members.append(e); }

  Meters calculateLength(const ConstElementProviderPtr& provider) const;

  bool contains(const ConstNetworkVertexPtr& v) const;

  ConstNetworkVertexPtr getFrom() const { return _from; }

  const QList<ConstElementPtr>& getMembers() const { return _members; }

  ConstNetworkVertexPtr getTo() const { return _to; }

  bool isDirected() const { return _directed; }

  /**
   * Returns true if this is a stub edge (both vertices are the same). Stubs are handy when trying
   * to match an edge to a vertex.
   */
  bool isStub() const { return _from->getElementId() == _to->getElementId(); }

  bool operator==(const NetworkEdge& other) const { return toString() == other.toString(); }

  void setMembers(QList<ConstElementPtr> members) { _members = members; }

  QString toString() const;

private:
  ConstNetworkVertexPtr _from, _to;
  bool _directed;
  QList<ConstElementPtr> _members;
};

typedef shared_ptr<NetworkEdge> NetworkEdgePtr;
typedef shared_ptr<const NetworkEdge> ConstNetworkEdgePtr;

// not implemented
bool operator<(ConstNetworkEdgePtr, ConstNetworkEdgePtr);

inline uint qHash(const ConstNetworkEdgePtr& v)
{
  uint result = 0;
  foreach (const ConstElementPtr& m, v->getMembers())
  {
    result = ::qHash(Tgs::cantorPairing(qHash(m->getElementId()), result));
  }
  result = ::qHash(Tgs::cantorPairing(qHash(v->getFrom()), result));
  result = ::qHash(Tgs::cantorPairing(qHash(v->getTo()), result));

  return result;
}

inline uint qHash(const NetworkEdgePtr& v)
{
  return qHash(dynamic_pointer_cast<const NetworkEdge>(v));
}

}

#endif // NETWORKEDGE_H
