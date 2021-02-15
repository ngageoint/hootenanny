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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef CONFIGUTILS_H
#define CONFIGUTILS_H

// Hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/geometry/GeometricRelationship.h>
#include <hoot/core/criterion/InBoundsCriterion.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Utilities for dealing with hoot config options
 */
class ConfigUtils
{

public:

  /**
   * Determines if any of the filter by bounds options are enabled
   *
   * @return true if any of the bounds options are enabled
   */
  static bool boundsOptionEnabled();

  /**
   * Gets the current configured bounds geometry
   *
   * @return a geometry
   */
  static std::shared_ptr<geos::geom::Geometry> getBounds();

  /**
   * TODO
   *
   * @return
   */
  static QString getBoundsString();

  /**
   * Determines which feature bounds checking relationship should be used based on the configured
   * options for bounds
   *
   * @return a geometric relationship
   */
  static GeometricRelationship getBoundsRelationship();

  /**
   * Gets the current configured bounds condition
   *
   * @param map map in use for the current operation
   * @return a criterion used for checking whether an element satisfies the current configured
   * bounds condition
   */
  static std::shared_ptr<InBoundsCriterion> getBoundsFilter(const ConstOsmMapPtr& map);

  /**
   * Removes ApiTruncateTagVisitor from conflate ops if the corresponding config option is
   * activated
   */
  static void checkForTagValueTruncationOverride();

  /**
   * If DuplicateNodeRemover is in the chain, this allows the reader to read the duplicate elements
   * in so they can be removed by DuplicateNodeRemover.
   */
  static void checkForDuplicateElementCorrectionMismatch(const QStringList& ops);

  /**
   * Removes an entry from a list configuration option globally
   *
   * @param opKey key of the configuration option to remove a member from
   * @param entryToRemove value of the list member from the configuration option to remove
   */
  static void removeListOpEntry(const QString& opName, const QString& entryToRemove);

  /**
   * TODO
   *
   * @param opName
   * @param opEntry
   * @param index
   */
  static void insertListOpEntry(const QString& opName, const QString& opEntry, const int index);
};

}

#endif // CONFIGUTILS_H
