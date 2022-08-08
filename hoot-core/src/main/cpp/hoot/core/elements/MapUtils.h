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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef MAP_UTILS_H
#define MAP_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Various utilities for working with maps that should be kept out of OsmMap to avoid circular
 * dependencies
 */
class MapUtils
{

public:

  /**
   * Returns a subset of elements from a map filtered by a criterion
   *
   * @param map map to copy elements from
   * @param filter filter to apply to the map
   * @param copyChildren if true, child elements are copied
   * @return a copied subset map
   */
  static OsmMapPtr getMapSubset(
    const ConstOsmMapPtr& map, const ElementCriterionPtr& filter, const bool copyChildren = true);

  /**
   * Determines if a map contains only nodes that are not way nodes
   *
   * @param map the map to examine
   * @return true if the map is made up of non-way node points only; false otherwise
   * @todo probably duplicating CriterionUtils::constainSatisfyingElements
   */
  static bool mapIsPointsOnly(const OsmMapPtr& map);

  /**
   * Combines two maps into one; throwOutDupes ignores any elements in the second map with the ID
   * as an element in the first map
   *
   * @param map1
   * @param map2
   * @param throwOutDupes
   */
  static void combineMaps(const OsmMapPtr& map1, const OsmMapPtr& map2, const bool throwOutDupes);

  /**
   * Returns the first element with a "note" tag
   *
   * @param map map containing the element
   * @param note note tag value to search for
   * @param elementType optional element type for additional filtering
   * @return returns the first element found with the intput tag key/value pair and element type, if
   * specified; returns a null element otherwise
   */
  static ElementPtr getFirstElementWithNote(
    const OsmMapPtr& map, const QString& note,
    const ElementType& elementType = ElementType::Unknown);
  /**
   * Returns the first element with a specified tag
   *
   * @param map map containing the element
   * @param tagKey tag key to search for
   * @param tagValue tag value to search for
   * @param elementType optional element type for additional filtering
   * @return returns the first element found with the intput tag key/value pair and element type, if
   * specified; returns a null element otherwise
   */
  static ElementPtr getFirstElementWithTag(
    const OsmMapPtr& map, const QString& tagKey, const QString& tagValue,
    const ElementType& elementType = ElementType::Unknown);
  /**
   * @brief getFirstElementWithStatus Returns the first element with a specified status
   * @param map map containing the element
   * @param status status to search for
   * @return returns the first element found with the status; returns a null element otherwise
   */
  static ConstElementPtr getFirstElementWithStatus(const ConstOsmMapPtr& map, const Status& status);

  /**
   * @brief getNumReviews returns the number of unique reviews in a map
   * @param map the map to examine
   * @return a review count
   */
  static int getNumReviews(const OsmMapPtr& map);
};

}

#endif // MAP_UTILS_H
