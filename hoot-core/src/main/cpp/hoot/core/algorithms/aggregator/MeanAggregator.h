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
#ifndef MEANAGGREGATOR_H
#define MEANAGGREGATOR_H

#include <hoot/core/algorithms/aggregator/ValueAggregator.h>

namespace hoot
{

class MeanAggregator : public ValueAggregator
{
public:

  static QString className() { return "MeanAggregator"; }

  MeanAggregator() = default;
  ~MeanAggregator() = default;

  double aggregate(std::vector<double>& d) const override;

  QString toString() const override { return "MeanAggregator"; }

  QString getDescription() const override
  { return "Aggregates data based on the mean value"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }
};

}

#endif // MEANAGGREGATOR_H
