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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#include "NumericComparisonType.h"

// Qt
#include <QVariant>

namespace hoot
{

NumericComparisonType::NumericComparisonType() :
_type(EqualTo)
{
}

NumericComparisonType::NumericComparisonType(Type type) :
_type(type)
{
}

NumericComparisonType::NumericComparisonType(int type) :
_type(intToType(type))
{
}

bool NumericComparisonType::operator==(NumericComparisonType t) const
{
  return t._type == _type;
}

bool NumericComparisonType::operator!=(NumericComparisonType t) const
{
  return t._type != _type;
}

NumericComparisonType::Type NumericComparisonType::intToType(const int intType)
{
  switch (intType)
  {
    case 0:
      return NumericComparisonType::EqualTo;
    case 1:
      return NumericComparisonType::LessThan;
    case 2:
      return NumericComparisonType::LessThanOrEqualTo;
    case 3:
      return NumericComparisonType::GreaterThan;
    case 4:
      return NumericComparisonType::GreaterThanOrEqualTo;
    default:
      throw IllegalArgumentException("Invalid comparison type: " + intType);
  }
}

QString NumericComparisonType::toString() const
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

NumericComparisonType::Type NumericComparisonType::fromString(QString typeString)
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

bool NumericComparisonType::satisfiesComparison(
  const double valueToEvaluate, const double comparisonValue) const
{
  switch (_type)
  {
    case NumericComparisonType::EqualTo:
      return valueToEvaluate == comparisonValue;
    case NumericComparisonType::LessThan:
      return valueToEvaluate < comparisonValue;
    case NumericComparisonType::LessThanOrEqualTo:
      return valueToEvaluate <= comparisonValue;
    case NumericComparisonType::GreaterThan:
      return valueToEvaluate > comparisonValue;
    case NumericComparisonType::GreaterThanOrEqualTo:
      return valueToEvaluate >= comparisonValue;
    default:
      throw IllegalArgumentException("Invalid comparison type: " + _type);
  }
}

}
