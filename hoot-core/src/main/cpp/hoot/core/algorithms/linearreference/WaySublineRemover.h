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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#ifndef WAY_SUBLINE_REMOVER_H
#define WAY_SUBLINE_REMOVER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class WayLocation;
class WaySubline;

/**
 * @brief The WaySublineRemover class removes a sublines from ways.
 */
class WaySublineRemover
{
public:

  static QString className() { return "WaySublineRemover"; }

  /**
   * @brief removeSubline removes a subline from a way.
   * @param way the way to modify
   * @param subline the subline in the way to remove
   * @param map the map owning the way
   * @return IDs of the ways created and added back to the map as a result of the subline removal
   */
  static std::vector<ElementId> removeSubline(
    const WayPtr& way, const WaySubline& subline, const OsmMapPtr& map);
  /**
   * @brief removeSubline removes a subline from a way.
   * @param way the way to modify
   * @param start the starting point at which to remove a subline from the way
   * @param end the ending point at which to remove a subline from the way
   * @param map the map owning the way
   * @return IDs of the ways created and added back to the map as a result of the subline removal
   */
  static std::vector<ElementId> removeSubline(
    const WayPtr& way, const WayLocation& start, const WayLocation& end, const OsmMapPtr& map);

private:

  /*
   * Splits a way at the specified split location and keeps either the first of the split segments
   * or the second.
   */
  static std::vector<ElementId> _split(
    const WayPtr& way, const WayLocation& splitLocation, const OsmMapPtr& map,
    const bool keepFirstSegment);
};

}

#endif // WAY_SUBLINE_REMOVER_H
