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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagAncestorDifferencer.h>

namespace hoot
{

class TagAncestorDifferencerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(TagAncestorDifferencerTest);
  CPPUNIT_TEST(compareRailwaysTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void compareRailwaysTest()
  {
    TagAncestorDifferencer uut("public_transport=platform");

    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr n1 = std::make_shared<Node>(Status::Unknown1, 1, 0, 0, 15.0);
    Tags t1;
    t1["railway"] = "platform";
    t1["highway"] = "primary";
    n1->setTags(t1);
    map->addNode(n1);

    NodePtr n2 = std::make_shared<Node>(Status::Unknown2, 2, 0, 0, 15.0));
    Tags t2;
    t2["public_transport"] = "platform";
    t2["train"] = "yes";
    n2->setTags(t2);
    map->addNode(n2);

    NodePtr n3(new Node(Status::Unknown2, 2, 0, 0, 15.0));
    Tags t3;
    t3["public_transport"] = "platform";
    t3["bus"] = "yes";
    t3["highway"] = "secondary";
    n3->setTags(t3);
    map->addNode(n3);

    NodePtr n4(new Node(Status::Unknown2, 2, 0, 0, 15.0));
    Tags t4;
    t4["public_transport"] = "platform";
    t4["bus"] = "yes";
    t4["train"] = "yes";
    n4->setTags(t4);
    map->addNode(n4);

    // railway=platform doesn't share an ancestor
    HOOT_STR_EQUALS(1, uut.diff(map, n1, n2));
    HOOT_STR_EQUALS(1, uut.diff(map, n1, n3));
    // shared ancestor and slight similarity
    HOOT_STR_EQUALS(0.68, uut.diff(map, n2, n3));
    // shared ancestor and exactly the same
    HOOT_STR_EQUALS(0, uut.diff(map, n2, n4));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TagAncestorDifferencerTest, "quick");

}
