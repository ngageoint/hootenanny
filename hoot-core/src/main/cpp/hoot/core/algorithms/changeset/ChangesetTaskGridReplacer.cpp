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
_readNodeDensityInputFullThenCrop(false),
_maxNodeDensityNodesPerCell(1000),
_reverseTaskGrid(false),
_killAfterNumChangesetDerivations(-1),
_numChangesetsDerived(0),
_totalChangesetDeriveTime(0.0),
_averageChangesetDeriveTime(0.0),
_totalNodesCreated(0),
_totalNodesModified(0),
_totalNodesDeleted(0),
_totalWaysCreated(0),
_totalWaysModified(0),
_totalWaysDeleted(0),
_totalRelationsCreated(0),
_totalRelationsModified(0),
_totalRelationsDeleted(0),
_totalCreations(0),
_totalModifications(0),
_totalDeletions(0)
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

  _opTimer.start();
  _subTaskTimer.start();

  _dataToReplaceUrl = toReplace;
  _replacementUrl = replacement;
  _totalNodesCreated = 0;
  _totalNodesModified = 0;
  _totalNodesDeleted = 0;
  _totalWaysCreated = 0;
  _totalWaysModified = 0;
  _totalWaysDeleted = 0;
  _totalRelationsCreated = 0;
  _totalRelationsModified = 0;
  _totalRelationsDeleted = 0;

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

QMap<int, ChangesetTaskGridReplacer::TaskGridCell> ChangesetTaskGridReplacer::_getTaskGrid()
{
  LOG_STATUS("Creating task grid...");

  QMap<int, TaskGridCell> taskGrid;
  if (!_gridInputs.isEmpty())
  {
    LOG_INFO("Reading " << _gridInputs.size() << " task grid file(s)...");
    QMap<int, geos::geom::Envelope> taskGridTemp;
    for (int i = 0; i < _gridInputs.size(); i++)
    {
      const QString gridInput = _gridInputs.at(i);
      LOG_INFO("Reading task grid file: ..." << gridInput.right(25) << "...");
      taskGridTemp = GeometryUtils::readBoundsFileWithIds(gridInput);
      for (QMap<int, geos::geom::Envelope>::const_iterator taskGridTempItr = taskGridTemp.begin();
           taskGridTempItr != taskGridTemp.end(); ++taskGridTempItr)
      {
        TaskGridCell taskGridCell;
        // We don't know the node count when reading the custom grid file.
        taskGridCell.replacementNodeCount = -1;
        taskGridCell.bounds = taskGridTempItr.value();
        taskGrid[taskGridTempItr.key()] = taskGridCell;
      }
    }
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
  // changeset derive will overwrite these later, if needed
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), _nodeDensityGridBounds);
  conf().set(ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(), false);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(), true);

  LOG_STATUS(
    "Preparing input data for task grid cell calculation from: ..." <<
    _replacementUrl.right(25) << "...");
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

  if (_readNodeDensityInputFullThenCrop)
  {
    // Restore the default setting, or the changeset replacement maps will be loaded very slowly
    // if loading from URL.
    conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), false);
  }

  LOG_STATUS(
    StringUtils::formatLargeNumber(map->size()) << " task grid calc elements prepared " <<
    "in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  return map;
}

QMap<int, ChangesetTaskGridReplacer::TaskGridCell> ChangesetTaskGridReplacer::_calcNodeDensityTaskGrid(
  OsmMapPtr map)
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
  QMap<int, ChangesetTaskGridReplacer::TaskGridCell> taskGrid;
  int cellCtr = 1;
  for (size_t tx = 0; tx < rawTaskGrid.size(); tx++)
  {
    for (size_t ty = 0; ty < rawTaskGrid[tx].size(); ty++)
    {
      TaskGridCell taskGridCell;
      taskGridCell.replacementNodeCount = nodeCounts[tx][ty];
      taskGridCell.bounds = rawTaskGrid[tx][ty];
      taskGrid[cellCtr] = taskGridCell;
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

void ChangesetTaskGridReplacer::_replaceEntireTaskGrid(const QMap<int, TaskGridCell>& taskGrid)
{
  LOG_STATUS("Replacing data in task grid...");

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
    for (QMap<int, TaskGridCell>::const_iterator taskGridItr = taskGrid.begin();
         taskGridItr != taskGrid.end(); ++taskGridItr)
    {
      const int taskGridCellId = taskGridItr.key();
      const TaskGridCell taskGridCell = taskGridItr.value();
      _replaceTaskGridCell(
        taskGridCellId, changesetCtr + 1, taskGridCell.bounds, taskGrid.size(),
        taskGridCell.replacementNodeCount);

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
    QMapIterator<int, TaskGridCell> taskGridItr(taskGrid);
    taskGridItr.toBack();
    while (taskGridItr.hasPrevious())
    {
      taskGridItr.previous();

      const int taskGridCellId = taskGridItr.key();
      const TaskGridCell taskGridCell = taskGridItr.value();
      _replaceTaskGridCell(
        taskGridCellId, changesetCtr + 1, taskGridCell.bounds, taskGrid.size(),
        taskGridCell.replacementNodeCount);

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
  const int taskGridSize, const int numReplacementNodes)
{
  const QString boundsStr = GeometryUtils::toString(bounds);
  QFile changesetFile(
    _changesetsOutputDir + "/changeset-cell-" +
    StringUtils::padFrontOfNumberStringWithZeroes(taskGridCellId, 3) + ".osc.sql");

  QString msg =
    "***********Deriving changeset " +
    QString::number(changesetNum) + " / " + StringUtils::formatLargeNumber(taskGridSize) +
    " for task grid cell: " + QString::number(taskGridCellId);
  if (numReplacementNodes != -1)
  {
    msg += ", replacement nodes: " + StringUtils::formatLargeNumber(numReplacementNodes);
  }
  msg+= "*********";
  LOG_STATUS(msg);

  _changesetCreator->setChangesetId(QString::number(taskGridCellId));
  _changesetCreator->create(
    _dataToReplaceUrl, _replacementUrl, bounds, changesetFile.fileName());

  _numChangesetsDerived++;
  _totalChangesetDeriveTime += _subTaskTimer.elapsed() / 1000.0;
  _averageChangesetDeriveTime = _totalChangesetDeriveTime / (double)_numChangesetsDerived;
  _subTaskTimer.restart();
  LOG_STATUS("Average changeset derive time: " << _averageChangesetDeriveTime << " seconds.");

  LOG_STATUS(
    "Applying changeset: " << changesetNum << " / " <<
    StringUtils::formatLargeNumber(taskGridSize) << " for task grid cell: " << taskGridCellId <<
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
      _changesetsOutputDir + "/" + QString::number(taskGridCellId) + "-replaced-data.osm");
  }

  LOG_STATUS(
    "Total replacement time elapsed: " << StringUtils::millisecondsToDhms(_opTimer.elapsed()));
}

void ChangesetTaskGridReplacer::_printChangesetStats()
{
  const QMap<QString, long> changesetStats = _changesetApplier->getChangesetStats();
  _totalNodesCreated += changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY];
  _totalNodesModified += changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY];
  _totalNodesDeleted += changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY];
  _totalWaysCreated += changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY];
  _totalWaysModified += changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY];
  _totalWaysDeleted += changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY];
  _totalRelationsCreated += changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY];
  _totalRelationsModified += changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY];
  _totalRelationsDeleted += changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY];
  _totalCreations += changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_CREATE_KEY];
  _totalModifications += changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_MODIFY_KEY];
  _totalDeletions += changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY];
  LOG_STATUS(
    "Node Totals:\n" <<
    "\tCreated: " << StringUtils::formatLargeNumber(_totalNodesCreated) << "\n" <<
    "\tModified: " << StringUtils::formatLargeNumber(_totalNodesModified) << "\n" <<
    "\tDeleted: " << StringUtils::formatLargeNumber(_totalNodesDeleted) << "\n" <<
    "Way Totals:\n" <<
    "\tCreated: " << StringUtils::formatLargeNumber(_totalWaysCreated) << "\n" <<
    "\tModified: " << StringUtils::formatLargeNumber(_totalWaysModified) << "\n" <<
    "\tDeleted: " << StringUtils::formatLargeNumber(_totalWaysDeleted) << "\n" <<
    "Relation Totals:\n" <<
    "\tCreated: " << StringUtils::formatLargeNumber(_totalRelationsCreated) << "\n" <<
    "\tModified: " << StringUtils::formatLargeNumber(_totalRelationsModified) << "\n" <<
    "\tDeleted: " << StringUtils::formatLargeNumber(_totalRelationsDeleted) << "\n" <<
    "Overall Totals:\n" <<
    "\tCreated: " << StringUtils::formatLargeNumber(_totalCreations) << "\n" <<
    "\tModified: " << StringUtils::formatLargeNumber(_totalModifications) << "\n" <<
    "\tDeleted: " << StringUtils::formatLargeNumber(_totalDeletions));
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
    "\tModified data read in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();
  LOG_STATUS(
    "\tModified data original size: " << StringUtils::formatLargeNumber(_originalDataSize));
  LOG_STATUS("\tModified data current size: " << StringUtils::formatLargeNumber(map->size()));
}

}
