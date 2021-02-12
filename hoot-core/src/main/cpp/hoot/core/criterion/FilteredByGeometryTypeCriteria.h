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
#ifndef FILTERED_BY_GEOMETRY_TYPE_CRITERIA_H
#define FILTERED_BY_GEOMETRY_TYPE_CRITERIA_H

// Qt
#include <QStringList>

namespace hoot
{

/**
 * This interface allows for identifying feature geometry types associated with a hoot operator.
 * This is used during conflation to weed out unnecessary conflate ops specified in the
 * configuration. A list of class names is returned instead of instantiated criterion classes for
 * performance reasons. Callers are responsible for verifying that the classes are valid
 * GeometryTypeCriterion classes.
 */
class FilteredByGeometryTypeCriteria
{
public:

  FilteredByGeometryTypeCriteria() = default;
  virtual ~FilteredByGeometryTypeCriteria() = default;

  /**
   * Returns all associated GeometryTypeCriterion class names. The criteria are logically OR'd
   * together. If a class that doesn't inherit from GeometryTypeCriterion is returned by an
   * implementer, an exception will be thrown.
   *
   * @return a list of class names
   */
  virtual QStringList getCriteria() const = 0;
};

}

#endif // FILTERED_BY_GEOMETRY_TYPE_CRITERIA_H
