#include "NetworkEdge.h"

#include <hoot/core/util/ElementConverter.h>

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

Meters NetworkEdge::calculateLength(const ConstElementProviderPtr& provider) const
{
  if (_members.size() != 1)
  {
    throw NotImplementedException("Edges with multiple members are not yet supported.");
  }

  return ElementConverter(provider).calculateLength(_members[0]);
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
