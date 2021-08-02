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
 * @copyright Copyright (C) 2015, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MATCHMEMBERS_H
#define MATCHMEMBERS_H

// Hoot
#include <hoot/core/util/HootException.h>

// Standard
#include <math.h>

namespace hoot
{

class MatchMembers
{
public:

  enum Type
  {
    None = 0,
    Poi = 1,
    Polygon = 2,
    Polyline = 4
  };

  MatchMembers() : _type(Type::None) {}
  MatchMembers(Type t) : _type(t) {}

  bool operator==(MatchMembers t) const { return t._type == _type; }
  bool operator!=(MatchMembers t) const { return t._type != _type; }

  Type toEnum() const { return _type; }

  QString toString() const
  {
    switch (_type)
    {
    case None:
      return "None";
    case Poi:
      return "Poi";
    case Polygon:
      return "Polygon";
    case Polyline:
      return "Polyline";
    default:
      throw HootException("Invalid match type.");
    }
  }

private:

  Type _type;
};

inline MatchMembers::Type operator|(const MatchMembers::Type& t1, const MatchMembers::Type& t2)
{
  return MatchMembers::Type((int)(t1) | (int)(t2));
}

inline MatchMembers operator|(const MatchMembers& m1, const MatchMembers& m2)
{
  return MatchMembers(m1.toEnum() | m2.toEnum());
}

inline bool operator&(const MatchMembers& m1, MatchMembers::Type t)
{
  return (int)m1.toEnum() & (int)t;
}

}

#endif // MATCHMEMBERS_H
