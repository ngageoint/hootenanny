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
#include <hoot/core/info/IoSingleStat.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Progress.h>

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

  //if (args.size() < 2 || args.size() > 3)
  if (args.size() != 3)
  {
    cout << getHelp() << endl << endl;
    throw HootException(QString("%1 takes three parameters.").arg(getName()));
  }

  // TODO: check this; why would there ever not be a second input?
  QString input1 = args[0];
  QString input2, output;

  //if (args.size() == 3)
  //{
    input2 = args[1];
    output = args[2];
  //}
  //else
  //{
    //output = args[1];
  //}

  QString msg =
    "Conflating " + input1.right(40) + " with " + input2.right(40) + " and writing the output to " +
     output.right(40);
  if (isDiffConflate)
  {
    msg = msg.prepend("Differentially ");
  }
  LOG_INFO(msg);

  double bytesRead = IoSingleStat(IoSingleStat::RChar).value;
  LOG_VART(bytesRead);
  QList<QList<SingleStat>> allStats;

  // This is being made to work with non-Differential conflate only for now.
  Progress progress("Conflate");
  int numTotalTasks = 7;
  // The number of steps here must be updated as you add/remove conflation steps.
  //int numTotalTasks =
    //7 + ConfigOptions.getConflatePreOps().size() + ConfigOptions.getConflatePostOps().size();
  if (displayStats)
  {
    numTotalTasks += 3;
  }
  if (isDiffConflate)
  {
    numTotalTasks += 1;
  }
  int currentTask = 1;
  progress.setReportType("text");
  progress.setState("Running");

  // read input 1
  progress.set(
    (float)(currentTask - 1) / (float)numTotalTasks, "Running", false, "Loading reference data...");
  OsmMapPtr map(new OsmMap());
  IoUtils::loadMap(map, input1, ConfigOptions().getReaderConflateUseDataSourceIds1(),
                   Status::Unknown1);
  currentTask++;

  ChangesetProviderPtr pTagChanges;
  if (isDiffConflate)
  {
    // Store original IDs for tag diff
    progress.set(
      (float)(currentTask - 1) / (float)numTotalTasks, "Running", false,
      "Storing original features for tag differential...");
    diffConflator.storeOriginalMap(map);
    diffConflator.markInputElements(map);
    currentTask++;
  }

  // read input 2
  //if (!input2.isEmpty())
  //{
    progress.set(
      (float)(currentTask - 1) / (float)numTotalTasks, "Running", false, "Loading secondary data...");
    IoUtils::loadMap(
      map, input2, ConfigOptions().getReaderConflateUseDataSourceIds2(), Status::Unknown2);
    currentTask++;
  //}

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
    progress.set(
      (float)(currentTask - 1) / (float)numTotalTasks, "Running", false,
      "Calculating reference data statistics...");
    input1Cso.apply(map);
    allStats.append(input1Cso.getStats());
    stats.append(SingleStat("Time to Calculate Stats for Input 1 (sec)", t.getElapsedAndRestart()));
    currentTask++;

    //if (input2 != "")
    //{
      progress.set(
        (float)(currentTask - 1) / (float)numTotalTasks, "Running", false,
        "Calculating secondary data statistics...");
      input2Cso.apply(map);
      allStats.append(input2Cso.getStats());
      stats.append(SingleStat("Time to Calculate Stats for Input 2 (sec)",
        t.getElapsedAndRestart()));
      currentTask++;
    //}
  }

  size_t initialElementCount = map->getElementCount();
  stats.append(SingleStat("Initial Element Count", initialElementCount));
  LOG_INFO("Total elements read: " << StringUtils::formatLargeNumber(initialElementCount));
  OsmMapWriterFactory::writeDebugMap(map, "after-load");

  // apply any user specified pre-conflate operations
  Progress preOpsProgress("Conflate");
  preOpsProgress.setPercentComplete((float)(currentTask - 1) / (float)numTotalTasks);
  preOpsProgress.setTaskWeight(1.0 / (float)ConfigOptions().getConflatePreOps().size());
  preOpsProgress.setState("Running");
  NamedOp preOps(ConfigOptions().getConflatePreOps());
  preOps.setProgress(preOpsProgress);
  preOps.apply(map);

  //LOG_VARD(preOps.getPercentComplete());
  //progress.setPercentComplete(preOps.getPercentComplete());

  stats.append(SingleStat("Apply Pre-Conflate Ops Time (sec)", t.getElapsedAndRestart()));
  OsmMapWriterFactory::writeDebugMap(map, "after-pre-ops");
  currentTask++;

  OsmMapPtr result = map;

  progress.set(
    (float)(currentTask - 1) / (float)numTotalTasks, "Running", false, "Conflating data...");
  if (isDiffConflate)
  {
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
  currentTask++;

  // apply any user specified post-conflate operations
  _updatePostConfigOptionsForAttributeConflation();
  Progress postOpsProgress("Conflate");
  postOpsProgress.setPercentComplete((float)(currentTask - 1) / (float)numTotalTasks);
  postOpsProgress.setTaskWeight(1.0 / (float)ConfigOptions().getConflatePostOps().size());
  postOpsProgress.setState("Running");
  NamedOp postOps(ConfigOptions().getConflatePostOps());
  postOps.setProgress(postOpsProgress);
  postOps.apply(map);

  //LOG_VARD(postOps.getPercentComplete());
  //progress.setPercentComplete(postOps.getPercentComplete());

  stats.append(SingleStat("Apply Post-Conflate Ops Time (sec)", t.getElapsedAndRestart()));
  OsmMapWriterFactory::writeDebugMap(result, "after-post-ops");
  currentTask++;

  // doing this after the conflate post ops run, since some invalid reviews are removed by them
  progress.set(
    (float)(currentTask - 1) / (float)numTotalTasks, "Running", false,
    "Counting feature reviews...");
  CountUniqueReviewsVisitor countReviewsVis;
  result->visitRo(countReviewsVis);
  LOG_INFO("Generated " << countReviewsVis.getStat() << " feature reviews.");
  currentTask++;

  MapProjector::projectToWgs84(result);
  stats.append(SingleStat("Project to WGS84 Time (sec)", t.getElapsedAndRestart()));
  OsmMapWriterFactory::writeDebugMap(result, "after-wgs84-projection");

  // Figure out what to write
  progress.set(
    (float)(currentTask - 1) / (float)numTotalTasks, "Running", false, "Writing conflated output...");
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
      currentTask++;
    }
    IoUtils::saveMap(result, output);
    OsmMapWriterFactory::writeDebugMap(result, "after-conflate-output-write");
  }
  currentTask++;

  // Do the tags if we need to
  if (isDiffConflate && diffConflator.conflatingTags())
  {
    QString outFileName = output;
    outFileName.replace(".osm", "");
  }

  double timingOutput = t.getElapsedAndRestart();

  if (displayStats)
  {
    progress.set(
      (float)(currentTask - 1) / (float)numTotalTasks, "Running", false,
      "Calculating output data statistics...");
    CalculateStatsOp outputCso("output map", true);
    outputCso.apply(result);
    QList<SingleStat> outputStats = outputCso.getStats();
    //if (input2 != "")
    //{
      ConflateStatsHelper(input1Cso.getStats(), input2Cso.getStats(), outputCso.getStats())
        .updateStats(
          outputStats,
          outputCso.indexOfSingleStat("Total Unmatched Features"));
    //}
    allStats.append(outputStats);
    stats.append(SingleStat("Time to Calculate Stats for Output (sec)", t.getElapsedAndRestart()));
    currentTask++;
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
    progress.set(
      (float)(currentTask - 1) / (float)numTotalTasks, "Running", false,
      "Calculating differential output statistics...");
    diffConflator.calculateStats(result, stats);
    currentTask++;
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
  progress.set(1.0, "Successful", true, "Conflation job completed.");

  return 0;
}

void ConflateCmd::_updatePostConfigOptionsForAttributeConflation()
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
        ConfigOptions().getRemoveElementsVisitorElementCriteria().contains(
          "hoot::ReviewRelationCriterion") &&
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
      QStringList removeElementsCriteria =
        conf().get(ConfigOptions::getRemoveElementsVisitorElementCriteriaKey()).toStringList();
      removeElementsCriteria.replaceInStrings(
        "hoot::ReviewRelationCriterion", "hoot::ReviewScoreCriterion");
      conf().set(
        ConfigOptions::getRemoveElementsVisitorElementCriteriaKey(), removeElementsCriteria);
    }

    LOG_DEBUG(
      "Post conflate ops after Attribute Conflation adjustment: " <<
      conf().get("conflate.post.ops").toStringList());
  }
}

}
