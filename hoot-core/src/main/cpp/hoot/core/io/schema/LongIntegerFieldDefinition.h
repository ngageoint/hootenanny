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
#ifndef LONGINTEGERFIELDDEFINITION_H
#define LONGINTEGERFIELDDEFINITION_H

// Standard
#include <set>

#include "FieldDefinition.h"

namespace hoot
{
using namespace std;

class LongIntegerFieldDefinition : public FieldDefinition
{
public:
  LongIntegerFieldDefinition();

  void addEnumeratedValue(long long v) { _enumeratedValues.insert(v); }

  virtual QVariant getDefaultValue() const;

  virtual QVariant::Type getType() const { return QVariant::LongLong; }

  virtual bool hasDefaultValue() const;

  bool hasEnumeratedValue(long long v) { return _enumeratedValues.find(v) != _enumeratedValues.end(); }

  void setDefaultValue(long long v) { _defaultValue = v; }

  void setMaxValue(long long max) { _max = max; }

  void setMinValue(long long min) { _min = min; }

  virtual QString toString() const;

  virtual void validate(const QVariant& v, StrictChecking strict) const;

private:
  long long _min;
  long long _max;
  long long _defaultValue;
  set<long long> _enumeratedValues;
};

}

#endif // LONGINTEGERFIELDDEFINITION_H
