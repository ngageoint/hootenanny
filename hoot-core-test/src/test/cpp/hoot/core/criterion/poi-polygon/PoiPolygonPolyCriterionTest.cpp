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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>

using namespace geos::geom;

namespace hoot
{

class PoiPolygonPolyCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonPolyCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    PoiPolygonPolyCriterion uut;

    //  way only
    NodePtr node1 = std::make_shared<Node>(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0);
    CPPUNIT_ASSERT(!uut.isSatisfied(node1));

    //  type specifically excluded outside of schema
    WayPtr way1 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way1->getTags().set(MetadataTags::Area(), "yes");
    way1->getTags().set("landuse", "grass");
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));

    //  is area, but not a poi or building type
    WayPtr way2 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way2->getTags().set(MetadataTags::Area(), "yes");
    way2->getTags().set("email", "blah");
    CPPUNIT_ASSERT(!uut.isSatisfied(way2));

    //  is area and a poi or building type
    WayPtr way3 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way3->getTags().set(MetadataTags::Area(), "yes");
    way3->getTags().set("amenity", "school");
    CPPUNIT_ASSERT(uut.isSatisfied(way3));

    //  is building and a poi or building type
    WayPtr way4 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way4->getTags().set(MetadataTags::Building(), "yes");
    way4->getTags().set("amenity", "school");
    CPPUNIT_ASSERT(uut.isSatisfied(way4));

    //  is area and has name
    WayPtr way5 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way5->getTags().set(MetadataTags::Area(), "yes");
    way5->getTags().set("name", "blah");
    way5->getTags().set("email", "blah");
    CPPUNIT_ASSERT(uut.isSatisfied(way5));

    //  is building and has name
    WayPtr way6 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way6->getTags().set(MetadataTags::Building(), "yes");
    way6->getTags().set("name", "blah");
    way6->getTags().set("email", "blah");
    CPPUNIT_ASSERT(uut.isSatisfied(way6));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonPolyCriterionTest, "quick");

}
