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
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class OsmXmlChangesetFileWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmXmlChangesetFileWriterTest);
  CPPUNIT_TEST(runSimpleTest);
  // TODO: add a test for multiple changeset provider inputs
  CPPUNIT_TEST_SUITE_END();

public:

  OsmXmlChangesetFileWriterTest()
    : HootTestFixture("test-files/io/OsmXmlChangesetFileWriterTest/",
                      "test-output/io/OsmXmlChangesetFileWriterTest/")
  {
  }

  void runSimpleTest()
  {
    std::shared_ptr<ChangesetProvider> changesetProvider(new TestOsmChangesetProvider(false));
    OsmXmlChangesetFileWriter uut;
    uut.write(_outputPath + "changeset.osc", changesetProvider);
    FileUtils::writeFully(_outputPath + "stats", uut.getStatsTable(ChangesetStatsFormat::Text));
    FileUtils::writeFully(
      _outputPath + "stats.json", uut.getStatsTable(ChangesetStatsFormat::Json));

    HOOT_FILE_EQUALS(_inputPath + "changeset.osc", _outputPath + "changeset.osc");
    HOOT_FILE_EQUALS(_inputPath + "stats", _outputPath + "stats");
    HOOT_FILE_EQUALS(_inputPath + "stats.json", _outputPath + "stats.json");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmXmlChangesetFileWriterTest, "quick");

}



