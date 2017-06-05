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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RegressionReleaseTest.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/test/TestSetup.h>

// Qt
#include <QTextStream>

namespace hoot
{

RegressionReleaseTest::RegressionReleaseTest(QDir d, QStringList confs) :
AbstractTest(d, confs),
_minPassingScore(-1.0)
{
}

void RegressionReleaseTest::runTest()
{
  //TestUtils::resetEnvironment();
  LOG_DEBUG("Running regression release test: " << _d.absolutePath());

  // configures and cleans up the conf() environment
  //LOG_VART(_confs);
  //TestSetup st(_confs);

  QFileInfo makeFile(_d, "Makefile");
  if (!makeFile.exists())
  {
    //for some reason, not seeing these exceptions logged at test time, so logging them here for now
    const QString msg = "Unable to find Makefile for regression release test: " + _d.absolutePath();
    LOG_ERROR(msg);
    throw IllegalArgumentException(msg);
  }

  const QString startingDir = QDir::currentPath();
  if (!QDir::setCurrent(_d.absolutePath()))
  {
    const QString msg = "Unable to change to test directory: " + _d.absolutePath();
    LOG_ERROR(msg);
    throw IllegalArgumentException(msg);
  }

  LOG_DEBUG("Cleaning test: " << getName());
  QString cmd = "make clean-output";
  int retval = system(cmd.toStdString().c_str());
  if (retval != 0)
  {
    const QString msg =
      QString("Failed cleaning data for regression release test.  Status: " +
      QString::number(retval));
    LOG_ERROR(msg);
    CPPUNIT_ASSERT_MESSAGE(msg.toStdString(), false);
  }
  LOG_DEBUG("Running test: " << getName());
  cmd = "make test";
  retval = system(cmd.toStdString().c_str());
  if (retval != 0)
  {
    const QString msg =
      QString("Failed executing regression release test.  Status: " +
      QString::number(retval));
    LOG_ERROR(msg);
    CPPUNIT_ASSERT_MESSAGE(msg.toStdString(), false);
  }

  //check test score and pass if >= _minPassingScore; fail otherwise
  QDir scoresDir("scores");
  QStringList nameFilters;
  nameFilters.append("*scores.txt");
  const QStringList scoresDirContents = scoresDir.entryList(nameFilters, QDir::Files);
  if (scoresDirContents.size() != 1)
  {
    const QString msg =
      "Found " + QString::number(scoresDirContents.size()) + " score files and expected to " +
      "find one scores file.";
    LOG_ERROR(msg);
    throw HootException(msg);
  }
  LOG_VARD(scoresDirContents[0]);
  QFile scoresFile("scores/" + scoresDirContents[0]);
  if (!scoresFile.open(QIODevice::ReadOnly))
  {
    const QString msg = "Unable to open scores file: scores/" + scoresDirContents[0];
    LOG_ERROR(msg);
    throw HootException(msg);
  }
  QTextStream inStream(&scoresFile);
  QString line;
  bool foundConflatedScoreLine = false;
  unsigned int overallScore = -1;
  do
  {
    line = inStream.readLine();
    LOG_VARD(line);
    if (line.toLower().contains("conflated"))
    {
      foundConflatedScoreLine = true;
    }
    else if (foundConflatedScoreLine && line.toLower().startsWith("overall"))
    {
      overallScore = line.split("\s")[1].toInt();
      LOG_VARD(overallScore);
    }
  }
  while (!line.isNull() && overallScore == -1);
  LOG_VARD(overallScore);

  LOG_VARD(_minPassingScore);
  if (overallScore >= _minPassingScore)
  {
    LOG_DEBUG("Test: " << getName() << " passed with score: " << overallScore);
    _minPassingScore = overallScore;
  }
  else
  {
    LOG_DEBUG("Test: " << getName() << " failed with score: " << overallScore);
    CPPUNIT_ASSERT_MESSAGE(
      QString("Failed executing regression release test: " +
        QString::fromStdString(getName())).toStdString(),
      false);
  }
  LOG_VARD(_minPassingScore);

  if (!QDir::setCurrent(startingDir))
  {
    const QString msg = "Unable to change back to hoot tests directory: " + startingDir;
    LOG_ERROR(msg);
    throw HootException(msg);
  }
  LOG_VARD(QDir::currentPath());
}

}
