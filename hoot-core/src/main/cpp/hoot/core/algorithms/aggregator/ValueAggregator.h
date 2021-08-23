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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef VALUEAGGREGATOR_H
#define VALUEAGGREGATOR_H

// Hoot
#include <hoot/core/info/ApiEntityInfo.h>

namespace hoot
{

class ValueAggregator : public ApiEntityInfo
{
public:

  static QString className() { return "hoot::ValueAggregator"; }

  ValueAggregator() = default;
  virtual ~ValueAggregator() = default;

  /**
   * @brief aggregate aggregates the values in the vector d into a single value. The aggregation may
   * be something like min, max, mode, mean, sum, etc.
   * @param d The vector may be re-ordered, but the values shouldn't be removed or changed.
   */
  virtual double aggregate(std::vector<double>& d) const = 0;

  QString toString() const override { return ""; }
};

using ValueAggregatorPtr = std::shared_ptr<ValueAggregator>;

}


#endif // VALUEAGGREGATOR_H
