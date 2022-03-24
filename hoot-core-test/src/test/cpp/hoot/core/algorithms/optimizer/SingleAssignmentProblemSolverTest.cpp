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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/optimizer/SingleAssignmentProblemSolver.h>

using namespace std;

namespace hoot
{

class ActorTask
{
public:
  double value;
  ActorTask(double v) : value(v) {}

  QString toString() const
  {
    return QString("%1").arg(value);
  }
};

class CostFunction : public SingleAssignmentProblemSolver<ActorTask, ActorTask>::CostFunction
{
public:
  virtual double cost(const ActorTask *a, const ActorTask *t) const
  {
    if (a->value == t->value)
      return a->value * t->value;
    else
      return min(a->value, t->value);
  }
};

class SingleAssignmentProblemSolverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SingleAssignmentProblemSolverTest);
  CPPUNIT_TEST(runTest1);
  CPPUNIT_TEST(runTest2);
  CPPUNIT_TEST_SUITE_END();

public:
  vector<std::shared_ptr<ActorTask>> toDelete;

  void addActors(SingleAssignmentProblemSolver<ActorTask, ActorTask>& solver, int* v)
  {
    int i = 0;
    while (v[i] >= 0)
    {
      std::shared_ptr<ActorTask> ptr = std::make_shared<ActorTask>(v[i++]);
      toDelete.push_back(ptr);
      solver.addActor(ptr.get());
    }
  }

  void addTasks(SingleAssignmentProblemSolver<ActorTask, ActorTask>& solver, int* v)
  {
    int i = 0;
    while (v[i] >= 0)
    {
      std::shared_ptr<ActorTask> ptr = std::make_shared<ActorTask>(v[i++]);
      toDelete.push_back(ptr);
      solver.addTask(ptr.get());
    }
  }

  double getTotalCost(const vector<SingleAssignmentProblemSolver<ActorTask, ActorTask>::ResultPair>& pairs)
  {
    CostFunction costFunction;
    double totalCost = 0.0;
    for (const auto& p : pairs)
    {
      if (p.actor != nullptr && p.task != nullptr)
        totalCost += costFunction.cost(p.actor, p.task);
    }
    return totalCost;
  }

  void runTest1()
  {
    CostFunction cost;
    SingleAssignmentProblemSolver<ActorTask, ActorTask> solver(cost);

    int actors[] = {4, 5, 6, 7, -1};
    int tasks[] = {4, 3, 6, -1};
    addActors(solver, actors);
    addTasks(solver, tasks);

    vector<SingleAssignmentProblemSolver<ActorTask, ActorTask>::ResultPair> pairs = solver.calculatePairing();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(55, getTotalCost(pairs), 0.1);
  }

  void runTest2()
  {
    CostFunction cost;
    SingleAssignmentProblemSolver<ActorTask, ActorTask> solver(cost);

    int actors[] = {4, 5, 6, 7, 8, 9, 10, -1};
    int tasks[] = {1, 2, 4, 3, 6, -1};
    addActors(solver, actors);
    addTasks(solver, tasks);

    vector<SingleAssignmentProblemSolver<ActorTask, ActorTask>::ResultPair> pairs = solver.calculatePairing();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(58, getTotalCost(pairs), 0.1);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SingleAssignmentProblemSolverTest, "quick");

}
