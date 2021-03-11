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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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

  WayStringPtr getWayString1() override { return _ws1; }
  WayStringPtr getWayString2() override { return _ws2; }

  WayLocation map1To2(const WayLocation& l1, ElementId preferredEid) override;
  WayLocation map2To1(const WayLocation& l2, ElementId preferredEid) override;

  void setWayString1(const WayStringPtr& ws1) override { _ws1 = ws1; }
  void setWayString2(const WayStringPtr& ws2) override { _ws2 = ws2; }

private:

  WayStringPtr _ws1, _ws2;
  Meters _length1, _length2;
};

}

#endif // NAIVEWAYMATCHSTRINGMAPPING_H
