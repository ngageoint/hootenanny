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
 * @copyright Copyright (C) 2019, 2020 Maxar (http://www.maxar.com/)
 */

#include "OsmApiReaderTestServer.h"

//  hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/OsmApiUtils.h>
#include <hoot/core/geometry/GeometryUtils.h>

//  Run tests against a local test server
#define RUN_LOCAL_TEST_SERVER

namespace hoot
{

class OsmApiReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiReaderTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runSplitGeographicTest);
  CPPUNIT_TEST(runSplitElementsTest);
  CPPUNIT_TEST(runFailureTest);
  CPPUNIT_TEST_SUITE_END();

public:

  const QString OSM_API_URL = "https://www.openstreetmap.org";
  const QString LOCAL_TEST_API_URL = "http://localhost:%1";
#ifdef RUN_LOCAL_OSM_API_SERVER
  const QString LOCAL_OSM_API_URL = "http://<Enter local OSM API URL here>";
#endif

  /** Separate port numbers so that tests can run in parallel */
  const int PORT_SIMPLE =         9900;
  const int PORT_SPLIT_GEO =      9901;
  const int PORT_SPLIT_ELEMENTS = 9902;

  OsmApiReaderTest()
    : HootTestFixture("test-files/io/OsmApiReaderTest/",
                      "test-output/io/OsmApiReaderTest/")
  {
    setResetType(ResetBasic);
  }

  void runSimpleTest()
  {
    SimpleReaderTestServer server(PORT_SIMPLE);
    server.start();

    OsmMapPtr map(new OsmMap());

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
  }

  void runSplitGeographicTest()
  {
    GeographicSplitReaderTestServer server(PORT_SPLIT_GEO);
    server.start();

    OsmMapPtr map(new OsmMap());

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
  }

  void runSplitElementsTest()
  {
    ElementSplitReaderTestServer server(PORT_SPLIT_ELEMENTS);
    server.start();

    OsmMapPtr map(new OsmMap());

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
  }

  void runFailureTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
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
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiReaderTest, "slow");

}
