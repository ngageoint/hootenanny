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
 * @copyright Copyright (C) 2015, 2017, 2019 Maxar (http://www.maxar.com/)
 */

#ifndef __ABSTRACTTESTFITNESSFUNCTION_H__
#define __ABSTRACTTESTFITNESSFUNCTION_H__

// Cpp Unit
#include <cppunit/TestResult.h>

// Hoot
#include <hoot/core/test/AbstractTestSuite.h>
#include <hoot/core/test/AbstractTest.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QTemporaryFile>

// Tgs
#include <tgs/Optimization/FitnessFunction.h>
#include <tgs/Optimization/State.h>

namespace hoot
{

class Settings;

/**
 * Fitness function which optimizes against Hootenanny conflate case test data
 *
 * This class uses the error log setting to allow for more easily viewing the results of the
 * optimization without having to view conflation log clutter...there may be a better way to
 * handle this.
 */
class AbstractTestFitnessFunction : public Tgs::FitnessFunction
{

public:

  AbstractTestFitnessFunction();

  virtual double f(const Tgs::ConstStatePtr& s);

  int getTestCount() { return _testCount; }

  /**
   * Returns a list of test names that failed for the best test run(s) (test runs with lowest
   * numbers of failures)
   *
   * @return a list of test names; or an empty list if there were no test failures for the best
   * test run(s)
   */
  QStringList getFailingTestsForBestRuns() const { return _failingTestsForBestRuns; }

  /**
   * Returns the current number of failing tests for the best test runs(s) (lowest number of
   * failures)
   *
   * @return count of test failures
   */
  int getLowestNumFailingTestsPerRun() const { return _lowestNumFailingTestsPerRun; }

  /**
   * Executes just before the test runs
   */
  virtual void initTest(Settings& testSettings) = 0;

  /**
   * Executes just after the test runs
   */
  virtual void afterTestRun() = 0;

protected:

  //number of tests in the test suite
  int _testCount;

  //test suite run by this fitness function
  std::shared_ptr<AbstractTestSuite> _testSuite;

  //name of the temporary settings file generated for the current test run
  QString _settingsFileName;

  //failed tests for the current test run
  QStringList _failedTests;

  //current test being run
  AbstractTest* _test;

private:

  //list members are one or more test names joined by a ';'
  QStringList _failingTestsForBestRuns;

  int _lowestNumFailingTestsPerRun;

  QString _failedTestsToString(const QStringList failedTests) const;
};

}

#endif // __ABSTRACTTESTFITNESSFUNCTION_H__
