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

#ifndef VERSION_UTILS_H
#define VERSION_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Various utilities to be used when examining an element's version
 */
class VersionUtils
{

public:

  /**
   * Returns the number of elements with a changeset version less than one
   *
   * @param map the map to examine
   * @return the number of elements meeting the criteria
   */
  static int versionLessThanOneCount(const OsmMapPtr& map);

  /**
   * Logs a warning if any element in the map has a changeset version less than one
   *
   * @param map the map to examine
   * @return true if a warning was logged; false otherwise
   */
  static bool checkVersionLessThanOneCountAndLogWarning(const OsmMapPtr& map);
};

}

#endif // VERSION_UTILS_H
