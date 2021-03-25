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
 * @copyright Copyright (C) 2015, 2016, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "Random.h"

// Standard Includes
#include <math.h>
#include <stdlib.h>

namespace Tgs
{
  std::shared_ptr<Random> Random::_instance;

  Random::Random(unsigned int s) :
  _is_single(false)
  {
    seed(s);
  }

  Random::Random() :
  _is_single(true)
  {
    seed(0);
  }

  double Random::generateGaussian(double mean, double sigma)
  {
    double x, y, r2;
    do
    {
      // choose x,y in uniform square (-1,-1) to (+1,+1)
      x = -1 + 2 * generateUniform();
      y = -1 + 2 * generateUniform();
      // see if it is in the unit circle
      r2 = x * x + y * y;
    }
    while (r2 > 1.0 || r2 == 0);
    // Box-Muller transform
    return mean + sigma * y * sqrt (-2.0 * log (r2) / r2);
  }

  double Random::generateUniform()
  {
    return (double)generateInt() / (double)RAND_MAX;
  }

  bool Random::coinToss()
  {
    return (generateInt() % 2) == 1;
  }

  int Random::generateInt(int max)
  {
    if (max <= 0)
      return 0;
    else
      return generateInt() % max;
  }

  int Random::generateInt()
  {
    if (_is_single)
      return rand();
    else
      return rand_r(&_seed);
  }

  void Random::seed(unsigned int s)
  {
    if (_is_single)
      srand(s);
    else
      _seed = s;
  }

  void Random::seed()
  {
    seed(0);
  }
}
