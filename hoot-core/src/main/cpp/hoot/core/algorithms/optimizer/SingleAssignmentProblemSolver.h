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

#ifndef SINGLEASSIGNMENTPROBLEMSOLVER_H
#define SINGLEASSIGNMENTPROBLEMSOLVER_H

// hoot
#include <hoot/core/algorithms/optimizer/IntegerProgrammingSolver.h>

// standard
#include <set>
#include <vector>

namespace hoot
{

/**
 * This is a form of the assignment problem [1], but I couldn't find a specific form of the
 * assignment problem that is an exact match.
 *
 * In this case we have a set of actors and tasks. Each actor can be assigned to only one task and
 * each task can have up to 1 actor. We're attempting to maximize the sum of the costs of assigning
 * the actors to tasks.
 *
 * In conflation this occurs frequently when attempting to assign sublines to matching sublines, or
 * determining the set of matching tags that maximizes overlap.
 *
 * 1. http://en.wikipedia.org/wiki/Assignment_problem
 */
template<typename Actor, typename Task>
class SingleAssignmentProblemSolver
{
public:

  class CostFunction
  {
  public:
    /**
     * Returns the cost associated with assigning actor a to task t.
     */
    virtual double cost(const Actor* a, const Task* t) const = 0;
  };

  struct ResultPair
  {
    const Actor* actor;
    const Task* task;

    QString toString() const
    {
      return (actor ? actor->toString() : QString("null")) + " " +
             (task ? task->toString() : QString("null"));
    }
  };

  SingleAssignmentProblemSolver(CostFunction& costFunction) : _costFunction(costFunction) { }

  void addActor(const Actor* a) { _actors.push_back(a); }
  void addTask(const Task* t) { _tasks.push_back(t); }

  /**
   * Calculate one of the optimal pairings. All the actors that are not paired will be placed into
   * a result pair w/ a null task. All the tasks that are not paired will be placed into a result
   * pair w/ a null actor.
   */
  std::vector<ResultPair> calculatePairing() const
  {
    std::vector<SingleAssignmentProblemSolver::ResultPair> result;

    IntegerProgrammingSolver solver;

    _populateSolver(solver);

    solver.solve();

    std::set<const Actor*> usedActors;
    std::set<const Task*> usedTasks;

    // go through all the columns (variables)
    for (int i = 0; i < solver.getNumColumns(); i++)
    {
      // if the value is close to 1 (as opposed to 0)
      if (solver.getColumnPrimalValue(i + 1) > 0.99)
      {
        ResultPair r;
        r.actor = _actors[i % _actors.size()];
        r.task = _tasks[i / _actors.size()];
        usedActors.insert(r.actor);
        usedTasks.insert(r.task);
        result.push_back(r);
      }
    }

    for (size_t i = 0; i < _actors.size(); i++)
    {
      if (usedActors.find(_actors[i]) == usedActors.end())
      {
        ResultPair r;
        r.actor = _actors[i];
        r.task = nullptr;
        result.push_back(r);
      }
    }

    for (size_t i = 0; i < _tasks.size(); i++)
    {
      if (usedTasks.find(_tasks[i]) == usedTasks.end())
      {
        ResultPair r;
        r.actor = nullptr;
        r.task = _tasks[i];
        result.push_back(r);
      }
    }

    return result;
  }

private:
  CostFunction& _costFunction;

  std::vector<const Actor*> _actors;
  std::vector<const Task*> _tasks;

  void _populateSolver(IntegerProgrammingSolver& solver) const
  {
    // try to maximize the score.
    solver.setObjectiveDirection(GLP_MAX);

    // one column for each actor/task pair.
    solver.addColumns(_actors.size() * _tasks.size());
    for (size_t t = 0; t < _tasks.size(); t++)
    {
      for (size_t a = 0; a < _actors.size(); a++)
      {
        int column = t * _actors.size() + a + 1;
        solver.setColumnKind(column, GLP_BV);
        double c = _costFunction.cost(_actors[a], _tasks[t]);
        // each actor/task has a cost
        solver.setObjectiveCoefficient(column, c);
      }
    }

    int rows = _tasks.size() * _actors.size() * 2;
    solver.addRows(rows);

    std::vector<int> ia;
    std::vector<int> ja;
    std::vector<double> ra;
    int i = 1;
    // GLPK's arrays are 1 based. Why!?
    ia.push_back(0);
    ja.push_back(0);
    ra.push_back(0);

    for (size_t a = 0; a < _actors.size(); a++)
    {
      solver.setRowBounds(_tasks.size() + a + 1, GLP_DB, 0.0, 1.0);
    }

    // all actor/task pairs for a given task have a coefficient of 1 and must <= 1. Or in other words
    // only one actor can be assigned to a task.
    for (size_t t = 0; t < _tasks.size(); t++)
    {
      solver.setRowBounds(t + 1, GLP_DB, 0.0, 1.0);
      for (size_t a = 0; a < _actors.size(); a++)
      {
        // Set the coefficients to 1 for each of the actors and set the max value to 1. This
        // will make it so only up to one of the values can be 1 at a time for a given target.
        ia.push_back(t + 1);
        ja.push_back(t * _actors.size() + a + 1);
        ra.push_back(1.0);
        ia.push_back(_tasks.size() + a + 1);
        ja.push_back(t * _actors.size() + a + 1);
        ra.push_back(1.0);
        i++;
      }
    }

    // Populate the row coefficients.
    solver.loadMatrix(ia, ja, ra);
  }

};

}

#endif // SINGLEASSIGNMENTPROBLEMSOLVER_H
