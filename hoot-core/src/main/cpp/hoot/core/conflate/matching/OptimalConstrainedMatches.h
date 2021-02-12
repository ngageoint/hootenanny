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
#ifndef OPTIMALCONSTRAINEDMATCHES_H
#define OPTIMALCONSTRAINEDMATCHES_H

// Hoot
#include <hoot/core/conflate/matching/ConstrainedMatches.h>

namespace hoot
{

class IntegerProgrammingSolver;

/**
 * A class for determining a subset of matches that meet a set of constraints (isConflicting) and
 * attempting to maximize the quality of the matches chosen.
 *
 * Maximizing the set of matches is kind of tricky. For now, best is defined as maximizing the sum
 * of the Match scores.
 */
class OptimalConstrainedMatches : public ConstrainedMatches
{
public:

  OptimalConstrainedMatches(const ConstOsmMapPtr& map);
  virtual ~OptimalConstrainedMatches() = default;

  /**
   * Calculate a consistent subset of matches while attempting to maximizing the sum of match
   * scores. All matches will be considered so if you don't want matches below a threshold included
   * then don't add them.
   */
  virtual std::vector<ConstMatchPtr> calculateSubset() override;

private:

  void _populateSolver(IntegerProgrammingSolver& solver);
};

}

#endif // CONSTRAINEDMATCHES_H
