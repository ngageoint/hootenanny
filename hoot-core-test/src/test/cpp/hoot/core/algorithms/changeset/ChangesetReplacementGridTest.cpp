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
#include <hoot/core/ops/MapCropper.h>

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
    QString dataToReplaceFile = "NOMEData.osm";
    //const QString replacementDataUrl = ServicesDbTestUtils::getDbModifyUrl(testName).toString();
    QString replacementDataFile = "OSMData.osm";
    const QString cropInputBounds = ""/*-115.1541,36.2614,-115.1336,36.2775"*/;
    const QString replacementBounds = "-115.3952,35.8639,-114.8036,36.5296";

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
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), rootDir + "/debug.osm");

    QElapsedTimer opTimer;
    opTimer.start();
    QElapsedTimer subTaskTimer;
    subTaskTimer.start();

    if (!cropInputBounds.isEmpty())
    {
      MapCropper cropper(GeometryUtils::envelopeFromConfigString(cropInputBounds));
      cropper.setRemoveMissingElements(false);
      cropper.setRemoveSuperflousFeatures(false);

      LOG_STATUS("Cropping data to replace...");
      map.reset(new OsmMap());
      OsmMapReaderFactory::read(map, rootDir + "/" + dataToReplaceFile, true, Status::Unknown1);
      LOG_STATUS(
        "Data to replace pre-cropped size: " << StringUtils::formatLargeNumber(map->size()));
      cropper.apply(map);
      QString cropOutputFile = dataToReplaceFile.replace(".osm", "-cropped.osm");
      OsmMapWriterFactory::write(map, rootDir + "/" + cropOutputFile);
      dataToReplaceFile = cropOutputFile;
      LOG_STATUS(
        "Data to replace cropped in: " <<
        StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
      subTaskTimer.restart();

      LOG_STATUS("Cropping replacement data...");
      map.reset(new OsmMap());
      OsmMapReaderFactory::read(map, rootDir + "/" + replacementDataFile, true, Status::Unknown2);
      LOG_STATUS(
        "Replacement data pre-cropped size: " << StringUtils::formatLargeNumber(map->size()));
      cropper.apply(map);
      cropOutputFile = replacementDataFile.replace(".osm", "-cropped.osm");
      OsmMapWriterFactory::write(map, rootDir + "/" + cropOutputFile);
      replacementDataFile = cropOutputFile;
      LOG_STATUS(
        "Replacement data cropped in: " <<
        StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
      subTaskTimer.restart();
    }

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
    OsmMapReaderFactory::read(map, rootDir + "/" + dataToReplaceFile, true, Status::Unknown1);
    OsmMapWriterFactory::write(map, dataToReplaceUrl);
    LOG_STATUS(
      "Elements being replaced: " << StringUtils::formatLargeNumber(map->size()));
    LOG_STATUS(
      "Data to replace loaded in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    // TODO: Do we need the replacement data in a db at all?; the crop query is really slow for this
    // data
//    LOG_STATUS("Loading the replacement data db...");
//    map.reset(new OsmMap());
//    OsmMapReaderFactory::read(map, rootDir + "/" + replacementDataFile, true, Status::Unknown2);
//    OsmMapWriterFactory::write(map, replacementDataUrl);
//    LOG_STATUS(
//      "Replacing elements: " << StringUtils::formatLargeNumber(map->size()));
//    LOG_STATUS(
//      "Replacement data loaded in: " <<
//      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
//    subTaskTimer.restart();

//    LOG_STATUS("Reading the replacement data out of the db for task grid cell calculation...");
//    map.reset(new OsmMap());
//    OsmMapReaderFactory::read(map, replacementDataUrl, true, Status::Unknown2);
//    LOG_STATUS(
//      "Replacement data read in: " <<
//      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
//    subTaskTimer.restart();

    LOG_STATUS("Reading the replacement data for task grid cell calculation...");
    map.reset(new OsmMap());
    conf().set(ConfigOptions::getConvertBoundingBoxKey(), replacementBounds);
    OsmMapReaderFactory::read(map, rootDir + "/" + replacementDataFile, true, Status::Unknown2);
    conf().set(ConfigOptions::getConvertBoundingBoxKey(), "");
    conf().set(ConfigOptions::getDebugMapsWriteKey(), true);
    OsmMapWriterFactory::writeDebugMap(map, "task-grid-calc-map");
    conf().set(ConfigOptions::getDebugMapsWriteKey(), false);
    LOG_STATUS("Replacement elements: " << StringUtils::formatLargeNumber(map->size()));
    LOG_STATUS(
      "Replacement data read in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    // TODO: this is wrong
//    LOG_STATUS("Reading the data to replace for task grid cell calculation...");
//    map.reset(new OsmMap());
//    conf().set(ConfigOptions::getConvertBoundingBoxKey(), replacementBounds);
//    OsmMapReaderFactory::read(map, dataToReplaceUrl, true, Status::Unknown1);
//    conf().set(ConfigOptions::getConvertBoundingBoxKey(), "");
//    conf().set(ConfigOptions::getDebugMapsWriteKey(), true);
//    OsmMapWriterFactory::writeDebugMap(map, "task-grid-calc-map");
//    conf().set(ConfigOptions::getDebugMapsWriteKey(), false);
//    LOG_STATUS(
//      "Data to replace read in: " <<
//      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
//    subTaskTimer.restart();

    LOG_STATUS("Calculating task grid cells...");
    NodeDensityTileBoundsCalculator boundsCalc;
    boundsCalc.setPixelSize(0.001);
    boundsCalc.setMaxNodesPerTile(50000);
    boundsCalc.setMaxNumTries(3);
    boundsCalc.setMaxTimePerAttempt(300);
    boundsCalc.setPixelSizeRetryReductionFactor(10);
    boundsCalc.setFailWithNoSolution(false);
    boundsCalc.setSlop(0.1);
    boundsCalc.calculateTiles(map);
    map.reset();
    const std::vector<std::vector<geos::geom::Envelope>> taskGrid = boundsCalc.getTiles();
    LOG_VAR(taskGrid.size());
    const std::vector<std::vector<long>> nodeCounts = boundsCalc.getNodeCounts();
    TileBoundsWriter::writeTilesToOsm(taskGrid, nodeCounts, rootDir + "/bounds.osm");
    LOG_STATUS(
      "Calculated " << StringUtils::formatLargeNumber(boundsCalc.getTileCount()) <<
      " task grid cells.");
    LOG_STATUS(
      "Maximum node count in any one tile: " <<
      StringUtils::formatLargeNumber(boundsCalc.getMaxNodeCountInOneTile()));
    LOG_STATUS(
      "Minimum node count in any one tile: " <<
      StringUtils::formatLargeNumber(boundsCalc.getMinNodeCountInOneTile()));
    LOG_STATUS("Pixel size used: " << boundsCalc.getPixelSize());
    LOG_STATUS(
      "Maximum node count per tile used: " <<
      StringUtils::formatLargeNumber(boundsCalc.getMaxNodesPerTile()));
    LOG_STATUS(
      "Task grid cells calculated in: " <<
      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
    subTaskTimer.restart();

    ChangesetReplacementCreator changesetCreator(true, "", dataToReplaceUrl);
    changesetCreator.setFullReplacement(true);
    changesetCreator.setBoundsInterpretation(
      ChangesetReplacementCreator::BoundsInterpretation::Lenient);
    changesetCreator.setWaySnappingEnabled(true);
    changesetCreator.setConflationEnabled(false);
    changesetCreator.setCleaningEnabled(false);
    changesetCreator.setTagOobConnectedWays(true);

    OsmApiDbSqlChangesetApplier changesetApplier(dataToReplaceUrl);

    // for each task grid cell
    int boundsCtr = 1;
    for (size_t tx = 0; tx < taskGrid.size(); tx++)
    {
      for (size_t ty = 0; ty < taskGrid[tx].size(); ty++)
      {
        const QString changesetId = QString::number((int)tx) + "-" + QString::number((int)ty);
        const geos::geom::Envelope bounds = taskGrid[tx][ty];
        const QString boundsStr = GeometryUtils::toString(bounds);
        const QString changesetFile = rootDir + "changeset-" + changesetId + ".osc.sql";

        // derive the difference between the replacement data and the being replaced
        LOG_STATUS(
          "Deriving changeset " << StringUtils::formatLargeNumber(boundsCtr) << " / " <<
          StringUtils::formatLargeNumber(boundsCalc.getTileCount()) << ": " << changesetId <<
          " over bounds: " << boundsStr << "...");
        changesetCreator.create(
          dataToReplaceUrl, /*replacementDataUrl*/rootDir + "/" + replacementDataFile, bounds,
          changesetFile);
        LOG_STATUS(
          "Changeset derived in: " <<
          StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
        subTaskTimer.restart();

        // apply the difference back to the data being replaced
        LOG_STATUS(
          "Applying changeset: " << StringUtils::formatLargeNumber(boundsCtr) << " / " <<
          StringUtils::formatLargeNumber(boundsCalc.getTileCount()) << ": " << changesetId <<
          " over bounds: " << boundsStr << "...");
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
    LOG_STATUS("Modified data size: " << StringUtils::formatLargeNumber(map->size()));
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

//    LOG_STATUS("Cleaning up the replacement data db...");
//    ServicesDbTestUtils::deleteUser(userEmail);
//    HootApiDbWriter().deleteMap(testName);
//    LOG_STATUS(
//      "Replacement data cleaned in: " <<
//      StringUtils::millisecondsToDhms(subTaskTimer.elapsed()));
//    subTaskTimer.restart();

    LOG_STATUS(
      "Entire task grid cell replaced operation finished in: " <<
      StringUtils::millisecondsToDhms(opTimer.elapsed()));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementGridTest, "glacial");

}
