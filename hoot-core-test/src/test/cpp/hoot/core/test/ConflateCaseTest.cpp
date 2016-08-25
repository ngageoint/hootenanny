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
#include "ConflateCaseTest.h"

// hoot
#include <hoot/core/cmd/ConflateCmd.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/conflate/MergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>

#include "../TestUtils.h"

namespace hoot
{

class SetupTest
{
public:
  SetupTest() { reset(); }
  ~SetupTest() { reset(); }

  void reset()
  {
    TestUtils::resetEnvironment();
  }
};

ConflateCaseTest::ConflateCaseTest(QDir d, QStringList confs) :
  CppUnit::TestCase(d.absolutePath().toStdString()),
  _d(d),
  _confs(confs)
{
}

void ConflateCaseTest::runTest()
{
  // configures and cleans up the conf() environment
  SetupTest st;

  for (int i = 0; i < _confs.size(); i++)
  {
    conf().loadJson(_confs[i]);
  }

  bool failed = false;

  ConflateCmd cmd;

  if (QFileInfo(_d, "README.txt").exists() == false)
  {
    LOG_WARN("Please create a meaningful README.txt in " + _d.path());
  }
  QFileInfo in1(_d, "Input1.osm");
  if (in1.exists() == false)
  {
    throw IllegalArgumentException("Unable to find Input1.osm in conflate case: " +
      _d.absolutePath());
  }
  QFileInfo in2(_d, "Input2.osm");
  if (in2.exists() == false)
  {
    throw IllegalArgumentException("Unable to find Input2.osm in conflate case: " +
      _d.absolutePath());
  }

  QString testOutput = _d.absoluteFilePath("Output.osm");

  QStringList args;
  args << in1.absoluteFilePath();
  args << in2.absoluteFilePath();
  args << testOutput;
  int result = -1;
  try
  {
    result = cmd.runSimple(args);
  }
  catch(HootException& e)
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
    failed = true;
  }

  if (!TestUtils::compareMaps(expected.absoluteFilePath(), testOutput))
  {
    failed = true;
  }

  if (failed)
  {
    CPPUNIT_ASSERT_MESSAGE(QString("Maps do not match").toStdString(), false);
  }
}

}
