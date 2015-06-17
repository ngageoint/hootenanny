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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FULL_COVARIANCE_H
#define FULL_COVARIANCE_H

// hoot
#include "PermuteGridCalculator.h"

namespace hoot
{

using namespace cv;

/**
 *
 */
class FullCovariance : public PermuteGridCalculator
{
public:

  static string className() { return "hoot::FullCovariance"; }

  FullCovariance();

  //virtual ~FullCovariance() {}

  /**
   * @see PermuteGridCalculator
   */
  virtual Mat permute(geos::geom::Envelope env, int& pointRows, int& pointCols);

private:

  /**
   * Create a regular grid with _gridSpacing spacing that contains the envelope env. The number of
   * rows and columns is returned in rows and cols. The matrix contains two columns (x & y) and
   * a row for each point.
   */
  Mat _createGrid(const geos::geom::Envelope& env, int& rows, int& cols) const;

  /**
   * Create the systematic error matrix using 1 dimension of the point data and a specified sigma.
   * A single sigma values is used for all values in the matrix.
   */
  Mat _createS(const Mat& points, Meters sigma);

  /**
   * Interleave Ex and Ey.
   */
  Mat _interleave(const Mat& Ex, const Mat& Ey) const;

  /**
   * Method for calculating rho for a given distance. This uses equation (10) given in [1], the
   * Community Sensor Model (CSM) simplification.
   */
  double _rho(Meters d) const;
};

}

#endif // FULL_COVARIANCE_H
