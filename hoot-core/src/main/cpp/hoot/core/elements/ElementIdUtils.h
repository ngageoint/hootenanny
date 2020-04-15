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

#ifndef ELEMENT_ID_UTILS_H
#define ELEMENT_ID_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Various utilities for working with element IDs
 */
class ElementIdUtils
{

public:

  /**
   * Retrieves a collection of element IDs for a collection of elements
   *
   * @param elements a collection of elements
   * @return a collection of element IDs
   * @todo may be able to replace nodesToNodeIds with this more generic version
   */
  static QSet<ElementId> elementsToElementIds(const std::vector<ElementPtr>& elements);

  /**
   * Determines if all element IDs in a map are positive
   *
   * @param map the map to examine
   * @return true if all elements in the input map have a positive ID; false otherwise
   */
  static bool allElementIdsPositive(const ConstOsmMapPtr& map);

  /**
   * Determines if all element IDs in a map are negative
   *
   * @param map the map to examine
   * @return true if all elements in the input map have a negative ID; false otherwise
   */
  static bool allElementIdsNegative(const ConstOsmMapPtr& map);

  /**
   * Determines if all hoot:id element tags in a map match their element IDs
   *
   * @param map the map to examine
   * @return true if all the hoot:id tag values match the owning element's ID; false otherwise
   */
  static bool allIdTagsMatchIds(const ConstOsmMapPtr& map);
};

}

#endif // ELEMENT_ID_UTILS_H
