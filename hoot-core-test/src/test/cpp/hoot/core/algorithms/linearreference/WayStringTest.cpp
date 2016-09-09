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

#include "../../TestUtils.h"

namespace hoot
{

class WayStringTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(WayStringTest);
  CPPUNIT_TEST(runCopySimplifiedWayTest);
  CPPUNIT_TEST(runPartialWayTest);
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
    ws->append(WaySubline(WayLocation::createAtEndOfWay(map, w2), WayLocation(map, w2, 0)));

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
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayStringTest, "quick");

}

