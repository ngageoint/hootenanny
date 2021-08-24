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
#ifndef OSMNETWORK_H
#define OSMNETWORK_H

// hoot
#include <hoot/core/conflate/network/NetworkEdge.h>
#include <hoot/core/conflate/network/NetworkVertex.h>

namespace hoot
{

/**
 * The OsmNetwork represents major transportational vertices and edges in the map. These may or
 * may not have a one to one correlation with nodes, ways and relations. For instance, a vertex
 * may represent all the ways and nodes in a roundabout, or simply the node at a four way stop.
 * Though the former has yet to be implemented (as of 2016-07-06).
 *
 * Why not use boost graphs? Well, I used them for the OsmSchema implementation and in my opinion
 * it is write-once read-never code. While it is likely very efficient it is very difficult to
 * write and understand.
 *
 * Network vs. Graph - Typically I think of graph analysis when writing applications, however in
 * GIS paralance network is more common. I'm just trying to fit in.
 *
 * It is assumed that after the network is created the Network*Ptrs will not change. Changing them
 * will result in undefined behaviour.
 */
class OsmNetwork
{
public:

  using VertexMap = QMultiHash<ElementId, ConstNetworkVertexPtr>;
  using EdgeMap = QMultiHash<ElementId, ConstNetworkEdgePtr>;
  using VertexToEdgeMap = QMultiHash<ConstNetworkVertexPtr, ConstNetworkEdgePtr>;

  OsmNetwork() = default;

  void addEdge(NetworkEdgePtr edge);

  void addVertex(ConstNetworkVertexPtr node);

  bool contains(ConstNetworkVertexPtr v) const { return _vertexToEdge.contains(v); }

  QList<ConstNetworkEdgePtr> getEdgesFromVertex(ConstNetworkVertexPtr v) const;

  const EdgeMap& getEdgeMap() const { return _eidToEdge; }

  /**
   * Returns the single vertex associated with eid. If there is no vertex associated with eid then
   * a null vertex pointer is returned. If there are multiple vertices associated with eid then
   * an exception will be thrown.
   */
  ConstNetworkVertexPtr getSingleVertex(ElementId eid) const;

  const VertexMap& getVertexMap() const { return _eidToVertex; }

  void removeEdge(ConstNetworkEdgePtr e);

  void removeVertex(ConstNetworkVertexPtr v);

  QString toString();

private:

  QList<ConstNetworkEdgePtr> _edges;
  VertexMap _eidToVertex;
  EdgeMap _eidToEdge;
  VertexToEdgeMap _vertexToEdge;
};

using OsmNetworkPtr = std::shared_ptr<OsmNetwork>;
using ConstOsmNetworkPtr = std::shared_ptr<const OsmNetwork>;

// not implemented
bool operator<(ConstOsmNetworkPtr, ConstOsmNetworkPtr);

}

#endif // OSMNETWORK_H
