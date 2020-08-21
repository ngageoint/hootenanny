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
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>
#include <hoot/core/visitors/RemoveInvalidRelationVisitor.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>

namespace hoot
{

ChangesetTaskGridReplacer::ChangesetTaskGridReplacer() :
_originalDataSize(0),
_gridType(GridType::NodeDensity),
_reverseTaskGrid(false),
_readNodeDensityInputFullThenCrop(false),
_maxNodeDensityNodesPerCell(1000),
_uniformGridDimensionSize(2),
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
    throw IllegalArgumentException("Data being replaced must be from an OSM API database.");
  }
  if (!replacement.toLower().startsWith("hootapidb://"))
  {
    throw IllegalArgumentException("Replacement data must be from a Hootenanny API database.");
  }
  // TODO: add input error handling for task grid generation type

  _opTimer.start();
  _subTaskTimer.start();

  _dataToReplaceUrl = toReplace;
  _replacementUrl = replacement;
  _initChangesetStats();

  _initConfig();

  try
  {
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

void ChangesetTaskGridReplacer::_initChangesetStats()
{
  _changesetStats.clear();
  _changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_CREATE_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_MODIFY_KEY] = 0;
  _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY] = 0;
}

QList<ChangesetTaskGridReplacer::TaskGridCell> ChangesetTaskGridReplacer::_getTaskGrid()
{
  QList<TaskGridCell> taskGrid;
  if (_gridType == GridType::InputFile)
  {
    if (_gridInputs.isEmpty())
    {
      throw IllegalArgumentException("TODO");
    }

    taskGrid = _getTaskGridFromBoundsFiles(_gridInputs);
  }
  else if (_gridType == GridType::NodeDensity)
  {
    if (_taskGridOutputFile.trimmed().isEmpty())
    {
      throw IllegalArgumentException("TODO");
    }

    taskGrid = _calcNodeDensityTaskGrid(_getNodeDensityTaskGridInput());
  }
  else
  {
    if (_taskGridOutputFile.trimmed().isEmpty())
    {
      throw IllegalArgumentException("TODO");
    }

    taskGrid = _getUniformTaskGrid(_uniformGridDimensionSize);
  }
  return taskGrid;
}

QList<ChangesetTaskGridReplacer::TaskGridCell> ChangesetTaskGridReplacer::_getTaskGridFromBoundsFiles(
  const QStringList& inputs)
{
  LOG_INFO("Reading " << inputs.size() << " task grid file(s)...");
  QList<TaskGridCell> taskGrid;
  QMap<int, geos::geom::Envelope> taskGridTemp;
  for (int i = 0; i < inputs.size(); i++)
  {
    const QString gridInput = inputs.at(i);
    LOG_INFO("Reading task grid file: ..." << gridInput.right(25) << "...");
    taskGridTemp = GeometryUtils::readBoundsFileWithIds(gridInput);
    for (QMap<int, geos::geom::Envelope>::const_iterator taskGridTempItr = taskGridTemp.begin();
         taskGridTempItr != taskGridTemp.end(); ++taskGridTempItr)
    {
      TaskGridCell taskGridCell;
      taskGridCell.id = taskGridTempItr.key();
      // We don't know the node count when reading the custom grid file.
      taskGridCell.replacementNodeCount = -1;
      taskGridCell.bounds = taskGridTempItr.value();
      taskGrid.append(taskGridCell);
    }
  }
  LOG_STATUS(
    "Read " << StringUtils::formatLargeNumber(taskGrid.size()) << " task grid cells from " <<
    inputs.size() << " file(s).");
  return taskGrid;
}

QList<ChangesetTaskGridReplacer::TaskGridCell> ChangesetTaskGridReplacer::_getUniformTaskGrid(
  const int dimensionSize)
{
  LOG_INFO(
    "Creating uniform task grid with " << dimensionSize << "x" << dimensionSize <<
    " cells across bounds: " << _taskGridBounds << "...");

  QList<TaskGridCell> taskGrid;

  const geos::geom::Envelope taskGridEnv = GeometryUtils::envelopeFromConfigString(_taskGridBounds);
  const double widthPerCell = taskGridEnv.getWidth() / (double)dimensionSize;
  const double heightPerCell = taskGridEnv.getHeight() / (double)dimensionSize;

  const int rows = ceil(taskGridEnv.getHeight() / heightPerCell);
  const int cols = ceil(taskGridEnv.getWidth() / widthPerCell);

  double xLeftOrigin = taskGridEnv.getMinX();
  double xRightOrigin = taskGridEnv.getMinX() + widthPerCell;
  const double yTopOrigin = taskGridEnv.getMaxY();
  const double yBottomOrigin = taskGridEnv.getMaxY() - heightPerCell;

  int cellCtr = 1;
  // used to write boundaries to file after the grid is created
  QList<geos::geom::Envelope> boundaries;
  for (int i = 0; i < cols; i++)
  {
    double cellYTop = yTopOrigin;
    double cellYBottom = yBottomOrigin;

    for (int j = 0; j < rows; j++)
    {
      TaskGridCell taskGridCell;
      taskGridCell.id = cellCtr;
      // We don't know the individual cell node counts when creating a uniform grid.
      taskGridCell.replacementNodeCount = -1;
      geos::geom::Envelope taskGridCellEnv(xLeftOrigin, xRightOrigin, cellYBottom, cellYTop);
      taskGridCell.bounds = taskGridCellEnv;
      boundaries.append(taskGridCell.bounds);
      taskGrid.append(taskGridCell);

      cellYTop = cellYTop - heightPerCell;
      cellYBottom = cellYBottom - heightPerCell;

      cellCtr++;
    }

    xLeftOrigin = xLeftOrigin + widthPerCell;
    xRightOrigin = xRightOrigin + widthPerCell;
  }

  // write out task grid to file
  OsmMapWriterFactory::write(
    GeometryUtils::createMapFromBoundsCollection(boundaries), _taskGridOutputFile);

  LOG_STATUS(
    "Created uniform task grid with " << dimensionSize << "x" << dimensionSize <<
    " cells across bounds: " << _taskGridBounds << "...");
  return taskGrid;
}

OsmMapPtr ChangesetTaskGridReplacer::_getNodeDensityTaskGridInput()
{
  // changeset derive will overwrite these later, if needed
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), _taskGridBounds);
  conf().set(ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(), false);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(), true);

  // TODO: replace the string truncation lengths with getProgressVarPrintLengthMax
  LOG_STATUS(
    "Preparing input data for task grid cell calculation from: ..." <<
    _replacementUrl.right(25) << ", across bounds: " << _taskGridBounds << "...");
  assert(HootApiDbReader::isSupported(_replacementUrl));

  // Getting large amounts of data via bbox can actually be slower than reading all of the data
  // and then cropping it, due to bounded query performance. #4192 improved this gap, but the
  // full read then crop can still be faster sometimes if the memory is available for it. All of
  // North Vegas takes 2:01 to read fully then crop to just the city bounds vs 2:30 to get via
  // bbox query.
  if (_readNodeDensityInputFullThenCrop)
  {
    conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), true);
  }

  // IMPORTANT: data used here must be unknown1 for node density calc to work
  OsmMapPtr map(new OsmMap());
  // small optimization since node density only needs nodes in the input; can only do this with
  // a db reader right now
  // TODO: I don't think the node only read is working correctly. Put a note in
  // NodeDensityTilesCmd.
//    std::shared_ptr<ApiDbReader> reader =
//      std::dynamic_pointer_cast<ApiDbReader>(
//        OsmMapReaderFactory::createReader(_replacementUrl, true, Status::Unknown1));
//    reader->setReturnNodesOnly(true);
//    reader->open(_replacementUrl);
//    reader->read(map);
  OsmMapReaderFactory::read(map, _replacementUrl, true, Status::Unknown1);

  // Restore the default setting if it was changed, or the changeset replacement maps will be loaded
  // very slowly if loading from URL.
  conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), false);

  LOG_STATUS(
    StringUtils::formatLargeNumber(map->size()) << " task grid calc elements prepared " <<
    "in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  return map;
}

QList<ChangesetTaskGridReplacer::TaskGridCell> ChangesetTaskGridReplacer::_calcNodeDensityTaskGrid(
  OsmMapPtr map)
{
  LOG_STATUS(
    "Calculating task grid cells for replacement data to: ..." <<
    _taskGridOutputFile.right(25) << "...");
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
  TileBoundsWriter::writeTilesToOsm(rawTaskGrid, nodeCounts, _taskGridOutputFile);

  // flatten the collection; use a list to maintain order
  QList<TaskGridCell> taskGrid;
  int cellCtr = 1;
  for (size_t tx = 0; tx < rawTaskGrid.size(); tx++)
  {
    for (size_t ty = 0; ty < rawTaskGrid[tx].size(); ty++)
    {
      TaskGridCell taskGridCell;
      taskGridCell.id = cellCtr;
      taskGridCell.replacementNodeCount = nodeCounts[tx][ty];
      taskGridCell.bounds = rawTaskGrid[tx][ty];
      taskGrid.append(taskGridCell);
      cellCtr++;
    }
  }
  assert(boundsCalc.getTileCount() == taskGrid.size());

  LOG_STATUS(
    "Calculated " << StringUtils::formatLargeNumber(taskGrid.size()) << " task grid cells in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()) << ".");
  LOG_STATUS(
    "\tMaximum node count in any one tile: " <<
    StringUtils::formatLargeNumber(boundsCalc.getMaxNodeCountInOneTile()));
  LOG_STATUS(
    "\tMinimum node count in any one tile: " <<
    StringUtils::formatLargeNumber(boundsCalc.getMinNodeCountInOneTile()));
  LOG_INFO("Pixel size used: " << boundsCalc.getPixelSize());
  LOG_INFO(
    "\tMaximum node count per tile used: " <<
    StringUtils::formatLargeNumber(boundsCalc.getMaxNodesPerTile()));
  _subTaskTimer.restart();

  return taskGrid;
}

void ChangesetTaskGridReplacer::_replaceEntireTaskGrid(const QList<TaskGridCell>& taskGrid)
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
  // probably a cleaner way to do this reversal handling...
  if (!_reverseTaskGrid)
  {
    for (QList<TaskGridCell>::const_iterator taskGridItr = taskGrid.begin();
         taskGridItr != taskGrid.end(); ++taskGridItr)
    {
      const TaskGridCell taskGridCell = *taskGridItr;
      _replaceTaskGridCell(taskGridCell, changesetCtr + 1, taskGrid.size());

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
    for (QList<TaskGridCell>::const_reverse_iterator taskGridItr = taskGrid.crbegin();
         taskGridItr != taskGrid.crend(); ++taskGridItr)
    {
      const TaskGridCell taskGridCell = *taskGridItr;
      _replaceTaskGridCell(taskGridCell, changesetCtr + 1, taskGrid.size());

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
  const TaskGridCell& taskGridCell, const int changesetNum, const int taskGridSize)
{
  if (_taskCellSkipIds.contains(taskGridCell.id))
  {
    LOG_STATUS("***********Skipping task grid cell: " << taskGridCell.id << "*********");
    _subTaskTimer.restart();
    return;
  }

  const QString boundsStr = GeometryUtils::toString(taskGridCell.bounds);
  QFile changesetFile(
    _changesetsOutputDir + "/changeset-cell-" +
    StringUtils::padFrontOfNumberStringWithZeroes(taskGridCell.id, 3) + ".osc.sql");

  QString msg =
    "***********Deriving changeset " +
    QString::number(changesetNum) + " / " + StringUtils::formatLargeNumber(taskGridSize) +
    " for task grid cell: " + QString::number(taskGridCell.id);
  if (taskGridCell.replacementNodeCount != -1)
  {
    msg +=
      ", replacement nodes: " + StringUtils::formatLargeNumber(taskGridCell.replacementNodeCount);
  }
  msg+= "*********";
  LOG_STATUS(msg);

  _changesetCreator->setChangesetId(QString::number(taskGridCell.id));
  _changesetCreator->create(
    _dataToReplaceUrl, _replacementUrl, taskGridCell.bounds, changesetFile.fileName());

  _numChangesetsDerived++;
  _totalChangesetDeriveTime += _subTaskTimer.elapsed() / 1000.0;
  _averageChangesetDeriveTime = _totalChangesetDeriveTime / (double)_numChangesetsDerived;
  _subTaskTimer.restart();
  LOG_STATUS("Average changeset derive time: " << _averageChangesetDeriveTime << " seconds.");

  LOG_STATUS(
    "Applying changeset: " << changesetNum << " / " <<
    StringUtils::formatLargeNumber(taskGridSize) << " for task grid cell: " << taskGridCell.id <<
    ", over bounds: " << GeometryUtils::envelopeFromConfigString(boundsStr) << ", from file: ..." <<
    changesetFile.fileName().right(25) << "...");

  _changesetApplier->write(changesetFile);
  _printChangesetStats();

  LOG_STATUS(
    "\tChangeset applied in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();


  // VERY SLOW
  if (ConfigOptions().getDebugMapsWrite() && changesetNum < taskGridSize)
  {
    _getUpdatedData(
      _changesetsOutputDir + "/" + QString::number(taskGridCell.id) + "-replaced-data.osm");
  }

  LOG_STATUS(
    "Total replacement time elapsed: " << StringUtils::millisecondsToDhms(_opTimer.elapsed()));
}

void ChangesetTaskGridReplacer::_printChangesetStats()
{
  const QMap<QString, long> changesetStats = _changesetApplier->getChangesetStats();

  _changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_CREATE_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_CREATE_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_MODIFY_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_MODIFY_KEY];
  _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY] +=
    changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY];

  LOG_STATUS(
    "\nNode Totals:\n" <<
    "   Created: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]) << "\n" <<
    "   Modified: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]) << "\n" <<
    "   Deleted: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]) << "\n" <<
    "Way Totals:\n" <<
    "   Created: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]) << "\n" <<
    "   Modified: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]) << "\n" <<
    "   Deleted: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]) << "\n" <<
    "Relation Totals:\n" <<
    "   Created: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]) << "\n" <<
    "   Modified: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]) << "\n" <<
    "   Deleted: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]) << "\n" <<
    "Overall Totals:\n" <<
    "   Created: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_CREATE_KEY]) << "\n" <<
    "   Modified: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_MODIFY_KEY]) << "\n" <<
    "   Deleted: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY]));
}

void ChangesetTaskGridReplacer::_getUpdatedData(const QString& outputFile)
{
  LOG_STATUS(
    "Reading the modified data out of: " << _dataToReplaceUrl << " to: ..." <<
    outputFile.right(25) << "...");

  // clear this out so we get all the data back
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), "");
  // change these, so we can open the files in josm; not sure if these are helping...
  conf().set(ConfigOptions::getConvertBoundingBoxRemoveMissingElementsKey(), true);
  conf().set(ConfigOptions::getMapReaderAddChildRefsWhenMissingKey(), false);

  OsmMapPtr map(new OsmMap());
  OsmMapReaderFactory::read(map, _dataToReplaceUrl, true, Status::Unknown1);

  // had to do this cleaning to get the relations to behave
  RemoveMissingElementsVisitor missingElementRemover;
  map->visitRw(missingElementRemover);
  RemoveInvalidRelationVisitor invalidRelationRemover;
  map->visitRw(invalidRelationRemover);
  RemoveEmptyRelationsOp().apply(map);

  OsmMapWriterFactory::write(map, outputFile);

  LOG_STATUS(
    "Modified data original size: " << StringUtils::formatLargeNumber(_originalDataSize) <<
    ", current size: " << StringUtils::formatLargeNumber(map->size()) << ", read in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();
}

}
