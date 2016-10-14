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
#include "NaiveWayMatchStringMapping.h"

namespace hoot
{

NaiveWayMatchStringMapping::NaiveWayMatchStringMapping(WayStringPtr str1,
  WayStringPtr str2) :
  _ws1(str1),
  _ws2(str2)
{
  _length1 = _ws1->calculateLength();
  _length2 = _ws2->calculateLength();
}

WayLocation NaiveWayMatchStringMapping::map1To2(WayLocation l1, ElementId preferredEid)
{
  // be certain that the ends match exactly (avoid floating point rounding issues)
  if (_ws1->at(0).getStart() == l1)
  {
    return _ws2->at(0).getStart();
  }
  if (_ws1->back().getEnd() == l1)
  {
    return _ws2->back().getEnd();
  }

  Meters distanceOn1 = _ws1->calculateDistanceOnString(l1);
  double p = distanceOn1 / _length1;
  Meters distanceOn2 = p * _length2;
  return _ws2->calculateLocationFromStart(distanceOn2, preferredEid);
}

WayLocation NaiveWayMatchStringMapping::map2To1(WayLocation l2, ElementId preferredEid)
{
  // be certain that the ends match exactly (avoid floating point rounding issues)
  if (_ws2->at(0).getStart() == l2)
  {
    return _ws1->at(0).getStart();
  }
  if (_ws2->back().getEnd() == l2)
  {
    return _ws1->back().getEnd();
  }

  Meters distanceOn2 = _ws2->calculateDistanceOnString(l2);
  double p = distanceOn2 / _length2;
  Meters distanceOn1 = p * _length1;
  return _ws1->calculateLocationFromStart(distanceOn1, preferredEid);
}

}
