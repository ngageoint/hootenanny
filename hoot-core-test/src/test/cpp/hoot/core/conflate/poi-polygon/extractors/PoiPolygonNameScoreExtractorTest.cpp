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
#include <hoot/core/conflate/poi-polygon/extractors/PoiPolygonNameScoreExtractor.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../../../TestUtils.h"

namespace hoot
{

class PoiPolygonNameScoreExtractorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonNameScoreExtractorTest);
  CPPUNIT_TEST(scoreTest);
  CPPUNIT_TEST(elementHasNameTest);
  CPPUNIT_TEST(elementNameTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void scoreTest()
  {
    PoiPolygonNameScoreExtractor uut;
    OsmMapPtr map(new OsmMap());

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set("name", "blah");
    WayPtr way1(new Way(Status::Unknown2, -1, 15.0));
    way1->getTags().set("name", "blah");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.extract(*map, node1, way1), 0.0);

    WayPtr way2(new Way(Status::Unknown2, -1, 15.0));
    way2->getTags().set("name", "dfghdgf");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.10669, uut.extract(*map, node1, way2), 0.001);
  }

  void elementHasNameTest()
  {
    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set("name", "blah");
    CPPUNIT_ASSERT(PoiPolygonNameScoreExtractor::elementHasName(node1));

    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set("blah", "blah");
    CPPUNIT_ASSERT(!PoiPolygonNameScoreExtractor::elementHasName(node2));
  }

  void elementNameTest()
  {
    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set("name", "blah");
    HOOT_STR_EQUALS("blah", PoiPolygonNameScoreExtractor::getElementName(node1));

    NodePtr node2(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node2->getTags().set("blah", "blah");
    HOOT_STR_EQUALS("", PoiPolygonNameScoreExtractor::getElementName(node2));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonNameScoreExtractorTest, "quick");

}
