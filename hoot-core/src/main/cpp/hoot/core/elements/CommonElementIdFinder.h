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

#ifndef COMMON_ELEMENT_ID_FINDER_H
#define COMMON_ELEMENT_ID_FINDER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * TODO
 */
class CommonElementIdFinder
{

public:

  CommonElementIdFinder() = default;
  ~CommonElementIdFinder() = default;

  /**
   * TODO
   *
   * @param map1
   * @param map2
   * @return
   */
  static QSet<ElementId> findCommonElementIds(const OsmMapPtr& map1, const OsmMapPtr& map2);
};

}

#endif // COMMON_ELEMENT_ID_FINDER_H
