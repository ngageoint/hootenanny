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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

// hoot
#include <hoot/core/util/Units.h>

// standard
#include <vector>

// Qt
#include <QString>

using namespace std;

namespace hoot
{

/**
 * A class for storing a histogram of angle values.
 */
class Histogram
{
public:

  Histogram(int bins);

  const vector<double>& getAllBins() const { return _bins; }

  void addAngle(Radians theta, double length);

  size_t getBin(Radians theta);

  /**
   * Returns the angle of the center of the specified bin.
   */
  Radians getBinCenter(size_t bin) const;

  /**
   * Normalize all the bins so the sum of the bins is 1.0.
   */
  void normalize();

  /**
   * Returns a value from 0.0 to 1.0 describing the diff. 1.0 is exactly the same.
   */
  double diff(Histogram& other);

  /**
   * Smooth the histogram with a gaussian filter with the specified sigma.
   */
  void smooth(Radians sigma);

  QString toString() const;

private:

  vector<double> _bins;

  Radians _getBinAngle(size_t i);

};

}

#endif // HISTOGRAM_H
