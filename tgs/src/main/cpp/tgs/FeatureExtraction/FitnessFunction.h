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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__FITNESS_FUNCTION_H__
#define __TGS__FITNESS_FUNCTION_H__

#include "../TgsExport.h"

namespace Tgs
{
  class Genome;

  /**
   * Interface for calculating the fitness of a given genome.
   */
  class TGS_EXPORT FitnessFunction
  {
  public:
    virtual ~FitnessFunction() {}

    /**
     * Calculates the fitness of a genome and returns the result. Ideally this should be a 
     * relatively fast operation, although that is not always possible.
     *
     * @param genome Genome to be evaluated
     * @return Result is a value >= 0. Larger values mean the genome is more fit.
     */
    virtual double calculateFitness(const Genome& genome) = 0;
  };
}

#endif
