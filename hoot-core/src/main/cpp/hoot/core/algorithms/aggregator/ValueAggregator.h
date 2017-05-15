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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef VALUEAGGREGATOR_H
#define VALUEAGGREGATOR_H

// Qt
#include <QString>

// Standard
#include <vector>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{

class ValueAggregator
{
public:

  static std::string className() { return "hoot::ValueAggregator"; }

  /**
   * Aggregates the values in the vector d into a single value. The aggregation may be something
   * like min, max, mode, mean, sum, etc.
   *
   * @param d The vector may be re-ordered, but the values shouldn't be removed or changed.
   */
  virtual double aggregate(std::vector<double>& d) const = 0;

  virtual QString toString() const = 0;
};

typedef boost::shared_ptr<ValueAggregator> ValueAggregatorPtr;

}


#endif // VALUEAGGREGATOR_H
