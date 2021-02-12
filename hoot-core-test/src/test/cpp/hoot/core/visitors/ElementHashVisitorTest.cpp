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
 * @copyright Copyright (C) 2014, 2017, 2018, 2020 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/visitors/ElementHashVisitor.h>

namespace hoot
{

/*
 * This is testing the hash writing. The hash collecting is being tested via
 * ServiceChangesetReplacement* tests.
 */
class ElementHashVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementHashVisitorTest);
  CPPUNIT_TEST(runNodeTest);
  CPPUNIT_TEST(runNodeCeTest);
  CPPUNIT_TEST(runWayTest);
  CPPUNIT_TEST(runRelationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runNodeTest()
  {
    ElementHashVisitor uut;

    NodePtr n1(new Node(Status::Unknown1, -1, -104.8852148123, 38.8467218123, 5));
    n1->getTags()["highway"] = "bus_stop";
    n1->getTags()["name"] = "Bus Stop 1";
    n1->getTags()["source"] = "imagery";

    HOOT_STR_EQUALS(
      "{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"},\"x\":-104.8852148,\"y\":38.8467218}",
      uut.toJson(n1));

    HOOT_STR_EQUALS(
      "4e3cca7fb2117486cd06c5b2d0c5de5730f0de55",
      QString::fromUtf8(uut.toHash(n1).toHex().data()));
  }

  void runNodeCeTest()
  {
    ElementHashVisitor uut;
    uut.setIncludeCircularError(true);

    NodePtr n1(new Node(Status::Unknown1, -1, -104.8852148123, 38.8467218123, 5));
    n1->getTags()["highway"] = "bus_stop";
    n1->getTags()["name"] = "Bus Stop 1";
    n1->getTags()["source"] = "imagery";

    HOOT_STR_EQUALS(
      "{\"type\":\"node\",\"tags\":{\"error:circular\":\"5\",\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"},\"x\":-104.8852148,\"y\":38.8467218}",
      uut.toJson(n1));

    // validated here: http://onlinemd5.com/
    // for some reason the commandline sha1sum gives a different result.
    HOOT_STR_EQUALS(
      "c954370caabde21e9c5f4c5c9c7f333b44625615",
      QString::fromUtf8(uut.toHash(n1).toHex().data()));
  }

  void runWayTest()
  {
    ElementHashVisitor uut;
    OsmMapPtr map(new OsmMap());
    uut.setOsmMap(map.get());

    NodePtr n1(new Node(Status::Unknown1, -1, -104.8852148123, 38.8467218123, 5));
    n1->getTags()["highway"] = "bus_stop";
    n1->getTags()["name"] = "Bus Stop 1";
    n1->getTags()["source"] = "imagery";
    map->addNode(n1);
    NodePtr n2(new Node(Status::Unknown1, -2, -104.8852158123, 38.8467228123, 15));
    n2->getTags()["highway"] = "bus_stop";
    n2->getTags()["name"] = "Bus Stop 2";
    n2->getTags()["source"] = "imagery";
    map->addNode(n2);
    WayPtr way(new Way(Status::Unknown1, -1, 15.0));
    way->getTags().set("area", "yes");
    way->addNode(n1->getId());
    way->addNode(n2->getId());
    map->addWay(way);

    HOOT_STR_EQUALS(
      "{\"type\":\"way\",\"tags\":{\"area\":\"yes\"},\"nodes\":[{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"},\"x\":-104.8852148,\"y\":38.8467218},{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 2\"},\"x\":-104.8852158,\"y\":38.8467228}]}",
      uut.toJson(way));

    HOOT_STR_EQUALS(
      "9f3f100a3ebf767d6cdd61c4340c121d7d47ecb6",
      QString::fromUtf8(uut.toHash(way).toHex().data()));
  }

  void runRelationTest()
  {
    ElementHashVisitor uut;
    OsmMapPtr map(new OsmMap());
    uut.setOsmMap(map.get());

    NodePtr n1(new Node(Status::Unknown1, -1, -104.8852148123, 38.8467218123, 5));
    n1->getTags()["highway"] = "bus_stop";
    n1->getTags()["name"] = "Bus Stop 1";
    n1->getTags()["source"] = "imagery";
    map->addNode(n1);
    NodePtr n2(new Node(Status::Unknown1, -2, -104.8852158123, 38.8467228123, 15));
    n2->getTags()["highway"] = "bus_stop";
    n2->getTags()["name"] = "Bus Stop 2";
    n2->getTags()["source"] = "imagery";
    map->addNode(n2);
    WayPtr way(new Way(Status::Unknown1, -1, 15.0));
    way->getTags().set("area", "yes");
    way->addNode(n1->getId());
    way->addNode(n2->getId());
    map->addWay(way);
    RelationPtr relation(new Relation(Status::Unknown1, -1, 15, "route"));
    relation->addElement("test1", n1->getElementId());
    relation->addElement("test1", n2->getElementId());
    relation->addElement("test2", way->getElementId());

    HOOT_STR_EQUALS(
      "{\"type\":\"relation\",\"tags\":{},\"members\":[{\"type\":\"node\",\"role\":\"test1\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"},\"x\":-104.8852148,\"y\":38.8467218},{\"type\":\"node\",\"role\":\"test1\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 2\"},\"x\":-104.8852158,\"y\":38.8467228},{\"type\":\"way\",\"role\":\"test2\",\"tags\":{\"area\":\"yes\"},\"nodes\":[{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"},\"x\":-104.8852148,\"y\":38.8467218},{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 2\"},\"x\":-104.8852158,\"y\":38.8467228}]}]}",
      uut.toJson(relation));

    HOOT_STR_EQUALS(
      "bf9c3b39f8bef543898f5f542fc9be3f9b010c7f",
      QString::fromUtf8(uut.toHash(relation).toHex().data()));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementHashVisitorTest, "quick");

}


