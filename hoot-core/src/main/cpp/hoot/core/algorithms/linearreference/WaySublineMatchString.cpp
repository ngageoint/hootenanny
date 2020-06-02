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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "WaySublineMatchString.h"

#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

WaySublineMatchString::WaySublineMatchString(const WaySublineMatchString& other)
{
  // no need to check for overlaps, the incoming object should already be consistent.
  _matches = other._matches;
}

WaySublineMatchString::WaySublineMatchString(const MatchCollection& m)
{
  for (size_t i = 0; i < m.size(); i++)
  {
    for (size_t j = 0; j < m.size(); j++)
    {
      if (i != j && m[i].overlaps(m[j]))
      {
        LOG_VART(m);
        throw OverlappingMatchesException(
          "The match collection must not contain overlapping matches.");
      }
    }
  }
  _matches = m;
}

WaySublineMatchString::WaySublineMatchString(const WaySublineMatchString& other,
  const OsmMapPtr& newMap)
{
  _matches.resize(other._matches.size());

  for (size_t i = 0; i < other.getMatches().size(); i++)
  {
    _matches[i] = WaySublineMatch(other.getMatches()[i], newMap);
  }
}

bool WaySublineMatchString::contains(const WaySublineMatchString& other) const
{
  bool result = true;
  for (size_t i = 0; i < other._matches.size(); i++)
  {
    result = result && contains(other._matches[i]);
  }

  return result;
}

bool WaySublineMatchString::contains(const WaySublineMatch &other) const
{
  bool result = false;

  for (size_t i = 0; i < _matches.size(); i++)
  {
    if (_matches[i].getSubline1().contains(other.getSubline1()) ||
        _matches[i].getSubline2().contains(other.getSubline2()))
    {
      result = true;
    }
  }

  return result;
}

Meters WaySublineMatchString::getLength() const
{
  Meters result = 0;
  for (size_t i = 0; i < _matches.size(); i++)
  {
    result += _matches[i].getSubline1().getLength();
    result += _matches[i].getSubline2().getLength();
  }

  return result / 2.0;
}

vector<bool> WaySublineMatchString::getReverseVector1() const
{
  vector<bool> result(_matches.size(), false);
  return result;
}

vector<bool> WaySublineMatchString::getReverseVector2() const
{
  vector<bool> result(_matches.size());
  for (size_t i = 0; i < _matches.size(); i++)
  {
    result[i] = _matches[i].isReverseMatch();
  }
  return result;
}

WaySublineCollection WaySublineMatchString::getSublineString1() const
{
  WaySublineCollection result;
  for (size_t i = 0; i < _matches.size(); i++)
  {
    result.addSubline(_matches[i].getSubline1());
  }
  return result;
}

WaySublineCollection WaySublineMatchString::getSublineString2() const
{
  WaySublineCollection result;
  for (size_t i = 0; i < _matches.size(); i++)
  {
    result.addSubline(_matches[i].getSubline2());
  }
  return result;
}

bool WaySublineMatchString::isValid() const
{
  if (_matches.size() > 0)
  {
    for (size_t i = 0; i < _matches.size(); i++)
    {
      const WaySublineMatch& m = _matches[i];
      LOG_VART(m.getSubline1());
      LOG_VART(m.getSubline2());
      if (m.getSubline1().isZeroLength() || m.getSubline2().isZeroLength())
      {
        return false;
      }
    }

    return true;
  }
  else
  {
    return false;
  }
}

void WaySublineMatchString::removeEmptyMatches()
{
  LOG_TRACE("Removing empty matches...");
  MatchCollection copy = _matches;
  _matches.clear();
  for (size_t i = 0; i < copy.size(); i++)
  {
    const WaySublineMatch& m = _matches[i];
    if (!m.getSubline1().isZeroLength() && !m.getSubline2().isZeroLength())
    {
      _matches.push_back(m);
    }
  }
}

bool WaySublineMatchString::touches(const WaySublineMatchString& other) const
{
  WaySublineCollection wss1 = getSublineString1();
  WaySublineCollection wss2 = getSublineString2();
  WaySublineCollection owss1 = other.getSublineString1();
  WaySublineCollection owss2 = other.getSublineString2();
  return wss1.touches(owss1) ||
      wss2.touches(owss2) ||
      wss1.touches(owss2) ||
      wss2.touches(owss1);
}

QString WaySublineMatchString::toString() const
{
  QString result;
  //result += "subline 1: " + getSublineString1().toString() + "\n";
  //result += "subline 2: " + getSublineString2().toString() + "\n";
  result += QString("matches:\n");
  QStringList l;
  for (size_t i = 0; i < _matches.size(); i++)
  {
    l.append(_matches[i].toString());
  }
  result += l.join("\n");
  return result;
}

}
