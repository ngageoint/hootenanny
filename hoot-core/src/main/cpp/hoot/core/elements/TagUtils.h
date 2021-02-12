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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

#ifndef TAG_UTILS_H
#define TAG_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Various utilities for querying tags on elements
 */
class TagUtils
{

public:

  /**
   * Determines if all elements in a specified collection have any tag key from a specified set of
   * keys
   *
   * @param tagKeys the tag keys to search for
   * @param elements the elements to examine
   * @return true if all elements from the input collection of elements contain at least one of the
   * tag keys specified in tagKeys; false otherwise
   */
  static bool allElementsHaveAnyTagKey(const QStringList& tagKeys,
                                       const std::vector<ElementPtr>& elements);

  /**
   * Determines if any elements in a specified collection have any tag key from a specified set of
   * keys
   *
   * @param tagKeys the tag keys to search for
   * @param elements the elements to examine
   * @return true if any elements from the input collection of elements contain at least one of the
   * tag keys specified in tagKeys; false otherwise
   */
  static bool anyElementsHaveAnyTagKey(const QStringList& tagKeys,
                                       const std::vector<ElementPtr>& elements);

  /**
   * Determines if any elements in a specified collection have any tag key/value pair from a
   * specified set of kvps
   *
   * @param kvps the tag key/value pairs to search for
   * @param elements the elements to examine
   * @return true if any elements from the input collection of elements contain at least one of the
   * key/value pairs specified in kvps; false otherwise
   */
  static bool anyElementsHaveAnyKvp(const QStringList& kvps,
                                    const std::vector<ElementPtr>& elements);

  /**
   * Determines if all elements in a specified collection have any tag key from a specified set of
   * keys
   *
   * @param tagKeys the tag keys to search for
   * @param elementIds IDs of the elements to examine
   * @param map the map containing the elements
   * @return true if all elements from the input collection of elements contain at least one of the
   * tag keys specified in tagKeys; false otherwise
   */
  static bool allElementsHaveAnyTagKey(const QStringList& tagKeys,
                                       const std::set<ElementId>& elementIds, OsmMapPtr& map);

  /**
   * Determines if any elements in a specified collection have any tag key from a specified set of
   * keys
   *
   * @param tagKeys the tag keys to search for
   * @param elementIds IDs of the elements to examine
   * @param map the map containing the elements
   * @return true if any elements from the input collection of elements contain at least one of the
   * tag keys specified in tagKeys; false otherwise
   */
  static bool anyElementsHaveAnyTagKey(const QStringList& tagKeys,
                                       const std::set<ElementId>& elementIds, OsmMapPtr& map);

  /**
   * Determines if any elements in a specified collection have any tag key/value pair from a
   * specified set of kvps
   *
   * @param kvps the tag key/value pairs to search for
   * @param elementIds IDs of the elements to examine
   * @param map the map containing the elements
   * @return true if any elements from the input collection of elements contain at least one of the
   * key/value pairs specified in kvps; false otherwise
   */
  static bool anyElementsHaveAnyKvp(const QStringList& kvps,
                                    const std::set<ElementId>& elementIds, OsmMapPtr& map);

  /**
   * Determines if two elements have conflicting name tags
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if their name tags conflict; false otherwise
   */
  static bool nameConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2);
};

}

#endif // TAG_UTILS_H
