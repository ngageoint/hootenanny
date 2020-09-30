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
  static OsmMapPtr getMapSubset(const ConstOsmMapPtr& map, const ElementCriterionPtr& filter,
                                const bool copyChildren = true);

  /**
   * Determines if a map contains only nodes that are not way nodes
   *
   * @param map the map to examine
   * @return true if the map is made up of non-way node points only; false otherwise
   * @todo probably duplicating CriterionUtils::constainSatisfyingElements
   */
  static bool mapIsPointsOnly(const OsmMapPtr& map);

  /**
   * Splits a map into two separate maps according to element status
   *
   * @param sourceMap the map to split
   * @param unknown1Map the map to output with Unknown1 status
   * @param unknown2Map the map to output with Unknown2 status
   */
  static void splitMapByStatus(OsmMapPtr& sourceMap, OsmMapPtr& unknown1Map,
                               OsmMapPtr& unknown2Map);

  /**
   * TODO
   *
   * @param map
   * @todo Should we have something like reader.include.debug.tags instead of this?
   */
  static void dropMetadataTags(const OsmMapPtr& map);

  /**
   * Combines two maps into one; throwOutDupes ignores any elements in the second map with the ID
   * as an element in the first map
   *
   * @param map1
   * @param map2
   * @param throwOutDupes
   */
  static void combineMaps(OsmMapPtr& map1, OsmMapPtr& map2, const bool throwOutDupes);
};

}

#endif // MAP_UTILS_H
