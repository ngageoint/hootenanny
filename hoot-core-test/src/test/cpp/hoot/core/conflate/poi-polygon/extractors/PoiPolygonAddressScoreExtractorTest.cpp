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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/conflate/poi-polygon/extractors/PoiPolygonAddressScoreExtractor.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../../../TestUtils.h"

namespace hoot
{

class PoiPolygonAddressScoreExtractorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonAddressScoreExtractorTest);
  CPPUNIT_TEST(runTagTest);
  CPPUNIT_TEST(runCombinedTagTest);
  CPPUNIT_TEST(runWayTest);
  CPPUNIT_TEST(runRelationTest);
  CPPUNIT_TEST(runExactMatchingFalseTest);
  CPPUNIT_TEST(runRangeTest);
  CPPUNIT_TEST(runAltFormatTest);
  CPPUNIT_TEST(runSubLetterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTagTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setExactAddressMatching(true);
    OsmMapPtr map;

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "123 main street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    WayPtr way2(new Way(Status::Unknown2, -1, 15.0));
    way2->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "567 first street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.0);
  }

  void runCombinedTagTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setExactAddressMatching(true);
    OsmMapPtr map;

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    way1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    WayPtr way2(new Way(Status::Unknown2, -1, 15.0));
    way2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "567");
    way2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "first street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.0);
  }

  void runExactMatchingFalseTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setExactAddressMatching(false);
    OsmMapPtr map;

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME, "567 main street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.667, uut.extract(*map, node1, way1), 0.01);
  }

  void runRangeTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setExactAddressMatching(true);
    OsmMapPtr map;

//    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
//    node1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123-125");
//    node1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
//    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
//    way1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "124");
//    way1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123-125");
    node2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    WayPtr way2(new Way(Status::Unknown2, -1, 15.0));
    way2->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    way2->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node2, way2), 0.0);

    NodePtr node3(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node3->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123-125");
    node3->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    WayPtr way3(new Way(Status::Unknown2, -1, 15.0));
    way3->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "567");
    way3->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "first street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node3, way3), 0.0);
  }

  void runAltFormatTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setExactAddressMatching(true);
    OsmMapPtr map;

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(
      PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2, "Main Street 123 20121 mytown");
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2, "main street 123");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set(
      PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2, "Main Street 123 20121 mytown");
    WayPtr way2(new Way(Status::Unknown2, -1, 15.0));
    way2->getTags().set(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME_2, "first street 567");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node2, way2), 0.0);
  }

  void runSubLetterTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    uut.setExactAddressMatching(true);
    OsmMapPtr map;

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123");
    node1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "Main Street");
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set(PoiPolygonAddressScoreExtractor::HOUSE_NUMBER_TAG_NAME, "123a");
    way1->getTags().set(PoiPolygonAddressScoreExtractor::STREET_TAG_NAME, "main street");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
  }

  void runWayTest()
  {

  }

  void runRelationTest()
  {

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonAddressScoreExtractorTest, "quick");

}
