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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */
#include "IntegerFieldDefinition.h"

// Standard
#include <limits>
#include <sstream>

using namespace std;

namespace hoot
{

IntegerFieldDefinition::IntegerFieldDefinition()
  : _min(-numeric_limits<int>::max()),
    _max(numeric_limits<int>::max()),
    _defaultValue(-numeric_limits<int>::max())
{
}

QVariant IntegerFieldDefinition::getDefaultValue() const
{
  if (getDefaultIsNull())
    return QVariant();
  else
    return QVariant(_defaultValue);
}

bool IntegerFieldDefinition::hasDefaultValue() const
{
  return _defaultValue != -numeric_limits<int>::max() || getDefaultIsNull();
}

QString IntegerFieldDefinition::toString() const
{
  stringstream ss;
  ss << _enumeratedValues;
  return QString("Int name: %1 min: %2 max: %3 default: %4 enumerations: %5")
      .arg(getName()).arg(_min).arg(_max).arg(_defaultValue).arg(QString::fromStdString(ss.str()));
}

void IntegerFieldDefinition::validate(const QVariant& v, StrictChecking strict) const
{
  // the value is null, no problem.
  if (getAllowNull() && v.isValid() == false)
    return;

  bool ok;
  v.toString().toInt(&ok);
  if (ok == false)
    _reportError(getName(), "Cannot convert value to long integer: " + v.toString(), strict);

  int vint = v.toInt();

  if (_enumeratedValues.find(vint) != _enumeratedValues.end())
    return;
  else if (_max == numeric_limits<int>::max() && _min == -numeric_limits<int>::max() && !_enumeratedValues.empty())
    _reportError(getName(), "Integer value is not a valid enumeration and bounds are not set. " + v.toString(), strict);
  else if (vint > _max)
  {
    _reportError(getName(), QString("Value is above max. value: %1 max: %2 (%3)")
                 .arg(v.toString())
                 .arg(_max)
                 .arg(getName()),
                 strict);
  }
  else if (vint < _min)
  {
    _reportError(getName(), QString("Value is below min. value: %1 min: %2 (%3)")
                 .arg(v.toString())
                 .arg(_min)
                 .arg(getName()),
                 strict);
  }
}

}
