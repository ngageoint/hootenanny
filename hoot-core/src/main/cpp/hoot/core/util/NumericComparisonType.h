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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef NUMERIC_COMPARISON_TYPE_H
#define NUMERIC_COMPARISON_TYPE_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Enumeration dealing with comparisons between numeric values
 */
class NumericComparisonType
{

public:

  typedef enum Type
  {
    EqualTo = 0,
    LessThan,
    LessThanOrEqualTo,
    GreaterThan,
    GreaterThanOrEqualTo
  } Type;

  NumericComparisonType();
  NumericComparisonType(Type type);
  NumericComparisonType(int type);

  bool operator==(NumericComparisonType t) const;
  bool operator!=(NumericComparisonType t) const;

  Type getEnum() const { return _type; }

  QString toString() const;
  static Type fromString(QString typeString);

  /**
   * Determines if a value satisfies this numeric comparison
   *
   * @param valueToEvaluate the value to evaluate
   * @param comparisonValue the value to evaluate against
   * @return true if valueToEvaluate satisfies the configured relationship with comparisonValue;
   * false otherwise
   */
  bool satisfiesComparison(const double valueToEvaluate, const double comparisonValue) const;

private:

  NumericComparisonType::Type _type;

  static Type intToType(const int intType);
};

}

#endif // NUMERIC_COMPARISON_TYPE_H
