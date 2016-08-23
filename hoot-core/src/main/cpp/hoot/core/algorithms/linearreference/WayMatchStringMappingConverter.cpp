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
#include "WayMatchStringMappingConverter.h"

namespace hoot
{

WayMatchStringMappingConverter::WayMatchStringMappingConverter()
{
}

WaySublineMatchStringPtr WayMatchStringMappingConverter::toWaySublineMatchString(
  WayMatchStringMappingPtr mapping)
{
  WayStringPtr wstr1 = mapping->getWayString1();
  WayStringPtr wstr2 = mapping->getWayString2();
  WayLocation wl1 = wstr1->at(0).getStart();
  WayLocation wl2 = wstr2->at(0).getStart();
  int i1 = 0;
  int i2 = 0;

  WaySublineMatchString::MatchCollection coll;

  // keep stepping through till you reach the end of both ways.
  while (wl1 != wstr1->back().getEnd() || wl2 != wstr2->back().getEnd())
  {
    WaySubline& ws1 = wstr1->at(i1);
    WaySubline& ws2 = wstr2->at(i2);

    // calculate the distance on str1 that the next subline map to. Not that we aren't calculating
    // the distance on str2, but mapping from str2 to str1, then calculate the distance.
    Meters dnext1 = wstr1->calculateDistanceOnString(ws1.getEnd());
    Meters dnext2 = wstr1->calculateDistanceOnString(mapping->map2To1(ws2.getEnd()));

    WaySubline sub1, sub2;

    // Take the shorter of the two steps
    if (dnext1 < dnext2)
    {
      sub1 = WaySubline(wl1, ws1.getEnd());
      sub2 = WaySubline(wl2, mapping->map1To2(ws1.getEnd(), ws2.getElementId()));
    }
    else
    {
      sub1 = WaySubline(wl1, mapping->map2To1(ws2.getEnd(), ws1.getElementId()));
      sub2 = WaySubline(wl2, ws2.getEnd());
    }

    bool reversed = sub1.isBackwards() != sub2.isBackwards();
    // make a temp copy so we can force the subline forward.
    WaySubline tmp1 = sub1;
    WaySubline tmp2 = sub2;
    tmp1.ensureForwards();
    tmp2.ensureForwards();
    coll.push_back(WaySublineMatch(tmp1, tmp2, reversed));

    wl1 = sub1.getEnd();
    wl2 = sub2.getEnd();

    if (wl1 == wstr1->at(i1).getEnd() && i1 < wstr1->getSize() - 1)
    {
      ++i1;
      wl1 = wstr1->at(i1).getStart();
    }
    if (wl2 == wstr2->at(i2).getEnd() && i2 < wstr2->getSize() - 1)
    {
      ++i2;
      wl2 = wstr2->at(i2).getStart();
    }
  }

  return WaySublineMatchStringPtr(new WaySublineMatchString(coll));
}

}
