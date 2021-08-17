
/*
 * This file provides the beginnings of conversion from C++ GEOS structures to their Java
 * counterparts. We could use JTS structures at some point in the future.
 */

%{

// geos
#include <geos/geom/Envelope.h>

%}

namespace geos
{
namespace geom
{
class Envelope
{
public:
  Envelope();
  Envelope(double minX, double maxX, double minY, double maxY);
  double getMinX();
  double getMinY();
  double getMaxX();
  double getMaxY();
};
}
}
