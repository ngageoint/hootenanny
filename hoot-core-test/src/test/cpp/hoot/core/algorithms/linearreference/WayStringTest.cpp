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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/algorithms/linearreference/WayString.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/visitors/CountVisitor.h>

#include "../../TestUtils.h"

namespace hoot
{

class WayStringTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(WayStringTest);
  CPPUNIT_TEST(runBasicTests);
  CPPUNIT_TEST(runRulesTest);
  CPPUNIT_TEST(runCopySimplifiedWayTest);
  CPPUNIT_TEST(runPartialWayTest);
  CPPUNIT_TEST(runPartialWayConnectTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmMapPtr createMap()
  {
    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);

    return map;
  }

  void runBasicTests()
  {
    // Create map
    OsmMapPtr map = createMap();

    // Create ways
    Coordinate w1c[] = { Coordinate(10, 0),
                         Coordinate(20, 0),
                         Coordinate(30, 0),
                         Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c, 1, "");
    w1->setTag("name", "w1");

    Coordinate w2c[] = { Coordinate(40, 0),
                         Coordinate(50, 0),
                         Coordinate(60, 0),
                         Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown1, w2c, 2, "");
    w2->setTag("name", "w2");
    w1->addNode(w2->getFirstNodeId());

    Coordinate w3c[] = { Coordinate(70, 10),
                         Coordinate(70, 20),
                         Coordinate(70, 30),
                         Coordinate::getNull() };
    WayPtr w3 = TestUtils::createWay(map, Status::Unknown1, w3c, 3, "");
    w3->setTag("name", "w3");
    w2->addNode(w3->getFirstNodeId());

    map->addWay(w1);
    map->addWay(w2);
    map->addWay(w3);

    // WayString::append
    WayStringPtr wstring(new WayString());
    WaySubline w1subline(WayLocation(map, w1, 0), WayLocation::createAtEndOfWay(map, w1));
    wstring->append(w1subline);
    WaySubline w2subline(WayLocation(map, w2, 0), WayLocation::createAtEndOfWay(map, w2));
    wstring->append(w2subline);
    WaySubline w3subline(WayLocation(map, w3, 0), WayLocation::createAtEndOfWay(map, w3));
    wstring->append(w3subline);

    // WayString::at
    CPPUNIT_ASSERT(w1subline == wstring->at(0));

    // WayString::back
    CPPUNIT_ASSERT(w3subline == wstring->back());

    // WayString::calculateDistanceOnString
    Meters dist1 = wstring->calculateDistanceOnString(w1subline.getStart());
    CPPUNIT_ASSERT_EQUAL(0.0, dist1);
    Meters dist2 = wstring->calculateDistanceOnString(w1subline.getEnd());
    CPPUNIT_ASSERT_EQUAL(30.0, dist2);
    Meters dist3 = wstring->calculateDistanceOnString(w2subline.getStart());
    CPPUNIT_ASSERT_EQUAL(30.0, dist3);
    Meters dist4 = wstring->calculateDistanceOnString(w2subline.getEnd());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(64.1421356, dist4, 1e-5);
    Meters dist5 = wstring->calculateDistanceOnString(w3subline.getEnd());
    CPPUNIT_ASSERT_EQUAL(w1subline.calculateLength()
                         + w2subline.calculateLength()
                         + w3subline.calculateLength(), dist5);

    // WayString::calculateLength
    Meters dist6 = wstring->calculateLength();
    CPPUNIT_ASSERT_EQUAL(dist5, dist6);

    // WayString::getMaxCircularError
    Meters circErr = wstring->getMaxCircularError();
    CPPUNIT_ASSERT_EQUAL(w3->getCircularError(), circErr);

    // WayString::getSize
    int sz = wstring->getSize();
    CPPUNIT_ASSERT_EQUAL(3, sz);

    // WayString::visitRo
    CountVisitor counter;
    wstring->visitRo(*map, counter);
    CPPUNIT_ASSERT_EQUAL(14, counter.getCount());
  }

  // Intentionally violate the rules to make sure an exception is thrown
  void runRulesTest()
  {
    // Create map
    OsmMapPtr map = createMap();

    // Create ways
    Coordinate w1c[] = { Coordinate(10, 0),
                         Coordinate(20, 0),
                         Coordinate(30, 0),
                         Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c, 1, "");
    w1->setTag("name", "w1");

    Coordinate w2c[] = { Coordinate(40, 0),
                         Coordinate(50, 0),
                         Coordinate(60, 0),
                         Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown1, w2c, 2, "");
    w2->setTag("name", "w2");

    Coordinate w3c[] = { Coordinate(70, 10),
                         Coordinate(70, 20),
                         Coordinate(70, 30),
                         Coordinate::getNull() };
    WayPtr w3 = TestUtils::createWay(map, Status::Unknown1, w3c, 3, "");
    w3->setTag("name", "w3");

    map->addWay(w1);
    map->addWay(w2);
    map->addWay(w3);

    // Try to create a waystring with sublines that come from the same way,
    // but are not consecutive
    bool exThrown = false;
    try
    {
      WayStringPtr wstring(new WayString());
      WaySubline w1subline(WayLocation(map, w1, 0.0), WayLocation(map, w1, 10.0));
      wstring->append(w1subline);
      WaySubline w2subline(WayLocation(map, w1, 11.0), WayLocation(map, w1, 15.0));
      wstring->append(w2subline);
    }
    catch (HootException ex)
    {
      exThrown = true;
      HOOT_STR_EQUALS("All consecutive ways must connect end to start in a WayString.", ex.what());
    }
    CPPUNIT_ASSERT(exThrown);

    // Try to create a waystring with sublines with different ways that do not stop/start
    // at an extreme
    exThrown = false;
    try
    {
      WayStringPtr wstring(new WayString());
      WaySubline w1subline(WayLocation(map, w1, 0.0), WayLocation::createAtEndOfWay(map, w1));
      wstring->append(w1subline);
      WaySubline w2subline(WayLocation(map, w2, 10.0), WayLocation::createAtEndOfWay(map, w2));
      wstring->append(w2subline);
    }
    catch (HootException ex)
    {
      exThrown = true;
      HOOT_STR_EQUALS("If ways are different they must connect at an extreme node.", ex.what());
    }
    CPPUNIT_ASSERT(exThrown);


    // Try to create a waystring with sublines with different ways that are not
    // consecutive
    exThrown = false;
    try
    {
      WayStringPtr wstring(new WayString());
      WaySubline w1subline(WayLocation(map, w1, 0), WayLocation::createAtEndOfWay(map, w1));
      wstring->append(w1subline);
      WaySubline w2subline(WayLocation(map, w2, 0), WayLocation::createAtEndOfWay(map, w2));
      wstring->append(w2subline);
      WaySubline w3subline(WayLocation(map, w3, 0), WayLocation::createAtEndOfWay(map, w3));
      wstring->append(w3subline);
    }
    catch (HootException ex)
    {
      exThrown = true;
      HOOT_STR_EQUALS("Ways must connect at a node in the WayString.", ex.what());
    }
    CPPUNIT_ASSERT(exThrown);
  }

  /**
   * Verify that a way string with one forward and one backward subline will create a proper
   * simplified way.
   */
  void runCopySimplifiedWayTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0),
                         Coordinate(30, 0),
                         Coordinate(70, 0),
                         Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c, 1, "");
    w1->setTag("name", "w1");

    Coordinate w2c[] = { Coordinate(150, 0),
                         Coordinate(100, 0),
                         Coordinate(90, 0),
                         Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown1, w2c, 1, "");
    w2->setTag("name", "w2");

    vector<long> nodes = w2->getNodeIds();
    vector<long> newNodes;
    foreach (long nid, nodes)
    {
      newNodes.push_back(nid);
    }
    newNodes.push_back(w1->getLastNodeId());
    w2->setNodes(newNodes);

    map->addWay(w1);
    map->addWay(w2);

    WayStringPtr ws(new WayString());
    ws->append(WaySubline(WayLocation(map, w1, 0), WayLocation::createAtEndOfWay(map, w1)));
    ws->append(WaySubline(WayLocation::createAtEndOfWay(map, w2), WayLocation(map, w2, 0.0)));

    WayPtr wayCopy = ws->copySimplifiedWayIntoMap(*map, map);
    HOOT_STR_EQUALS("way(-3)\n"
                    "nodes: [6]{-1, -2, -3, -6, -5, -4}\n"
                    "tags: alt_name = w2\n"
                    "name = w1\n"
                    "cached envelope: 0,-1,0,-1\n"
                    "status: unknown1\n"
                    "version: 0\n"
                    "visible: 1",
                    wayCopy);
  }

  /**
   * Verify that a way string with a partial way will create an appropriate simplified way.
   */
  void runPartialWayTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0),
                         Coordinate(30, 0),
                         Coordinate(70, 0),
                         Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c, 1, "");
    w1->setTag("name", "w1");

    map->addWay(w1);

    WayStringPtr ws(new WayString());
    ws->append(WaySubline(WayLocation(map, w1, 0), WayLocation(map, w1, 40)));

    WayPtr wayCopy = ws->copySimplifiedWayIntoMap(*map, map);

    // there was a bug at one point that modifed the original way, make sure it didn't change
    // length.
    HOOT_STR_EQUALS(70, ElementConverter(map).calculateLength(w1));
    HOOT_STR_EQUALS(40, ElementConverter(map).calculateLength(wayCopy));
    HOOT_STR_EQUALS("way(-2)\n"
                    "nodes: [3]{-1, -2, -4}\n"
                    "tags: name = w1\n"
                    "cached envelope: 0,-1,0,-1\n"
                    "status: unknown1\n"
                    "version: 0\n"
                    "visible: 1",
                    wayCopy);
  }

  /**
   * Try connecting with a partial subline, and make sure it works
   */
  void runPartialWayConnectTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0),
                         Coordinate(0, 50),
                         Coordinate::getNull() };
    WayPtr w1 = TestUtils::createWay(map, Status::Unknown1, w1c, 1, "");
    w1->setTag("name", "w1");

    Coordinate w2c[] = { Coordinate(0, 50),
                         Coordinate(0, 100),
                         Coordinate::getNull() };
    WayPtr w2 = TestUtils::createWay(map, Status::Unknown1, w2c, 1, "");
    w2->setTag("name", "w2");
    w1->addNode(w2->getFirstNodeId());

    map->addWay(w1);
    map->addWay(w2);

    WayStringPtr ws(new WayString());
    WaySubline w1subline(WayLocation(map, w1, 0), WayLocation::createAtEndOfWay(map, w1));
    WaySubline w2subline(WayLocation(map, w2, 0), WayLocation(map, w2, 1, 0.75));

    ws->append(w1subline);
    ws->append(w2subline);

    WayPtr wayCopy = ws->copySimplifiedWayIntoMap(*map, map);

    HOOT_STR_EQUALS("way(-3)\n"
                    "nodes: [4]{-1, -2, -3, -4}\n"
                    "tags: alt_name = w2\n"
                    "name = w1\n"
                    "cached envelope: 0,-1,0,-1\n"
                    "status: unknown1\n"
                    "version: 0\n"
                    "visible: 1",
                    wayCopy);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayStringTest, "quick");

}
