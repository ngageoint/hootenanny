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
#ifndef WAYMATCHLOCATION_H
#define WAYMATCHLOCATION_H

#include "WaySubline.h"

namespace hoot
{

class WaySublineCollection;

class WaySublineMatch
{
public:
  WaySublineMatch();

  WaySublineMatch(const WaySublineMatch& other, const ConstOsmMapPtr &newMap);

  WaySublineMatch(const WaySubline& ws1, const WaySubline& ws2, bool reversed = false);

  /**
   * Returns the mean length of subline1 and subline2.
   */
  Meters getLength() const { return (getSubline1().getLength() + getSubline2().getLength()) / 2.0; }

  const WaySubline& getSubline1() const { return _ws1; }

  const WaySubline& getSubline2() const { return _ws2; }

  WaySublineCollection getSublineString1() const;

  WaySublineCollection getSublineString2() const;

  bool isReverseMatch() const { return _reversed; }

  bool isValid() const { return _ws1.isValid() && _ws2.isValid(); }

  /**
   * Returns true if the two sublines are part of the same way and they overlap.
   */
  bool overlaps(const WaySublineMatch& ws) const;

  QString toString() const;

private:
  WaySubline _ws1, _ws2;
  bool _reversed;
};

}

#endif // WAYMATCHLOCATION_H
