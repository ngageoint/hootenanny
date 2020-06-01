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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/criterion/ReviewScoreCriterion.h>
#include <hoot/core/criterion/ReviewRelationCriterion.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CountUniqueReviewsVisitor.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/elements/VersionUtils.h>
#include <hoot/core/ops/RemoveRoundabouts.h>
#include <hoot/core/ops/ReplaceRoundabouts.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/io/ChangesetStatsFormat.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/conflate/SuperfluousConflateOpRemover.h>
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>

// Standard
#include <fstream>

// Tgs
#include <tgs/System/Timer.h>

// Qt
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>

using namespace std;
using namespace Tgs;

namespace hoot
{

const QString ConflateCmd::JOB_SOURCE = "Conflate";

HOOT_FACTORY_REGISTER(Command, ConflateCmd)

ConflateCmd::ConflateCmd() :
_numTotalTasks(0),
_filterOps(ConfigOptions().getConflateRemoveSuperfluousOps())
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

int ConflateCmd::runSimple(QStringList& args)
{
  Timer totalTime;
  Timer t;

  BoundedCommand::runSimple(args);

  // This parsing is for map stats. Changeset stats args for differential are processed further down
  // below.
  QList<SingleStat> stats;
  bool displayStats = false;
  QString outputStatsFile;
  if (args.contains("--stats"))
  {
    displayStats = true;
    const int statsIndex = args.indexOf("--stats");
    if (statsIndex != -1 && statsIndex != (args.size() - 1) &&
        args[statsIndex + 1].toLower().endsWith(".json"))
    {
      outputStatsFile = args[statsIndex + 1];
      args.removeAll(outputStatsFile);
    }
    args.removeAll("--stats");
  }
  LOG_VARD(displayStats);
  LOG_VARD(outputStatsFile);

  ConfigUtils::checkForTagValueTruncationOverride();
  QStringList allOps = ConfigOptions().getConflatePreOps();
  allOps += ConfigOptions().getConflatePostOps();
  ConfigUtils::checkForDuplicateElementCorrectionMismatch(allOps);

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
  LOG_VARD(diffConflator.conflatingTags());

  // Check for separate output files (for geometry & tags)
  bool separateOutput = false;
  if (args.contains("--separate-output"))
  {
    const QString errorMsg =
      QString("--separate-output is only valid when combiend with the --differential and ") +
      QString("--include-tags options.");
    if (!isDiffConflate || !diffConflator.conflatingTags())
    {
      throw IllegalArgumentException(errorMsg);
    }
    separateOutput = true;
    args.removeAt(args.indexOf("--separate-output"));
  }
  LOG_VARD(separateOutput);

  bool displayChangesetStats = false;
  QString outputChangesetStatsFile;
  if (args.contains("--changeset-stats"))
  {
    if (!isDiffConflate)
    {
      throw IllegalArgumentException(
        "--changeset-stats is only valid when combined with the --differential option.");
    }
    else
    {
      displayChangesetStats = true;
      const int statsIndex = args.indexOf("--changeset-stats");
      LOG_VARD(statsIndex);
      // If the input immediately after the changeset stats arg isn't a valid changeset stats file
      // output format, we'll just silently skip it and assume we're outputting stats to the display
      // only. This mimics how the map stats args and stats args in other commands are parsed. We
      // may want to eventually return an error or warning here instead.
      if (statsIndex != -1 && statsIndex != (args.size() - 1) &&
          !args[statsIndex + 1].startsWith("--"))
      {
        outputChangesetStatsFile = args[statsIndex + 1];
        QFileInfo changesetStatsInfo(outputChangesetStatsFile);
        if (!ChangesetStatsFormat::isValidFileOutputFormat(changesetStatsInfo.completeSuffix()))
        {
          outputChangesetStatsFile = "";
        }
        else
        {
          args.removeAll(outputChangesetStatsFile);
        }
      }
      args.removeAll("--changeset-stats");
    }
  }
  LOG_VARD(displayChangesetStats);
  LOG_VARD(outputChangesetStatsFile);

  LOG_VARD(args.size() );
  if (args.size() < 3 || args.size() > 4)
  {
    cout << getHelp() << endl << endl;
    throw IllegalArgumentException(
      QString("%1 takes three or four parameters. You provided %2: %3")
        .arg(getName())
        .arg(args.size())
        .arg(args.join(",")));
  }

  const QString input1 = args[0];
  const QString input2 = args[1];
  QString output = args[2];

  if (!IoUtils::isUrl(output))
  {
    // write the output dir now so we don't get a nasty surprise at the end of a long job that it
    // can't be written
    IoUtils::writeOutputDir(output);
  }

  QString osmApiDbUrl;
  if (output.endsWith(".osc.sql"))
  {
    if (args.size() != 4)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 with SQL changeset output takes four parameters.").arg(getName()));
    }
    else if (displayChangesetStats)
    {
      throw IllegalArgumentException(
        QString("Changeset statistics (--changeset-stats) may only be calculated with an XML ") +
        QString("changeset output (.osc)."));
    }
    osmApiDbUrl = args[3];
  }
  else if (args.size() > 3)
  {
    std::cout << getHelp() << std::endl << std::endl;
    throw IllegalArgumentException(
      QString("%1 with output: " + output + " takes three parameters. You provided %2: %3")
        .arg(getName())
        .arg(args.size())
        .arg(args.join(",")));
  }

  Progress progress(ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running);
  const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
  QString msg =
    "Conflating " + input1.right(maxFilePrintLength) + " with " +
    input2.right(maxFilePrintLength) + " and writing the output to " +
    output.right(maxFilePrintLength);
  if (isDiffConflate)
  {
    if (diffConflator.conflatingTags())
    {
      msg = msg.replace("Conflating", "Differentially conflating (tags only) ");
    }
    else
    {
      msg = msg.replace("Conflating", "Differentially conflating ");
    }
  }

  progress.set(0.0, msg);

  double bytesRead = IoSingleStat(IoSingleStat::RChar).value;
  LOG_VART(bytesRead);
  QList<QList<SingleStat>> allStats;

  // The highway.merge.tags.only option only gets used with Attribute Conflation for now, so we'll
  // use it as the sole identifier for it. If that ever changes, then we'll need a different way
  // to recognize when AC is occurring.
  const bool isAttributeConflate = ConfigOptions().getHighwayMergeTagsOnly();
  if (isAttributeConflate)
  {
    _updateConfigOptionsForAttributeConflation();
  }
  if (isDiffConflate || isAttributeConflate)
  {
    _disableRoundaboutRemoval();
  }

  if (_filterOps)
  {
    // Let's see if we can remove any ops in the configuration that will have no effect on the
    // feature types we're conflating in order to improve runtime performance.
    SuperfluousConflateOpRemover::removeSuperfluousOps();
  }

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

  // Only add one task for each set of conflate ops, since NamedOp will create its own task step for
  // each op internally.
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
  if (!ConfigOptions().getConflateUseDataSourceIds1() &&
       ConfigOptions().getConflateUseDataSourceIds2() &&
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
      map, input2, ConfigOptions().getConflateUseDataSourceIds2(), Status::Unknown2);
    currentTask++;

    // read input 1
    progress.set(
      _getJobPercentComplete(currentTask - 1),
      "Loading reference map: ..." + input1.right(maxFilePrintLength) + "...");
    IoUtils::loadMap(map, input1, ConfigOptions().getConflateUseDataSourceIds1(),
                     Status::Unknown1);
    currentTask++;
  }
  else
  {
    // read input 1
    progress.set(
      _getJobPercentComplete(currentTask - 1),
      "Loading reference map: ..." + input1.right(maxFilePrintLength) + "...");
    IoUtils::loadMap(map, input1, ConfigOptions().getConflateUseDataSourceIds1(), Status::Unknown1);
    currentTask++;

    if (isDiffConflate)
    {
      if (output.endsWith(".osc") || output.endsWith(".osc.sql"))
      {
        VersionUtils::checkVersionLessThanOneCountAndLogWarning(map);
      }

      // Store original IDs for tag diff
      progress.set(
        _getJobPercentComplete(currentTask - 1),
       "Storing original features for tag differential...");
      diffConflator.storeOriginalMap(map);
      diffConflator.markInputElements(map);
      currentTask++;
    }

    progress.set(
      _getJobPercentComplete(currentTask - 1),
      "Loading secondary map: ..." + input2.right(maxFilePrintLength) + "...");
    IoUtils::loadMap(
      map, input2, ConfigOptions().getConflateUseDataSourceIds2(), Status::Unknown2);
    currentTask++;
  }
  MemoryUsageChecker::getInstance().check();
  LOG_STATUS(
    "Conflating map with " << StringUtils::formatLargeNumber(map->size()) << " elements...");

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
  MemoryUsageChecker::getInstance().check();

  size_t initialElementCount = map->getElementCount();
  stats.append(SingleStat("Initial Element Count", initialElementCount));
  OsmMapWriterFactory::writeDebugMap(map, "after-load");

  if (ConfigOptions().getConflatePreOps().size() > 0)
  {
    // By default rubbersheeting has no filters. When conflating, we need to add some.
    conf().set(
      ConfigOptions::getRubberSheetElementCriteriaKey(),
      ConfigOptions().getConflateRubberSheetElementCriteria());

    // apply any user specified pre-conflate operations
    LOG_STATUS("Running pre-conflate operations...");
    QElapsedTimer timer;
    timer.start();
    NamedOp preOps(ConfigOptions().getConflatePreOps());
    preOps.setProgress(
      Progress(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        _getJobPercentComplete(currentTask - 1), _getTaskWeight()));
    preOps.apply(map);
    stats.append(SingleStat("Apply Pre-Conflate Ops Time (sec)", t.getElapsedAndRestart()));
    OsmMapWriterFactory::writeDebugMap(map, "after-pre-ops");
    currentTask++;
    LOG_STATUS(
      "Conflate pre-operations ran in " + StringUtils::millisecondsToDhms(timer.elapsed()) <<
      " total.");
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

  if (ConfigOptions().getConflatePostOps().size() > 0)
  {
    // apply any user specified post-conflate operations
    LOG_STATUS("Running post-conflate operations...");
    QElapsedTimer timer;
    timer.start();
    NamedOp postOps(ConfigOptions().getConflatePostOps());
    postOps.setProgress(
      Progress(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        _getJobPercentComplete(currentTask - 1), _getTaskWeight()));
    postOps.apply(map);
    stats.append(SingleStat("Apply Post-Conflate Ops Time (sec)", t.getElapsedAndRestart()));
    OsmMapWriterFactory::writeDebugMap(result, "after-post-ops");
    currentTask++;
    LOG_STATUS(
      "Conflate post-operations ran in " + StringUtils::millisecondsToDhms(timer.elapsed()) <<
      " total.");
  }

  // doing this after the conflate post ops run, since some invalid reviews are removed by them
  progress.set(_getJobPercentComplete(currentTask - 1), "Counting feature reviews...");
  CountUniqueReviewsVisitor countReviewsVis;
  result->visitRo(countReviewsVis);
  LOG_INFO(
    "Generated " << StringUtils::formatLargeNumber(countReviewsVis.getStat()) <<
    " feature reviews.");
  currentTask++;

  MapProjector::projectToWgs84(result);
  stats.append(SingleStat("Project to WGS84 Time (sec)", t.getElapsedAndRestart()));
  OsmMapWriterFactory::writeDebugMap(result, "after-wgs84-projection");

  // Figure out what to write
  progress.set(
    _getJobPercentComplete(currentTask - 1),
    "Writing conflated output: ..." + output.right(maxFilePrintLength) + "...");
  if (isDiffConflate && (output.endsWith(".osc") || output.endsWith(".osc.sql")))
  {
    // Get the changeset stats output format from the changeset stats file extension, or if no
    // extension is there assume a text table output to the display.
    ChangesetStatsFormat statsFormat;
    if (displayChangesetStats)
    {
      if (!outputChangesetStatsFile.isEmpty())
      {
        QFileInfo changesetStatsFileInfo(outputChangesetStatsFile);
        statsFormat.setFormat(
          ChangesetStatsFormat::fromString(changesetStatsFileInfo.completeSuffix()));
      }
      else
      {
        statsFormat.setFormat(ChangesetStatsFormat::Text);
      }
    }
    diffConflator.writeChangeset(result, output, separateOutput, statsFormat, osmApiDbUrl);
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

  if (isDiffConflate && displayStats)
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
    allStats.append(stats);
    if (outputStatsFile.isEmpty())
    {
      QString statsMsg = MapStatsWriter().statsToString(allStats, "\t");
      cout << "stats = (stat) OR (input map 1 stat) (input map 2 stat) (output map stat)\n" <<
              statsMsg << endl;
    }
    else
    {
      MapStatsWriter().writeStatsToJson(allStats, outputStatsFile);
      cout << "stats = (stat) OR (input map 1 stat) (input map 2 stat) (output map stat) in file: " <<
              outputStatsFile << endl;
    }
  }

  if (displayChangesetStats)
  {
    if (outputChangesetStatsFile.isEmpty())
    {
      // output to display
      LOG_STATUS("Changeset Geometry Stats:\n" << diffConflator.getGeometryChangesetStats());
      if (diffConflator.conflatingTags())
      {
        LOG_STATUS("\nChangeset Tag Stats:\n" << diffConflator.getTagChangesetStats() << "\n");
      }
    }
    else
    {
      // output to file
      if (separateOutput)
      {
        // output separate files for geometry and tag change stats
        FileUtils::writeFully(outputChangesetStatsFile, diffConflator.getGeometryChangesetStats());
        if (diffConflator.conflatingTags())
        {
          QString tagsOutFile = outputChangesetStatsFile.replace(".json", "");
          tagsOutFile.append(".tags.json");
          FileUtils::writeFully(tagsOutFile, diffConflator.getTagChangesetStats());
        }
      }
      else
      {
        // output a single stats file with both geometry and tags change stats
        FileUtils::writeFully(outputChangesetStatsFile, diffConflator.getUnifiedChangesetStats());
      }
    }
  }

  progress.set(
    1.0, Progress::JobState::Successful,
    "Conflation job completed in " +
    StringUtils::millisecondsToDhms((qint64)(totalElapsed * 1000)) + " for reference map: ..." +
    input1.right(maxFilePrintLength) + " and secondary map: ..." +
    input2.right(maxFilePrintLength) + " and written to output: ..." +
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

void ConflateCmd::_disableRoundaboutRemoval()
{
  // This applies to both Attribute and Differential Conflation.

  // Since Differential throws out all matches, there's no way we can have a bad merge between
  // ref/secondary roundabouts. Therefore, no need to replace/remove them. If there's a match, we'll
  // end up with no secondary roundabout in the diff output and only the ref roundabout when the
  // diff is applied back to the ref.

  // Our roundabout handling used with other types of conflation makes no sense for Attribute
  // Conflation. For the other types of conflation we remove roundabouts b/c hoot isn't very good
  // at merging them together, so we keep the ref roundabouts and skip conflating them altogether.
  // Since we always keep the ref geometry in AC, there's no opportunity for bad merging and,
  // thus, no need to remove them in the first place. So, we'll override that behavior here.

  // If the work for #3442 is done, then we could handle this removal in AttributeConflation.conf
  // add DifferentialConflation.conf instead of doing it here.

  QStringList preConflateOps = ConfigOptions().getConflatePreOps();
  const QString removeRoundaboutsClassName = QString::fromStdString(RemoveRoundabouts::className());
  if (preConflateOps.contains(removeRoundaboutsClassName))
  {
    preConflateOps.removeAll(removeRoundaboutsClassName);
    conf().set(ConfigOptions::getConflatePreOpsKey(), preConflateOps);
  }

  QStringList postConflateOps = ConfigOptions().getConflatePostOps();
  const QString replaceRoundaboutsClassName =
    QString::fromStdString(ReplaceRoundabouts::className());
  if (postConflateOps.contains(replaceRoundaboutsClassName))
  {
    postConflateOps.removeAll(replaceRoundaboutsClassName);
    conf().set(ConfigOptions::getConflatePostOpsKey(), postConflateOps);
  }
}

void ConflateCmd::_updateConfigOptionsForAttributeConflation()
{
  // These are some custom adjustments to config opts that must be done for Attribute Conflation.
  // There may be a way to eliminate these by adding more custom behavior to the UI.

  const QString reviewRelationCritName =
    QString::fromStdString(ReviewRelationCriterion::className());

  // This swaps the logic that removes all reviews with the logic that removes them based on score
  // thresholding.
  if (ConfigOptions().getAttributeConflationAllowReviewsByScore())
  {
    QStringList removeElementsCriteria =
      conf().get(ConfigOptions::getRemoveElementsVisitorElementCriteriaKey()).toStringList();
    removeElementsCriteria.replaceInStrings(
      reviewRelationCritName, QString::fromStdString(ReviewScoreCriterion::className()));
    conf().set(
      ConfigOptions::getRemoveElementsVisitorElementCriteriaKey(), removeElementsCriteria);
  }
}

}
