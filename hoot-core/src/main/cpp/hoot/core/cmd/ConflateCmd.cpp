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
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/conflate/stats/ConflateStatsHelper.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/info/IoSingleStat.h>
#include <hoot/core/io/MapStatsWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>

// Standard
#include <fstream>

// Tgs
#include <tgs/System/Timer.h>

#include <QFileInfo>

using namespace std;
using namespace Tgs;

namespace hoot
{

const QString ConflateCmd::JOB_SOURCE = "Conflate";

HOOT_FACTORY_REGISTER(Command, ConflateCmd)

ConflateCmd::ConflateCmd() :
_numTotalTasks(0)
{
}

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

  if (args.size() != 3)
  {
    cout << getHelp() << endl << endl;
    throw HootException(QString("%1 takes three parameters.").arg(getName()));
  }

  const QString input1 = args[0];
  const QString input2 = args[1];
  QString output = args[2];

  Progress progress(ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running);
  const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
  QString msg =
    "Conflating ..." + input1.right(maxFilePrintLength) + " with ..." +
    input2.right(maxFilePrintLength) + " and writing the output to ..." +
    output.right(maxFilePrintLength);
  if (isDiffConflate)
  {
    msg = msg.prepend("Differentially ");
  }
  progress.set(0.0, msg);

  double bytesRead = IoSingleStat(IoSingleStat::RChar).value;
  LOG_VART(bytesRead);
  QList<QList<SingleStat>> allStats;

  // The number of steps here must be updated as you add/remove job steps in the logic.
  _numTotalTasks = 5;
  if (displayStats)
  {
    _numTotalTasks += 3;
  }
  if (isDiffConflate)
  {
    _numTotalTasks++;
  }
  if (ConfigOptions().getConflatePreOps().size() > 0)
  {
    _numTotalTasks++;
  }
  if (ConfigOptions().getConflatePostOps().size() > 0)
  {
    _numTotalTasks++;
  }
  int currentTask = 1;

  OsmMapPtr map(new OsmMap());
  ChangesetProviderPtr pTagChanges;

  //  Loading order is important if datasource IDs 2 is true but 1 is not
  if (!ConfigOptions().getReaderConflateUseDataSourceIds1() &&
       ConfigOptions().getReaderConflateUseDataSourceIds2() &&
      !isDiffConflate)
  {
    //  For Attribute conflation, the secondary IDs are the ones that we want
    //  to preserve.  So loading them first allows for all of them to be loaded
    //  without conflict, even if they are negative.  Then the reference dataset
    //  is loaded with negative IDs
    progress.set(
      _getJobPercentComplete(currentTask - 1),
      "Loading secondary map: ..." + input2.right(maxFilePrintLength) + "...");
    IoUtils::loadMap(
      map, input2, ConfigOptions().getReaderConflateUseDataSourceIds2(), Status::Unknown2);
    currentTask++;

    // read input 1
    progress.set(
      _getJobPercentComplete(currentTask - 1),
      "Loading reference map: ..." + input1.right(maxFilePrintLength) + "...");
    IoUtils::loadMap(map, input1, ConfigOptions().getReaderConflateUseDataSourceIds1(),
                     Status::Unknown1);
    currentTask++;
  }
  else
  {
    // read input 1
    progress.set(
      _getJobPercentComplete(currentTask - 1),
      "Loading reference map: ..." + input1.right(maxFilePrintLength) + "...");
    IoUtils::loadMap(map, input1, ConfigOptions().getReaderConflateUseDataSourceIds1(),
                     Status::Unknown1);
    currentTask++;

    if (isDiffConflate)
    {
      // Store original IDs for tag diff
      progress.set(
        _getJobPercentComplete(currentTask - 1), "Storing original features for tag differential...");
      diffConflator.storeOriginalMap(map);
      diffConflator.markInputElements(map);
      currentTask++;
    }

    progress.set(
      _getJobPercentComplete(currentTask - 1),
      "Loading secondary map: ..." + input2.right(maxFilePrintLength) + "...");
    IoUtils::loadMap(
      map, input2, ConfigOptions().getReaderConflateUseDataSourceIds2(), Status::Unknown2);
    currentTask++;
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
    progress.set(
      _getJobPercentComplete(currentTask - 1),
      "Calculating reference statistics for: ..." + input1.right(maxFilePrintLength) + "...");
    input1Cso.apply(map);
    allStats.append(input1Cso.getStats());
    stats.append(SingleStat("Calculate Stats for Input 1 Time (sec)", t.getElapsedAndRestart()));
    currentTask++;

    progress.set(
      _getJobPercentComplete(currentTask - 1),
      "Calculating secondary data statistics for: ..." + input2.right(maxFilePrintLength) + "...");
    input2Cso.apply(map);
    allStats.append(input2Cso.getStats());
    stats.append(SingleStat("Calculate Stats for Input 2 Time (sec)", t.getElapsedAndRestart()));
    currentTask++;
  }

  size_t initialElementCount = map->getElementCount();
  stats.append(SingleStat("Initial Element Count", initialElementCount));
  LOG_INFO("Total elements read: " << StringUtils::formatLargeNumber(initialElementCount));
  OsmMapWriterFactory::writeDebugMap(map, "after-load");

  if (ConfigOptions().getConflatePreOps().size() > 0)
  {
    // apply any user specified pre-conflate operations
    NamedOp preOps(ConfigOptions().getConflatePreOps());
    preOps.setProgress(
      Progress(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        _getJobPercentComplete(currentTask - 1), _getTaskWeight()));
    preOps.apply(map);
    stats.append(SingleStat("Apply Pre-Conflate Ops Time (sec)", t.getElapsedAndRestart()));
    OsmMapWriterFactory::writeDebugMap(map, "after-pre-ops");
    currentTask++;
  }

  OsmMapPtr result = map;

  if (isDiffConflate)
  {
    diffConflator.setProgress(
      Progress(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        _getJobPercentComplete(currentTask - 1), _getTaskWeight()));
    diffConflator.apply(result);
    if (diffConflator.conflatingTags())
    {
      pTagChanges = diffConflator.getTagDiff();
    }
    stats.append(diffConflator.getStats());
  }
  else
  {
    UnifyingConflator conflator;
    conflator.setProgress(
      Progress(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        _getJobPercentComplete(currentTask - 1), _getTaskWeight()));
    conflator.apply(result);
    stats.append(conflator.getStats());
  }
  stats.append(SingleStat("Conflation Time (sec)", t.getElapsedAndRestart()));
  currentTask++;

  _updatePostConfigOptionsForAttributeConflation();
  if (ConfigOptions().getConflatePostOps().size() > 0)
  {
    // apply any user specified post-conflate operations
    NamedOp postOps(ConfigOptions().getConflatePostOps());
    postOps.setProgress(
      Progress(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        _getJobPercentComplete(currentTask - 1), _getTaskWeight()));
    postOps.apply(map);
    stats.append(SingleStat("Apply Post-Conflate Ops Time (sec)", t.getElapsedAndRestart()));
    OsmMapWriterFactory::writeDebugMap(result, "after-post-ops");
    currentTask++;
  }

  // doing this after the conflate post ops run, since some invalid reviews are removed by them
  progress.set(_getJobPercentComplete(currentTask - 1), "Counting feature reviews...");
  CountUniqueReviewsVisitor countReviewsVis;
  result->visitRo(countReviewsVis);
  LOG_INFO("Generated " << countReviewsVis.getStat() << " feature reviews.");
  currentTask++;

  MapProjector::projectToWgs84(result);
  stats.append(SingleStat("Project to WGS84 Time (sec)", t.getElapsedAndRestart()));
  OsmMapWriterFactory::writeDebugMap(result, "after-wgs84-projection");

  // Figure out what to write
  progress.set(
    _getJobPercentComplete(currentTask - 1),
    "Writing conflated output: ..." + output.right(maxFilePrintLength) + "...");
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
      _getJobPercentComplete(currentTask - 1),
      "Calculating output data statistics for: ..." + output.right(maxFilePrintLength) + "...");
    CalculateStatsOp outputCso("output map", true);
    outputCso.apply(result);
    QList<SingleStat> outputStats = outputCso.getStats();
    ConflateStatsHelper(input1Cso.getStats(), input2Cso.getStats(), outputCso.getStats())
      .updateStats(
        outputStats,
        outputCso.indexOfSingleStat("Total Unmatched Features"));
    allStats.append(outputStats);
    stats.append(SingleStat("Calculate Stats for Output Time (sec)", t.getElapsedAndRestart()));
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
      _getJobPercentComplete(currentTask - 1),
      "Calculating differential output statistics for: ..." + output.right(maxFilePrintLength) +
      "...");
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

  progress.set(
    1.0, Progress::JobState::Successful,
    "Conflation job completed for reference: ..." + input1.right(maxFilePrintLength) +
    " and secondary: ..." + input2.right(maxFilePrintLength) + " and written to output: ..." +
    output.right(maxFilePrintLength));

  return 0;
}

float ConflateCmd::_getTaskWeight() const
{
  return 1.0 / (float)_numTotalTasks;
}

float ConflateCmd::_getJobPercentComplete(const int currentTaskNum) const
{
  return (float)currentTaskNum / (float)_numTotalTasks;
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
