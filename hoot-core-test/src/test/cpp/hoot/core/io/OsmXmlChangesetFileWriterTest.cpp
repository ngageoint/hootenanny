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
 * @copyright Copyright (C) 2012, 2013, 2014, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetProvider.h>
#include <hoot/core/algorithms/changeset/TestOsmChangesetProvider.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/OsmJosmXmlChangesetFileWriter.h>
#include <hoot/core/io/OsmOscChangesetFileWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class OsmXmlChangesetFileWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmXmlChangesetFileWriterTest);
  CPPUNIT_TEST(runSimpleOscTest);
  CPPUNIT_TEST(runSimpleJosmOsmTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmXmlChangesetFileWriterTest()
    : HootTestFixture("test-files/io/OsmXmlChangesetFileWriterTest/",
                      "test-output/io/OsmXmlChangesetFileWriterTest/")
  {
  }

  void runSimpleOscTest()
  {
    std::shared_ptr<ChangesetProvider> changesetProvider = std::make_shared<TestOsmChangesetProvider>(false);
    OsmOscChangesetFileWriter uut;
    uut.setConfiguration(conf());
    uut.write(_outputPath + "changeset.osc", changesetProvider);
    FileUtils::writeFully(_outputPath + "stats_osc", uut.getStatsTable(ChangesetStatsFormat::TextFormat));
    FileUtils::writeFully(_outputPath + "stats_osc.json", uut.getStatsTable(ChangesetStatsFormat::JsonFormat));

    HOOT_FILE_EQUALS(_inputPath + "changeset.osc", _outputPath + "changeset.osc");
    HOOT_FILE_EQUALS(_inputPath + "stats_osc", _outputPath + "stats_osc");
    HOOT_FILE_EQUALS(_inputPath + "stats_osc.json", _outputPath + "stats_osc.json");
  }

  void runSimpleJosmOsmTest()
  {
    std::shared_ptr<ChangesetProvider> changesetProvider = std::make_shared<TestOsmChangesetProvider>(false);
    OsmJosmXmlChangesetFileWriter uut;
    uut.setConfiguration(conf());
    uut.write(_outputPath + "changeset.osm", changesetProvider);
    FileUtils::writeFully(_outputPath + "stats_osm", uut.getStatsTable(ChangesetStatsFormat::TextFormat));
    FileUtils::writeFully(_outputPath + "stats_osm.json", uut.getStatsTable(ChangesetStatsFormat::JsonFormat));

    //  NOTE: HOOT_FILE_EQUALS() loads the OSM files and compares them but in this case a simple string comparison works
    HOOT_STR_EQUALS(FileUtils::readFully(_inputPath + "changeset.osm"), FileUtils::readFully(_outputPath + "changeset.osm"));
    HOOT_FILE_EQUALS(_inputPath + "stats_osm", _outputPath + "stats_osm");
    HOOT_FILE_EQUALS(_inputPath + "stats_osm.json", _outputPath + "stats_osm.json");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmXmlChangesetFileWriterTest, "quick");

}
