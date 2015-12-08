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
