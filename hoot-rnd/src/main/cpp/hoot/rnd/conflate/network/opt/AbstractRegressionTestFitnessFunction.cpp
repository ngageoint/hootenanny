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

// Qt
#include <QFileInfo>

namespace hoot
{

AbstractRegressionTestFitnessFunction::AbstractRegressionTestFitnessFunction(QString dir,
                                                                             QString configFile,
                                                                             QString testDirExtension) :
AbstractTestFitnessFunction(),
_configFile(configFile)
{
  _testSuite.reset(new RegressionTestSuite(dir, testDirExtension));
  QStringList confs;
  _testSuite->loadDir(dir, confs);
  _testCount = _testSuite->getChildTestCount();
  LOG_VARD(_testCount);
  _testsToBestScores.clear();
}

void AbstractRegressionTestFitnessFunction::_createConfig(const QString testName,
                                                          Settings& testSettings)
{
  //Calling test->addConfig with the network config file to add in the non-variable config options
  //won't work here, since configs added in that manner have no effect on regression tests.
  //Instead, we need to manually add those settings in.

  //add the default network settings to the test settings
  Settings updatedSettings = conf();
  updatedSettings.loadJson(_configFile);
  foreach (QString k, testSettings.getAll().keys())
  {
    updatedSettings.set(k, testSettings.get(k).toString());
  }
  LOG_VART(updatedSettings);
  LOG_VARD(updatedSettings.get("network.optimization.iterations"));
  LOG_VARD(updatedSettings.get("way.matcher.max.angle"));
  LOG_VARD(updatedSettings.get("network.conflicts.aggression"));
  LOG_VARD(updatedSettings.get("network.conflicts.weight.influence"));
  LOG_VARD(updatedSettings.get("network.conflicts.outbound.weighting"));
  LOG_VARD(updatedSettings.get("way.matcher.heading.delta"));
  LOG_VARD(updatedSettings.get("network.match.threshold"));
  LOG_VARD(updatedSettings.get("network.miss.threshold"));
  LOG_VARD(updatedSettings.get("way.merger.min.split.size"));
  LOG_VARD(updatedSettings.get("network.conflicts.stub.through.weighting"));
  LOG_VARD(updatedSettings.get("network.review.threshold"));
  LOG_VARD(updatedSettings.get("network.conflicts.partial.handicap"));
  LOG_VARD(updatedSettings.get("network.conflicts.stub.handicap"));
  LOG_VARD(updatedSettings.get("network.max.stub.length"));

  //for now, this will only work with network conflation regression release tests, since
  //they are the only ones set up to handle configuration file management in this fashion; changing
  //the unifying version of the tests to do the same, if needed, wouldn't be too difficult
  const QString settingsFileDestName = testName + "/Config.conf";
  QFile settingsFileDest(settingsFileDestName);
  if (settingsFileDest.exists() && !settingsFileDest.remove())
  {
    throw new HootException(
      "Unable to remove previous test configuration file: " + settingsFileDestName);
  }
  LOG_DEBUG("Writing test conf file to: " << settingsFileDestName << "...");
  updatedSettings.storeJson(settingsFileDestName);
}

void AbstractRegressionTestFitnessFunction::_getBestScoreFromTest(const double score,
                                                                  const QString testName)
{
  LOG_VARD(testName);
  LOG_VARD(score);
  if (!_testsToBestScores.contains(testName))
  {
    _testsToBestScores[testName] = -1.0;
  }
  if (score > _testsToBestScores[testName])
  {
    LOG_ERROR(score << " is a new high score for: " << testName);
    LOG_ERROR("\n\n***BOOM GOES THE DYNAMITE!***\n");
    _testsToBestScores[testName] = score;
  }
  LOG_VARD(_testsToBestScores[testName]);
}

QString AbstractRegressionTestFitnessFunction::bestScoresPerTestToString() const
{
  QString str = "Best scores:\n";
  for (QMap< QString, double >::const_iterator it = _testsToBestScores.begin();
       it != _testsToBestScores.end(); ++it)
  {
    str += "\t" + it.key() + ": " + QString::number(it.value()) + "\n";
  }
  str.chop(1);
  return str;
}

void AbstractRegressionTestFitnessFunction::initTest(Settings& testSettings)
{
  LOG_ERROR("Initializing test: " << _test->getName() << "...");
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

  return 1 / regressionTest->getScore();
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
  _getBestScoreFromTest(
    regressionTest->getScore(), QString::fromStdString(regressionTest->getName()));
  LOG_ERROR(bestScoresPerTestToString());
}

}
