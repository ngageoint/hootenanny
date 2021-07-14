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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/splitter/MultiLineStringSplitter.h>
#include <hoot/core/algorithms/linearreference/MultiLineStringLocation.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>

// TGS
#include <tgs/System/Time.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class MultiLineStringSplitterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiLineStringSplitterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runMultiLineStringLocationSplitTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MultiLineStringSplitterTest()
  {
    setResetType(ResetBasic);
  }

  OsmMapPtr createMap()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    std::shared_ptr<OGREnvelope> env(GeometryUtils::toOGREnvelope(Envelope(0, 1, 0, 1)));
    MapProjector::projectToPlanar(map, *env);

    return map;
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
    WayPtr w1 = TestUtils::createWay(map, w1c);

    WaySublineCollection string;
    string.addSubline(WaySubline(WayLocation(map, w1, 30), WayLocation(map, w1, 70)));
    ElementPtr match;
    ElementPtr scraps;
    vector<bool> reverse(1, false);
    MultiLineStringSplitter().split(map, string, reverse, match, scraps);

    HOOT_STR_EQUALS("Way(-2)", match->getElementId());
    HOOT_STR_EQUALS("Relation(-1)", scraps->getElementId());
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
    WayPtr way1 = TestUtils::createWay(map, w1c);
    RelationPtr relation =
      std::make_shared<Relation>(
      way1->getStatus(), map->createNextRelationId(), way1->getCircularError(),
      MetadataTags::RelationMultilineString());
    relation->addElement("", way1->getElementId());
    map->addElement(relation);

    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((size_t)1, map->getWays().size());

    MultiLineStringLocation multiLineStringLocation(map, relation, 0,
      WayLocation(map, way1, 0, 0.5));

    ElementPtr match;
    MultiLineStringSplitter().split(map, multiLineStringLocation, match);

    CPPUNIT_ASSERT_EQUAL((size_t)3, map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL((size_t)2, map->getWays().size());

    HOOT_STR_EQUALS("Way(-2)", match->getElementId());
    WayPtr matchWay = map->getWay(match->getId());
    const std::vector<long>& matchWayNodeIds = matchWay->getNodeIds();
    CPPUNIT_ASSERT_EQUAL(-1L, matchWayNodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-3L, matchWayNodeIds.at(1));
    way1 = map->getWay(-1);
    const std::vector<long>& way1NodeIds = way1->getNodeIds();
    CPPUNIT_ASSERT_EQUAL(-1L, way1NodeIds.at(0));
    CPPUNIT_ASSERT_EQUAL(-2L, way1NodeIds.at(1));
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiLineStringSplitterTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiLineStringSplitterTest, "quick");

}
