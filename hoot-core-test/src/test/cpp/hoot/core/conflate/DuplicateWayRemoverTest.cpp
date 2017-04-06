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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/DuplicateWayRemover.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
using namespace hoot;


// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>

// Standard
#include <sstream>
using namespace std;

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

#include "../TestUtils.h"

namespace hoot
{

class DuplicateWayRemoverTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(DuplicateWayRemoverTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runStrictTagMatchingOnTest);
  CPPUNIT_TEST(runStrictTagMatchingOffTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/algorithms/LongestCommonNodeStringTest.osm", map);

    MapProjector::projectToOrthographic(map);
    DuplicateWayRemover::removeDuplicates(map);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/conflate/LongestCommonNodeStringTest.osm");

    HOOT_FILE_EQUALS("test-files/conflate/LongestCommonNodeStringTest.osm",
                     "test-output/conflate/LongestCommonNodeStringTest.osm");
  }

  /*
   * In this test we add in some non-matching, non-name text tags for two ways, and since strict
   * matching is *on*, we *should not* see those two ways get merged.
   */
  void runStrictTagMatchingOnTest()
  {
    QDir().mkdir("test-output/conflate");
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/DcTigerRoads.osm", true, Status::Unknown1);

    //create a non matching text tag between two of the ways that will be examined
    map->getWay(FindWaysVisitor::findWaysByTag(map, "name", "Constitution Ave NW")[0])->getTags().set("email", "blah");

    DuplicateWayRemover dupeWayRemover;
    dupeWayRemover.setStrictTagMatching(true);

    MapProjector::projectToOrthographic(map);
    dupeWayRemover.apply(map);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/conflate/DuplicateWayRemoverStrictTagMatchingOnTest.osm");

    HOOT_FILE_EQUALS("test-files/conflate/DuplicateWayRemoverStrictTagMatchingOnTest.osm",
                     "test-output/conflate/DuplicateWayRemoverStrictTagMatchingOnTest.osm");
  }

  /*
   * In this test we add in some non-matching, non-name text tags for two ways, and since strict
   * matching is *off*, we *should* see those two ways get merged.
   */
  void runStrictTagMatchingOffTest()
  {
    QDir().mkdir("test-output/conflate");
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/DcTigerRoads.osm", true, Status::Unknown1);

    //create a non matching text tag between two of the ways that will be examined
    map->getWay(FindWaysVisitor::findWaysByTag(map, "name", "Constitution Ave NW")[0])->getTags().set("email", "blah");

    DuplicateWayRemover dupeWayRemover;
    dupeWayRemover.setStrictTagMatching(false);

    MapProjector::projectToOrthographic(map);
    dupeWayRemover.apply(map);
    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, "test-output/conflate/DuplicateWayRemoverStrictTagMatchingOffTest.osm");

    HOOT_FILE_EQUALS("test-files/conflate/DuplicateWayRemoverStrictTagMatchingOffTest.osm",
                     "test-output/conflate/DuplicateWayRemoverStrictTagMatchingOffTest.osm");
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DuplicateWayRemoverTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DuplicateWayRemoverTest, "current");

