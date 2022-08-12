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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ChangesetTaskGridReplacer.h"

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetReplacement.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OsmApiDbSqlChangesetApplier.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/core/visitors/RemoveInvalidRelationVisitor.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>

namespace hoot
{

ChangesetTaskGridReplacer::ChangesetTaskGridReplacer()
  : _originalDataSize(0),
    _reverseTaskGrid(false),
    _currentTaskGridCellId(-1),
    _killAfterNumChangesetDerivations(-1),
    _numChangesetsDerived(0),
    _totalChangesetDeriveTime(0.0),
    _averageChangesetDeriveTime(0.0),
    _tagQualityIssues(false)
{
}

OsmMapPtr ChangesetTaskGridReplacer::replace(const QString& toReplace, const QString& replacement, const TaskGrid& taskGrid)
{
  if (!toReplace.startsWith("osmapidb://", Qt::CaseInsensitive))
    throw IllegalArgumentException("Data being replaced must be from an OSM API database.");
  if (!replacement.startsWith("hootapidb://", Qt::CaseInsensitive))
    throw IllegalArgumentException("Replacement data must be from a Hootenanny API database.");

  _opTimer.start();
  _subTaskTimer.start();

  _dataToReplaceUrl = toReplace;
  _replacementUrl = replacement;
  _initChangesetStats();

  if (_jobName.trimmed().isEmpty())
    _jobName = "job-" + UuidHelper::createUuid().toString();

  _initConfig();
  _taskGridBounds = taskGrid.getBounds();
  _currentTaskGridCellId = -1;

  try
  {
    _replaceEntireTaskGrid(taskGrid);
    LOG_STATUS(
      "Task grid cell replacement operation successfully completed in: " <<
      StringUtils::millisecondsToDhms(_opTimer.elapsed()));
    return _writeUpdatedData(_finalOutput);
  }
  catch (const HootException& e)
  {
    LOG_ERROR(
      "Entire task grid cell replacement operation partially completed with error while " <<
      "replacing task grid cell number: " << _currentTaskGridCellId << ", " <<
      _numChangesetsDerived << " of " << taskGrid.size() <<
      " cells replaced, time elapsed: " << StringUtils::millisecondsToDhms(_opTimer.elapsed()) <<
      "; Error: " << e.getWhat());
  }

  LOG_STATUS("Average changeset derive time: " << _averageChangesetDeriveTime << " seconds.");
  return OsmMapPtr();
}

void ChangesetTaskGridReplacer::_initConfig() const
{
  conf().set(ConfigOptions::getOsmapidbBulkInserterReserveRecordIdsBeforeWritingDataKey(), true);
  conf().set(ConfigOptions::getApidbBulkInserterValidateDataKey(), true);
  conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
  conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
  conf().set(ConfigOptions::getBoundsRemoveMissingElementsKey(), false);
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

void ChangesetTaskGridReplacer::_replaceEntireTaskGrid(const TaskGrid& taskGrid)
{
  _changesetCreator =
    Factory::getInstance().constructObject<ChangesetReplacement>(
      ConfigOptions().getChangesetReplacementImplementation());
  _changesetCreator->setChangesetOptions(true, "", _dataToReplaceUrl);
  LOG_VARD(_changesetCreator->toString());

  _changesetApplier = std::make_shared<OsmApiDbSqlChangesetApplier>(_dataToReplaceUrl);

  // for each task grid cell
    // derive the difference between the replacement data and the being replaced
    // apply the difference back to the data being replaced

  _numChangesetsDerived = 0;
  _totalChangesetDeriveTime = 0.0;
  int changesetCtr = 0;
  const QList<TaskGrid::TaskGridCell> taskGridCells = taskGrid.getCells();
  try
  {
    // probably a cleaner way to do this reversal handling...
    if (!_reverseTaskGrid)
    {
      for (const auto& taskGridCell : qAsConst(taskGridCells))
      {
        _replaceTaskGridCell(taskGridCell, changesetCtr + 1, taskGridCells.size());
        changesetCtr++;
      }
    }
    else
    {
      for (auto taskGridCellItr = taskGridCells.crbegin(); taskGridCellItr != taskGridCells.crend(); ++taskGridCellItr)
      {
        const TaskGrid::TaskGridCell taskGridCell = *taskGridCellItr;
        _replaceTaskGridCell(taskGridCell, changesetCtr + 1, taskGridCells.size());
        changesetCtr++;
      }
    }
  }
  catch (const HootException& e)
  {
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
      throw;
  }
}

void ChangesetTaskGridReplacer::_replaceTaskGridCell(const TaskGrid::TaskGridCell& taskGridCell, const int changesetNum, const int taskGridSize)
{
  _currentTaskGridCellId = taskGridCell.id;

  // Include IDs override skip IDs. if include IDs is populated at all and this ID isn't in the
  // list, skip it. Otherwise, if the skip IDs have it, also skip it.
  if ((!_taskCellIncludeIds.isEmpty() && !_taskCellIncludeIds.contains(taskGridCell.id)) ||
      _taskCellSkipIds.contains(taskGridCell.id))
  {
    LOG_STATUS("***********Skipping task grid cell: " << taskGridCell.id << "*********");
    _subTaskTimer.restart();
    return;
  }

  QFile changesetFile(
    _changesetsOutputDir + "/" + _jobName + "-changeset-cell-" +
    StringUtils::padFrontOfNumberStringWithZeroes(taskGridCell.id, 3) + ".osc.sql");

  QString msg =
    "***********Deriving changeset " +
    QString::number(changesetNum) + " / " + StringUtils::formatLargeNumber(taskGridSize) +
    " for task grid cell: " + QString::number(taskGridCell.id);
  if (taskGridCell.replacementNodeCount != -1)
    msg += ", replacement nodes: " + StringUtils::formatLargeNumber(taskGridCell.replacementNodeCount);
  msg+= "*********";
  LOG_STATUS(msg);

  _changesetCreator->setChangesetId(QString::number(taskGridCell.id));
  _changesetCreator->create(_dataToReplaceUrl, _replacementUrl, taskGridCell.bounds, changesetFile.fileName());
  const int numChanges = _changesetCreator->getNumChanges();

  _numChangesetsDerived++;
  _totalChangesetDeriveTime += _subTaskTimer.elapsed() / 1000.0;
  _averageChangesetDeriveTime = _totalChangesetDeriveTime / (double)_numChangesetsDerived;
  _subTaskTimer.restart();
  LOG_STATUS("Average changeset derive time: " << _averageChangesetDeriveTime << " seconds.");

  LOG_STATUS(
    "Applying changeset: " << changesetNum << " of " <<
    StringUtils::formatLargeNumber(taskGridSize) << " with " <<
    StringUtils::formatLargeNumber(numChanges) << " changes for task grid cell: " <<
    taskGridCell.id << ", over bounds: " << GeometryUtils::envelopeToString(taskGridCell.bounds) <<
    ", from file: ..." << FileUtils::toLogFormat(changesetFile.fileName(), 25) << "...");

  _changesetApplier->write(changesetFile);
  _printChangesetStats();

  LOG_STATUS(
    "Changeset with " << StringUtils::formatLargeNumber(numChanges) << " changes applied in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  if (_killAfterNumChangesetDerivations > 0 &&
      _numChangesetsDerived >= _killAfterNumChangesetDerivations)
  {
    throw HootException(
      "Killing replacement after " + QString::number(_numChangesetsDerived) +
      " changeset derivations...");
  }

  // VERY SLOW
  if (ConfigOptions().getDebugMapsWrite() && changesetNum < taskGridSize)
  {
    _writeUpdatedData(
      _changesetsOutputDir + "/" + _jobName + "-" + QString::number(taskGridCell.id) +
      "-replaced-data.osm");
  }

  if (changesetNum < taskGridSize)
  {
    LOG_STATUS(
      "Total replacement time elapsed: " << StringUtils::millisecondsToDhms(_opTimer.elapsed()));
  }
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
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::NODE_CREATE_KEY]) << "\n" <<
    "   Modified: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::NODE_MODIFY_KEY]) << "\n" <<
    "   Deleted: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::NODE_DELETE_KEY]) << "\n" <<
    "Way Totals:\n" <<
    "   Created: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::WAY_CREATE_KEY]) << "\n" <<
    "   Modified: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::WAY_MODIFY_KEY]) << "\n" <<
    "   Deleted: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::WAY_DELETE_KEY]) << "\n" <<
    "Relation Totals:\n" <<
    "   Created: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::RELATION_CREATE_KEY]) << "\n" <<
    "   Modified: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::RELATION_MODIFY_KEY]) << "\n" <<
    "   Deleted: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::RELATION_DELETE_KEY]) << "\n" <<
    "Overall Totals:\n" <<
    "   Created: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_CREATE_KEY]) << "\n" <<
    "   Modified: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_MODIFY_KEY]) << "\n" <<
    "   Deleted: " <<
      StringUtils::formatLargeNumber(_changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY]) << "\n" <<
    "   Total Changes: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_CREATE_KEY] +
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_MODIFY_KEY] +
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY]));
}

OsmMapPtr ChangesetTaskGridReplacer::_writeUpdatedData(const QString& outputFile)
{
  // clear this out so we get all the data back
  conf().set(ConfigOptions::getBoundsKey(), "");

  LOG_STATUS("Reading the modified data out of: ..." << FileUtils::toLogFormat(_dataToReplaceUrl, 25) << "...");
  OsmMapPtr map = std::make_shared<OsmMap>();
  OsmMapReaderFactory::read(map, _dataToReplaceUrl, true, Status::Unknown1);

  // had to do this cleaning to get the relations to behave and be viewable in JOSM
  RemoveMissingElementsVisitor missingElementRemover;
  map->visitRw(missingElementRemover);
  LOG_STATUS(missingElementRemover.getCompletedStatusMessage());
  RemoveInvalidRelationVisitor invalidRelationRemover;
  map->visitRw(invalidRelationRemover);
  LOG_STATUS(invalidRelationRemover.getCompletedStatusMessage());
  RemoveEmptyRelationsOp emptyRelationRemover;
  emptyRelationRemover.apply(map);
  LOG_STATUS(emptyRelationRemover.getCompletedStatusMessage());

  if (_tagQualityIssues)
  {
    // Tag element with potential data quality issues caused by the replacement operations. If this
    // isn't done after the previous cleaning step, you'll get some element NPE's.
    _metricTagger.setBounds(_taskGridBounds);
    _metricTagger.apply(map);
  }

  if (!outputFile.isEmpty())
  {
    // write the full map out
    LOG_STATUS("Writing the modified data to: ..." << FileUtils::toLogFormat(outputFile, 25) << "...");
    OsmMapWriterFactory::write(map, outputFile);
  }

  LOG_STATUS(
    "Modified data original size: " << StringUtils::formatLargeNumber(_originalDataSize) <<
    ", current size: " << StringUtils::formatLargeNumber(map->size()) << ", read out in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  return map;
}

}
