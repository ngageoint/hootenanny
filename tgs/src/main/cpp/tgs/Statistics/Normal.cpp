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
 * @copyright Copyright (C) 2015, 2018, 2021 Maxar (http://www.maxar.com/)
 */

#include "Normal.h"

// Standard
#include <math.h>

namespace Tgs
{
  double Normal::epsilon = 7.5e-8;

  double Normal::normal(double x, double sigma, double mu)
  {
    ////
    // http://en.wikipedia.org/wiki/Normal_distribution
    ////
    double c = 1 / (sigma * sqrt(2 * M_PI));
    double ep = pow(M_E, -1.0 * pow(x - mu, 2.0) / (2.0 * sigma * sigma));
    return c * ep;
  }

  double Normal::phi(double x)
  {
    ////
    // http://en.wikipedia.org/wiki/Normal_distribution#Numerical_approximations_for_the_normal_CDF
    ////

    double b0 = 0.2316419;
    double b1 = 0.319381530;
    double b2 = -0.356563782;
    double b3 = 1.781477937;
    double b4 = -1.821255978;
    double b5 = 1.330274429;

    double t = 1 / (1 + b0 * x);

    double result = 1 - normal(x) *
                    (b1 * t +
                     b2 * t * t +
                     b3 * t * t * t +
                     b4 * t * t * t * t +
                     b5 * t * t * t * t * t);

    // this avoid negative numbers for large negative x's.
    if (result < epsilon)
    {
      result = epsilon;
    }
    // avoid results greater than 1.0 for large x's.
    else if (result > 1.0)
    {
      result = 1.0;
    }

    return result;
  }
}
