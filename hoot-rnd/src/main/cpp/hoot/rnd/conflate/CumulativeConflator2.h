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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
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
 * Allows for conflating multiple maps in a cumulative fashion.
 *
 * Sample test command:
 *
 * hoot conflate-cumulative --status \
 *   -C ReferenceConflation.conf \
 *   -C UnifyingAlgorithm.conf \
 *   -D uuid.helper.repeatable=true \
 *   -D writer.include.debug.tags=true \
 *   -D log.class.filter="CumulativeConflator2;ConflateCumulativeCmd" \
 *   -D match.creators="hoot::HighwayMatchCreator" \
 *   -D merger.creators="hoot::HighwayMergerCreator" \
 *   -D bounds="8.4762,12.0504,8.4793,12.0526" \
 *   -D bounds.keep.entire.features.crossing.bounds=false \
 *   -D conflate.pre.ops+="hoot::SmallDisconnectedWayRemover;hoot::UnlikelyRoadRemover" \
 *   -D conflate.post.ops+="hoot::UnconnectedWaySnapper" \
 *   -D small.disconnected.way.remover.max.node.count=3 \
 *   -D small.disconnected.way.remover.max.length=20.0 \
 *   -D dual.highway.marker.mark.crossing.roads=true \
 *   -D snap.unconnected.ways.snap.tolerance=7.0 \
 *   -D snap.unconnected.ways.snap.way.statuses=Input1 \
 *   -D snap.unconnected.ways.snap.to.way.statuses=Input1 \
 *   -D unlikely.road.remover.max.heading.variance=60.0 \
 *   -D unlikely.road.remover.max.length=25.0 \
 *   /home/vagrant/hoot/tmp/kano_033133330302/input/ \
 *   /home/vagrant/hoot/tmp/kano_033133330302/output/out-kano-final.osm \
 *   --keep-intermediate-outputs \
 *   --reverse-inputs \
 *   --max-iterations -1 \
 *   --transfer-tags /home/vagrant/hoot/tmp/kano-osm-cropped-2.osm
 *
 * SmallDisconnectedWayRemover makes more sense to be at the front of the pre ops, but seems to
 * perform better so far at the end.
 *
 * UnconnectedWaySnapper makes more sense to be a pre op but has much better runtime performance as
 * a post op. Input1 only is used as the snap source/target, since the input data is known to be
 * incomplete in areas, and if Input2 is used instead, you will lose snapping against the initial
 * Input1 layer.
 */
class CumulativeConflator2
{
public:

  CumulativeConflator2();

  /**
   * Conflates multiple maps in a cumulative fashion.
   *
   * @param input input directory with files to conflate
   * @param output output path to write conflated data
   */
  void conflate(const QDir& input, const QString& output);

  void setReverseInputs(bool reverse) { _reverseInputs = reverse; }
  void setTransferTagsInput(QString input) { _transferTagsInput = input; }
  void setLeaveTransferredTags(bool leave) { _leaveTransferredTags = leave; }
  void setMaxIterations(int max) { _maxIterations = max; }
  void setArgs(const QStringList& args) { _args = args; }
  void setKeepIntermediateOutputs(bool keep) { _keepIntermediateOutputs = keep; }
  //void setInputSortScoreType(QString scoreTypeStr) { _inputSortScoreType = scoreTypeStr; }

private:

  bool _reverseInputs;
  QString _transferTagsInput;
  bool _leaveTransferredTags;
  int _maxIterations;
  bool _keepIntermediateOutputs;
  //QString _inputSortScoreType;

  QStringList _args;

  static const int FILE_NUMBER_PAD_SIZE = 3;

  QElapsedTimer _conflateTimer;

  int _getNumIterations(const QStringList& inputs) const;

  void _resetInitConfig(const QStringList& args);
  void _initDropDividedRoadsConfig();

  void _transferTagsToInputs(const QDir& input, QStringList& inputs, const QString& output);
  void _removeTransferredTags(const QString& url);
};

}

#endif // CUMULATIVE_CONFLATOR_2_H
