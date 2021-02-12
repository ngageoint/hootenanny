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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */
#ifndef NAIVEWAYMATCHSTRINGMAPPING_H
#define NAIVEWAYMATCHSTRINGMAPPING_H

#include <hoot/core/algorithms/linearreference/WayMatchStringMapping.h>

namespace hoot
{

class WayString;

class NaiveWayMatchStringMapping : public WayMatchStringMapping
{
public:

  NaiveWayMatchStringMapping(WayStringPtr str1, WayStringPtr str2);
  virtual ~NaiveWayMatchStringMapping() = default;

  virtual WayStringPtr getWayString1() { return _ws1; }
  virtual WayStringPtr getWayString2() { return _ws2; }

  virtual WayLocation map1To2(WayLocation l1, ElementId preferredEid) override;
  virtual WayLocation map2To1(WayLocation l2, ElementId preferredEid) override;

  virtual void setWayString1(const WayStringPtr& ws1) { _ws1 = ws1; }
  virtual void setWayString2(const WayStringPtr& ws2) { _ws2 = ws2; }

private:

  WayStringPtr _ws1, _ws2;
  Meters _length1, _length2;
};

}

#endif // NAIVEWAYMATCHSTRINGMAPPING_H
