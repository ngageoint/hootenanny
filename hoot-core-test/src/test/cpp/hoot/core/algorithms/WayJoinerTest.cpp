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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "../TestUtils.h"

//  Hoot
#include <hoot/core/algorithms/WayJoiner.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/conflate/splitter/CornerSplitter.h>
#include <hoot/core/conflate/splitter/IntersectionSplitter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MapProjector.h>

namespace hoot
{

class WayJoinerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(WayJoinerTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runCornerSplitterTest);
  CPPUNIT_TEST(runIntersectionSplitterTest);
  CPPUNIT_TEST(runConflateTest);
  CPPUNIT_TEST_SUITE_END();
public:

  void setUp()
  {
    TestUtils::mkpath("test-output/algorithms/wayjoiner");
  }

  void runSimpleTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/algorithms/wayjoiner/WayJoinerSimpleInput.osm", map);

    IntersectionSplitter::splitIntersections(map);

    WayJoiner::join(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/algorithms/wayjoiner/WayJoinerSimpleOutput.osm");

    HOOT_FILE_EQUALS("test-output/algorithms/wayjoiner/WayJoinerSimpleOutput.osm",
                     "test-files/algorithms/wayjoiner/WayJoinerSimpleExpected.osm");
  }

  void runCornerSplitterTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/algorithms/wayjoiner/WayJoinerCornerSplitterInput.osm", map);

    CornerSplitter::splitCorners(map);

    WayJoiner::join(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/algorithms/wayjoiner/WayJoinerCornerSplitterOutput.osm");

    HOOT_FILE_EQUALS("test-output/algorithms/wayjoiner/WayJoinerCornerSplitterOutput.osm",
                     "test-files/algorithms/wayjoiner/WayJoinerCornerSplitterExpected.osm");
  }

  void runIntersectionSplitterTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/algorithms/wayjoiner/WayJoinerIntersectionSplitterInput.osm", map);

    IntersectionSplitter::splitIntersections(map);

    WayJoiner::join(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/algorithms/wayjoiner/WayJoinerIntersectionSplitterOutput.osm");

    HOOT_FILE_EQUALS("test-output/algorithms/wayjoiner/WayJoinerIntersectionSplitterOutput.osm",
                     "test-files/algorithms/wayjoiner/WayJoinerIntersectionSplitterExpected.osm");
  }

  void runConflateTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();

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
    writer.write(map, "test-output/algorithms/wayjoiner/WayJoinerConflateOutput.osm");

    HOOT_FILE_EQUALS("test-output/algorithms/wayjoiner/WayJoinerConflateOutput.osm",
                     "test-files/algorithms/wayjoiner/WayJoinerConflateExpected.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayJoinerTest, "quick");

}
