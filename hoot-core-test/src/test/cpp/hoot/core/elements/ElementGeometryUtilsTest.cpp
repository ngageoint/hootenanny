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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

class ElementGeometryUtilsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementGeometryUtilsTest);
  CPPUNIT_TEST(calculateLengthTest);
  CPPUNIT_TEST(haveRelationshipTest);
  CPPUNIT_TEST(haveRelationshipNullInputTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void calculateLengthTest()
  {
    OsmMapPtr map(new OsmMap());
    MapProjector::projectToPlanar(map);
    QList<NodePtr> nodes;
    nodes.push_back(TestUtils::createNode(map, Status::Unknown1,  0.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, Status::Unknown1, 10.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, Status::Unknown1, 10.0, 10.0));
    nodes.push_back(TestUtils::createNode(map, Status::Unknown1,  0.0, 10.0));
    WayPtr way = TestUtils::createWay(map, nodes);

    //  Check the length of a way
    CPPUNIT_ASSERT_DOUBLES_EQUAL(30.0, ElementGeometryUtils::calculateLength(way, map), 1e-3);

    //  The length of a node should be 0 (error)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.0, ElementGeometryUtils::calculateLength(map->getNode(way->getNodeId(0)), map), 1e-3);

    //  The length of an area should be 0 (error)
    way->addNode(way->getNodeId(0));
    way->setTag("building", "yes");
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
        element1, element1, GeometricRelationship::Contains, map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("One of the input elements is null.").toStdString(), exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementGeometryUtilsTest, "quick");

}

