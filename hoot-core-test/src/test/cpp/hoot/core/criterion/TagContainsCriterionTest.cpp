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
 * @copyright Copyright (C) 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
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
  CPPUNIT_TEST(runBasicTest2);
  CPPUNIT_TEST(runConfigureTest);
  CPPUNIT_TEST(runCaseSensitivityTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    QStringList kvps;
    kvps.append("key1=val");
    kvps.append("key2=va2");

    TagContainsCriterion uut;
    uut.setKvps(kvps);

    NodePtr node = std::make_shared<Node>(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0);

    node->getTags().set("key1", "val1");
    node->getTags().set("key2", "val2");
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

  void runBasicTest2()
  {
    TagContainsCriterion uut("key1", "val");

    NodePtr node = std::make_shared<Node>(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0);

    node->getTags().set("key1", "val1");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    // only one match is required
    node->getTags().clear();
    node->getTags().set("key1", "val");
    node->getTags().set("key2", "blah");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().clear();
    node->getTags().set("key1", "blah");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }

  void runConfigureTest()
  {
    QStringList kvps;
    kvps.append("key1=val");
    kvps.append("key2=va2");

    Settings settings;
    settings.set(ConfigOptions::getTagContainsCriterionKvpsKey(), kvps);
    settings.set(ConfigOptions::getTagContainsCriterionCaseSensitiveKey(), false);
    TagContainsCriterion uut;
    uut.setConfiguration(settings);

    NodePtr node = std::make_shared<Node>(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0);

    node->getTags().set("key1", "val1");
    node->getTags().set("key2", "val2");
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

    NodePtr node = std::make_shared<Node>(Status::Unknown1, -1, Coordinate(0.0, 0.0), 15.0);

    node->getTags().set("key", "val1");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    node->getTags().set("key", "VAL1");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    // This doesn't seem right.
    node->getTags().set("KEY", "val1");
    CPPUNIT_ASSERT(uut.isSatisfied(node));

    uut.setCaseSensitive(true);

    node->getTags().set("key", "VAL1");
    CPPUNIT_ASSERT(!uut.isSatisfied(node));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagContainsCriterionTest, "quick");

}
