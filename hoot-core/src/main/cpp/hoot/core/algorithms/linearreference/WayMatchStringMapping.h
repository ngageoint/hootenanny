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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYMATCHSTRINGMAPPING_H
#define WAYMATCHSTRINGMAPPING_H

#include "WayString.h"
#include "WaySublineMatchString.h"

namespace hoot
{

/**
 * Maintains a mapping from one waystring to another. This allows the caller to get a corresponding
 * WayLocation on WayString1 for a WayLocation on WayString2, or vice versa.
 *
 * This interface guarantees:
 *
 * - The beginning of WayString1 will map to the beginning of WayString2
 * - The end of WayString1 will map to the end of WayString2
 * - The mapping is not necessarily commutative. For instance, you could have multiple points on
 *   1 map to a single point on 2 so that single point on 2 may map to any of the corresponding
 *   points on 1.
 * - The results will be consistent for the same input.
 *
 * If WayStringPtr or any of its children change then the operation of this class is undefined. If
 * You do change or move a WayStringPtr or its children please call setWayString* accordingly to
 * update any underlying data structures.
 */
class WayMatchStringMapping
{
public:
  WayMatchStringMapping();

  virtual WayStringPtr getWayString1() = 0;

  virtual WayStringPtr getWayString2() = 0;

  /**
   * @param preferedEid Prefer to use this element ID if possible. (e.g. if the mapped point falls
   *        between two sublines.
   */
  virtual WayLocation map1To2(WayLocation l1, ElementId preferedEid = ElementId()) = 0;

  /**
   * @param preferedEid Prefer to use this element ID if possible. (e.g. if the mapped point falls
   *        between two sublines.
   */
  virtual WayLocation map2To1(WayLocation l2, ElementId preferedEid = ElementId()) = 0;

  virtual void setWayString1(const WayStringPtr& ws1) = 0;

  virtual void setWayString2(const WayStringPtr& ws2) = 0;

};

typedef boost::shared_ptr<WayMatchStringMapping> WayMatchStringMappingPtr;
typedef boost::shared_ptr<const WayMatchStringMapping> ConstWayMatchStringMappingPtr;

}

#endif // WAYMATCHSTRINGMAPPING_H
