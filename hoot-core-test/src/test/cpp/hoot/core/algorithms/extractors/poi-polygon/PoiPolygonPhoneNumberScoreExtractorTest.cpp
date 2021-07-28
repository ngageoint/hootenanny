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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */


// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonPhoneNumberScoreExtractor.h>
#include <hoot/core/util/Log.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class PoiPolygonPhoneNumberScoreExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonPhoneNumberScoreExtractorTest);
  // TODO: re-enable
//  CPPUNIT_TEST(basicTest);
//  CPPUNIT_TEST(regionValidationTest);
//  CPPUNIT_TEST(invalidNumberTest);
//  CPPUNIT_TEST(tagKeyTest);
//  CPPUNIT_TEST(invalidRegionCodeTest);
//  CPPUNIT_TEST(additionalTagKeysTest);
//  CPPUNIT_TEST(findInTextTest);
  CPPUNIT_TEST_SUITE_END();

public:

  PoiPolygonPhoneNumberScoreExtractorTest() : HootTestFixture(UNUSED_PATH, UNUSED_PATH)
  {
    setResetType(ResetAll);
  }

//  void basicTest()
//  {
//    PoiPolygonPhoneNumberScoreExtractor uut;
//    OsmMapPtr map = std::make_shared<OsmMap>();

//    NodePtr node1 =
//      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
//    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);

//    node1->getTags().set("phone", "(123) 456 7890");
//    way1->getTags().set("phone", "123 456 7890");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
//  }

//  void regionValidationTest()
//  {
//    PoiPolygonPhoneNumberScoreExtractor uut;
//    OsmMapPtr map = std::make_shared<OsmMap>();

//    NodePtr node1 =
//      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
//    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);

//    node1->getTags().set("phone", "(123) 456 7890");
//    way1->getTags().set("phone", "123 456 7890");

//    uut._phoneNumberParser.setRegionCode("US");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

//    uut._phoneNumberParser.setRegionCode("CH");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);

//    node1->getTags().set("phone", "+41 44 668 1800");
//    way1->getTags().set("phone", "044 668 1800");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
//  }

//  void invalidNumberTest()
//  {
//    PoiPolygonPhoneNumberScoreExtractor uut;
//    OsmMapPtr map = std::make_shared<OsmMap>();

//    NodePtr node1 =
//      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
//    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);

//    node1->getTags().set("phone", "blah");
//    way1->getTags().set("phone", "123 456 7890");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
//  }

//  void tagKeyTest()
//  {
//    PoiPolygonPhoneNumberScoreExtractor uut;
//    OsmMapPtr map = std::make_shared<OsmMap>();

//    NodePtr node1 =
//      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
//    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);

//    node1->getTags().set("source:phone", "(123) 456 7890");
//    way1->getTags().set("phone", "123 456 7890");
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);
//  }

//  void invalidRegionCodeTest()
//  {
//    PoiPolygonPhoneNumberScoreExtractor uut;
//    QString exceptionMsg("");
//    try
//    {
//      uut._phoneNumberParser.setRegionCode("blah");
//    }
//    catch (const HootException& e)
//    {
//      exceptionMsg = e.what();
//    }
//    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid phone number region code"));
//  }

//  void additionalTagKeysTest()
//  {
//    PoiPolygonPhoneNumberScoreExtractor uut;
//    OsmMapPtr map = std::make_shared<OsmMap>();

//    NodePtr node1 =
//      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
//    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);

//    node1->getTags().set("note", "(123) 456 7890");
//    way1->getTags().set("phone", "123 456 7890");

//    uut._phoneNumberParser.setAdditionalTagKeys(QStringList("note"));
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

//    uut._phoneNumberParser.setAdditionalTagKeys(QStringList());
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
//  }

//  void findInTextTest()
//  {
//    PoiPolygonPhoneNumberScoreExtractor uut;
//    OsmMapPtr map = std::make_shared<OsmMap>();

//    NodePtr node1 =
//      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
//    WayPtr way1 = std::make_shared<Way>(Status::Unknown2, -1, 15.0);

//    node1->getTags().set(
//      "phone",
//      "this is some text with a phone number: (123) 456 7890; and here's another one: 123 456 7890");
//    way1->getTags().set("phone", "123 456 7890");

//    uut._phoneNumberParser.setRegionCode("US");
//    uut._phoneNumberParser.setSearchInText(true);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

//    uut._phoneNumberParser.setSearchInText(false);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.extract(*map, node1, way1), 0.0);
//  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonPhoneNumberScoreExtractorTest, "quick");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonPhoneNumberScoreExtractorTest, "serial");

}
