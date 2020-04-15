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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

namespace hoot
{

class ApiTagTruncateVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ApiTagTruncateVisitorTest);
  CPPUNIT_TEST(runUUIDTest);
  CPPUNIT_TEST(runDatesTest);
  CPPUNIT_TEST(runListTest);
  CPPUNIT_TEST(runTruncateTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ApiTagTruncateVisitorTest()
  {
  }

  void runUUIDTest()
  {
    OsmMapPtr map(new OsmMap());
    Tags tags;
    QString value = UuidHelper::createUuid().toString();

    tags["uuid"] = value;
    tags["note"] = "node1";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.appendValue("uuid", UuidHelper::createUuid().toString());
    tags["note"] = "node2";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 1.0, 15.0, tags);

    QStringList uuids;
    for (int i = 0; i < 5; ++i)
      uuids.append(UuidHelper::createUuid().toString());
    tags.appendValue("uuid", uuids);
    tags["note"] = "node3";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 2.0, 15.0, tags);

    ApiTagTruncateVisitor v;
    map->visitRw(v);

    HOOT_STR_EQUALS("Truncated tag key/value pairs for 2 elements", v.getCompletedStatusMessage());

    NodePtr n1 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node1")[0]);
    NodePtr n2 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node2")[0]);
    NodePtr n3 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node3")[0]);

    CPPUNIT_ASSERT_EQUAL(38, n1->getTags()["uuid"].length());
    CPPUNIT_ASSERT_EQUAL(38, n2->getTags()["uuid"].length());
    CPPUNIT_ASSERT_EQUAL(38, n3->getTags()["uuid"].length());
  }

  void runDatesTest()
  {
    OsmMapPtr map(new OsmMap());
    Tags tags;
    QString value = DateTimeUtils::toTimeString(0);

    tags.insert(MetadataTags::SourceDateTime(), value);
    tags[MetadataTags::SourceDateTime()] = value;
    tags["note"] = "node1";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.appendValue(MetadataTags::SourceDateTime(), DateTimeUtils::toTimeString(0));
    tags["note"] = "node2";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 1.0, 15.0, tags);

    QStringList dates;
    for (int i = 0; i < 30; ++i)
      dates.append(DateTimeUtils::toTimeString(0));
    tags.appendValue(MetadataTags::SourceIngestDateTime(), dates);
    tags["note"] = "node3";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 2.0, 15.0, tags);

    ApiTagTruncateVisitor v;
    map->visitRw(v);

    HOOT_STR_EQUALS("Truncated tag key/value pairs for 2 elements", v.getCompletedStatusMessage());

    NodePtr n1 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node1")[0]);
    NodePtr n2 =
       map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node2")[0]);
    NodePtr n3 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node3")[0]);

    CPPUNIT_ASSERT_EQUAL(20, n1->getTags()[MetadataTags::SourceDateTime()].length());
    CPPUNIT_ASSERT_EQUAL(20, n2->getTags()[MetadataTags::SourceDateTime()].length());
    CPPUNIT_ASSERT_EQUAL(20, n3->getTags()[MetadataTags::SourceIngestDateTime()].length());
  }

  void runListTest()
  {
    OsmMapPtr map(new OsmMap());
    Tags tags;
    QString value = "Some random text goes here that is going into a list.";

    tags["some_key"] = value;
    tags["note"] = "node1";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.appendValue("some_key", value);
    tags["note"] = "node2";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 1.0, 15.0, tags);

    QStringList dates;
    for (int i = 0; i < 5; ++i)
      dates.append(value);
    tags.appendValue("some_key", dates);
    tags["note"] = "node3";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 2.0, 15.0, tags);

    ApiTagTruncateVisitor v;
    map->visitRw(v);

    HOOT_STR_EQUALS("Truncated tag key/value pairs for 1 elements", v.getCompletedStatusMessage());

    NodePtr n1 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node1")[0]);
    NodePtr n2 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node2")[0]);
    NodePtr n3 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node3")[0]);

    //  value.length() == 53
    CPPUNIT_ASSERT_EQUAL(53, n1->getTags()["some_key"].length());
    CPPUNIT_ASSERT_EQUAL(107, n2->getTags()["some_key"].length());
    CPPUNIT_ASSERT_EQUAL(215, n3->getTags()["some_key"].length());
  }

  void runTruncateTest()
  {
    OsmMapPtr map(new OsmMap());
    Tags tags;
    QString value(250, 'B');

    tags["some_key"] = value;
    tags["note"] = "node1";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    QString valueTooLong(400, 'C');
    tags["some_key"] = valueTooLong;
    tags["note"] = "node2";
    TestUtils::createNode(map, Status::Unknown1, 0.0, 1.0, 15.0, tags);

    ApiTagTruncateVisitor v;
    map->visitRw(v);

    HOOT_STR_EQUALS("Truncated tag key/value pairs for 1 elements", v.getCompletedStatusMessage());

    NodePtr n1 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node1")[0]);
    NodePtr n2 =
      map->getNode(ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", "node2")[0]);

    CPPUNIT_ASSERT_EQUAL(250, n1->getTags()["some_key"].length());
    CPPUNIT_ASSERT_EQUAL(255, n2->getTags()["some_key"].length());
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ApiTagTruncateVisitorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ApiTagTruncateVisitorTest, "quick");

}
