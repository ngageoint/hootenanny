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
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiChangesetElement.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class OsmApiChangesetElementTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiChangesetElementTest);
  CPPUNIT_TEST(runXmlNodeTest);
  CPPUNIT_TEST(runNonAsciiXmlNodeTest);
  CPPUNIT_TEST(runXmlWayTest);
  CPPUNIT_TEST(runXmlRelationTest);
  CPPUNIT_TEST_SUITE_END();

public:

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
    tagAttributesAr.append("v", QString::fromUtf8("سفارة جمهورية مصر العربية"));
    QXmlStreamAttributes tagAttributesZh;
    tagAttributesZh.append("k", "name:zh");
    tagAttributesZh.append("v", QString::fromUtf8("埃及大使馆"));
    QXmlStreamAttributes tagAttributesFr;
    tagAttributesFr.append("k", "name:fr");
    tagAttributesFr.append("v", QString::fromUtf8("Ambassade de l'Égypte"));
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
                    "lat=\"11.6021625999999998\" lon=\"43.1529881000000017\" changeset=\"1\">\n"
                    "\t\t\t<tag k=\"name:ar\" v=\"سفارة جمهورية مصر العربية\"/>\n"
                    "\t\t\t<tag k=\"name:zh\" v=\"埃及大使馆\"/>\n"
                    "\t\t\t<tag k=\"name:fr\" v=\"Ambassade de l'Égypte\"/>\n"
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

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetElementTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetElementTest, "current");

}
