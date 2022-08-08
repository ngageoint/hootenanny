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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef MAXAGGREGATOR_H
#define MAXAGGREGATOR_H

#include <hoot/core/algorithms/aggregator/ValueAggregator.h>

namespace hoot
{

class MaxAggregator : public ValueAggregator
{
public:

  /**
   * @brief className Returns the name of the class
   * @return a class name
   */
  static QString className() { return "MaxAggregator"; }

  /**
   * @brief Constructor
   */
  MaxAggregator() = default;
  /**
   * @brief Destructor
   */
  ~MaxAggregator() = default;

  /**
   * @see ValueAggregator
   */
  double aggregate(std::vector<double>& d) const override;

  /**
   * @see ApiEntityInfo
   */
  QString toString() const override { return "MaxAggregator"; }
  /**
   * @see ApiEntityInfo
   */
  QString getDescription() const override
  { return "Aggregates data based on the maximum value"; }
  /**
   * @see ApiEntityInfo
   */
  QString getName() const override { return className(); }
  /**
   * @see ApiEntityInfo
   */
  QString getClassName() const override { return className(); }
};

}

#endif // MAXAGGREGATOR_H
