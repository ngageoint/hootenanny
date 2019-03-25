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
 * @copyright Copyright (C) 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/splitter/CornerSplitter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class CornerSplitterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(CornerSplitterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runRoundedTest);
  CPPUNIT_TEST(runDogLegTest);
  CPPUNIT_TEST_SUITE_END();

public:

  CornerSplitterTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath("test-output/conflate/splitter");
  }

  void runTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/splitter/CornerSplitter.osm", map);

    MapProjector::projectToPlanar(map);

    CornerSplitter::splitCorners(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/conflate/splitter/CornerSplitterOut.osm");

    HOOT_FILE_EQUALS("test-output/conflate/splitter/CornerSplitterOut.osm",
                     "test-files/conflate/splitter/CornerSplitterExpected.osm");
  }

  void runRoundedTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/splitter/CornerSplitter.osm", map);

    MapProjector::projectToPlanar(map);

    Settings s;
    //  Turn on the rounded corner splitting
    s.set(ConfigOptions::getCornerSplitterRoundedSplitKey(), true);
    s.set(ConfigOptions::getCornerSplitterRoundedMaxNodeCountKey(), 10);
    s.set(ConfigOptions::getCornerSplitterRoundedThresholdKey(), 75.0);

    CornerSplitter splitter(map);
    splitter.setConfiguration(s);
    splitter.splitCorners();

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/conflate/splitter/CornerSplitterRoundedOut.osm");

    HOOT_FILE_EQUALS("test-output/conflate/splitter/CornerSplitterRoundedOut.osm",
                     "test-files/conflate/splitter/CornerSplitterRoundedExpected.osm");
  }

  void runDogLegTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/splitter/CornerSplitterDogLeg.osm", map);

    MapProjector::projectToPlanar(map);

    CornerSplitter::splitCorners(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/conflate/splitter/CornerSplitterDogLegOut.osm");

    HOOT_FILE_EQUALS("test-output/conflate/splitter/CornerSplitterDogLegOut.osm",
                     "test-files/conflate/splitter/CornerSplitterDogLegExpected.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CornerSplitterTest, "quick");

}
