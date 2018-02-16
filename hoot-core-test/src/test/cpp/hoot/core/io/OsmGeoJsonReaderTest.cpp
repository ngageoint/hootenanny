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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// geos
#include <geos/io/WKTReader.h>
#include <geos/geom/Point.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmGeoJsonReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDebug>
#include <QDir>

// TGS
#include <tgs/Statistics/Random.h>

#include "../TestUtils.h"

namespace hoot
{

class OsmGeoJsonReaderTest : public CppUnit::TestFixture
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

  void runAllDataTypesTest()
  {
    runTest("test-files/io/GeoJson/AllDataTypes.geojson", "AllDataTypes.osm");
  }

  void runDcTigerTest()
  {
    runTest("test-files/io/GeoJson/DcTigerRoads.geojson", "DcTigerRoads.osm");
  }

  void runBostonSubsetRoadBuildingTest()
  {
    runTest("test-files/io/GeoJson/BostonSubsetRoadBuilding.geojson", "BostonSubsetRoadBuilding.osm");
  }

  void runGenericGeoJsonTest()
  {
    runTest("test-files/io/GeoJson/CensusUnitedStates.geojson", "CensusUnitedStates.osm");
  }

  void runObjectGeoJsonTest()
  {
    runTest("test-files/io/GeoJson/SampleObjectsReader.geojson", "SampleObjectsReader.osm");
  }

  void runMultiObjectGeoJsonTest()
  {
    runTest("test-files/io/GeoJson/MultiObjectsReader.geojson", "MultiObjectsReader.osm");
  }

  void runTest(const QString& input, const QString& output)
  {
    OsmGeoJsonReader reader;

    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.open(input);
    reader.read(map);

    QDir().mkpath("test-output/io/GeoJson/");
    OsmXmlWriter writer;
    writer.open(QString("test-output/io/GeoJson/%1").arg(output));
    writer.write(map);
    HOOT_FILE_EQUALS(QString("test-files/io/GeoJson/%1").arg(output),
                     QString("test-output/io/GeoJson/%1").arg(output));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmGeoJsonReaderTest, "slow");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmGeoJsonReaderTest, "current");

}
