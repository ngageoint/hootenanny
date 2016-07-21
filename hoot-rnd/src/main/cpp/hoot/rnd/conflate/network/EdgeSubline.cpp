#include "EdgeSubline.h"

namespace hoot
{

EdgeSubline::EdgeSubline(ConstEdgeLocationPtr start, ConstEdgeLocationPtr end) :
  _start(start),
  _end(end)
{
  assert(start->getEdge() == _end->getEdge());
}

EdgeSubline::EdgeSubline(ConstNetworkEdgePtr e, double start, double end) :
  _start(new EdgeLocation(e, start)),
  _end(new EdgeLocation(e, end))
{
}

shared_ptr<EdgeSubline> EdgeSubline::clone() const
{
  return shared_ptr<EdgeSubline>(new EdgeSubline(_start, _end));
}

shared_ptr<EdgeSubline> EdgeSubline::createFullSubline(ConstNetworkEdgePtr e)
{
  return shared_ptr<EdgeSubline>(new EdgeSubline(e, 0.0, 1.0));
}

QString EdgeSubline::toString() const
{
  return QString("{ _start: %1, _end: %2 }").arg(_start->toString()).arg(_end->toString());
}

}
