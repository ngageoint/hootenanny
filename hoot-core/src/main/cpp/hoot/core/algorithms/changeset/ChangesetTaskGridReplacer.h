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

#ifndef CHANGESET_TASK_GRID_REPLACER_H
#define CHANGESET_TASK_GRID_REPLACER_H

// Hoot
#include <hoot/core/algorithms/changeset/TaskGrid.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/DataQualityMetricTagger.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class ChangesetReplacement;
class OsmApiDbSqlChangesetApplier;

/**
 * @brief The ChangesetTaskGridReplacer class can replace data in an OSM API database across
 * multiple AOI's via changeset generation and application.
 *
 * Its primarily meant as a testing harness to support ServiceChangesetReplacementGridTest. However,
 * in theory, with some tweaks it could be used in a production environment if desired. Any of the
 * TaskGridGenerator implementations can be used to create a task grid to pass to this class.
 */
class ChangesetTaskGridReplacer
{

public:

  ChangesetTaskGridReplacer();
  virtual ~ChangesetTaskGridReplacer() = default;

  /**
   * @brief replace replaces data within a task grid cell.
   * @param toReplace URL to the data to replace; must be an OSM API database
   * @param replacement URL to the replacement data; must be a Hoot API database
   * @param taskGrid the task grid that partitions the individual replacement operations
   * @return a map with replaced data
   */
  OsmMapPtr replace(const QString& toReplace, const QString& replacement, const TaskGrid& taskGrid);

  DataQualityMetricTagger getOutputMetrics() const { return _metricTagger; }
  QMap<QString, long> getChangesetStats() const { return _changesetStats; }

  void setOriginalDataSize(int size) { _originalDataSize = size; }
  void setJobName(const QString& name) { _jobName = name; }
  void setReverseTaskGrid(bool reverse) { _reverseTaskGrid = reverse; }
  void setTaskCellIncludeIds(const QList<int>& ids) { _taskCellIncludeIds = ids; }
  void setTaskCellSkipIds(const QList<int>& ids) { _taskCellSkipIds = ids; }
  void setChangesetsOutputDir(const QString& dir)
  { _changesetsOutputDir = dir; }
  void setKillAfterNumChangesetDerivations(int numDerivations)
  { _killAfterNumChangesetDerivations = numDerivations; }
  void setWriteFinalOutput(const QString& output) { _finalOutput = output; }
  void setTagQualityIssues(bool tag) { _tagQualityIssues = tag; }

private:

  // overall operation timer
  QElapsedTimer _opTimer;
  // per task timer
  QElapsedTimer _subTaskTimer;

  // data being replaced; must be osmapidb://
  QString _dataToReplaceUrl;
  int _originalDataSize;
  // replacement data; must be hootapidb://
  QString _replacementUrl;

  // useful for identifying test output files when multiple tests are run at once
  QString _jobName;

  // allows for including the processing of any grid cell with an "id" tag value in this ID list
  QList<int> _taskCellIncludeIds;
  // allows for skipping the processing of any grid cell with an "id" tag value in this ID list;
  // overrides _taskCellIncludeIds
  QList<int> _taskCellSkipIds;
  // swap the order in which the task grid cells; useful for testing adjacency replacement issues
  bool _reverseTaskGrid;
  // the area of data being replaced
  geos::geom::Envelope _taskGridBounds;

  // derives the replacement changesets
  std::shared_ptr<ChangesetReplacement> _changesetCreator;
  // all changeset files generated are stored here
  QString _changesetsOutputDir;
  // the ID of the task grid cell currently being processed
  int _currentTaskGridCellId;
  // allows for ending a large replacement early; useful for debugging; set to -1 to disable
  int _killAfterNumChangesetDerivations;
  int _numChangesetsDerived;
  // seconds
  double _totalChangesetDeriveTime;
  double _averageChangesetDeriveTime;

  // applies the replacement changesets
  std::shared_ptr<OsmApiDbSqlChangesetApplier> _changesetApplier;
  QMap<QString, long> _changesetStats;

  // optional location to write the final completely replaced ref output
  QString _finalOutput;

  // adds tags to features that are suspect as result of the replacement op
  bool _tagQualityIssues;
  DataQualityMetricTagger _metricTagger;

  void _initConfig() const;

  void _replaceEntireTaskGrid(const TaskGrid& taskGrid);
  void _replaceTaskGridCell(
    const TaskGrid::TaskGridCell& taskGridCell, const int changesetNum, const int taskGridSize);
  void _initChangesetStats();
  void _printChangesetStats();

  // writes out the updated data
  OsmMapPtr _writeUpdatedData(const QString& outputFile);
};

}

#endif // CHANGESET_TASK_GRID_REPLACER_H
