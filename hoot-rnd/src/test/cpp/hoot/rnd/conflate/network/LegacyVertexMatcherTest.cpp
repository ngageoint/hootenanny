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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/filters/HighwayCriterion.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/rnd/conflate/network/LegacyVertexMatcher.h>
#include <hoot/rnd/conflate/network/NetworkDetails.h>
#include <hoot/rnd/conflate/network/OsmNetworkExtractor.h>


namespace hoot
{

class LegacyVertexMatcherTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(LegacyVertexMatcherTest);
  CPPUNIT_TEST(toyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Extract a toy network and verify that the result is as expected.
   */
  void toyTest()
  {
    TestUtils::resetEnvironment();
    OsmMapPtr map(new OsmMap());

    //       c
    //       |
    // a-----b-----e
    //       |
    //       d

    Tags tags;
    tags.set("highway", "road");

    NodePtr a1 = TestUtils::createNode(map, Status::Unknown1, -50, 0);
    NodePtr b1 = TestUtils::createNode(map, Status::Unknown1, 0, 0);
    NodePtr c1 = TestUtils::createNode(map, Status::Unknown1, 0, 50);
    NodePtr d1 = TestUtils::createNode(map, Status::Unknown1, 0, -50);
    NodePtr e1 = TestUtils::createNode(map, Status::Unknown1, 50, 0);

    WayPtr ab1 = TestUtils::createWay(map, QList<NodePtr>() << a1 << b1, Status::Unknown1, 15,
      tags);
    WayPtr bc1 = TestUtils::createWay(map, QList<NodePtr>() << b1 << c1, Status::Unknown1, 15,
      tags);
    WayPtr bd1 = TestUtils::createWay(map, QList<NodePtr>() << b1 << d1, Status::Unknown1, 15,
      tags);
    WayPtr be1 = TestUtils::createWay(map, QList<NodePtr>() << b1 << e1, Status::Unknown1, 15,
      tags);

    NodePtr a2 = TestUtils::createNode(map, Status::Unknown2, -51, 1);
    NodePtr b2 = TestUtils::createNode(map, Status::Unknown2, 1, 1);
    NodePtr c2 = TestUtils::createNode(map, Status::Unknown2, 1, 51);
    NodePtr d2 = TestUtils::createNode(map, Status::Unknown2, 1, -51);
    NodePtr e2 = TestUtils::createNode(map, Status::Unknown2, 51, 1);

    WayPtr ba2 = TestUtils::createWay(map, QList<NodePtr>() << b2 << a2, Status::Unknown2, 15,
      tags);
    WayPtr bc2 = TestUtils::createWay(map, QList<NodePtr>() << b2 << c2, Status::Unknown2, 15,
      tags);
    WayPtr db2 = TestUtils::createWay(map, QList<NodePtr>() << d2 << b2, Status::Unknown2, 15,
      tags);
    WayPtr be2 = TestUtils::createWay(map, QList<NodePtr>() << b2 << e2, Status::Unknown2, 15,
      tags);

    OsmNetworkExtractor ext1;
    ElementCriterionPtr criterion1(new StatusCriterion(Status::Unknown1));
    ext1.setCriterion(criterion1);
    OsmNetworkPtr n1 = ext1.extractNetwork(map);

    OsmNetworkExtractor ext2;
    ElementCriterionPtr criterion2(new StatusCriterion(Status::Unknown2));
    ext2.setCriterion(criterion2);
    OsmNetworkPtr n2 = ext2.extractNetwork(map);

    NetworkDetails details(map, n1, n2);

    LegacyVertexMatcher uut(map);
    uut.identifyVertexMatches(n1, n2, details);

    HOOT_STR_EQUALS(1, uut.isCandidateMatch(n1->getSingleVertex(a1->getElementId()),
      n2->getSingleVertex(a2->getElementId()), details));
    HOOT_STR_EQUALS(0, uut.isConfidentTiePoint(n1->getSingleVertex(a1->getElementId()),
      n2->getSingleVertex(a2->getElementId())));
    HOOT_STR_EQUALS(0, uut.scoreMatch(n1->getSingleVertex(a1->getElementId()),
      n2->getSingleVertex(a2->getElementId())));

    HOOT_STR_EQUALS(1, uut.isCandidateMatch(n1->getSingleVertex(b1->getElementId()),
      n2->getSingleVertex(b2->getElementId()), details));
    HOOT_STR_EQUALS(1, uut.isConfidentTiePoint(n1->getSingleVertex(b1->getElementId()),
      n2->getSingleVertex(b2->getElementId())));
    HOOT_STR_EQUALS(1, uut.scoreMatch(n1->getSingleVertex(b1->getElementId()),
      n2->getSingleVertex(b2->getElementId())));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LegacyVertexMatcherTest, "quick");

}
