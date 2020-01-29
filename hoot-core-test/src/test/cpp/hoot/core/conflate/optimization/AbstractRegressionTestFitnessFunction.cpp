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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "AbstractRegressionTestFitnessFunction.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/conflate/optimization/RegressionTestSuite.h>
#include <hoot/core/conflate/optimization/AbstractRegressionTest.h>

// Qt
#include <QFileInfo>

// Standard
#include <cassert>

namespace hoot
{

AbstractRegressionTestFitnessFunction::AbstractRegressionTestFitnessFunction(const QString& dir,
                                                                             const QString& configFile,
                                                                             const QString& testDirExtension)
  : AbstractTestFitnessFunction(),
    _configFile(configFile)
{
  _testSuite.reset(new RegressionTestSuite(dir, testDirExtension));
  QStringList confs;
  _testSuite->loadDir(dir, confs);
  _testCount = _testSuite->getChildTestCount();
  LOG_VARD(_testCount);
  _testsToBestScores.clear();
}

void AbstractRegressionTestFitnessFunction::_createConfig(const QString& testName,
                                                          Settings& testSettings)
{
  //Calling test->addConfig with the network config file to add in the non-variable config options
  //won't work here, since configs added in that manner have no effect on regression tests.
  //Instead, we need to manually add those settings in.

  //add the default network settings to the test settings
  // starting from conf() makes the config files huge, when I only want to see the ones
  //in _configFile plus those in testSettings, but if I don't start from an existing config (or
  //call loadDefaults) then the config parser errors out...
  Settings updatedSettings = conf();
  if (!_configFile.trimmed().isEmpty())
  {
    updatedSettings.loadJson(_configFile);
  }
  foreach (QString k, testSettings.getAll().keys())
  {
    updatedSettings.set(k, testSettings.get(k).toString());
  }
  LOG_VART(updatedSettings);

  //for now, this will only work with network conflation regression release tests, since
  //they are the only ones set up to handle configuration file management in this fashion; changing
  //the unifying version of the tests to do the same, if needed, wouldn't be too difficult
  const QString settingsFileDestName = testName + "/Config.conf";
  QFile settingsFileDest(settingsFileDestName);
  if (settingsFileDest.exists() && !settingsFileDest.remove())
  {
    throw HootException("Unable to remove previous test configuration file: " + settingsFileDestName);
  }
  LOG_DEBUG("Writing test conf file to: " << settingsFileDestName << "...");
  updatedSettings.storeJson(settingsFileDestName);
}

void AbstractRegressionTestFitnessFunction::_checkForBetterScoreFromTest(
  AbstractRegressionTest* regressionTest)
{
  const QString testName = QString::fromStdString(regressionTest->getName());
  LOG_VARD(testName);
  LOG_VARD(regressionTest->getScore());
  LOG_VARD(regressionTest->getTestStatus());

  if (!_testsToBestScores.contains(testName))
  {
    _testsToBestScores[testName] = -1.0;
  }

  if (regressionTest->getScore() > _testsToBestScores[testName])
  {
    _testsToBestScores[testName] = regressionTest->getScore();

    if (regressionTest->getScore() != 0)  //zero means the test errored out
    {
      if (regressionTest->getTestStatus() == 0)
      {
        LOG_INFO(
          regressionTest->getScore() << " is a new high score from passing test: " << testName);
        LOG_INFO("\n\n***BOOM GOES THE DYNAMITE!***\n");
      }
      else
      {
        LOG_INFO(
          regressionTest->getScore() << " is a new high score from failing test: " << testName);
      }
    }
  }

  LOG_VARD(_testsToBestScores[testName]);
}

QString AbstractRegressionTestFitnessFunction::bestScoresPerTestToString() const
{
  QString str = "Best scores:\n";
  for (QMap<QString, double>::const_iterator it = _testsToBestScores.begin();
       it != _testsToBestScores.end(); ++it)
  {
    str += "\t" + it.key() + ": " + QString::number(it.value()) + "\n";
  }
  str.chop(1);
  return str;
}

void AbstractRegressionTestFitnessFunction::initTest(Settings& testSettings)
{
  LOG_INFO("Initializing test: " << _test->getName() << "...");
  LOG_VARD(QDir::currentPath());
  _createConfig(QString::fromStdString(_test->getName()), testSettings);
}

double AbstractRegressionTestFitnessFunction::f(const Tgs::ConstStatePtr& s)
{
  AbstractTestFitnessFunction::f(s);

  AbstractRegressionTest* regressionTest = dynamic_cast<AbstractRegressionTest*>(_test);
  if (!regressionTest)
  {
    throw HootException("Invalid test class.");
  }
  LOG_VARD(regressionTest->getScore());
  LOG_VARD(regressionTest->getTestStatus());
  //score should be positive if the test ran or zero if the test errored out; never = -1
  assert(regressionTest->getScore() != -1);
  if (regressionTest->getScore() == -1)
  {
    throw HootException("invalid score");
  }

  if (regressionTest->getScore() == 0)
  {
    return regressionTest->getScore();
  }
  else
  {
    return 1 / regressionTest->getScore();
  }
}

void AbstractRegressionTestFitnessFunction::afterTestRun()
{
  LOG_DEBUG("Updating test after run: " << _test->getName() << "...");
  LOG_VARD(QDir::currentPath());
  AbstractRegressionTest* regressionTest = dynamic_cast<AbstractRegressionTest*>(_test);
  if (!regressionTest)
  {
    throw HootException("Invalid test class.");
  }
  _checkForBetterScoreFromTest(regressionTest);
  LOG_INFO(bestScoresPerTestToString());
}

}
