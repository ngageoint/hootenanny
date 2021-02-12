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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "Feature.h"

// hoot
#include <hoot/core/io/schema/FeatureDefinition.h>
#include <hoot/core/io/schema/FieldDefinition.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

Feature::Feature(const std::shared_ptr<const FeatureDefinition>& d) : _definition(d)
{
}

QString Feature::toString() const
{
  return hoot::toString(_values);
}

void Feature::validate(StrictChecking strict)
{
  // make sure we didn't get more fields than necessary
  for (QVariantMap::const_iterator it = _values.begin(); it != _values.end(); ++it)
  {
    if (_definition->hasField(it.key()) == false)
    {
      QString error = QString("Returned a field name that isn't appropriate for this layer. %1")
          .arg(it.key());
      if (strict == StrictOn)
      {
        throw FieldDefinition::InvalidValueException(it.key(), error);
      }
      else if (strict == StrictWarn)
      {
        LOG_WARN(error);
      }
    }
  }

  for (size_t i = 0; i < _definition->getFieldCount(); ++i)
  {
    // is
    std::shared_ptr<const FieldDefinition> d = _definition->getFieldDefinition(i);
    if (_values.contains(d->getName()) == false)
    {
      if (d->hasDefaultValue() == false)
      {
        QString error = QString("Field has no default value and no value was specified. (%1)")
          .arg(d->getName());
        if (strict == StrictOn)
        {
          throw FieldDefinition::InvalidValueException(d->getName(), error);
        }
        else if (strict == StrictWarn)
        {
          LOG_WARN(error);
        }
      }
      else
      {
        _values[d->getName()] = d->getDefaultValue();
      }
    }
    else
    {
      d->validate(_values[d->getName()], strict);
    }
  }
}

}
