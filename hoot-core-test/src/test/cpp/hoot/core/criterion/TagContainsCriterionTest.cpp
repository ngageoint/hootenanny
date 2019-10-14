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
#include <hoot/core/criterion/TagContainsCriterion.h>

using namespace geos::geom;

namespace hoot
{

class TagContainsCriterionTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagContainsCriterionTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runCaseSensitivityTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    QStringList kvps;
    kvps.append("key1=val1");
    kvps.append("key2=val2");

    TagContainsCriterion uut;
    uut.setKvps(kvps);

    NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));

    node->getTags().set("key1", "val");
    node->getTags().set("key2", "val");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    // only one match is required
    node->getTags().clear();
    node->getTags().set("key1", "val");
    node->getTags().set("key2", "blah");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("key1", "blah");
    node->getTags().set("key2", "blah");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runCaseSensitivityTest()
  {
    TagContainsCriterion uut;
    uut.setKvps(QStringList("key=val"));
    uut.setCaseSensitive(false);

    NodePtr node(new Node(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0));

    node->getTags().set("key", "val");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().set("key", "VAL");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    // the case sensitivity is for vals only and not keys
    node->getTags().set("KEY", "val");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));

    uut.setCaseSensitive(true);

    node->getTags().set("key", "VAL");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagContainsCriterionTest, "quick");

}
