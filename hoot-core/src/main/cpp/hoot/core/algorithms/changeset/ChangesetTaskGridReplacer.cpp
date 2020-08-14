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
#include "ChangesetTaskGridReplacer.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/conflate/tile/NodeDensityTileBoundsCalculator.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/io/TileBoundsWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/visitors/StatusUpdateVisitor.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/io/OsmApiDbSqlChangesetApplier.h>

namespace hoot
{

ChangesetTaskGridReplacer::ChangesetTaskGridReplacer() :
_originalDataSize(0),
_preloadMapsBeforeChangesetDerivation(false),
_gridType(GridType::NodeDensity),
_maxNodeDensityNodesPerCell(1000),
_reverseTaskGrid(false),
_killAfterNumChangesetDerivations(-1),
_numChangesetsDerived(0),
_totalChangesetDeriveTime(0.0),
_averageChangesetDeriveTime(0.0)
{
}

void ChangesetTaskGridReplacer::replace(const QString& toReplace, const QString& replacement)
{
  if (!toReplace.toLower().startsWith("osmapidb://"))
  {
    throw IllegalArgumentException("TODO");
  }
  // will support file for this later
  if (!replacement.toLower().startsWith("hootapidb://"))
  {
    throw IllegalArgumentException("TODO");
  }

  _opTimer.start();
  _subTaskTimer.start();

  _dataToReplaceUrl = toReplace;
  _replacementUrl = replacement;

  _initConfig();

  try
  {
    if (_preloadMapsBeforeChangesetDerivation)
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
          _killAfterNumChangesetDerivations << " changeset derivations in: " <<
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
    if (!_finalOutput.isEmpty())
    {
      _getUpdatedData(_finalOutput);
    }

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
}

void ChangesetTaskGridReplacer::_initConfig()
{
  conf().set(ConfigOptions::getOsmapidbBulkInserterReserveRecordIdsBeforeWritingDataKey(), true);
  conf().set(ConfigOptions::getApidbBulkInserterValidateDataKey(), true);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxRemoveMissingElementsKey(), false);
  conf().set(ConfigOptions::getMapReaderAddChildRefsWhenMissingKey(), true);
}

void ChangesetTaskGridReplacer::_cacheFullMaps()
{
  // turn off cropping on read
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), "");

  LOG_STATUS(
    "Caching the full data to be replaced out of the db for changeset calculation from: " <<
    _dataToReplaceUrl << "...");
  _dataToReplaceFullMap.reset(new OsmMap());
  _dataToReplaceFullMap->setName("DataToReplace");
  OsmMapReaderFactory::read(_dataToReplaceFullMap, _dataToReplaceUrl, true, Status::Unknown1);
  LOG_STATUS(
    StringUtils::formatLargeNumber(_dataToReplaceFullMap->size()) << " elements loaded in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  _replacementDataFullMap.reset(new OsmMap());
  _replacementDataFullMap->setName("ReplacementData");
  LOG_STATUS(
    "Caching the full replacement data out of the db for changeset calculation from: " <<
    _replacementUrl << "...");
  OsmMapReaderFactory::read(_replacementDataFullMap, _replacementUrl, false, Status::Unknown2);
  LOG_STATUS(
    StringUtils::formatLargeNumber(_replacementDataFullMap->size()) << " elements loaded in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  // revert back to original value
  //conf().set(ConfigOptions::getConvertBoundingBoxKey(), _nodeDensityGridBounds);
}

QMap<int, geos::geom::Envelope> ChangesetTaskGridReplacer::_getTaskGrid()
{
  QMap<int, geos::geom::Envelope> taskGrid;
  if (!_gridInput.isEmpty())
  {
    LOG_STATUS("Reading task grid file: ..." << _gridInput.right(25) << "...")
    taskGrid = GeometryUtils::readBoundsFileWithIds(_gridInput);
    LOG_STATUS(
      "Read " << StringUtils::formatLargeNumber(taskGrid.size()) << " task grid cells.");
  }
  else
  {
    taskGrid = _calcNodeDensityTaskGrid(_getNodeDensityTaskGridInput());
  }
  return taskGrid;
}

OsmMapPtr ChangesetTaskGridReplacer::_getNodeDensityTaskGridInput()
{
  OsmMapPtr map(new OsmMap());

  // changeset derive will overwrite these later, if needed
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), _nodeDensityGridBounds);
  conf().set(ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(), false);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(), true);

  // IMPORTANT: data used here must be unknown1 for node density calc to work
  if (_preloadMapsBeforeChangesetDerivation)
  {
    LOG_STATUS(
      "Preparing input data for task grid cell calculation from: " <<
      _replacementDataFullMap->getName() << "...");
    map.reset(new OsmMap(_replacementDataFullMap));
    StatusUpdateVisitor statusUpdater(Status::Unknown1);
    // LOG_STATUS(statusUpdater.getInitStatusMessage());
    map->visitRw(statusUpdater);
    MapCropper cropper(GeometryUtils::envelopeFromConfigString(_nodeDensityGridBounds));
    LOG_STATUS(cropper.getInitStatusMessage());
    cropper.apply(map);
  }
  //else if (_writeReplacementDataToDb)
  else
  {
    // read from hootapidb

    LOG_STATUS(
      "Preparing input data for task grid cell calculation from: " << _replacementUrl <<
      "...");
    // Reading by bounds ends up being much slower than it should be here. The bounded query
    // needs work (#4180). We'll read all and crop after the fact instead. This is really bad
    // memory-wise if the source db is very large, of course.
    conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), true);
    OsmMapReaderFactory::read(map, _replacementUrl, true, Status::Unknown1);
    // Restore the default setting, or the changeset replacement maps will be loaded very slowly
    // if loading from URL.
    conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), false);
  }
//  else
//  {
//    // read from file

//    const QString replacementDataPath = ROOT_DIR + "/" + _replacementDataFile;
//    LOG_STATUS(
//      "Preparing input data for task grid cell calculation: ..." <<
//      replacementDataPath.right(25) << "...");
//    OsmMapReaderFactory::read(map, ROOT_DIR + "/" + _replacementDataFile, true, Status::Unknown1);
//  }

  LOG_STATUS(
    StringUtils::formatLargeNumber(map->size()) << " task grid calc elements prepared in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  return map;
}

QMap<int, geos::geom::Envelope> ChangesetTaskGridReplacer::_calcNodeDensityTaskGrid(OsmMapPtr map)
{
  LOG_STATUS(
    "Calculating task grid cells for replacement data to: ..." <<
    _nodeDensityTaskGridOutputFile.right(25) << "...");
  NodeDensityTileBoundsCalculator boundsCalc;
  boundsCalc.setPixelSize(0.001);
  boundsCalc.setMaxNodesPerTile(_maxNodeDensityNodesPerCell);
  boundsCalc.setMaxNumTries(3);
  boundsCalc.setMaxTimePerAttempt(300);
  boundsCalc.setPixelSizeRetryReductionFactor(10);
  boundsCalc.setSlop(0.1);
  boundsCalc.calculateTiles(map);
  map.reset();
  const std::vector<std::vector<geos::geom::Envelope>> rawTaskGrid = boundsCalc.getTiles();
  const std::vector<std::vector<long>> nodeCounts = boundsCalc.getNodeCounts();
  TileBoundsWriter::writeTilesToOsm(rawTaskGrid, nodeCounts, _nodeDensityTaskGridOutputFile);

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

void ChangesetTaskGridReplacer::_replaceEntireTaskGrid(
  const QMap<int, geos::geom::Envelope>& taskGrid)
{
  // recommended C&R production config
  _changesetCreator.reset(new ChangesetReplacementCreator(true, "", _dataToReplaceUrl));
  _changesetCreator->setFullReplacement(true);
  _changesetCreator->setBoundsInterpretation(
    ChangesetReplacementCreator::BoundsInterpretation::Lenient/*Hybrid*/);
  _changesetCreator->setWaySnappingEnabled(true);
  _changesetCreator->setConflationEnabled(false/*true*/);
  _changesetCreator->setCleaningEnabled(false/*true*/);
  _changesetCreator->setTagOobConnectedWays(true);

  _changesetApplier.reset(new OsmApiDbSqlChangesetApplier(_dataToReplaceUrl));

  // for each task grid cell
    // derive the difference between the replacement data and the being replaced
    // apply the difference back to the data being replaced

  _numChangesetsDerived = 0;
  _totalChangesetDeriveTime = 0.0;
  int changesetCtr = 0;
  // probably a cleaner way to do this...
  if (!_reverseTaskGrid)
  {
    for (QMap<int, geos::geom::Envelope>::const_iterator taskGridItr = taskGrid.begin();
         taskGridItr != taskGrid.end(); ++taskGridItr)
    {
      _replaceTaskGridCell(
        taskGridItr.key(), changesetCtr + 1, taskGridItr.value(), taskGrid.size());

      if (_killAfterNumChangesetDerivations > 0 &&
          _numChangesetsDerived >= _killAfterNumChangesetDerivations)
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

      if (_killAfterNumChangesetDerivations > 0 &&
          _numChangesetsDerived >= _killAfterNumChangesetDerivations)
      {
        throw HootException(
          "Killing replacement after " + QString::number(_numChangesetsDerived) +
          " changeset derivations...");
      }

      changesetCtr++;
    }
  }
}

void ChangesetTaskGridReplacer::_replaceTaskGridCell(
  const int taskGridCellId, const int changesetNum, const geos::geom::Envelope& bounds,
  const int taskGridSize)
{
  const QString boundsStr = GeometryUtils::toString(bounds);
  QFile changesetFile(
    _changesetsOutputDir + "/changeset-cell-" + QString::number(taskGridCellId) + ".osc.sql");

  LOG_STATUS(
    "Deriving changeset " << changesetNum << " / " <<
    StringUtils::formatLargeNumber(taskGridSize) << " for task grid cell: " << taskGridCellId <<
    ", over bounds: " << boundsStr << ", to file: ..." << changesetFile.fileName().right(25) <<
    "...");
  _changesetCreator->setChangesetId(QString::number(taskGridCellId));
  if (!_preloadMapsBeforeChangesetDerivation)
  {
//    QString secondaryInput;
//    if (_writeReplacementDataToDb)
//    {
//      secondaryInput = REPLACEMENT_DATA_URL;
//    }
//    else
//    {
//      secondaryInput = ROOT_DIR + "/" + _replacementDataFile;
//    }
    _changesetCreator->create(
      _dataToReplaceUrl, _replacementUrl, bounds, changesetFile.fileName());
  }
  else
  {
    assert(!_dataToReplaceFullMap->isEmpty());
    assert(!_replacementDataFullMap->isEmpty());
    _changesetCreator->create(
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
  _changesetApplier->write(changesetFile);
  LOG_STATUS(
    "Changeset applied in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  // VERY SLOW
  if (ConfigOptions().getDebugMapsWrite() && changesetNum < taskGridSize)
  {
    _getUpdatedData(
      _changesetsOutputDir + "/" + QString::number(taskGridCellId) + "-replaced-data.osm");
  }
}

void ChangesetTaskGridReplacer::_getUpdatedData(const QString& outputFile)
{
  LOG_STATUS(
    "Reading the modified data out of: " << _dataToReplaceUrl << " to: ..." <<
    outputFile.right(25) << "...");

  // clear this out so we get all the data back
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), "");
  // change these, so we can open the files in josm; TODO: this isn't helping
  //conf().set(ConfigOptions::getConvertBoundingBoxRemoveMissingElementsKey(), true);
  //conf().set(ConfigOptions::getMapReaderAddChildRefsWhenMissingKey(), false);

  OsmMapPtr map(new OsmMap());
  OsmMapReaderFactory::read(map, _dataToReplaceUrl, true, Status::Unknown1);
  OsmMapWriterFactory::write(map, outputFile);
  LOG_STATUS(
    "Modified data read in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();
  LOG_STATUS(
    "Modified data original size: " << StringUtils::formatLargeNumber(_originalDataSize));
  LOG_STATUS("Modified data current size: " << StringUtils::formatLargeNumber(map->size()));

  //conf().set(ConfigOptions::getConvertBoundingBoxKey(), _nodeDensityGridBounds);
}

}
