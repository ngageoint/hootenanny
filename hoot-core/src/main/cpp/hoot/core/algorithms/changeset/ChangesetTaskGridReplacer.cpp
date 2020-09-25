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
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/io/OsmApiDbSqlChangesetApplier.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>
#include <hoot/core/visitors/RemoveInvalidRelationVisitor.h>
#include <hoot/core/ops/RemoveEmptyRelationsOp.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/criterion/DisconnectedWayCriterion.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/criterion/ElementIdCriterion.h>
#include <hoot/core/criterion/EmptyWayCriterion.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/ops/RemoveRoundabouts.h>
#include <hoot/core/ops/ReplaceRoundabouts.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/NonConflatableCriterion.h>
#include <hoot/core/ops/DuplicateElementMarker.h>

namespace hoot
{

ChangesetTaskGridReplacer::ChangesetTaskGridReplacer() :
_originalDataSize(0),
_reverseTaskGrid(false),
_currentTaskGridCellId(-1),
_killAfterNumChangesetDerivations(-1),
_numChangesetsDerived(0),
_totalChangesetDeriveTime(0.0),
_averageChangesetDeriveTime(0.0),
_tagQualityIssues(false),
_calcDiffWithReplacement(false),
_outputNonConflatable(false)
{
}

void ChangesetTaskGridReplacer::replace(
  const QString& toReplace, const QString& replacement, const TaskGrid& taskGrid)
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
  _initChangesetStats();

  _initConfig();
  _taskGridBounds = taskGrid.getBounds();
  _currentTaskGridCellId = -1;

  try
  {
    bool exceptionOccurred = false;
    try
    {
      _replaceEntireTaskGrid(taskGrid);
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

    if (!exceptionOccurred)
    {
      LOG_STATUS(
        "Task grid cell replacement operation successfully completed in: " <<
        StringUtils::millisecondsToDhms(_opTimer.elapsed()));
    }

    if (!_finalOutput.isEmpty())
    {
      _getUpdatedData(_finalOutput);
      if (_calcDiffWithReplacement)
      {
        // Calculate a diff between the data we just replaced and the original replacement data to
        // aid in finding any errors during the replacement process.
        const QString diffOutput = _finalOutput.replace(".osm", "-diff.osm");
        _calculateDiffWithOriginalReplacementData(diffOutput);
      }
    }
  }
  catch (const HootException& e)
  {
    LOG_ERROR(
      "Entire task grid cell replacement operation partially completed with error while " <<
      " replacing task grid cell number: " << _currentTaskGridCellId << ", " <<
      _numChangesetsDerived << " / " << taskGrid.size() <<
      " cells replaced, time elapsed: " << StringUtils::millisecondsToDhms(_opTimer.elapsed()) <<
      "; Error: " << e.getWhat());
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

void ChangesetTaskGridReplacer::_replaceEntireTaskGrid(const TaskGrid& taskGrid)
{
  // recommended C&R production config
  _changesetCreator.reset(
    Factory::getInstance().constructObject<ChangesetReplacementCreator>(
      ConfigOptions().getChangesetReplacementImplementation()));
  _changesetCreator->setFullReplacement(true);
  _changesetCreator->setBoundsInterpretation(
    ChangesetReplacementCreator::BoundsInterpretation::Lenient);
  _changesetCreator->setChangesetOptions(true, "", _dataToReplaceUrl);

  _changesetApplier.reset(new OsmApiDbSqlChangesetApplier(_dataToReplaceUrl));

  // for each task grid cell
    // derive the difference between the replacement data and the being replaced
    // apply the difference back to the data being replaced

  _numChangesetsDerived = 0;
  _totalChangesetDeriveTime = 0.0;
  int changesetCtr = 0;
  const QList<TaskGrid::TaskGridCell> taskGridCells = taskGrid.getCells();
  // probably a cleaner way to do this reversal handling...
  if (!_reverseTaskGrid)
  {
    for (QList<TaskGrid::TaskGridCell>::const_iterator taskGridCellItr = taskGridCells.begin();
         taskGridCellItr != taskGridCells.end(); ++taskGridCellItr)
    {
      const TaskGrid::TaskGridCell taskGridCell = *taskGridCellItr;
      _replaceTaskGridCell(taskGridCell, changesetCtr + 1, taskGridCells.size());
      changesetCtr++;
    }
  }
  else
  {
    for (QList<TaskGrid::TaskGridCell>::const_reverse_iterator taskGridCellItr = taskGridCells.crbegin();
         taskGridCellItr != taskGridCells.crend(); ++taskGridCellItr)
    {
      const TaskGrid::TaskGridCell taskGridCell = *taskGridCellItr;
      _replaceTaskGridCell(taskGridCell, changesetCtr + 1, taskGridCells.size());
      changesetCtr++;
    }
  }
}

void ChangesetTaskGridReplacer::_replaceTaskGridCell(
  const TaskGrid::TaskGridCell& taskGridCell, const int changesetNum, const int taskGridSize)
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
  const int numChanges = _changesetCreator->getNumChanges();

  _numChangesetsDerived++;
  _totalChangesetDeriveTime += _subTaskTimer.elapsed() / 1000.0;
  _averageChangesetDeriveTime = _totalChangesetDeriveTime / (double)_numChangesetsDerived;
  _subTaskTimer.restart();
  LOG_STATUS("Average changeset derive time: " << _averageChangesetDeriveTime << " seconds.");

  LOG_STATUS(
    "Applying changeset: " << changesetNum << " / " <<
    StringUtils::formatLargeNumber(taskGridSize) << " with " <<
    StringUtils::formatLargeNumber(numChanges) << " changes for task grid cell: " <<
    taskGridCell.id << ", over bounds: " << GeometryUtils::envelopeFromConfigString(boundsStr) <<
    ", from file: ..." << changesetFile.fileName().right(25) << "...");

  _changesetApplier->write(changesetFile);
  _printChangesetStats();

  LOG_STATUS(
    "\tChangeset with " << StringUtils::formatLargeNumber(numChanges) << " changes applied in: " <<
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
    _getUpdatedData(
      _changesetsOutputDir + "/" + QString::number(taskGridCell.id) + "-replaced-data.osm");
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
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY]) << "\n" <<
    "   Total Changes: " <<
      StringUtils::formatLargeNumber(
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_CREATE_KEY] +
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_MODIFY_KEY] +
        _changesetStats[OsmApiDbSqlChangesetApplier::TOTAL_DELETE_KEY]));
}

void ChangesetTaskGridReplacer::_getUpdatedData(const QString& outputFile)
{
  // clear this out so we get all the data back
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), "");

  LOG_STATUS("Reading the modified data out of: ..." << _dataToReplaceUrl.right(25) << "...");
  OsmMapPtr map(new OsmMap());
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

  if (_outputNonConflatable)
  {
    // Output any features that hoot doesn't know how to conflate into their own file, for
    // debugging purposes.
    QString nonConflatableOutput = outputFile;
    nonConflatableOutput.replace(".osm", "-non-conflatable.osm");
    _writeNonConflatable(map, nonConflatableOutput);
  }

  if (_tagQualityIssues)
  {
    // tag element with potential data quality issues caused by the replacement operations; If this
    // isn't done after the previous cleaning step, you'll get some element NPE's.
    _writeQualityIssueTags(map);
  }

  // write the full map out
  LOG_STATUS("Writing the modified data to: ..." << outputFile.right(25) << "...");
  OsmMapWriterFactory::write(map, outputFile);

  LOG_STATUS(
    "Modified data original size: " << StringUtils::formatLargeNumber(_originalDataSize) <<
    ", current size: " << StringUtils::formatLargeNumber(map->size()) << ", read out in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();
}

void ChangesetTaskGridReplacer::_writeQualityIssueTags(OsmMapPtr& map)
{
  LOG_STATUS("Tagging features with quality issues...");

  std::shared_ptr<SetTagValueVisitor> tagVis;
  std::shared_ptr<FilteredVisitor> filteredVis;
  std::shared_ptr<ElementCriterion> crit;

  // We're only guaranteeing output data quality for the data in the task grid cells actually
  // replaced. Any data outside of the replacement grid may end up with quality issues that can't be
  // fixed until its replaced. So, restrict each of these quality checks to be in the replacement
  // AOI.

  tagVis.reset(new SetTagValueVisitor(MetadataTags::HootSuperfluous(), "yes"));
  crit.reset(
    new ElementIdCriterion(
      ElementType::Node,
      SuperfluousNodeRemover::collectSuperfluousNodeIds(map, false, _taskGridBounds)));
  filteredVis.reset(new FilteredVisitor(crit, tagVis));
  map->visitRo(*filteredVis);
  LOG_STATUS(
    "Tagged " << StringUtils::formatLargeNumber(tagVis->getNumFeaturesAffected()) <<
    " orphaned nodes in output.");

  // SuperfluousNodeRemover took in a bounds above, but the remaining quality checks do not so
  // combine their criteria with an InBoundsCriterion to make sure we only count elements within the
  // replacement bounds.
  std::shared_ptr<InBoundsCriterion> inBoundsCrit(new InBoundsCriterion(true));
  inBoundsCrit->setBounds(_taskGridBounds);
  inBoundsCrit->setOsmMap(map.get());

  tagVis.reset(new SetTagValueVisitor(MetadataTags::HootDisconnected(), "yes"));
  crit.reset(
    new ChainCriterion(ElementCriterionPtr(new DisconnectedWayCriterion(map)), inBoundsCrit));
  filteredVis.reset(new FilteredVisitor(crit, tagVis));
  map->visitRo(*filteredVis);
  LOG_STATUS(
    "Tagged " << StringUtils::formatLargeNumber(tagVis->getNumFeaturesAffected()) <<
    " disconnected ways in output.");

  tagVis.reset(new SetTagValueVisitor(MetadataTags::HootEmptyWay(), "yes"));
  crit.reset(new ChainCriterion(ElementCriterionPtr(new EmptyWayCriterion()), inBoundsCrit));
  filteredVis.reset(new FilteredVisitor(crit, tagVis));
  map->visitRo(*filteredVis);
  LOG_STATUS(
    "Tagged " << StringUtils::formatLargeNumber(tagVis->getNumFeaturesAffected()) <<
    " empty ways in output.");

  const int numDupes = DuplicateElementMarker::markDuplicates(map, 8);
  LOG_STATUS(
    "Tagged " << StringUtils::formatLargeNumber(numDupes) <<
    " duplicate feature pairs in output.");
}

void ChangesetTaskGridReplacer::_writeNonConflatable(const ConstOsmMapPtr& map,
                                                     const QString& outputFile)
{
  LOG_STATUS("Writing non-conflatable data to: ..." << outputFile.right(25) << " ...");
  OsmMapPtr nonConflatableMap(new OsmMap(map));
  std::shared_ptr<RemoveElementsVisitor> elementRemover(new RemoveElementsVisitor(true));
  elementRemover->setRecursive(true);
  std::shared_ptr<ElementCriterion> nonConflatableCrit(
    new NonConflatableCriterion(nonConflatableMap));
  elementRemover->addCriterion(nonConflatableCrit);;
  nonConflatableMap->visitRw(*elementRemover);
  if (nonConflatableMap->size() > 0)
  {
    OsmMapWriterFactory::write(nonConflatableMap, outputFile);
    LOG_STATUS(
      "Non-conflatable data of size: " <<
      StringUtils::formatLargeNumber(nonConflatableMap->size()) << " written in: " <<
      StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  }
  else
  {
    LOG_STATUS("No non-conflatable elements present.");
  }
  _subTaskTimer.restart();
}

void ChangesetTaskGridReplacer::_calculateDiffWithOriginalReplacementData(const QString& outputFile)
{
  // We only want to calculate the diff out to the task grid bounds, b/c that's the data that was
  // actually replaced.
  conf().set(
    ConfigOptions::getConvertBoundingBoxKey(),
    GeometryUtils::envelopeToConfigString(_taskGridBounds));
  // use lenient bounds
  conf().set(ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(), true);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(), false);
  //conf().set(ConfigOptions::getDifferentialTreatReviewsAsMatchesKey(), false);

  // By default rubbersheeting has no filters. When conflating, we need to add the ones from the
  // config.
  conf().set(
    ConfigOptions::getRubberSheetElementCriteriaKey(),
    ConfigOptions().getConflateRubberSheetElementCriteria());
  // don't remove/replace roundabouts during diff conflate
  QStringList preConflateOps = ConfigOptions().getConflatePreOps();
  const QString removeRoundaboutsClassName = QString::fromStdString(RemoveRoundabouts::className());
  if (preConflateOps.contains(removeRoundaboutsClassName))
  {
    preConflateOps.removeAll(removeRoundaboutsClassName);
    conf().set(ConfigOptions::getConflatePreOpsKey(), preConflateOps);
  }
  QStringList postConflateOps = ConfigOptions().getConflatePostOps();
  const QString replaceRoundaboutsClassName =
    QString::fromStdString(ReplaceRoundabouts::className());
  if (postConflateOps.contains(replaceRoundaboutsClassName))
  {
    postConflateOps.removeAll(replaceRoundaboutsClassName);
    conf().set(ConfigOptions::getConflatePostOpsKey(), postConflateOps);
  }

  LOG_STATUS(
    "Loading replacement data for diff calc from: ..." << _replacementUrl.right(25) << "...");
  OsmMapPtr diffMap(new OsmMap());
  IoUtils::loadMap(diffMap, _replacementUrl, true, Status::Unknown1);
  const int replacementMapSize = diffMap->size();
  LOG_STATUS(
    "Replacement data loaded in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  const QString replacedDataUrl = _dataToReplaceUrl;
  LOG_STATUS("Loading replaced data for diff calc from: ..." << replacedDataUrl.right(25) << "...");
  IoUtils::loadMap(diffMap, replacedDataUrl, false, Status::Unknown2);
  LOG_STATUS(
    "Replaced data loaded in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  // had to do this cleaning to get the relations to behave
  RemoveMissingElementsVisitor missingElementRemover;
  diffMap->visitRw(missingElementRemover);
  LOG_STATUS(missingElementRemover.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(diffMap, "task-grid-replacer-diff-input");

  LOG_STATUS(
    "Calculating the diff between replaced data of size: " <<
    StringUtils::formatLargeNumber(diffMap->size() - replacementMapSize) <<
    " and replacement data of size: " << StringUtils::formatLargeNumber(replacementMapSize)  <<
    "...");
  NamedOp(ConfigOptions().getConflatePreOps()).apply(diffMap);
  DiffConflator diffGen;
  diffGen.apply(diffMap);
  NamedOp(ConfigOptions().getConflatePostOps()).apply(diffMap);
  LOG_STATUS(
    "Calculated a diff with: " << StringUtils::formatLargeNumber(diffMap->size()) <<
    " features in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()) << " (skipped " <<
    StringUtils::formatLargeNumber(diffGen.getNumUnconflatableElementsDiscarded()) <<
    " unconflatable)");
  _subTaskTimer.restart();

  LOG_STATUS(
    "Writing the diff output of size: " << StringUtils::formatLargeNumber(diffMap->size()) <<
    " to: ..." << outputFile.right(25) << "...");
  IoUtils::saveMap(diffMap, outputFile);
  LOG_STATUS(
    "Wrote the diff output of size: " << StringUtils::formatLargeNumber(diffMap->size()) <<
    " in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();
}

}
