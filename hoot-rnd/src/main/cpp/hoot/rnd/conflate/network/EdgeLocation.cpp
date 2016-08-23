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

QString EdgeLocation::toString() const
{
  return QString("{ _e: %1, _portion: %2 }").arg(hoot::toString(_e)).arg(_portion);
}

}
