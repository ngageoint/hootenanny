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
#ifndef NUMERIC_COMPARISON_TYPE_H
#define NUMERIC_COMPARISON_TYPE_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Enumeration dealing with comparisons between numeric values
 */
class NumericComparisonType
{

public:

  typedef enum Type
  {
    EqualTo = 0,
    LessThan,
    LessThanOrEqualTo,
    GreaterThan,
    GreaterThanOrEqualTo
  } Type;

  NumericComparisonType() : _type(EqualTo) {}
  NumericComparisonType(Type type) : _type(type) {}

  bool operator==(NumericComparisonType t) const { return t._type == _type; }
  bool operator!=(NumericComparisonType t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  QString toString() const
  {
    switch (_type)
    {
    case NumericComparisonType::EqualTo:
      return "EqualTo";
    case NumericComparisonType::LessThan:
      return "LessThan";
    case NumericComparisonType::LessThanOrEqualTo:
      return "LessThanOrEqualTo";
    case NumericComparisonType::GreaterThan:
      return "GreaterThan";
    case NumericComparisonType::GreaterThanOrEqualTo:
      return "GreaterThanOrEqualTo";
    default:
      return QString("Unknown (%1)").arg(_type);
    }
  }

  static Type fromString(QString typeString)
  {
    typeString = typeString.toLower().trimmed();
    if (typeString == "equalto")
    {
      return EqualTo;
    }
    else if (typeString == "lessthan")
    {
      return LessThan;
    }
    else if (typeString == "lessthanorequalto")
    {
      return LessThanOrEqualTo;
    }
    else if (typeString == "greaterthan")
    {
      return GreaterThan;
    }
    else if (typeString == "greaterthanorequalto")
    {
      return GreaterThanOrEqualTo;
    }
    else
    {
      throw IllegalArgumentException("Invalid numeric comparison type string: " + typeString);
    }
  }

private:

  NumericComparisonType::Type _type;

};

}

#endif // NUMERIC_COMPARISON_TYPE_H
