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

#include "AbstractTestFitnessFunction.h"

// Hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/test/SimpleTestListener.h>
#include <hoot/core/test/TempFileName.h>

namespace hoot
{

AbstractTestFitnessFunction::AbstractTestFitnessFunction() :
_testCount(0),
_lowestNumFailingTestsPerRun(-1)
{
}

double AbstractTestFitnessFunction::f(const Tgs::ConstStatePtr& s)
{
  LOG_DEBUG("Running fitness function...");

  Settings settings;
  foreach (QString k, s->getAllValues().keys())
  {
    settings.set(k, s->get(k));
  }
  //if you need to add any other temporary custom settings for this test that wouldn't
  //normally be used with the network conflation case tests, add those here
  //settings.set("", "");
  LOG_VART(settings);
  TempFileName temp;
  _settingsFileName = temp.getFileName();
  LOG_VARD(_settingsFileName);
  settings.storeJson(_settingsFileName);

  QStringList failedTests;
  for (int i = 0; i < _testCount; ++i)
  {
    AbstractTest* test = dynamic_cast<AbstractTest*>(_testSuite->getChildTestAt(i));
    const QString testName = QString::fromStdString(test->getName());
    LOG_DEBUG("Running " << testName << "...");
    initTest(test);
    //add our custom sa test option values
    test->addConfig(_settingsFileName);
    CppUnit::TestResult result;
    SimpleTestListener listener;
    result.addListener(&listener);
    test->run(&result);
    afterTestRun(test);

    if (listener.isFailure())
    {
      LOG_DEBUG("Failure: " << testName);
      failedTests.append(testName);
    }
  }

  QString failedTestsStr;
  if (failedTests.size() > 0)
  {
    failedTestsStr = _failedTestsToString(failedTests);
  }
  if (failedTests.size() < _lowestNumFailingTestsPerRun || _lowestNumFailingTestsPerRun == -1)
  {
    _lowestNumFailingTestsPerRun = failedTests.size();
    _failingTestsForBestRuns.clear();
    if (!failedTestsStr.isEmpty())
    {
      _failingTestsForBestRuns.append(failedTestsStr);
    }
  }
  else if (failedTests.size() == _lowestNumFailingTestsPerRun &&
           !failedTestsStr.isEmpty() && !_failingTestsForBestRuns.contains(failedTestsStr))
  {
    _failingTestsForBestRuns.append(failedTestsStr);
  }

  if (failedTests.size() == 0)
  {
    //This message will actually show if, by chance, the first selected random state
    //is successful.  However, that state is just a starting point for the actual simulated
    //annealing iterations.
    LOG_ERROR("\n\n***BOOM GOES THE DYNAMITE!***\n");
  }
  else
  {
    QString failureMsg =
      QString::number(failedTests.size()) + "/" + QString::number(_testCount) +
      " tests failed:\n\n";
    for (int i = 0; i < failedTests.size(); i++)
    {
      failureMsg += "\t" + failedTests[i] + "\n";
    }
    LOG_ERROR(failureMsg);
    LOG_ERROR("Lowest number of tests failed so far: " << _lowestNumFailingTestsPerRun);
    LOG_ERROR("");
  }

  return (double)failedTests.size() / (double)_testCount;
}

QString AbstractTestFitnessFunction::_failedTestsToString(const QStringList failedTests) const
{
  QString concatTestNames;
  for (int i = 0; i < failedTests.size(); i++)
  {
    concatTestNames += failedTests.at(i) + ";";
  }
  concatTestNames.chop(1);
  return concatTestNames;
}

}
