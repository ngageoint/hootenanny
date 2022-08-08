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
 * @copyright Copyright (C) 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "AbstractRegressionTest.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

namespace hoot
{

AbstractRegressionTest::AbstractRegressionTest(QDir d, QStringList confs) :
AbstractTest(d, confs),
_score(-1.0),
_testStatus(-1)
{
}

void AbstractRegressionTest::runTest()
{
  LOG_DEBUG("Running regression release test: " << _d.absolutePath());

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
    const QString msg =
      "Unable to change to regression release test directory: " + _d.absolutePath();
    LOG_ERROR(msg);
    throw IllegalArgumentException(msg);
  }

  LOG_DEBUG("Cleaning test: " << getName());
  QString cmd = "make clean-output";
  _testStatus = system(cmd.toStdString().c_str());
  if (_testStatus != 0)
  {
    const QString msg =
      QString("Failed cleaning data for regression release test.  Status: " +
      QString::number(_testStatus));
    LOG_ERROR(msg);
    throw HootException(msg);
  }

  LOG_DEBUG("Running test: " << getName());
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    cmd = "make test";
  }
  else
  {
    cmd = "make -s test";
  }
  _testStatus = system(cmd.toStdString().c_str());

  //only try to parse the score if the test completed
  try
  {
    //if (_testStatus == 0 || _testStatus == -1)
    //{
      _parseScore();
    //}
  }
  catch (const HootException& /*e*/)
  {
    //will throw if test errored out and didn't generate a scores file; since I can't nail down
    //the exit status for that situation, this is the only way to handle it
    _score = 0;
  }

  //change back to the starting dir before issuing the cppunit failure to ensure it actually happens
  if (!QDir::setCurrent(startingDir))
  {
    const QString msg = "Unable to change back to hoot tests directory: " + startingDir;
    LOG_ERROR(msg);
    throw HootException(msg);
  }
  LOG_VARD(QDir::currentPath());

  if (_testStatus != 0)
  {
    const QString msg =
      QString("Failed executing regression release test.  Status: " +
      QString::number(_testStatus));
    //thought I should always be getting status = -1 for score failures according to CheckScores.py,
    //but am not
    //if (_testStatus == -1)
    //{
      LOG_INFO(msg); //failed on score; expected to happen occasionally
    //}
    //else
    //{
      //LOG_ERROR(msg);
    //}
    //don't throw here, b/c test failures caused by low scores are expected happen occasionally
    CPPUNIT_ASSERT_MESSAGE(msg.toStdString(), false);
  }
  else
  {
    LOG_INFO("Test: " << getName() << " passed with score: " << _score);
  }
}

}
