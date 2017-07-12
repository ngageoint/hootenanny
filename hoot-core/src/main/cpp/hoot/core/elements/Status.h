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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef STATUS_H
#define STATUS_H

// Qt
#include <QString>

namespace hoot
{

class Status
{
public:
  typedef enum TypeEnum
  {
    Invalid = 0,
    Unknown1,
    Unknown2,
    Conflated
  } TypeEnum;

  typedef int Type;

  Status() { _type = Invalid; }
  Status(Type type) { _type = type; }

  bool operator==(Status t) const { return t._type == _type; }
  bool operator!=(Status t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  /**
   * @brief getInput Unlike the enumeration names and strings, this returns the zero-based number
   *   for the input.
   * @return The zero-based input for this status or throws an exception if this isn't an input.
   *   E.g. Unknown1 returns 0, Unknown2 return 1, etc.
   */
  int getInput() const;

  QString toString() const;

  // This is not pretty and it is a copy of "toString".
  // It is a lot easier to change this when the users want different text output
  // instead of changing "toString" and all of code, unit tests etc that rely on it.
  QString toTextStatus() const;

  /**
   * @return Returns true if the status is one of any input value (multiary)
   */
  bool isInput() const { return _type == Unknown1 || _type == Unknown2 || _type > Conflated; }

  /**
   * @deprecated
   * @brief isUnknown Provided for backwards compatibility.
   * @return Returns true if the status is one of the first two inputs.
   */
  bool isUnknown() { return _type == Unknown1 || _type == Unknown2; }

  /**
   * @brief fromString Parses type from either a human readable string or the numeric string.
   * @param typeString The string to parse.
   * @return The type parsed, or throws an exception if it is an invalid string.
   */
  static Type fromString(QString typeString);

private:
  Status::Type _type;
};

}

#endif // STATUS_H
