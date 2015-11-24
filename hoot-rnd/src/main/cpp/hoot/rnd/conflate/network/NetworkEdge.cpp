#include "NetworkEdge.h"

namespace hoot
{

NetworkEdge::NetworkEdge() : _directed(false)
{
}

NetworkEdge::NetworkEdge(NetworkVertexPtr from, NetworkVertexPtr to, bool directed) :
  _from(from),
  _to(to),
  _directed(directed)
{

}

}
