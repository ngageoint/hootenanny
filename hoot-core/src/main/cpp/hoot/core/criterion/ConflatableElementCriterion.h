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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CONFLATABLE_ELEMENT_CRITERION_H
#define CONFLATABLE_ELEMENT_CRITERION_H

// hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>

namespace hoot
{

/**
 * Simple abstract base class that signifies an ElementCriterion that describes a conflatable
 * feature type; e.g. BuildingCriterion.  All ElementCriterion used for identifying conflatable
 * features should inherit from this class
 */
class ConflatableElementCriterion : public GeometryTypeCriterion
{
public:

  static std::string className() { return "hoot::ConflatableElementCriterion"; }

  virtual ~ConflatableElementCriterion() {}

  /**
   * Determines which criteria consider an element as conflatable
   *
   * @param e the element to determine conflatability of
   * @return a list of ConflatableElementCriterion class names that consider the element as
   * conflatable
   */
  static QStringList getConflatableCriteriaForElement(const ConstElementPtr& e);

  /**
   * Returns instantiations of conflatable criteria index by class name
   *
   * @return a conflatable criteria map of class names to criterion
   */
  static QMap<QString, ElementCriterionPtr> getConflatableCriteria();

  /**
   * Returns instantiations of conflatable criteria index by class name
   *
   * @param geometryType a geometry type filter; only criteria supporting the specified geometry
   * type will be returned
   * @return a conflatable criteria map of class names to criterion
   */
  static QMap<QString, ElementCriterionPtr> getConflatableCriteria(
    const GeometryType& geometryType);

  /**
   * Returns all ConflatableElementCriterion class names for a given geometry type
   *
   * @param geometryType geometry type
   * @return a list of class names inheriting from ConflatableElementCriterion
   */
  static QStringList getCriterionClassNamesByGeometryType(const GeometryType& geometryType);

private:

  // criterion class names to criterion objects
  static QMap<QString, ElementCriterionPtr> _conflatableCriteria;

  // geometry type string to a map of criterion class names to criterion objects
  static QMap<QString, QMap<QString, ElementCriterionPtr>> _conflatableCriteriaByGeometryType;

  static void _createConflatableCriteria();
};

}

#endif // CONFLATABLE_ELEMENT_CRITERION_H
