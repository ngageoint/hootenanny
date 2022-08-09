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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/ExactTagDifferencer.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

class ExactTagDifferencerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ExactTagDifferencerTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    ExactTagDifferencer uut;
    OsmMapPtr map = std::make_shared<OsmMap>();

    NodePtr n1 = std::make_shared<Node>(Status::Unknown1, 1, 0, 0, 15.0);
    Tags t1;
    t1["railway"] = "platform";
    t1["highway"] = "primary";
    n1->setTags(t1);
    map->addNode(n1);

    NodePtr n2 = std::make_shared<Node>(Status::Unknown1, 2, 0, 0, 15.0);
    Tags t2;
    t2["railway"] = "platform";
    t2["highway"] = "primary";
    n2->setTags(t2);
    map->addNode(n2);

    NodePtr n3 = std::make_shared<Node>(Status::Unknown1, 3, 0, 0, 15.0);
    Tags t3;
    t3["railway"] = "platform";
    n3->setTags(t3);
    map->addNode(n3);

    NodePtr n4 = std::make_shared<Node>(Status::Unknown1, 4, 0, 0, 15.0);
    Tags t4;
    t4["highway"] = "primary";
    n4->setTags(t4);
    map->addNode(n4);

    NodePtr n5 = std::make_shared<Node>(Status::Unknown1, 5, 0, 0, 15.0);
    Tags t5;
    t5["public_transport"] = "platform";
    n5->setTags(t5);
    map->addNode(n5);

    NodePtr n6 = std::make_shared<Node>(Status::Unknown1, 6, 0, 0, 15.0);
    Tags t6;
    t6[MetadataTags::HootStatus()] = "Unknown2";
    n6->setTags(t6);
    map->addNode(n6);

    NodePtr n7 = std::make_shared<Node>(Status::Unknown1, 7, 0, 0, 15.0);
    Tags t7;
    t7[MetadataTags::HootStatus()] = "Unknown2";
    n7->setTags(t7);
    map->addNode(n7);

    NodePtr n8 = std::make_shared<Node>(Status::Unknown1, 8, 0, 0, 15.0);
    Tags t8;
    t8[MetadataTags::HootStatus()] = "1";
    n8->setTags(t8);
    map->addNode(n8);

    NodePtr n9 = std::make_shared<Node>(Status::Unknown1, 9, 0, 0, 15.0);
    Tags t9;
    t9["monitoring:water_level"] = "yes";
    t9[MetadataTags::HootStatus()] = "1";
    t9["note"] = "Source 2";
    t9["man_made"] = "monitoring_station";
    n9->setTags(t9);
    map->addNode(n9);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.diff(map, n1, n2), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.diff(map, n2, n1), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n1, n3), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n3, n1), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n1, n4), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n4, n1), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n1, n5), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n5, n1), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n1, n6), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n6, n1), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.diff(map, n7, n6), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, uut.diff(map, n6, n7), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n8, n9), 1e-3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, uut.diff(map, n9, n8), 1e-3);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ExactTagDifferencerTest, "quick");

}
