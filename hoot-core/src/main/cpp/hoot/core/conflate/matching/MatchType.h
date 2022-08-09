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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MATCHTYPE_H
#define MATCHTYPE_H

// Standard
#include <math.h>

namespace hoot
{

class MatchType
{
public:

  enum Type
  {
    Miss = 0,
    Match = 1,
    Review = 2
  };

  MatchType(Type t) : _type(t) {}

  MatchType(double v)
  {
    int t = static_cast<int>(round(v));
    if (t < Miss || t > Review)
      throw HootException("Invalid match type.");
    _type = (Type)t;
  }

  MatchType(const QString& v)
  {
    QString l = v.toLower();
    if (l == "miss")
      _type = Miss;
    else if (l == "match")
      _type = Match;
    else if (l == "review")
      _type = Review;
    else
      throw HootException("Invalid match type.");
  }

  bool operator==(MatchType t) const { return t._type == _type; }
  bool operator!=(MatchType t) const { return t._type != _type; }

  Type toEnum() const { return _type; }

  QString toString() const
  {
    switch (_type)
    {
    case Miss:
      return "Miss";
    case Match:
      return "Match";
    case Review:
      return "Review";
    default:
      throw HootException("Invalid match type.");
    }
  }

  static bool isValidMatchTypeString(const QString& str)
  {
    const QString temp = str.toLower();
    return temp == "match" || temp == "review" || temp == "miss";
  }

private:

  Type _type;
};

}

#endif // MATCHTYPE_H
