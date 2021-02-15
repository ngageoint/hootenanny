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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__GENOME_H__
#define __TGS__GENOME_H__

// Standard Includes
#include <limits>
#include <string>

#include "../SharedPtr.h"
#include "../TgsExport.h"

namespace Tgs
{
  /**
   * An interface for a genome.
   */
  class TGS_EXPORT Genome
  {
  public:

    Genome() { setInvalidScore(); }

    virtual ~Genome() {}

    /**
     * Clears any cache that may be stored up during the evaluation process.
     */
    virtual void clearCache() {}

    /**
     * Make an exact copy of this genome and return it.
     */
    virtual boost::shared_ptr<Genome> clone() const = 0;

    /**
     * Initialize with random values.
     */
    virtual void initialize() = 0;

    /**
     * Mutate the existing genome into a slight variation of itself.
     * @param p A value from 0 to 1, 0 is no mutation, 1 is a lot of mutation.
     */
    virtual void mutate(double p) = 0;

    /**
     * Mate the two specified genomes to create two new offspring.
     */
    virtual void crossoverSexually(const Genome& father, const Genome& mother,
      boost::shared_ptr<Genome>& brother, boost::shared_ptr<Genome>& sister) = 0;

    double getScore() const { return _score; }

    bool isValidScore() const { return _score != -std::numeric_limits<double>::infinity(); }

    void setInvalidScore() { _score = -std::numeric_limits<double>::infinity(); }

    void setScore(double score) { _score = score; }

    virtual std::string toString() const = 0;

  private:
    double _score;
  };
}

#endif
