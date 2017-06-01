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

#include "RegressionReleaseTestFitnessFunction.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include "RegressionReleaseTestSuite.h"
#include "RegressionReleaseTest.h"

namespace hoot
{

RegressionReleaseTestFitnessFunction::RegressionReleaseTestFitnessFunction() :
AbstractTestFitnessFunction()
{
  //TODO: make this configurable?
  _dir = "/fouo/hoot-tests/network-tests.child/release_test.child";
  _testSuite.reset(new RegressionReleaseTestSuite(_dir));
  QStringList confs;
  _testSuite->loadDir(_dir, confs);
  _testCount = _testSuite->getChildTestCount();
  _highestOverallScores.clear();
}

void RegressionReleaseTestFitnessFunction::initTest(AbstractTest* test)
{
  QFile settingsFile(_settingsFileName);
  //for now, this will only work with network conflation regression release tests, since
  //they are set up to handle this configuration file
  const QString settingsFileDest =
    _dir + "/" + QString::fromStdString(test->getName()) + "/Config.conf";
  if (!settingsFile.copy(settingsFileDest))
  {
    throw new HootException("Unable to copy configuration file to: " + settingsFileDest);
  }

  RegressionReleaseTest* regressionReleaseTest = dynamic_cast<RegressionReleaseTest*>(test);
  LOG_VARD(QString::fromStdString(test->getName()));
  if (!_highestOverallScores.contains(QString::fromStdString(test->getName())))
  {
    _highestOverallScores[QString::fromStdString(test->getName())] = -1.0;
  }
  LOG_VARD(_highestOverallScores[QString::fromStdString(test->getName())]);
  if (_highestOverallScores[QString::fromStdString(test->getName())] != -1.0)
  {
    regressionReleaseTest->setMinPassingScore(
      _highestOverallScores[QString::fromStdString(test->getName())]);
  }
}

void RegressionReleaseTestFitnessFunction::afterTestRun(AbstractTest* test)
{
  RegressionReleaseTest* regressionReleaseTest = dynamic_cast<RegressionReleaseTest*>(test);
  LOG_VARD(QString::fromStdString(test->getName()));
  LOG_VARD(regressionReleaseTest->getMinPassingScore());
  if (regressionReleaseTest->getMinPassingScore() >
      _highestOverallScores[QString::fromStdString(test->getName())])
  {
    _highestOverallScores[QString::fromStdString(test->getName())] =
      regressionReleaseTest->getMinPassingScore();
  }
}

}
