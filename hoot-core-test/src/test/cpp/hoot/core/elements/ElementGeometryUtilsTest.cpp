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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/elements/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

//  Geos
//#include <geos/geom/Coordinate.h>
//#include <geos/geom/Geometry.h>
//#include <geos/geom/LineString.h>
//#include <geos/geom/Polygon.h>
//using namespace geos::geom;

namespace hoot
{

class ElementGeometryUtilsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementGeometryUtilsTest);
  CPPUNIT_TEST(calculateLengthTest);
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

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementGeometryUtilsTest, "quick");

}

