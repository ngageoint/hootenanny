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
#include <hoot/core/criterion/AreaCriterion.h>

using namespace geos::geom;

namespace hoot
{

class AreaCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AreaCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runPoiTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    AreaCriterion uut;

    NodePtr node1(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));
    node1->getTags().set("area", "yes");
    CPPUNIT_ASSERT(!uut.isSatisfied(node1));

    WayPtr way1 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way1->getTags().set("area", "yes");
    CPPUNIT_ASSERT(uut.isSatisfied(way1));

    WayPtr way2(new Way(Status::Unknown1, -1, 15.0));
    CPPUNIT_ASSERT(!uut.isSatisfied(way2));

    Tags t;
    t["area"] = "yes";
    CPPUNIT_ASSERT_EQUAL(true, uut.isSatisfied(t, ElementType::Way));
    CPPUNIT_ASSERT_EQUAL(true, uut.isSatisfied(t, ElementType::Relation));
    CPPUNIT_ASSERT_EQUAL(false, uut.isSatisfied(t, ElementType::Node));

    t.clear();
    t["highway"] = "primary";
    CPPUNIT_ASSERT_EQUAL(false, uut.isSatisfied(t, ElementType::Way));

    t.clear();
    t[MetadataTags::BuildingPart()] = "yes";
    CPPUNIT_ASSERT_EQUAL(true, uut.isSatisfied(t, ElementType::Way));
    t[MetadataTags::BuildingPart()] = "no";
    CPPUNIT_ASSERT_EQUAL(false, uut.isSatisfied(t, ElementType::Way));
    t[MetadataTags::BuildingPart()] = "invalid";
    CPPUNIT_ASSERT_EQUAL(false, uut.isSatisfied(t, ElementType::Way));

    t.clear();
    t["leisure"] = "foo";
    CPPUNIT_ASSERT_EQUAL(true, uut.isSatisfied(t, ElementType::Way));
    CPPUNIT_ASSERT_EQUAL(true, uut.isSatisfied(t, ElementType::Relation));

    t.clear();
    t["leisure"] = "track";
    CPPUNIT_ASSERT_EQUAL(false, uut.isSatisfied(t, ElementType::Way));
    CPPUNIT_ASSERT_EQUAL(false, uut.isSatisfied(t, ElementType::Relation));
  }

  void runPoiTest()
  {
    AreaCriterion uut;

    geos::geom::Coordinate c[] = { Coordinate(0.0, 0.0), Coordinate(100.0, 0.0),
                       Coordinate(100.0, 10.0), Coordinate(0.0, 10.0),
                       Coordinate::getNull() };
    Tags tags;
    tags.set("poi", "yes");
    OsmMapPtr map = std::make_shared<OsmMap>();
    ConstWayPtr w1 = TestUtils::createWay(map, c, "", Status::Unknown1, 15.0, tags);

    CPPUNIT_ASSERT(!uut.isSatisfied(w1));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AreaCriterionTest, "quick");

}
