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
  CPPUNIT_TEST(runNodeTest);
  CPPUNIT_TEST(runWayTest);
  CPPUNIT_TEST(runRelationTest);
  CPPUNIT_TEST(runExactMatchingFalseTest);
  CPPUNIT_TEST(runRangeTest);
  CPPUNIT_TEST(runAltFormatTest);
  CPPUNIT_TEST(runSubLetterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runNodeTest()
  {
    PoiPolygonAddressScoreExtractor uut;
    OsmMapPtr map;

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set("name", "blah");
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set("name", "blah");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);


  }

  void runWayTest()
  {

  }

  void runRelationTest()
  {

  }

  void runExactMatchingFalseTest()
  {

  }

  void runRangeTest()
  {

  }

  void runAltFormatTest()
  {

  }

  void runSubLetterTest()
  {

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonAddressScoreExtractorTest, "quick");

}
