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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FullCovariance.h"

// boost
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/RandomNumberUtils.h>
#include <hoot/core/util/Octave.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(PermuteGridCalculator, FullCovariance)

FullCovariance::FullCovariance()
{

}

Mat FullCovariance::permute(geos::geom::Envelope env, int& pointRows, int& pointCols)
{
  // 1) Assume n unique 2D point locations in any arbitrary order.
  Mat points = _createGrid(env, pointRows, pointCols);
  int n = pointRows * pointCols;

  LOG_DEBUG("unique points: " << points);

  // 2) Assume nxn random error covariance matrices for x and y equal to,
  /// @note we're using a fixed sigma for all entries in Rx or Ry.
  Mat Rx = Mat::eye(n, n, CV_64F) * (_sigmaRx * _sigmaRx);
  Mat Ry = Mat::eye(n, n, CV_64F) * (_sigmaRy * _sigmaRy);

  LOG_DEBUG("Rx: \n" << Rx);
  LOG_DEBUG("Ry: \n" << Ry);

  // 3) Assume nxn systematic error covariance matrices for n points in x and y equal to,
  /// @note we're using a fixed sigma for all entries in Sx or Sy.
  /// @note the paper says "horizontal distance". I'm using euclidean distance.
  Mat Sx = _createS(points, _sigmaSx);
  Mat Sy = _createS(points, _sigmaSy);

  LOG_DEBUG("Sx: \n" << Sx);
  LOG_DEBUG("Sy: \n" << Sy);

  // 7) Define corresponding nxn (symmetric) error covariance matrices Px and Py as,
  Mat Px = Rx + Sx;
  Mat Py = Ry + Sy;

  LOG_DEBUG("Px: \n" << Px);
  LOG_DEBUG("Py: \n" << Py);

  // where each instance of and is a nx1 vector realization of n independent distributed random
  // variables, e.g., distributed as N(0,1), U(-1,1), or as desired.
  /// @note using U(-1, 1)
  boost::minstd_rand rng;
  if (_seed == -1)
  {
    rng.seed(RandomNumberUtils::generateSeed());
  }
  else
  {
    rng.seed(_seed);
  }
  boost::uniform_real<> uni(-1.0, 1.0);
  Mat a = Mat_<double>(n, 1);
  Mat b = Mat_<double>(n, 1);
  for (int i = 0; i < n; i++)
  {
    a.at<double>(i) = uni(rng);
    b.at<double>(i) = uni(rng);
  }

  LOG_DEBUG("a: " << a);
  LOG_DEBUG("b: " << b);

  // 8) Generate a realization of the corresponding n (x,y)-errors as,
  Mat Ex = Octave::getInstance().sqrtm(Px) * a;
  Mat Ey = Octave::getInstance().sqrtm(Py) * b;

  LOG_DEBUG("sqrtm(Px): \n" << Octave::getInstance().sqrtm(Px));
  LOG_DEBUG("sqrtm(Py): \n" << Octave::getInstance().sqrtm(Py));
  LOG_DEBUG("Ex: " << Ex);
  LOG_DEBUG("Ey: " << Ey);

  // 9) Join the x-errors and y-errors for the n points in a 2nx1 vector as,
  Mat EX = _interleave(Ex, Ey);

  LOG_DEBUG("EX: " << EX);

  return EX;
}

Mat FullCovariance::_createGrid(const geos::geom::Envelope& env, int& rows, int& cols) const
{
  rows = max<int>(2, ceil(env.getHeight() / (double)_gridSpacing) + 1);
  cols = max<int>(2, ceil(env.getWidth() / (double)_gridSpacing) + 1);
  // create a matrix where the columns are x,y and the rows are points.
  Mat_<double> result(rows * cols, 2);

  for (int yi = 0; yi < rows; yi++)
  {
    for (int xi = 0; xi < cols; xi++)
    {
      double x = env.getMinX() + _gridSpacing * (double)xi;
      double y = env.getMinY() + _gridSpacing * (double)yi;
      int r = yi * cols + xi;
      result.at<double>(r, 0) = x;
      result.at<double>(r, 1) = y;
    }
  }

  return result;
}

Mat FullCovariance::_createS(const Mat& points, Meters sigma)
{
  assert(points.cols == 2);

  // populate the diagonal.
  Mat result = Mat::eye(points.rows, points.rows, CV_64F) * (sigma * sigma);

  // 4)  Assume a strictly positive definite correlation function (SPDCF) between points  i  and  j
  // that is common to all  point components.  A SPDCF ensures  the generation of a  valid
  // covariance matrix with positive eigenvalues.
  for (int i = 0; i < points.rows; i++)
  {
    Coordinate c1 = Coordinate(points.at<double>(i, 0), points.at<double>(i, 1));
    for (int j = i + 1; j < points.rows; j++)
    {
      Coordinate c2 = Coordinate(points.at<double>(j, 0), points.at<double>(j, 1));
      Meters d = c1.distance(c2);
      result.at<double>(i, j) = _rho(d) * sigma * sigma;
      result.at<double>(j, i) = _rho(d) * sigma * sigma;
    }

  }

  return result;
}

Mat FullCovariance::_interleave(const Mat& Ex, const Mat& Ey) const
{
  Mat result(Ex.rows * 2, 1, CV_64F);

  for (int i = 0; i < Ex.rows; i++)
  {
    result.at<double>(i * 2, 0) = Ex.at<double>(i, 0);
    result.at<double>(i * 2 + 1, 0) = Ey.at<double>(i, 0);
  }

  return result;
}

double FullCovariance::_rho(Meters d) const
{
  // Simplified Community Sensor Model. See section 2.1.2 of [1] for a discussion.
  return (1.0 + _beta) / (_beta + exp(d / _D));
}

}
