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
 * @copyright Copyright (C) 2012, 2013, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/OsmJsonReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/io/OsmReader.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class OsmJsonReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmJsonReaderTest);
  CPPUNIT_TEST(nodeTest);
  CPPUNIT_TEST(wayTest);
  CPPUNIT_TEST(relationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void nodeTest()
  {
    cout << "nodeTest..." << endl;
    QString testJsonStr =
     "{                                      \n"
     " 'version': 0.6,                       \n"
     " 'generator': 'Overpass API',          \n"
     " 'osm3s': {                            \n"
     "   'timestamp_osm_base': 'date',       \n"
     "   'copyright': 'The data included in this document is from www.openstreetmap.org. It has there been collected by a large group of contributors. For individual attribution of each item please refer to http://www.openstreetmap.org/api/0.6/[node|way|relation]/#id/history'\n"
     " },                                    \n"
     " 'elements': [                         \n"
     " {                                     \n"
     "   'type': 'node',                     \n"
     "   'id': -1,                           \n"
     "   'lat': 2.0,                         \n"
     "   'lon': -3.0                         \n"
     " },                                    \n"
     " {                                     \n"
     "   'type': 'node',                     \n"
     "   'id': -2,                           \n"
     "   'lat': 3.0,                         \n"
     "   'lon': -3.0,                        \n"
     "   'timestamp': '2010-01-01T00:00:00Z',\n"
     "   'version': 4,                       \n"
     "   'changeset': 5,                     \n"
     "   'user': 'somebody',                 \n"
     "   'uid': 6                            \n"
     " },                                    \n"
     " {                                     \n"
     "   'type': 'node',                     \n"
     "   'id': -3,                           \n"
     "   'lat': 4.0,                         \n"
     "   'lon': -3.0,                        \n"
     "   'tags': {                           \n"
     "     'highway': 'bus_stop',            \n"
     "     'name': 'Micah\\'s Street'        \n"
     "   }                                   \n"
     " }                                     \n"
     "]                                      \n"
     "}                                      \n";

    OsmJsonReader uut;
    OsmMapPtr pMap = uut.loadFromString(testJsonStr);
    OsmWriter writer;
    writer.write(pMap, "/tmp/NodeTest.osm");

    QString testOsmStr =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<osm version=\"0.6\">\n"
      "    <node visible=\"true\" id=\"-3\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"4.0\" lon=\"-3.0\">\n"
      "        <tag k=\"name\" v=\"Micah's Street\"/>\n"
      "        <tag k=\"highway\" v=\"bus_stop\"/>\n"
      "        <tag k=\"error:circular\" v=\"15\"/>\n"
      "    </node>\n"
      "    <node visible=\"true\" id=\"-2\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"3.0\" lon=\"-3.0\"/>\n"
      "    <node visible=\"true\" id=\"-1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"2.0\" lon=\"-3.0\"/>\n"
      "</osm>\n";

    OsmMapPtr pTestMap(new OsmMap());
    OsmReader reader;
    reader.setUseDataSourceIds(true);
    reader.readFromString(testOsmStr, pTestMap);

    CPPUNIT_ASSERT(TestUtils::compareMaps(pMap, pTestMap));
  }

  void wayTest()
  {
    cout << "wayTest..." << endl;
    QString testJsonStr = "{\"elements\": [\n"
                          "{\"type\":\"node\",\"id\":-1,\"lat\":0,\"lon\":0},\n"
                          "{\"type\":\"node\",\"id\":-2,\"lat\":0,\"lon\":20},\n"
                          "{\"type\":\"node\",\"id\":-3,\"lat\":20,\"lon\":21},\n"
                          "{\"type\":\"node\",\"id\":-4,\"lat\":20,\"lon\":0},\n"
                          "{\"type\":\"node\",\"id\":-5,\"lat\":0,\"lon\":0},\n"
                          "{\"type\":\"way\",\"id\":-1,\"nodes\":[-1,-2,-3,-4,-5],\"tags\":{\"note\":\"w1\",\"alt_name\":\"bar\",\"name\":\"foo\",\"area\":\"yes\",\"amenity\":\"bar\",\"error:circular\":\"5\"}}]\n"
                          "}\n";
    OsmJsonReader uut;
    OsmMapPtr pMap = uut.loadFromString(testJsonStr);
    OsmWriter writer;
    writer.write(pMap, "/tmp/WayTest.osm");

    QString testOsmStr =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<osm version=\"0.6\">\n"
      "    <node visible=\"true\" id=\"-5\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"0.000\" lon=\"0.00\"/>\n"
      "    <node visible=\"true\" id=\"-4\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"20.00\" lon=\"0.00\"/>\n"
      "    <node visible=\"true\" id=\"-3\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"20.00\" lon=\"21.0\"/>\n"
      "    <node visible=\"true\" id=\"-2\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"0.000\" lon=\"20.0\"/>\n"
      "    <node visible=\"true\" id=\"-1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"0.000\" lon=\"0.00\"/>\n"
      "    <way visible=\"true\" id=\"-1\"  timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">\n"
      "        <nd ref=\"-1\"/>\n"
      "        <nd ref=\"-2\"/>\n"
      "        <nd ref=\"-3\"/>\n"
      "        <nd ref=\"-4\"/>\n"
      "        <nd ref=\"-5\"/>\n"
      "        <tag k=\"note\" v=\"w1\"/>\n"
      "        <tag k=\"error:circular\" v=\"5\"/>\n"
      "        <tag k=\"alt_name\" v=\"bar\"/>\n"
      "        <tag k=\"name\" v=\"foo\"/>\n"
      "        <tag k=\"area\" v=\"yes\"/>\n"
      "        <tag k=\"amenity\" v=\"bar\"/>\n"
      "    </way>\n"
      "</osm>\n";

    OsmMapPtr pTestMap(new OsmMap());
    OsmReader reader;
    reader.setUseDataSourceIds(true);
    reader.readFromString(testOsmStr, pTestMap);

    CPPUNIT_ASSERT(TestUtils::compareMaps(pMap, pTestMap));
  }

  void relationTest()
  {
    cout << "relationTest..." << endl;
    QString testJsonStr =
      "{                                       \n"
      " 'version': 0.6,                        \n"
      " 'generator': 'Overpass API',           \n"
      " 'osm3s': {                             \n"
      "   'timestamp_osm_base': 'date',        \n"
      "   'copyright': 'blah blah blah'        \n"
      " },                                     \n"
      " 'elements': [                          \n"
      " {                                      \n"
      "   'type': 'relation',                  \n"
      "   'id': 1,                             \n"
      "   'members': [                         \n"
      "     {                                  \n"
      "       'type': 'relation',              \n"
      "       'ref': 1745069,                  \n"
      "       'role': ''                       \n"
      "     },                                 \n"
      "     {                                  \n"
      "       'type': 'relation',              \n"
      "       'ref': 172789,                   \n"
      "       'role': ''                       \n"
      "     }                                  \n"
      "   ],                                   \n"
      "   'tags': {                            \n"
      "     'from': 'Konrad-Adenauer-Platz',   \n"
      "     'name': 'VRS 636',                 \n"
      "     'network': 'VRS',                  \n"
      "     'operator': 'SWB',                 \n"
      "     'ref': '636',                      \n"
      "     'route': 'bus',                    \n"
      "     'to': 'Gielgen',                   \n"
      "     'type': 'route_master',            \n"
      "     'via': 'Ramersdorf'                \n"
      "   }                                    \n"
      " }                                      \n"
      " ]                                      \n"
      "}                                       \n";

    OsmJsonReader uut;
    OsmMapPtr pMap = uut.loadFromString(testJsonStr);

    OsmWriter writer;
    writer.write(pMap, "/tmp/RelationTest.osm");

    QString testOsmStr =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<osm version=\"0.6\">\n"
      "    <relation visible=\"true\" id=\"1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">\n"
      "        <member type=\"relation\" ref=\"1745069\" role=\"\"/>\n"
      "        <member type=\"relation\" ref=\"172789\" role=\"\"/>\n"
      "        <tag k=\"network\" v=\"VRS\"/>\n"
      "        <tag k=\"route\" v=\"bus\"/>\n"
      "        <tag k=\"operator\" v=\"SWB\"/>\n"
      "        <tag k=\"via\" v=\"Ramersdorf\"/>\n"
      "        <tag k=\"from\" v=\"Konrad-Adenauer-Platz\"/>\n"
      "        <tag k=\"type\" v=\"route_master\"/>\n"
      "        <tag k=\"to\" v=\"Gielgen\"/>\n"
      "        <tag k=\"name\" v=\"VRS 636\"/>\n"
      "        <tag k=\"ref\" v=\"636\"/>\n"
      "        <tag k=\"error:circular\" v=\"15\"/>\n"
      "    </relation>\n"
      "</osm>\n";

    OsmMapPtr pTestMap(new OsmMap());
    OsmReader reader;
    reader.setUseDataSourceIds(true);
    reader.readFromString(testOsmStr, pTestMap);

    CPPUNIT_ASSERT(TestUtils::compareMaps(pMap, pTestMap));
  }
}; // class OsmJsonReaderTest
} // namespace hoot

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::OsmJsonReaderTest, "current");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::OsmJsonReaderTest, "quick");

