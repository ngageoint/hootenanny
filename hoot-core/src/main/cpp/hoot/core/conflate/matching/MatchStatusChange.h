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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MATCH_STATUS_CHANGE_H
#define MATCH_STATUS_CHANGE_H

// Hoot
#include <hoot/core/util/HootException.h>

namespace hoot
{

class MatchStatusChange
{
public:

  enum Type
  {
    MatchToMiss = 0,
    MatchToReview,
    MissToMatch,
    MissToReview,
    ReviewToMatch,
    ReviewToMiss
  };

  MatchStatusChange(Type t) : _type(t) {}

  MatchStatusChange(QString v)
  {
    QString l = v.toLower();
    if (l == "matchtomiss")
    {
      _type = MatchToMiss;
    }
    else if (l == "matchtoreview")
    {
      _type = MatchToReview;
    }
    else if (l == "misstomatch")
    {
      _type = MissToMatch;
    }
    else if (l == "misstoreview")
    {
      _type = MissToReview;
    }
    else if (l == "reviewtomatch")
    {
      _type = ReviewToMatch;
    }
    else if (l == "reviewtomiss")
    {
      _type = ReviewToMiss;
    }
    else
    {
      throw HootException("Invalid match status change type.");
    }
  }

  bool operator==(MatchStatusChange t) const { return t._type == _type; }
  bool operator!=(MatchStatusChange t) const { return t._type != _type; }

  Type toEnum() const { return _type; }

  QString toString() const
  {
    switch (_type)
    {
    case MatchToMiss:
      return "MatchToMiss";
    case MatchToReview:
      return "MatchToReview";
    case MissToMatch:
      return "MissToMatch";
    case MissToReview:
      return "MissToReview";
    case ReviewToMatch:
      return "ReviewToMatch";
    case ReviewToMiss:
      return "ReviewToMiss";
    default:
      throw HootException("Invalid match status change type.");
    }
  }

private:

  Type _type;
};

}

#endif // MATCH_STATUS_CHANGE_H
