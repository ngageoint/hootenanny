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
#include "WaySublineCollection.h"

using namespace std;

namespace hoot
{

int WaySublineCollection::logWarnCount = 0;

void WaySublineCollection::addSubline(const WaySubline& subline)
{
  if (_sublines.size() == 0)
    _sublines.push_back(subline);
  else
  {
    for (const auto& sub : _sublines)
    {
      if (subline.overlaps(sub))
      {
        // We used to throw here, but found a situation where what otherwise would have been a
        // perfectly good diff couldn't be generated b/c of this error. So, switched to log warnings
        // instead with no noticeable negative impact on conflate output.

        // throw HootException("A subline string may not contain overlapping sublines.");
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("A subline string may not contain overlapping sublines.");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
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
  for (const auto& subline : _sublines)
    result += subline.getLength();
  return result;
}

WaySublineCollection WaySublineCollection::invert() const
{
  WaySublineCollection result;
  if (_sublines.empty())
    return result;

  // We are going to sort all the way sublines, so we can generate the inverted sublines starting
  // at the beginning and working on to the end. We'll maintain a simple variable for where the
  // next subline starts and then use a little simple logic to determine when we've found a legit
  // inverted subline and push it on to the result.

  // make a copy so we can sort
  vector<WaySubline> sublines = _sublines;
  sort(sublines.begin(), sublines.end(), compareSublines);

  WayLocation sublineStart = WayLocation(_sublines[0].getMap(), sublines[0].getWay(), 0, 0);
  // go through all the sorted sublines
  for (const auto& sub : sublines)
  {
    if (sublineStart.getWay() != sub.getWay())
    {
      // if this isn't an empty subline
      if (sublineStart.isLast() == false)
      {
        result.addSubline(WaySubline(sublineStart,
          WayLocation::createAtEndOfWay(_sublines[0].getMap(), sublineStart.getWay())));
      }
      sublineStart = WayLocation(_sublines[0].getMap(), sub.getWay(), 0, 0);
    }

    assert(sublineStart.getWay() == sub.getWay());
    if (sublineStart == sub.getStart()) // an empty subline at the beginning
      sublineStart = sub.getEnd();
    else
    {
      // add another subline from the sublineStart to the beginning of the next subline
      result.addSubline(WaySubline(sublineStart, sub.getStart()));
      // the next negative subline starts at the end of this positive subline
      sublineStart = sub.getEnd();
    }
  }

  // If we haven't reached the end, then add one more subline for the end of the line.
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
  for (const auto& subline : _sublines)
    l.append(subline.toString());
  return l.join("\n");
}

bool WaySublineCollection::touches(const WaySublineCollection& other) const
{
  for (const auto& sub1 : _sublines)
  {
    for (const auto& sub2 : other.getSublines())
    {
      if (sub1.touches(sub2))
        return true;
    }
  }
  return false;
}

}
