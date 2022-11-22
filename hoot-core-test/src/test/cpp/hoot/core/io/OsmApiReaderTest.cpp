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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "OsmApiReaderTestServer.h"

//  hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OsmApiReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>

#include <hoot/core/util/OsmApiUtils.h>

//  Run tests against a local test server
//#define RUN_LOCAL_TEST_SERVER

//  Run tests against a production (OSM API and Overpass API)
//#define RUN_PRODUCTION_TESTS

//  Qt
#include <QProcess>

namespace hoot
{

class OsmApiReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiReaderTest);
  CPPUNIT_TEST(runLoadBoundsTest);

#ifdef RUN_LOCAL_TEST_SERVER
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runSplitGeographicTest);
  CPPUNIT_TEST(runSplitElementsTest);
  CPPUNIT_TEST(runFailureTest);
  CPPUNIT_TEST(runPolygonBoundsTest);
  CPPUNIT_TEST(runStreamTest);
#endif

#ifdef RUN_PRODUCTION_TESTS
  CPPUNIT_TEST(runOsmApiXmlTest);
  CPPUNIT_TEST(runOverpassXmlTest);
#endif

  CPPUNIT_TEST_SUITE_END();

public:

  const QString OSM_API_URL = "https://www.openstreetmap.org";
  const QString LOCAL_TEST_API_URL = "http://localhost:%1";

  /** Separate port numbers so that tests can run in parallel */
  const int PORT_SIMPLE =         9900;
  const int PORT_SPLIT_GEO =      9901;
  const int PORT_SPLIT_ELEMENTS = 9902;
  const int PORT_POLYGON =        9903;
  const int PORT_STREAM =         9904;

  OsmApiReaderTest()
    : HootTestFixture("test-files/io/OsmApiReaderTest/",
                      "test-output/io/OsmApiReaderTest/")
  {
  }

  void runLoadBoundsTest()
  {
    OsmApiReader reader;
    Settings s;
    //  Use a bounding box in `bounds`
    s.set(ConfigOptions::getBoundsKey(), "38.4902,35.7982,38.6193,35.8536");
    s.set(ConfigOptions::getBoundsInputFileKey(), "");
    reader.setConfiguration(s);
    CPPUNIT_ASSERT(reader._loadBounds());
    CPPUNIT_ASSERT(!reader._isPolygon);

    //  Use a bounding polygon that is a rectangle in `bounds`
    s.set(ConfigOptions::getBoundsKey(), "-72.474,18.545;-72.474,18.548;-72.471,18.548;-72.471,18.545;-72.474,18.545");
    s.set(ConfigOptions::getBoundsInputFileKey(), "");
    reader.setConfiguration(s);
    CPPUNIT_ASSERT(reader._loadBounds());
    CPPUNIT_ASSERT(!reader._isPolygon);

    //  Use a bounding polygon in `bounds`
    s.set(ConfigOptions::getBoundsKey(), "-72.474,18.545;-72.471,18.548;-72.471,18.545;-72.474,18.545");
    s.set(ConfigOptions::getBoundsInputFileKey(), "");
    reader.setConfiguration(s);
    CPPUNIT_ASSERT(reader._loadBounds());
    CPPUNIT_ASSERT(reader._isPolygon);

    //  Use a bounding box in `bounds.input.file`
    s.set(ConfigOptions::getBoundsKey(), "");
    s.set(ConfigOptions::getBoundsInputFileKey(), _inputPath + "ToyTestBboxBounds.osm");
    reader.setConfiguration(s);
    CPPUNIT_ASSERT(reader._loadBounds());
    CPPUNIT_ASSERT(!reader._isPolygon);

    //  Use a bounding polygon in `bounds.input.file`
    s.set(ConfigOptions::getBoundsKey(), "");
    s.set(ConfigOptions::getBoundsInputFileKey(), _inputPath + "ToyTestPolyBounds.osm");
    reader.setConfiguration(s);
    CPPUNIT_ASSERT(reader._loadBounds());
    CPPUNIT_ASSERT(reader._isPolygon);
  }

  void runSimpleTest()
  {
#ifdef RUN_LOCAL_TEST_SERVER
    SimpleReaderTestServer server(PORT_SIMPLE);
    server.start();

    OsmMapPtr map = std::make_shared<OsmMap>();

    OsmApiReader reader;
    //  Set the bounds to be smaller than 0.25 degrees by 0.25 degrees so that there is no splitting
    reader.setBounds(geos::geom::Envelope(-111.3914, -111.1942, 40.5557, 40.7577));
    reader.open(LOCAL_TEST_API_URL.arg(PORT_SIMPLE) + OsmApiEndpoints::API_PATH_MAP);
    reader.read(map);

    server.shutdown();

    QString output = _outputPath + "SimpleTestOutput.osm";

    OsmXmlWriter writer;
    writer.write(map, output);
    writer.close();

    HOOT_FILE_EQUALS(_inputPath + "SimpleTestExpected.osm", output);
#endif
  }

  void runSplitGeographicTest()
  {
#ifdef RUN_LOCAL_TEST_SERVER
    GeographicSplitReaderTestServer server(PORT_SPLIT_GEO);
    server.start();

    OsmMapPtr map = std::make_shared<OsmMap>();

    OsmApiReader reader;
    Settings s;
    s.set(ConfigOptions::getReaderHttpBboxThreadCountKey(), 1);
    reader.setConfiguration(s);
    //  Set the bounds to be larger than 0.25 degrees by 0.25 degrees to cause a geographic split
    reader.setBounds(geos::geom::Envelope(-111.3914, -111.1142, 40.4557, 40.7577));
    reader.open(LOCAL_TEST_API_URL.arg(PORT_SPLIT_GEO) + OsmApiEndpoints::API_PATH_MAP);
    reader.read(map);

    server.shutdown();

    QString output = _outputPath + "SplitGeographicTestOutput.osm";

    OsmXmlWriter writer;
    writer.write(map, output);
    writer.close();

    HOOT_FILE_EQUALS(_inputPath + "SplitTestExpected.osm", output);
#endif
  }

  void runSplitElementsTest()
  {
#ifdef RUN_LOCAL_TEST_SERVER
    ElementSplitReaderTestServer server(PORT_SPLIT_ELEMENTS);
    server.start();

    OsmMapPtr map = std::make_shared<OsmMap>();

    OsmApiReader reader;
    Settings s;
    s.set(ConfigOptions::getReaderHttpBboxThreadCountKey(), 1);
    reader.setConfiguration(s);
    //  Set the bounds to be smaller than 0.25 degrees by 0.25 degrees so that it is split by elements on the server
    reader.setBounds(geos::geom::Envelope(-111.3914, -111.1942, 40.5557, 40.7577));
    reader.open(LOCAL_TEST_API_URL.arg(PORT_SPLIT_ELEMENTS) + OsmApiEndpoints::API_PATH_MAP);
    reader.read(map);

    server.shutdown();

    QString output = _outputPath + "SplitElementsTestOutput.osm";

    OsmXmlWriter writer;
    writer.write(map, output);
    writer.close();

    HOOT_FILE_EQUALS(_inputPath + "SplitTestExpected.osm", output);
#endif
  }

  void runFailureTest()
  {
#ifdef RUN_LOCAL_TEST_SERVER
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map = std::make_shared<OsmMap>();
      OsmApiReader reader;
      //  Set the bounds to be larger than 1 degree by 1 degree so that it will fail
      reader.setBounds(geos::geom::Envelope(-111.3914, -110.2914, 40.5557, 41.7557));
      //  Can reuse PORT_SIMPLE here because it isn't actually used before the test fails
      reader.open(LOCAL_TEST_API_URL.arg(PORT_SIMPLE) + OsmApiEndpoints::API_PATH_MAP);
      reader.read(map);
    }
    catch (const UnsupportedException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg == "Cannot request areas larger than 1.0000 square degrees.");
#endif
  }

  void runPolygonBoundsTest()
  {
#ifdef RUN_LOCAL_TEST_SERVER
    SimpleReaderTestServer server(PORT_POLYGON);
    server.start();

    OsmMapPtr map = std::make_shared<OsmMap>();

    OsmApiReader reader;
    Settings s;
    s.set(ConfigOptions::getReaderHttpBboxThreadCountKey(), 1);
    s.set(ConfigOptions::getBoundsInputFileKey(), _inputPath + "ToyTestPolyBounds.osm");
    reader.setConfiguration(s);
    reader.open(LOCAL_TEST_API_URL.arg(PORT_POLYGON) + OsmApiEndpoints::API_PATH_MAP);
    reader.read(map);

    server.shutdown();

    QString output = _outputPath + "PolygonBoundsTestOutput.osm";

    OsmXmlWriter writer;
    writer.write(map, output);
    writer.close();

    HOOT_FILE_EQUALS(_inputPath + "PolygonBoundsTestExpected.osm", output);
#endif

  }

  void runOsmApiXmlTest()
  {
#ifdef RUN_PRODUCTION_TESTS
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmApiReader reader;
    Settings s;
    s.set(ConfigOptions::getReaderHttpBboxThreadCountKey(), 1);
    s.set(ConfigOptions::getBoundsInputFileKey(), _inputPath + "ToyTestBboxBounds.osm");
    reader.setConfiguration(s);
    //  Read XML from OSM API and parse it
    reader.open(OSM_API_URL + OsmApiEndpoints::API_PATH_MAP);
    reader.read(map);

    QString output = _outputPath + "OsmApiOutput.osm";

    OsmXmlWriter writer;
    writer.write(map, output);
    writer.close();
#endif
  }

  void runOverpassXmlTest()
  {
#ifdef RUN_PRODUCTION_TESTS
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmApiReader reader;
    Settings s;
    s.set(ConfigOptions::getReaderHttpBboxThreadCountKey(), 1);
    s.set(ConfigOptions::getBoundsInputFileKey(), _inputPath + "ToyTestBboxBounds.osm");
    reader.setConfiguration(s);
    //  Read XML from Overpass and parse it
    reader.open("https://overpass-api.de/api/interpreter?data=[out:xml];(node({{bbox}});(._;<;>;););out meta;");
    reader.read(map);

    QString output = _outputPath + "OverpassOutput.osm";

    OsmXmlWriter writer;
    writer.write(map, output);
    writer.close();
#endif
  }

  void runStreamTest()
  {
#ifdef RUN_LOCAL_TEST_SERVER
    OverpassReaderTestServer server(PORT_STREAM);
    server.start();

    QString output_dir = _outputPath + "stream_test";
    QString output_file = output_dir + ".shp";

    FileUtils::removeDir(output_dir);

    QString cmd = "hoot";
    QStringList args;
    args << "convert"
         << "-D reader.http.bbox.max.size=0.025"
         << "-D schema.translation.script=translations/TDSv71.js"
         << "-D bounds=2.277303,48.851684,2.311635,48.864701"
         << "-D overpass.api.host=localhost"
         << "\"http://localhost:9904/api/interpreter?data=[out:xml];(node({{bbox}});(._;<;>;););out meta;\""
         << output_file;

    QProcess p;
    p.start(cmd, args,QIODevice::ReadOnly);
    p.waitForFinished();

    server.shutdown();
#endif
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiReaderTest, "slow");

}
