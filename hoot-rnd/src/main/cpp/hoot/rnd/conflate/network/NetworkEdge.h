#ifndef NETWORKEDGE_H
#define NETWORKEDGE_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/rnd/conflate/network/NetworkVertex.h>

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

  NetworkEdge(NetworkVertexPtr from, NetworkVertexPtr to, bool directed);

  void addMember(ElementPtr e) { _members.append(e); }

  NetworkVertexPtr getFrom() { return _from; }

  NetworkVertexPtr getTo() { return _to; }

  bool isDirected() { return _directed; }

private:
  NetworkVertexPtr _from, _to;
  bool _directed;
  QList<ElementPtr> _members;
};

typedef shared_ptr<NetworkEdge> NetworkEdgePtr;

}

#endif // NETWORKEDGE_H
