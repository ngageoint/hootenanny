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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonSchema.h>

namespace hoot
{

/*
 * A lot of what's not in here gets tested by the poi/poly conflate tests. The rest that is in here
 * isn't currently being triggered by the input data in the conflate tests.
 */
class PoiPolygonSchemaTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(PoiPolygonSchemaTest);
    CPPUNIT_TEST(runSpecificSchoolMatchTest);
    CPPUNIT_TEST(runIsPlaygroundTest);
    CPPUNIT_TEST(runHasSpecificTypeTest);
    CPPUNIT_TEST_SUITE_END();

public:

  PoiPolygonSchemaTest() = default;

  void runSpecificSchoolMatchTest()
  {
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->setTag("amenity", "animal_shelter");
    CPPUNIT_ASSERT(!PoiPolygonSchema::isSpecificSchool(node1));

    node1->setTag("amenity", "school");
    CPPUNIT_ASSERT(!PoiPolygonSchema::isSpecificSchool(node1));

    node1->setTag("name", "North Clay Junior High");
    CPPUNIT_ASSERT(PoiPolygonSchema::isSpecificSchool(node1));
  }

  void runIsPlaygroundTest()
  {
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->setTag("amenity", "animal_shelter");
    CPPUNIT_ASSERT(!PoiPolygonSchema::isPlayground(node1));

    node1->clear();
    node1->setTag("leisure", "playground");
    CPPUNIT_ASSERT(PoiPolygonSchema::isPlayground(node1));
  }

  void runHasSpecificTypeTest()
  {
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->setTag("poi", "yes");
    CPPUNIT_ASSERT(!PoiPolygonSchema::hasSpecificType(node1));

    node1->clear();
    node1->setTag("amenity", "school");
    CPPUNIT_ASSERT(PoiPolygonSchema::hasSpecificType(node1));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonSchemaTest, "quick");

}
