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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// geos
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/FindNodesInWayFactory.h>
#include <hoot/core/geometry/RelationToMultiPolygonConverter.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class RelationToMultiPolygonConverterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RelationToMultiPolygonConverterTest);
  CPPUNIT_TEST(runBadOuterRingsTest);
  CPPUNIT_TEST(runMultiPolygonExample1Test);
  CPPUNIT_TEST(runMultiPolygonExample7Test);
  CPPUNIT_TEST(runMultipleWaysFormingARing);
  CPPUNIT_TEST_SUITE_END();

public:

  FindNodesInWayFactory f;

  void addPoint(OsmMapPtr map, WayPtr w, double x, double y)
  {
    // use a node factory so nodes w/ the same coordinates get the same ids
    NodePtr n =
      f.createNode(map->shared_from_this(), Coordinate(x, y), Status::Unknown1,
      w->getCircularError());
    map->addNode(n);
    w->addNode(n->getId());
    f.addWay(w);
  }

  void closeWay(WayPtr w)
  {
    w->addNode(w->getNodeId(0));
  }

  /**
   * This was found when processing Maax data over Syria. I since simplified the problem into the
   * lines below. If two ways have the same starting point, but different ending points this was
   * causing errors.
   */
  void runBadOuterRingsTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr uut =
      std::make_shared<Relation>(Status::Unknown1, 1, 10, MetadataTags::RelationMultiPolygon());
    WayPtr w;
    // way #1
    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 5, 6);
    addPoint(map, w, 8, 11);
    addPoint(map, w, 12, 9);
    uut->addElement(MetadataTags::RoleOuter(), w);

    // way #2
    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 5, 6);
    addPoint(map, w, 8, 2);
    addPoint(map, w, 13, 5);
    uut->addElement(MetadataTags::RoleOuter(), w);

    std::shared_ptr<Geometry> g = RelationToMultiPolygonConverter(map, uut).createMultipolygon();

    CPPUNIT_ASSERT_EQUAL(string("MULTIPOLYGON (((13.0000000000000000 5.0000000000000000, 8.0000000000000000 2.0000000000000000, 5.0000000000000000 6.0000000000000000, 8.0000000000000000 11.0000000000000000, 12.0000000000000000 9.0000000000000000, 13.0000000000000000 5.0000000000000000)))"),
                         g->toString());
  }

  /**
   * Taken from OSM Wiki, "One outer and one inner ring"
   * http://wiki.openstreetmap.org/wiki/Relation:multipolygon#One_outer_and_one_inner_ring
   */
  void runMultiPolygonExample1Test()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr uut =
      std::make_shared<Relation>(Status::Unknown1, 1, 10, MetadataTags::RelationMultiPolygon());
    WayPtr w;
    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 8, 2);
    addPoint(map, w, 5, 6);
    addPoint(map, w, 8, 11);
    addPoint(map, w, 12, 9);
    addPoint(map, w, 13, 5);
    closeWay(w);
    uut->addElement(MetadataTags::RoleOuter(), w);

    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 9, 5);
    addPoint(map, w, 7, 6);
    addPoint(map, w, 8, 8);
    addPoint(map, w, 10, 7);
    closeWay(w);
    uut->addElement(MetadataTags::RoleInner(), w);

    std::shared_ptr<Geometry> g = RelationToMultiPolygonConverter(map, uut).createMultipolygon();

    CPPUNIT_ASSERT_EQUAL(string("MULTIPOLYGON (((8.0000000000000000 2.0000000000000000, 5.0000000000000000 6.0000000000000000, 8.0000000000000000 11.0000000000000000, 12.0000000000000000 9.0000000000000000, 13.0000000000000000 5.0000000000000000, 8.0000000000000000 2.0000000000000000), (9.0000000000000000 5.0000000000000000, 7.0000000000000000 6.0000000000000000, 8.0000000000000000 8.0000000000000000, 10.0000000000000000 7.0000000000000000, 9.0000000000000000 5.0000000000000000)))"),
                         g->toString());
  }

  /**
   * Taken from OSM Wiki, "Island within a hole"
   * http://wiki.openstreetmap.org/wiki/Relation:multipolygon#Island_within_a_hole
   */
  void runMultiPolygonExample7Test()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr uut =
      std::make_shared<Relation>(Status::Unknown1, 1, 10, MetadataTags::RelationMultiPolygon());
    WayPtr w;
    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 9, 1);
    addPoint(map, w, 2, 6);
    addPoint(map, w, 6, 12);
    addPoint(map, w, 13, 11);
    addPoint(map, w, 14, 4);
    closeWay(w);
    uut->addElement(MetadataTags::RoleOuter(), w);

    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 10, 2);
    addPoint(map, w, 5, 6);
    addPoint(map, w, 8, 11);
    addPoint(map, w, 12, 7);
    closeWay(w);
    uut->addElement(MetadataTags::RoleInner(), w);

    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 9, 5);
    addPoint(map, w, 7, 6);
    addPoint(map, w, 8, 7);
    addPoint(map, w, 10, 7);
    closeWay(w);
    uut->addElement(MetadataTags::RoleOuter(), w);

    std::shared_ptr<Geometry> g = RelationToMultiPolygonConverter(map, uut).createMultipolygon();

    CPPUNIT_ASSERT_EQUAL(string("MULTIPOLYGON (((9.0000000000000000 1.0000000000000000, 2.0000000000000000 6.0000000000000000, 6.0000000000000000 12.0000000000000000, 13.0000000000000000 11.0000000000000000, 14.0000000000000000 4.0000000000000000, 9.0000000000000000 1.0000000000000000), (10.0000000000000000 2.0000000000000000, 5.0000000000000000 6.0000000000000000, 8.0000000000000000 11.0000000000000000, 12.0000000000000000 7.0000000000000000, 10.0000000000000000 2.0000000000000000)), ((9.0000000000000000 5.0000000000000000, 7.0000000000000000 6.0000000000000000, 8.0000000000000000 7.0000000000000000, 10.0000000000000000 7.0000000000000000, 9.0000000000000000 5.0000000000000000)))"),
                         g->toString());
  }

  /**
   * Taken from OSM Wiki, "Multiple ways forming a ring", 2014-06-17
   * http://wiki.openstreetmap.org/wiki/Relation:multipolygon#Multiple_ways_forming_a_ring
   */
  void runMultipleWaysFormingARing()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    RelationPtr uut =
      std::make_shared<Relation>(Status::Unknown1, 1, 10, MetadataTags::RelationMultiPolygon());
    WayPtr w;
    // way #1
    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 5, 6);
    addPoint(map, w, 8, 11);
    addPoint(map, w, 12, 9);
    uut->addElement(MetadataTags::RoleOuter(), w);

    // way #2
    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 5, 6);
    addPoint(map, w, 8, 2);
    addPoint(map, w, 13, 5);
    addPoint(map, w, 12, 9);
    uut->addElement(MetadataTags::RoleOuter(), w);

    // way #3
    w = std::make_shared<Way>(Status::Unknown1, map->createNextWayId(), 10);
    map->addWay(w);
    addPoint(map, w, 7, 6);
    addPoint(map, w, 8, 8);
    addPoint(map, w, 10, 7);
    addPoint(map, w, 9, 5);
    closeWay(w);
    uut->addElement(MetadataTags::RoleInner(), w);

    std::shared_ptr<Geometry> g = RelationToMultiPolygonConverter(map, uut).createMultipolygon();

    CPPUNIT_ASSERT_EQUAL(string("MULTIPOLYGON (((5.0000000000000000 6.0000000000000000, 8.0000000000000000 11.0000000000000000, 12.0000000000000000 9.0000000000000000, 13.0000000000000000 5.0000000000000000, 8.0000000000000000 2.0000000000000000, 5.0000000000000000 6.0000000000000000), (7.0000000000000000 6.0000000000000000, 8.0000000000000000 8.0000000000000000, 10.0000000000000000 7.0000000000000000, 9.0000000000000000 5.0000000000000000, 7.0000000000000000 6.0000000000000000)))"),
                         g->toString());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RelationToMultiPolygonConverterTest, "quick");

}
