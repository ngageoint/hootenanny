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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/ConfigOptions.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class CookieCutterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(CookieCutterTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runCropTest);
  CPPUNIT_TEST(runBufferTest);
  CPPUNIT_TEST(runNegativeBufferTest);
  CPPUNIT_TEST(runCropAndBufferTest);
  CPPUNIT_TEST_SUITE_END();

public:

  CookieCutterTest()
    : HootTestFixture("test-files/conflate/CookieCutterTest/",
                      "test-output/conflate/CookieCutterTest/")
  {
    setResetType(ResetAll);
  }

  void runTest()
  {
    OsmXmlReader reader;
    OsmSchema::getInstance().loadDefault();
    OsmMapPtr doughMap = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    OsmMapPtr cutShapeMap = std::make_shared<OsmMap>();
    reader.read(
      "test-files/algorithms/alpha-shape/AlphaShapeGeneratorTest/AlphaShapeGeneratorNegativeBufferTest-out.osm",
      cutShapeMap);

    CookieCutter(false).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    OsmXmlWriter writer;
    writer.write(cookieCutMap, _outputPath + "CookieCutterTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "CookieCutterTest.osm",
                     _outputPath + "CookieCutterTest.osm");
  }

  void runCropTest()
  {
    OsmXmlReader reader;
    OsmMapPtr doughMap = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    OsmMapPtr cutShapeMap = std::make_shared<OsmMap>();
    reader.read(
      "test-files/algorithms/alpha-shape/AlphaShapeGeneratorTest/AlphaShapeGeneratorNegativeBufferTest-out.osm",
      cutShapeMap);

    CookieCutter(true).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    OsmXmlWriter writer;
    writer.write(cookieCutMap, _outputPath + "CookieCutterCropTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "CookieCutterCropTest.osm",
                     _outputPath + "CookieCutterCropTest.osm");
  }

  void runBufferTest()
  {
    OsmXmlReader reader;
    OsmMapPtr doughMap = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    OsmMapPtr cutShapeMap = std::make_shared<OsmMap>();
    reader.read(
      "test-files/algorithms/alpha-shape/AlphaShapeGeneratorTest/AlphaShapeGeneratorNegativeBufferTest-out.osm",
      cutShapeMap);

    CookieCutter(false, 100.0).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    OsmXmlWriter writer;
    writer.write(cookieCutMap, _outputPath + "CookieCutterBufferTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "CookieCutterBufferTest.osm",
                     _outputPath + "CookieCutterBufferTest.osm");
  }

  void runNegativeBufferTest()
  {
    OsmXmlReader reader;
    OsmMapPtr doughMap = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    OsmMapPtr cutShapeMap = std::make_shared<OsmMap>();
    reader.read(
      "test-files/algorithms/alpha-shape/AlphaShapeGeneratorTest/AlphaShapeGeneratorNegativeBufferTest-out.osm",
      cutShapeMap);

    CookieCutter(false, -100.0).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    OsmXmlWriter writer;
    writer.write(cookieCutMap, _outputPath + "CookieCutterNegativeBufferTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "CookieCutterNegativeBufferTest.osm",
                     _outputPath + "CookieCutterNegativeBufferTest.osm");
  }

  void runCropAndBufferTest()
  {
    OsmXmlReader reader;
    OsmMapPtr doughMap = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", doughMap);
    OsmMapPtr cutShapeMap = std::make_shared<OsmMap>();
    reader.read(
      "test-files/algorithms/alpha-shape/AlphaShapeGeneratorTest/AlphaShapeGeneratorNegativeBufferTest-out.osm",
      cutShapeMap);

    CookieCutter(true, 100.0).cut(cutShapeMap, doughMap);
    OsmMapPtr cookieCutMap = doughMap;

    MapProjector::projectToWgs84(cookieCutMap);

    OsmXmlWriter writer;
    writer.write(cookieCutMap, _outputPath + "CookieCutterCropAndBufferTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "CookieCutterCropAndBufferTest.osm",
                     _outputPath + "CookieCutterCropAndBufferTest.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CookieCutterTest, "slow");

}
