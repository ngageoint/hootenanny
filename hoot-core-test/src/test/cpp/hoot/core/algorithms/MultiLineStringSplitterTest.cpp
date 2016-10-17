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
// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Coordinate.h>
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/MultiLineStringSplitter.h>
#include <hoot/core/algorithms/linearreference/MultiLineStringLocation.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// Qt
#include <QDir>
#include <QString>

// Standard
#include <string>

// TGS
#include <tgs/System/Time.h>

#include "../TestUtils.h"

namespace hoot
{

class MultiLineStringSplitterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MultiLineStringSplitterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runMultiLineStringLocationSplitTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmMapPtr createMap()
  {
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    auto_ptr<OGREnvelope> env(GeometryUtils::toOGREnvelope(Envelope(0, 1, 0, 1)));
    MapProjector::projectToPlanar(map, *env);

    return map;
  }

  WayPtr createWay(OsmMapPtr map, Coordinate* c, Status s = Status::Unknown1)
  {
    WayPtr result((new Way(s, map->createNextWayId(), 15.0)));

    for (int i = 0; c[i].isNull() == false; i++)
    {
      NodePtr n(new Node(s, map->createNextNodeId(), c[i], 15.0));
      map->addNode(n);
      result->addNode(n->getId());
    }

    map->addWay(result);

    return result;
  }

  /**
   * Creates two ways like this:
   *
   * x and o signifiy Unknown1 and Unknown2, respectively.
   *
   * o--x---x--o
   *
   * These two ways should simply be merged w/o any scraps, etc.
   */
  void runTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr w1 = createWay(map, w1c, Status::Unknown1);

    WaySublineCollection string;
    string.addSubline(WaySubline(WayLocation(map, w1, 30), WayLocation(map, w1, 70)));
    ElementPtr match;
    ElementPtr scraps;
    vector<bool> reverse(1, false);
    MultiLineStringSplitter().split(map, string, reverse, match, scraps);

    HOOT_STR_EQUALS("Way:-2", match->getElementId());
    HOOT_STR_EQUALS("Relation:-1", scraps->getElementId());
    WayPtr m1 = map->getWay(match->getId());
    WayPtr s1 = map->getWay(-3);
    WayPtr s2 = map->getWay(-4);
    HOOT_STR_EQUALS(m1->getNodeId(0), s1->getLastNodeId());
    HOOT_STR_EQUALS(m1->getNodeId(1), s2->getNodeId(0));
  }

  void runMultiLineStringLocationSplitTest()
  {
    OsmMapPtr map = createMap();

    Coordinate w1c[] = { Coordinate(0, 0), Coordinate(100, 0), Coordinate::getNull() };
    WayPtr way1 = createWay(map, w1c, Status::Unknown1);
    //LOG_VAR(way1->getElementId());
    //LOG_VAR(way1->getNodeIds());
    RelationPtr relation(new Relation(way1->getStatus(), map->createNextRelationId(),
      way1->getCircularError(), Relation::MULTILINESTRING));
    //LOG_VAR(relation->getElementId());
    relation->addElement("", way1->getElementId());
    map->addElement(relation);

    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getNodeMap().size());
    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getWays().size());

    MultiLineStringLocation multiLineStringLocation(map, relation, 0,
      WayLocation(map, way1, 0, 0.5));

    ElementPtr match;
    MultiLineStringSplitter().split(map, multiLineStringLocation, match);
    //LOG_VAR(match->getElementId());

    CPPUNIT_ASSERT_EQUAL((size_t)3, map->getNodeMap().size());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWays().size());

    HOOT_STR_EQUALS("Way:-2", match->getElementId());
    WayPtr matchWay = map->getWay(match->getId());
    //LOG_VAR(matchWay->getNodeIds());
    const std::vector<long>& matchWayNodeIds = matchWay->getNodeIds();
    CPPUNIT_ASSERT_EQUAL(-1L, matchWayNodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-3L, matchWayNodeIds.at(1));
    way1 = map->getWay(-1);
    const std::vector<long>& way1NodeIds = way1->getNodeIds();
    //LOG_VAR(way1->getNodeIds());
    CPPUNIT_ASSERT_EQUAL(-1L, way1NodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-2L, way1NodeIds.at(1));
    //HOOT_STR_EQUALS(m1->getNodeId(0), s1->getLastNodeId()););
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiLineStringSplitterTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiLineStringSplitterTest, "quick");

}
