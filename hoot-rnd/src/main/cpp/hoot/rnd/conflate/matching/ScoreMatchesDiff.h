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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SCORE_MATCHES_DIFF_H
#define SCORE_MATCHES_DIFF_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/matching/MatchType.h>

// Qt
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QSet>

namespace hoot
{

/**
 * Compares two output files from score-matches and writes a file with the differences in the match
 * scoring. Useful when trying to determine why conflation logic changes results in increased
 * incorrect matches.
 */
class ScoreMatchesDiff
{
public:

  ScoreMatchesDiff();
  ~ScoreMatchesDiff();

  /**
   * Calculates the differential between to separate score-matches outputs
   *
   * @param input1 the first file to examine
   * @param input2 the second file to examine
   */
  void calculateDiff(const QString& input1, const QString& input2);

  /**
   * Prints previously calculated match scoring differential
   *
   * @param output the file to write the differential information to
   * @return true if the printing was successful; false otherwise
   */
  bool printDiff(const QString& output);

  /**
   * Clears calculated differential data
   */
  void clear();

private:

  friend class ScoreMatchesDiffTest;

  // IDs of elements involved in a wrong match from the second input that aren't involved in a wrong
  // match in the first input.
  QSet<ElementId> _newlyWrong;
  // IDs of elements involved in a correct match from the second input that aren't involved in a
  // correct match in the first input.
  QSet<ElementId> _newlyCorrect;
  // difference in match status between any element that wasn't involved in a wrong match in the
  // first input but is involved in a wrong match in the second
  QMap<QString, QSet<ElementId>> _newlyWrongMatchSwitches;
  // difference in match status between any element that was involved in a wrong match in the
  // first input but isn't involved in a wrong match in the second
  QMap<QString, QSet<ElementId>> _newlyCorrectMatchSwitches;

  // elements added to the second file when compared to the first
  QSet<ElementId> _elementIdsAdded;
  // elements removed from the first file when compared to the second
  QSet<ElementId> _elementIdsRemoved;

  // number of manual matches made
  int _numManualMatches;

  // difference in number of reviews in the two inputs
  int _reviewDifferential;

  QString _input1;
  QString _input2;
  QString _output;
  std::shared_ptr<QFile> _outputFile;

  std::shared_ptr<QFile> _getOutputFile(const QString& outputPath);

  /*
   * Returns all element IDs in the map
   */
  QSet<ElementId> _getAllIds(const ConstOsmMapPtr& map);

  /*
   * Returns all elements with the given match status (hoot:expected or hoot:actual), grouped by ID
   */
  QMap<ElementId, QString> _getMatchStatuses(const ConstOsmMapPtr& map, const QString& tagKey);

  /*
   * Returns the IDS of all elements involved in a wrong match
   */
  QSet<ElementId> _getWrong(const ConstOsmMapPtr& map);

  /*
   * For a given set of elements, groups them by how their match statuses changed between the two
   * inputs
   */
  QMap<QString, QSet<ElementId>> _getMatchScoringDiff(
    const QSet<ElementId>& elementIds, const QMap<ElementId, QString>& actualTagMappings1,
    const QMap<ElementId, QString>& actualTagMappings2);

  /*
   * Records element differences between the two inputs
   */
  void _setAddedAndRemovedElements(
    const QSet<ElementId>& all1Ids, const QSet<ElementId>& all2Ids,
    QSet<ElementId>& elementIdsAdded, QSet<ElementId>& elementIdsRemoved);

  void _writeConflateStatusSummary(QTextStream& out);
  void _writeConflateStatusDetail(QTextStream& out);
};

}

#endif // SCORE_MATCHES_DIFF_H
