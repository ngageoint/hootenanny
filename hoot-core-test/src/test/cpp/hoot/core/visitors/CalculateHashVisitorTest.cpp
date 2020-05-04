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
 * @copyright Copyright (C) 2014, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/visitors/CalculateHashVisitor.h>

namespace hoot
{

class CalculateHashVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(CalculateHashVisitorTest);
  CPPUNIT_TEST(runNodeTest);
  CPPUNIT_TEST(runNodeCeTest);
  CPPUNIT_TEST(runWayTest);
  CPPUNIT_TEST(runRelationTest);
  // TODO: finish
  //CPPUNIT_TEST(runVisitWriteTest);
  //CPPUNIT_TEST(runVisitCollectTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runNodeTest()
  {
    CalculateHashVisitor uut;

    NodePtr n1(new Node(Status::Unknown1, -1, -104.8852148123, 38.8467218123, 5));
    n1->getTags()["highway"] = "bus_stop";
    n1->getTags()["name"] = "Bus Stop 1";
    n1->getTags()["source"] = "imagery";

    HOOT_STR_EQUALS(
      "{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[-104.8852148,38.8467218]}}",
      uut.toJson(n1));

    HOOT_STR_EQUALS(
      "bfdbfd357e4ad418aef22c78caf3148e18263a66",
      QString::fromUtf8(uut.toHash(n1).toHex().data()));
  }

  void runNodeCeTest()
  {
    CalculateHashVisitor uut;
    uut.setIncludeCircularError(true);

    NodePtr n1(new Node(Status::Unknown1, -1, -104.8852148123, 38.8467218123, 5));
    n1->getTags()["highway"] = "bus_stop";
    n1->getTags()["name"] = "Bus Stop 1";
    n1->getTags()["source"] = "imagery";

    HOOT_STR_EQUALS(
      "{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{\"error:circular\":\"5\",\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[-104.8852148,38.8467218]}}",
      uut.toJson(n1));

    // validated here: http://onlinemd5.com/
    // for some reason the commandline sha1sum gives a different result.
    HOOT_STR_EQUALS(
      "746c440be4fc11f9631d62d26cbba6aa97f73e3c",
      QString::fromUtf8(uut.toHash(n1).toHex().data()));
  }

  void runWayTest()
  {
    CalculateHashVisitor uut;
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
      "{\"type\":\"Feature\",\"properties\":{\"type\":\"way\",\"tags\":{\"area\":\"yes\"},\"nodes\":[{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[-104.8852148,38.8467218]}},{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 2\"}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[-104.8852158,38.8467228]}}]}}",
      uut.toJson(way));

    HOOT_STR_EQUALS(
      "172de51b420b6fd0eed44fc8932000c7e2b4d612",
      QString::fromUtf8(uut.toHash(way).toHex().data()));
  }

  void runRelationTest()
  {
    CalculateHashVisitor uut;
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
      "{\"type\":\"Feature\",\"properties\":{\"type\":\"relation\",\"tags\":{},\"members\":[{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[-104.8852148,38.8467218]}},{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 2\"}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[-104.8852158,38.8467228]}},{\"type\":\"Feature\",\"properties\":{\"type\":\"way\",\"tags\":{\"area\":\"yes\"},\"nodes\":[{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 1\"}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[-104.8852148,38.8467218]}},{\"type\":\"Feature\",\"properties\":{\"type\":\"node\",\"tags\":{\"highway\":\"bus_stop\",\"name\":\"Bus Stop 2\"}},\"geometry\":{\"type\":\"Point\",\"coordinates\":[-104.8852158,38.8467228]}}]}}]}}",
      uut.toJson(relation));

    HOOT_STR_EQUALS(
      "18a81b4d4a9472fc52d3487aea7cb675765d102f",
      QString::fromUtf8(uut.toHash(relation).toHex().data()));
  }

//  void runVisitWriteTest()
//  {

//  }

//  void runVisitCollectTest()
//  {

//  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CalculateHashVisitorTest, "quick");

}


