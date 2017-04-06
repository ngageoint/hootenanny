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

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/QuantileAggregator.h>
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/MinAggregator.h>
#include <hoot/core/conflate/polygon/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QByteArray>

// Tgs
#include <tgs/StreamUtils.h>

#include "../../../TestUtils.h"

namespace hoot
{

class EdgeDistanceExtractorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(EdgeDistanceExtractorTest);
  CPPUNIT_TEST(runBuildingsTest);
  CPPUNIT_TEST(runRoadsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::resetEnvironment();
  }

  void runBuildingsTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyBuildingsTestB.osm", map);

    MapProjector::projectToPlanar(map);

    vector<long> r1 = FindWaysVisitor::findWaysByTag(map, MetadataTags::Ref1(), "Target");
    vector<long> r2 = FindWaysVisitor::findWaysByTag(map, "name", "Target Grocery");

    boost::shared_ptr<const Way> w1 = map->getWay(r1[0]);
    boost::shared_ptr<const Way> w2 = map->getWay(r2[0]);

    EdgeDistanceExtractor uut(new MeanAggregator(), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.9833, uut.distance(*map, w1, w2), 0.01);

    EdgeDistanceExtractor uut2(new RmseAggregator(), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(15.5733, uut2.distance(*map, w1, w2), 0.01);

    EdgeDistanceExtractor uut3(new MinAggregator(), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0227, uut3.distance(*map, w1, w2), 0.01);

    EdgeDistanceExtractor uut4(new QuantileAggregator(0.5), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5029, uut4.distance(*map, w1, w2), 0.01);

    EdgeDistanceExtractor uut5(new QuantileAggregator(0.1), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3982, uut5.distance(*map, w1, w2), 0.01);
  }

  void runRoadsTest()
  {
    OsmXmlReader reader;

    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/extractor/EdgeDistanceExtractor/ToyTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/extractor/EdgeDistanceExtractor/ToyTestB.osm", map);

    MapProjector::projectToPlanar(map);

    EdgeDistanceExtractor uut2(new RmseAggregator());

    vector<long> r1 = FindWaysVisitor::findWaysByTag(map, "note", "1");
    vector<long> r2 = FindWaysVisitor::findWaysByTag(map, "note", "b");

    boost::shared_ptr<const Way> w1 = map->getWay(r1[0]);
    boost::shared_ptr<const Way> w2 = map->getWay(r2[0]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.50153, uut2.distance(*map, w1, w2), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.978273, uut2.extract(*map, w1, w2), 0.01);

    vector<long> r3 = FindWaysVisitor::findWaysByTag(map, "note", "25");
    vector<long> r4 = FindWaysVisitor::findWaysByTag(map, "note", "z");

    boost::shared_ptr<const Way> w3 = map->getWay(r3[0]);
    boost::shared_ptr<const Way> w4 = map->getWay(r4[0]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, uut2.distance(*map, w3, w4), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut2.extract(*map, w3, w4), 0.01);
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeDistanceExtractorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeDistanceExtractorTest, "quick");

}
