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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/io/WKTReader.h>
#include <geos/geom/Point.h>

// hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/RemoveDuplicateAreaVisitor.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

// Qt
#include <QDebug>
#include <QDir>

// TGS
#include <tgs/Statistics/Random.h>

#include "../TestUtils.h"

namespace hoot
{
using namespace Tgs;

class RemoveDuplicateAreaVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RemoveDuplicateAreaVisitorTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/visitors/RemoveDuplicateAreaVisitorTest.osm", map);
    MapProjector::projectToPlanar(map);

    RemoveDuplicateAreaVisitor uut;
    map->visitRw(uut);

//#warning debug
//    MapProjector::reprojectToWgs84(map);
//    QDir().mkpath("test-output/visitors/");
//    OsmXmlWriter writer;
//    writer.write(map, "test-output/visitors/RemoveDuplicateAreaVisitorTest.osm");

    // these "duplicates" should not be removed.
    CPPUNIT_ASSERT_EQUAL(2ul, FindWaysVisitor::findWaysByTag(map, "note", "tag difference").size());
    CPPUNIT_ASSERT_EQUAL(2ul, FindWaysVisitor::findWaysByTag(map, "note", "small difference").size());
    CPPUNIT_ASSERT_EQUAL(2ul, FindWaysVisitor::findWaysByTag(map, "note", "different name").size());

    // these duplicates should be removed.
    CPPUNIT_ASSERT_EQUAL(1ul, FindWaysVisitor::findWaysByTag(map, "note", "double").size());
    CPPUNIT_ASSERT_EQUAL(1ul, FindWaysVisitor::findWaysByTag(map, "note", "triple").size());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveDuplicateAreaVisitorTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveDuplicateAreaVisitorTest, "current");

}


