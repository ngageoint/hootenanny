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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef ELEMENT_GEOMETRY_UTILS_H
#define ELEMENT_GEOMETRY_UTILS_H

// Hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometricRelationship.h>

// GEOS
#include <geos/geom/Geometry.h>

namespace hoot
{

/**
 * Utility methods for using Element with GEOS geometry based functions and hoot's more generic
 * GeometryType concept.
 */
class ElementGeometryUtils
{

public:

  /**
   * Determines if two elements have a geometric relationship
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @param relationship the geometric relationship to check for between the two elements
   * @param map map owning the input elements
   * @return true if the two elements have the specified geometric relationship; false otherwise or
   * if the relationship could not be calculated
   */
  static bool haveGeometricRelationship(const ConstElementPtr& element1, const ConstElementPtr& element2,
                                        const GeometricRelationship& relationship, ConstOsmMapPtr map);

  /**
   * Calculate the length of the given way in meters. The projection must be planar.
   */
  static Meters calculateLength(const ConstElementPtr& e, const ConstElementProviderPtr& constProvider);

private:

  static int _badGeomCount;

  static std::shared_ptr<geos::geom::Geometry> _getGeometry(const ConstElementPtr& element, ConstOsmMapPtr map);
};

}

#endif // ELEMENT_GEOMETRY_UTILS_H
