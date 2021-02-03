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

#ifndef CUMULATIVE_CONFLATOR_2_H
#define CUMULATIVE_CONFLATOR_2_H

// Qt
#include <QStringList>
#include <QDir>
#include <QElapsedTimer>

namespace hoot
{

/**
 * Allows for conflating maps in a cumulative fashion.
 *
 * hoot conflate-cumulative --status
 * -C ReferenceConflation.conf \
 * -C UnifyingAlgorithm.conf \
 * -D uuid.helper.repeatable=true -D writer.include.debug.tags=true \
 * -D log.class.filter="CumulativeConflator2;ConflateCumulativeCmd" \
 * -D match.creators="hoot::HighwayMatchCreator" \
 * -D merger.creators="hoot::HighwayMergerCreator" \
 * -D bounds="8.4762,12.0504,8.4793,12.0526" \
 * -D bounds.keep.entire.features.crossing.bounds=false \
 * -D direction.finder.angle.threshold=179.0 \
 * /home/vagrant/hoot/tmp/kano_033133330302/input/ \
 * /home/vagrant/hoot/tmp/kano_033133330302/output/out-kano-final.osm \
 * --keep-intermediate-outputs \
 * --reverse-inputs \
 * --max-iterations -1 \
 * --transfer-tags /home/vagrant/hoot/tmp/kano-osm-cropped-2.osm
 */
class CumulativeConflator2
{
public:

  typedef enum ScoreType
  {
    None = 0,
    Raster,
    Graph,
    Angle
  }
  ScoreType;

  CumulativeConflator2();

  /**
   * Conflates maps in a cumulative fashion.
   *
   * @param input input directory with files to conflate
   * @param output output path to write conflated data
   */
  void conflate(const QDir& input, const QString& output);

  void setReverseInputs(bool reverse) { _reverseInputs = reverse; }
  void setScoreOutput(bool score) { _scoreOutput = score; }
  void setDifferential(bool isDifferential) { _isDifferential = isDifferential; }
  void setTransferTagsInput(QString input) { _transferTagsInput = input; }
  void setLeaveTransferredTags(bool leave) { _leaveTransferredTags = leave; }
  void setMaxIterations(int max) { _maxIterations = max; }
  void setArgs(const QStringList& args) { _args = args; }
  void setKeepIntermediateOutputs(bool keep) { _keepIntermediateOutputs = keep; }
  void setInputSortScoreType(QString scoreTypeStr)
  { _inputSortScoreType = _scoreTypeFromString(scoreTypeStr); }

private:

  bool _reverseInputs;
  bool _scoreOutput;
  bool _isDifferential;
  QString _transferTagsInput;
  bool _leaveTransferredTags;
  int _maxIterations;
  bool _keepIntermediateOutputs;
  ScoreType _inputSortScoreType;

  QStringList _args;

  static const int FILE_NUMBER_PAD_SIZE = 3;

  QElapsedTimer _conflateTimer;

  static ScoreType _scoreTypeFromString(QString& scoreTypeStr);
  int _getNumIterations(const QStringList& inputs) const;

  void _resetInitConfig(const QStringList& args);
  void _initDropDividedRoadsConfig();

  void _transferTagsToInputs(const QDir& input, QStringList& inputs, const QString& output);
  void _removeTransferredTags(const QString& url);
};

}

#endif // CUMULATIVE_CONFLATOR_2_H
