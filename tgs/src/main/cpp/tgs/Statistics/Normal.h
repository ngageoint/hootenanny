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
 * @copyright Copyright (C) 2015, 2018 Maxar (http://www.maxar.com/)
 */

#ifndef NORMAL_H
#define NORMAL_H

#include <tgs/TgsExport.h>

namespace Tgs
{
  class TGS_EXPORT Normal
  {
  public:

    /**
     * Calculates the value on the standard normal curve at x.
     */
    static double normal(double x, double sigma = 1, double mu = 0.0);

    /**
     * Calculates phi (CDF of normal curve) for the standard normal. This should be accurate to
     * 7.5e-8
     */
    static double phi(double x);

    static double phi(double x, double sigma) { return phi(x / sigma); }

    static double epsilon;
  };
}

#endif // NORMAL_H
