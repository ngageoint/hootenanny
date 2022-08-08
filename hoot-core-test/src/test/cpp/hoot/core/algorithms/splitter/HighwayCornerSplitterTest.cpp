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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/splitter/HighwayCornerSplitter.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

class HighwayCornerSplitterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HighwayCornerSplitterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runRoundedTest);
  CPPUNIT_TEST(runDogLegTest);
  CPPUNIT_TEST_SUITE_END();

public:

  HighwayCornerSplitterTest()
    : HootTestFixture("test-files/algorithms/splitter/",
                      "test-output/algorithms/splitter/")
  {
  }

  void runTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "HighwayCornerSplitter.osm", map);

    MapProjector::projectToPlanar(map);

    HighwayCornerSplitter::splitCorners(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "HighwayCornerSplitterOut.osm");

    HOOT_FILE_EQUALS( _inputPath + "HighwayCornerSplitterExpected.osm",
                     _outputPath + "HighwayCornerSplitterOut.osm");
  }

  void runRoundedTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "HighwayCornerSplitter.osm", map);

    MapProjector::projectToPlanar(map);

    Settings s;
    //  Turn on the rounded corner splitting
    s.set(ConfigOptions::getHighwayCornerSplitterRoundedSplitKey(), true);
    s.set(ConfigOptions::getHighwayCornerSplitterRoundedMaxNodeCountKey(), 10);
    s.set(ConfigOptions::getHighwayCornerSplitterRoundedThresholdKey(), 75.0);

    HighwayCornerSplitter splitter(map);
    splitter.setConfiguration(s);
    splitter.splitCorners();

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "HighwayCornerSplitterRoundedOut.osm");

    HOOT_FILE_EQUALS( _inputPath + "HighwayCornerSplitterRoundedExpected.osm",
                     _outputPath + "HighwayCornerSplitterRoundedOut.osm");
  }

  void runDogLegTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "HighwayCornerSplitterDogLeg.osm", map);

    MapProjector::projectToPlanar(map);

    HighwayCornerSplitter::splitCorners(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "HighwayCornerSplitterDogLegOut.osm");

    HOOT_FILE_EQUALS( _inputPath + "HighwayCornerSplitterDogLegExpected.osm",
                     _outputPath + "HighwayCornerSplitterDogLegOut.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HighwayCornerSplitterTest, "quick");

}
