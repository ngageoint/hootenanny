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
#include "DirectSequentialSimulation.h"

// boost
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

// Tgs
#include <tgs/System/Time.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/RandomNumberUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(PermuteGridCalculator, DirectSequentialSimulation)

DirectSequentialSimulation::DirectSequentialSimulation()
{

}

Mat DirectSequentialSimulation::permute(geos::geom::Envelope env, int& pointRows, int& pointCols)
{
  boost::minstd_rand rng;
  if (_seed == -1)
  {
    // grab the time in seconds.
    double s = Tgs::Time::getTime() / 1e3;
    // keep some lower juicy bits from the time.
    s = s - floor(s);
    int seed = (int)(s * 1e9);
    rng.seed(seed);
  }
  else
  {
    rng.seed(_seed);
  }

  Mat Ex = _gm2dPerturb2(env, _sigmaSx, rng);
  Mat Ey = _gm2dPerturb2(env, _sigmaSy, rng);
  pointRows = Ex.rows;
  pointCols = Ex.cols;

  // 9) Join the x-errors and y-errors for the n points in a 2nx1 vector as,
  Mat EX(Ex.rows * Ex.cols * 2, 1, CV_64F);

  for (int i = 0; i < Ex.rows; i++)
  {
    for (int j = 0; j < Ex.cols; j++)
    {
      EX.at<double>((i * Ex.cols + j) * 2, 0) = Ex.at<double>(i, j);
      EX.at<double>((i * Ex.cols + j) * 2 + 1, 0) = Ey.at<double>(i, j);
    }
  }

  return EX;
}

Mat DirectSequentialSimulation::_gm2dPerturb2(geos::geom::Envelope env, Meters sigma,
                                              boost::minstd_rand& rng)
{
  // Uses John Dollof's Gauss-Markov method to perturb values assigned to a
  // 2D grid with correlation.  Returns 2D grid of perturbations.
  //
  // [lines, samples] = size(2D grid)
  // g = post spacing of grid
  // Dx,Dy = correlation parameters in X and Y
  // sig = standard deviation of parameter being perturbed

  // In practice, the first few rows and columns should be removed from the grid to reduce boundary
  // effects
  env.expandBy(3 * _gridSpacing);

  int rows = max<int>(2, ceil(env.getHeight() / (double)_gridSpacing) + 1);
  int cols = max<int>(2, ceil(env.getWidth() / (double)_gridSpacing) + 1);

  double r = exp(-_gridSpacing / _D);
  double s = exp(-_gridSpacing / _D);

  double sigu = sqrt((1 - r * r) * (1 - s * s) * sigma * sigma);

  Mat Pb = Mat::zeros(rows, cols, CV_64F);

  boost::normal_distribution<> nd;
  boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<> > N(rng, nd);

  // first do first corner cell
  Pb.at<double>(0, 0) = N() * sigma;

  // then the rest of the rows (for the first column)
  for (int j = 1; j < rows; ++j)
  {
    Pb.at<double>(j, 0) = s * Pb.at<double>(j - 1, 0) + N() * sigu;
  }
  // then rest of column (for fixed 1st row)
  for (int k = 1; k < cols; ++k)
  {
    Pb.at<double>(0, k) = r * Pb.at<double>(0, k - 1) + N() * sigu;
  }

  // then fill in the rest of the grid
  for (int k = 2; k < cols; ++k)
  {
    for (int j = 2; j < rows; ++j)
    {
      Pb.at<double>(j, k) = r * Pb.at<double>(j - 1, k) + s * Pb.at<double>(j, k - 1) -
          r * s * Pb.at<double>(j - 1, k - 1) + N() * sigu;
    }
  }

  return Mat(Pb, cv::Range(3, rows - 3), cv::Range(3, cols - 3));
}

}
