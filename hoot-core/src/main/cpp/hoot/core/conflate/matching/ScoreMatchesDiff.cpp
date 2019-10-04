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
#include "ScoreMatchesDiff.h"

#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/visitors/ElementIdToTagValueMapper.h>
#include <hoot/core/io/ElementVisitorInputStream.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/conflate/matching/MatchStatusChange.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/CountManualMatchesVisitor.h>

namespace hoot
{

ScoreMatchesDiff::ScoreMatchesDiff() :
_numManualMatches1(0),
_numManualMatches2(0)
{
}

ScoreMatchesDiff::~ScoreMatchesDiff()
{
  clear();
}

void ScoreMatchesDiff::clear()
{
  _newlyWrongMatchSwitches.clear();
  _newlyCorrectMatchSwitches.clear();
  _elementIdsAdded.clear();
  _elementIdsRemoved.clear();

  _input1 = "";
  _input2 = "";
  if (_outputFile)
  {
    _outputFile->close();
  }
  _outputFile.reset();

  _numManualMatches1 = 0;
  _numManualMatches2 = 0;
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
    "Calculating match scoring differential for " << _input1.right(25) << " and " <<
    _input2.right(25) << "...");

  // tried not making this memory bound, but it made my head hurt too much...

  OsmMapPtr map1(new OsmMap());
  IoUtils::loadMap(map1, _input1, true);
  OsmMapPtr map2(new OsmMap());
  IoUtils::loadMap(map2, _input2, true);

  // get a list of all expected/actual match types

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

  // compile list of newly wrong and corrected in second

  const QSet<ElementId> wrong1 = _getWrong(map1);
  const QSet<ElementId> wrong2 = _getWrong(map2);
  //QSet<ElementId> allWrong = wrong1;
  //allWrong.unite(wrong2);
  QSet<ElementId> newlyWrong = wrong2;
  newlyWrong.subtract(wrong1);
  QSet<ElementId> newlyCorrect = wrong1;
  newlyCorrect.subtract(wrong2);

  LOG_VARD(wrong1.size());
  LOG_VARD(wrong2.size());
  LOG_VARD(newlyWrong.size());
  LOG_VARD(newlyCorrect.size());

  // for newly wrong/correct, compare actual to expected to group match changes by type

  _newlyWrongMatchSwitches = _getConflateStatusDiff(newlyWrong, expected2, actual2);
  _newlyCorrectMatchSwitches = _getConflateStatusDiff(newlyCorrect, expected2, actual2);

//  // Get a list of all element ids.
  const QSet<ElementId> all1Ids = _getAllIds(_input1);
  LOG_VARD(all1Ids.size());
//  // Record the value of hoot:actual for each element.
//  const QMap<ElementId, QString> actualTagMappings1 =
//    _getActualTagMappings(_input1, MetadataTags::HootActual());
//  LOG_VARD(actualTagMappings1.size());
  const QSet<ElementId> all2Ids = _getAllIds(_input2);
  LOG_VARD(all2Ids.size());
//  const QMap<ElementId, QString> actualTagMappings2 =
//    _getActualTagMappings(_input2, MetadataTags::HootActual());
//  LOG_VARD(actualTagMappings2.size());

//  // Come up with a list of only those hoot:actual entries that differ for a feature between the
//  // two files.
//  const QSet<ElementId> elementIdsWithDifferingConflateStatus =
//    _getElementIdsWithDifferingConflateStatus(actualTagMappings1, actualTagMappings2);
//  LOG_VARD(elementIdsWithDifferingConflateStatus.size());

//  // Group all changed matches by type.
//  _matchSwitches =
//    _getConflateStatusDiff(
//      elementIdsWithDifferingConflateStatus, actualTagMappings1, actualTagMappings2);
//  LOG_VARD(_matchSwitches.size());

  // Determine any ids that are new or have been removed.
  _setAddedAndRemovedElements(all1Ids, all2Ids, _elementIdsAdded, _elementIdsRemoved);
  LOG_VARD(_elementIdsAdded.size());
  LOG_VARD(_elementIdsRemoved.size());

  std::shared_ptr<CountManualMatchesVisitor> manualMatchVisitor(
    new CountManualMatchesVisitor());
  map1->visitRo(*manualMatchVisitor);
  _numManualMatches1 += manualMatchVisitor->getStat();
  LOG_VARD(_numManualMatches1);
  map2->visitRo(*manualMatchVisitor);
  _numManualMatches2 += manualMatchVisitor->getStat();
  LOG_VARD(_numManualMatches2);

  LOG_INFO(
    "Match scoring differential calculated for " <<
    StringUtils::formatLargeNumber(all1Ids.size() + all2Ids.size()) << " total elements.");
}

void ScoreMatchesDiff::printDiff(const QString& output)
{
  if (!output.toLower().endsWith(".txt"))
  {
    throw IllegalArgumentException(
      "Unsupported output format: " + output + " Must be an .txt file.");
  }
  else if (_newlyWrongMatchSwitches.isEmpty() && _newlyCorrectMatchSwitches.isEmpty() &&
           _elementIdsAdded.isEmpty() && _elementIdsRemoved.isEmpty())
  {
    LOG_WARN("No differences in conflate status. Did you calculate a differential?");
    return;
  }

  LOG_INFO(
    "Writing match scoring differential for " << _input1.right(25) << " and " <<
     _input2.right(25) << " to " << output.right(25) << "...");

  _outputFile = _getOutputFile(output);
  QTextStream out(_outputFile.get());

  out << "Input files: " << _input1.right(25) << " and " << _input2.right(25) << "\n\n";
  _writeConflateStatusSummary(out);
  _writeConflateStatusDetail(out);

  LOG_INFO("Match scoring differential written to " << output << ".");
}

std::shared_ptr<QFile> ScoreMatchesDiff::_getOutputFile(const QString& outputPath)
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

QSet<ElementId> ScoreMatchesDiff::_getAllIds(const QString& input)
{
  std::shared_ptr<ElementVisitorInputStream> inputStrm =
    IoUtils::getVisitorInputStream(
      input, QString::fromStdString(UniqueElementIdVisitor::className()), true);
  std::shared_ptr<UniqueElementIdVisitor> vis =
    std::dynamic_pointer_cast<UniqueElementIdVisitor>(inputStrm->getVisitor());
  inputStrm->processAll();
  return CollectionUtils::stdSetToQSet(vis->getElementSet());
}

QMap<ElementId, QString> ScoreMatchesDiff::_getMatchStatuses(
  const ConstOsmMapPtr& map, const QString& tagKey)
{
  ElementIdToTagValueMapper vis;
  vis.setTagKey(tagKey);
  map->visitRo(vis);
  return vis.getIdToTagValueMappings();
}

QSet<ElementId> ScoreMatchesDiff::_getWrong(const ConstOsmMapPtr& map)
{
  UniqueElementIdVisitor idVis;
  FilteredVisitor filteredVis(TagCriterion(MetadataTags::HootWrong(), "yes"), idVis);
  map->visitRo(filteredVis);
  return CollectionUtils::stdSetToQSet(idVis.getElementSet());
}

//QSet<ElementId> ScoreMatchesDiff::_getElementIdsWithDifferingConflateStatus(
//  const QMap<ElementId, QString>& actualTagMappings1,
//  const QMap<ElementId, QString>& actualTagMappings2)
//{
//  QSet<ElementId> elementIdsWithDifferingConflateStatus;
//  for (QMap<ElementId, QString>::const_iterator itr = actualTagMappings1.begin();
//       itr != actualTagMappings1.end(); ++itr)
//  {
//    if (actualTagMappings2[itr.key()] != itr.value())
//    {
//      elementIdsWithDifferingConflateStatus.insert(itr.key());
//    }
//  }
//  return elementIdsWithDifferingConflateStatus;
//}

QMap<QString, QSet<ElementId>> ScoreMatchesDiff::_getConflateStatusDiff(
  const QSet<ElementId>& elementIds, const QMap<ElementId, QString>& expectedTagMappings,
  const QMap<ElementId, QString>& actualTagMappings)
{
  QMap<QString, QSet<ElementId>> matchSwitches;
  for (QSet<ElementId>::const_iterator itr = elementIds.begin(); itr != elementIds.end(); ++itr)
  {
    const ElementId id = *itr;
    QString expectedMatchTypeStr = expectedTagMappings[id];
    MatchType expectedMatchType = MatchType(expectedMatchTypeStr);
    QString actualMatchTypeStr = actualTagMappings[id];
    MatchType actualMatchType = MatchType(actualMatchTypeStr);

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
      throw IllegalArgumentException("TODO");
    }
  }
  return matchSwitches;
}

void ScoreMatchesDiff::_setAddedAndRemovedElements(
  const QSet<ElementId>& all1Ids, const QSet<ElementId>& all2Ids,
  QSet<ElementId>& elementIdsAdded, QSet<ElementId>& elementIdsRemoved)
{
  QSet<ElementId> allIds = all1Ids;
  allIds = allIds.unite(all2Ids);
  elementIdsAdded = allIds;
  elementIdsAdded.subtract(all1Ids);
  elementIdsRemoved = allIds;
  elementIdsRemoved.subtract(all2Ids);
}

void ScoreMatchesDiff::_writeConflateStatusSummary(QTextStream& out)
{
  out << "Conflate status summary:\n\n";
  out << StringUtils::formatLargeNumber(_numManualMatches1) << " manual matches in first file.\n";
  out << StringUtils::formatLargeNumber(_numManualMatches2) << " manual matches in second file.\n";
  out << StringUtils::formatLargeNumber(_elementIdsRemoved.size()) << " old elements were removed.\n";
  out << StringUtils::formatLargeNumber(_elementIdsAdded.size()) << " new elements were added.\n";
  out << StringUtils::formatLargeNumber(_newlyWrongMatchSwitches.size()) <<
         " new wrong matches were added.\n";
  out << StringUtils::formatLargeNumber(_newlyCorrectMatchSwitches.size()) <<
         " new correct matches were added.\n";
  for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyWrongMatchSwitches.begin();
       itr != _newlyWrongMatchSwitches.end(); ++itr)
  {
    const QString changeType = itr.key();
    const int numChanged = itr.value().size();
    out << StringUtils::formatLargeNumber(numChanged) << " new wrong matches switched from "
        << changeType << ".\n";
  }
  for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyCorrectMatchSwitches.begin();
       itr != _newlyCorrectMatchSwitches.end(); ++itr)
  {
    const QString changeType = itr.key();
    const int numChanged = itr.value().size();
    out << StringUtils::formatLargeNumber(numChanged) << " new correct matches switched from "
        << changeType << ".\n";
  }
}

void ScoreMatchesDiff::_writeConflateStatusDetail(QTextStream& out)
{
  out << "\n\nMatch type changes:\n\n";
  if (_newlyWrongMatchSwitches.size() + _newlyCorrectMatchSwitches.size() == 0)
  {
    out << "None";
  }
  else
  {
    out << "\n\nNew wrong matches:\n\n";
    for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyWrongMatchSwitches.begin();
         itr != _newlyWrongMatchSwitches.end(); ++itr)
    {
      const QString matchSwitchTypeStr = itr.key();
      out << matchSwitchTypeStr << "\n\n";
      const QSet<ElementId> ids = itr.value();
      for (QSet<ElementId>::const_iterator itr2 = ids.begin(); itr2 != ids.end(); ++itr2)
      {
        const ElementId id = *itr2;
        out << id.toString() << "\n";
      }
    }
    out << "\n\nNew correct matches:\n\n";
    for (QMap<QString, QSet<ElementId>>::const_iterator itr = _newlyCorrectMatchSwitches.begin();
         itr != _newlyCorrectMatchSwitches.end(); ++itr)
    {
      const QString matchSwitchTypeStr = itr.key();
      out << matchSwitchTypeStr << "\n\n";
      const QSet<ElementId> ids = itr.value();
      for (QSet<ElementId>::const_iterator itr2 = ids.begin(); itr2 != ids.end(); ++itr2)
      {
        const ElementId id = *itr2;
        out << id.toString() << "\n";
      }
    }
  }

  out << "\n\nAdded elements:\n\n";
  if (_elementIdsAdded.size() == 0)
  {
     out << "None";
  }
  else
  {
    for (QSet<ElementId>::const_iterator itr = _elementIdsAdded.begin();
         itr != _elementIdsAdded.end(); ++itr)
    {
      const ElementId id = *itr;
      out << id.toString() << "\n";
    }
  }

  out << "\n\nRemoved elements:\n\n";
  if (_elementIdsRemoved.size() == 0)
  {
     out << "None";
  }
  else
  {
    for (QSet<ElementId>::const_iterator itr = _elementIdsRemoved.begin();
         itr != _elementIdsRemoved.end(); ++itr)
    {
      const ElementId id = *itr;
      out << id.toString() << "\n";
    }
  }
}

}
