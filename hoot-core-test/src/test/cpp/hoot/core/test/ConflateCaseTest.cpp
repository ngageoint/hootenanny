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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "ConflateCaseTest.h"

// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/cmd/ConflateCmd.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/test/TestSetup.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>


namespace hoot
{

static const QString multiaryConflateClass = "hoot::MultiaryPoiConflateCmd";

ConflateCaseTest::ConflateCaseTest(QDir d, QStringList confs) :
AbstractTest(d, confs)
{
}

void ConflateCaseTest::_runConflateCmd()
{
  if (QFileInfo(_d, "README.txt").exists() == false)
  {
    LOG_WARN("Please create a meaningful README.txt in " + _d.path());
  }
  QFileInfo in1(_d, "Input1.osm");
  if (in1.exists() == false)
  {
    throw IllegalArgumentException(
      "Unable to find Input1.osm in conflate case: " + _d.absolutePath());
  }
  QFileInfo in2(_d, "Input2.osm");
  if (in2.exists() == false)
  {
    throw IllegalArgumentException(
      "Unable to find Input2.osm in conflate case: " + _d.absolutePath());
  }

  // This is also set in Testing.conf.
  conf().set("conflate.tag.disable.value.truncation", "true");

  QString testOutput = _d.absoluteFilePath("Output.osm");

  QStringList args;
  args << in1.absoluteFilePath();
  args << in2.absoluteFilePath();
  args << testOutput;
  bool differential = ConfigOptions().getTestCaseConflateDifferential();
  const bool differentialWithTags = ConfigOptions().getTestCaseConflateDifferentialIncludeTags();
  if (differentialWithTags)
  {
    // let this override and correct what would otherwise be an invalid config
    differential = true;
  }
  if (differential)
  {
    args << "--differential";
  }
  if (differentialWithTags)
  {
    args << "--include-tags";
  }

  int result = -1;
  try
  {
    result = ConflateCmd().runSimple(args);
  }
  catch (const HootException& e)
  {
    CPPUNIT_ASSERT_MESSAGE(e.what(), false);
  }

  QFileInfo expected(_d, "Expected.osm");
  if (expected.exists() == false)
  {
    throw IllegalArgumentException(
      "Unable to find Expected.osm in conflate case: " + _d.absolutePath());
  }

  if (result != 0)
  {
    CPPUNIT_ASSERT_MESSAGE(QString("Conflate command had nonzero exit status").toStdString(), false);
  }

  if (!TestUtils::compareMaps(expected.absoluteFilePath(), testOutput))
  {
    CPPUNIT_ASSERT_MESSAGE(QString("Maps do not match").toStdString(), false);
  }
}

void ConflateCaseTest::_runMultiaryConflateCmd()
{
  BaseCommandPtr cmd =
    std::dynamic_pointer_cast<BaseCommand>(
      Factory::getInstance().constructObject<Command>(multiaryConflateClass));

  if (QFileInfo(_d, "README.txt").exists() == false)
  {
    LOG_WARN("Please create a meaningful README.txt in " + _d.path());
  }
  QList<QFileInfo> in;
  bool stillLooking = true;

  int i = 1;
  while (stillLooking)
  {
    QFileInfo fi(_d, QString("Input%1.osm").arg(i++));
    if (fi.exists())
    {
      in.append(fi);
    }
    else
    {
      stillLooking = false;
    }
  }

  if (in.size() < 2)
  {
    throw IllegalArgumentException(
      "Found fewer than two inputs in conflate case: " + _d.absolutePath());
  }

  QString testOutput = _d.absoluteFilePath("Output.osm");

  QStringList args;
  foreach (QFileInfo fi, in)
  {
    args << fi.absoluteFilePath();
  }
  args << testOutput;
  int result = -1;
  try
  {
    result = cmd->runSimple(args);
  }
  catch (const HootException& e)
  {
    CPPUNIT_ASSERT_MESSAGE(e.what(), false);
  }

  QFileInfo expected(_d, "Expected.osm");
  if (expected.exists() == false)
  {
    throw IllegalArgumentException("Unable to find Expected.osm in conflate case: " +
      _d.absolutePath());
  }

  if (result != 0)
  {
    CPPUNIT_ASSERT_MESSAGE(QString("Conflate command had nonzero exit status").toStdString(),
      false);
  }

  if (!TestUtils::compareMaps(expected.absoluteFilePath(), testOutput))
  {
    CPPUNIT_ASSERT_MESSAGE(QString("Maps do not match").toStdString(), false);
  }
}

void ConflateCaseTest::runTest()
{
  LOG_DEBUG("Running conflate case test...");

  // configures and cleans up the conf() environment
  TestSetup st(_confs);
  MatchFactory::getInstance().reset();

  if (ConfigOptions().getTestCaseConflateCmd() == ConflateCmd::className())
  {
    _runConflateCmd();
  }
  else if (ConfigOptions().getTestCaseConflateCmd() == multiaryConflateClass)
  {
    _runMultiaryConflateCmd();
  }
}

}
