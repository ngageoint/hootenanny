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
#include <hoot/core/elements/ElementId.h>

// Qt
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QSet>

namespace hoot
{

/**
 * TODO
 */
class ScoreMatchesDiff
{
public:

  ScoreMatchesDiff();
  ~ScoreMatchesDiff();

  /**
   * TODO
   *
   * @param input1
   * @param input2
   */
  void calculateDiff(const QString& input1, const QString& input2);

  /**
   * TODO
   *
   * @param output
   */
  void printResults(const QString& output);

  /**
   * TODO
   */
  void clear();

private:

  QMap<QString, QSet<ElementId>> _matchSwitches;
  QSet<ElementId> _elementIdsAdded;
  QSet<ElementId> _elementIdsRemoved;

  QString _input1;
  QString _input2;
  std::shared_ptr<QFile> _outputFile;

  std::shared_ptr<QFile> _getOutputFile(const QString& outputPath);

  QSet<ElementId> _getAllIds(const QString& input);

  QMap<ElementId, QString> _getActualTagMappings(const QString& input);

  QSet<ElementId> _getElementIdsWithDifferingConflateStatus(
    const QMap<ElementId, QString>& actualTagMappings1,
    const QMap<ElementId, QString>& actualTagMappings2);

  QMap<QString, QSet<ElementId>> _getConflateStatusDiff(
    const QSet<ElementId>& elementIds, const QMap<ElementId, QString>& actualTagMappings1,
    const QMap<ElementId, QString>& actualTagMappings2);

  void _getAddedAndRemovedElements(
    const QSet<ElementId>& all1Ids, const QSet<ElementId>& all2Ids,
    QSet<ElementId>& elementIdsAdded, QSet<ElementId>& elementIdsRemoved);

  void _writeConflateStatusSummary(QTextStream& out);
  void _writeConflateStatusDetail(QTextStream& out);
};

}

#endif // SCORE_MATCHES_DIFF_H
