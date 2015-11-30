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

QString NetworkEdge::toString() const
{
  QString result;

  QStringList memberIds;
  for (int i = 0; i < _members.size(); ++i)
  {
    memberIds << _members[i]->getElementId().toString();
  }

  if (_directed)
  {
    result = QString("%1 -- %2 --> %3").arg(_from->toString()).arg(memberIds.join(",")).
      arg(_to->toString());
  }
  else
  {
    result = QString("%1 -- %2 -- %3").arg(_from->toString()).arg(memberIds.join(",")).
      arg(_to->toString());
  }

  return result;
}

}
