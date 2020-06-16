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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MapProjector.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class AlphaShapeGeneratorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AlphaShapeGeneratorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runBufferTest);
  CPPUNIT_TEST(runNegativeBufferTest);
  CPPUNIT_TEST(runAutoRetryTest);
  CPPUNIT_TEST(runManualCoverTest);
  CPPUNIT_TEST_SUITE_END();

public:

  AlphaShapeGeneratorTest() :
  HootTestFixture(
    "test-files/algorithms/alpha-shape/AlphaShapeGeneratorTest/",
    "test-output/algorithms/alpha-shape/AlphaShapeGeneratorTest/")
  {
    setResetType(ResetAll);
  }

  void runBasicTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", map);

    AlphaShapeGenerator uut(1000.0, 0.0);
    uut.setManuallyCoverSmallPointClusters(false);
    uut.setRetryOnTooSmallInitialAlpha(false);
    OsmMapPtr cutShapeMap = uut.generateMap(map);

    MapProjector::projectToWgs84(cutShapeMap);
    OsmXmlWriter writer;
    writer.write(cutShapeMap, _outputPath + "AlphaShapeGeneratorBasicTest-out.osm");

    HOOT_FILE_EQUALS(_inputPath + "AlphaShapeGeneratorBasicTest-out.osm",
                     _outputPath + "AlphaShapeGeneratorBasicTest-out.osm");
  }

  void runBufferTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", map);

    AlphaShapeGenerator uut(1000.0, 500.0);
    uut.setManuallyCoverSmallPointClusters(false);
    uut.setRetryOnTooSmallInitialAlpha(false);
    OsmMapPtr cutShapeMap = uut.generateMap(map);

    MapProjector::projectToWgs84(cutShapeMap);
    OsmXmlWriter writer;
    writer.write(cutShapeMap, _outputPath + "AlphaShapeGeneratorBufferTest-out.osm");

    HOOT_FILE_EQUALS(_inputPath + "AlphaShapeGeneratorBufferTest-out.osm",
                     _outputPath + "AlphaShapeGeneratorBufferTest-out.osm");
  }

  void runNegativeBufferTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcTigerRoads.osm", map);

    AlphaShapeGenerator uut(1000.0, -500.0);
    uut.setManuallyCoverSmallPointClusters(false);
    uut.setRetryOnTooSmallInitialAlpha(false);
    OsmMapPtr cutShapeMap = uut.generateMap(map);

    MapProjector::projectToWgs84(cutShapeMap);
    OsmXmlWriter writer;
    writer.write(cutShapeMap, _outputPath + "AlphaShapeGeneratorNegativeBufferTest-out.osm");

    HOOT_FILE_EQUALS(_inputPath + "AlphaShapeGeneratorNegativeBufferTest-out.osm",
                     _outputPath + "AlphaShapeGeneratorNegativeBufferTest-out.osm");
  }

  void runAutoRetryTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "AlphaShapeGeneratorAutoRetryTest-in.osm", map);

    AlphaShapeGenerator uut(1000.0, 0.0);
    uut.setManuallyCoverSmallPointClusters(false);
    uut.setRetryOnTooSmallInitialAlpha(true);
    OsmMapPtr cutShapeMap = uut.generateMap(map);

    MapProjector::projectToWgs84(cutShapeMap);
    OsmXmlWriter writer;
    writer.write(cutShapeMap, _outputPath + "AlphaShapeGeneratorAutoRetryTest-out.osm");

    HOOT_FILE_EQUALS(_inputPath + "AlphaShapeGeneratorAutoRetryTest-out.osm",
                     _outputPath + "AlphaShapeGeneratorAutoRetryTest-out.osm");
  }

  void runManualCoverTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "AlphaShapeGeneratorManualCoverTest-in.osm", map);

    AlphaShapeGenerator uut(10000.0, 500.0);
    uut.setManuallyCoverSmallPointClusters(true);
    uut.setRetryOnTooSmallInitialAlpha(true);
    OsmMapPtr cutShapeMap = uut.generateMap(map);

    MapProjector::projectToWgs84(cutShapeMap);
    OsmXmlWriter writer;
    writer.write(cutShapeMap, _outputPath + "AlphaShapeGeneratorManualCoverTest-out.osm");

    HOOT_FILE_EQUALS(_inputPath + "AlphaShapeGeneratorManualCoverTest-out.osm",
                     _outputPath + "AlphaShapeGeneratorManualCoverTest-out.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AlphaShapeGeneratorTest, "slow");

}
