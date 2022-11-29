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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmGeoJsonWriter.h>
#include <hoot/core/io/OsmXmlReader.h>

// TGS
#include <tgs/Statistics/Random.h>

namespace hoot
{

class OsmGeoJsonWriterTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmGeoJsonWriterTest);
  CPPUNIT_TEST(runAllDataTypesTest);
  CPPUNIT_TEST(runDcTigerTest);
  CPPUNIT_TEST(runBostonSubsetRoadBuildingTest);
  CPPUNIT_TEST(runSplitBostonRoadBuildingTest);
  CPPUNIT_TEST(runObjectGeoJsonTest);
  CPPUNIT_TEST(runObjectGeoJsonHootTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmGeoJsonWriterTest()
    : HootTestFixture("test-files/io/GeoJson/",
                      "test-output/io/GeoJson/")
  {
  }

  void runAllDataTypesTest()
  {
    runTest("test-files/conflate/unified/AllDataTypesA.osm", "AllDataTypes.geojson");
  }

  void runDcTigerTest()
  {
    runTest("test-files/DcTigerRoads.osm", "DcTigerRoads.geojson");
  }

  void runBostonSubsetRoadBuildingTest()
  {
    //  Suppress the warning from the OsmXmlReader about missing nodes for ways by temporarily changing
    //  the log level.  We expect the nodes to be missing since the Boston data has issues
    Log::WarningLevel logLevel = Log::getInstance().getLevel();
    if (Log::getInstance().getLevel() >= Log::Info)
      Log::getInstance().setLevel(Log::Error);
    runTest("test-files/BostonSubsetRoadBuilding_FromOsm.osm", "BostonSubsetRoadBuilding.geojson");
    Log::getInstance().setLevel(logLevel);
  }

  void runSplitBostonRoadBuildingTest()
  {
    //  The Boston data splits out into 18 different files
    QStringList multi_files({
            "AgricultureSrf",
            "CultureSrf",
            "FacilitySrf",
            "HydrographyCrv",
            "HydrographySrf",
            "InformationPnt",
            "o2s_A",
            "o2s_L",
            "o2s_P",
            "RecreationSrf",
            "SettlementPnt",
            "StructureCrv",
            "StructurePnt",
            "StructureSrf",
            "TransportationGroundCrv",
            "TransportationGroundPnt",
            "TransportationGroundSrf",
            "VegetationSrf"
          });
    for (int i = 0; i < multi_files.size(); ++i)
      multi_files[i] = QString("BostonSubsetRoadBuildingSplit-%1.geojson").arg(multi_files[i]);
    //  Suppress the warning from the OsmXmlReader about missing nodes for ways by temporarily changing
    //  the log level.  We expect the nodes to be missing since the Boston data has issues
    Log::WarningLevel logLevel = Log::getInstance().getLevel();
    if (Log::getInstance().getLevel() >= Log::Info)
      Log::getInstance().setLevel(Log::Error);
    Settings s;
    s.set(ConfigOptions::getGeojsonWriteThematicStructureKey(), true);
    s.set(ConfigOptions::getSchemaTranslationScriptKey(), "translations/TDSv70.js");
    runTest("test-files/BostonSubsetRoadBuilding_FromOsm.osm", "BostonSubsetRoadBuildingSplit.geojson", &s, multi_files);
    Log::getInstance().setLevel(logLevel);
  }

  void runObjectGeoJsonTest()
  {
    runTest(_inputPath + "SampleObjectsWriter.osm", "SampleObjectsWriter.geojson");
  }

  void runObjectGeoJsonHootTest()
  {
    Settings s;
    s.set(ConfigOptions::getJsonPrettyPrintKey(), true);
    runTest(_inputPath + "SampleObjectsWriter.osm", "SampleObjectsWriterHoot.geojson", &s);
  }

  void runTest(const QString& input, const QString& output, Settings* settings = nullptr, const QStringList& multiOutput = QStringList())
  {
    OsmXmlReader reader;

    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(input, map);

    OsmGeoJsonWriter writer;
    if (settings)
      writer.setConfiguration(*settings);

    writer.open(_outputPath + output);

    writer.setIncludeCircularError(false);

    writer.write(map);
    if (multiOutput.empty())
    {
      HOOT_FILE_EQUALS( _inputPath + output,
                       _outputPath + output);
    }
    else
    {
      for (const auto& filename : qAsConst(multiOutput))
      {
        HOOT_FILE_EQUALS( _inputPath + filename,
                         _outputPath + filename);
      }
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmGeoJsonWriterTest, "quick");

}
