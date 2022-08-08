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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef GEOMETRY_TYPE_CRITERION_H
#define GEOMETRY_TYPE_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * A criterion that can be mapped to a single geometry type.
 *
 * We may eventually run into situations where a criterion needs to be mapped to more than one
 * geometry type.
 */
class GeometryTypeCriterion : public ElementCriterion
{
public:

  enum GeometryType
  {
    Point = 0,
    Line,
    Polygon,
    Unknown
  };

  static QString className() { return "GeometryTypeCriterion"; }

  GeometryTypeCriterion() = default;
  virtual ~GeometryTypeCriterion() = default;

  virtual GeometryType getGeometryType() const = 0;

  /**
   * Returns all GeometryTypeCriterion class names for a given geometry type or all
   * GeometryTypeCriterion class names if GeometryType::Unknown is passed in as the geometry type
   *
   * @param geometryType geometry type
   * @return a list of class names
   */
  static QStringList getCriterionClassNamesByGeometryType(const GeometryType& geometryType);

  /**
   * Returns a string representation of a geometry type
   *
   * @param geometryType geometry type
   * @return a geometry type string
   */
  static QString typeToString(const GeometryType& geometryType);

  /**
   * Converts a string representation of a geometry to a geometry type
   *
   * @param geometryTypeStr string to convert
   * @return a geometry type
   */
  static GeometryType typeFromString(const QString& geometryTypeStr);
};

using GeometryTypeCriterionPtr = std::shared_ptr<GeometryTypeCriterion>;

}

#endif // GEOMETRY_TYPE_CRITERION_H
