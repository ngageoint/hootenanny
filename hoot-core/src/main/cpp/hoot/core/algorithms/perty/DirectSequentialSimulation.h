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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DIRECT_SEQUENTIAL_SIMULATION_H
#define DIRECT_SEQUENTIAL_SIMULATION_H

// boost
#include <boost/random/linear_congruential.hpp>

// hoot
#include <hoot/core/algorithms/perty/PermuteGridCalculator.h>

namespace hoot
{

/**
 *
 */
class DirectSequentialSimulation : public PermuteGridCalculator
{
public:

  static QString className() { return "hoot::DirectSequentialSimulation"; }

  DirectSequentialSimulation() = default;
  ~DirectSequentialSimulation() = default;

  /**
   * @see PermuteGridCalculator
   */
  cv::Mat permute(const geos::geom::Envelope& env, int& pointRows, int& pointCols) override;

private:

  cv::Mat _gm2dPerturb2(geos::geom::Envelope env, Meters sigma, boost::minstd_rand& rng);
};

}

#endif // DIRECT_SEQUENTIAL_SIMULATION_H
