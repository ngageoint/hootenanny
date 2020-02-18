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
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/ops/RemoveRoundabouts.h>
#include <hoot/core/ops/ReplaceRoundabouts.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/ops/MapCleaner.h>


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
    else if (args[args.size() - 2] == "--stats")
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

  ConfigUtils::checkForTagValueTruncationOverride();

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

  QFileInfo outputInfo(output);
  LOG_VARD(outputInfo.dir().absolutePath());
  const bool outputDirSuccess = QDir().mkpath(outputInfo.dir().absolutePath());
  if (!outputDirSuccess)
  {
    throw IllegalArgumentException("Unable to create output path for: " + output);
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
    osmApiDbUrl = args[3];
  }
  else if (args.size() > 3)
  {
    std::cout << getHelp() << std::endl << std::endl;
    throw IllegalArgumentException(
      QString("%1 with output: " + output + " takes three parameters.").arg(getName()));
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
  if (ConfigOptions().getHighwayMergeTagsOnly())
  {
    _updateConfigOptionsForAttributeConflation();
  }
  if (isDiffConflate || ConfigOptions().getHighwayMergeTagsOnly())
  {
    _disableRoundaboutRemoval();
  }
  if (_filterOps)
  {
    // Let's see if we can remove any ops in the configuration that will have no effect on the
    // feature types we're conflating in order to improve runtime performance.
    _removeSuperfluousOps();
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
        OsmUtils::checkVersionLessThanOneCountAndLogWarning(map);
      }

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
      map, input2, ConfigOptions().getConflateUseDataSourceIds2(), Status::Unknown2);
    currentTask++;
  }
  LOG_STATUS("Conflating map with " << StringUtils::formatLargeNumber(map->size()) << " elements...");

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
  OsmMapWriterFactory::writeDebugMap(map, "after-load");

  if (ConfigOptions().getConflatePreOps().size() > 0)
  {
    // apply any user specified pre-conflate operations
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
    diffConflator.writeChangeset(result, output, separateOutput, osmApiDbUrl);
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

void ConflateCmd::_removeSuperfluousOps()
{
  // get all crits involved in the current matcher configuration
  const QSet<QString> matcherCrits = _getMatchCreatorCrits();

  QSet<QString> removedOps;

  // for each of the conflate pre/post and map cleaner transforms (if conflate pre/post specifies
  // MapCleaner) filter out any that aren't associated with the same ElementCriterion as the ones
  // associated with the matchers

  const QStringList modifiedPreConflateOps =
    _filterOutUnneededOps(
      matcherCrits, ConfigOptions().getConflatePreOps(), removedOps);
  if (modifiedPreConflateOps.size() != ConfigOptions().getConflatePreOps().size())
  {
    conf().set(ConfigOptions::getConflatePreOpsKey(), modifiedPreConflateOps);
  }

  const QStringList modifiedPostConflateOps =
    _filterOutUnneededOps(
      matcherCrits, ConfigOptions().getConflatePostOps(), removedOps);
  if (modifiedPostConflateOps.size() != ConfigOptions().getConflatePostOps().size())
  {
    conf().set(ConfigOptions::getConflatePostOpsKey(), modifiedPostConflateOps);
  }

  const QString mapCleanerName = QString::fromStdString(MapCleaner::className());
  if (modifiedPreConflateOps.contains(mapCleanerName) ||
      modifiedPostConflateOps.contains(mapCleanerName))
  {
    const QStringList modifiedCleaningOps =
      _filterOutUnneededOps(
        matcherCrits, ConfigOptions().getMapCleanerTransforms(), removedOps);
    if (modifiedCleaningOps.size() != ConfigOptions().getMapCleanerTransforms().size())
    {
      conf().set(ConfigOptions::getMapCleanerTransformsKey(), modifiedCleaningOps);
    }
  }

  if (removedOps.size() > 0)
  {
    QStringList removedOpsList = removedOps.values();
    qSort(removedOpsList);
    LOG_INFO(
      "Removed the following conflate pre/post operations with no relevance to the selected " <<
      "matchers: " << removedOpsList.join(", "));
  }
}

QStringList ConflateCmd::_filterOutUnneededOps(
  const QSet<QString>& matcherCrits, const QStringList& ops, QSet<QString>& removedOps)
{
  LOG_TRACE("ops before: " << ops);

  QStringList modifiedOps;

  for (int i = 0; i < ops.size(); i++)
  {
    const QString opName = ops.at(i);
    LOG_VART(opName);

    // MapCleaner's ops are configured with map.cleaner.transforms, so don't exclude it here.
    if (opName == QString::fromStdString(MapCleaner::className()))
    {
      modifiedOps.append(opName);
      continue;
    }

    // All the ops should be map ops or element vis and, thus, support FilteredByCriteria, but
    // we'll check anyway to be safe.
    std::shared_ptr<FilteredByCriteria> op;
    if (Factory::getInstance().hasBase<OsmMapOperation>(opName.toStdString()))
    {
      op =
        std::dynamic_pointer_cast<FilteredByCriteria>(
          std::shared_ptr<OsmMapOperation>(
            Factory::getInstance().constructObject<OsmMapOperation>(opName)));
    }
    else if (Factory::getInstance().hasBase<ElementVisitor>(opName.toStdString()))
    {
      op =
        std::dynamic_pointer_cast<FilteredByCriteria>(
          std::shared_ptr<ElementVisitor>(
            Factory::getInstance().constructObject<ElementVisitor>(opName)));
    }

    if (op)
    {
      // get all the class names of the crits that this op is associated with
      const QStringList opCrits = op->getCriteria();
      LOG_VART(opCrits);

      // If the op is not associated with any crit, we assume it should never be disabled based on
      // the feature type being conflated.
      if (opCrits.isEmpty())
      {
        modifiedOps.append(opName);
        continue;
      }

      // If any of the op's crits match with those in the matchers' list, we'll use it. Otherwise,
      // we disable it.
      bool opAdded = false;
      for (int j = 0; j < opCrits.size(); j++)
      {
        const QString opCrit = opCrits.at(j);
        if (matcherCrits.contains(opCrit))
        {
          modifiedOps.append(opName);
          opAdded = true;
          break;
        }
      }
      if (!opAdded)
      {
        removedOps.insert(opName);
      }
    }
    else
    {
      removedOps.insert(opName);
    }
  }

  LOG_TRACE("ops after: " << modifiedOps);
  LOG_VART(removedOps);
  return modifiedOps;
}

QSet<QString> ConflateCmd::_getMatchCreatorCrits()
{
  QSet<QString> matcherCrits;

  // get all of the matchers from our current config
  std::vector<std::shared_ptr<MatchCreator>> matchCreators =
      MatchFactory::getInstance().getCreators();
  for (std::vector<std::shared_ptr<MatchCreator>>::const_iterator it = matchCreators.begin();
       it != matchCreators.end(); ++it)
  {
    std::shared_ptr<MatchCreator> matchCreator = *it;
    std::shared_ptr<FilteredByCriteria> critFilter =
      std::dynamic_pointer_cast<FilteredByCriteria>(matchCreator);
    const QStringList crits = critFilter->getCriteria();

    // Technically, not sure we'd have to error out here, but it will be good to know if any
    // matchers weren't configured with crits to keep conflate bugs from sneaking in over time.
    if (crits.size() == 0)
    {
      throw HootException(
        "Match creator: " + matchCreator->getName() +
        " does not specify any associated feature type criteria.");
    }

    for (int i = 0; i < crits.size(); i++)
    {
      const QString critStr = crits.at(i);
      LOG_VART(critStr);
      // doublecheck this is a valid crit
      if (Factory::getInstance().hasBase<ElementCriterion>(critStr.toStdString()))
      {
        // add the crit
        matcherCrits.insert(critStr);

        // also add any generic geometry crits the crit inherits from

        const QStringList pointCrits =
          GeometryTypeCriterion::getCriterionClassNamesByType(
            GeometryTypeCriterion::GeometryType::Point);
        LOG_VART(pointCrits);
        if (pointCrits.contains(critStr))
        {
          matcherCrits.insert(QString::fromStdString(PointCriterion::className()));
        }

        const QStringList lineCrits =
          GeometryTypeCriterion::getCriterionClassNamesByType(
            GeometryTypeCriterion::GeometryType::Line);
        LOG_VART(lineCrits);
        if (lineCrits.contains(critStr))
        {
          matcherCrits.insert(QString::fromStdString(LinearCriterion::className()));
        }

        const QStringList polyCrits =
          GeometryTypeCriterion::getCriterionClassNamesByType(
            GeometryTypeCriterion::GeometryType::Polygon);
        LOG_VART(polyCrits);
        if (polyCrits.contains(critStr))
        {
          matcherCrits.insert(QString::fromStdString(PolygonCriterion::className()));
        }
      }
    }
  }

  LOG_VART(matcherCrits);
  return matcherCrits;
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
