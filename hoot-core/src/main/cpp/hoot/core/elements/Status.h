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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef STATUS_H
#define STATUS_H

namespace hoot
{

class Status
{
public:

  enum TypeEnum
  {
    Invalid = 0,
    Unknown1,
    Unknown2,
    Conflated,
    TagChange, // Tags have been changed, but not geometry
    EnumEnd // Used for calculating multiary inputs
  };

  using Type = int;

  Status() { _type = Invalid; }
  Status(Type type) { _type = type; }

  bool operator==(Status t) const { return t._type == _type; }
  bool operator!=(Status t) const { return t._type != _type; }

  /**
   * @brief fromInput Create a status object from the zero-based input number
   * @return Return Unknown1 for i=0, Unknown2 for i=1, Input003 for i=2, etc.
   */
  static Status fromInput(int i);

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
   * @brief isUnknown Provided for backwards compatibility.
   * @return Returns true if the status is one of the first two inputs.
   */
  bool isUnknown() const { return _type == Unknown1 || _type == Unknown2; }

  /**
   * @brief fromString Parses type from either a human readable status string or the numeric status
   * string.
   * @param statusString The status string to parse.
   * @return The status parsed or throws an exception if it is an invalid status string.
   */
  static Type fromString(QString statusString);

private:

  Status::Type _type;
};

}

#endif // STATUS_H
