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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef STATUS_H
#define STATUS_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

namespace hoot {

class Status
{
public:
  typedef enum Type
  {
    Invalid = 0,
    Unknown1,
    Unknown2,
    Conflated
  } Type;

  Status() { _type = Invalid; }
  Status(Type type) { _type = type; }

  bool operator==(Status t) const { return t._type == _type; }
  bool operator!=(Status t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  QString toString() const
  {
    switch (_type)
    {
    case Status::Invalid:
      return "Invalid";
    case Status::Unknown1:
      return "Unknown1";
    case Status::Unknown2:
      return "Unknown2";
    case Status::Conflated:
      return "Conflated";
    default:
      return QString("Unknown (%1)").arg(_type);
    }
  }

  // This is not pretty and it is a copy of "toString".
  // It is a lot easier to change this when the users want different text output
  // instead of changing "toString" and all of code, unit tests etc that rely on it.
  QString toTextStatus() const
  {
    switch (_type)
    {
    case Status::Invalid:
      return "Invalid";
    case Status::Unknown1:
      return "Input1";
    case Status::Unknown2:
      return "Input2";
    case Status::Conflated:
      return "Conflated";
    default:
      return QString("Unknown (%1)").arg(_type);
    }
  }

  bool isUnknown() { return _type == Unknown1 || _type == Unknown2; }

  static Type fromString(QString typeString)
  {
    typeString = typeString.toLower();
    if (typeString == "invalid")
    {
      return Invalid;
    }
    else if (typeString == "unknown1" || typeString == "input1")
    {
      return Unknown1;
    }
    else if (typeString == "unknown2" || typeString == "input2")
    {
      return Unknown2;
    }
    else if (typeString == "conflated")
    {
      return Conflated;
    }
    else
    {
      throw HootException("Invalid element type string: " + typeString);
    }
  }

private:
  Status::Type _type;
};

}

#endif // STATUS_H
