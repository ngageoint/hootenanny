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
#include "AbstractRegressionTest.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

namespace hoot
{

AbstractRegressionTest::AbstractRegressionTest(QDir d, QStringList confs) :
AbstractTest(d, confs),
_score(-1.0)
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
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    cmd = "make test";
  }
  else
  {
    cmd = "make -s test";
  }
  retval = system(cmd.toStdString().c_str());
  if (retval != 0)
  {
    const QString msg =
      QString("Failed executing regression release test.  Status: " +
      QString::number(retval));
    LOG_ERROR(msg);
    if (!QDir::setCurrent(startingDir))
    {
      const QString msg = "Unable to change back to hoot tests directory: " + startingDir;
      LOG_ERROR(msg);
      throw HootException(msg);
    }
    LOG_VARD(QDir::currentPath());
    CPPUNIT_ASSERT_MESSAGE(msg.toStdString(), false);
  }

  _parseScore();

  if (!QDir::setCurrent(startingDir))
  {
    const QString msg = "Unable to change back to hoot tests directory: " + startingDir;
    LOG_ERROR(msg);
    throw HootException(msg);
  }
  LOG_VARD(QDir::currentPath());
}

}
