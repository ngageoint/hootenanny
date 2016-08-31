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
#include "OsmNetwork.h"

namespace hoot
{

OsmNetwork::OsmNetwork()
{
}

void OsmNetwork::addEdge(NetworkEdgePtr edge)
{
  if (_eidToVertex.contains(edge->getFrom()->getElementId()) == false ||
    _eidToVertex.contains(edge->getTo()->getElementId()) == false)
  {
    throw IllegalArgumentException("Please add the vertices on an edge before adding the edge.");
  }
  foreach (const ConstElementPtr& e, edge->getMembers())
  {
    _eidToEdge.insert(e->getElementId(), edge);
  }
  _vertexToEdge.insertMulti(edge->getFrom(), edge);
  _vertexToEdge.insertMulti(edge->getTo(), edge);
  _edges.append(edge);
}

void OsmNetwork::addVertex(ConstNetworkVertexPtr node)
{
  _eidToVertex.insert(node->getElementId(), node);
}

QList<ConstNetworkEdgePtr> OsmNetwork::getEdgesFromVertex(ConstNetworkVertexPtr v) const
{
  return _vertexToEdge.values(v);
}

ConstNetworkVertexPtr OsmNetwork::getSingleVertex(ElementId eid) const
{
  QList<ConstNetworkVertexPtr> vertices = _eidToVertex.values(eid);
  if (vertices.size() > 1)
  {
    LOG_VARW(eid);
    LOG_VARW(vertices);
    throw IllegalArgumentException("Expected to receive a single vertex, but got more than one.");
  }
  else if (vertices.size() == 1)
  {
    return vertices[0];
  }
  else
  {
    return ConstNetworkVertexPtr();
  }
}

void OsmNetwork::removeEdge(ConstNetworkEdgePtr edge)
{
  foreach (const ConstElementPtr& e, edge->getMembers())
  {
    _eidToEdge.remove(e->getElementId(), edge);
  }
  _vertexToEdge.remove(edge->getFrom(), edge);
  _vertexToEdge.remove(edge->getTo(), edge);
  _edges.removeAll(edge);
}

void OsmNetwork::removeVertex(ConstNetworkVertexPtr v)
{
  if (_vertexToEdge.count(v) >= 1)
  {
    LOG_VARW(v);
    throw IllegalArgumentException("When removing a vertex the vertex cannot be part of an edge.");
  }

  _eidToVertex.remove(v->getElementId(), v);
}

QString OsmNetwork::toString()
{
  QStringList result;

  QSet<ElementId> touchedVertices;

  foreach (const ConstNetworkEdgePtr& e, _edges)
  {
    result << e->toString();
    touchedVertices.insert(e->getFrom()->getElementId());
    touchedVertices.insert(e->getTo()->getElementId());
  }

  QSet<ElementId> untouchedVertices = QSet<ElementId>::fromList(_eidToVertex.keys()).
    subtract(touchedVertices);

  foreach (const ElementId& eid, untouchedVertices)
  {
    result << hoot::toString(_eidToVertex.values(eid));
  }

  return result.join("\n");
}

}
