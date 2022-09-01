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
 * @copyright Copyright (C) 2015, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "StringFieldDefinition.h"

namespace hoot
{

QString StringFieldDefinition::toString() const
{
  return QString("String name: %1 default: %2").arg(getName(), _defaultValue);
}

QVariant StringFieldDefinition::getDefaultValue() const
{
  if (getDefaultIsNull())
    return QVariant();
  else
    return QVariant(_defaultValue);
}

bool StringFieldDefinition::hasDefaultValue() const
{
  return _defaultValue.isNull() == false || getDefaultIsNull();
}

void StringFieldDefinition::validate(const QVariant& v, StrictChecking strict) const
{
  if (getAllowNull() && v.isValid() == false) // the value is null, no problem.
    return;
  else if (!getAllowNull() && v.isValid() == false)
    _reportError(getName(), "Nulls are not allowed, but field was set to null.", strict);
  else if (v.canConvert(QVariant::String) == false)
    _reportError(getName(), "Could not convert string variant to string.", strict);
}

}
