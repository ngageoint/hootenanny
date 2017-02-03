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
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot-core-test/src/test/cpp/hoot/core/TestUtils.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDir>

namespace hoot
{

class OsmJsonReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmJsonReaderTest);
  CPPUNIT_TEST(nodeTest);
  CPPUNIT_TEST(wayTest);
  CPPUNIT_TEST(relationTest);
  CPPUNIT_TEST(urlTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void nodeTest()
  {
    QString testJsonStr =
     "{                                      \n"
     " 'version': 0.6,                       \n"
     " 'generator': 'Overpass API',          \n"
     " 'osm3s': {                            \n"
     "   'timestamp_osm_base': 'date',       \n"
     "   'copyright': 'c 1999'               \n"
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

    // Need to test read from file, too
    OsmMapPtr pMap2 = uut.loadFromFile("test-files/nodes.json");

    // Test against osm xml
    QString testOsmStr =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<osm version=\"0.6\">\n"
      "    <node visible=\"true\" id=\"-3\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"4.0\" lon=\"-3.0\">\n"
      "        <tag k=\"name\" v=\"Micah's Street\"/>\n"
      "        <tag k=\"highway\" v=\"bus_stop\"/>\n"
      "        <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"15\"/>\n"
      "    </node>\n"
      "    <node visible=\"true\" id=\"-2\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"3.0\" lon=\"-3.0\"/>\n"
      "    <node visible=\"true\" id=\"-1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"2.0\" lon=\"-3.0\"/>\n"
      "</osm>\n";

    OsmMapPtr pTestMap(new OsmMap());
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.readFromString(testOsmStr, pTestMap);

    CPPUNIT_ASSERT(TestUtils::compareMaps(pMap, pTestMap));
    CPPUNIT_ASSERT(TestUtils::compareMaps(pMap2, pTestMap));
  }

  void wayTest()
  {
    QString testJsonStr = "{\"elements\": [\n"
                          "{\"type\":\"node\",\"id\":-1,\"lat\":0,\"lon\":0},\n"
                          "{\"type\":\"node\",\"id\":-2,\"lat\":0,\"lon\":20},\n"
                          "{\"type\":\"node\",\"id\":-3,\"lat\":20,\"lon\":21},\n"
                          "{\"type\":\"node\",\"id\":-4,\"lat\":20,\"lon\":0},\n"
                          "{\"type\":\"node\",\"id\":-5,\"lat\":0,\"lon\":0},\n"
                          "{\"type\":\"way\",\"id\":-1,\"nodes\":[-1,-2,-3,-4,-5],\"tags\":{\"note\":\"w1\",\"alt_name\":\"bar\",\"name\":\"foo\",\"area\":\"yes\",\"amenity\":\"bar\",\"" + MetadataTags::ErrorCircular() + "\":\"5\"}}]\n"
                          "}\n";
    OsmJsonReader uut;
    OsmMapPtr pMap = uut.loadFromString(testJsonStr);

    // Test against osm xml
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
      "        <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"5\"/>\n"
      "        <tag k=\"alt_name\" v=\"bar\"/>\n"
      "        <tag k=\"name\" v=\"foo\"/>\n"
      "        <tag k=\"area\" v=\"yes\"/>\n"
      "        <tag k=\"amenity\" v=\"bar\"/>\n"
      "    </way>\n"
      "</osm>\n";

    OsmMapPtr pTestMap(new OsmMap());
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.readFromString(testOsmStr, pTestMap);

    CPPUNIT_ASSERT(TestUtils::compareMaps(pMap, pTestMap));
  }

  void relationTest()
  {
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
      "   'type': 'node',                      \n"
      "   'id': -1,                            \n"
      "   'lat': 0,                            \n"
      "   'lon': 0                             \n"
      " },                                     \n"
      " {                                      \n"
      "   'type': 'node',                      \n"
      "   'id': -2,                            \n"
      "   'lat': 0,                            \n"
      "   'lon': 20                            \n"
      " },                                     \n"
      " {                                      \n"
      "   'type': 'node',                      \n"
      "   'id': -3,                            \n"
      "   'lat': 20,                           \n"
      "   'lon': 21                            \n"
      " },                                     \n"
      " {                                      \n"
      "   'type': 'node',                      \n"
      "   'id': -4,                            \n"
      "   'lat': 20,                           \n"
      "   'lon': 0                             \n"
      " },                                     \n"
      " {                                      \n"
      "   'type': 'node',                      \n"
      "   'id': -5,                            \n"
      "   'lat': 0,                            \n"
      "   'lon': 0                             \n"
      " },                                     \n"
      " {                                      \n"
      "   'type': 'way',                       \n"
      "   'id': -1,                            \n"
      "   'nodes': [-1,-2,-3,-4,-5],           \n"
      "   'tags': {                            \n"
      "     'note': 'w1',                      \n"
      "     'alt_name': 'bar',                 \n"
      "     'name': 'foo',                     \n"
      "     'area': 'yes',                     \n"
      "     'amenity': 'bar',                  \n"
      "     '" + MetadataTags::ErrorCircular() + "': '5'\n"
      "   }                                    \n"
      " },                                     \n"
      " {                                      \n"
      "   'type': 'relation',                  \n"
      "   'id': 1745069,                       \n"
      "   'members': [                         \n"
      "     {                                  \n"
      "       'type': 'way',                   \n"
      "       'ref': -1,                       \n"
      "       'role': ''                       \n"
      "     }                                  \n"
      "   ]                                    \n"
      " },                                     \n"
      " {                                      \n"
      "   'type': 'relation',                  \n"
      "   'id': 172789                         \n"
      " },                                     \n"
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
      "     'via': 'Ramersdorf'                \n"
      "   }                                    \n"
      " }                                      \n"
      " ]                                      \n"
      "}                                       \n";

    OsmJsonReader uut;
    OsmMapPtr pMap = uut.loadFromString(testJsonStr);

    // Useful for debug
    //OsmXmlWriter writer;
    //writer.setIncludeIds(true);
    //writer.write(pMap, "/tmp/test.osm");

    // Test against osm xml
    QString testOsmStr =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>                                                                       \n"
      "<osm version=\"0.6\">                                                                                            \n"
      "  <node visible=\"true\" id=\"-5\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"0.00\" lon=\"0.00\"/>  \n"
      "  <node visible=\"true\" id=\"-4\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"20.0\" lon=\"0.00\"/>  \n"
      "  <node visible=\"true\" id=\"-3\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"20.0\" lon=\"21.0\"/>  \n"
      "  <node visible=\"true\" id=\"-2\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"0.00\" lon=\"20.0\"/>  \n"
      "  <node visible=\"true\" id=\"-1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"0.00\" lon=\"0.00\"/>  \n"
      "  <way  visible=\"true\" id=\"-1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">                             \n"
      "      <nd ref=\"-1\"/>                                                                                           \n"
      "      <nd ref=\"-2\"/>                                                                                           \n"
      "      <nd ref=\"-3\"/>                                                                                           \n"
      "      <nd ref=\"-4\"/>                                                                                           \n"
      "      <nd ref=\"-5\"/>                                                                                           \n"
      "      <tag k=\"note\" v=\"w1\"/>                                                                                 \n"
      "      <tag k=\"alt_name\" v=\"bar\"/>                                                                            \n"
      "      <tag k=\"name\" v=\"foo\"/>                                                                                \n"
      "      <tag k=\"area\" v=\"yes\"/>                                                                                \n"
      "      <tag k=\"amenity\" v=\"bar\"/>                                                                             \n"
      "      <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"5\"/>                                                 \n"
      "  </way>                                                                                                         \n"
      "  <relation visible=\"true\" id=\"1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">                          \n"
      "      <member type=\"relation\" ref=\"1745069\" role=\"\"/>                                                      \n"
      "      <member type=\"relation\" ref=\"172789\" role=\"\"/>                                                       \n"
      "      <tag k=\"network\" v=\"VRS\"/>                                                                             \n"
      "      <tag k=\"route\" v=\"bus\"/>                                                                               \n"
      "      <tag k=\"operator\" v=\"SWB\"/>                                                                            \n"
      "      <tag k=\"via\" v=\"Ramersdorf\"/>                                                                          \n"
      "      <tag k=\"from\" v=\"Konrad-Adenauer-Platz\"/>                                                              \n"
      "      <tag k=\"to\" v=\"Gielgen\"/>                                                                              \n"
      "      <tag k=\"name\" v=\"VRS 636\"/>                                                                            \n"
      "      <tag k=\"ref\" v=\"636\"/>                                                                                 \n"
      "      <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"15\"/>                                                \n"
      "  </relation>                                                                                                    \n"
      "  <relation visible=\"true\" id=\"172789\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">                     \n"
      "      <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"15\"/>                                                \n"
      "  </relation>                                                                                                    \n"
      "  <relation visible=\"true\" id=\"1745069\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">                    \n"
      "      <member type=\"way\" ref=\"-1\" role=\"\"/>                                                                \n"
      "      <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"15\"/>                                                \n"
      "  </relation>                                                                                                    \n"
      "</osm>                                                                                                           \n";

    OsmMapPtr pTestMap(new OsmMap());
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.readFromString(testOsmStr, pTestMap);

    CPPUNIT_ASSERT(TestUtils::compareMaps(pMap, pTestMap));
  }

  // Try hitting the network to get some data...
  void urlTest()
  {
    OsmMapPtr pMap;
    QString urlNodes = "http://overpass-api.de/api/interpreter?data=[out:json];node(35.20,-120.59,35.21,-120.58);out;";
    QString urlWays  = "http://overpass-api.de/api/interpreter?data=[out:json];way(35.20,-120.59,35.21,-120.58);out;";
    OsmJsonReader uut;

    const uint32_t RETRY_SECS = 30;
    const uint32_t MAX_TRIES = 5;
    uint32_t numTries = 0;
    bool success = false;

    // Try a few times, to handle server timeouts
    while (!success && numTries < MAX_TRIES)
    {
      try
      {
        // Get clean map
        pMap.reset(new OsmMap());

        // Get Nodes
        CPPUNIT_ASSERT(uut.isSupported(urlNodes));
        uut.open(urlNodes);
        uut.read(pMap);

        // Get Ways
        CPPUNIT_ASSERT(uut.isSupported(urlWays));
        uut.open(urlWays);
        uut.read(pMap);
        success = true;
      }
      catch (HootException ex)
      {
        numTries++;
        LOG_INFO(ex.getWhat() + QString(" will retry after %1 seconds").arg(RETRY_SECS));
        sleep(RETRY_SECS);
      }
    }
    CPPUNIT_ASSERT(success);

    // File URL
    QString fname = QDir::currentPath() + "/test-files/nodes.json";
    QString urlFile = "file://" + fname;
    OsmMapPtr pMap2(new OsmMap());
    CPPUNIT_ASSERT(uut.isSupported(urlFile));
    uut.open(urlFile);
    uut.read(pMap2);

    // Not failure == success
  }
}; // class OsmJsonReaderTest
} // namespace hoot

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::OsmJsonReaderTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::OsmJsonReaderTest, "slow");

