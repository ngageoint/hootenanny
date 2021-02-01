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

namespace hoot
{

/**
 * Allows for conflating maps in a cumulative fashion.
 *
 * @todo Test the following:
 *
 * - forward and reverse (--reverse-inputs)
 *   - div road is more gnarly using forward; rest is similar
 * - unifying vs network (from existing options)
 *   - network oddly is not merging nearly as much; may be due to the small aoi
 * - pre-attribute conflate against osm option to get div road tags on first input
 *   (--add-tags <source-file>); also tag anything that overlaps/cross the osm div road?
 *   -
 * - match/review thresh adjustment (from existing options)
 * - drop reviews and/or drop secondary features involved in reviews in output? (post conflate op)
 *   -
 * - auto merge reviews?
 * - throw out unmatched secondary features that cross over / overlap previously conflated data?
 *   (post conflate op)
 *   -
 * - use something similar to SmallHighwayMerger to throw out any short two node ways in output?
 *   (post conflate op)
 *   -
 * - sort inputs by raster or graph score (--sort-inputs-by-score <type>)
 *   -
 * - score outputs with logic from compare command and throw out result if score doesn't get any
 *   better (--score-output)
 *   -
 * - option for diff conflate and merge workflow (--differential)
 *   -
 * - alg ensemble approach tied to score (--ensemble)
 *   -
 * - avg conflate?
 *
 * hoot conflate-cumulative --status
 *   -C ReferenceConflation.conf \
 *   -C UnifyingAlgorithm.conf \
 *   -D uuid.helper.repeatable=true \
 *   -D writer.include.debug.tags=true \
 *   -D log.class.filter="CumulativeConflator2;ConflateCumulativeCmd" \
 *   -D match.creators="hoot::HighwayMatchCreator" \
 *   -D merger.creators="hoot::HighwayMergerCreator" \
 *   -D bounds="8.4762,12.0504,8.4793,12.0526" \
 *   -D bounds.keep.entire.features.crossing.bounds=false \
 *   -D highway.match.threshold=0.161 \
 *   -D highway.review.threshold=0.25 \
 *   -D highway.miss.threshold=0.999 \
 *   /home/vagrant/hoot/tmp/kano_033133330302/input/ \
 *   /home/vagrant/hoot/tmp/kano_033133330302/output/kano-final.osm \
 *   --keep-intermediate-outputs \
 *   --reverse-inputs \
 *   --max-iterations -1
 */
class CumulativeConflator2
{
public:

  typedef enum ScoreType
  {
    None = 0,
    Raster,
    Graph
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
  void setAddTagsInput(QString addTagsInput) { _addTagsInput = addTagsInput; }
  void setRunEnsemble(bool runEnsemble) { _runEnsemble = runEnsemble; }
  void setMaxIterations(int max) { _maxIterations = max; }
  void setArgs(const QStringList& args) { _args = args; }
  void setKeepIntermediateOutputs(bool keep) { _keepIntermediateOutputs = keep; }
  void setInputSortScoreType(QString scoreTypeStr)
  { _inputSortScoreType = _scoreTypeFromString(scoreTypeStr); }

private:

  bool _reverseInputs;
  bool _scoreOutput;
  bool _isDifferential;
  QString _addTagsInput;
  bool _runEnsemble;
  int _maxIterations;
  bool _keepIntermediateOutputs;
  ScoreType _inputSortScoreType;

  QStringList _args;

  void _resetInitConfig();

  static ScoreType _scoreTypeFromString(QString& scoreTypeStr);
};

}

#endif // CUMULATIVE_CONFLATOR_2_H
