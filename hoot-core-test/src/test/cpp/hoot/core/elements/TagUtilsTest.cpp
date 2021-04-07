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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/TagUtils.h>

namespace hoot
{

class TagUtilsTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(TagUtilsTest);
    CPPUNIT_TEST(runKvpTest);
    CPPUNIT_TEST(runInvalidKvpTest);
    CPPUNIT_TEST(runKeyTest);
    CPPUNIT_TEST_SUITE_END();

public:

  TagUtilsTest() = default;

  void runKvpTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->setTag("foo", "bar");
    map->addNode(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->setTag("foo", "baz");
    map->addNode(node2);

    std::set<ElementId> elementIds;
    elementIds.insert(ElementId::node(1));
    elementIds.insert(ElementId::node(2));

    QStringList kvps;
    kvps.append("foo=bar");
    kvps.append("blah=bleh");
    CPPUNIT_ASSERT(TagUtils::anyElementsHaveAnyKvp(kvps, elementIds, map));

    kvps.clear();
    kvps.append("blah=bleh");
    kvps.append("foo=baz");
    CPPUNIT_ASSERT(TagUtils::anyElementsHaveAnyKvp(kvps, elementIds, map));

    kvps.clear();
    kvps.append("blah=blah");
    kvps.append("blah=bleh");
    CPPUNIT_ASSERT(!TagUtils::anyElementsHaveAnyKvp(kvps, elementIds, map));
  }

  void runInvalidKvpTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->setTag("foo", "bar");
    map->addNode(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->setTag("foo", "baz");
    map->addNode(node2);

    std::set<ElementId> elementIds;
    elementIds.insert(ElementId::node(1));
    elementIds.insert(ElementId::node(2));

    QStringList kvps;
    kvps.append("foobar");
    kvps.append("blah=bleh");

    QString exceptionMsg;
    try
    {
      TagUtils::anyElementsHaveAnyKvp(kvps, elementIds, map)  ;
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid kvp: foobar").toStdString(), exceptionMsg.toStdString());
  }

  void runKeyTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node1 =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node1->setTag("foo", "bar");
    map->addNode(node1);
    NodePtr node2 =
      std::make_shared<Node>(Status::Unknown1, 2, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node2->setTag("foo", "baz");
    map->addNode(node2);

    std::set<ElementId> elementIds;
    elementIds.insert(ElementId::node(1));
    elementIds.insert(ElementId::node(2));

    QStringList keys;
    keys.append("foo");
    CPPUNIT_ASSERT(TagUtils::allElementsHaveAnyTagKey(keys, elementIds, map));

    keys.clear();
    keys.append("bar");
    CPPUNIT_ASSERT(!TagUtils::allElementsHaveAnyTagKey(keys, elementIds, map));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagUtilsTest, "quick");

}
