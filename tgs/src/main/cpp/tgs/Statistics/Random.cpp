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
  unsigned int Random::_srand;
  boost::minstd_rand Random::_gen;

  Random::Random()
  {
    seed();
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

    /* Box-Muller transform */
    return mean + sigma * y * sqrt (-2.0 * log (r2) / r2);
  }

  double Random::generateUniform()
  {
    boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > rnd(_gen, boost::uniform_real<>(0, 1));
    return rnd();
  }

  bool Random::coinToss()
  {
    boost::variate_generator<boost::minstd_rand&, boost::uniform_int<> > rnd(_gen, boost::uniform_int<>(0, 1));
    return rnd() == 1;
  }

  int Random::generateInt()
  {
    boost::variate_generator<boost::minstd_rand&, boost::uniform_int<> > rnd(_gen, boost::uniform_int<>(0, RAND_MAX));
    return rnd();
  }

  int Random::generateInt(int max)
  {
    boost::variate_generator<boost::minstd_rand&, boost::uniform_int<> > rnd(_gen, boost::uniform_int<>(0, max));
    return rnd();
  }

  void Random::seed(unsigned int srand)
  {
    _gen = boost::minstd_rand(srand);
  }

  void Random::seed()
  {
    boost::variate_generator<boost::minstd_rand&, boost::uniform_int<> > rnd(_gen, boost::uniform_int<>(0, RAND_MAX));
    _gen = boost::minstd_rand((unsigned int)rnd());
  }

  /*** Delete these functions later */
  void Random::s_seed(unsigned int s_rand)
  {
    srand(s_rand);
  }

  int Random::s_generateInt()
  {
    return rand();
  }

  double Random::s_generateUniform()
  {
    return ((double)rand() / (double)RAND_MAX);
  }
}
