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
 * @copyright Copyright (C) 2015, 2017, 2018, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef INTEGERFIELDDEFINITION_H
#define INTEGERFIELDDEFINITION_H

// Standard
#include <set>

#include <hoot/core/io/schema/FieldDefinition.h>

namespace hoot
{

class IntegerFieldDefinition : public FieldDefinition
{
public:

  IntegerFieldDefinition();
  virtual ~IntegerFieldDefinition() = default;

  void addEnumeratedValue(int v) { _enumeratedValues.insert(v); }

  virtual QVariant getDefaultValue() const;

  virtual QVariant::Type getType() const { return QVariant::Int; }

  virtual bool hasDefaultValue() const;

  bool hasEnumeratedValue(int v) { return _enumeratedValues.find(v) != _enumeratedValues.end(); }

  void setDefaultValue(double v) { _defaultValue = v; }

  void setMaxValue(double max) { _max = max; }

  void setMinValue(double min) { _min = min; }

  virtual QString toString() const;

  virtual void validate(const QVariant& v, StrictChecking strict) const;

private:

  int _min;
  int _max;
  int _defaultValue;
  std::set<int> _enumeratedValues;
};

}

#endif // INTEGERFIELDDEFINITION_H
