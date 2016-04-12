#include "EdgeMatch.h"

namespace hoot
{

/// @todo This and qHash are effective, but inefficient, please come up with a better method.
bool operator==(const hoot::EdgeMatchPtr& em1, const hoot::EdgeMatchPtr& em2)
{
  return em1->toString() == em2->toString();
}

uint qHash(const hoot::EdgeMatchPtr& em)
{
  return qHash(em->toString());
}

EdgeMatch::EdgeMatch()
{
  _edges1.reset(new EdgeString());
  _edges2.reset(new EdgeString());
}

shared_ptr<EdgeMatch> EdgeMatch::clone() const
{
  EdgeMatchPtr result(new EdgeMatch);
  result->_edges1 = _edges1->clone();
  result->_edges2 = _edges2->clone();

  return result;
}

bool EdgeMatch::contains(ConstNetworkEdgePtr e) const
{
  return getString1()->contains(e) || getString2()->contains(e);
}

QString EdgeMatch::toString() const
{
  return QString("s1: %1 s2: %2").arg(_edges1->toString()).arg(_edges2->toString());
}

}
