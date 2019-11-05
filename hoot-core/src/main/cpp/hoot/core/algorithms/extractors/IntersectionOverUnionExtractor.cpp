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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "IntersectionOverUnionExtractor.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ElementConverter.h>

namespace hoot
{

double IntersectionOverUnionExtractor::extract(
  const OsmMap& map, const ConstElementPtr& target, const ConstElementPtr& candidate) const
{
  // only calc'ing this for way polys...not sure it makes sense with anything else

  if (target->getElementType() == ElementType::Way &&
      candidate->getElementType() == ElementType::Way)
  {
    ElementConverter elementConverter(map.shared_from_this());
    std::shared_ptr<geos::geom::Geometry> targetGeom = elementConverter.convertToGeometry(target);
    std::shared_ptr<geos::geom::Geometry> candidateGeom =
      elementConverter.convertToGeometry(candidate);
    if (!targetGeom->isEmpty() && !candidateGeom->isEmpty())
    {
      std::shared_ptr<geos::geom::Geometry> intersectionGeom(
        targetGeom->intersection(candidateGeom.get()));
      std::shared_ptr<geos::geom::Geometry> unionGeom(
        targetGeom->Union(candidateGeom.get()));

      if (intersectionGeom && unionGeom)
      {
        const double intersectionArea = intersectionGeom->getArea();
        const double unionArea = unionGeom->getArea();
        if (unionArea > 0.0 && intersectionArea >= 0.0)
        {
          return intersectionArea / unionArea;
        }
      }
    }
  }

  return nullValue();
}

}
