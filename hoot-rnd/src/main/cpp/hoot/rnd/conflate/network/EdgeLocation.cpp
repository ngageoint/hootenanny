#include "EdgeLocation.h"

namespace hoot
{

const double EdgeLocation::SLOPPY_EPSILON = WayLocation::SLOPPY_EPSILON;

ConstNetworkVertexPtr EdgeLocation::getVertex(double epsilon) const
{
  if (isExtreme(epsilon) == false)
  {
    throw IllegalArgumentException("Attempted to get a vertex on an edge location that isn't on "
     "a vertex.");
  }

  if (_portion < 0.5)
  {
    return _e->getFrom();
  }
  else
  {
    return _e->getTo();
  }
}

Meters EdgeLocation::getOffset(const ConstElementProviderPtr& provider) const
{
  return _portion * _e->calculateLength(provider);
}

shared_ptr<EdgeLocation> EdgeLocation::move(const ConstElementProviderPtr& provider,
  Meters distance) const
{
  Meters l = _e->calculateLength(provider);

  Meters offset = _portion * l + distance;
  Meters portion = min(1.0, max(offset / l, 0.0));

  return shared_ptr<EdgeLocation>(new EdgeLocation(_e, portion));
}

QString EdgeLocation::toString() const
{
  return QString("{ _e: %1, _portion: %2 }").arg(hoot::toString(_e)).arg(_portion);
}

}
