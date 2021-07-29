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
 * @copyright Copyright (C) 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/HasAddressCriterion.h>

namespace hoot
{

class HasAddressCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HasAddressCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  HasAddressCriterionTest()
  {
    setResetType(ResetAll);
  }

  void runBasicTest()
  {
    HasAddressCriterion uut;
    conf().set("address.match.enabled", "true");
    uut.setConfiguration(conf());

    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "123 Main Street");
    CPPUNIT_ASSERT(uut.isSatisfied(node1));

    WayPtr way1 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way1->getTags().set(AddressTagKeys::FULL_ADDRESS_TAG_NAME, "blah");
    CPPUNIT_ASSERT(!uut.isSatisfied(way1));

    WayPtr way2 = std::make_shared<Way>(Status::Unknown1, -1, 15.0);
    way2->getTags().set("blah", "123 Main Street");
    CPPUNIT_ASSERT(!uut.isSatisfied(way2));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HasAddressCriterionTest, "slow");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HasAddressCriterionTest, "serial");

}
