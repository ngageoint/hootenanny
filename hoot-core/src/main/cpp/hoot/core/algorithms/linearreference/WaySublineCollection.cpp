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
#include "WaySublineCollection.h"

namespace hoot
{

WaySublineCollection::WaySublineCollection()
{
}

void WaySublineCollection::addSubline(const WaySubline& subline)
{
  if (_sublines.size() == 0)
  {
    _sublines.push_back(subline);
  }
  else
  {
    for (size_t i = 0; i < _sublines.size(); i++)
    {
      if (subline.overlaps(_sublines[i]))
      {
        throw HootException("A subline string may not contain overlapping sublines.");
      }
    }

    _sublines.push_back(subline);
  }
}

bool compareSublines(const WaySubline& ws1, const WaySubline& ws2)
{
  return ws1.getStart() < ws2.getStart();
}

Meters WaySublineCollection::getLength() const
{
  Meters result = 0;
  for (size_t i = 0; i < _sublines.size(); i++)
  {
    result += _sublines[i].getLength();
  }

  return result;
}

WaySublineCollection WaySublineCollection::invert() const
{
  WaySublineCollection result;
  if (_sublines.size() == 0)
  {
    return result;
  }

  // We are going to sort all the way sublines so we can generate the inverted sublines starting
  // at the beginning and working on to the end. We'll maintain a simple variable for where the
  // next subline starts and then use a little simple logic to determine when we've found a legit
  // inverted subline and push it on to the result.

  // make a copy so we can sort.
  vector<WaySubline> sublines = _sublines;
  sort(sublines.begin(), sublines.end(), compareSublines);

  WayLocation sublineStart = WayLocation(_sublines[0].getMap(), sublines[0].getWay(), 0, 0);
  // go through all the sorted sublines
  for (size_t i = 0; i < sublines.size(); i++)
  {
    if (sublineStart.getWay() != sublines[i].getWay())
    {
      // if this isn't an empty subline
      if (sublineStart.isLast() == false)
      {
        result.addSubline(WaySubline(sublineStart,
          WayLocation::createAtEndOfWay(_sublines[0].getMap(), sublineStart.getWay())));
      }
      sublineStart = WayLocation(_sublines[0].getMap(), sublines[i].getWay(), 0, 0);
    }

    assert(sublineStart.getWay() == sublines[i].getWay());
    if (sublineStart == sublines[i].getStart())
    {
      // an empty subline at the beginning
      sublineStart = sublines[i].getEnd();
    }
    else
    {
      // add another subline from the sublineStart to the beginning of the next subline.
      result.addSubline(WaySubline(sublineStart, sublines[i].getStart()));
      // the next negative subline starts at the end of this positive subline
      sublineStart = sublines[i].getEnd();
    }
  }

  // if we haven't reached the end, then add one more subline for the end of the line.
  if (sublineStart.isLast() == false)
  {
    result.addSubline(WaySubline(sublineStart,
      WayLocation::createAtEndOfWay(_sublines[0].getMap(), sublineStart.getWay())));
  }

  return result;
}

QString WaySublineCollection::toString() const
{
  QStringList l;
  for (size_t i = 0; i < _sublines.size(); i++)
  {
    l.append(_sublines[i].toString());
  }
  return l.join("\n");
}

bool WaySublineCollection::touches(const WaySublineCollection& other) const
{
  bool result = false;
  for (size_t i = 0; i < _sublines.size() && result == false; i++)
  {
    for (size_t j = 0; j < other.getSublines().size() && result == false; j++)
    {
      result = _sublines[i].touches(other._sublines[j]);
    }
  }

  return result;
}

}
