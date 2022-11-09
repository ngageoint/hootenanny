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
 * @copyright Copyright (C) 2015, 2016, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Status.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlDiskCache.h>
#include <hoot/core/elements/ElementComparer.h>

namespace hoot
{

class OsmXmlDiskCacheTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmXmlDiskCacheTest);
  CPPUNIT_TEST(runAddSingleNodeTest);
  CPPUNIT_TEST(runMultipleNodeTest);
  CPPUNIT_TEST(runAddAllTypesTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runAddSingleNodeTest()
  {
    OsmXmlDiskCache myCache("/tmp/hoot.disk.cache.osm");

    // Make a new node!
    uint64_t nodeId(1);
    uint64_t version(1);
    double lat(1.001);
    double lon(1.001);
    Meters circularError(15.0);
    ConstElementPtr refNode = std::make_shared<const Node>(Status::Unknown1,
                                                           nodeId, lon, lat,
                                                           circularError,
                                                           ElementData::CHANGESET_EMPTY,
                                                           version);

    // Cache the node
    myCache.addElement(refNode);

    // Fetch the node
    NodePtr cachedNode = myCache.getNode(nodeId);

    // Verify
    ElementComparer comparitor;
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(refNode), cachedNode));
  }

  void runMultipleNodeTest()
  {
    OsmXmlDiskCache myCache("/tmp/hoot.disk.cache.osm");

    // Make a new node!
    uint64_t nodeId(1);
    uint64_t version(1);
    double lat(1.001);
    double lon(1.001);
    Meters circularError(15.0);
    ConstElementPtr refNode1 = std::make_shared<const Node>(Status::Unknown1,
                                                            nodeId, lon, lat,
                                                            circularError,
                                                            ElementData::CHANGESET_EMPTY,
                                                            version);
    myCache.addElement(refNode1);

    nodeId = 2;
    lat = 1.002;
    lon = 1.002;
    ConstElementPtr refNode2 = std::make_shared<const Node>(Status::Unknown1,
                                                            nodeId, lon, lat,
                                                            circularError,
                                                            ElementData::CHANGESET_EMPTY,
                                                            version);
    myCache.addElement(refNode2);

    nodeId = 3;
    lat = 1.003;
    lon = 1.003;
    ConstElementPtr refNode3 = std::make_shared<const Node>(Status::Unknown1,
                                                            nodeId, lon, lat,
                                                            circularError,
                                                            ElementData::CHANGESET_EMPTY,
                                                            version);
    myCache.addElement(refNode3);

    nodeId = 4;
    lat = 1.004;
    lon = 1.004;
    ConstElementPtr refNode4 = std::make_shared<const Node>(Status::Unknown1,
                                                            nodeId, lon, lat,
                                                            circularError,
                                                            ElementData::CHANGESET_EMPTY,
                                                            version);
    myCache.addElement(refNode4);

    nodeId = 5;
    lat = 1.005;
    lon = 1.005;
    ConstElementPtr refNode5 = std::make_shared<const Node>(Status::Unknown1,
                                                            nodeId, lon, lat,
                                                            circularError,
                                                            ElementData::CHANGESET_EMPTY,
                                                            version);
    myCache.addElement(refNode5);

    // Get node1 back & verify it's the same
    ElementComparer comparitor;
    NodePtr cachedNode = myCache.getNode(1);
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(refNode1), cachedNode));

    // Get node2 back & verify it's the same
    cachedNode = myCache.getNode(2);
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(refNode2), cachedNode));

    // Verify it's not the same as node1
    CPPUNIT_ASSERT(!comparitor.isSame(std::const_pointer_cast<Element>(refNode1), cachedNode));

    // Get node3 & verify
    cachedNode = myCache.getNode(3);
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(refNode3), cachedNode));

    // Go get node1 and 2 again and verify
    cachedNode = myCache.getNode(1);
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(refNode1), cachedNode));

    cachedNode = myCache.getNode(2);
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(refNode2), cachedNode));

    // Jump ahead to node5
    cachedNode = myCache.getNode(5);
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(refNode5), cachedNode));

    // Do node3, then node5 again
    cachedNode = myCache.getNode(3);
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(refNode3), cachedNode));

    cachedNode = myCache.getNode(5);
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(refNode5), cachedNode));
  }

  void runAddAllTypesTest()
  {
    OsmXmlDiskCache myCache("/tmp/hoot.disk.cache");
    ElementComparer comparitor;

    // Add new node
    uint64_t nodeId(321);
    uint64_t version(1);
    double lat(2.0);
    double lon(3.0);
    Meters circularError(15.0);
    ConstElementPtr newNode = std::make_shared<const Node>(Status::Unknown1,
                                                           nodeId, lon, lat,
                                                           circularError,
                                                           ElementData::CHANGESET_EMPTY,
                                                           version);
    myCache.addElement(newNode);

    // Add new way
    ConstElementPtr newWay = std::make_shared<const Way>(Status::Unknown1,
                                                         10, circularError,
                                                         ElementData::CHANGESET_EMPTY,
                                                         version);
    myCache.addElement(newWay);

    // Add new relation
    ConstElementPtr newRelation = std::make_shared<Relation>(Status::Unknown1,
                                                             20, circularError,
                                                             "", ElementData::CHANGESET_EMPTY,
                                                             version);
    myCache.addElement(newRelation);

    // Check Node
    NodePtr myNodeCopy = myCache.getNode(newNode->getId());
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(newNode), myNodeCopy));

    // Check Way
    WayPtr myWayCopy = myCache.getWay(newWay->getId());
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(newWay), myWayCopy));

    // Check Relation
    RelationPtr myRelCopy = myCache.getRelation(newRelation->getId());
    CPPUNIT_ASSERT(comparitor.isSame(std::const_pointer_cast<Element>(newRelation), myRelCopy));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmXmlDiskCacheTest, "quick");

}
