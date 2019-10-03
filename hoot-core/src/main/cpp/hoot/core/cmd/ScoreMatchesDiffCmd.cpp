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

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/visitors/ElementIdToTagValueMapper.h>
#include <hoot/core/io/ElementVisitorInputStream.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/conflate/matching/MatchStatusChange.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/core/util/CollectionUtils.h>

namespace hoot
{

class ScoreMatchesDiffCmd : public BaseCommand
{

public:

  static std::string className() { return "hoot::ScoreMatchesDiffCmd"; }

  ScoreMatchesDiffCmd()
  {
//    _matchSwitchTypeCounts[MatchToMiss] = 0;
//    _matchSwitchTypeCounts[MatchToReview] = 0;
//    _matchSwitchTypeCounts[MissToMatch] = 0;
//    _matchSwitchTypeCounts[MissToReview] = 0;
//    _matchSwitchTypeCounts[ReviewToMatch] = 0;
//    _matchSwitchTypeCounts[ReviewToMiss] = 0;
  }

  virtual QString getName() const override { return "score-matches-diff"; }

  virtual QString getDescription() const override
  { return "Compares scored conflation performance between different invocations of score-matches"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() != 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    const QString input1 = args[0].trimmed();
    const QString input2 = args[1].trimmed();
    const QString output = args[2].trimmed();

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
    else if (!output.toLower().endsWith(".txt"))
    {
      throw IllegalArgumentException(
        "Unsupported output format: " + output + " Must be an .txt file.");
    }

    std::shared_ptr<QFile> outputFile;
    try
    {
      outputFile = _getOutputFile(output);
    }
    catch (const HootException& e)
    {
      LOG_ERROR(e.what());
      return 1;
    }
    QTextStream out(outputFile.get());

    // Go through each input file and record the value of hoot:actual for each feature.
    const QSet<ElementId> all1Ids = _getAllIds(input1);
    const QMap<ElementId, QString> actualTagMappings1 = _getActualTagMappings(input1);
    const QSet<ElementId> all2Ids = _getAllIds(input2);
    const QMap<ElementId, QString> actualTagMappings2 = _getActualTagMappings(input2);

    // Come up with a list of only those hoot:actual entries that differ for a feature between the
    // two files.
    const QSet<ElementId> elementIdsWithDifferingConflateStatus =
      _getElementIdsWithDifferingConflateStatus(actualTagMappings1, actualTagMappings2);

    // Group all changed matches by type.
    const QMap<QString, QSet<ElementId>> matchSwitches =
      _getConflateStatusDiff(
        elementIdsWithDifferingConflateStatus, actualTagMappings1, actualTagMappings2);

    // Compile a list of feature ids in the first dataset and not the second and vice versa and
    // write those to the summary file as well.
    QSet<ElementId> elementIdsAdded;
    QSet<ElementId> elementIdsRemoved;
    _getAddedAndRemovedElements(all1Ids, all2Ids, elementIdsAdded, elementIdsRemoved);

    out << "Input files: " << input1.right(25) << " and " << input2.right(25) << "\n\n";

    // Also, output a summary with the number of features that switched from match to miss, match to
    // review, miss to match, miss to review, review to match, and review to miss.
    _writeConflateStatusSummary(
      matchSwitches, elementIdsAdded.size(), elementIdsRemoved.size(), out);

    // For each of the features in the list, write a line to a summary text file with the element's
    // id and before and after conflate statuses to denote the conflate differences between the two
    // files.
    _writeConflateStatusDetail(matchSwitches, elementIdsAdded, elementIdsRemoved, out);

    outputFile->close();

    return 0;
  }

private:

  std::shared_ptr<QFile> _getOutputFile(const QString& outputPath)
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

  void _writeConflateStatusSummary(
    const QMap<QString, QSet<ElementId>>& matchSwitches, const int numElementIdsAdded,
    const int numElementIdsRemoved, QTextStream& out)
  {
    out << "\nConflate status summary:\n\n";
    out << StringUtils::formatLargeNumber(numElementIdsRemoved) << " old elements were removed.\n";
    out << StringUtils::formatLargeNumber(numElementIdsAdded) << " new elements were added.\n";
    for (QMap<QString, QSet<ElementId>>::const_iterator itr = matchSwitches.begin();
         itr != matchSwitches.end(); ++itr)
    {
      const QString changeType = itr.key();
      const int numChanged = itr.value().size();
      out << StringUtils::formatLargeNumber(numChanged) << " elements switched from "
          << changeType << ".\n";
    }
  }

  QMap<QString, QSet<ElementId>> _getConflateStatusDiff(
    const QSet<ElementId>& elementIds, const QMap<ElementId, QString>& actualTagMappings1,
    const QMap<ElementId, QString>& actualTagMappings2)
  {
    QMap<QString, QSet<ElementId>> matchSwitches;
    for (QSet<ElementId>::const_iterator itr = elementIds.begin(); itr != elementIds.end(); ++itr)
    {
      const ElementId id = *itr;
      QString oldMatchTypeStr = actualTagMappings1[id];
      MatchType oldMatchType = MatchType(oldMatchTypeStr);
      QString newMatchTypeStr = actualTagMappings2[id];
      MatchType newMatchType = MatchType(newMatchTypeStr);

      if (oldMatchType == MatchType::Match && newMatchType == MatchType::Miss)
      {
        matchSwitches[MatchStatusChange(MatchStatusChange::MatchToMiss).toString()].insert(id);
      }
      else if (oldMatchType == MatchType::Match && newMatchType == MatchType::Review)
      {
        matchSwitches[MatchStatusChange(MatchStatusChange::MatchToReview).toString()].insert(id);
      }
      else if (oldMatchType == MatchType::Miss && newMatchType == MatchType::Match)
      {
        matchSwitches[MatchStatusChange(MatchStatusChange::MissToMatch).toString()].insert(id);
      }
      else if (oldMatchType == MatchType::Miss && newMatchType == MatchType::Review)
      {
        matchSwitches[MatchStatusChange(MatchStatusChange::MissToReview).toString()].insert(id);
      }
      else if (oldMatchType == MatchType::Review && newMatchType == MatchType::Match)
      {
        matchSwitches[MatchStatusChange(MatchStatusChange::ReviewToMatch).toString()].insert(id);
      }
      else if (oldMatchType == MatchType::Review && newMatchType == MatchType::Miss)
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

  void _writeConflateStatusDetail(
    const QMap<QString, QSet<ElementId>>& matchSwitches,
    const QSet<ElementId>& elementIdsAdded, const QSet<ElementId>& elementIdsRemoved,
    QTextStream& out)
  {
    out << "Match type changes:\n\n";
    for (QMap<QString, QSet<ElementId>>::const_iterator itr = matchSwitches.begin();
         itr != matchSwitches.end(); ++itr)
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

    out << "Added elements:\n\n";
    for (QSet<ElementId>::const_iterator itr = elementIdsAdded.begin();
         itr != elementIdsAdded.end(); ++itr)
    {
      const ElementId id = *itr;
      out << id.toString() << "\n";
    }

    out << "Removed elements:\n\n";
    for (QSet<ElementId>::const_iterator itr = elementIdsRemoved.begin();
         itr != elementIdsRemoved.end(); ++itr)
    {
      const ElementId id = *itr;
      out << id.toString() << "\n";
    }
  }

  void _getAddedAndRemovedElements(
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

  QSet<ElementId> _getAllIds(const QString& input)
  {
    std::shared_ptr<ElementVisitorInputStream> inputStrm =
      IoUtils::getVisitorInputStream(
        input, QString::fromStdString(UniqueElementIdVisitor::className()), true);
    std::shared_ptr<UniqueElementIdVisitor> vis =
      std::dynamic_pointer_cast<UniqueElementIdVisitor>(inputStrm->getVisitor());
    inputStrm->processAll();
    return CollectionUtils::stdSetToQSet(vis->getElementSet());
  }

  QMap<ElementId, QString> _getActualTagMappings(const QString& input)
  {
    std::shared_ptr<ElementVisitorInputStream> inputStrm =
      IoUtils::getVisitorInputStream(
        input, QString::fromStdString(ElementIdToTagValueMapper::className()), true);
    std::shared_ptr<ElementIdToTagValueMapper> vis =
      std::dynamic_pointer_cast<ElementIdToTagValueMapper>(inputStrm->getVisitor());
    vis->setTagKey(MetadataTags::HootActual());
    inputStrm->processAll();
    return vis->getIdToTagValueMappings();
  }

  QSet<ElementId> _getElementIdsWithDifferingConflateStatus(
    const QMap<ElementId, QString>& actualTagMappings1,
    const QMap<ElementId, QString>& actualTagMappings2)
  {
    QSet<ElementId> elementIdsWithDifferingConflateStatus;
    for (QMap<ElementId, QString>::const_iterator itr = actualTagMappings1.begin();
         itr != actualTagMappings1.end(); ++itr)
    {
      if (actualTagMappings2[itr.key()] != itr.value())
      {
        elementIdsWithDifferingConflateStatus.insert(itr.key());
      }
    }
    return elementIdsWithDifferingConflateStatus;
  }
};

HOOT_FACTORY_REGISTER(Command, ScoreMatchesDiffCmd)

}

