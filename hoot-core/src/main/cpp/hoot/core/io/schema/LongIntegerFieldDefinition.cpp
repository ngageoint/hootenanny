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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "LongIntegerFieldDefinition.h"

// hoot
#include <hoot/core/util/Log.h>

// Standard
#include <limits>
#include <sstream>

namespace hoot
{
using namespace std;

LongIntegerFieldDefinition::LongIntegerFieldDefinition()
{
  _defaultValue = -numeric_limits<long long>::max();
  _min = -numeric_limits<long long>::max();
  _max = numeric_limits<long long>::max();
}

QVariant LongIntegerFieldDefinition::getDefaultValue() const
{
  if (getDefaultIsNull())
  {
    return QVariant();
  }
  else
  {
    return QVariant(_defaultValue);
  }
}

bool LongIntegerFieldDefinition::hasDefaultValue() const
{
  return _defaultValue != -numeric_limits<long long>::max() || getDefaultIsNull();
}

QString LongIntegerFieldDefinition::toString() const
{
  stringstream ss;
  ss << _enumeratedValues;
  return QString("Int name: %1 min: %2 max: %3 default: %4 enumerations: %5")
      .arg(getName()).arg(_min).arg(_max).arg(_defaultValue).arg(QString::fromStdString(ss.str()));
}

void LongIntegerFieldDefinition::validate(const QVariant& v, StrictChecking strict) const
{
  if (getAllowNull() && v.isValid() == false)
  {
    // the value is null, no problem.
    return;
  }

  bool ok;
  v.toString().toLongLong(&ok);
  if (ok == false)
  {
    _reportError(getName(), "Cannot convert value to long long integer: " + v.toString(), strict);
  }
  long long vint = v.toLongLong();

  if (_enumeratedValues.find(vint) != _enumeratedValues.end())
  {
    // we're good
  }
  else if (_max == numeric_limits<long long>::max() && _min == -numeric_limits<long long>::max() &&
           _enumeratedValues.size() != 0)
  {
/* Setting Strict to StrictWarn stops Hoot dieing when an enumerated value is outside what
   is in the Schema.

   _reportError(getName(),
      "Integer value is not a valid enumeration and bounds are not set. " + v.toString(), strictWarn);
*/
    _reportError(getName(),
      "Integer value is not a valid enumeration and bounds are not set. " + v.toString(), strict);
      }
  else if (vint > _max)
  {
    _reportError(getName(), QString("Value is above max. value: %1 max: %2 (%3)")
      .arg(v.toString())
      .arg(_max)
      .arg(getName()), strict);
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
