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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/QuantileAggregator.h>
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/MinAggregator.h>
#include <hoot/core/algorithms/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Tgs
#include <tgs/StreamUtils.h>

using namespace std;

namespace hoot
{

class EdgeDistanceExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(EdgeDistanceExtractorTest);
  CPPUNIT_TEST(runBuildingsTest);
  CPPUNIT_TEST(runRoadsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  EdgeDistanceExtractorTest()
    : HootTestFixture("test-files/algorithms/extractors/EdgeDistanceExtractor/",
                      UNUSED_PATH)
  {
    setResetType(ResetAll);
  }

  void runBuildingsTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyBuildingsTestB.osm", map);

    MapProjector::projectToPlanar(map);

    vector<long> r1 =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref1(), "Target");
    vector<long> r2 =
      ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "name", "Target Grocery");

    ConstWayPtr w1 = map->getWay(r1[0]);
    ConstWayPtr w2 = map->getWay(r2[0]);

    EdgeDistanceExtractor uut(ValueAggregatorPtr(new MeanAggregator()), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.9833, uut.distance(*map, w1, w2), 0.01);

    EdgeDistanceExtractor uut2(ValueAggregatorPtr(new RmseAggregator()), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(15.5733, uut2.distance(*map, w1, w2), 0.01);

    EdgeDistanceExtractor uut3(ValueAggregatorPtr(new MinAggregator()), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0227, uut3.distance(*map, w1, w2), 0.01);

    EdgeDistanceExtractor uut4(ValueAggregatorPtr(new QuantileAggregator(0.5)), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5029, uut4.distance(*map, w1, w2), 0.01);

    EdgeDistanceExtractor uut5(ValueAggregatorPtr(new QuantileAggregator(0.1)), 5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3982, uut5.distance(*map, w1, w2), 0.01);
  }

  void runRoadsTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "ToyTestB.osm", map);

    MapProjector::projectToPlanar(map);

    EdgeDistanceExtractor uut2(ValueAggregatorPtr(new RmseAggregator()));

    vector<long> r1 = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "1");
    vector<long> r2 = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "b");

    ConstWayPtr w1 = map->getWay(r1[0]);
    ConstWayPtr w2 = map->getWay(r2[0]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.50153, uut2.distance(*map, w1, w2), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.978273, uut2.extract(*map, w1, w2), 0.01);

    vector<long> r3 = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "25");
    vector<long> r4 = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "z");

    ConstWayPtr w3 = map->getWay(r3[0]);
    ConstWayPtr w4 = map->getWay(r4[0]);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, uut2.distance(*map, w3, w4), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, uut2.extract(*map, w3, w4), 0.01);
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeDistanceExtractorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EdgeDistanceExtractorTest, "quick");

}
