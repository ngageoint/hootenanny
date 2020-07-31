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

static const QString ROOT_DIR = "/home/vagrant/hoot/tmp/4158/combined-data";
static const QString TEST_NAME = "ChangesetReplacementGridTest::runGridCellTest";
static const QString USER_EMAIL = TEST_NAME + "@hoottestcpp.org";
static const QString DATA_TO_REPLACE_URL = ServicesDbTestUtils::getOsmApiDbUrl().toString();
static const QString DATA_TO_REPLACE_FILE = "NOMEData.osm";
static const QString REPLACEMENT_DATA_URL =
  ServicesDbTestUtils::getDbModifyUrl(TEST_NAME).toString();
static const QString REPLACEMENT_DATA_FILE = "OSMData.osm";
static const QString CROP_INPUT_BOUNDS = ""/*-115.1541,36.2614,-115.1336,36.2775"*/;
static const QString REPLACEMENT_BOUNDS = "-115.3528,36.0919,-114.9817,36.3447";
static const QString TASK_GRID_FILE = ROOT_DIR + "/bounds.osm";

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

  ChangesetReplacementGridTest() :
  _dataToReplaceFile(DATA_TO_REPLACE_FILE),
  _replacementDataFile(REPLACEMENT_DATA_FILE)
  {
    setResetType(ResetAll);
  }

  void runGridCellTest()
  {
    _initConfig();

    _opTimer.start();
    _subTaskTimer.start();

    if (!CROP_INPUT_BOUNDS.isEmpty())
    {
      _cropInputs();
    }

    _initDbs();
    _loadDataToReplaceDb();

    int numTaskGridCells = 0;
    const std::vector<std::vector<geos::geom::Envelope>> taskGrid = _calcTaskGrid(numTaskGridCells);

    _replaceData(taskGrid, numTaskGridCells);
    _getUpdatedData();

    _cleanup();

    LOG_STATUS(
      "Entire task grid cell replaced operation finished in: " <<
      StringUtils::millisecondsToDhms(_opTimer.elapsed()));
  }

private:

  QElapsedTimer _opTimer;
  QElapsedTimer _subTaskTimer;

  QString _dataToReplaceFile;
  QString _replacementDataFile;

  void _initConfig()
  {
    conf().set(ConfigOptions::getOsmapidbBulkInserterReserveRecordIdsBeforeWritingDataKey(), true);
    conf().set(ConfigOptions::getApidbBulkInserterValidateDataKey(), true);
    conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), true);
    conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
    conf().set(ConfigOptions::getConvertBoundingBoxRemoveMissingElementsKey(), false);
    conf().set(ConfigOptions::getDebugMapsWriteKey(), false);
    conf().set(ConfigOptions::getDebugMapsRemoveMissingElementsKey(), true);
    conf().set(ConfigOptions::getMapReaderAddChildRefsWhenMissingKey(), true);
    conf().set(ConfigOptions::getApiDbEmailKey(), USER_EMAIL);
    conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), true);
    conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), true);
    conf().set(ConfigOptions::getChangesetUserIdKey(), 1);
    conf().set(ConfigOptions::getChangesetMaxSizeKey(), 999999);
    conf().set(ConfigOptions::getSnapUnconnectedWaysExistingWayNodeToleranceKey(), 0.5);
    conf().set(ConfigOptions::getSnapUnconnectedWaysSnapToleranceKey(), 5.0);
    conf().set(ConfigOptions::getDebugMapsWriteKey(), true);
    conf().set(ConfigOptions::getDebugMapsFilenameKey(), ROOT_DIR + "/debug.osm");
  }

  void _initDbs()
  {
    LOG_STATUS("Initializing the data to replace db at: " << DATA_TO_REPLACE_URL << "...");
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    ApiDb::execSqlFile(DATA_TO_REPLACE_URL, "test-files/servicesdb/users.sql");
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, DATA_TO_REPLACE_URL, true, Status::Unknown1);
    if (map->size() != 0)
    {
      throw HootException("Data to replace db is not empty at start.");
    }
    LOG_STATUS(
      "Data to replace db initialized in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _cropInputs()
  {
    MapCropper cropper(GeometryUtils::envelopeFromConfigString(CROP_INPUT_BOUNDS));
    cropper.setRemoveMissingElements(false);
    cropper.setRemoveSuperflousFeatures(false);

    LOG_STATUS(
      "Cropping data to replace from: " << ROOT_DIR << "/" << DATA_TO_REPLACE_FILE << "...");
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, ROOT_DIR + "/" + DATA_TO_REPLACE_FILE, true, Status::Unknown1);
    LOG_STATUS(
      "Data to replace pre-cropped size: " << StringUtils::formatLargeNumber(map->size()));
    cropper.apply(map);
    QString cropOutputFile = DATA_TO_REPLACE_FILE;
    cropOutputFile = cropOutputFile.replace(".osm", "-cropped.osm");
    OsmMapWriterFactory::write(map, ROOT_DIR + "/" + cropOutputFile);
    _dataToReplaceFile = cropOutputFile;
    LOG_STATUS(
      "Data to replace cropped in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    LOG_STATUS(
      "Cropping replacement data from: " << ROOT_DIR << "/" << REPLACEMENT_DATA_FILE << "...");
    map.reset(new OsmMap());
    OsmMapReaderFactory::read(map, ROOT_DIR + "/" + REPLACEMENT_DATA_FILE, true, Status::Unknown2);
    LOG_STATUS(
      "Replacement data pre-cropped size: " << StringUtils::formatLargeNumber(map->size()));
    cropper.apply(map);
    cropOutputFile = REPLACEMENT_DATA_FILE;
    cropOutputFile = cropOutputFile.replace(".osm", "-cropped.osm");
    OsmMapWriterFactory::write(map, ROOT_DIR + "/" + cropOutputFile);
    _replacementDataFile = cropOutputFile;
    LOG_STATUS(
      "Replacement data cropped in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _loadDataToReplaceDb()
  {
    LOG_STATUS(
      "Loading the data to replace db from: " << ROOT_DIR << "/" << DATA_TO_REPLACE_FILE <<
      " to: " << DATA_TO_REPLACE_URL << "...");
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, ROOT_DIR + "/" + DATA_TO_REPLACE_FILE, true, Status::Unknown1);
    OsmMapWriterFactory::write(map, DATA_TO_REPLACE_URL);
    LOG_STATUS(
      "Total elements in data being replaced: " << StringUtils::formatLargeNumber(map->size()));
    LOG_STATUS(
      "Data to replace loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  std::vector<std::vector<geos::geom::Envelope>> _calcTaskGrid(int& numTaskGridCells)
  {
    // This ends up being much slower than using a file. The bounded query needs work.
//    LOG_STATUS(
//      "Loading the replacement data db from: " << rootDir << "/" << REPLACEMENT_DATA_FILE <<
//      " to: " << REPLACEMENT_DATA_URL << "...");
//    map.reset(new OsmMap());
//    OsmMapReaderFactory::read(map, rootDir + "/" + REPLACEMENT_DATA_FILE, true, Status::Unknown2);
//    OsmMapWriterFactory::write(map, REPLACEMENT_DATA_URL);
//    LOG_STATUS(
//      "Replacing elements: " << StringUtils::formatLargeNumber(map->size()));
//    LOG_STATUS(
//      "Replacement data loaded in: " <<
//      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
//    _subTaskTimer.restart();

//    LOG_STATUS(
//      "Reading the replacement data out of the db for task grid cell calculation from: " <<
//      REPLACEMENT_DATA_URL << "...");
//    map.reset(new OsmMap());
//    // changeset derive will overwrite these as needed
//    conf().set(ConfigOptions::getConvertBoundingBoxKey(), REPLACEMENT_BOUNDS);
//    conf().set(ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(), false);
//    conf().set(
//      ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
//    conf().set(ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(), true);
//    // IMPORTANT: must be unknown1 for node density to work
//    OsmMapReaderFactory::read(map, REPLACEMENT_DATA_URL, true, Status::Unknown1);
//    LOG_STATUS(
//      "Replacement data read in: " <<
//      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
//    _subTaskTimer.restart();

    LOG_STATUS("Reading the replacement data for task grid cell calculation...");
    OsmMapPtr map(new OsmMap());
    // changeset derive will overwrite these as needed
    conf().set(ConfigOptions::getConvertBoundingBoxKey(), REPLACEMENT_BOUNDS);
    conf().set(ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(), false);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
    conf().set(ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(), true);
    // IMPORTANT: must be unknown1 for node density to work
    OsmMapReaderFactory::read(map, ROOT_DIR + "/" + REPLACEMENT_DATA_FILE, true, Status::Unknown1);
    LOG_STATUS("Replacement elements: " << StringUtils::formatLargeNumber(map->size()));
    LOG_STATUS(
      "Replacement data read in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    LOG_STATUS("Calculating task grid cells for replacement data to: " << TASK_GRID_FILE << "...");
    NodeDensityTileBoundsCalculator boundsCalc;
    boundsCalc.setPixelSize(0.001);
    boundsCalc.setMaxNodesPerTile(100000);
    boundsCalc.setMaxNumTries(3);
    boundsCalc.setMaxTimePerAttempt(300);
    boundsCalc.setPixelSizeRetryReductionFactor(10);
    boundsCalc.setFailWithNoSolution(false);
    boundsCalc.setSlop(0.1);
    boundsCalc.calculateTiles(map);
    map.reset();
    const std::vector<std::vector<geos::geom::Envelope>> taskGrid = boundsCalc.getTiles();
    numTaskGridCells = boundsCalc.getTileCount();
    LOG_VAR(taskGrid.size());
    const std::vector<std::vector<long>> nodeCounts = boundsCalc.getNodeCounts();
    TileBoundsWriter::writeTilesToOsm(taskGrid, nodeCounts, TASK_GRID_FILE);
    LOG_STATUS(
      "Calculated " << StringUtils::formatLargeNumber(numTaskGridCells) << " task grid cells.");
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
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    return taskGrid;
  }

  void _replaceData(const std::vector<std::vector<geos::geom::Envelope>>& taskGrid,
                    const int numTiles)
  {
    // recommended C&R production config
    ChangesetReplacementCreator changesetCreator(true, "", DATA_TO_REPLACE_URL);
    changesetCreator.setFullReplacement(true);
    changesetCreator.setBoundsInterpretation(
      ChangesetReplacementCreator::BoundsInterpretation::Lenient);
    changesetCreator.setWaySnappingEnabled(true);
    changesetCreator.setConflationEnabled(false);
    changesetCreator.setCleaningEnabled(false);
    changesetCreator.setTagOobConnectedWays(true);

    OsmApiDbSqlChangesetApplier changesetApplier(DATA_TO_REPLACE_URL);

    // for each task grid cell
    int boundsCtr = 1;
    for (size_t tx = 0; tx < taskGrid.size(); tx++)
    {
      for (size_t ty = 0; ty < taskGrid[tx].size(); ty++)
      {
        const QString changesetId = QString::number((int)tx) + "-" + QString::number((int)ty);
        const geos::geom::Envelope bounds = taskGrid[tx][ty];
        const QString boundsStr = GeometryUtils::toString(bounds);
        QFile changesetFile(ROOT_DIR + "/changeset-" + changesetId + ".osc.sql");

        // derive the difference between the replacement data and the being replaced
        LOG_STATUS(
          "Deriving changeset " << StringUtils::formatLargeNumber(boundsCtr) << " / " <<
          StringUtils::formatLargeNumber(numTiles) << ": " << changesetId << " over bounds: " <<
          boundsStr << " to file: " << changesetFile.fileName() << "...");
        changesetCreator.create(
          DATA_TO_REPLACE_URL, /*REPLACEMENT_DATA_URL*/ROOT_DIR + "/" + REPLACEMENT_DATA_FILE,
          bounds, changesetFile.fileName());
        LOG_STATUS(
          "Changeset derived in: " <<
          StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
        _subTaskTimer.restart();

        // apply the difference back to the data being replaced
        LOG_STATUS(
          "Applying changeset: " << StringUtils::formatLargeNumber(boundsCtr) << " / " <<
          StringUtils::formatLargeNumber(numTiles) << ": " << changesetId << " over bounds: " <<
          boundsStr << " from file: " << changesetFile.fileName() << "...");
        changesetApplier.write(changesetFile);
        LOG_STATUS(
          "Changeset applied in: " <<
          StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
        _subTaskTimer.restart();

        boundsCtr++;
      }
    }
  }

  void _getUpdatedData()
  {
    LOG_STATUS("Reading the modified data out of: " << DATA_TO_REPLACE_URL << "...");
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, DATA_TO_REPLACE_URL, true, Status::Unknown1);
    LOG_STATUS("Modified data size: " << StringUtils::formatLargeNumber(map->size()));
    OsmMapWriterFactory::write(map, ROOT_DIR + "/replaced-data.osm");
    LOG_STATUS(
      "Modified data read in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _cleanup()
  {
    LOG_STATUS("Cleaning up the modified db at: " << DATA_TO_REPLACE_URL << "...");
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    LOG_STATUS(
      "Modified db cleaned in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

//    LOG_STATUS("Cleaning up the replacement data db at: " << REPLACEMENT_DATA_URL << "...");
//    ServicesDbTestUtils::deleteUser(userEmail);
//    HootApiDbWriter().deleteMap(testName);
//    LOG_STATUS(
//      "Replacement data cleaned in: " <<
//      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
//    _subTaskTimer.restart();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementGridTest, "glacial");

}
