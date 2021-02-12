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
 * @copyright Copyright (C) 2016, 2019 Maxar (http://www.maxar.com/)
 */
#ifndef WAYMAPPING_H
#define WAYMAPPING_H

#include <hoot/core/algorithms/linearreference/WayLocation.h>

namespace hoot
{

/**
 * Given a WaySublineMatchString, produce a mapping from one way string to the other.
 */
class WayMapping
{
public:

  virtual ~WayMapping();

  /**
   * Given a point on subline string 1, return the corresponding point on subline string 2.
   */
  virtual WayLocationPtr map1to2(ConstWayLocationPtr wl1) const = 0;

  /**
   * Given a point on subline string 2, return the corresponding point on subline string 1.
   */
  virtual WayLocationPtr map2to1(ConstWayLocationPtr wl1) const = 0;
};

}

#endif // WAYMAPPING_H
