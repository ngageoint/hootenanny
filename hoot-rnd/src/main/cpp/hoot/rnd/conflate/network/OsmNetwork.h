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
 */
class OsmNetwork
{
public:
  OsmNetwork();

  void addEdge(NetworkEdgePtr edge) { _edges.append(edge); }

  void addVertex(NetworkVertexPtr node) { _vertices.append(node); }

  QList<NetworkEdgePtr> getEdges() const { return _edges; }

  QList<NetworkVertexPtr> getVertices() const { return _vertices; }


private:
  QList<NetworkEdgePtr> _edges;
  QList<NetworkVertexPtr> _vertices;
};

typedef shared_ptr<OsmNetwork> OsmNetworkPtr;

}

#endif // OSMNETWORK_H
