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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/network/NetworkVertex.h>
#include <hoot/core/elements/Node.h>

using namespace geos::geom;

namespace hoot
{

TEST_UTILS_REGISTER_RESET(NetworkVertex)

class NetworkVertexTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NetworkVertexTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  /**
   * Test basic functionality of NetworkVertex class
   */
  void basicTest()
  {
    // Test the non-const stuff...
    NodePtr pN1 = std::make_shared<Node>(Status::Unknown1, -100, Coordinate(1.0, 1.0), 10.0);
    NodePtr pN2 = std::make_shared<Node>(Status::Unknown1, -200, Coordinate(2.0, 2.0), 10.0);
    NetworkVertexPtr pNV1 = std::make_shared<NetworkVertex>(pN1);
    NetworkVertexPtr pNV2 = std::make_shared<NetworkVertex>(pN2);

    // getElement
    CPPUNIT_ASSERT_EQUAL(reinterpret_cast<const Element*>(pN1.get()),
                         reinterpret_cast<const Element*>(pNV1->getElement().get()));

    // getElementId
    CPPUNIT_ASSERT_EQUAL(pN1->getElementId(), pNV1->getElementId());
    CPPUNIT_ASSERT_EQUAL(pN2->getElementId(), pNV2->getElementId());

    // toString
    HOOT_STR_EQUALS("(0) Node(-100)", pNV1->toString());
    HOOT_STR_EQUALS("(1) Node(-200)", pNV2->toString());

    // Less Than
    CPPUNIT_ASSERT(pNV2 < pNV1);

    // Q Hash
    CPPUNIT_ASSERT_EQUAL(99u, qHash(pNV1));

    // Test the const stuff
    NodePtr pN3 = std::make_shared<Node>(Status::Unknown1, -300, Coordinate(3.0, 3.0), 10.0);
    NodePtr pN4 = std::make_shared<Node>(Status::Unknown1, -400, Coordinate(4.0, 4.0), 10.0);
    ConstNetworkVertexPtr cNV3 = std::make_shared<NetworkVertex>(pN3);
    ConstNetworkVertexPtr cNV4 = std::make_shared<NetworkVertex>(pN4);

    // Less than
    CPPUNIT_ASSERT(cNV4 < cNV3);

    // Q Hash
    CPPUNIT_ASSERT_EQUAL(299u, qHash(cNV3));

  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NetworkVertexTest, "quick");

}
