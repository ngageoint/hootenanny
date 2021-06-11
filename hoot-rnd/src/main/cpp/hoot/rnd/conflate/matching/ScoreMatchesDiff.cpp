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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ScoreMatchesDiff.h"

#include <hoot/core/conflate/matching/MatchStatusChange.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/visitors/CountManualMatchesVisitor.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>
#include <hoot/core/visitors/ElementIdToTagValueMapper.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>

namespace hoot
{

ScoreMatchesDiff::ScoreMatchesDiff() :
_numManualMatches(0),
_reviewDifferential(0)
{
}

ScoreMatchesDiff::~ScoreMatchesDiff()
{
  clear();
}

void ScoreMatchesDiff::clear()
{
  _newlyWrong.clear();
  _newlyCorrect.clear();
  _newlyWrongMatchSwitches.clear();
  _newlyCorrectMatchSwitches.clear();
  _elementIdsAdded.clear();
  _elementIdsRemoved.clear();

  _input1 = "";
  _input2 = "";
  _output = "";
  if (_outputFile)
  {
    _outputFile->close();
  }
  _outputFile.reset();

  _numManualMatches = 0;
}

void ScoreMatchesDiff::calculateDiff(const QString& input1, const QString& input2)
{
  if (!OsmXmlReader().isSupported(input1))
  {
    throw IllegalArgumentException(
      "Unsupported input format: " + input1 + " Must be an .osm file.");
  }
  else if (!OsmXmlReader().isSupported(input2))
  {
    throw IllegalArgumentException(
      "Unsupported input format: " + input2 + " Must be an .osm file.");
  }

  _input1 = input1;
  _input2 = input2;
  LOG_INFO(
    "Calculating match scoring differential for " << FileUtils::toLogFormat(_input1, 25) << " and " <<
    FileUtils::toLogFormat(_input2, 25) << "...");

  // Considered making this not memory bound, but our scoring data outputs aren't ever humongous
  // and the inputs had to be conflated at some point where they were read completely into
  // memory anyway. There's a lot of repeated element processing here that could be optimized.

  OsmMapPtr map1(new OsmMap());
  IoUtils::loadMap(map1, _input1, true);
  map1->setName("original");
  OsmMapPtr map2(new OsmMap());
  IoUtils::loadMap(map2, _input2, true);
  map2->setName("new");

  LOG_VARD(map1->size());
  LOG_VARD(map2->size());

  // count the manual matches made

  std::shared_ptr<CountManualMatchesVisitor> manualMatchVisitor(
    new CountManualMatchesVisitor());
  map1->visitRo(*manualMatchVisitor);
  double numManualMatches1 = manualMatchVisitor->getStat();
  manualMatchVisitor.reset(new CountManualMatchesVisitor());
  map2->visitRo(*manualMatchVisitor);
  double numManualMatches2 = manualMatchVisitor->getStat();

  LOG_VARD(numManualMatches1);
  LOG_VARD(numManualMatches2);

  // TODO: This ends up being true a lot, so maybe not a good check?
//  if (numManualMatches1 != numManualMatches2)
//  {
//    throw HootException(
//      QString("The two input datasets have a different number of manual matches (") +
//      QString::number(numManualMatches1) + " and " + QString::number(numManualMatches2) +
//      QString(") and, therefore, must not been derived from the same input data."));
//  }
  _numManualMatches = numManualMatches1;

  // get all expected/actual match types

  const QMap<ElementId, QString> expected1 =
    _getMatchStatuses(map1, MetadataTags::HootExpected());
  const QMap<ElementId, QString> actual1 =
    _getMatchStatuses(map1, MetadataTags::HootActual());
  const QMap<ElementId, QString> expected2 =
    _getMatchStatuses(map2, MetadataTags::HootExpected());
  const QMap<ElementId, QString> actual2 =
    _getMatchStatuses(map2, MetadataTags::HootActual());

  LOG_VARD(expected1.size());
  LOG_VARD(actual1.size());
  assert(expected1.size() == actual1.size());
  LOG_VARD(expected2.size());
  LOG_VARD(actual2.size());
  assert(expected2.size() == actual2.size());

  // get newly wrong and corrected matches

  const QSet<ElementId> wrong1 = _getWrong(map1);
  const QSet<ElementId> wrong2 = _getWrong(map2);
  _newlyWrong = wrong2;
  _newlyWrong.subtract(wrong1);
  _newlyCorrect = wrong1;
  _newlyCorrect.subtract(wrong2);

  LOG_VARD(wrong1.size());
  LOG_VARD(wrong2.size());
  LOG_VARD(_newlyWrong.size());
  LOG_VARD(_newlyCorrect.size());

  // for newly wrong/correct, compare actual to expected to group match changes by type
  // Is sending in expected2/actual2 correct here?

  _newlyWrongMatchSwitches = _getMatchScoringDiff(_newlyWrong, expected2, actual2);
  _newlyCorrectMatchSwitches = _getMatchScoringDiff(_newlyCorrect, expected2, actual2);

  LOG_VARD(_newlyWrongMatchSwitches.size());
  for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyWrongMatchSwitches.begin();
       itr != _newlyWrongMatchSwitches.end(); ++itr)
  {
    LOG_DEBUG(itr.key() << " " << itr.value().size());
  }
  LOG_VARD(_newlyCorrectMatchSwitches.size());
  for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyCorrectMatchSwitches.begin();
       itr != _newlyCorrectMatchSwitches.end(); ++itr)
  {
    LOG_DEBUG(itr.key() << " " << itr.value().size());
  }

  // get a list of all element ids

  const QSet<ElementId> all1Ids = _getAllIds(map1);
  const QSet<ElementId> all2Ids = _getAllIds(map2);

  LOG_VARD(all1Ids.size());
  LOG_VARD(all2Ids.size());

  // determine any ids that are new or have been removed

  _setAddedAndRemovedElements(all1Ids, all2Ids, _elementIdsAdded, _elementIdsRemoved);

  LOG_VARD(_elementIdsAdded.size());
  LOG_VARD(_elementIdsRemoved.size());

  // count the reviews

  CountUniqueReviewsVisitor countReviewsVis;
  map1->visitRo(countReviewsVis);
  const int numReviews1 = (int)countReviewsVis.getStat();
  countReviewsVis.clear();
  map2->visitRo(countReviewsVis);
  const int numReviews2 = (int)countReviewsVis.getStat();
  _reviewDifferential = numReviews2 - numReviews1;

  LOG_VARD(numReviews1);
  LOG_VARD(numReviews2);
  LOG_VARD(_reviewDifferential);

  LOG_INFO(
    "Match scoring differential calculated for " <<
    StringUtils::formatLargeNumber(all1Ids.size() + all2Ids.size()) << " total elements.");
}

bool ScoreMatchesDiff::printDiff(const QString& output)
{
  _output = output;
  if (_newlyWrongMatchSwitches.isEmpty() && _newlyCorrectMatchSwitches.isEmpty() &&
      _elementIdsAdded.isEmpty() && _elementIdsRemoved.isEmpty())
  {
    LOG_WARN(
      "There are no differences in match scoring status between the two input files. Did you " <<
      "calculate a differential? Are the input files identical?");
    return false;
  }

  LOG_INFO(
    "Writing match scoring differential for " << FileUtils::toLogFormat(_input1, 25) << " and " <<
     FileUtils::toLogFormat(_input2, 25) << " to " << FileUtils::toLogFormat(_output, 25) << "...");

  _outputFile = _getOutputFile(_output);
  QTextStream out(_outputFile.get());

  out << "Input files: ..." << FileUtils::toLogFormat(_input1, 25) << " and ..." << FileUtils::toLogFormat(_input2, 25) << "\n\n";
  _writeConflateStatusSummary(out);
  _writeConflateStatusDetail(out);

  return true;
}

std::shared_ptr<QFile> ScoreMatchesDiff::_getOutputFile(const QString& outputPath) const
{
  std::shared_ptr<QFile> outputFile(new QFile(outputPath));
  if (outputFile->exists())
  {
    outputFile->remove();
  }
  if (!outputFile->open(QFile::WriteOnly | QFile::Text))
  {
    throw HootException("Unable to write to output file.");
  }
  return outputFile;
}

QSet<ElementId> ScoreMatchesDiff::_getAllIds(const ConstOsmMapPtr& map)
{
  LOG_DEBUG("Retrieving all IDs for " << map->getName() << "...");
  UniqueElementIdVisitor idVis;
  map->visitRo(idVis);
  return CollectionUtils::stdSetToQSet(idVis.getElementSet());
}

QMap<ElementId, QString> ScoreMatchesDiff::_getMatchStatuses(
  const ConstOsmMapPtr& map, const QString& tagKey) const
{
  LOG_DEBUG("Retrieving match status: " << tagKey << " for " << map->getName() << "...");
  ElementIdToTagValueMapper vis;
  vis.setTagKey(tagKey);
  map->visitRo(vis);
  return vis.getIdToTagValueMappings();
}

QSet<ElementId> ScoreMatchesDiff::_getWrong(const ConstOsmMapPtr& map)
{
  LOG_DEBUG("Retrieving wrongly match elements for " << map->getName() << "...");
  UniqueElementIdVisitor idVis;
  TagCriterion crit(MetadataTags::HootWrong(), "1");
  FilteredVisitor filteredVis(crit, idVis);
  map->visitRo(filteredVis);
  return CollectionUtils::stdSetToQSet(idVis.getElementSet());
}

QMap<QString, QSet<ElementId>> ScoreMatchesDiff::_getMatchScoringDiff(
  const QSet<ElementId>& elementIds, const QMap<ElementId, QString>& expectedTagMappings,
  const QMap<ElementId, QString>& actualTagMappings) const
{
  LOG_DEBUG("Calculating match scoring diff...");
  QMap<QString, QSet<ElementId>> matchSwitches;
  for (QSet<ElementId>::const_iterator itr = elementIds.begin(); itr != elementIds.end(); ++itr)
  {
    const ElementId id = *itr;
    if (!expectedTagMappings.contains(id))
    {
      // TODO: This ends up happening a lot, so maybe not a good reason to throw an error?
      //throw HootException("Can't find ID: " + id.toString() + " for expected.");
      continue;
    }
    QString expectedMatchTypeStr = expectedTagMappings[id];
    MatchType expectedMatchType = MatchType(expectedMatchTypeStr);
    if (!actualTagMappings.contains(id))
    {
      // See note above.
      //throw HootException("Can't find ID: " + id.toString() + " for actual.");
      continue;
    }
    QString actualMatchTypeStr = actualTagMappings[id];
    MatchType actualMatchType = MatchType(actualMatchTypeStr);

    LOG_VART(expectedMatchTypeStr);
    LOG_VART(actualMatchTypeStr);

    if (expectedMatchType == MatchType::Match && actualMatchType == MatchType::Miss)
    {
      matchSwitches[MatchStatusChange(MatchStatusChange::MatchToMiss).toString()].insert(id);
    }
    else if (expectedMatchType == MatchType::Match && actualMatchType == MatchType::Review)
    {
      matchSwitches[MatchStatusChange(MatchStatusChange::MatchToReview).toString()].insert(id);
    }
    else if (expectedMatchType == MatchType::Miss && actualMatchType == MatchType::Match)
    {
      matchSwitches[MatchStatusChange(MatchStatusChange::MissToMatch).toString()].insert(id);
    }
    else if (expectedMatchType == MatchType::Miss && actualMatchType == MatchType::Review)
    {
      matchSwitches[MatchStatusChange(MatchStatusChange::MissToReview).toString()].insert(id);
    }
    else if (expectedMatchType == MatchType::Review && actualMatchType == MatchType::Match)
    {
      matchSwitches[MatchStatusChange(MatchStatusChange::ReviewToMatch).toString()].insert(id);
    }
    else if (expectedMatchType == MatchType::Review && actualMatchType == MatchType::Miss)
    {
      matchSwitches[MatchStatusChange(MatchStatusChange::ReviewToMiss).toString()].insert(id);
    }
    else
    {
      // shouldn't ever get here
      throw HootException("Error parsing match types with ScoreMatchesDiff.");
    }
  }
  return matchSwitches;
}

void ScoreMatchesDiff::_setAddedAndRemovedElements(
  const QSet<ElementId>& all1Ids, const QSet<ElementId>& all2Ids,
  QSet<ElementId>& elementIdsAdded, QSet<ElementId>& elementIdsRemoved) const
{
  LOG_DEBUG("Recording added/removed elements...");
  QSet<ElementId> allIds = all1Ids;
  allIds = allIds.unite(all2Ids);
  elementIdsAdded = allIds;
  elementIdsAdded.subtract(all1Ids);
  elementIdsRemoved = allIds;
  elementIdsRemoved.subtract(all2Ids);
}

void ScoreMatchesDiff::_writeConflateStatusSummary(QTextStream& out)
{
  LOG_DEBUG("Writing conflate status summary...");

  QString summary;
  summary += "Match Scoring Differential Summary:\n\n";
  summary +=
    StringUtils::formatLargeNumber(_numManualMatches) + " manual matches.\n";
  summary +=
    StringUtils::formatLargeNumber(_elementIdsRemoved.size()) +
    " elements from the first file were removed in the second file.\n";
  summary +=
    StringUtils::formatLargeNumber(_elementIdsAdded.size()) +
    " new elements were added to the second file.\n";
  summary +=
    StringUtils::formatLargeNumber(_newlyWrong.size()) +
    " new elements are involved in wrong matches.\n";
  summary +=
    StringUtils::formatLargeNumber(_newlyCorrect.size()) +
    " new elements are involved in correct matches.\n";
  for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyWrongMatchSwitches.begin();
       itr != _newlyWrongMatchSwitches.end(); ++itr)
  {
    const QString changeType = itr.key();
    const int numChanged = itr.value().size();
    summary +=
      StringUtils::formatLargeNumber(numChanged) + " new wrong matches switched from " +
      changeType + ".\n";
  }
  for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyCorrectMatchSwitches.begin();
       itr != _newlyCorrectMatchSwitches.end(); ++itr)
  {
    const QString changeType = itr.key();
    const int numChanged = itr.value().size();
    summary +=
      StringUtils::formatLargeNumber(numChanged) + " new correct matches switched from " +
      changeType + ".\n";
  }
  if (_reviewDifferential > 0)
  {
    summary += QString::number(_reviewDifferential) + " reviews were added.\n";
  }
  else if (_reviewDifferential == 0)
  {
    summary += "No reviews were added.\n";
  }
  else if (_reviewDifferential < 0)
  {
    summary += QString::number(abs(_reviewDifferential)) + " reviews were lost.\n";
  }

  summary = summary.trimmed();
  std::cout << summary << std::endl;
  std::cout << "\nDetailed information available in: " << _output << "." << std::endl;

  out << summary;
}

void ScoreMatchesDiff::_writeConflateStatusDetail(QTextStream& out)
{
  LOG_DEBUG("Printing conflate status detail...");

  QString detail;
  detail += "\n\nMatch Type Changes\n";
  if (_newlyWrongMatchSwitches.size() + _newlyCorrectMatchSwitches.size() == 0)
  {
    detail += "none";
  }
  else
  {
    detail += "\nNew Wrong Matches:\n";
    if (_newlyWrongMatchSwitches.empty())
    {
      detail += "none";
    }
    else
    {
      for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyWrongMatchSwitches.begin();
           itr != _newlyWrongMatchSwitches.end(); ++itr)
      {
        const QString matchSwitchTypeStr = itr.key();
        detail += "\n" + matchSwitchTypeStr + ":\n\n";
        QList<ElementId> ids = itr.value().toList();
        qSort(ids);
        for (QList<ElementId>::const_iterator itr2 = ids.begin(); itr2 != ids.end(); ++itr2)
        {
          const ElementId id = *itr2;
          detail += id.toString() + "\n";
        }
      }
      detail = detail.trimmed();
    }

    detail += "\n\nNew Correct Matches:\n\n";
    if (_newlyCorrectMatchSwitches.empty())
    {
      detail += "none";
    }
    else
    {
      for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyCorrectMatchSwitches.begin();
           itr != _newlyCorrectMatchSwitches.end(); ++itr)
      {
        const QString matchSwitchTypeStr = itr.key();
        detail += "\n" + matchSwitchTypeStr + "\n\n";
        QList<ElementId> ids = itr.value().toList();
        qSort(ids);
        for (QList<ElementId>::const_iterator itr2 = ids.begin(); itr2 != ids.end(); ++itr2)
        {
          const ElementId id = *itr2;
          detail += id.toString() + "\n";
        }
      }
    }
    detail = detail.trimmed();
  }

  detail +="\n\nAdded Elements:\n\n";
  if (_elementIdsAdded.empty())
  {
    detail += "none";
  }
  else
  {
    QList<ElementId> elementIdsAdded = _elementIdsAdded.toList();
    qSort(elementIdsAdded);
    for (QList<ElementId>::const_iterator itr = elementIdsAdded.begin();
         itr != elementIdsAdded.end(); ++itr)
    {
      const ElementId id = *itr;
      detail += id.toString() + "\n";
    }
    detail = detail.trimmed();
  }

  detail += "\n\nRemoved Elements:\n\n";
  if (_elementIdsRemoved.empty())
  {
     detail += "none";
  }
  else
  {
    QList<ElementId> elementIdsRemoved = _elementIdsRemoved.toList();
    qSort(elementIdsRemoved);
    for (QList<ElementId>::const_iterator itr = elementIdsRemoved.begin();
         itr != elementIdsRemoved.end(); ++itr)
    {
      const ElementId id = *itr;
      detail += id.toString() + "\n";
    }
    detail = detail.trimmed();
  }

  out << detail;
}

}
