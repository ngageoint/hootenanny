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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/visitors/OneWayRoadStandardizer.h>

namespace hoot
{

class OneWayRoadStandardizerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OneWayRoadStandardizerTest);
  CPPUNIT_TEST(runWayTest);
  CPPUNIT_TEST(runRelationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OneWayRoadStandardizerTest()
  {
    setResetType(ResetBasic);
  }

  void runWayTest()
  {
    {
      OsmMapPtr map(new OsmMap());
      geos::geom::Coordinate coords[] =
      { geos::geom::Coordinate(0, 0), geos::geom::Coordinate(10, 10),
        geos::geom::Coordinate::getNull() };
      WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords);
      way->getTags()["oneway"] = "-1";
      way->getTags()["highway"] = "residential";

      OneWayRoadStandardizer uut;
      uut.setOsmMap(map.get());
      map->visitRw(uut);

      // check that the oneway tag was updated
      CPPUNIT_ASSERT(way->getTags()["oneway"] == "yes");
      // check to see if the way got reversed
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getX() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getY() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getX() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getY() == 0);
    }

    {
      OsmMapPtr map(new OsmMap());
      geos::geom::Coordinate coords[] =
      { geos::geom::Coordinate(0, 0), geos::geom::Coordinate(10, 10),
        geos::geom::Coordinate::getNull() };
      WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords);
      way->getTags()["oneway"] = "reverse";
      way->getTags()["highway"] = "residential";

      OneWayRoadStandardizer uut;
      uut.setOsmMap(map.get());
      map->visitRw(uut);

      // check that the oneway tag was updated
      CPPUNIT_ASSERT(way->getTags()["oneway"] == "yes");
      // check to see if the way got reversed
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getX() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getY() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getX() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getY() == 0);
    }

    {
      OsmMapPtr map(new OsmMap());
      geos::geom::Coordinate coords[] =
      { geos::geom::Coordinate(0, 0), geos::geom::Coordinate(10, 10),
        geos::geom::Coordinate::getNull() };
      WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords);
      // This way isn't reversed, so nothing should happen to it.
      way->getTags()["oneway"] = "yes";
      way->getTags()["highway"] = "residential";

      OneWayRoadStandardizer uut;
      uut.setOsmMap(map.get());
      map->visitRw(uut);

      // check that the oneway tag remains unchanged
      CPPUNIT_ASSERT(way->getTags()["oneway"] == "yes");
      // check to see make sure the way didn't get reversed
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getX() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getY() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getX() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getY() == 10);
    }

    {
      OsmMapPtr map(new OsmMap());
      geos::geom::Coordinate coords[] =
      { geos::geom::Coordinate(0, 0), geos::geom::Coordinate(10, 10),
        geos::geom::Coordinate::getNull() };
      WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords);
      // This way isn't a road, so nothing should happen to it.
      way->getTags()["oneway"] = "reverse";
      way->getTags()["building"] = "yes";

      OneWayRoadStandardizer uut;
      uut.setOsmMap(map.get());
      map->visitRw(uut);

      // check that the oneway tag remains unchanged
      CPPUNIT_ASSERT(way->getTags()["oneway"] == "reverse");
      // check to see make sure the way didn't get reversed
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getX() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getY() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getX() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getY() == 10);
    }
  }

  void runRelationTest()
  {
    {
      OsmMapPtr map(new OsmMap());
      geos::geom::Coordinate coords[] =
      { geos::geom::Coordinate(0, 0), geos::geom::Coordinate(10, 10),
        geos::geom::Coordinate::getNull() };
      // The member way doesn't need any special tags in order to be reversed.
      WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords);
      QList<ElementPtr> elements;
      elements.append(way);
      RelationPtr relation = TestUtils::createRelation(map, elements);
      relation->getTags()["oneway"] = "-1";
      relation->getTags()["highway"] = "residential";

      OneWayRoadStandardizer uut;
      uut.setOsmMap(map.get());
      map->visitRw(uut);

      // check that the oneway tag was updated
      CPPUNIT_ASSERT(relation->getTags()["oneway"] == "yes");
      // way member's tags shouldn't have been modified
      CPPUNIT_ASSERT_EQUAL(0, way->getTagCount());
      // check to see if the way member got reversed
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getX() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getY() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getX() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getY() == 0);
    }

    {
      OsmMapPtr map(new OsmMap());
      geos::geom::Coordinate coords[] =
      { geos::geom::Coordinate(0, 0), geos::geom::Coordinate(10, 10),
        geos::geom::Coordinate::getNull() };
      // The member way doesn't need any special tags in order to be reversed.
      WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords);
      QList<ElementPtr> elements;
      elements.append(way);
      RelationPtr relation = TestUtils::createRelation(map, elements);
      relation->getTags()["oneway"] = "reverse";
      relation->getTags()["highway"] = "residential";

      OneWayRoadStandardizer uut;
      uut.setOsmMap(map.get());
      map->visitRw(uut);

      // check that the oneway tag was updated
      CPPUNIT_ASSERT(relation->getTags()["oneway"] == "yes");
      // way member's tags shouldn't have been modified
      CPPUNIT_ASSERT_EQUAL(0, way->getTagCount());
      // check to see if the way member got reversed
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getX() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getY() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getX() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getY() == 0);
    }

    {
      OsmMapPtr map(new OsmMap());
      geos::geom::Coordinate coords[] =
      { geos::geom::Coordinate(0, 0), geos::geom::Coordinate(10, 10),
        geos::geom::Coordinate::getNull() };
      WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords);
      QList<ElementPtr> elements;
      elements.append(way);
      // This relation isn't reversed, so nothing should happen to it.
      RelationPtr relation = TestUtils::createRelation(map, elements);
      relation->getTags()["oneway"] = "yes";
      relation->getTags()["highway"] = "residential";

      OneWayRoadStandardizer uut;
      uut.setOsmMap(map.get());
      map->visitRw(uut);

      // check that the oneway tag remains unchanged
      CPPUNIT_ASSERT(relation->getTags()["oneway"] == "yes");
      // way member's tags shouldn't have been modified
      CPPUNIT_ASSERT_EQUAL(0, way->getTagCount());
      // check to see make sure the way member didn't get reversed
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getX() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getY() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getX() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getY() == 10);
    }

    {
      OsmMapPtr map(new OsmMap());
      geos::geom::Coordinate coords[] =
      { geos::geom::Coordinate(0, 0), geos::geom::Coordinate(10, 10),
        geos::geom::Coordinate::getNull() };
      WayPtr way = TestUtils::createWay(map, Status::Unknown1, coords);
      QList<ElementPtr> elements;
      elements.append(way);
      RelationPtr relation = TestUtils::createRelation(map, elements);
      // This relation isn't a road, so nothing should happen to it.
      relation->getTags()["oneway"] = "reverse";
      relation->getTags()["building"] = "yes";

      OneWayRoadStandardizer uut;
      uut.setOsmMap(map.get());
      map->visitRw(uut);

      // check that the oneway tag remains unchanged
      CPPUNIT_ASSERT(relation->getTags()["oneway"] == "reverse");
      // way member's tags shouldn't have been modified
      CPPUNIT_ASSERT_EQUAL(0, way->getTagCount());
      // check to see make sure the way member didn't get reversed
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getX() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getFirstNodeId())->getY() == 0);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getX() == 10);
      CPPUNIT_ASSERT(map->getNode(way->getLastNodeId())->getY() == 10);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OneWayRoadStandardizerTest, "quick");

}


