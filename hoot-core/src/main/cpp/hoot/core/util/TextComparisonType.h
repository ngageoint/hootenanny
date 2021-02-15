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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef TEXT_COMPARISON_TYPE_H
#define TEXT_COMPARISON_TYPE_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Enumeration dealing with comparisons between text values
 */
class TextComparisonType
{

public:

  typedef enum Type
  {
    EqualTo = 0,
    Contains,
    StartsWith,
    EndsWith
  } Type;

  TextComparisonType() : _type(EqualTo) {}
  TextComparisonType(Type type) : _type(type) {}

  bool operator==(TextComparisonType t) const { return t._type == _type; }
  bool operator!=(TextComparisonType t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  QString toString() const
  {
    switch (_type)
    {
    case TextComparisonType::EqualTo:
      return "EqualTo";
    case TextComparisonType::Contains:
      return "Contains";
    case TextComparisonType::StartsWith:
      return "StartsWith";
    case TextComparisonType::EndsWith:
      return "EndsWith";
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
    else if (typeString == "contains")
    {
      return Contains;
    }
    else if (typeString == "startswith")
    {
      return StartsWith;
    }
    else if (typeString == "endswith")
    {
      return EndsWith;
    }
    else
    {
      throw IllegalArgumentException("Invalid text comparison type string: " + typeString);
    }
  }

private:

  TextComparisonType::Type _type;

};

}

#endif // TEXT_COMPARISON_TYPE_H
