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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef OSM_UTILS_H
#define OSM_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * General, non-specific utilities for use with the OSM data.
 *
 * Put any Element related utilities here that either don't fit into any of the existing OSM related
 * utils classes, would clutter up any of the core Element classes, or are isolated enough not to
 * warrant making a new utils class for them yet.
 */
class OsmUtils
{
public:

  /**
   * Get a more detailed string representation of a relation
   *
   * @param relation relation to get info from
   * @param map map owning the relation
   * @return a detailed relation string
   */
  static QString getRelationDetailString(const ConstRelationPtr& relation, const ConstOsmMapPtr& map);

  /**
   * Constructs a detailed string for an element suitable for trace logging
   *
   * @param element the element to create a string for
   * @param map map owning the element
   * @return a string describing the element
   */
  static QString getElementDetailString(const ConstElementPtr& element, const ConstOsmMapPtr& map);

  /**
   *  Constructs a detailed string for collection of elements suitable for trace logging
   *
   * @param elements the elements to create a string for
   * @param map map owning the elements
   * @return a string describing the elements
   */
  static QString getElementsDetailString(const std::vector<ElementPtr>& elements, const ConstOsmMapPtr& map);

  /**
   * Determines if an element is contained by any way or relation in a map
   *
   * @param elementId the ID of the element to search for
   * @param map the map containing the element
   * @return true if any way or relation in the map contains the element; false otherwise
   */
  static bool isChild(const ElementId& elementId, const ConstOsmMapPtr& map);
};

}

#endif // OSM_UTILS_H
