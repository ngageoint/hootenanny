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


// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/filters/ParallelWayFilter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/manipulators/DividedHighwayManipulation.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
using namespace hoot;

// Qt
#include <QDebug>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{

class DividedHighwayMergerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(DividedHighwayMergerTest);
    //CPPUNIT_TEST(allManipulationsTest);
    CPPUNIT_TEST(preSplitTest);
    CPPUNIT_TEST(parallelFilterTest);
    CPPUNIT_TEST_SUITE_END();

public:

  void allManipulationsTest()
  {
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DividedHighway.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/UndividedHighway.osm", map);

    Conflator conflator;
    conflator.loadSource(map);
    conflator.conflate();

    boost::shared_ptr<OsmMap> after(new OsmMap(conflator.getBestMap()));
    MapProjector::projectToWgs84(after);

    OsmXmlWriter writer;
    writer.write(after, "test-output/DividedHighwayMergerTest.osm");
    writer.write(map, "test-output/DividedHighwayMergerTestPre.osm");
  }

  void preSplitTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DividedHighwayPreSplit.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/UndividedHighwayPreSplit.osm", map);

    Conflator conflator;
    conflator.loadSource(map);
    conflator.conflate();

    boost::shared_ptr<OsmMap> after(new OsmMap(conflator.getBestMap()));
    MapProjector::projectToWgs84(after);

    OsmXmlWriter writer;
    writer.setIncludeIds(true);
    writer.write(after, "test-output/DividedHighwayMergerPreSplitTest.osm");
  }

  void parallelFilterTest()
  {
    OsmXmlReader reader;

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DividedHighway.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/UndividedHighway.osm", map);

    MapProjector::projectToOrthographic(map);

    long n0  = FindWaysVisitor::findWaysByTag(map, "note", "0")[0];
    long n1  = FindWaysVisitor::findWaysByTag(map, "note", "1")[0];
    long n2  = FindWaysVisitor::findWaysByTag(map, "note", "2")[0];
    long n6  = FindWaysVisitor::findWaysByTag(map, "note", "6")[0];
    long n8  = FindWaysVisitor::findWaysByTag(map, "note", "8")[0];
    long n9  = FindWaysVisitor::findWaysByTag(map, "note", "9")[0];
    long n10 = FindWaysVisitor::findWaysByTag(map, "note", "10")[0];
    long n11 = FindWaysVisitor::findWaysByTag(map, "note", "11")[0];
    long n12 = FindWaysVisitor::findWaysByTag(map, "note", "12")[0];
    long n13 = FindWaysVisitor::findWaysByTag(map, "note", "13")[0];
    long n14 = FindWaysVisitor::findWaysByTag(map, "note", "14")[0];
    long n15 = FindWaysVisitor::findWaysByTag(map, "note", "15")[0];
    long n16 = FindWaysVisitor::findWaysByTag(map, "note", "16")[0];
    long n17 = FindWaysVisitor::findWaysByTag(map, "note", "17")[0];
    long n18 = FindWaysVisitor::findWaysByTag(map, "note", "18")[0];
    long n19 = FindWaysVisitor::findWaysByTag(map, "note", "19")[0];

    ParallelWayFilter f1(map, map->getWay(n0));

    CPPUNIT_ASSERT_EQUAL(f1.isFiltered(map->getWay(n1)), false);
    CPPUNIT_ASSERT_EQUAL(f1.isFiltered(map->getWay(n2)), false);
    CPPUNIT_ASSERT_EQUAL(f1.isFiltered(map->getWay(n6)), true);
    CPPUNIT_ASSERT_EQUAL(f1.isFiltered(map->getWay(n8)), true);

    ParallelWayFilter f2(map, map->getWay(n9));

    CPPUNIT_ASSERT_EQUAL(f2.isFiltered(map->getWay(n10)), false);
    CPPUNIT_ASSERT_EQUAL(f2.isFiltered(map->getWay(n11)), false);
    CPPUNIT_ASSERT_EQUAL(f2.isFiltered(map->getWay(n12)), true);
    CPPUNIT_ASSERT_EQUAL(f2.isFiltered(map->getWay(n13)), true);
    CPPUNIT_ASSERT_EQUAL(f2.isFiltered(map->getWay(n14)), true);

    ParallelWayFilter f3(map, map->getWay(n15));

    CPPUNIT_ASSERT_EQUAL(f3.isFiltered(map->getWay(n16)), false);
    CPPUNIT_ASSERT_EQUAL(f3.isFiltered(map->getWay(n10)), true);

    ParallelWayFilter f17(map, map->getWay(n17));

    CPPUNIT_ASSERT_EQUAL(f17.isFiltered(map->getWay(n18)), false);
    CPPUNIT_ASSERT_EQUAL(f17.isFiltered(map->getWay(n19)), false);

  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(DividedHighwayMergerTest);

}
