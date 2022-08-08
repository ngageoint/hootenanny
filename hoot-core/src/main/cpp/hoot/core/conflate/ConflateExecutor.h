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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef CONFLATE_EXECUTOR_H
#define CONFLATE_EXECUTOR_H

// Hoot
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/info/IoSingleStat.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/util/Progress.h>

// Tgs
#include <tgs/System/Timer.h>

namespace hoot
{

/**
 * @brief The ConflateExecutor class is a facade around conflate classes to abstract some details
 * internal to conflate.
 *
 * The primary conflate classes are UnifyingConflator and DiffConflator. UnifyingConflator handles
 * all non-differential workflows and DiffConflator handles the differential conflate workflow.
 * @todo refactor the stats display logic into its own class?
 */
class ConflateExecutor
{

public:

  static QString className() { return "ConflateExecutor"; }

  static const QString JOB_SOURCE;

  ConflateExecutor();

  /**
   * @brief conflate conflates to inputs and writes the conflated data to an output.
   * @param input1 URL of first map to conflate
   * @param input2 URL of second map to conflate
   * @param outputPath URL to write conflated output
   */
  void conflate(const QString& input1, const QString& input2, const QString& output);

  void setIsDiffConflate(bool isDiffConflate) { _isDiffConflate = isDiffConflate; }
  void setDiffConflateEnableTags(bool enable) { if (enable) _diffConflator.enableTags(); }
  void setDiffConflateSeparateOutput(bool separate) { _diffConflateSeparateOutput = separate; }
  void setDiffRemoveLinearPartialMatchesAsWhole(bool remove)
  { _diffConflator.setRemoveLinearPartialMatchesAsWhole(remove); }
  void setDiffRemoveRiverPartialMatchesAsWhole(bool remove)
  { _diffConflator.setRemoveRiverPartialMatchesAsWhole(remove); }
  void setOsmApiDbUrl(QString url) { _osmApiDbUrl = url; }
  void setDisplayStats(bool display) { _displayStats = display; }
  void setOutputStatsFile(QString file) { _outputStatsFile = file; }
  void setDisplayChangesetStats(bool display) { _displayChangesetStats = display; }
  void setOutputChangesetStatsFile(QString file) { _outputChangesetStatsFile = file; }

private:

  bool _isDiffConflate;
  bool _diffConflateSeparateOutput;
  DiffConflator _diffConflator;
  ChangesetProviderPtr _pTagChanges;

  bool _isAttributeConflate;
  bool _isAverageConflate;

  QString _osmApiDbUrl;

  QList<QList<SingleStat>> _allStats;
  QList<SingleStat> _stats;
  bool _displayStats;
  QString _outputStatsFile;
  bool _displayChangesetStats;
  QString _outputChangesetStatsFile;

  bool _filterOps;

  std::shared_ptr<Progress> _progress;
  int _numTotalTasks;
  int _currentTask;
  int _maxFilePrintLength;

  Tgs::Timer _taskTimer;

  void _initConfig(const QString& output);
  /**
   * @brief _updateConfigOptionsForAttributeConflation makes some custom adjustments to config opts
   * that must be done for Attribute Conflation.

     There may be a way to eliminate some of these by adding more custom behavior to the UI.
   */
  void _updateConfigOptionsForAttributeConflation() const;
  /**
   * @brief _updateConfigOptionsForDifferentialConflation makes some custom adjustments to config
   * opts that must be done for Differential Conflation.
   */
  void _updateConfigOptionsForDifferentialConflation() const;
  void _updateConfigOptionsForBounds() const;
  void _updateTranslationDirection(const QString& output) const;
  void _disableRoundaboutRemoval() const;
  void _checkForTagValueTruncationOverride();
  void _setRubberSheetElementCriteria() const;

  void _initTaskCount();
  float _getJobPercentComplete(const int currentTaskNum) const;
  float _getTaskWeight() const;

  void _load(const QString& input1, const QString& input2,
             const OsmMapPtr& map, const bool isChangesetOut);

  void _runConflate(OsmMapPtr& map);

  void _runConflateOps(OsmMapPtr& map, const bool runPre);

  void _writeOutput(const OsmMapPtr& map, const QString& output, const bool isChangesetOutput);
  void _writeStats(const OsmMapPtr& map, const CalculateStatsOp& input1Cso,
                   const CalculateStatsOp& input2Cso, const QString& outputFileName);
  void _writeChangesetStats();
};

}

#endif // CONFLATE_EXECUTOR_H
