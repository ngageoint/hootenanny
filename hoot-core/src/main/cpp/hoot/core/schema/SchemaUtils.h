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

#ifndef SCHEMA_UTILS_H
#define SCHEMA_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

/**
 * Various utilities for working with the schema. Helps keep OsmMap out of OsmSchema as a dependency.
 */
class SchemaUtils
{

public:

  /**
   * TODO
   *
   * @param map
   * @return
   */
  static bool anyElementsHaveType(const ConstOsmMapPtr& map);

  /**
   * TODO
   *
   * @param map
   * @return
   */
  static bool allElementsHaveType(const ConstOsmMapPtr& map);
};

}

#endif // SCHEMA_UTILS_H
