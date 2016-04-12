#include "EdgeString.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

EdgeString::EdgeString()
{
}

void EdgeString::addFirstEdge(ConstNetworkEdgePtr e, bool reversed)
{
  assert(_edges.size() == 0);
  _edges.append(EdgeEntry(e, reversed));
}

void EdgeString::appendEdge(ConstNetworkEdgePtr e)
{
  assert(_edges.size() > 0);

  bool reversed;
  if (e->getFrom() == getTo())
  {
    reversed = false;
  }
  else if (e->getTo() == getTo())
  {
    reversed = true;
  }
  else
  {
    throw HootException("Error attempting to append an edge that isn't connected.");
  }

  _edges.append(EdgeEntry(e, reversed));
}

shared_ptr<EdgeString> EdgeString::clone() const
{
  EdgeStringPtr result(new EdgeString());
  result->_edges = _edges;
  return result;
}

/**
 * Returns true if the specified edge is in this EdgeString.
 */
bool EdgeString::contains(ConstNetworkEdgePtr e) const
{
  for (int i = 0; i < _edges.size(); ++i)
  {
    if (_edges[i].e == e)
    {
      return true;
    }
  }

  return false;
}

ConstNetworkVertexPtr EdgeString::getFrom() const
{
  if (_edges.front().reversed)
  {
    return _edges.front().e->getTo();
  }
  else
  {
    return _edges.front().e->getFrom();
  }
}

ConstNetworkVertexPtr EdgeString::getTo() const
{
  if (_edges.back().reversed)
  {
    return _edges.back().e->getFrom();
  }
  else
  {
    return _edges.back().e->getTo();
  }
}

void EdgeString::prependEdge(ConstNetworkEdgePtr e)
{
  assert(_edges.size() > 0);

  bool reversed;
  if (e->getFrom() == getFrom())
  {
    reversed = true;
  }
  else if (e->getTo() == getFrom())
  {
    reversed = false;
  }
  else
  {
    throw HootException("Error attempting to append an edge that isn't connected.");
  }

  _edges.prepend(EdgeEntry(e, reversed));
}

void EdgeString::reverse()
{
  std::reverse(_edges.begin(), _edges.end());
  for (int i = 0; i < _edges.size(); ++i)
  {
    _edges[i].reversed = !_edges[i].reversed;
  }
}

QString EdgeString::toString() const
{
  return hoot::toString(_edges);
}

}
