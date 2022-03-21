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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/schema/MetadataTags.h>

//  Geos
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
using namespace geos::geom;

namespace hoot
{

class ElementToGeometryConverterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementToGeometryConverterTest);
  CPPUNIT_TEST(convertToGeometryTest);
  CPPUNIT_TEST(convertToLinestringTest);
  CPPUNIT_TEST(convertToPolygonTest);
  CPPUNIT_TEST(getGeometryTypeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void convertToGeometryTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ElementToGeometryConverter ec(map);
    QList<NodePtr> nodes;
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0, 10.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0, 10.0));

    //  Check the geometry of a node
    std::shared_ptr<Geometry> geometry = ec.convertToGeometry(ElementPtr(nodes[0]));
    HOOT_STR_EQUALS("Point", geometry->getGeometryType());

    //  Check the geometry of a way
    WayPtr way = TestUtils::createWay(map, nodes);
    geometry = ec.convertToGeometry(way);
    HOOT_STR_EQUALS("LineString", geometry->getGeometryType());

    //  Check the geometry of a closed way without any tags
    way->addNode(way->getNodeId(0));
    geometry = ec.convertToGeometry(way);
    HOOT_STR_EQUALS("LineString", geometry->getGeometryType());
    //  Add the building tag to make it an area
    way->setTag("building", "yes");
    geometry = ec.convertToGeometry(way);
    HOOT_STR_EQUALS("Polygon", geometry->getGeometryType());

    //  Check the geometry of a relation
    QList<NodePtr> nodes2;
    nodes2.push_back(TestUtils::createNode(map, "", Status::Unknown1, 2.5, 2.5));
    nodes2.push_back(TestUtils::createNode(map, "", Status::Unknown1, 7.5, 2.5));
    nodes2.push_back(TestUtils::createNode(map, "", Status::Unknown1, 7.5, 7.5));
    nodes2.push_back(TestUtils::createNode(map, "", Status::Unknown1, 2.5, 7.5));
    nodes2.push_back(nodes2[0]);
    WayPtr way2 = TestUtils::createWay(map, nodes2);
    way2->setTag("building", "yes");
    RelationPtr relation =
      TestUtils::createRelation(map, QList<ElementPtr>(), "", Status::Unknown1);
    relation->setType(MetadataTags::RelationMultiPolygon());
    relation->addElement(MetadataTags::RelationOuter(), way);
    relation->addElement(MetadataTags::RelationInner(), way2);
    geometry = ec.convertToGeometry(relation);
    HOOT_STR_EQUALS("MultiPolygon", geometry->getGeometryType());
  }

  void convertToLinestringTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ElementToGeometryConverter ec(map);
    QList<NodePtr> nodes;
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0, 10.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0, 10.0));
    WayPtr way = TestUtils::createWay(map, nodes);
    //  Check the convert to linestring method
    std::shared_ptr<LineString> ls = ec.convertToLineString(way);
    CPPUNIT_ASSERT_EQUAL(Dimension::L, ls->getDimension());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(30.0, ls->getLength(), 1e-3);
    HOOT_STR_EQUALS("LineString", ls->getGeometryType());
  }

  void convertToPolygonTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ElementToGeometryConverter ec(map);
    QList<NodePtr> nodes;
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0, 10.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0, 10.0));
    nodes.push_back(nodes[0]);
    WayPtr way = TestUtils::createWay(map, nodes);
    //  Check the covert to polygon method
    std::shared_ptr<Polygon> poly = ec.convertToPolygon(way);
    CPPUNIT_ASSERT_EQUAL(Dimension::A, poly->getDimension());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, poly->getArea(), 1e-3);
    HOOT_STR_EQUALS("Polygon", poly->getGeometryType());
  }

  void getGeometryTypeTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    ElementToGeometryConverter ec(map);
    QList<NodePtr> nodes;
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0,  0.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1, 10.0, 10.0));
    nodes.push_back(TestUtils::createNode(map, "", Status::Unknown1,  0.0, 10.0));
    WayPtr way = TestUtils::createWay(map, nodes);
    //  non-closed linestring
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(way), GEOS_LINESTRING);

    //  Close the way
    way->addNode(nodes[0]->getId());
    //  Set FCODE to
    way->setTag("F_CODE", "BH140");
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(way), GEOS_POLYGON);

    //  Set WCC
    way->setTag("WCC", "7");
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(way), GEOS_POLYGON);

    //  Test against a single node
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(nodes[0]), GEOS_POINT);

    //  Relation geometry types
    QList<ElementPtr> elements;
    for (QList<NodePtr>::iterator it = nodes.begin(); it != nodes.end(); ++it)
      elements.append(*it);
    RelationPtr relation = TestUtils::createRelation(map, elements);
    //  Reviews are collections
    relation->setType(MetadataTags::RelationReview());
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(relation), GEOS_GEOMETRYCOLLECTION);

    //  Multipoint types that come from GeoJSON are multipoint
    relation->setType(MetadataTags::RelationMultiPoint());
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(relation), GEOS_MULTIPOINT);

    //  Restriction relations are collections
    relation->setType(MetadataTags::RelationRestriction());
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(relation), GEOS_GEOMETRYCOLLECTION);

    //  Multipolygon check
    relation->setType(MetadataTags::RelationMultiPolygon());
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(relation), GEOS_MULTIPOLYGON);

    //  Boundaries are multilinestrings
    relation->setType(MetadataTags::RelationBoundary());
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(relation), GEOS_MULTILINESTRING);

    //  Multilinestrings are multilinestrings
    relation->setType(MetadataTags::RelationMultilineString());
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(relation), GEOS_MULTILINESTRING);

    //  Empty relations with no members are collections also
    elements.clear();
    relation = TestUtils::createRelation(map, elements);
    CPPUNIT_ASSERT_EQUAL(ec.getGeometryType(relation), GEOS_GEOMETRYCOLLECTION);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementToGeometryConverterTest, "quick");

}

