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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMNETWORK_H
#define OSMNETWORK_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/rnd/conflate/network/NetworkEdge.h>
#include <hoot/rnd/conflate/network/NetworkVertex.h>

// Qt
#include <QList>

namespace hoot
{

/**
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
  typedef QHash<ElementId, NetworkVertexPtr> VertexMap;
  typedef QHash<ElementId, NetworkEdgePtr> EdgeMap;
  typedef QMultiHash<ConstNetworkVertexPtr, ConstNetworkEdgePtr> VertexToEdgeMap;

  OsmNetwork();

  void addEdge(NetworkEdgePtr edge);

  void addVertex(NetworkVertexPtr node);

  QList<ConstNetworkEdgePtr> getEdgesFromVertex(ConstNetworkVertexPtr v) const;

  const EdgeMap& getEdgeMap() const { return _eidToEdge; }

  NetworkVertexPtr getVertex(ElementId eid) const { return _eidToVertex[eid]; }

  const VertexMap& getVertexMap() const { return _eidToVertex; }

  QString toString();

private:
  QList<NetworkEdgePtr> _edges;
  VertexMap _eidToVertex;
  EdgeMap _eidToEdge;
  VertexToEdgeMap _vertexToEdge;
};

typedef shared_ptr<OsmNetwork> OsmNetworkPtr;
typedef shared_ptr<const OsmNetwork> ConstOsmNetworkPtr;

}

#endif // OSMNETWORK_H
