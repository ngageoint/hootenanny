#include "GlpkBinaryOptimizer.h"

namespace hoot
{

const double GlpkBinaryOptimizer::EPSILON = 1e-6;

GlpkBinaryOptimizer::GlpkBinaryOptimizer()
{
}

void GlpkBinaryOptimizer::_populateSolver(IntegerProgrammingSolver& solver,
  const vector<double>& weights, const vector< pair<size_t, size_t> >& constraints)
{
  // try to maximize the score.
  solver.setObjectiveDirection(GLP_MAX);

  // each match is a column (variable) in the function that we want to maximize.
  solver.addColumns(weights.size());
  for (size_t i = 0; i < weights.size(); i++)
  {
    solver.setColumnKind(i + 1, GLP_BV);
    // The score of a match is the coefficient. This makes higher score matches worth
    // more in the function.
    solver.setObjectiveCoefficient(i + 1, weights[i] + EPSILON);
  }

  // there is one row (constraint) for each conflict.
  solver.addRows(constraints.size());
  vector<int> ia(constraints.size() * 2 + 1);
  vector<int> ja(constraints.size() * 2 + 1);
  vector<double> ra(constraints.size() * 2 + 1);

  for (size_t i = 0; i < constraints.size(); ++i)
  {
    // Set the coefficients to 1 for each of the conflicting pairs and set the max value to 1. This
    // will make it so only one of the values can be 1 at a time, or they can both be 0.
    solver.setRowBounds(i + 1, GLP_DB, 0.0, 1.0);
    ia[i * 2 + 1] = i + 1;
    ja[i * 2 + 1] = (int)constraints[i].first + 1;
    ra[i * 2 + 1] = 1.0;
    ia[i * 2 + 2] = i + 1;
    ja[i * 2 + 2] = (int)constraints[i].second + 1;
    ra[i * 2 + 2] = 1.0;
  }

  // Populate the row coefficients.
  solver.loadMatrix(ia, ja, ra);
}

double GlpkBinaryOptimizer::_solve(const vector<double>& weights,
  const vector< pair<size_t, size_t> >& constraints, vector<size_t>& solution)
{
  double result = 0.0;

  if (weights.size() == 0)
  {
    solution.clear();
    return result;
  }

  // if there are no constraints, then there is nothing to solve.
  if (constraints.size() == 0)
  {
    solution.resize(weights.size());

    for (size_t i = 0; i < weights.size(); ++i)
    {
      solution[i] = i;
      result += weights[i];
    }

    return result;
  }

  // populate the solver with
  IntegerProgrammingSolver solver;
  _populateSolver(solver, weights, constraints);
  if (getTimeLimit() > 0)
  {
    solver.setTimeLimit(getTimeLimit());
  }

  // solve the Integer Programming problem.
  solver.solve();
  result = solver.getObjectiveValue();

  solution.clear();
  solution.reserve(weights.size());

  // go through all the columns (variables)
  for (int i = 0; i < solver.getNumColumns(); i++)
  {
    // if the value is close to 1 (as opposed to 0)
    if (solver.getColumnPrimalValue(i + 1) > 0.99)
    {
      // it is a keeper
      solution.push_back(i);
    }
  }

  return result;
}

}
