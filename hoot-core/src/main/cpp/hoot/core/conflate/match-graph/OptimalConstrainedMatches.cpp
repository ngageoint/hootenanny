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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OptimalConstrainedMatches.h"

// hoot
#include <hoot/core/algorithms/optimizer/IntegerProgrammingSolver.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

OptimalConstrainedMatches::OptimalConstrainedMatches(const ConstOsmMapPtr &map) :
  ConstrainedMatches(map)
{
  _score = -1;
  _timeLimit = -1;
}

vector<const Match *> OptimalConstrainedMatches::calculateSubset()
{
  _score = -1;
  vector<const Match*> result;

  if (_matches.size() == 0)
  {
    return result;
  }

  // figure out all the pairs of matches that conflict.
  LOG_INFO("Calculating optimal match conflicts...");
  _calculateMatchConflicts();
  LOG_DEBUG("Done calculating optimal match conflicts");

  // if there are no conflicts, then there is nothing to solve.
  if (_conflicts.size() == 0)
  {
    return _matches;
  }

  // populate the solver with
  IntegerProgrammingSolver solver;
  _populateSolver(solver);
  if (_timeLimit > 0)
  {
    solver.setTimeLimit(_timeLimit);
  }

  // solve the Integer Programming problem.
  solver.solve();
  _score = solver.getObjectiveValue();

  result.reserve(_matches.size());

  // go through all the columns (variables)
  for (int i = 0; i < solver.getNumColumns(); i++)
  {
    // if the value is close to 1 (as opposed to 0)
    if (solver.getColumnPrimalValue(i + 1) > 0.99)
    {
      // it is a keeper
      result.push_back(_matches[i]);
    }
  }

  return result;
}

void OptimalConstrainedMatches::_populateSolver(IntegerProgrammingSolver& solver)
{
  // try to maximize the score.
  solver.setObjectiveDirection(GLP_MAX);

  // each match is a column (variable) in the function that we want to maximize.
  solver.addColumns(_matches.size());
  for (size_t i = 0; i < _matches.size(); i++)
  {
    solver.setColumnKind(i + 1, GLP_BV);
    //solver.setColumnBounds(i + 1, GLP_DB, 0.0, 1.0);
    // The score of a match is the coefficient. This makes higher score matches worth
    // more in the function.
    solver.setObjectiveCoefficient(i + 1, _matches[i]->getScore() + EPSILON);
  }

  // there is one row (constraint) for each conflict.
  solver.addRows(_conflicts.size());
  vector<int> ia(_conflicts.size() * 2 + 1);
  vector<int> ja(_conflicts.size() * 2 + 1);
  vector<double> ra(_conflicts.size() * 2 + 1);
  int i = 0;
  for (MatchConflicts::ConflictMap::const_iterator it = _conflicts.constBegin();
       it != _conflicts.constEnd(); ++it)
  {
    // Set the coefficients to 1 for each of the conflicting pairs and set the max value to 1. This
    // will make it so only one of the values can be 1 at a time, or they can both be 0.
    solver.setRowBounds(i + 1, GLP_DB, 0.0, 1.0);
    ia[i * 2 + 1] = i + 1;
    ja[i * 2 + 1] = (int)it.key() + 1;
    ra[i * 2 + 1] = 1.0;
    ia[i * 2 + 2] = i + 1;
    ja[i * 2 + 2] = (int)it.value() + 1;
    ra[i * 2 + 2] = 1.0;
    i++;
  }

  // Populate the row coefficients.
  solver.loadMatrix(ia, ja, ra);
}

}
