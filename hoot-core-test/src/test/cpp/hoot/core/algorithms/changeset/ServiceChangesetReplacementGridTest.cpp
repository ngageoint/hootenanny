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
#include <hoot/core/visitors/StatusUpdateVisitor.h>

namespace hoot
{

static const QString TEST_NAME = "ChangesetReplacementGridTest::runGridCellTest";
static const QString USER_EMAIL = TEST_NAME + "@hoottestcpp.org";

static const QString DATA_TO_REPLACE_URL = ServicesDbTestUtils::getOsmApiDbUrl().toString();
static const QString REPLACEMENT_DATA_URL =
  ServicesDbTestUtils::getDbModifyUrl(TEST_NAME).toString();

// end a large replacement early; set to -1 to disable
static const int KILL_AFTER_NUM_CHANGESET_DERIVATIONS = -1;
// read the replacement data just from the source file or load it into a hootapidb layer first?;
// data replaced is always loaded into osmapidb for changeset application purposes
static const bool WRITE_REPLACEMENT_DATA_TO_DB = false;
// By passing in pre-loaded maps, we can cut down on I/O quite a bit (requires memory double the
// combined size of both maps).
static const bool PRELOAD_MAPS_BEFORE_CHANGESET_DERIVATION = false;

// CROP_INPUT_BOUNDS - always for working with a subset of the input data; leave empty to disable
// TASK_GRID_BOUNDS - section of the sum off data in all task grid cells; needed for node density
//                    calc only
// MAX_NODE_DENSITY_NODES_PER_CELL - allows for attempting to cap the max number of node density
//                                   nodes per grid cell
// TASK_GRID_INPUT_FILE - overrides node density calc with a custom bounds file
// REVERSE_TASK_GRID - swap the ordering the cells for testing adjacency replacement issues

///////////////////////////////////////////////////////

// Vegas

//static const QString ROOT_DIR = "/home/vagrant/hoot/tmp/4158/combined-data";
//static const QString DATA_TO_REPLACE_FILE = "NOMEData.osm";
//static const QString REPLACEMENT_DATA_FILE = "OSMData.osm";
//static const QString NODE_DENSITY_TASK_GRID_OUTPUT_FILE = ROOT_DIR + "/bounds.osm";
//static const QString TASK_GRID_INPUT_FILE = "";
//static const bool REVERSE_TASK_GRID = false;

// 4 sq blocks of the city - 4 changesets
// FAILS ON CHANGESET APP FOR PRELOADED MAPS ONLY
//static const QString CROP_INPUT_BOUNDS = "-115.3314,36.2825,-115.2527,36.3387";
//static const QString TASK_GRID_BOUNDS = "-115.3059,36.2849,-115.2883,36.2991";
//static const long MAX_NODE_DENSITY_NODES_PER_CELL = 1000;

// ~1/4 of city - 64 changesets
// FAILS ON CHANGESET APP FOR PRELOADED MAPS ONLY WHEN HOOTAPIDB ISN'T USED
//static const QString CROP_INPUT_BOUNDS = "-115.3441,36.2012,-115.1942,36.3398";
//static const QString TASK_GRID_BOUNDS = "-115.3332,36.2178,-115.1837,36.3400";
//static const long MAX_NODE_DENSITY_NODES_PER_CELL = 10000;

// whole city - 64 changesets
//static const QString CROP_INPUT_BOUNDS = "";
//static const QString TASK_GRID_BOUNDS = "-115.3528,36.0919,-114.9817,36.3447";
//static const long MAX_NODE_DENSITY_NODES_PER_CELL = 100000;

/////////////////////////

// 1622b

static const QString ROOT_DIR = "/home/vagrant/hoot/tmp/4182";
static const QString DATA_TO_REPLACE_FILE = "329_ReferenceNOMEData_31JULY2020.osm";
static const QString REPLACEMENT_DATA_FILE = "329_SecondaryOSMData_31JULY2020.osm";
static const QString NODE_DENSITY_TASK_GRID_OUTPUT_FILE = "";
static const QString TASK_GRID_INPUT_FILE = ROOT_DIR + "/Task14and15.osm";
static const bool REVERSE_TASK_GRID = false;

static const QString CROP_INPUT_BOUNDS = "";
static const QString TASK_GRID_BOUNDS = "";
static const long MAX_NODE_DENSITY_NODES_PER_CELL = 10000;

////////////////////////////////////////////////

/*
 * This is meant as a test harness for debugging cut and replace across adjacent task grid cells.
 * Not meant to be run automatically as part of the unit tests.
 *
 * TODO: add the option to auto-generate a uniform grid
 */
class ServiceChangesetReplacementGridTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceChangesetReplacementGridTest);
  // ENABLE TEST FOR DEBUGGING ONLY
  CPPUNIT_TEST(runGridCellTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ServiceChangesetReplacementGridTest() :
  _dataToReplaceFile(DATA_TO_REPLACE_FILE),
  _originalDataSize(0),
  _replacementDataFile(REPLACEMENT_DATA_FILE),
  _changesetCreator(ChangesetReplacementCreator(true, "", DATA_TO_REPLACE_URL)),
  _changesetApplier(DATA_TO_REPLACE_URL),
  _numChangesetsDerived(0),
  _totalChangesetDeriveTime(0.0),
  _averageChangesetDeriveTime(0.0)
  {
    setResetType(ResetAll);

    _dataToReplaceFullMap.reset(new OsmMap());
    _dataToReplaceFullMap->setName("DataToReplace");
    _replacementDataFullMap.reset(new OsmMap());
    _replacementDataFullMap->setName("ReplacementData");
  }

  void runGridCellTest()
  {
    _opTimer.start();
    _subTaskTimer.start();

    _initConfig();

    try
    {
      if (!CROP_INPUT_BOUNDS.isEmpty())
      {
        _cropInputs();
      }

      _loadDataToReplaceDb();
      if (WRITE_REPLACEMENT_DATA_TO_DB)
      {
        _loadReplacementDataDb();
      }
      if (PRELOAD_MAPS_BEFORE_CHANGESET_DERIVATION)
      {
        // There needs to be some optimization here that avoids unnecessary loading of the maps.
        // Have tried to do it but keep getting changeset element ID errors, so leaving it alone for
        // now.
        _cacheFullMaps();
      }

      bool exceptionOccurred = false;
      try
      {
        _replaceEntireTaskGrid(_getTaskGrid());
      }
      catch (const HootException& e)
      {
        exceptionOccurred = true;
        const QString exceptionMsg = e.getWhat().toLower();
        if (exceptionMsg.startsWith("killing"))
        {
          LOG_STATUS(
            "Entire task grid cell replacement halted early at " <<
            KILL_AFTER_NUM_CHANGESET_DERIVATIONS << " changeset derivations in: " <<
            StringUtils::millisecondsToDhms(_opTimer.elapsed()));
        }
        else if (exceptionMsg.startsWith("error executing query"))
        {
          LOG_ERROR(
            "Error applying changeset at: " <<
            StringUtils::millisecondsToDhms(_opTimer.elapsed()) << "; Error: " << e.getWhat());
        }
        else
        {
          throw e;
        }
      }
      _getUpdatedData();

      if (!exceptionOccurred)
      {
        LOG_STATUS(
          "Task grid cell replacement operation successfully completed in: " <<
          StringUtils::millisecondsToDhms(_opTimer.elapsed()));
      }
    }
    catch (const HootException& e)
    {
      LOG_ERROR(
        "Entire task grid cell replacement operation partially completed with error at: " <<
        StringUtils::millisecondsToDhms(_opTimer.elapsed()) << "; Error: " << e.getWhat());
    }

    LOG_STATUS("Average changeset derive time: " << _averageChangesetDeriveTime << " seconds.");

    _cleanup();
  }

private:

  QElapsedTimer _opTimer;
  QElapsedTimer _subTaskTimer;

  QString _dataToReplaceFile;
  int _originalDataSize;
  QString _replacementDataFile;
  OsmMapPtr _dataToReplaceFullMap;
  OsmMapPtr _replacementDataFullMap;

  ChangesetReplacementCreator _changesetCreator;
  OsmApiDbSqlChangesetApplier _changesetApplier;
  int _numChangesetsDerived;
  // seconds
  double _totalChangesetDeriveTime;
  double _averageChangesetDeriveTime;

  void _initConfig()
  {
    conf().set(ConfigOptions::getOsmapidbBulkInserterReserveRecordIdsBeforeWritingDataKey(), true);
    conf().set(ConfigOptions::getApidbBulkInserterValidateDataKey(), true);
    conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), false);
    conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
    conf().set(ConfigOptions::getConvertBoundingBoxRemoveMissingElementsKey(), false);
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
    conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), false);
    conf().set(ConfigOptions::getChangesetReplacementCacheInputFileMapsKey(), true);
  }

  void _cropInputs()
  {
    MapCropper cropper(GeometryUtils::envelopeFromConfigString(CROP_INPUT_BOUNDS));
    cropper.setRemoveMissingElements(false);
    cropper.setRemoveSuperflousFeatures(false);

    QString cropOutputFile = DATA_TO_REPLACE_FILE;
    cropOutputFile = cropOutputFile.replace(".osm", "-cropped.osm");
    _dataToReplaceFile = cropOutputFile;
    LOG_STATUS(
      "Reading data to replace from: ..." << ROOT_DIR << "/" << DATA_TO_REPLACE_FILE <<
      " to: ..." << ROOT_DIR << "/" << _dataToReplaceFile.right(25) << "...");
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, ROOT_DIR + "/" + DATA_TO_REPLACE_FILE, true, Status::Unknown1);
    LOG_STATUS(
      "Data to replace pre-cropped size: " << StringUtils::formatLargeNumber(map->size()));
    LOG_STATUS(cropper.getInitStatusMessage());
    cropper.apply(map);
    LOG_STATUS("Data to replace cropped size: " << StringUtils::formatLargeNumber(map->size()));
    OsmMapWriterFactory::write(map, ROOT_DIR + "/" + _dataToReplaceFile);
    LOG_STATUS(
      "Data to replace cropped in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    cropOutputFile = REPLACEMENT_DATA_FILE;
    cropOutputFile = cropOutputFile.replace(".osm", "-cropped.osm");
    _replacementDataFile = cropOutputFile;
    LOG_STATUS(
      "Reading replacement data from: ..." << ROOT_DIR << "/" << REPLACEMENT_DATA_FILE <<
      " to: ..." << ROOT_DIR << "/" << _replacementDataFile.right(25) << "...");
    map.reset(new OsmMap());
    OsmMapReaderFactory::read(map, ROOT_DIR + "/" + REPLACEMENT_DATA_FILE, true, Status::Unknown2);
    LOG_STATUS(
      "Replacement data pre-cropped size: " << StringUtils::formatLargeNumber(map->size()));
    LOG_STATUS(cropper.getInitStatusMessage());
    cropper.apply(map);
    LOG_STATUS("Replacement data cropped size: " << StringUtils::formatLargeNumber(map->size()));
    OsmMapWriterFactory::write(map, ROOT_DIR + "/" + _replacementDataFile);
    LOG_STATUS(
      "Replacement data cropped in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _loadDataToReplaceDb()
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

    const QString dataToReplacePath = ROOT_DIR + "/" + _dataToReplaceFile;
    LOG_STATUS(
      "Loading the data to replace db from: ..." << dataToReplacePath.right(25) <<
      " to: " << DATA_TO_REPLACE_URL << "...");
    map.reset(new OsmMap());
    OsmMapReaderFactory::read(map, dataToReplacePath, true, Status::Unknown1);
    OsmMapWriterFactory::write(map, DATA_TO_REPLACE_URL);
    _originalDataSize = (int)map->size();
    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " elements to replace loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _loadReplacementDataDb()
  {
    const QString replacementDataPath = ROOT_DIR + "/" + _replacementDataFile;
    LOG_STATUS(
      "Loading the replacement data db from: ..." << replacementDataPath <<
      " to: " << REPLACEMENT_DATA_URL << "...");
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, replacementDataPath, false, Status::Unknown2);
    OsmMapWriterFactory::write(map, REPLACEMENT_DATA_URL);
    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " replacement elements loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _cacheFullMaps()
  {
    // turn off cropping on read
    //conf().set(ConfigOptions::getConvertBoundingBoxKey(), "");

    LOG_STATUS(
      "Caching the full data to be replaced out of the db for changeset calculation from: " <<
      DATA_TO_REPLACE_URL << "...");
    OsmMapReaderFactory::read(_dataToReplaceFullMap, DATA_TO_REPLACE_URL, true, Status::Unknown1);
    LOG_STATUS(
      StringUtils::formatLargeNumber(_dataToReplaceFullMap->size()) << " elements loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    if (WRITE_REPLACEMENT_DATA_TO_DB)
    {
      LOG_STATUS(
        "Caching the full replacement data out of the db for changeset calculation from: " <<
        REPLACEMENT_DATA_URL << "...");
      OsmMapReaderFactory::read(
        _replacementDataFullMap, REPLACEMENT_DATA_URL, false, Status::Unknown2);
    }
    else
    {
      const QString replacementDataPath = ROOT_DIR + "/" + _replacementDataFile;
      LOG_STATUS("Caching the full replacement data from: ..." << replacementDataPath << "...");
      OsmMapReaderFactory::read(
        _replacementDataFullMap, replacementDataPath, false, Status::Unknown2);
    }
    LOG_STATUS(
      StringUtils::formatLargeNumber(_replacementDataFullMap->size()) << " elements loaded in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    // revert back to original value
    //conf().set(ConfigOptions::getConvertBoundingBoxKey(), TASK_GRID_BOUNDS);
  }

  QMap<int, geos::geom::Envelope> _getTaskGrid()
  {
    QMap<int, geos::geom::Envelope> taskGrid;
    if (!TASK_GRID_INPUT_FILE.isEmpty())
    {
      LOG_STATUS("Reading task grid file: ..." << TASK_GRID_INPUT_FILE.right(25) << "...")
      taskGrid = GeometryUtils::readBoundsFileWithIds(TASK_GRID_INPUT_FILE);
      LOG_STATUS(
        "Read " << StringUtils::formatLargeNumber(taskGrid.size()) << " task grid cells.");
    }
    else
    {
      taskGrid = _calcNodeDensityTaskGrid(_getNodeDensityTaskGridInput());
    }
    return taskGrid;
  }

  OsmMapPtr _getNodeDensityTaskGridInput()
  {
    OsmMapPtr map(new OsmMap());

    // changeset derive will overwrite these later, if needed
    conf().set(ConfigOptions::getConvertBoundingBoxKey(), TASK_GRID_BOUNDS);
    conf().set(ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(), false);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
    conf().set(ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(), true);

    // IMPORTANT: data used here must be unknown1 for node density calc to work
    if (PRELOAD_MAPS_BEFORE_CHANGESET_DERIVATION)
    {
      LOG_STATUS(
        "Preparing input data for task grid cell calculation from: " <<
        _replacementDataFullMap->getName() << "...");
      map.reset(new OsmMap(_replacementDataFullMap));
      StatusUpdateVisitor statusUpdater(Status::Unknown1);
      // LOG_STATUS(statusUpdater.getInitStatusMessage());
      map->visitRw(statusUpdater);
      MapCropper cropper(GeometryUtils::envelopeFromConfigString(TASK_GRID_BOUNDS));
      LOG_STATUS(cropper.getInitStatusMessage());
      cropper.apply(map);
    }
    else if (WRITE_REPLACEMENT_DATA_TO_DB)
    {
      // read from hootapidb

      LOG_STATUS(
        "Preparing input data for task grid cell calculation from: " << REPLACEMENT_DATA_URL <<
        "...");
      // Reading by bounds ends up being much slower than it should be here. The bounded query
      // needs work (#4180). We'll read all and crop after the fact instead. This is really bad
      // memory-wise if the source db is very large, of course.
      conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), true);
      OsmMapReaderFactory::read(map, REPLACEMENT_DATA_URL, true, Status::Unknown1);
      // Restore the default setting, or the changeset replacement maps will be loaded very slowly
      // if loading from URL.
      conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), false);
    }
    else
    {
      // read from file

      const QString replacementDataPath = ROOT_DIR + "/" + _replacementDataFile;
      LOG_STATUS(
        "Preparing input data for task grid cell calculation: ..." <<
        replacementDataPath.right(25) << "...");
      OsmMapReaderFactory::read(map, ROOT_DIR + "/" + _replacementDataFile, true, Status::Unknown1);
    }

    LOG_STATUS(
      StringUtils::formatLargeNumber(map->size()) << " task grid calc elements prepared in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    return map;
  }

  QMap<int, geos::geom::Envelope> _calcNodeDensityTaskGrid(OsmMapPtr map)
  { 
    LOG_STATUS(
      "Calculating task grid cells for replacement data to: ..." <<
      NODE_DENSITY_TASK_GRID_OUTPUT_FILE.right(25) << "...");
    NodeDensityTileBoundsCalculator boundsCalc;
    boundsCalc.setPixelSize(0.001);
    boundsCalc.setMaxNodesPerTile(MAX_NODE_DENSITY_NODES_PER_CELL);
    boundsCalc.setMaxNumTries(3);
    boundsCalc.setMaxTimePerAttempt(300);
    boundsCalc.setPixelSizeRetryReductionFactor(10);
    boundsCalc.setSlop(0.1);
    boundsCalc.calculateTiles(map);
    map.reset();
    const std::vector<std::vector<geos::geom::Envelope>> rawTaskGrid = boundsCalc.getTiles();
    const std::vector<std::vector<long>> nodeCounts = boundsCalc.getNodeCounts();
    TileBoundsWriter::writeTilesToOsm(rawTaskGrid, nodeCounts, NODE_DENSITY_TASK_GRID_OUTPUT_FILE);

    // flatten the collection; use a list to maintain order
    QMap<int, geos::geom::Envelope> taskGrid;
    int cellCtr = 1;
    for (size_t tx = 0; tx < rawTaskGrid.size(); tx++)
    {
      for (size_t ty = 0; ty < rawTaskGrid[tx].size(); ty++)
      {
        taskGrid[cellCtr] = rawTaskGrid[tx][ty];
        cellCtr++;
      }
    }
    assert(boundsCalc.getTileCount() == taskGrid.size());

    LOG_STATUS(
      "Calculated " << StringUtils::formatLargeNumber(taskGrid.size()) << " task grid cells.");
    LOG_STATUS(
      "Maximum node count in any one tile: " <<
      StringUtils::formatLargeNumber(boundsCalc.getMaxNodeCountInOneTile()));
    LOG_STATUS(
      "Minimum node count in any one tile: " <<
      StringUtils::formatLargeNumber(boundsCalc.getMinNodeCountInOneTile()));
    LOG_INFO("Pixel size used: " << boundsCalc.getPixelSize());
    LOG_INFO(
      "Maximum node count per tile used: " <<
      StringUtils::formatLargeNumber(boundsCalc.getMaxNodesPerTile()));
    LOG_STATUS(
      "Task grid cells calculated in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    return taskGrid;
  }

  void _replaceEntireTaskGrid(const QMap<int, geos::geom::Envelope>& taskGrid)
  {
    // recommended C&R production config
    _changesetCreator.setFullReplacement(true);
    _changesetCreator.setBoundsInterpretation(
      ChangesetReplacementCreator::BoundsInterpretation::Lenient);
    _changesetCreator.setWaySnappingEnabled(true);
    _changesetCreator.setConflationEnabled(false);
    _changesetCreator.setCleaningEnabled(false);
    _changesetCreator.setTagOobConnectedWays(true);

    // for each task grid cell
      // derive the difference between the replacement data and the being replaced
      // apply the difference back to the data being replaced

    _numChangesetsDerived = 0;
    _totalChangesetDeriveTime = 0.0;
    int changesetCtr = 0;
    // probably a cleaner way to do this...
    if (!REVERSE_TASK_GRID)
    {
      for (QMap<int, geos::geom::Envelope>::const_iterator taskGridItr = taskGrid.begin();
           taskGridItr != taskGrid.end(); ++taskGridItr)
      {
        _replaceTaskGridCell(
          taskGridItr.key(), changesetCtr + 1, taskGridItr.value(), taskGrid.size());

        if (KILL_AFTER_NUM_CHANGESET_DERIVATIONS > 0 &&
            _numChangesetsDerived >= KILL_AFTER_NUM_CHANGESET_DERIVATIONS)
        {
          throw HootException(
            "Killing replacement after " + QString::number(_numChangesetsDerived) +
            " changeset derivations...");
        }

        changesetCtr++;
      }
    }
    else
    {
      QMapIterator<int, geos::geom::Envelope> taskGridItr(taskGrid);
      taskGridItr.toBack();
      while (taskGridItr.hasPrevious())
      {
        taskGridItr.previous();

        _replaceTaskGridCell(
          taskGridItr.key(), changesetCtr + 1, taskGridItr.value(), taskGrid.size());

        if (KILL_AFTER_NUM_CHANGESET_DERIVATIONS > 0 &&
            _numChangesetsDerived >= KILL_AFTER_NUM_CHANGESET_DERIVATIONS)
        {
          throw HootException(
            "Killing replacement after " + QString::number(_numChangesetsDerived) +
            " changeset derivations...");
        }

        changesetCtr++;
      }
    }
  }

  void _replaceTaskGridCell(const int taskGridCellId, const int changesetNum,
                            const geos::geom::Envelope& bounds, const int taskGridSize)
  {
    const QString boundsStr = GeometryUtils::toString(bounds);
    QFile changesetFile(
      ROOT_DIR + "/changeset-cell-" + QString::number(taskGridCellId) + ".osc.sql");

    LOG_STATUS(
      "Deriving changeset " << changesetNum << " / " <<
      StringUtils::formatLargeNumber(taskGridSize) << " for task grid cell: " << taskGridCellId <<
      ", over bounds: " << boundsStr << ", to file: ..." << changesetFile.fileName().right(25) <<
      "...");
    _changesetCreator.setChangesetId(QString::number(taskGridCellId));
    if (!PRELOAD_MAPS_BEFORE_CHANGESET_DERIVATION)
    {
      QString secondaryInput;
      if (WRITE_REPLACEMENT_DATA_TO_DB)
      {
        secondaryInput = REPLACEMENT_DATA_URL;
      }
      else
      {
        secondaryInput = ROOT_DIR + "/" + _replacementDataFile;
      }
      _changesetCreator.create(
        DATA_TO_REPLACE_URL, secondaryInput, bounds, changesetFile.fileName());
    }
    else
    {
      assert(!_dataToReplaceFullMap->isEmpty());
      assert(!_replacementDataFullMap->isEmpty());
      _changesetCreator.create(
        _dataToReplaceFullMap, _replacementDataFullMap, bounds, changesetFile.fileName());
    }
    _numChangesetsDerived++;
    _totalChangesetDeriveTime += _subTaskTimer.elapsed() / 1000.0;
    _averageChangesetDeriveTime = _totalChangesetDeriveTime / (double)_numChangesetsDerived;
    _subTaskTimer.restart();

    LOG_STATUS("Average changeset derive time: " << _averageChangesetDeriveTime << " seconds.");

    LOG_STATUS(
      "Applying changeset: " << changesetNum << " / " <<
      StringUtils::formatLargeNumber(taskGridSize) << " for task grid cell: " << taskGridCellId <<
      ", over bounds: " << boundsStr << ", from file: ..." << changesetFile.fileName().right(25) <<
      "...");
    _changesetApplier.write(changesetFile);
    LOG_STATUS(
      "Changeset applied in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
  }

  void _getUpdatedData()
  {
    const QString replaceDataFile = ROOT_DIR + "/replaced-data.osm";
    LOG_STATUS(
      "Reading the modified data out of: " << DATA_TO_REPLACE_URL << " to: ..." <<
      replaceDataFile.right(25) << "...");

    // clear this out so we get all the data back
    conf().set(ConfigOptions::getConvertBoundingBoxKey(), "");
    // change these, so we can open the files in josm; TODO: this isn't helping
    conf().set(ConfigOptions::getConvertBoundingBoxRemoveMissingElementsKey(), true);
    conf().set(ConfigOptions::getMapReaderAddChildRefsWhenMissingKey(), false);

    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, DATA_TO_REPLACE_URL, true, Status::Unknown1);
    OsmMapWriterFactory::write(map, ROOT_DIR + "/replaced-data.osm");
    LOG_STATUS(
      "Modified data read in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();
    LOG_STATUS(
      "Modified data original size: " << StringUtils::formatLargeNumber(_originalDataSize));
    LOG_STATUS("Modified data current size: " << StringUtils::formatLargeNumber(map->size()));
  }

  void _cleanup()
  {
    LOG_STATUS("Cleaning up the modified db at: " << DATA_TO_REPLACE_URL << "...");
    ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase();
    LOG_STATUS(
      "Modified db cleaned in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
    _subTaskTimer.restart();

    if (WRITE_REPLACEMENT_DATA_TO_DB)
    {
      LOG_STATUS("Cleaning up the replacement data db at: " << REPLACEMENT_DATA_URL << "...");
      ServicesDbTestUtils::deleteUser(USER_EMAIL);
      HootApiDbWriter().deleteMap(TEST_NAME);
      LOG_STATUS(
        "Replacement data cleaned in: " <<
        StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
      _subTaskTimer.restart();
    }
  }
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceChangesetReplacementGridTest, "glacial");
#endif

}
