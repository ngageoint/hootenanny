#include "EdgeSubline.h"

namespace hoot
{

bool operator==(ConstEdgeSublinePtr es1, ConstEdgeSublinePtr es2)
{
  bool result = false;

  if (es1.get() == es2.get())
  {
    result = true;
  }
  else if (es1->getStart() == es2->getStart() && es1->getEnd() == es2->getEnd())
  {
    result = true;
  }

//  bool strResult = es1->toString() == es2->toString();

//  if (result != strResult)
//  {
//    LOG_VARE(result);
//    LOG_VARE(strResult);
//    LOG_VARE(es1);
//    LOG_VARE(es2);
//    throw HootException();
//  }

  return result;
}

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

bool EdgeSubline::contains(ConstNetworkVertexPtr v) const
{
  bool result = false;

  if (getStart()->isExtreme() && getStart()->getVertex() == v)
  {
    result = true;
  }
  else if (getEnd()->isExtreme() && getEnd()->getVertex() == v)
  {
    result = true;
  }

  return result;
}

shared_ptr<EdgeSubline> EdgeSubline::createFullSubline(ConstNetworkEdgePtr e)
{
  return shared_ptr<EdgeSubline>(new EdgeSubline(e, 0.0, 1.0));
}

bool EdgeSubline::overlaps(shared_ptr<const EdgeSubline> other) const
{
  bool result = false;

  if (other->getEdge() == getEdge())
  {
    /// this    *---f----l----*
    /// other   *-----f----l--*
    if (other->getFormer() < getLatter() && other->getLatter() >= getLatter())
    {
      result = true;
    }
    /// this    *-----f----l--*
    /// other   *--f----l-----*
    else if (other->getFormer() <= getFormer() && other->getLatter() > getFormer())
    {
      result = true;
    }
  }

  return result;
}

QString EdgeSubline::toString() const
{
  return QString("{ _start: %1, _end: %2 }").arg(hoot::toString(_start)).arg(hoot::toString(_end));
}

}
