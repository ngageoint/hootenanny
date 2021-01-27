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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CONFLATOR_H
#define CONFLATOR_H

// Hoot
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/info/IoSingleStat.h>
#include <hoot/core/ops/CalculateStatsOp.h>

// Tgs
#include <tgs/System/Timer.h>

namespace hoot
{

/**
 * Facade around conflate classes to abstract some details internal to conflate.
 *
 * The primary conflate classes are UnifyingConflator and DiffConflator. UnifyingConflator handles
 * all non-differential workflows and DiffConflator handles the differential conflate workflow.
 *
 * @todo refactor the stats display logic into its own class?
 */
class Conflator
{

public:

  static const QString JOB_SOURCE;

  Conflator();

  /**
   * Conflates to inputs and writes the conflated data to an output
   *
   * @param input1 URL of first map to conflate
   * @param input2 URL of second map to conflate
   * @param outputPath URL to write conflated output
   */
  void conflate(const QString& input1, const QString& input2, QString& output);

  void setIsDiffConflate(bool isDiffConflate) { _isDiffConflate = isDiffConflate; }
  void setDiffConflateEnableTags(bool enable) { if (enable) _diffConflator.enableTags(); }
  void setDiffConflateSeparateOutput(bool separate) { _diffConflateSeparateOutput = separate; }

  void setOsmApiDbUrl(QString url) { _osmApiDbUrl = url; }

  void setDisplayStats(bool display) { _displayStats = display; }
  void setOutputStatsFile(QString file) { _outputStatsFile = file; }
  void setDisplayChangesetStats(bool display) { _displayChangesetStats = display; }
  void setOutputChangesetStatsFile(QString file) { _outputChangesetStatsFile = file; }

private:

  // See DiffConflator for more info on these vars.
  bool _isDiffConflate;
  bool _diffConflateSeparateOutput;
  DiffConflator _diffConflator;
  ChangesetProviderPtr _pTagChanges;

  // needed when generating changesets against an OSMAPIDB
  QString _osmApiDbUrl;

  QList<QList<SingleStat>> _allStats;
  QList<SingleStat> _stats;
  bool _displayStats;
  QString _outputStatsFile;
  bool _displayChangesetStats;
  QString _outputChangesetStatsFile;

  // allows for tailoring the pre/post conflate ops to the data being conflated
  bool _filterOps;

  std::shared_ptr<Progress> _progress;
  int _numTotalTasks;
  int _currentTask;
  int _maxFilePrintLength;

  Tgs::Timer _taskTimer;

  void _initConfig();
  void _updateConfigOptionsForAttributeConflation();
  void _updateConfigOptionsForDifferentialConflation();
  void _updateConfigOptionsForBounds();
  void _disableRoundaboutRemoval();
  void _checkForTagValueTruncationOverride();

  void _initTaskCount();
  float _getJobPercentComplete(const int currentTaskNum) const;
  float _getTaskWeight() const;

  void _load(const QString& input1, const QString& input2, OsmMapPtr& map,
             const bool isChangesetOut);
  void _getInputStats(
    OsmMapPtr& map, const QString& input1, const QString& input2,
    std::shared_ptr<CalculateStatsOp> input1Cso, std::shared_ptr<CalculateStatsOp> input2Cso);

  void _runConflate(OsmMapPtr& map);

  void _runConflateOps(OsmMapPtr& map, const bool runPre);

  void _writeOutput(OsmMapPtr& map, QString& output, const bool isChangesetOutput);
  void _writeStats(OsmMapPtr& map, std::shared_ptr<CalculateStatsOp> input1Cso,
                   std::shared_ptr<CalculateStatsOp> input2Cso, const QString& outputFileName);
};

}

#endif // CONFLATOR_H
