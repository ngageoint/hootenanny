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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/WayLengthCriterion.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class WayLengthCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayLengthCriterionTest);
  CPPUNIT_TEST(runPositiveTest);
  CPPUNIT_TEST(runNegativeTest);
  CPPUNIT_TEST(runBypassTest);
  CPPUNIT_TEST(runMissingMapTest);
  CPPUNIT_TEST(runMapNotInPlanarTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runPositiveTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    MapProjector::projectToPlanar(map);
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 10.0), 15.0);
    map->addNode(node2);
    WayPtr way = std::make_shared<Way>(Status::Unknown1, 1);
    way->addNode(node1->getId());
    way->addNode(node2->getId());
    map->addWay(way);

    std::shared_ptr<WayLengthCriterion> uut =
      std::make_shared<WayLengthCriterion>(1.0, NumericComparisonType::GreaterThanOrEqualTo, map);
    CPPUNIT_ASSERT(uut->isSatisfied(way));
  }

  void runNegativeTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    MapProjector::projectToPlanar(map);
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    map->addNode(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 10.0), 15.0);
    map->addNode(node2);
    WayPtr way = std::make_shared<Way>(Status::Unknown1, 1);
    way->addNode(node1->getId());
    way->addNode(node2->getId());
    map->addWay(way);

    std::shared_ptr<WayLengthCriterion> uut =
      std::make_shared<WayLengthCriterion>(0.0, NumericComparisonType::EqualTo, map);
    CPPUNIT_ASSERT(!uut->isSatisfied(way));
  }

  void runBypassTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    MapProjector::projectToPlanar(map);
    WayPtr way = std::make_shared<Way>(Status::Unknown1, 1);
    map->addWay(way);

    // -1.0 for the length bypasses the crit; any way will work here
    std::shared_ptr<WayLengthCriterion> uut =
      std::make_shared<WayLengthCriterion>(-1.0, NumericComparisonType::EqualTo, map);
    CPPUNIT_ASSERT(uut->isSatisfied(way));
  }

  void runMissingMapTest()
  {
    WayPtr way = std::make_shared<Way>(Status::Unknown1, 1);

    std::shared_ptr<WayLengthCriterion> uut =
      std::make_shared<WayLengthCriterion>(1.0, NumericComparisonType::EqualTo, nullptr);
    QString exceptionMsg;
    try
    {
      uut->isSatisfied(way);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("WayLengthCriterion requires a map.").toStdString(), exceptionMsg.toStdString());
  }

  void runMapNotInPlanarTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>(); // skip projecting map to planar
    WayPtr way = std::make_shared<Way>(Status::Unknown1, 1);
    map->addWay(way);

    std::shared_ptr<WayLengthCriterion> uut =
      std::make_shared<WayLengthCriterion>(1.0, NumericComparisonType::EqualTo, map);
    QString exceptionMsg;
    try
    {
      uut->isSatisfied(way);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Map must be in planar coordinate system.").toStdString(),
      exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayLengthCriterionTest, "quick");

}
