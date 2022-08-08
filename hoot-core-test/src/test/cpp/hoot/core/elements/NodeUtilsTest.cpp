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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/NodeUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class NodeUtilsTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(NodeUtilsTest);
    CPPUNIT_TEST(runNodeIdsTest);
    CPPUNIT_TEST(runCoordsMatchTest);
    CPPUNIT_TEST(runCoordsToStringTest);
    CPPUNIT_TEST_SUITE_END();

public:

  NodeUtilsTest() = default;

  void runNodeIdsTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm");

    QList<ConstNodePtr> nodes;
    const NodeMap& mapNodes = map->getNodes();
    for (NodeMap::const_iterator nodeIter = mapNodes.begin(); nodeIter != mapNodes.end();
         ++nodeIter)
    {
      nodes.append(nodeIter->second);
    }
    CPPUNIT_ASSERT_EQUAL(36, NodeUtils::nodesToNodeIds(nodes).size());
    CPPUNIT_ASSERT_EQUAL(
      (size_t)36, NodeUtils::nodesToNodeIds(nodes.toVector().toStdVector()).size());

    const QSet<long> nodeIds = map->getNodeIds();
    CPPUNIT_ASSERT_EQUAL(36, NodeUtils::nodeIdsToNodes(QList<long>::fromSet(nodeIds), map).size());
    CPPUNIT_ASSERT_EQUAL(
      (size_t)36,
      NodeUtils::nodeIdsToNodes(QList<long>::fromSet(nodeIds).toVector().toStdVector(), map).size());
  }

  void runCoordsMatchTest()
  {
    std::vector<ConstNodePtr> nodes1;
    ConstNodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    nodes1.push_back(node1);
    ConstNodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 5.0), 15.0);
    nodes1.push_back(node2);
    std::vector<ConstNodePtr> nodes2;
    ConstNodePtr node3 =
      std::make_shared<Node>(Status::Unknown1, 3, geos::geom::Coordinate(0.0, 0.0), 15.0);
    nodes2.push_back(node3);
    ConstNodePtr node4 =
      std::make_shared<Node>(Status::Unknown1, 4, geos::geom::Coordinate(0.0, 5.0), 15.0);
    nodes2.push_back(node4);
    CPPUNIT_ASSERT(NodeUtils::nodeCoordsMatch(nodes1, nodes2));

    std::vector<ConstNodePtr> nodes3;
    ConstNodePtr node5 =
      std::make_shared<Node>(Status::Unknown1, 5, geos::geom::Coordinate(0.0, 0.0), 15.0);
    nodes3.push_back(node5);
    ConstNodePtr node6 =
      std::make_shared<Node>(Status::Unknown1, 6, geos::geom::Coordinate(0.0, 10.0), 15.0);
    nodes3.push_back(node6);
    CPPUNIT_ASSERT(!NodeUtils::nodeCoordsMatch(nodes1, nodes3));

    std::vector<ConstNodePtr> nodes4;
    ConstNodePtr node7 =
      std::make_shared<Node>(Status::Unknown1, 7, geos::geom::Coordinate(0.0, 0.0), 15.0);
    nodes4.push_back(node5);
    CPPUNIT_ASSERT(!NodeUtils::nodeCoordsMatch(nodes1, nodes4));

    std::vector<ConstNodePtr> nodes5;
    ConstNodePtr node8;
    nodes5.push_back(node8);
    ConstNodePtr node9 =
      std::make_shared<Node>(Status::Unknown1, 9, geos::geom::Coordinate(0.0, 0.0), 15.0);
    nodes5.push_back(node9);
    CPPUNIT_ASSERT(!NodeUtils::nodeCoordsMatch(nodes1, nodes5));
  }

  void runCoordsToStringTest()
  {
    std::vector<ConstNodePtr> nodes;
    ConstNodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    nodes.push_back(node1);
    ConstNodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 5.0), 15.0);
    nodes.push_back(node2);
    HOOT_STR_EQUALS(
      "ID: 1, X: 0.0000000, Y: 0.0000000; ID: 2, X: 0.0000000, Y: 5.0000000",
      NodeUtils::nodeCoordsToString(nodes).toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NodeUtilsTest, "quick");

}
