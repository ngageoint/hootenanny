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

#include "AbstractRegressionTestFitnessFunction.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Settings.h>
#include "RegressionTestSuite.h"
#include "AbstractRegressionTest.h"

namespace hoot
{

AbstractRegressionTestFitnessFunction::AbstractRegressionTestFitnessFunction(QString dir,
                                                                             QString configFile,
                                                                             QString testDirExtension,
                                                                             QString baseDirExtension) :
AbstractTestFitnessFunction(),
_configFile(configFile)
{
  _testSuite.reset(new RegressionTestSuite(dir, testDirExtension, baseDirExtension));
  QStringList confs;
  _testSuite->loadDir(dir, confs);
  _testCount = _testSuite->getChildTestCount();
  _highestOverallScores.clear();
}

void AbstractRegressionTestFitnessFunction::_createConfig(const QString testName)
{
  //Calling test->addConfig with the network config file to add in the non-variable config options
  //won't work here, since configs added in that manner have no effect on regression tests.
  //Instead, we need to manually add those settings in.

  //add the default network settings to the test settings
  Settings testSettings;
  testSettings.loadDefaults();
  testSettings.loadJson(_settingsFileName);
  Settings baseSettings;
  baseSettings.loadDefaults();
  baseSettings.loadJson(_configFile);
  foreach (QString k, baseSettings.getAll().keys())
  {
    testSettings.set(k, baseSettings.get(k).toString());
  }
  LOG_VARD(testSettings);
  LOG_VARD(testSettings.get("network.matcher"));
  LOG_VARD(testSettings.get("match.creators"));

  //for now, this will only work with network conflation regression release tests, since
  //they are the only ones set up to handle this configuration file management
  const QString settingsFileDestName = testName + "/Config.conf";
  QFile settingsFileDest(settingsFileDestName);
  if (settingsFileDest.exists() && !settingsFileDest.remove())
  {
    throw new HootException(
      "Unable to remove previous test configuration file: " + settingsFileDestName);
  }
  LOG_DEBUG("Writing test conf file to: " << settingsFileDestName << "...");
  testSettings.storeJson(settingsFileDestName);
}

void AbstractRegressionTestFitnessFunction::_updateTestWithCurrentScore(AbstractTest* test)
{
  AbstractRegressionTest* regressionTest = dynamic_cast<AbstractRegressionTest*>(test);
  if (!regressionTest)
  {
    throw HootException("Invalid test class.");
  }
  LOG_VARD(QString::fromStdString(test->getName()));
  if (!_highestOverallScores.contains(QString::fromStdString(test->getName())))
  {
    _highestOverallScores[QString::fromStdString(test->getName())] = -1;
  }
  LOG_VARD(_highestOverallScores[QString::fromStdString(test->getName())]);
  regressionTest->setMinPassingScore(
    _highestOverallScores[QString::fromStdString(test->getName())]);
  LOG_VARD(regressionTest->getMinPassingScore());
}

void AbstractRegressionTestFitnessFunction::_updateCurrentScoreFromTest(const int score,
                                                                        const QString testName)
{
  LOG_VARD(testName);
  LOG_VARD(score);
  if (!_highestOverallScores.contains(testName))
  {
    _highestOverallScores[testName] = -1;
  }
  if (score > _highestOverallScores[testName])
  {
    _highestOverallScores[testName] = score;
  }
  LOG_VARD(_highestOverallScores[testName]);
}

QString AbstractRegressionTestFitnessFunction::highestOverallScoresToString() const
{
  QString str = "Best scores:\n";
  for (QMap< QString, int >::const_iterator it = _highestOverallScores.begin();
       it != _highestOverallScores.end(); ++it)
  {
    str += "\t" + it.key() + ": " + QString::number(it.value()) + "\n";
  }
  str.chop(1);
  return str;
}

void AbstractRegressionTestFitnessFunction::initTest()
{
  LOG_ERROR("Initializing test: " << _test->getName() << "...");
  LOG_VARD(QDir::currentPath());
  _createConfig(QString::fromStdString(_test->getName()));
  _updateTestWithCurrentScore(_test);
}

double AbstractRegressionTestFitnessFunction::f(const Tgs::ConstStatePtr& s)
{
  AbstractTestFitnessFunction::f(s);

  AbstractRegressionTest* regressionTest = dynamic_cast<AbstractRegressionTest*>(_test);
  if (!regressionTest)
  {
    throw HootException("Invalid test class.");
  }
  LOG_VARD(regressionTest->getOverallScore());

  return (double)(1 / regressionTest->getOverallScore());
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
  _updateCurrentScoreFromTest(
    regressionTest->getOverallScore(),
    QString::fromStdString(regressionTest->getName()));
  LOG_ERROR(highestOverallScoresToString());
}

}
