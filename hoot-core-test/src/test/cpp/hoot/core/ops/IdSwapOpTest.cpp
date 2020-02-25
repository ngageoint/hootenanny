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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/IdSwap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/IdSwapOp.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

namespace hoot
{

class IdSwapOpTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(IdSwapOpTest);
  CPPUNIT_TEST(runQuickTest);
  CPPUNIT_TEST_SUITE_END();

public:

  IdSwapOpTest()
    : HootTestFixture("test-files/ops/IdSwapOp/",
                      "test-output/ops/IdSwapOp/")
  {
    setResetType(ResetBasic);
  }

  void runQuickTest()
  {
    long pid1 = 1111;
    long pid2 = 2222;

    OsmMapPtr map(new OsmMap());
    geos::geom::Coordinate w1[] =
    { geos::geom::Coordinate(0.0, 0.0), geos::geom::Coordinate(1.0, 10.0),
      geos::geom::Coordinate(-1.0, 20.0), geos::geom::Coordinate(0.0, 30.0),
      geos::geom::Coordinate::getNull() };

    WayPtr way1 = TestUtils::createWay(map, w1);
    way1->setTag("find", "way1");
    way1->setTag("test:param", way1->getElementId().toString());
    way1->setPid(pid1);

    geos::geom::Coordinate w2[] =
    { geos::geom::Coordinate(0.0, 0.0), geos::geom::Coordinate(10.0, 1.0),
      geos::geom::Coordinate(20.0, -1.0), geos::geom::Coordinate(30.0, 0.0),
      geos::geom::Coordinate::getNull() };
    WayPtr way2 = TestUtils::createWay(map, w2);
    way2->setTag("find", "way2");
    way2->setTag("test:param", way2->getElementId().toString());
    way2->setPid(pid2);

    //  Add IDs to swap
    map->getIdSwap()->add(way1->getElementId(), way2->getElementId());

    //  Apply the ID swap operation
    IdSwapOp op;
    op.apply(map);

    WayPtr find1 =
      map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "find", "way1")[0]);
    WayPtr find2 =
      map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "find", "way2")[0]);

    CPPUNIT_ASSERT_EQUAL(1L, op.getNumFeaturesAffected());

    HOOT_STR_EQUALS(find1->getTags().get("test:param"), find2->getElementId().toString());
    HOOT_STR_EQUALS(find2->getTags().get("test:param"), find1->getElementId().toString());

    CPPUNIT_ASSERT_EQUAL(pid2, find1->getPid());
    CPPUNIT_ASSERT_EQUAL(pid1, find2->getPid());
  }


};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IdSwapOpTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IdSwapOpTest, "current");

}
