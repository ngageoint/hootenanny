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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "CaseTestFitnessFunction.h"

// hoot
#include <hoot/core/util/Log.h>

#include <hoot/test/ConflateCaseTestSuite.h>s

namespace hoot
{

CaseTestFitnessFunction::CaseTestFitnessFunction(QString dir, QString configFile) :
AbstractTestFitnessFunction(),
_configFile(configFile)
{
  _testSuite = std::make_shared<ConflateCaseTestSuite>(dir);
  QStringList confs;
  _testSuite->loadDir(dir, confs);
  _testCount = _testSuite->getChildTestCount();
}

void CaseTestFitnessFunction::initTest(Settings& /*testSettings*/)
{
  // This init will add the conflicts network case tests conf which is a subset of the overall
  // network cases tests conf.

  if (!_configFile.trimmed().isEmpty())
  {
    _test->addConfig(_configFile);
  }
}

double CaseTestFitnessFunction::f(const Tgs::ConstStatePtr& s)
{
  AbstractTestFitnessFunction::f(s);

  if (_failedTests.size() == 0)
  {
    //This message will actually show if, by chance, the first selected random state
    //is successful.  However, that state is just a starting point for the actual simulated
    //annealing iterations.
    LOG_INFO("\n\n***BOOM GOES THE DYNAMITE!***\n");
  }

  //optimize on the number of passing tests...no individual test scoring involved
  return (double)_failedTests.size() / (double)_testCount;
}

void CaseTestFitnessFunction::afterTestRun()
{  
}

}
