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

#ifndef COMMON_ELEMENT_ID_FINDER_H
#define COMMON_ELEMENT_ID_FINDER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Finds element IDs in common between two maps, as well as other functionality.
 */
class CommonElementIdFinder
{

public:

  CommonElementIdFinder() = default;
  ~CommonElementIdFinder() = default;

  /**
   * Finds a collection of element IDs in common between two maps
   *
   * @param map1 the first map to examine
   * @param map2 the second map to examine
   * @return a collection of element IDs
   */
  static QSet<ElementId> findCommonElementIds(const OsmMapPtr& map1, const OsmMapPtr& map2);

  /**
   * Finds a collection of element IDs that are in the first map but not in the second map
   *
   * @param map1 the first map to examine
   * @param map2 the second map to examine
   * @return a collection of element IDs
   */
  static QSet<ElementId> findElementIdsInFirstAndNotSecond(
    const OsmMapPtr& map1, const OsmMapPtr& map2);

  /**
   * Finds a collection of element IDs in that are in the second map but not in the first map
   *
   * @param map1 the first map to examine
   * @param map2 the second map to examine
   * @return a collection of element IDs
   */
  static QSet<ElementId> findElementIdsInSecondAndNotFirst(
    const OsmMapPtr& map1, const OsmMapPtr& map2);

private:

  static QSet<ElementId> _findElementIdDiff(
    const OsmMapPtr& map1, const OsmMapPtr& map2, const bool keepIdsFromMap1);
};

}

#endif // COMMON_ELEMENT_ID_FINDER_H
