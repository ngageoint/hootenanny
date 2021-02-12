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
 * @copyright Copyright (C) 2015, 2017, 2018, 2020 Maxar (http://www.maxar.com/)
 */
#ifndef DOUBLEFIELDDEFINITION_H
#define DOUBLEFIELDDEFINITION_H

// Standard
#include <set>

#include <hoot/core/io/schema/FieldDefinition.h>

namespace hoot
{

class DoubleFieldDefinition : public FieldDefinition
{
public:

  DoubleFieldDefinition();
  virtual ~DoubleFieldDefinition() = default;

  void addEnumeratedValue(double v) { _enumeratedValues.insert(v); }

  virtual QVariant::Type getType() const { return QVariant::Double; }

  virtual QVariant getDefaultValue() const;

  double getMaxValue() const { return _max; }

  double getMinValue() const { return _min; }

  virtual bool hasDefaultValue() const;

  bool hasEnumeratedValue(double v) { return _enumeratedValues.find(v) != _enumeratedValues.end(); }

  void setDefaultValue(double v) { _defaultValue = v; }

  void setMaxValue(double max) { _max = max; }

  void setMinValue(double min) { _min = min; }

  virtual QString toString() const;

  virtual void validate(const QVariant& v, StrictChecking strict) const;

private:

  double _defaultValue;
  double _min;
  double _max;
  std::set<double> _enumeratedValues;
};

}
#endif // DOUBLEFIELDDEFINITION_H
