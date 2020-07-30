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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/conflate/tile/NodeDensityTileBoundsCalculator.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/io/OsmApiDbSqlChangesetApplier.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/io/HootApiDbWriter.h>
#include <hoot/core/io/TileBoundsWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/*
 * TODO
 */
class ChangesetReplacementGridTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetReplacementGridTest);
  // FOR DEBUGGING ONLY
  CPPUNIT_TEST(runGridCellTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetReplacementGridTest()
  {
    setResetType(ResetAll);
  }

  void runGridCellTest()
  {
    const QString rootDir = "/home/vagrant/hoot/tmp/4158/combined-data";
    const QString testName = "ChangesetReplacementGridTest::runGridCellTest";
    OsmMapPtr map;
    const QString dataToReplaceUrl = ServicesDbTestUtils::getOsmApiDbUrl().toString();
    const QString replacementDataUrl = ServicesDbTestUtils::getDbModifyUrl(testName).toString();

    // config opts
    conf().set(ConfigOptions::getOsmapidbBulkInserterReserveRecordIdsBeforeWritingDataKey(), true);
    conf().set(ConfigOptions::getApidbBulkInserterValidateDataKey(), true);
    conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), true);
    conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
    conf().set(ConfigOptions::getConvertBoundingBoxRemoveMissingElementsKey(), false);
    conf().set(ConfigOptions::getDebugMapsWriteKey(), false);
    conf().set(ConfigOptions::getDebugMapsRemoveMissingElementsKey(), true);
    conf().set(ConfigOptions::getMapReaderAddChildRefsWhenMissingKey(), true);
    const QString userEmail = testName + "@hoottestcpp.org";
    conf().set(ConfigOptions::getApiDbEmailKey(), userEmail);
    conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), true);
    conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), true);
    conf().set(ConfigOptions::getChangesetUserIdKey(), 1);
    conf().set(ConfigOptions::getChangesetMaxSizeKey(), 999999);
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 0.5);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 5.0);

    QElapsedTimer opTimer;
    opTimer.start();
    QElapsedTimer subTaskTimer;
    subTaskTimer.start();

    LOG_STATUS("Initializing the data to replace db...");
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    ApiDb::execSqlFile(dataToReplaceUrl, "test-files/servicesdb/users.sql");
    map.reset(new OsmMap());
    OsmMapReaderFactory::read(map, dataToReplaceUrl, true, Status::Unknown1);
    if (map->size() != 0)
    {
      throw HootException("Data to replace db is not empty at start.");
    }
    LOG_STATUS(
      "Data to replace db initialized in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    LOG_STATUS("Loading the data to replace db...");
    map.reset(new OsmMap());
    OsmMapReaderFactory::read(map, rootDir + "/NOMEData.osm", true, Status::Unknown1);
    OsmMapWriterFactory::write(map, dataToReplaceUrl);
    LOG_STATUS("data to replace size after initial write: " << map->size());
    LOG_STATUS(
      "Data to replace loaded in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    LOG_STATUS("Loading the replacement data db...");
    map.reset(new OsmMap());
    OsmMapReaderFactory::read(map, rootDir + "/OSMData.osm", true, Status::Unknown2);
    OsmMapWriterFactory::write(map, replacementDataUrl);
    LOG_STATUS("replacement data size after initial write: " << map->size());
    LOG_STATUS(
      "Replacement data loaded in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    LOG_STATUS("Reading the data to replace out of the db...");
    map.reset(new OsmMap());
    OsmMapReaderFactory::read(map, dataToReplaceUrl, true, Status::Unknown1);
    LOG_STATUS("data to replace size after read: " << map->size());
    LOG_STATUS(
      "Data to replace read in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    LOG_STATUS("Calculating task grid cells...");
    NodeDensityTileBoundsCalculator boundsCalc;
    boundsCalc.setPixelSize(0.001);
    boundsCalc.setMaxNodesPerTile(10000);
    boundsCalc.setMaxNumTries(5);
    boundsCalc.setMaxTimePerAttempt(300);
    boundsCalc.setPixelSizeRetryReductionFactor(10);
    boundsCalc.calculateTiles(map);
    const std::vector<std::vector<geos::geom::Envelope>> taskGrid = boundsCalc.getTiles();
    const std::vector<std::vector<long>> nodeCounts = boundsCalc.getNodeCounts();
    TileBoundsWriter::writeTilesToOsm(taskGrid, nodeCounts, rootDir + "/bounds.osm");
    LOG_STATUS("Calculated " << boundsCalc.getTileCount() << " task grid cells.");
    LOG_STATUS(
      "Task grid cells calculated in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    ChangesetReplacementCreator changesetCreator(false, "", dataToReplaceUrl);
    changesetCreator.setFullReplacement(true);
    changesetCreator.setBoundsInterpretation(
      ChangesetReplacementCreator::BoundsInterpretation::Lenient);
    changesetCreator.setWaySnappingEnabled(true);
    changesetCreator.setConflationEnabled(false);
    changesetCreator.setCleaningEnabled(false);
    changesetCreator.setTagOobConnectedWays(true);
    OsmApiDbSqlChangesetApplier changesetApplier(dataToReplaceUrl);

    // derive and apply a changeset for each task grid cell
    int boundsCtr = 1;
    for (size_t tx = 0; tx < taskGrid.size(); tx++)
    {
      for (size_t ty = 0; ty < taskGrid[tx].size(); ty++)
      {
        const QString changesetId = QString::number((int)tx) + "-" + QString::number((int)ty);
        const geos::geom::Envelope bounds = taskGrid[tx][ty];
        const QString boundsStr = GeometryUtils::toString(bounds);
        const QString changesetFile = rootDir + "changeset-" + changesetId + ".osc.sql";

        LOG_STATUS(
          "Deriving changeset " << boundsCtr << " / " << boundsCalc.getTileCount() << ": " <<
          changesetId << " over bounds: " << boundsStr << "...");
        changesetCreator.create(dataToReplaceUrl, replacementDataUrl, bounds, changesetFile);
        LOG_STATUS(
          "Changeset derived in: " <<
          StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
        subTaskTimer.restart();

        LOG_STATUS(
          "Applying changeset: " << boundsCtr << " / " << boundsCalc.getTileCount() << ": " <<
          changesetId << " over bounds: " << boundsStr << "...");
        changesetApplier.write(changesetFile);
        LOG_STATUS(
          "Changeset applied in: " <<
          StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
        subTaskTimer.restart();

        boundsCtr++;
      }
    }

    LOG_STATUS("Reading the modified data...");
    map.reset(new OsmMap());
    OsmMapReaderFactory::read(map, dataToReplaceUrl, true, Status::Unknown1);
    LOG_STATUS("modified data size: " << map->size());
    OsmMapWriterFactory::write(map, rootDir + "/replaced-data.osm");
    LOG_STATUS(
      "Modified data read in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    LOG_STATUS("Cleaning up the modified db...");
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    LOG_STATUS(
      "Modified db cleaned in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    LOG_STATUS("Cleaning up the replacement data db...");
    ServicesDbTestUtils::deleteUser(userEmail);
    HootApiDbWriter().deleteMap(testName);
    LOG_STATUS(
      "Replacement data cleaned in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    LOG_STATUS(
      "Entire task grid cell replaced operation finished in: " <<
      StringUtils::millisecondsToDhms(opTimer.elapsed()));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementGridTest, "glacial");

}
