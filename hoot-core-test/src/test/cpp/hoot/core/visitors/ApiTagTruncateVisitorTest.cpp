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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
  CPPUNIT_TEST(runArabicTest);
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
    NodePtr n1 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.appendValue("uuid", UuidHelper::createUuid().toString());
    NodePtr n2 = TestUtils::createNode(map, Status::Unknown1, 0.0, 1.0, 15.0, tags);

    QStringList uuids;
    for (int i = 0; i < 5; ++i)
      uuids.append(UuidHelper::createUuid().toString());
    tags.appendValue("uuid", uuids);
    NodePtr n3 = TestUtils::createNode(map, Status::Unknown1, 0.0, 2.0, 15.0, tags);

    ApiTagTruncateVisitor v;
    map->visitRw(v);

    HOOT_STR_EQUALS("Truncated tag key/value pairs for 2 elements", v.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(38, n1->getTags()["uuid"].length());
    CPPUNIT_ASSERT_EQUAL(38, n2->getTags()["uuid"].length());
    CPPUNIT_ASSERT_EQUAL(38, n3->getTags()["uuid"].length());
  }

  void runDatesTest()
  {
    OsmMapPtr map(new OsmMap());
    Tags tags;
    QString value = DateTimeUtils::toTimeString(0);
    int dateLength = value.length();

    tags.insert(MetadataTags::SourceDateTime(), value);
    tags[MetadataTags::SourceDateTime()] = value;
    NodePtr n1 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.appendValue(MetadataTags::SourceDateTime(), DateTimeUtils::toTimeString(0));
    NodePtr n2 = TestUtils::createNode(map, Status::Unknown1, 0.0, 1.0, 15.0, tags);

    QStringList dates;
    for (int i = 0; i < 30; ++i)
      dates.append(DateTimeUtils::toTimeString(0));
    tags.appendValue(MetadataTags::SourceIngestDateTime(), dates);
    NodePtr n3 = TestUtils::createNode(map, Status::Unknown1, 0.0, 2.0, 15.0, tags);

    ApiTagTruncateVisitor v;
    map->visitRw(v);

    HOOT_STR_EQUALS("Truncated tag key/value pairs for 2 elements", v.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(dateLength, n1->getTags()[MetadataTags::SourceDateTime()].length());
    CPPUNIT_ASSERT_EQUAL(dateLength, n2->getTags()[MetadataTags::SourceDateTime()].length());
    CPPUNIT_ASSERT_EQUAL(dateLength, n3->getTags()[MetadataTags::SourceIngestDateTime()].length());
  }

  void runListTest()
  {
    int max = ConfigOptions().getMaxTagLength();
    OsmMapPtr map(new OsmMap());
    Tags tags;
    QString value = "Some random text goes here that is going into a list.";
    int length = value.length();

    tags["some_key"] = value;
    NodePtr n1 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags.appendValue("some_key", value);
    NodePtr n2 = TestUtils::createNode(map, Status::Unknown1, 0.0, 1.0, 15.0, tags);

    QStringList values;
    int iterations = (max - 1) / (length + 1);
    for (int i = 0; i <= iterations; ++i)
      values.append(value);
    tags.appendValue("some_key", values);
    NodePtr n3 = TestUtils::createNode(map, Status::Unknown1, 0.0, 2.0, 15.0, tags);

    ApiTagTruncateVisitor v;
    map->visitRw(v);

    HOOT_STR_EQUALS("Truncated tag key/value pairs for 1 elements", v.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(length, n1->getTags()["some_key"].length());
    CPPUNIT_ASSERT_EQUAL(length * 2 + 1, n2->getTags()["some_key"].length());
    CPPUNIT_ASSERT_EQUAL((length * iterations) + (iterations - 1), n3->getTags()["some_key"].length());
  }

  void runTruncateTest()
  {
    int max = ConfigOptions().getMaxTagLength();
    OsmMapPtr map(new OsmMap());
    Tags tags;

    int length = max - 5;
    QString value(length, 'B');

    tags["some_key"] = value;
    NodePtr n1 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    int tooLong = max * 2;
    QString valueTooLong(tooLong, 'C');
    tags["some_key"] = valueTooLong;
    NodePtr n2 = TestUtils::createNode(map, Status::Unknown1, 0.0, 1.0, 15.0, tags);

    ApiTagTruncateVisitor v;
    map->visitRw(v);

    HOOT_STR_EQUALS("Truncated tag key/value pairs for 1 elements", v.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(length, n1->getTags()["some_key"].length());
    CPPUNIT_ASSERT_EQUAL(max, n2->getTags()["some_key"].length());
  }

  void runArabicTest()
  {
    OsmMapPtr map(new OsmMap());
    Tags tags;

    tags["name"] = "&quot;This &amp; that&quot; or other stuff.";
    NodePtr n1 = TestUtils::createNode(map, Status::Unknown1, 0.0, 0.0, 15.0, tags);

    tags["name"] = "ﻉیﻡﺍﺮﺗ ﺏﺎﻏی ﻥیﻥ یﺉﺭی - ﻢﺤﻟ ﻖﺒﻟی ﻊﻣﺍﺮﺗ ﺏﺎﻏ";
    NodePtr n2 = TestUtils::createNode(map, Status::Unknown1, 0.0, 1.0, 15.0, tags);

    //  Length of the following text is 266 and should be truncated
    tags["name"] = "ﻊﻣﺍﺮﺗ ﺏﺎﻏ &quot;ﺹﺎﺤﺑ ﺩیﻭﺎﻧ ﻢﺤﻣﺩ ﻊﻟی ﻡیﺭﺯﺍ&quot; ﺩﺭ ﺖﺑﺭیﺯ کﻩ ﺮﺿﺍ ﺵﺎﻫ ﺐﻫ ﺐﻫﺎﻨﻫ ﻕﺎﺟﺍﺭیﻩ ﺯﺩﺎﺋی ﺂﻧ ﺭﺍ ﺖﺧﺭیﺏ کﺭﺩ ﻭ ﻎﻣ ﺎﻧگیﺰﺗﺭ ﺂﻧ کﻩ ﺰﻧﺩﺎﻧ کﻥﻮﻧی ﺖﺑﺭیﺯ ﺭﻭی ﻭیﺭﺎﻨﻫ ﻪﻣیﻥ ﺱﺎﺨﺘﻣﺎﻧ ﺐﻧﺍ ﺵﺪﻫ ﺎﺴﺗ Təbrizin tarixi Sahib Divan sarayi ki Qacarların izlərini itirmək amacilə sökülmüşdür";
    NodePtr n3 = TestUtils::createNode(map, Status::Unknown1, 1.0, 1.0, 15.0, tags);

    ApiTagTruncateVisitor v;
    Settings s;
    s.set(ConfigOptions().getMaxTagLengthKey(), 255);
    v.setConfiguration(s);
    map->visitRw(v);

    HOOT_STR_EQUALS("&quot;This &amp; that&quot; or other stuff.", n1->getTags()["name"]);
    HOOT_STR_EQUALS("ﻉیﻡﺍﺮﺗ ﺏﺎﻏی ﻥیﻥ یﺉﺭی - ﻢﺤﻟ ﻖﺒﻟی ﻊﻣﺍﺮﺗ ﺏﺎﻏ", n2->getTags()["name"]);
    HOOT_STR_EQUALS("ﻊﻣﺍﺮﺗ ﺏﺎﻏ &quot;ﺹﺎﺤﺑ ﺩیﻭﺎﻧ ﻢﺤﻣﺩ ﻊﻟی ﻡیﺭﺯﺍ&quot; ﺩﺭ ﺖﺑﺭیﺯ کﻩ ﺮﺿﺍ ﺵﺎﻫ ﺐﻫ ﺐﻫﺎﻨﻫ ﻕﺎﺟﺍﺭیﻩ ﺯﺩﺎﺋی ﺂﻧ ﺭﺍ ﺖﺧﺭیﺏ کﺭﺩ ﻭ ﻎﻣ ﺎﻧگیﺰﺗﺭ ﺂﻧ کﻩ ﺰﻧﺩﺎﻧ کﻥﻮﻧی ﺖﺑﺭیﺯ ﺭﻭی ﻭیﺭﺎﻨﻫ ﻪﻣیﻥ ﺱﺎﺨﺘﻣﺎﻧ ﺐﻧﺍ ﺵﺪﻫ ﺎﺴﺗ Təbrizin tarixi Sahib Divan sarayi ki Qacarların izlərini itirmək amacilə s",
                    n3->getTags()["name"]);
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ApiTagTruncateVisitorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ApiTagTruncateVisitorTest, "quick");

}
