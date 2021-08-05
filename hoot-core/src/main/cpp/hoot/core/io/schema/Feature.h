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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef RECORD_H
#define RECORD_H

// Qt
#include <QVariant>

#include <hoot/core/io/schema/StrictChecking.h>

namespace hoot
{

class FeatureDefinition;

class Feature
{
public:

  Feature(const std::shared_ptr<const FeatureDefinition>& d);

  /**
   * If f is valid then nothing happens, if f is invalid then an exception is thrown.
   * @throws FieldDefinition::InvalidValueException
   */
  void validate(StrictChecking strict);

  QString toString() const;

  const std::shared_ptr<const FeatureDefinition>& getFeatureDefinition() const
  { return _definition; }
  const QVariantMap& getValues() const { return _values; }

  void setField(const QString& key, const QVariant& value) { _values[key] = value; }

private:

  const std::shared_ptr<const FeatureDefinition>& _definition;
  QVariantMap _values;
};

}

#endif // RECORD_H
