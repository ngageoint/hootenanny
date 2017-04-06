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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/CookieCutter.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class CookieCutterTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CookieCutterTest);
    CPPUNIT_TEST(runTest);
    CPPUNIT_TEST(runCropTest);
    CPPUNIT_TEST(runBufferTest);
    CPPUNIT_TEST(runNegativeBufferTest);
    CPPUNIT_TEST(runCropAndBufferTest);
    CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    Settings::getInstance().clear();
    OsmXmlReader reader;
    OsmMap::resetCounters();
    OsmSchema::getInstance().loadDefault();
    boost::shared_ptr<OsmMap> doughMap(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    boost::shared_ptr<OsmMap> cutShapeMap(new OsmMap());
    reader.read("test-files/conflate/AlphaShapeGeneratorNegativeBufferTest.osm", cutShapeMap);

    CookieCutter(false).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    QDir().mkpath("test-output/conflate");
    OsmXmlWriter writer;
    writer.write(cookieCutMap, "test-output/conflate/CookieCutterTest.osm");

    HOOT_FILE_EQUALS("test-files/conflate/CookieCutterTest.osm",
                     "test-output/conflate/CookieCutterTest.osm");
  }

  void runCropTest()
  {
    Settings::getInstance().clear();
    OsmXmlReader reader;
    OsmMap::resetCounters();
    OsmSchema::getInstance().loadDefault();
    boost::shared_ptr<OsmMap> doughMap(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    boost::shared_ptr<OsmMap> cutShapeMap(new OsmMap());
    reader.read("test-files/conflate/AlphaShapeGeneratorNegativeBufferTest.osm", cutShapeMap);

    CookieCutter(true).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    QDir().mkpath("test-output/conflate");
    OsmXmlWriter writer;
    writer.write(cookieCutMap, "test-output/conflate/CookieCutterCropTest.osm");

    HOOT_FILE_EQUALS("test-files/conflate/CookieCutterCropTest.osm",
                     "test-output/conflate/CookieCutterCropTest.osm");
  }

  void runBufferTest()
  {
    Settings::getInstance().clear();
    OsmXmlReader reader;
    OsmMap::resetCounters();
    OsmSchema::getInstance().loadDefault();
    boost::shared_ptr<OsmMap> doughMap(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    boost::shared_ptr<OsmMap> cutShapeMap(new OsmMap());
    reader.read("test-files/conflate/AlphaShapeGeneratorNegativeBufferTest.osm", cutShapeMap);

    CookieCutter(false, 100.0).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    QDir().mkpath("test-output/conflate");
    OsmXmlWriter writer;
    writer.write(cookieCutMap, "test-output/conflate/CookieCutterBufferTest.osm");

    HOOT_FILE_EQUALS("test-files/conflate/CookieCutterBufferTest.osm",
                     "test-output/conflate/CookieCutterBufferTest.osm");
  }

  void runNegativeBufferTest()
  {
    Settings::getInstance().clear();
    OsmXmlReader reader;
    OsmMap::resetCounters();
    OsmSchema::getInstance().loadDefault();
    boost::shared_ptr<OsmMap> doughMap(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    boost::shared_ptr<OsmMap> cutShapeMap(new OsmMap());
    reader.read("test-files/conflate/AlphaShapeGeneratorNegativeBufferTest.osm", cutShapeMap);

    CookieCutter(false, -100.0).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    QDir().mkpath("test-output/conflate");
    OsmXmlWriter writer;
    writer.write(cookieCutMap, "test-output/conflate/CookieCutterNegativeBufferTest.osm");

    HOOT_FILE_EQUALS("test-files/conflate/CookieCutterNegativeBufferTest.osm",
                     "test-output/conflate/CookieCutterNegativeBufferTest.osm");
  }

  void runCropAndBufferTest()
  {
    Settings::getInstance().clear();
    OsmXmlReader reader;
    OsmMap::resetCounters();
    OsmSchema::getInstance().loadDefault();
    boost::shared_ptr<OsmMap> doughMap(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    boost::shared_ptr<OsmMap> cutShapeMap(new OsmMap());
    reader.read("test-files/conflate/AlphaShapeGeneratorNegativeBufferTest.osm", cutShapeMap);

    CookieCutter(true, 100.0).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    QDir().mkpath("test-output/conflate");
    OsmXmlWriter writer;
    writer.write(cookieCutMap, "test-output/conflate/CookieCutterCropAndBufferTest.osm");

    HOOT_FILE_EQUALS("test-files/conflate/CookieCutterCropAndBufferTest.osm",
                     "test-output/conflate/CookieCutterCropAndBufferTest.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CookieCutterTest, "slow");

}
