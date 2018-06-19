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

//  Hoot
#include <hoot/core/io/OsmChangesetElement.h>
#include <hoot/core/util/FileUtils.h>

#include "../TestUtils.h"

namespace hoot
{

class OsmChangesetElementTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmChangesetElementTest);
  CPPUNIT_TEST(runXmlNodeTest);
  CPPUNIT_TEST(runNonAsciiXmlNodeTest);
  CPPUNIT_TEST(runXmlWayTest);
  CPPUNIT_TEST(runXmlRelationTest);
  CPPUNIT_TEST(runXmlChangesetTest);
  CPPUNIT_TEST(runXmlChangesetJoinTest);
  CPPUNIT_TEST(runXmlChangesetUpdateTest);
  CPPUNIT_TEST(runXmlChangesetSplitTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
  }

  void runXmlNodeTest()
  {
    QXmlStreamAttributes attributes;
    attributes.append("id", "-1");
    attributes.append("version", "0");
    attributes.append("lat", "38.8549321261880536");
    attributes.append("lon", "-104.8979050333482093");
    XmlObject n;
    n.first = "node";
    n.second = attributes;
    XmlNode node(n, NULL);

    HOOT_STR_EQUALS("\t\t<node id=\"-1\" version=\"0\" "
                    "lat=\"38.8549321261880536\" lon=\"-104.8979050333482093\" changeset=\"1\"/>\n",
                    node.toString(1));

    QXmlStreamAttributes tagAttributes;
    tagAttributes.append("k", "name");
    tagAttributes.append("v", "node name");
    XmlObject nameTag;
    nameTag.first = "tag";
    nameTag.second = tagAttributes;
    node.addTag(nameTag);

    HOOT_STR_EQUALS("\t\t<node id=\"-1\" version=\"0\" "
                    "lat=\"38.8549321261880536\" lon=\"-104.8979050333482093\" changeset=\"1\">\n"
                    "\t\t\t<tag k=\"name\" v=\"node name\"/>\n"
                    "\t\t</node>\n",
                    node.toString(1));
  }

  void runNonAsciiXmlNodeTest()
  {
    QXmlStreamAttributes attributes;
    attributes.append("id", "-1");
    attributes.append("version", "0");
    attributes.append("lat", "11.6021625999999998");
    attributes.append("lon", "43.1529881000000017");
    XmlObject n;
    n.first = "node";
    n.second = attributes;
    XmlNode node(n, NULL);
    //  Name tags taken from OSM node in Djibouti
    QXmlStreamAttributes tagAttributesAr;
    tagAttributesAr.append("k", "name:ar");
    tagAttributesAr.append("v", "سفارة جمهورية مصر العربية");
    QXmlStreamAttributes tagAttributesZh;
    tagAttributesZh.append("k", "name:zh");
    tagAttributesZh.append("v", "埃及大使馆");
    QXmlStreamAttributes tagAttributesFr;
    tagAttributesFr.append("k", "name:fr");
    tagAttributesFr.append("v", "Ambassade de l'Égypte");
    XmlObject nameTagAr;
    nameTagAr.first = "tag";
    nameTagAr.second = tagAttributesAr;
    node.addTag(nameTagAr);
    XmlObject nameTagZh;
    nameTagZh.first = "tag";
    nameTagZh.second = tagAttributesZh;
    node.addTag(nameTagZh);
    XmlObject nameTagFr;
    nameTagFr.first = "tag";
    nameTagFr.second = tagAttributesFr;
    node.addTag(nameTagFr);

    HOOT_STR_EQUALS("\t\t<node id=\"-1\" version=\"0\" "
                    "lat=\"38.8549321261880536\" lon=\"-104.8979050333482093\" changeset=\"1\">\n"
                    "\t\t\t<tag k=\"name:ar\" v=\"سفارة جمهورية مصر العربية\"/>\n"
                    "\t\t\t<tag k=\"name:ar\" v=\"埃及大使馆\"/>\n"
                    "\t\t\t<tag k=\"name:ar\" v=\"Ambassade de l'Égypte\"/>\n"
                    "\t\t</node>\n",
                    node.toString(1));
  }

  void runXmlWayTest()
  {
    QXmlStreamAttributes attributes;
    attributes.append("id", "-1");
    attributes.append("version", "0");
    attributes.append("timestamp", "");
    XmlObject w;
    w.first = "way";
    w.second = attributes;
    XmlWay way(w, NULL);

    HOOT_STR_EQUALS("\t\t<way id=\"-1\" version=\"0\" timestamp=\"\" changeset=\"1\">\n\t\t</way>\n",
                    way.toString(1));

    way.addNode(-1);
    way.addNode(-2);

    HOOT_STR_EQUALS("\t\t<way id=\"-1\" version=\"0\" timestamp=\"\" changeset=\"1\">\n"
                    "\t\t\t<nd ref=\"-1\"/>\n"
                    "\t\t\t<nd ref=\"-2\"/>\n"
                    "\t\t</way>\n",
                    way.toString(1));

    QXmlStreamAttributes tagAttributes;
    tagAttributes.append("k", "name");
    tagAttributes.append("v", "way name");
    XmlObject nameTag;
    nameTag.first = "tag";
    nameTag.second = tagAttributes;
    way.addTag(nameTag);

    HOOT_STR_EQUALS("\t\t<way id=\"-1\" version=\"0\" timestamp=\"\" changeset=\"1\">\n"
                    "\t\t\t<nd ref=\"-1\"/>\n"
                    "\t\t\t<nd ref=\"-2\"/>\n"
                    "\t\t\t<tag k=\"name\" v=\"way name\"/>\n"
                    "\t\t</way>\n",
                    way.toString(1));
  }

  void runXmlRelationTest()
  {
    QXmlStreamAttributes attributes;
    attributes.append("id", "-1");
    attributes.append("version", "0");
    attributes.append("timestamp", "");
    XmlObject w;
    w.first = "relation";
    w.second = attributes;
    XmlRelation relation(w, NULL);

    HOOT_STR_EQUALS("\t\t<relation id=\"-1\" version=\"0\" timestamp=\"\" changeset=\"1\">\n\t\t</relation>\n",
                    relation.toString(1));

    QXmlStreamAttributes member1;
    member1.append("type", "way");
    member1.append("ref", "-1");
    member1.append("role", "outer");
    relation.addMember(member1);
    QXmlStreamAttributes member2;
    member2.append("type", "way");
    member2.append("ref", "-2");
    member2.append("role", "inner");
    relation.addMember(member2);
    QXmlStreamAttributes tagAttributes;
    tagAttributes.append("k", "type");
    tagAttributes.append("v", "multipolygon");
    XmlObject nameTag;
    nameTag.first = "tag";
    nameTag.second = tagAttributes;
    relation.addTag(nameTag);

    HOOT_STR_EQUALS("\t\t<relation id=\"-1\" version=\"0\" timestamp=\"\" changeset=\"1\">\n"
                    "\t\t\t<member type=\"way\" ref=\"-1\" role=\"outer\"/>\n"
                    "\t\t\t<member type=\"way\" ref=\"-2\" role=\"inner\"/>\n"
                    "\t\t\t<tag k=\"type\" v=\"multipolygon\"/>\n"
                    "\t\t</relation>\n",
                    relation.toString(1));
  }

  void runXmlChangesetTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmChangesetElementTest/ToyTestAInput.osc");

    QString expectedText = FileUtils::readFully("test-files/io/OsmChangesetElementTest/ToyTestAChangeset1.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetJoinTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmXmlChangesetFileWriterTest/changeset.split.osc");
    changeset.loadChangeset("test-files/io/OsmXmlChangesetFileWriterTest/changeset-001.split.osc");

    QString expectedText = FileUtils::readFully("test-files/io/OsmChangesetElementTest/ChangesetMergeExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetUpdateTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmChangesetElementTest/ToyTestAInput.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    QString update =
        "<diffResult generator=\"OpenStreetMap Server\" version=\"0.6\">\n"
        "  <node old_id=\"-1\" new_id=\"1\" new_version=\"1\"/>\n"
        "  <node old_id=\"-2\" new_id=\"2\" new_version=\"1\"/>\n"
        "  <node old_id=\"-32\" new_id=\"32\" new_version=\"1\"/>\n"
        "  <node old_id=\"-7\" new_id=\"7\" new_version=\"1\"/>\n"
        "  <node old_id=\"-8\" new_id=\"8\" new_version=\"1\"/>\n"
        "  <node old_id=\"-33\" new_id=\"33\" new_version=\"1\"/>\n"
        "  <way old_id=\"-1\" new_id=\"1\" new_version=\"1\"/>\n"
        "  <way old_id=\"-2\" new_id=\"2\" new_version=\"1\"/>\n"
        "</diffResult>";

    changeset.updateChangeset(update);

    QString expectedText = FileUtils::readFully("test-files/io/OsmChangesetElementTest/ToyTestAChangeset2.osc");

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 2));
  }

  void runXmlChangesetSplitTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmChangesetElementTest/ToyTestAInput.osc");

    changeset.setMaxSize(5);

    QStringList expectedFiles;
    expectedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit1.osc");
    expectedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit2.osc");
    expectedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit3.osc");

    QStringList updatedFiles;
    updatedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit1.response.xml");
    updatedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit2.response.xml");
    updatedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit3.response.xml");

    ChangesetInfoPtr info;
    int i = 1;
    while (!changeset.isDone())
    {
      info.reset(new ChangesetInfo());
      changeset.calculateChangeset(info);

      QString expectedText = FileUtils::readFully(expectedFiles[i - 1]);

      HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, i));

      QString updatedText = FileUtils::readFully(updatedFiles[i - 1]);
      changeset.updateChangeset(updatedText);

      ++i;
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmChangesetElementTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmChangesetElementTest, "current");

}
