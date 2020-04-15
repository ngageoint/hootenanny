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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ELEMENT_GEOMETRY_UTILS_H
#define ELEMENT_GEOMETRY_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/GeometricRelationship.h>

// GEOS
#include <geos/geom/Geometry.h>

namespace hoot
{

/**
 * TODO
 */
class ElementGeometryUtils
{

public:

  /**
   * Returns the distance between two elements
   *
   * @param element1 the first element to measure distance from
   * @param element2 the second element to measure distance from
   * @param map map owning the input elements
   * @return the distance between the two elements or -1.0 if the distance could not be calculated
   */
  static double getDistance(const ConstElementPtr& element1, const ConstElementPtr& element2,
                            ConstOsmMapPtr map);

  /**
   * Calculates the area of an element
   *
   * @param element the feature to calculate the area of
   * @param map map owning the input element
   * @return the area of the feature or -1.0 if the area could not be calculated
   */
  static double getArea(const ConstElementPtr& element, ConstOsmMapPtr map);

  /**
   * Determines two elements have a geometric relationship
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @param relationship the geometric relationship to check for between the two elements
   * @param map map owning the input elements
   * @return true if the two elements have the specified geometric relationship; false otherwise or
   * if the relationship could not be calculated
   * @todo should eventually back this with a cache, as is done in PoiPolygonInfoCache
   */
  static bool haveGeometricRelationship(
    const ConstElementPtr& element1, const ConstElementPtr& element2,
    const GeometricRelationship& relationship, ConstOsmMapPtr map);

private:

  static int _badGeomCount;

  static std::shared_ptr<geos::geom::Geometry> _getGeometry(
    const ConstElementPtr& element, ConstOsmMapPtr map);
};

}

#endif // ELEMENT_GEOMETRY_UTILS_H
