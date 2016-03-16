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

#include "Random.h"

// Standard Includes
#include <math.h>
#include <stdlib.h>

namespace Tgs
{
  boost::shared_ptr<Random> Random::_instance;
#ifdef NEW_RAND
  boost::shared_ptr<random_type> Random::_gen;
  boost::shared_ptr<generator_type> Random::_rnd;
#endif

  Random::Random()
  {
#ifdef NEW_RAND
    seed();
#endif
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
    return generateInt() % max;
  }

  int Random::generateInt()
  {
#ifdef NEW_RAND
    return _rnd->operator ()();
#else
    return rand();
#endif
  }

  void Random::seed(unsigned int s)
  {
#ifdef NEW_RAND
    _gen.reset(new random_type(s));
    _rnd.reset(new generator_type(*_gen, number_type(0, RAND_MAX)));
#else
    srand(s);
#endif
  }

  void Random::seed()
  {
#ifdef NEW_RAND
    random_type gen = random_type(time(0));
    generator_type rnd(gen, number_type(0, RAND_MAX));
    seed(rnd());
#else
    seed(0);
#endif
  }
}
