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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYSUBLINE_H
#define WAYSUBLINE_H

#include "WayLocation.h"

#include <hoot/core/util/GeometryConverter.h>

namespace hoot
{

class WaySubline
{
public:
  WaySubline();

  WaySubline(const WayLocation& start, const WayLocation& end);

  WaySubline(const WaySubline& from);

  WaySubline(const WaySubline& from, const ConstOsmMapPtr &newMap);

  WaySubline& operator=(const WaySubline& from);

  bool contains(const WayLocation& wl) const;

  /**
   * Returns true if all the points in other overlap with this.
   */
  bool contains(const WaySubline& other) const;

  WaySubline expand(Meters d) const;

  ElementId getElementId() const { return _start.getWay()->getElementId(); }

  const WayLocation& getEnd() const { return _end; }

  Meters getLength() const;

  const ConstOsmMapPtr& getMap() const { return _start.getMap(); }

  const WayLocation& getStart() const { return _start; }

  const ConstWayPtr& getWay() const { return _start.getWay(); }

  bool isZeroLength() const { return _start == _end; }

  bool isValid() const { return _start.isValid() && _end.isValid(); }

  /**
   * Returns true if the two sublines have interior points in common. If they only touch at one
   * point then they do not overlap.
   */
  bool overlaps(const WaySubline& other) const;

  /**
   * This conceptually does the same as if you were to reverse the way without changing the
   * subline values. Or said another way, if you reverse the way you'll have to call this to
   * keep the WaySubline correct.
   */
  WaySubline reverse(const ConstWayPtr& reversedWay) const;

  QString toString() const;

  /**
   * Create a new way that represents this subline and return it. This way will not be added to a
   * map.
   */
  WayPtr toWay(const OsmMapPtr& map, GeometryConverter::NodeFactory* nf = 0) const;

  /**
   * Returns true if the two sublines have any points in common.
   */
  bool touches(const WaySubline& other) const;

private:

  WayLocation _start, _end;
};

bool operator==(const WaySubline& a, const WaySubline& b);


}

#endif // WAYSUBLINE_H
