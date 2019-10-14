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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/NameCriterion.h>

namespace hoot
{

class NameCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NameCriterionTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    NameCriterion uut;
    NodePtr node(new Node(Status::Unknown1, -1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    node->getTags().set("name", "Old Town Tavern");
    node->getTags().set("key2", "blah");

    uut.setNames(QStringList("Old Town Tavern"));
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    uut.setNames(QStringList("OLD TOWN TAVERN"));
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    uut.setNames(QStringList("blah"));
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    uut.setCaseSensitive(true);

    uut.setNames(QStringList("OLD TOWN TAVERN"));
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NameCriterionTest, "quick");

}
