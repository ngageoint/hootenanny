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

#include "CaseTestFitnessFunction.h"

#include <hoot/core/test/ConflateCaseTestSuite.h>

namespace hoot
{

CaseTestFitnessFunction::CaseTestFitnessFunction() :
AbstractTestFitnessFunction()
{
  const QString dir = "test-files/cases/hoot-rnd/network/conflicts/";
  _testSuite.reset(new ConflateCaseTestSuite(dir));
  QStringList confs;
  _testSuite->loadDir(dir, confs);
  _testCount = _testSuite->getChildTestCount();
}

//this init will add the conflicts network case tests conf which is a subset of the overall
//network cases tests conf
void CaseTestFitnessFunction::initTest(AbstractTest* test)
{
  test->addConfig("test-files/cases/hoot-rnd/network/Config.conf");
}

double CaseTestFitnessFunction::f(const Tgs::ConstStatePtr& s)
{
  AbstractTestFitnessFunction::f(s);

  if (_failedTests.size() == 0)
  {
    //This message will actually show if, by chance, the first selected random state
    //is successful.  However, that state is just a starting point for the actual simulated
    //annealing iterations.
    LOG_ERROR("\n\n***BOOM GOES THE DYNAMITE!***\n");
  }
  else
  {
    QString failureMsg =
      QString::number(_failedTests.size()) + "/" + QString::number(_testCount) +
      " tests failed:\n\n";
    for (int i = 0; i < _failedTests.size(); i++)
    {
      failureMsg += "\t" + _failedTests[i] + "\n";
    }
    LOG_ERROR(failureMsg);
    LOG_ERROR("Lowest number of tests failed so far: " << _lowestNumFailingTestsPerRun);
  }

  return (double)_failedTests.size() / (double)_testCount;
}

void CaseTestFitnessFunction::afterTestRun(AbstractTest* /*test*/)
{  
}

}
