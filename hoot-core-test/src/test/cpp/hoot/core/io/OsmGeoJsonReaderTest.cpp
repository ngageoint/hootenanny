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
 * @copyright Copyright (C) 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// geos
#include <geos/io/WKTReader.h>
#include <geos/geom/Point.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmGeoJsonReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>

// TGS
#include <tgs/Statistics/Random.h>

namespace hoot
{

class OsmGeoJsonReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmGeoJsonReaderTest);
  CPPUNIT_TEST(runAllDataTypesTest);
  CPPUNIT_TEST(runDcTigerTest);
  CPPUNIT_TEST(runBostonSubsetRoadBuildingTest);
  CPPUNIT_TEST(runGenericGeoJsonTest);
  CPPUNIT_TEST(runObjectGeoJsonTest);
  CPPUNIT_TEST(runMultiObjectGeoJsonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmGeoJsonReaderTest()
    : HootTestFixture("test-files/io/GeoJson/",
                      "test-output/io/GeoJson/")
  {
    setResetType(ResetBasic);
  }

  void runAllDataTypesTest()
  {
    runTest("AllDataTypes.geojson", "AllDataTypes.osm");
  }

  void runDcTigerTest()
  {
    runTest("DcTigerRoads.geojson", "DcTigerRoads.osm");
  }

  void runBostonSubsetRoadBuildingTest()
  {
    runTest("BostonSubsetRoadBuilding.geojson", "BostonSubsetRoadBuilding.osm");
  }

  void runGenericGeoJsonTest()
  {
    runTest("CensusUnitedStates.geojson", "CensusUnitedStates.osm");
  }

  void runObjectGeoJsonTest()
  {
    runTest("SampleObjectsReader.geojson", "SampleObjectsReader.osm");
  }

  void runMultiObjectGeoJsonTest()
  {
    runTest("MultiObjectsReader.geojson", "MultiObjectsReader.osm");
  }

  void runTest(const QString& input, const QString& output)
  {
    OsmGeoJsonReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.open(_inputPath + input);
    reader.read(map);

    OsmXmlWriter writer;
    writer.open(_outputPath + output);
    writer.write(map);
    HOOT_FILE_EQUALS( _inputPath + output,
                     _outputPath + output);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmGeoJsonReaderTest, "slow");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmGeoJsonReaderTest, "current");

}
