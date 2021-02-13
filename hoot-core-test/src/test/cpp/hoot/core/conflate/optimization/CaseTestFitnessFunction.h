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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __CASETESTFITNESSFUNCTION_H__
#define __CASETESTFITNESSFUNCTION_H__

// Hoot
#include <hoot/core/conflate/optimization/AbstractTestFitnessFunction.h>

// Qt
#include <QTemporaryFile>

namespace hoot
{

/**
 * Fitness function which optimizes against Hootenanny conflate case test data
 *
 * This class uses the error log setting to allow for more easily viewing the results of the
 * optimization without having to view conflation log clutter...there may be a better way to
 * handle this.
 */
class CaseTestFitnessFunction : public AbstractTestFitnessFunction
{

public:

  CaseTestFitnessFunction(QString dir, QString configFile);

  /**
   * Fitness function that determines the performance based on the test output
   *
   * @param s a simulated annealing state
   * @return a fitness value (lower is better)
   */
  virtual double f(const Tgs::ConstStatePtr& s);

  /**
   * Executes just before the test runs
   */
  virtual void initTest(Settings& testSettings);

  /**
   * Executes just after the test runs
   */
  virtual void afterTestRun();

private:

  // a custom config file to pass to the tests run by this fitness function
  QString _configFile;
};

}

#endif // __CASETESTFITNESSFUNCTION_H__
