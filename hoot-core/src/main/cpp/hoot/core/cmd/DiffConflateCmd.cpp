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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/stats/ConflateStatsHelper.h>
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/filters/BuildingCriterion.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/stats/IoSingleStat.h>
#include <hoot/core/visitors/AddRef1Visitor.h>
#include <hoot/core/visitors/CriterionCountVisitor.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/MapStatsWriter.h>
#include <hoot/core/io/ChangesetDeriver.h>
#include <hoot/core/io/ElementSorter.h>
#include <hoot/core/io/OsmXmlChangesetFileWriter.h>
#include <hoot/core/util/Log.h>

// Standard
#include <fstream>

// Tgs
#include <tgs/System/Timer.h>

#include <QFileInfo>

using namespace std;
using namespace Tgs;

namespace hoot
{

class DiffConflateCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DiffConflateCmd"; }

  DiffConflateCmd() { }

  virtual QString getName() const { return "conflate-differential"; }

  virtual QString getDescription() const
  { return "Conflates two maps into a single map based on the difference between the inputs"; }

  // Convenience function used when deriving a changeset
  boost::shared_ptr<ChangesetDeriver> _sortInputs(OsmMapPtr pMap1, OsmMapPtr pMap2)
  {
    ElementSorterPtr sorted1(new ElementSorter(pMap1));
    ElementSorterPtr sorted2(new ElementSorter(pMap2));
    boost::shared_ptr<ChangesetDeriver> delta(new ChangesetDeriver(sorted1, sorted2));
    return delta;
  }

  void writeChangeset(OsmMapPtr pMap, QString outFileName)
  {
    // Make empty map
    OsmMapPtr pEmptyMap(new OsmMap());

    // Get Changeset Deriver
    boost::shared_ptr<ChangesetDeriver> pDeriver = _sortInputs(pEmptyMap, pMap);

    // Write the file!
    OsmXmlChangesetFileWriter writer;
    writer.write(outFileName, pDeriver);
  }

  int runSimple(QStringList args)
  {
    Timer totalTime;
    Timer t;

    QList<SingleStat> stats;
    bool displayStats = false;
    QString outputStatsFile;
    if (args.contains("--stats"))
    {
      if (args.endsWith("--stats"))
      {
        displayStats = true;
        //remove "--stats" from args list
        args.pop_back();
      }
      else if (args.size() == 5)
      {
        displayStats = true;
        outputStatsFile = args[4];
        //remove "--stats" and stats output file name from args list
        args.pop_back();
        args.pop_back();
      }
    }

    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    QString input1 = args[0];
    QString input2 = args[1];
    QString output = args[2];

    LOG_INFO("Differentially conflating " << input1.right(50) << " with " << input2.right(50) <<
             " and writing output to " << output.right(50));

    double bytesRead = IoSingleStat(IoSingleStat::RChar).value;
    LOG_VART(bytesRead);
    QList< QList<SingleStat> > allStats;

    // read input 1 into our working map
    OsmMapPtr map(new OsmMap());
    loadMap(map, input1, ConfigOptions().getReaderConflateUseDataSourceIds1(), Status::Unknown1);

    // Mark input1 elements (Use Ref1 visitor, because it's already coded up)
    Settings visitorConf;
    visitorConf.set(ConfigOptions::getAddRefVisitorInformationOnlyKey(), "false");
    boost::shared_ptr<AddRef1Visitor> pRef1v(new AddRef1Visitor());
    pRef1v->setConfiguration(visitorConf);
    map->visitRw(*pRef1v);

    // read input 2 into our working map
    loadMap(map, input2, ConfigOptions().getReaderConflateUseDataSourceIds2(), Status::Unknown2);

    double inputBytes = IoSingleStat(IoSingleStat::RChar).value - bytesRead;
    LOG_VART(inputBytes);
    double elapsed = t.getElapsedAndRestart();
    stats.append(SingleStat("Read Inputs Time (sec)", elapsed));
    stats.append(SingleStat("(Dubious) Read Inputs Bytes", inputBytes));
    stats.append(SingleStat("(Dubious) Read Inputs Bytes per Second", inputBytes / elapsed));

    CalculateStatsOp input1Cso(ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
                               "input map 1");
    CalculateStatsOp input2Cso(ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
                               "input map 2");
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

    LOG_INFO("Applying pre-conflation operations...");
    NamedOp(ConfigOptions().getConflatePreOps()).apply(map);

    stats.append(SingleStat("Apply Named Ops Time (sec)", t.getElapsedAndRestart()));

    OsmMapPtr result = map;
    {
      // call the diff conflator
      DiffConflator conflator;
      conflator.apply(result);
      stats.append(conflator.getStats());
      stats.append(SingleStat("Conflation Time (sec)", t.getElapsedAndRestart()));
    }

    // Apply any user specified operations.
    LOG_INFO("Applying post-conflation operations...");
    NamedOp(ConfigOptions().getConflatePostOps()).apply(result);

    MapProjector::projectToWgs84(result);
    stats.append(SingleStat("Project to WGS84 Time (sec)", t.getElapsedAndRestart()));

    // If output ends with .osc, write out a changeset
    // Else write normal stuff
    if (output.endsWith(".osc"))
    {
      // Write changeset
      writeChangeset(result, output);
    }
    else
    {
      // Write conflated map
      saveMap(result, output);
    }



    // Do more stats
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

    // Differential specific stats - get some numbers for our output
    // Number of new points
    // Number of new buildings
    // km of new roads

    ElementCriterionPtr pPoiCrit(new PoiCriterion());
    CriterionCountVisitor poiCounter;
    poiCounter.addCriterion(pPoiCrit);
    result->visitRo(poiCounter);
    stats.append((SingleStat("Count of New POIs", poiCounter.getCount())));

    ElementCriterionPtr pBuildingCrit(new BuildingCriterion(result));
    CriterionCountVisitor buildingCounter;
    buildingCounter.addCriterion(pBuildingCrit);
    result->visitRo(buildingCounter);
    stats.append((SingleStat("Count of New Buildings", buildingCounter.getCount())));

    LengthOfWaysVisitor lengthVisitor;
    result->visitRo(lengthVisitor);
    stats.append((SingleStat("Km of New Road", lengthVisitor.getStat()/1000.0)));

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

    LOG_INFO("Differential Conflation job completed.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, DiffConflateCmd)

}

