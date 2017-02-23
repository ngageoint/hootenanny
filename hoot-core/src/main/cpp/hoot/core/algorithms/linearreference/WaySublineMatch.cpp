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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "WaySublineMatch.h"

#include "WaySublineCollection.h"

namespace hoot
{

WaySublineMatch::WaySublineMatch()
{
  _reversed = false;
}

WaySublineMatch::WaySublineMatch(const WaySublineMatch& other, const ConstOsmMapPtr& newMap)
{
  _ws1 = WaySubline(other.getSubline1(), newMap);
  _ws2 = WaySubline(other.getSubline2(), newMap);
  _reversed = other.isReverseMatch();
}

WaySublineMatch::WaySublineMatch(const WaySubline& ws1, const WaySubline& ws2, bool reversed) :
  _ws1(ws1),
  _ws2(ws2)
{
  assert(_ws1.getWay() != _ws2.getWay());
  _reversed = reversed;
}

WaySublineCollection WaySublineMatch::getSublineString1() const
{
  WaySublineCollection result;
  result.addSubline(_ws1);
  return result;
}

WaySublineCollection WaySublineMatch::getSublineString2() const
{
  WaySublineCollection result;
  result.addSubline(_ws2);
  return result;
}

bool WaySublineMatch::overlaps(const WaySublineMatch& ws) const
{
  return (ws.getSubline1().getWay() == getSubline1().getWay() &&
          ws.getSubline1().overlaps(getSubline1())) ||
         (ws.getSubline2().getWay() == getSubline2().getWay() &&
          ws.getSubline2().overlaps(getSubline2()));
}

QString WaySublineMatch::toString() const
{
  return (isReverseMatch() ? "reversed, " : "") + QString("subline 1: ") +
    getSubline1().toString() + "\n" + "subline 2: " + getSubline2().toString();
}

}
