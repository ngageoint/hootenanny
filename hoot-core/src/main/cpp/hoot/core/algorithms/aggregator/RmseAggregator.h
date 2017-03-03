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
#ifndef RMSEAGGREGATOR_H
#define RMSEAGGREGATOR_H

// Standard
#include <math.h>

#include "ValueAggregator.h"

namespace hoot
{

using namespace std;

/**
 * This class assumes that the values d are the error values.
 */
class RmseAggregator : public ValueAggregator
{
public:

  static string className() { return "hoot::RmseAggregator"; }

  RmseAggregator();

  virtual double aggregate(vector<double>& d) const;

  virtual QString toString() const { return "RmseAggregator"; }
};

}
#endif // RMSEAGGREGATOR_H
