#include "SearchBoundsCalculator.h"

// hoot
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

using namespace geos::geom;

SearchBoundsCalculator::SearchBoundsCalculator(SearchRadiusProviderPtr radiusProvider)
{
  _radiusProvider = radiusProvider;
}

Envelope SearchBoundsCalculator::calculateSearchBounds(const ConstOsmMapPtr& map,
  const ConstNodePtr& n) const
{
  double radius = _radiusProvider->calculateSearchRadius(map, n);

  Coordinate right = GeometryUtils::calculateDestination(n->toCoordinate(), 90, radius);
  Coordinate top = GeometryUtils::calculateDestination(n->toCoordinate(), 0, radius);

  double deltaX = right.x - n->getX();
  double deltaY = top.y - n->getY();

  Coordinate min = Coordinate(n->getX() - deltaX, n->getY() - deltaY);
  Coordinate max = Coordinate(n->getX() + deltaX, n->getY() + deltaY);

  return Envelope(min, max);
}

}
