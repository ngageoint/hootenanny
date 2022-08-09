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
#include "DoubleFieldDefinition.h"

// Standard
#include <limits>
#include <sstream>

namespace hoot
{
using namespace std;

DoubleFieldDefinition::DoubleFieldDefinition()
{
  _defaultValue = -numeric_limits<double>::max();
  _min = -numeric_limits<double>::max();
  _max = numeric_limits<double>::max();
}

QVariant DoubleFieldDefinition::getDefaultValue() const
{
  if (getDefaultIsNull())
    return QVariant();
  else
    return QVariant(_defaultValue);
}

bool DoubleFieldDefinition::hasDefaultValue() const
{
  return _defaultValue != -numeric_limits<double>::max() || getDefaultIsNull();
}

QString DoubleFieldDefinition::toString() const
{
  stringstream ss;
  ss << _enumeratedValues;
  return QString("Double name: %1 min: %2 max: %3 default: %4 enumerations: %5")
      .arg(getName()).arg(_min).arg(_max).arg(_defaultValue).arg(QString::fromStdString(ss.str()));
}

void DoubleFieldDefinition::validate(const QVariant& v, StrictChecking strict) const
{
  // the value is null, no problem.
  if (getAllowNull() && v.isValid() == false)
    return;

  bool ok;
  v.toString().toDouble(&ok);
  if (ok == false)
    _reportError(getName(), "Unable to convert value to a double: " + v.toString(), strict);

  double d = v.toDouble();

  if (_enumeratedValues.find(d) != _enumeratedValues.end())
  {
    // we're good
  }
  else if (d < _min || d > _max)
  {
    _reportError(getName(), QString("Double value is out of bounds value: %1 min: %2 max: %3")
                 .arg(d).arg(_min).arg(_max), strict);
  }
}

}
