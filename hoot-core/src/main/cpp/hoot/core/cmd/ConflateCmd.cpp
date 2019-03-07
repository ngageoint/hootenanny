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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ConflateCmd.h"

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/stats/ConflateStatsHelper.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/io/MapStatsWriter.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/stats/IoSingleStat.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/visitors/CriterionCountVisitor.h>
#include <hoot/core/algorithms/changeset/MultipleChangesetProvider.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>
#include <hoot/core/visitors/MatchCandidateCountVisitor.h>
#include <hoot/core/conflate/matching/MatchFactory.h>

// Standard
#include <fstream>

// Tgs
#include <tgs/System/Timer.h>

#include <QFileInfo>

using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(Command, ConflateCmd)

void ConflateCmd::printStats(const QList<SingleStat>& stats)
{
  QString sep = "\t";
  cout << "== Stats ==" << endl;
  for (int i = 0; i < stats.size(); i++)
  {
    cout << stats[i].name << sep << stats[i].value << endl;
  }
}

int ConflateCmd::runSimple(QStringList args)
{
  Timer totalTime;
  Timer t;

  QList<SingleStat> stats;
  bool displayStats = false;
  //force stats to always be the last optional param so it can be followed by an optional
  //output file
  QString outputStatsFile;
  if (args.contains("--stats"))
  {
    if (args.endsWith("--stats"))
    {
      displayStats = true;
      //remove "--stats" from args list
      args.pop_back();
    }
    else if (args[args.size() - 1] == "--stats")
    {
      displayStats = true;
      outputStatsFile = args[args.size() - 1];
      //remove "--stats" and stats output file name from args list
      args.pop_back();
      args.pop_back();
    }
  }
  LOG_VARD(displayStats);
  LOG_VARD(outputStatsFile);

  DiffConflator diffConflator;

  bool isDiffConflate = false;

  if (args.contains("--differential"))
  {
    isDiffConflate = true;
    args.removeAt(args.indexOf("--differential"));    

    // Check for tags argument "--Include-Tags"

    if (args.contains("--include-tags"))
    {
      diffConflator.enableTags();
      args.removeAt(args.indexOf("--include-tags"));
    }
  }
  LOG_VARD(isDiffConflate);

  // Check for separate output files (for geometry & tags)
  bool separateOutput = false;
  if (args.contains("--separate-output"))
  {
    separateOutput = true;
    args.removeAt(args.indexOf("--separate-output"));
  }

  if (args.size() < 2 || args.size() > 3)
  {
    cout << getHelp() << endl << endl;
    throw HootException(QString("%1 takes two or three parameters.").arg(getName()));
  }

  QString input1 = args[0];
  QString input2, output;

  if (args.size() == 3)
  {
    input2 = args[1];
    output = args[2];
  }
  else
  {
    output = args[1];
  }

  QString msg =
    "Conflating " + input1.right(50) + " with " + input2.right(50) + " and writing the output to " +
    output.right(50);
  if (isDiffConflate)
  {
    msg = msg.prepend("Differentially ");
  }
  LOG_INFO(msg);

  double bytesRead = IoSingleStat(IoSingleStat::RChar).value;
  LOG_VART(bytesRead);
  QList<QList<SingleStat>> allStats;

  // read input 1
  OsmMapPtr map(new OsmMap());
  IoUtils::loadMap(map, input1,
                   ConfigOptions().getReaderConflateUseDataSourceIds1(),
                   Status::Unknown1);

  ChangesetProviderPtr pTagChanges;
  if (isDiffConflate)
  {
    // Store original IDs for tag diff
    diffConflator.storeOriginalMap(map);
    diffConflator.markInputElements(map);
  }

  // read input 2
  if (!input2.isEmpty())
  {
    IoUtils::loadMap(map, input2,
                     ConfigOptions().getReaderConflateUseDataSourceIds2(),
                     Status::Unknown2);
  }

  double inputBytes = IoSingleStat(IoSingleStat::RChar).value - bytesRead;
  LOG_VART(inputBytes);
  double elapsed = t.getElapsedAndRestart();
  stats.append(SingleStat("Read Inputs Time (sec)", elapsed));
  stats.append(SingleStat("(Dubious) Read Inputs Bytes", inputBytes));
  stats.append(SingleStat("(Dubious) Read Inputs Bytes per Second", inputBytes / elapsed));

  CalculateStatsOp input1Cso(
    ElementCriterionPtr(new StatusCriterion(Status::Unknown1)), "input map 1");
  CalculateStatsOp input2Cso(
    ElementCriterionPtr(new StatusCriterion(Status::Unknown2)), "input map 2");
  if (displayStats)
  {
    input1Cso.apply(map);
    allStats.append(input1Cso.getStats());
    stats.append(SingleStat("Time to Calculate Stats for Input 1 (sec)", t.getElapsedAndRestart()));

    if (input2 != "")
    {
      input2Cso.apply(map);
      allStats.append(input2Cso.getStats());
      stats.append(SingleStat("Time to Calculate Stats for Input 2 (sec)",
        t.getElapsedAndRestart()));
    }
  }

  size_t initialElementCount = map->getElementCount();
  stats.append(SingleStat("Initial Element Count", initialElementCount));
  OsmMapWriterFactory::writeDebugMap(map, "after-load");

  LOG_INFO("Applying pre-conflation operations...");
  NamedOp(ConfigOptions().getConflatePreOps()).apply(map);
  stats.append(SingleStat("Apply Named Ops Time (sec)", t.getElapsedAndRestart()));
  OsmMapWriterFactory::writeDebugMap(map, "after-pre-ops");

  /* Remove this hack after the following UI issues are fixed (should be fixed by hoot ui v2):
   *
   * https://github.com/ngageoint/hootenanny-ui/issues/969
   * https://github.com/ngageoint/hootenanny-ui/issues/970
   * https://github.com/ngageoint/hootenanny-ui/issues/971
   * https://github.com/ngageoint/hootenanny-ui/issues/972
   * */
  // should rename this to auto.correct.conflate.options now
  if (ConfigOptions().getAutocorrectOptions())
  {
    _tempFixDefaults();
  }

  // Let's see if we can get rid of any matchers and save some job run time.
  _removeUnneededMatchAndMergerCreators(map);

  OsmMapPtr result = map;

  if (isDiffConflate)
  {
    // call the diff conflator
    diffConflator.apply(result);
    if (diffConflator.conflatingTags())
    {
      pTagChanges = diffConflator.getTagDiff();
    }
    stats.append(diffConflator.getStats());
    stats.append(SingleStat("Conflation Time (sec)", t.getElapsedAndRestart()));
  }
  else
  {
    UnifyingConflator conflator;
    conflator.apply(result);
    stats.append(conflator.getStats());
    stats.append(SingleStat("Conflation Time (sec)", t.getElapsedAndRestart()));
  }

  // Apply any user specified operations.
  _updateConfigOptionsForAttributeConflation();
  LOG_INFO("Applying post-conflation operations...");
  LOG_VART(ConfigOptions().getConflatePostOps());
  NamedOp(ConfigOptions().getConflatePostOps()).apply(result);
  OsmMapWriterFactory::writeDebugMap(map, "after-post-ops");

  // doing this after the conflate post ops, since some invalid reviews are removed by them
  CountUniqueReviewsVisitor countReviewsVis;
  result->visitRo(countReviewsVis);
  LOG_INFO("Generated " << countReviewsVis.getStat() << " feature reviews.");

  MapProjector::projectToWgs84(result);
  stats.append(SingleStat("Project to WGS84 Time (sec)", t.getElapsedAndRestart()));

  // Figure out what to write
  if (isDiffConflate && output.endsWith(".osc"))
  {
    diffConflator.writeChangeset(result, output, separateOutput);
  }
  else
  {
    // Write a map
    if (isDiffConflate && diffConflator.conflatingTags())
    {
      // Add tag changes to our map
      diffConflator.addChangesToMap(result, pTagChanges);
    }
    IoUtils::saveMap(result, output);
  }

  // Do the tags if we need to
  if (isDiffConflate && diffConflator.conflatingTags())
  {
    LOG_INFO("Generating tag changeset...");
    // Write the file!
    QString outFileName = output;
    outFileName.replace(".osm", "");
  }

  double timingOutput = t.getElapsedAndRestart();

  if (displayStats)
  {
    CalculateStatsOp outputCso("output map", true);
    outputCso.apply(result);
    QList<SingleStat> outputStats = outputCso.getStats();
    if (input2 != "")
    {
      ConflateStatsHelper(input1Cso.getStats(), input2Cso.getStats(), outputCso.getStats())
        .updateStats(
          outputStats,
          outputCso.indexOfSingleStat("Total Unmatched Features"));
    }
    allStats.append(outputStats);
    stats.append(SingleStat("Time to Calculate Stats for Output (sec)", t.getElapsedAndRestart()));
  }

  double totalElapsed = totalTime.getElapsed();
  stats.append(SingleStat("(Dubious) Initial Elements Processed per Second",
                          initialElementCount / totalElapsed));
  stats.append(SingleStat("(Dubious) Final Elements Processed per Second",
                          result->getElementCount() / totalElapsed));
  stats.append(SingleStat("Write Output Time (sec)", timingOutput));
  stats.append(SingleStat("Final Element Count", result->getElementCount()));
  stats.append(SingleStat("Total Time Elapsed (sec)", totalElapsed));
  stats.append(IoSingleStat(IoSingleStat::RChar));
  stats.append(IoSingleStat(IoSingleStat::WChar));
  stats.append(IoSingleStat(IoSingleStat::SysCr));
  stats.append(IoSingleStat(IoSingleStat::SysCw));
  stats.append(IoSingleStat(IoSingleStat::ReadBytes));
  stats.append(IoSingleStat(IoSingleStat::WriteBytes));
  stats.append(IoSingleStat(IoSingleStat::CancelledWriteBytes));
  stats.append(SingleStat("(Dubious) Bytes Processed per Second", inputBytes / totalElapsed));

  if (isDiffConflate)
  {
    diffConflator.calculateStats(result, stats);
  }

  if (displayStats)
  {
    if (outputStatsFile.isEmpty())
    {
      allStats.append(stats);
      QString statsMsg = MapStatsWriter().statsToString(allStats, "\t");
      cout << "stats = (stat) OR (input map 1 stat) (input map 2 stat) (output map stat)\n" <<
              statsMsg << endl;
    }
    else
    {
      allStats.append(stats);
      MapStatsWriter().writeStatsToJson(allStats, outputStatsFile);
      cout << "stats = (stat) OR (input map 1 stat) (input map 2 stat) (output map stat) in file: " <<
              outputStatsFile << endl;
    }
  }

  LOG_INFO("Conflation job completed.");

  return 0;
}

void ConflateCmd::_removeUnneededMatchAndMergerCreators(ConstOsmMapPtr map)
{
  MatchCandidateCountVisitor matchCandidateCounter(MatchFactory::getInstance().getCreators());
  //LOG_INFO(matchCandidateCounter.getInitStatusMessage());
  map->visitRo(matchCandidateCounter);
  const QMap<QString, long> candidateCounts =
    boost::any_cast<QMap<QString, long>>(matchCandidateCounter.getData());
  LOG_INFO(matchCandidateCounter.getCompletedStatusMessage());
  LOG_DEBUG("Feature match candidates: " << candidateCounts);

  QStringList matchCreators = ConfigOptions().getMatchCreators();
  LOG_DEBUG("Match creators before candidate count checking: " << matchCreators);
  QStringList mergerCreators = ConfigOptions().getMergerCreators();
  LOG_DEBUG("Merger creators before candidate count checking: " << matchCreators);
  bool anyRemoved = false;
  for (QMap<QString, long>::const_iterator itr = candidateCounts.begin();
       itr != candidateCounts.end(); ++itr)
  {
    if (!matchCreators.contains(itr.key()))
    {
      LOG_INFO(
        "Removing match creator: " << itr.key() << ", as input data contains no candidate " <<
        "features for the corresponding type.")
      const int index = matchCreators.indexOf(itr.key());
      assert(index >= 0);
      matchCreators.removeAt(index);
      mergerCreators.removeAt(index);
      anyRemoved = true;
    }
  }
  if (anyRemoved)
  {
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchCreators);
    MatchFactory::setMatchCreators(matchCreators);
    conf().set(ConfigOptions::getMergerCreatorsKey(), mergerCreators);
  }
  LOG_DEBUG(
    "Match creators after match candidate count checking: " <<
    conf().get(ConfigOptions::getMatchCreatorsKey()));
  LOG_DEBUG(
    "Merger creators after match candidate count checking: " <<
    conf().get(ConfigOptions::getMergerCreatorsKey()));
}

void ConflateCmd::_updateConfigOptionsForAttributeConflation()
{
  // These are some custom adjustments to config opts that must be done for Attribute Conflation.
  // There may be a way to eliminate these by adding more custom behavior to the UI.

  // This option only gets used with Attribute Conflation for now, so we'll use it as the sole
  // identifier for it.  This could change in the future, but hopefully if that happens, by then
  // we have a better solution for changing these opts in place.
  if (ConfigOptions().getHighwayMergeTagsOnly())
  {
    // If we're running Attribute Conflation and removing building relations, we need to remove them
    // after the review relations have been removed or some building relations may still remain that
    // are involved in reviews.

    QStringList postConflateOps = ConfigOptions().getConflatePostOps();
    LOG_DEBUG("Post conflate ops before Attribute Conflation adjustment: " << postConflateOps);
    // Currently, all these things will be true if we're running Attribute Conflation, but I'm
    // specifying them anyway to harden this a bit.
    if (ConfigOptions().getBuildingOutlineUpdateOpRemoveBuildingRelations() &&
        postConflateOps.contains("hoot::RemoveElementsVisitor") &&
        ConfigOptions().getRemoveElementsVisitorElementCriterion() ==
          "hoot::ReviewRelationCriterion" &&
        postConflateOps.contains("hoot::BuildingOutlineUpdateOp"))
    {
      const int removeElementsVisIndex = postConflateOps.indexOf("hoot::RemoveElementsVisitor");
      const int buildingOutlineOpIndex = postConflateOps.indexOf("hoot::BuildingOutlineUpdateOp");
      if (removeElementsVisIndex > buildingOutlineOpIndex)
      {
        postConflateOps.removeAll("hoot::BuildingOutlineUpdateOp");
        postConflateOps.append("hoot::BuildingOutlineUpdateOp");
        conf().set("conflate.post.ops", postConflateOps);
      }
    }

    // This swaps the logic that removes all reviews with the logic that removes them based on score
    // thresholding.
    if (ConfigOptions().getAttributeConflationAllowReviewsByScore())
    {
      conf().set(
        ConfigOptions::getRemoveElementsVisitorElementCriterionKey(), "hoot::ReviewScoreCriterion");
    }

    LOG_DEBUG(
      "Post conflate ops after Attribute Conflation adjustment: " <<
      conf().get("conflate.post.ops").toStringList());
  }
}

void ConflateCmd::_tempFixDefaults()
{
  QStringList matchCreators = ConfigOptions().getMatchCreators();
  QStringList mergerCreators = ConfigOptions().getMergerCreators();
  LOG_VARD(matchCreators);
  LOG_VARD(mergerCreators);

  if ((matchCreators.size() == 0 || mergerCreators.size() == 0))
  {
    LOG_WARN("Match or merger creators empty.  Setting to defaults.");
    matchCreators = ConfigOptions::getMatchCreatorsDefaultValue();
    mergerCreators = ConfigOptions::getMergerCreatorsDefaultValue();
  }

  //fix matchers/mergers - https://github.com/ngageoint/hootenanny-ui/issues/972
  if (matchCreators.size() != mergerCreators.size())
  {
    //going to make the mergers match whatever the matchers are
    QStringList fixedMergerCreators;
    for (int i = 0; i < matchCreators.size(); i++)
    {
      const QString matchCreator = matchCreators.at(i);
      if (matchCreator == "hoot::BuildingMatchCreator")
      {
        fixedMergerCreators.append("hoot::BuildingMergerCreator");
      }
      else if (matchCreator.contains("hoot::ScriptMatchCreator"))
      {
        fixedMergerCreators.append("hoot::ScriptMergerCreator");
      }
      else if (matchCreator == "hoot::HighwayMatchCreator")
      {
        fixedMergerCreators.append("hoot::HighwayMergerCreator");
      }
      else if (matchCreator == "hoot::NetworkMatchCreator")
      {
        fixedMergerCreators.append("hoot::NetworkMergerCreator");
      }
      else if (matchCreator == "hoot::PoiPolygonMatchCreator")
      {
        fixedMergerCreators.append("hoot::PoiPolygonMergerCreator");
      }
    }
    LOG_DEBUG("Temp fixing merger.creators...");
    conf().set("merger.creators", fixedMergerCreators.join(";"));
  }
  LOG_VARD(mergerCreators);

  //fix way subline matcher options - https://github.com/ngageoint/hootenanny-ui/issues/970
  if (matchCreators.contains("hoot::NetworkMatchCreator") &&
      ConfigOptions().getWaySublineMatcher() != "hoot::FrechetSublineMatcher" &&
      ConfigOptions().getWaySublineMatcher() != "hoot::MaximalSublineMatcher")
  {
    LOG_DEBUG("Temp fixing way.subline.matcher...");
    conf().set("way.subline.matcher", "hoot::MaximalSublineMatcher");
  }
  else if (matchCreators.contains("hoot::HighwayMatchCreator") &&
           ConfigOptions().getWaySublineMatcher() != "hoot::FrechetSublineMatcher" &&
           ConfigOptions().getWaySublineMatcher() != "hoot::MaximalNearestSublineMatcher")
  {
    LOG_DEBUG("Temp fixing way.subline.matcher...");
    conf().set("way.subline.matcher", "hoot::MaximalNearestSublineMatcher");
  }
  LOG_VARD(ConfigOptions().getWaySublineMatcher());

  //fix highway classifier - https://github.com/ngageoint/hootenanny-ui/issues/971
  if (matchCreators.contains("hoot::NetworkMatchCreator") &&
      ConfigOptions().getConflateMatchHighwayClassifier() != "hoot::HighwayExpertClassifier")
  {
    LOG_DEBUG("Temp fixing conflate.match.highway.classifier...");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayExpertClassifier");
  }
  else if (matchCreators.contains("hoot::HighwayMatchCreator") &&
           ConfigOptions().getConflateMatchHighwayClassifier() != "hoot::HighwayRfClassifier")
  {
    LOG_DEBUG("Temp fixing conflate.match.highway.classifier...");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayRfClassifier");
  }
  LOG_VARD(ConfigOptions().getConflateMatchHighwayClassifier());

  //fix use of rubber sheeting and corner splitter - default value coming in from UI with network
  //will be correct, so just fix for unifying - https://github.com/ngageoint/hootenanny-ui/issues/969
  QStringList mapCleanerTransforms = ConfigOptions().getMapCleanerTransforms();
  if (matchCreators.contains("hoot::HighwayMatchCreator") &&
      (mapCleanerTransforms.contains("hoot::CornerSplitter") ||
       mapCleanerTransforms.contains("hoot::RubberSheet")))
  {
    if (mapCleanerTransforms.contains("hoot::CornerSplitter") &&
        !ConfigOptions().getHighwayMergeTagsOnly())
    {
      mapCleanerTransforms.removeAll("hoot::CornerSplitter");
    }
    if (mapCleanerTransforms.contains("hoot::RubberSheet"))
    {
      mapCleanerTransforms.removeAll("hoot::RubberSheet");
    }
    LOG_DEBUG("Temp fixing map.cleaner.transforms...");
    conf().set("map.cleaner.transforms", mapCleanerTransforms.join(";"));
  }
  LOG_VARD(ConfigOptions().getMapCleanerTransforms());
}

}
