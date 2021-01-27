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
 * @copyright Copyright (C) 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CONFLATOR_H
#define CONFLATOR_H

// Hoot
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/util/Progress.h>

namespace hoot
{

/**
 * TODO
 */
class Conflator
{

public:

  static const QString JOB_SOURCE;

  Conflator();

  /**
   * TODO
   *
   * @param input1
   * @param input2
   * @param outputPath
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

  bool _isDiffConflate;
  bool _diffConflateSeparateOutput;
  DiffConflator _diffConflator;

  QString _osmApiDbUrl;

  bool _displayStats;
  QString _outputStatsFile;
  bool _displayChangesetStats;
  QString _outputChangesetStatsFile;

  bool _filterOps;

  std::shared_ptr<Progress> _progress;
  int _numTotalTasks;
  int _currentTask;
  int _maxFilePrintLength;

  void _initConfig();

  void _load(const QString& input1, const QString& input2, OsmMapPtr& map,
             const bool isChangesetOut);

  void _updateConfigOptionsForAttributeConflation();
  void _updateConfigOptionsForDifferentialConflation();
  void _updateConfigOptionsForBounds();
  void _disableRoundaboutRemoval();
  void _checkForTagValueTruncationOverride();

  float _getJobPercentComplete(const int currentTaskNum) const;
  float _getTaskWeight() const;
};

}

#endif // CONFLATOR_H
