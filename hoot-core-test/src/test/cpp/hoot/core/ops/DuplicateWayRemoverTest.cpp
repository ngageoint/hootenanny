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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/DuplicateWayRemover.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard
#include <sstream>
using namespace std;

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{

class DuplicateWayRemoverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(DuplicateWayRemoverTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runStrictTagMatchingOnTest);
  CPPUNIT_TEST(runStrictTagMatchingOffTest);
  CPPUNIT_TEST_SUITE_END();

public:

  DuplicateWayRemoverTest()
    : HootTestFixture("test-files/ops/DuplicateWayRemoverTest/",
                      "test-output/ops/DuplicateWayRemoverTest/")
  {
    setResetType(ResetAll);
  }

  void runTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/algorithms/LongestCommonNodeStringTest.osm", map);

    MapProjector::projectToOrthographic(map);
    DuplicateWayRemover::removeDuplicates(map);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "LongestCommonNodeStringTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "LongestCommonNodeStringTest.osm",
                     _outputPath + "LongestCommonNodeStringTest.osm");
  }

  void runStrictTagMatchingOnTest()
  {
    /*
     * In this test we add in some non-matching, non-name text tags for two ways, and since strict
     * matching is *on*, we *should not* see those two ways get merged.
     */

    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/DcTigerRoads.osm", true, Status::Unknown1);

    //create a non matching text tag between two of the ways that will be examined
    map->getWay(
      ElementIdsVisitor::findElementsByTag(
        map, ElementType::Way, "name", "Constitution Ave NW")[0])->getTags().set("email", "blah");

    DuplicateWayRemover dupeWayRemover;
    dupeWayRemover.setStrictTagMatching(true);

    MapProjector::projectToOrthographic(map);
    dupeWayRemover.apply(map);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "DuplicateWayRemoverStrictTagMatchingOnTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "DuplicateWayRemoverStrictTagMatchingOnTest.osm",
                     _outputPath + "DuplicateWayRemoverStrictTagMatchingOnTest.osm");
  }

  void runStrictTagMatchingOffTest()
  {
    /*
     * In this test we add in some non-matching, non-name text tags for two ways, and since strict
     * matching is *off*, we *should* see those two ways get merged.
     */

    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/DcTigerRoads.osm", true, Status::Unknown1);

    //create a non matching text tag between two of the ways that will be examined
    map->getWay(
      ElementIdsVisitor::findElementsByTag(
        map, ElementType::Way, "name", "Constitution Ave NW")[0])->getTags().set("email", "blah");

    DuplicateWayRemover dupeWayRemover;
    dupeWayRemover.setStrictTagMatching(false);

    MapProjector::projectToOrthographic(map);
    dupeWayRemover.apply(map);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "DuplicateWayRemoverStrictTagMatchingOffTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "DuplicateWayRemoverStrictTagMatchingOffTest.osm",
                     _outputPath + "DuplicateWayRemoverStrictTagMatchingOffTest.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DuplicateWayRemoverTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DuplicateWayRemoverTest, "current");

}
