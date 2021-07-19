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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef NETWORKEDGE_H
#define NETWORKEDGE_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/conflate/network/NetworkVertex.h>
#include <hoot/core/elements/ElementProvider.h>

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
  NetworkEdge(ConstNetworkVertexPtr from, ConstNetworkVertexPtr to, bool directed = false,
    ConstElementPtr member = ConstElementPtr());

  void addMember(ConstElementPtr e) { _members.append(e); }

  Meters calculateLength(const ConstElementProviderPtr& provider) const;

  bool contains(const ConstNetworkVertexPtr& v) const;

  bool isDirected() const { return _directed; }
  /**
   * Returns true if this is a stub edge (both vertices are the same). Stubs are handy when trying
   * to match an edge to a vertex.
   */
  bool isStub() const { return _from->getElementId() == _to->getElementId(); }

  bool operator==(const NetworkEdge& other) const { return toString() == other.toString(); }

  QString toString() const;

  ConstNetworkVertexPtr getFrom() const { return _from; }
  const QList<ConstElementPtr>& getMembers() const { return _members; }
  ConstNetworkVertexPtr getTo() const { return _to; }

  void setMembers(QList<ConstElementPtr> members) { _members = members; }

private:

  ConstNetworkVertexPtr _from, _to;
  bool _directed;
  QList<ConstElementPtr> _members;
};

using NetworkEdgePtr = std::shared_ptr<NetworkEdge>;
using ConstNetworkEdgePtr = std::shared_ptr<const NetworkEdge>;

// not implemented
bool operator<(ConstNetworkEdgePtr, ConstNetworkEdgePtr);

inline bool operator==(const ConstNetworkEdgePtr& a, const ConstNetworkEdgePtr& b)
{
  return *a == *b;
}

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
  return qHash(std::dynamic_pointer_cast<const NetworkEdge>(v));
}

}

#endif // NETWORKEDGE_H
