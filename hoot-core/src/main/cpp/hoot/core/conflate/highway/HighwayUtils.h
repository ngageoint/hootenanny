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

#ifndef HIGHWAY_UTILS_H
#define HIGHWAY_UTILS_H

// Hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Utilities for use with roads
 */
class HighwayUtils
{

public:

  /**
   * Determines if two elements have conflicting one way street tags
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if their one way tags conflict; false otherwise
   */
  static bool oneWayConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Determines if a way has an explicitly negative one way tag (oneway=no, etc.)
   *
   * @param element the element to examine
   * @return true if the element contains a tag indicating it is not a one way street; false
   * otherwise
   */
  static bool explicitlyNotAOneWayStreet(const ConstElementPtr& element);

  /**
   * Determines if there are specific highway type conflicts between elements
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if both have specific highway tags (other than highway=road) and they disagree;
   * false otherwise
   */
  static bool nonGenericHighwayConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2);

};

}

#endif // HIGHWAY_UTILS_H
