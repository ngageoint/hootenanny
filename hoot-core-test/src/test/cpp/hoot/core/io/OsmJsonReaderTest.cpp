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
 * @copyright Copyright (C) 2012, 2013, 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/geometry/GeometryUtils.h>

// Qt
#include <QDir>

namespace hoot
{

class OsmJsonReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmJsonReaderTest);
  CPPUNIT_TEST(nodeTest);
  CPPUNIT_TEST(wayTest);
  CPPUNIT_TEST(relationTest);
  CPPUNIT_TEST(urlTest);
  CPPUNIT_TEST(scrubQuoteTest);
  CPPUNIT_TEST(scrubBigIntsTest);
  CPPUNIT_TEST(isSupportedTest);
  CPPUNIT_TEST(runBoundsTest);
  CPPUNIT_TEST(runBoundsLeaveConnectedOobWaysTest);
  CPPUNIT_TEST(elementTypeUnorderedTest);
  CPPUNIT_TEST(elementTypeUnorderedMissingTest);
  // TODO: add duplicates test
  CPPUNIT_TEST_SUITE_END();

public:

  OsmJsonReaderTest() :
  HootTestFixture("test-files/io/OsmJsonReaderTest/", "test-output/io/OsmJsonReaderTest/")
  {
    setResetType(ResetBasic);
  }

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
     "     'name': 'Mike\\'s Street'         \n"
     "   }                                   \n"
     " }                                     \n"
     "]                                      \n"
     "}                                      \n";

    OsmJsonReader uut;
    OsmMapPtr pMap(new OsmMap());
    uut.loadFromString(testJsonStr, pMap);

    // Need to test read from file, too
    OsmMapPtr pMap2 = uut.loadFromFile("test-files/nodes.json");

    // Test against osm xml
    QString testOsmStr =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<osm version=\"0.6\">\n"
      "    <node visible=\"true\" id=\"-3\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"4.0\" lon=\"-3.0\">\n"
      "        <tag k=\"name\" v=\"Mike's Street\"/>\n"
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
    OsmMapPtr pMap(new OsmMap());
    uut.loadFromString(testJsonStr, pMap);

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
    OsmMapPtr pMap(new OsmMap());
    uut.loadFromString(testJsonStr, pMap);

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

  void urlTest()
  {
    // Try hitting the network to get some data...

    // needed to suppress map crop missing element warnings
    DisableLog dl;

    OsmMapPtr pMap;
    const QString overpassHost = ConfigOptions().getOverpassApiHost();
    QString urlNodes = "http://" + overpassHost + "/api/interpreter?data=[out:json];node(35.20,-120.59,35.21,-120.58);out;";
    QString urlWays  = "http://" + overpassHost + "/api/interpreter?data=[out:json];way(35.20,-120.59,35.21,-120.58);out;";
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
      catch (const HootException& ex)
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

  void scrubQuoteTest()
  {
    QString singleQuoteTest =
        "{\n"
        " 'version': 0.6,\n"
        " 'generator': 'Overpass API',\n"
        " 'osm3s': {\n"
        "   'timestamp_osm_base': 'date',\n"
        "   'copyright': 'blah blah blah'\n"
        " },\n"
        " 'elements': [\n"
        " {\n"
        "   'type': 'node',\n"
        "   'id': -1,\n"
        "   'lat': 0,\n"
        "   'lon': 0\n"
        " }]\n"
        "}";
    QString doubleQuoteTest =
        "{\n"
        " \"version\": 0.6,\n"
        " \"generator\": \"Overpass API\",\n"
        " \"osm3s\": {\n"
        "   \"timestamp_osm_base\": \"date\",\n"
        "   \"copyright\": \"blah blah blah\"\n"
        " },\n"
        " \"elements\": [\n"
        " {\n"
        "   \"type\": \"node\",\n"
        "   \"id\": -1,\n"
        "   \"lat\": 0,\n"
        "   \"lon\": 0\n"
        " }]\n"
        "}";
    OsmJsonReader::scrubQuotes(singleQuoteTest);
    HOOT_STR_EQUALS(doubleQuoteTest, singleQuoteTest);

    QString overpassOriginal =
    "{\n"
    "  \"version\": 0.6,\n"
    "  \"generator\": \"Overpass API\",\n"
    "  \"osm3s\": {\n"
    "    \"timestamp_osm_base\": \"some date\",\n"
    "    \"copyright\": \"copyright\"\n"
    "  },\n"
    "  \"elements\": [\n"
    "{\n"
    "  \"type\": \"node\",\n"
    "  \"id\": 824816086,\n"
    "  \"lat\": 18.5396277,\n"
    "  \"lon\": -72.5293447,\n"
    "  \"timestamp\": \"date\",\n"
    "  \"version\": 2,\n"
    "  \"changeset\": 2,\n"
    "  \"tags\": {\n"
    "    \"amenity\": \"school\",\n"
    "    \"name\": \"La Prairie d'Yslande\",\n"
    "    \"operational_status\": \"open\",\n"
    "    \"operational_status_quality\": \"confirmed\",\n"
    "    \"school_district\": \"Gressier\",\n"
    "    \"school_type\": \"kindergarten\",\n"
    "    \"source\": \"CNIGS/OIM/FOCS\"\n"
    "  }\n"
    "}\n"
    "  ]\n"
    "}";

    const QString overpassCorrect(overpassOriginal);
    OsmJsonReader::scrubQuotes(overpassOriginal);
    HOOT_STR_EQUALS(overpassCorrect, overpassOriginal);
  }

  void scrubBigIntsTest()
  {
    QString bigIntTest =
        "{\n"
        " \"test1\": 1.123456789,\n"
        " \"test2\": 2222222222,\n"
        " \"test3\": \"3333333333\",\n"
        " \"test4\": -4444444444,\n"
        " \"test5\": \"-5555555555\",\n"
        " \"test6\": \"This is text with a big int 1111111111 right in the middle, don't break this.\"\n"
        "}";
    QString bigIntCorrect =
        "{\n"
        " \"test1\": 1.123456789,\n"
        " \"test2\": \"2222222222\",\n"
        " \"test3\": \"3333333333\",\n"
        " \"test4\": \"-4444444444\",\n"
        " \"test5\": \"-5555555555\",\n"
        " \"test6\": \"This is text with a big int 1111111111 right in the middle, don't break this.\"\n"
        "}";
    OsmJsonReader::scrubBigInts(bigIntTest);
    HOOT_STR_EQUALS(bigIntCorrect, bigIntTest);

    QString bigIntMultiline =
    "{\n"
    "  \"version\": 0.6,\n"
    "  \"generator\": \"Overpass API\",\n"
    "  \"osm3s\": {\n"
    "    \"timestamp_osm_base\": \"2017-07-14T15:12:03Z\",\n"
    "    \"copyright\": \"The data included in this document is from www.openstreetmap.org. The data is made available under ODbL.\"\n"
    "  },\n"
    "  \"elements\": [\n"
    "    {\n"
    "      \"type\": \"way\",\n"
    "      \"id\": \"10575627\",\n"
    "      \"nodes\": [\n"
    "        91356553,\n"
    "        91711357,\n"
    "        3886528792,\n"
    "        3886528800,\n"
    "        91711634\n"
    "      ],\n"
    "      \"tags\": {\n"
    "        \"highway\": \"tertiary\",\n"
    "        \"maxspeed\": \"55 mph\",\n"
    "        \"name\": \"Orcutt Road\",\n"
    "        \"source:maxspeed\": \"Survey\",\n"
    "        \"tiger:cfcc\": \"A41\",\n"
    "        \"tiger:county\": \"San Luis Obispo, CA\",\n"
    "        \"tiger:name_base\": \"Orcutt\",\n"
    "        \"tiger:name_type\": \"Rd\",\n"
    "        \"tiger:reviewed\": \"yes\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}";
    QString bigIntMultilineCorrect =
    "{\n"
    "  \"version\": 0.6,\n"
    "  \"generator\": \"Overpass API\",\n"
    "  \"osm3s\": {\n"
    "    \"timestamp_osm_base\": \"2017-07-14T15:12:03Z\",\n"
    "    \"copyright\": \"The data included in this document is from www.openstreetmap.org. The data is made available under ODbL.\"\n"
    "  },\n"
    "  \"elements\": [\n"
    "    {\n"
    "      \"type\": \"way\",\n"
    "      \"id\": \"10575627\",\n"
    "      \"nodes\": [\n"
    "        \"91356553\",\n"
    "        \"91711357\",\n"
    "        \"3886528792\",\n"
    "        \"3886528800\",\n"
    "        \"91711634\"\n"
    "      ],\n"
    "      \"tags\": {\n"
    "        \"highway\": \"tertiary\",\n"
    "        \"maxspeed\": \"55 mph\",\n"
    "        \"name\": \"Orcutt Road\",\n"
    "        \"source:maxspeed\": \"Survey\",\n"
    "        \"tiger:cfcc\": \"A41\",\n"
    "        \"tiger:county\": \"San Luis Obispo, CA\",\n"
    "        \"tiger:name_base\": \"Orcutt\",\n"
    "        \"tiger:name_type\": \"Rd\",\n"
    "        \"tiger:reviewed\": \"yes\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}";

    OsmJsonReader::scrubBigInts(bigIntMultiline);
    HOOT_STR_EQUALS(bigIntMultilineCorrect, bigIntMultiline);

    // One of the regex's to clean big ints originally had a leading semicolon in it (see related
    // note in OsmJsonReader::scrubBigInts), which would add double quotes to the text below within
    // a single json value and break it. Haven't encountered any data instances to be cleaned so far
    // that would require the semicolon to be in the regex, so removed it. If any instances do exist,
    // then we need to rethink that regex to correctly parse this.
    QString bigIntEmbedded =
      "{\n"
      " \"fixme\": \"DUPLICATE [Facebook:1477777628952292, Facebook:1477777665618955]\"\n"
      "}";
    const QString bigIntEmbeddedCorrect(bigIntEmbedded);
    OsmJsonReader::scrubBigInts(bigIntEmbedded);
    HOOT_STR_EQUALS(bigIntEmbeddedCorrect, bigIntEmbedded);
  }

  void isSupportedTest()
  {
    OsmJsonReader uut;
    const QString overpassHost = ConfigOptions().getOverpassApiHost();

    CPPUNIT_ASSERT(uut.isSupported("test-files/nodes.json"));
    CPPUNIT_ASSERT(!uut.isSupported("test-files/io/GeoJson/AllDataTypes.geojson"));
    // If the url is of the correct scheme and matches the host, we use it.
    CPPUNIT_ASSERT(uut.isSupported("http://" + overpassHost));
    CPPUNIT_ASSERT(uut.isSupported("https://" + overpassHost));
    // wrong scheme
    CPPUNIT_ASSERT(!uut.isSupported("ftp://" + overpassHost));
    // If the url doesn't match with our configured Overpass host, skip it.
    CPPUNIT_ASSERT(!uut.isSupported("http://blah"));
    CPPUNIT_ASSERT(!uut.isSupported("https://blah"));
  }

  void runBoundsTest()
  {
    // See related note in ServiceOsmApiDbReaderTest::runReadByBoundsTest.

    OsmJsonReader uut;
    uut.setBounds(
      GeometryUtils::envelopeToPolygon(geos::geom::Envelope(-104.8996,-104.8976,38.8531,38.8552)));
    OsmMapPtr map(new OsmMap());
    uut.open(_inputPath + "runBoundsTest-in.json");
    uut.read(map);
    uut.close();

    CPPUNIT_ASSERT_EQUAL(32, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
  }

  void runBoundsLeaveConnectedOobWaysTest()
  {
    // This will leave any ways in the output which are outside of the bounds but are directly
    // connected to ways which cross the bounds.

    const QString testFileName = "runBoundsLeaveConnectedOobWaysTest.osm";

    OsmJsonReader uut;
    uut.setBounds(
      GeometryUtils::envelopeToPolygon(
        geos::geom::Envelope(38.91362, 38.915478, 15.37365, 15.37506)));
    uut.setKeepImmediatelyConnectedWaysOutsideBounds(true);

    // set cropping up for strict bounds handling
    conf().set(ConfigOptions::getConvertBoundsKeepEntireFeaturesCrossingBoundsKey(), false);
    conf().set(ConfigOptions::getConvertBoundsKeepOnlyFeaturesInsideBoundsKey(), true);

    OsmMapPtr map(new OsmMap());
    uut.open(_inputPath + "runBoundsLeaveConnectedOobWaysTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + testFileName, false, true);

    HOOT_FILE_EQUALS(_inputPath + "/" + testFileName, _outputPath + "/" + testFileName);
  }

  void elementTypeUnorderedTest()
  {
    // This should load the elements even though child elements come after their parents.

    // TODO: we need some relations in the test input

    QString testFileName;
    OsmJsonReader uut;
    OsmMapPtr map;

    TestUtils::resetBasic();
    testFileName = "elementTypeUnorderedTest1.osm";
    uut.setUseDataSourceIds(true);
    map.reset(new OsmMap());
    uut.open(_inputPath + "elementTypeUnorderedTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + testFileName, false, true);
    HOOT_FILE_EQUALS(_inputPath + "/" + testFileName, _outputPath + "/" + testFileName);

    // same as above except we create our own element ids this time

    TestUtils::resetBasic();
    testFileName = "elementTypeUnorderedTest2.osm";
    uut.setUseDataSourceIds(false);
    map.reset(new OsmMap());
    uut.open(_inputPath + "elementTypeUnorderedTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + testFileName, false, true);
    HOOT_FILE_EQUALS(_inputPath + "/" + testFileName, _outputPath + "/" + testFileName);
  }

  void elementTypeUnorderedMissingTest()
  {
    // There is one node referenced by a way that doesn't exist in the file (id=2442180398). That
    // node should not be present in the output way.

    // TODO: we need some relations and way refs in the test input

    // The default behavior is to log missing elements as warnings, and we don't want to see that in
    // this test;
    DisableLog dl;

    QString outputFile;
    OsmJsonReader uut;
    OsmMapPtr map;

    TestUtils::resetBasic();
    outputFile = "elementTypeUnorderedMissingTest1.osm";
    uut.setUseDataSourceIds(true);
    map.reset(new OsmMap());
    uut.open(_inputPath + "elementTypeUnorderedMissingTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + outputFile, false, true);
    HOOT_FILE_EQUALS(_inputPath + "/" + outputFile, _outputPath + "/" + outputFile);

    // same as above except we create our own element ids this time

    TestUtils::resetBasic();
    outputFile = "elementTypeUnorderedMissingTest2.osm";
    uut.setUseDataSourceIds(false);
    map.reset(new OsmMap());
    uut.open(_inputPath + "elementTypeUnorderedMissingTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + outputFile, false, true);
    HOOT_FILE_EQUALS(_inputPath + "/" + outputFile, _outputPath + "/" + outputFile);
  }
};
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::OsmJsonReaderTest, "slow");

