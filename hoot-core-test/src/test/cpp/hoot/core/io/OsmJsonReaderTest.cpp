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
 * @copyright Copyright (C) 2012-2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/OsmApiUtils.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class OsmJsonReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmJsonReaderTest);
  CPPUNIT_TEST(nodeTest);
  CPPUNIT_TEST(wayTest);
  CPPUNIT_TEST(relationTest);
  CPPUNIT_TEST(urlTest);
  CPPUNIT_TEST(bigIntsReadTest);
  CPPUNIT_TEST(isSupportedTest);
  CPPUNIT_TEST(runBoundsTest);
  CPPUNIT_TEST(runBoundsLeaveConnectedOobWaysTest);
  CPPUNIT_TEST(elementTypeUnorderedTest);
  CPPUNIT_TEST(elementTypeUnorderedMissingTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmJsonReaderTest()
    : HootTestFixture("test-files/io/OsmJsonReaderTest/", "test-output/io/OsmJsonReaderTest/")
  {
    setResetType(ResetConfigs);
  }

  void nodeTest()
  {
    QString testJsonStr =
     "{"
     " 'version': 0.6,"
     " 'generator': 'Overpass API',"
     " 'osm3s': {"
     "   'timestamp_osm_base': 'date',"
     "   'copyright': 'c 1999'"
     " },"
     " 'elements': ["
     " {"
     "   'type': 'node',"
     "   'id': -1,"
     "   'lat': 2.0,"
     "   'lon': -3.0"
     " },"
     " {"
     "   'type': 'node',"
     "   'id': -2,"
     "   'lat': 3.0,"
     "   'lon': -3.0,"
     "   'timestamp': '2010-01-01T00:00:00Z',\n"
     "   'version': 4,"
     "   'changeset': 5,"
     "   'user': 'somebody',"
     "   'uid': 6"
     " },"
     " {"
     "   'type': 'node',"
     "   'id': -3,"
     "   'lat': 4.0,"
     "   'lon': -3.0,"
     "   'tags': {"
     "     'highway': 'bus_stop',"
     "     'name': 'Mike\\'s Street'"
     "   }"
     " }"
     "]"
     "}";
    //  Convert single quoted C++ string to valid JSON string
    StringUtils::scrubQuotes(testJsonStr);

    OsmJsonReader uut;
    OsmMapPtr pMap = std::make_shared<OsmMap>();
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

    OsmMapPtr pTestMap = std::make_shared<OsmMap>();
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
    OsmMapPtr pMap = std::make_shared<OsmMap>();
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

    OsmMapPtr pTestMap = std::make_shared<OsmMap>();
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.readFromString(testOsmStr, pTestMap);

    CPPUNIT_ASSERT(TestUtils::compareMaps(pMap, pTestMap));
  }

  void relationTest()
  {
    QString testJsonStr =
      "{"
      " 'version': 0.6,"
      " 'generator': 'Overpass API',"
      " 'osm3s': {"
      "   'timestamp_osm_base': 'date',"
      "   'copyright': 'blah blah blah'"
      " },"
      " 'elements': ["
      " {"
      "   'type': 'node',"
      "   'id': -1,"
      "   'lat': 0,"
      "   'lon': 0"
      " },"
      " {"
      "   'type': 'node',"
      "   'id': -2,"
      "   'lat': 0,"
      "   'lon': 20"
      " },"
      " {"
      "   'type': 'node',"
      "   'id': -3,"
      "   'lat': 20,"
      "   'lon': 21"
      " },"
      " {"
      "   'type': 'node',"
      "   'id': -4,"
      "   'lat': 20,"
      "   'lon': 0"
      " },"
      " {"
      "   'type': 'node',"
      "   'id': -5,"
      "   'lat': 0,"
      "   'lon': 0"
      " },"
      " {"
      "   'type': 'way',"
      "   'id': -1,"
      "   'nodes': [-1,-2,-3,-4,-5],"
      "   'tags': {"
      "     'note': 'w1',"
      "     'alt_name': 'bar',"
      "     'name': 'foo',"
      "     'area': 'yes',"
      "     'amenity': 'bar',"
      "     '" + MetadataTags::ErrorCircular() + "': '5'\n"
      "   }"
      " },"
      " {"
      "   'type': 'relation',"
      "   'id': 1745069,"
      "   'members': ["
      "     {"
      "       'type': 'way',"
      "       'ref': -1,"
      "       'role': ''"
      "     }"
      "   ]"
      " },"
      " {"
      "   'type': 'relation',"
      "   'id': 172789"
      " },"
      " {"
      "   'type': 'relation',"
      "   'id': 1,"
      "   'members': ["
      "     {"
      "       'type': 'relation',"
      "       'ref': 1745069,"
      "       'role': ''"
      "     },"
      "     {"
      "       'type': 'relation',"
      "       'ref': 172789,"
      "       'role': ''"
      "     }"
      "   ],"
      "   'tags': {"
      "     'from': 'Konrad-Adenauer-Platz',"
      "     'name': 'VRS 636',"
      "     'network': 'VRS',"
      "     'operator': 'SWB',"
      "     'ref': '636',"
      "     'route': 'bus',"
      "     'to': 'Gielgen',"
      "     'via': 'Ramersdorf'"
      "   }"
      " }"
      " ]"
      "}";
    //  Convert single quoted C++ string to valid JSON string
    StringUtils::scrubQuotes(testJsonStr);

    OsmJsonReader uut;
    OsmMapPtr pMap = std::make_shared<OsmMap>();
    uut.loadFromString(testJsonStr, pMap);

    // Useful for debug
    //OsmXmlWriter writer;
    //writer.setIncludeIds(true);
    //writer.write(pMap, "tmp/test.osm");

    // Test against osm xml
    QString testOsmStr =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<osm version=\"0.6\">"
      "  <node visible=\"true\" id=\"-5\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"0.00\" lon=\"0.00\"/>"
      "  <node visible=\"true\" id=\"-4\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"20.0\" lon=\"0.00\"/>"
      "  <node visible=\"true\" id=\"-3\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"20.0\" lon=\"21.0\"/>"
      "  <node visible=\"true\" id=\"-2\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"0.00\" lon=\"20.0\"/>"
      "  <node visible=\"true\" id=\"-1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\" lat=\"0.00\" lon=\"0.00\"/>"
      "  <way  visible=\"true\" id=\"-1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">"
      "      <nd ref=\"-1\"/>"
      "      <nd ref=\"-2\"/>"
      "      <nd ref=\"-3\"/>"
      "      <nd ref=\"-4\"/>"
      "      <nd ref=\"-5\"/>"
      "      <tag k=\"note\" v=\"w1\"/>"
      "      <tag k=\"alt_name\" v=\"bar\"/>"
      "      <tag k=\"name\" v=\"foo\"/>"
      "      <tag k=\"area\" v=\"yes\"/>"
      "      <tag k=\"amenity\" v=\"bar\"/>"
      "      <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"5\"/>"
      "  </way>"
      "  <relation visible=\"true\" id=\"1\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">"
      "      <member type=\"relation\" ref=\"1745069\" role=\"\"/>"
      "      <member type=\"relation\" ref=\"172789\" role=\"\"/>"
      "      <tag k=\"network\" v=\"VRS\"/>"
      "      <tag k=\"route\" v=\"bus\"/>"
      "      <tag k=\"operator\" v=\"SWB\"/>"
      "      <tag k=\"via\" v=\"Ramersdorf\"/>"
      "      <tag k=\"from\" v=\"Konrad-Adenauer-Platz\"/>"
      "      <tag k=\"to\" v=\"Gielgen\"/>"
      "      <tag k=\"name\" v=\"VRS 636\"/>"
      "      <tag k=\"ref\" v=\"636\"/>"
      "      <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"15\"/>"
      "  </relation>"
      "  <relation visible=\"true\" id=\"172789\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">"
      "      <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"15\"/>"
      "  </relation>"
      "  <relation visible=\"true\" id=\"1745069\" timestamp=\"1970-01-01T00:00:00Z\" version=\"1\">"
      "      <member type=\"way\" ref=\"-1\" role=\"\"/>"
      "      <tag k=\"" + MetadataTags::ErrorCircular() + "\" v=\"15\"/>"
      "  </relation>"
      "</osm>";

    OsmMapPtr pTestMap = std::make_shared<OsmMap>();
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.readFromString(testOsmStr, pTestMap);

    CPPUNIT_ASSERT(TestUtils::compareMaps(pMap, pTestMap));
  }

  void urlTest()
  {
    // Try hitting the network to get some data...
    const QString overpassHost = ConfigOptions().getOverpassApiHost();
    QString url = QString("https://%1/%2").arg(overpassHost, OsmApiEndpoints::OVERPASS_API_PATH);
    OsmJsonReader uut;

    Settings s;

    s.set(ConfigOptions::getBoundsKey(), "-120.59,35.20,-120.58,35.21");
    uut.setConfiguration(s);

    QString query = "[out:json][bbox];node;out;";
    queryOverpass(uut, QString("%1?data=%2").arg(url, query));

    uut.setQueryFilePath(_inputPath + "/overpass_query.overpassql");

    queryOverpass(uut, url);

    // File URL
    QString fname = QDir::currentPath() + "/test-files/nodes.json";
    QString urlFile = "file://" + fname;
    OsmMapPtr pMap = std::make_shared<OsmMap>();
    CPPUNIT_ASSERT(uut.isSupported(urlFile));
    uut.open(urlFile);
    uut.read(pMap);
  }

  void queryOverpass(OsmJsonReader& uut, const QString& url)
  {
    OsmMapPtr pMap;
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
        pMap = std::make_shared<OsmMap>();

        // Get data
        CPPUNIT_ASSERT(uut.isSupported(url));
        uut.open(url);
        uut.read(pMap);

        success = !uut.isError();
      }
      catch (const HootException& ex)
      {
        numTries++;
        LOG_INFO(ex.getWhat() + QString(" will retry after %1 seconds").arg(RETRY_SECS));
        sleep(RETRY_SECS);
      }
    }
    CPPUNIT_ASSERT(success);
    // Not failure == success
  }

  void bigIntsReadTest()
  {
    //  Boost 1.41 property tree json parser had trouble with integers bigger than 2^31.
    //  As of 1.53.0 this is no longer a problem.  Test to make sure it is working normally.
    QString testJsonStr =
     "{"
     " 'version': 0.6,"
     " 'generator': 'Overpass API',"
     " 'osm3s': {"
     "   'timestamp_osm_base': 'date',"
     "   'copyright': 'c 1999'"
     " },"
     " 'elements': ["
     " {"
     "   'type': 'node',"
     "   'id': 2222222222,"
     "   'lat': 2.0,"
     "   'lon': -3.0"
     " },"
     " {"
     "   'type': 'node',"
     "   'id': 2222222223,"
     "   'lat': 3.0,"
     "   'lon': -3.0,"
     "   'timestamp': '2010-01-01T00:00:00Z',\n"
     "   'version': 4,"
     "   'changeset': 5,"
     "   'user': 'somebody',"
     "   'uid': 6"
     " },"
     " {"
     "   'type': 'node',"
     "   'id': 2222222224,"
     "   'lat': 4.0,"
     "   'lon': -3.0,"
     "   'tags': {"
     "     'highway': 'bus_stop',"
     "     'name': 'Mike\\'s Street'"
     "   }"
     " }"
     "]"
     "}";
    //  Convert single quoted C++ string to valid JSON string
    StringUtils::scrubQuotes(testJsonStr);

    OsmJsonReader uut;
    OsmMapPtr pMap = std::make_shared<OsmMap>();
    uut.loadFromString(testJsonStr, pMap);

    CPPUNIT_ASSERT_EQUAL(3L, pMap->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(2222222222L, pMap->getNode(2222222222)->getId());
    CPPUNIT_ASSERT_EQUAL(2222222223L, pMap->getNode(2222222223)->getId());
    CPPUNIT_ASSERT_EQUAL(2222222224L, pMap->getNode(2222222224)->getId());
  }

  void isSupportedTest()
  {
    OsmJsonReader uut;
    const QString overpassHost = ConfigOptions().getOverpassApiHost();

    CPPUNIT_ASSERT(uut.isSupported("test-files/nodes.json"));
    CPPUNIT_ASSERT(!uut.isSupported("test-files/io/GeoJson/AllDataTypes.geojson"));
    //  If the url is of the correct scheme and matches the host, we use it.
    CPPUNIT_ASSERT(uut.isSupported("http://" + overpassHost + "?data=[out:json]"));
    CPPUNIT_ASSERT(uut.isSupported("https://" + overpassHost + "?data=[out:json]"));
    //  wrong scheme
    CPPUNIT_ASSERT(!uut.isSupported("ftp://" + overpassHost));
    //  If the url doesn't match with our configured Overpass host, skip it.
    CPPUNIT_ASSERT(!uut.isSupported("http://blah"));
    CPPUNIT_ASSERT(!uut.isSupported("https://blah"));
  }

  void runBoundsTest()
  {
    //  See related note in ServiceOsmApiDbReaderTest::runReadByBoundsTest.
    OsmJsonReader uut;
    uut.setBounds(geos::geom::Envelope(-104.8996,-104.8976,38.8531,38.8552));
    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.open(_inputPath + "runBoundsTest-in.json");
    uut.read(map);
    uut.close();

    CPPUNIT_ASSERT_EQUAL(32, (int)map->getNodeCount());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWayCount());
  }

  void runBoundsLeaveConnectedOobWaysTest()
  {
    //  This will leave any ways in the output which are outside of the bounds but are directly
    //  connected to ways which cross the bounds.

    const QString testFileName = "runBoundsLeaveConnectedOobWaysTest.osm";

    OsmJsonReader uut;
    uut.setBounds(geos::geom::Envelope(38.91362, 38.915478, 15.37365, 15.37506));
    uut.setKeepImmediatelyConnectedWaysOutsideBounds(true);

    //  Set cropping up for strict bounds handling
    conf().set(ConfigOptions::getBoundsKeepEntireFeaturesCrossingBoundsKey(), false);
    conf().set(ConfigOptions::getBoundsKeepOnlyFeaturesInsideBoundsKey(), true);

    OsmMapPtr map = std::make_shared<OsmMap>();
    uut.open(_inputPath + "runBoundsLeaveConnectedOobWaysTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + testFileName, false, true);

    HOOT_FILE_EQUALS(_inputPath + "/" + testFileName, _outputPath + "/" + testFileName);
  }

  void elementTypeUnorderedTest()
  {
    //  This should load the elements even though child elements come after their parents.
    QString testFileName;
    OsmJsonReader uut;
    OsmMapPtr map;

    TestUtils::resetBasic();
    testFileName = "elementTypeUnorderedTest1.osm";
    uut.setUseDataSourceIds(true);
    map = std::make_shared<OsmMap>();
    uut.open(_inputPath + "elementTypeUnorderedTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + testFileName, false, true);
    HOOT_FILE_EQUALS(_inputPath + "/" + testFileName, _outputPath + "/" + testFileName);

    //  Same as above except we create our own element ids this time
    TestUtils::resetBasic();
    testFileName = "elementTypeUnorderedTest2.osm";
    uut.setUseDataSourceIds(false);
    map = std::make_shared<OsmMap>();
    uut.open(_inputPath + "elementTypeUnorderedTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + testFileName, false, true);
    HOOT_FILE_EQUALS(_inputPath + "/" + testFileName, _outputPath + "/" + testFileName);
  }

  void elementTypeUnorderedMissingTest()
  {
    //  There is one node referenced by a way that doesn't exist in the file (id=2442180398). That
    //  node should not be present in the output way.

    //  The default behavior is to log missing elements as warnings, and we don't want to see that in
    //  this test;
    DisableLog dl;

    QString outputFile;
    OsmJsonReader uut;
    OsmMapPtr map;

    TestUtils::resetBasic();
    outputFile = "elementTypeUnorderedMissingTest1.osm";
    uut.setUseDataSourceIds(true);
    map = std::make_shared<OsmMap>();
    uut.open(_inputPath + "elementTypeUnorderedMissingTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + outputFile, false, true);
    HOOT_FILE_EQUALS(_inputPath + "/" + outputFile, _outputPath + "/" + outputFile);

    //  Same as above except we create our own element ids this time
    TestUtils::resetBasic();
    outputFile = "elementTypeUnorderedMissingTest2.osm";
    uut.setUseDataSourceIds(false);
    map = std::make_shared<OsmMap>();
    uut.open(_inputPath + "elementTypeUnorderedMissingTest-in.json");
    uut.read(map);
    uut.close();
    OsmMapWriterFactory::write(map, _outputPath + "/" + outputFile, false, true);
    HOOT_FILE_EQUALS(_inputPath + "/" + outputFile, _outputPath + "/" + outputFile);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmJsonReaderTest, "slow");

}

