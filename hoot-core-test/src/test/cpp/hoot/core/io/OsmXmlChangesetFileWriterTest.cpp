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
 * @copyright Copyright (C) 2012, 2013, 2014, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetProvider.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/OsmXmlChangesetFileWriter.h>
#include <hoot/core/algorithms/changeset/TestOsmChangesetProvider.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

class OsmXmlChangesetFileWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmXmlChangesetFileWriterTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runSplitTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmXmlChangesetFileWriterTest()
  {
    TestUtils::mkpath("test-output/io/OsmXmlChangesetFileWriterTest");
  }

  void runSimpleTest()
  {
    boost::shared_ptr<ChangesetProvider> changesetProvider(new TestOsmChangesetProvider(false));
    OsmXmlChangesetFileWriter().write(
      "test-output/io/OsmXmlChangesetFileWriterTest/changeset.osc", changesetProvider);

    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/OsmXmlChangesetFileWriterTest/changeset.osc"),
      TestUtils::readFile("test-output/io/OsmXmlChangesetFileWriterTest/changeset.osc"));
  }

  void runSplitTest()
  {
    boost::shared_ptr<ChangesetProvider> changesetProvider(new TestOsmChangesetProvider(false));
    OsmXmlChangesetFileWriter writer;
    Settings testSettings = conf();
    testSettings.set("changeset.max.size", "5");
    writer.setConfiguration(testSettings);
    writer.write(
      "test-output/io/OsmXmlChangesetFileWriterTest/changeset.split.osc", changesetProvider);

    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/OsmXmlChangesetFileWriterTest/changeset.split.osc"),
      TestUtils::readFile("test-output/io/OsmXmlChangesetFileWriterTest/changeset.split.osc"));
    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/OsmXmlChangesetFileWriterTest/changeset-001.split.osc"),
      TestUtils::readFile("test-output/io/OsmXmlChangesetFileWriterTest/changeset-001.split.osc"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmXmlChangesetFileWriterTest, "quick");

}



