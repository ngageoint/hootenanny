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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

//  Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/WayJoinerAdvanced.h>
#include <hoot/core/algorithms/WayJoinerBasic.h>
#include <hoot/core/algorithms/splitter/CornerSplitter.h>
#include <hoot/core/algorithms/splitter/IntersectionSplitter.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MapProjector.h>

namespace hoot
{

class WayJoinerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayJoinerTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runCornerSplitterTest);
  CPPUNIT_TEST(runIntersectionSplitterTest);
  CPPUNIT_TEST(runConflateTest);
  CPPUNIT_TEST(runAdvancedConflateTest);
  CPPUNIT_TEST_SUITE_END();
public:

  WayJoinerTest()
    : HootTestFixture("test-files/algorithms/wayjoiner/",
                      "test-output/algorithms/wayjoiner/")
  {
    setResetType(ResetAll);
  }

  void runSimpleTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "WayJoinerSimpleInput.osm", map);

    IntersectionSplitter::splitIntersections(map);

    WayJoinerBasic::joinWays(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "WayJoinerSimpleOutput.osm");

    HOOT_FILE_EQUALS(_outputPath + "WayJoinerSimpleOutput.osm",
                     _inputPath + "WayJoinerSimpleExpected.osm");
  }

  void runCornerSplitterTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "WayJoinerCornerSplitterInput.osm", map);

    CornerSplitter::splitCorners(map);

    WayJoinerBasic::joinWays(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "WayJoinerCornerSplitterOutput.osm");

    HOOT_FILE_EQUALS(_outputPath + "WayJoinerCornerSplitterOutput.osm",
                     _inputPath + "WayJoinerCornerSplitterExpected.osm");
  }

  void runIntersectionSplitterTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "WayJoinerIntersectionSplitterInput.osm", map);

    IntersectionSplitter::splitIntersections(map);

    WayJoinerBasic::joinWays(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "WayJoinerIntersectionSplitterOutput.osm");

    HOOT_FILE_EQUALS(_outputPath + "WayJoinerIntersectionSplitterOutput.osm",
                     _inputPath + "WayJoinerIntersectionSplitterExpected.osm");
  }

  void runConflateTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyTestB.osm", map);

    NamedOp(ConfigOptions().getConflatePreOps()).apply(map);

    UnifyingConflator conflator;
    conflator.apply(map);

    NamedOp(ConfigOptions().getConflatePostOps()).apply(map);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(true);
    writer.setIncludeHootInfo(true);
    writer.setIncludePid(true);
    writer.write(map, _outputPath + "WayJoinerConflateOutput.osm");

    HOOT_FILE_EQUALS(_outputPath + "WayJoinerConflateOutput.osm",
                     _inputPath + "WayJoinerConflateExpected.osm");
  }

  void runAdvancedConflateTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyTestB.osm", map);

    NamedOp(ConfigOptions().getConflatePreOps()).apply(map);

    UnifyingConflator conflator;
    Settings conf;
    //  Use the Advanced way joiner
    conf.set(ConfigOptions::getWayJoinerKey(), WayJoinerAdvanced::className());
    conflator.setConfiguration(conf);
    conflator.apply(map);

    NamedOp(ConfigOptions().getConflatePostOps()).apply(map);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(true);
    writer.setIncludeHootInfo(true);
    writer.setIncludePid(true);
    writer.write(map, _outputPath + "WayJoinerAdvancedConflateOutput.osm");

    HOOT_FILE_EQUALS(_outputPath + "WayJoinerAdvancedConflateOutput.osm",
                     _inputPath + "WayJoinerAdvancedConflateExpected.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayJoinerTest, "slow");

}
