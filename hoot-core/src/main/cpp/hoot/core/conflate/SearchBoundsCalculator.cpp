/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SearchBoundsCalculator.h"

// hoot
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

using namespace geos::geom;

SearchBoundsCalculator::SearchBoundsCalculator(const SearchRadiusProviderPtr& radiusProvider)
{
  _radiusProvider = radiusProvider;
}

Envelope SearchBoundsCalculator::calculateSearchBounds(const ConstOsmMapPtr& map,
  const ConstNodePtr& n) const
{
  LOG_INFO("Calculating search radius...");

  // This is likely to cause some performance issues, but only seems to be called by Multiary code
  // and not doing it here is causing ScriptMatchCreator crashes...so will deal with performance
  // issue when necessary.
  _radiusProvider->init(map);

  LOG_VART(map->getElementCount());
  LOG_VART(n->getElementId());

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
