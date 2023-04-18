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
 * @copyright Copyright (C) 2020-2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class ElementGeometryUtilsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementGeometryUtilsTest);
  CPPUNIT_TEST(calculateLengthTest);
  CPPUNIT_TEST(haveRelationshipTest);
  CPPUNIT_TEST(haveRelationshipNullInputTest);
  CPPUNIT_TEST(geometricRelationshipToStringTest);
  CPPUNIT_TEST(calculateElementCentroid);
  CPPUNIT_TEST_SUITE_END();

public:

  void calculateLengthTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    MapProjector::projectToPlanar(map);
    QList<NodePtr> nodes;
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0, 10.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0, 10.0));
    WayPtr way = TestUtils::createWay(map, nodes, "", Status::Unknown1);

    //  Check the length of a way
    CPPUNIT_ASSERT_DOUBLES_EQUAL(30.0, ElementGeometryUtils::calculateLength(way, map), 1e-3);

    //  The length of a node should be 0 (error)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, ElementGeometryUtils::calculateLength(map->getNode(way->getNodeId(0)), map), 1e-3);

    //  The length of an area should be 0 (error)
    way->addNode(way->getNodeId(0));
    way->setTag(MetadataTags::Building(), "yes");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ElementGeometryUtils::calculateLength(way, map), 1e-3);
  }

  void haveRelationshipTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, "test-files/cases/reference/unifying/geometry-generic-1/Input1.osm", false,
      Status::Unknown1);
    OsmMapReaderFactory::read(
      map, "test-files/cases/reference/unifying/geometry-generic-1/Input2.osm", false,
      Status::Unknown2);

    ConstElementPtr node1 = MapUtils::getFirstElementWithNote(map, "J");
    ConstElementPtr way1 = MapUtils::getFirstElementWithNote(map, "I");

    // Not really that concerned with testing how well GEOS actually works, just want to exercise
    // all of the code in the relationship enum. If you want to try out better input test data with
    // this, feel free.

    CPPUNIT_ASSERT(
      !ElementGeometryUtils::haveGeometricRelationship(
         way1, node1, GeometricRelationship::Contains, map));
    CPPUNIT_ASSERT(
      !ElementGeometryUtils::haveGeometricRelationship(
         way1, node1, GeometricRelationship::Covers, map));
    CPPUNIT_ASSERT(
      !ElementGeometryUtils::haveGeometricRelationship(
         way1, node1, GeometricRelationship::Crosses, map));
    CPPUNIT_ASSERT(
      ElementGeometryUtils::haveGeometricRelationship(
         way1, node1, GeometricRelationship::DisjointWith, map));
    CPPUNIT_ASSERT(
      !ElementGeometryUtils::haveGeometricRelationship(
         way1, node1, GeometricRelationship::Equals, map));
    CPPUNIT_ASSERT(
      !ElementGeometryUtils::haveGeometricRelationship(
         way1, node1, GeometricRelationship::Intersects, map));
    CPPUNIT_ASSERT(
      !ElementGeometryUtils::haveGeometricRelationship(
         way1, node1, GeometricRelationship::IsWithin, map));
    CPPUNIT_ASSERT(
      !ElementGeometryUtils::haveGeometricRelationship(
         way1, node1, GeometricRelationship::Overlaps, map));
    CPPUNIT_ASSERT(
      !ElementGeometryUtils::haveGeometricRelationship(
         way1, node1, GeometricRelationship::Touches, map));
  }

  void haveRelationshipNullInputTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ElementPtr element1;
    ElementPtr element2;

    QString exceptionMsg;
    try
    {
      ElementGeometryUtils::haveGeometricRelationship(
        element1, element2, GeometricRelationship::Contains, map);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("One of the input elements is null.").toStdString(), exceptionMsg.toStdString());
  }

  void geometricRelationshipToStringTest()
  {
    CPPUNIT_ASSERT(GeometricRelationship(GeometricRelationship::Contains).toString() == "Contains");
    CPPUNIT_ASSERT(GeometricRelationship(GeometricRelationship::Covers).toString() == "Covers");
    CPPUNIT_ASSERT(GeometricRelationship(GeometricRelationship::Crosses).toString() == "Crosses");
    CPPUNIT_ASSERT(
      GeometricRelationship(GeometricRelationship::DisjointWith).toString() == "DisjointWith");
    CPPUNIT_ASSERT(GeometricRelationship(GeometricRelationship::Equals).toString() == "Equals");
    CPPUNIT_ASSERT(
      GeometricRelationship(GeometricRelationship::Intersects).toString() == "Intersects");
    CPPUNIT_ASSERT(GeometricRelationship(GeometricRelationship::IsWithin).toString() == "IsWithin");
    CPPUNIT_ASSERT(GeometricRelationship(GeometricRelationship::Overlaps).toString() == "Overlaps");
    CPPUNIT_ASSERT(GeometricRelationship(GeometricRelationship::Touches).toString() == "Touches");

    QString exceptionMsg;
    try
    {
      GeometricRelationship(GeometricRelationship::Invalid).toString();
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(QString("Unknown type.").toStdString(), exceptionMsg.toStdString());
  }

  void calculateElementCentroid()
  {
    //  Setup nodes
    std::vector<NodePtr> nodes(
      {
        std::make_shared<Node>(Status::Unknown1, 1,  9.0,  9.0),
        std::make_shared<Node>(Status::Unknown1, 2, 11.0,  9.0),
        std::make_shared<Node>(Status::Unknown1, 3, 11.0, 11.0),
        std::make_shared<Node>(Status::Unknown1, 4,  9.0, 11.0),
        std::make_shared<Node>(Status::Unknown1, 5,  8.0,  8.0),
        std::make_shared<Node>(Status::Unknown1, 6, 16.0,  8.0),
        std::make_shared<Node>(Status::Unknown1, 7, 16.0, 16.0),
        std::make_shared<Node>(Status::Unknown1, 8,  8.0, 16.0),
      });
    //  Setup ways
    std::vector<WayPtr> ways(
      {
        std::make_shared<Way>(Status::Unknown1, 1),
        std::make_shared<Way>(Status::Unknown1, 2),
        std::make_shared<Way>(Status::Unknown1, 3),
        std::make_shared<Way>(Status::Unknown1, 4)
      });
    ways[0]->addNodes({ 1, 2, 3, 4 });
    ways[1]->addNodes({ 1, 2, 3, 4, 1 });
    ways[2]->addNodes({ 5, 6, 7, 8 });
    ways[3]->addNodes({ 5, 6, 7, 8, 5 });
    //  Setup relation
    RelationPtr relation = std::make_shared<Relation>(Status::Unknown1, 1);
    relation->addElement("inner", ElementType::Way, 2);
    relation->addElement("outer", ElementType::Way, 4);
    //  Add all elements to the map
    OsmMapPtr map = std::make_shared<OsmMap>();
    map->addElements(nodes.begin(), nodes.end());
    map->addElements(ways.begin(), ways.end());
    map->addRelation(relation);
    //  Validate the centroids of each type
    geos::geom::Coordinate n = ElementGeometryUtils::calculateElementCentroid(ElementId(ElementType::Node, 1), map);
    CPPUNIT_ASSERT_EQUAL(geos::geom::Coordinate(9.0, 9.0), n);
    geos::geom::Coordinate w1 = ElementGeometryUtils::calculateElementCentroid(ElementId(ElementType::Way, 1), map);
    CPPUNIT_ASSERT_EQUAL(geos::geom::Coordinate(10.0, 10.0), w1);
    geos::geom::Coordinate w2 = ElementGeometryUtils::calculateElementCentroid(ElementId(ElementType::Way, 2), map);
    CPPUNIT_ASSERT_EQUAL(geos::geom::Coordinate(10.0, 10.0), w2);
    geos::geom::Coordinate w3 = ElementGeometryUtils::calculateElementCentroid(ElementId(ElementType::Way, 3), map);
    CPPUNIT_ASSERT_EQUAL(geos::geom::Coordinate(12.0, 12.0), w3);
    geos::geom::Coordinate w4 = ElementGeometryUtils::calculateElementCentroid(ElementId(ElementType::Way, 4), map);
    CPPUNIT_ASSERT_EQUAL(geos::geom::Coordinate(12.0, 12.0), w4);
    geos::geom::Coordinate r = ElementGeometryUtils::calculateElementCentroid(ElementId(ElementType::Relation, 1), map);
    CPPUNIT_ASSERT_EQUAL(geos::geom::Coordinate(11.0, 11.0), r);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementGeometryUtilsTest, "quick");

}
