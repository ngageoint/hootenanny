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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CONFLATECMD_H
#define CONFLATECMD_H

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/Conflator.h>
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/ConflateStatsHelper.h>
#include <hoot/core/conflate/StatsComposer.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/io/MapStatsWriter.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/stats/IoSingleStat.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/visitors/SetTagVisitor.h>

// Standard
#include <fstream>

// Tgs
#include <tgs/System/Timer.h>

#include <QFileInfo>

namespace hoot
{
using namespace std;

class ConflateCmd : public BaseCommand
{

public:

  static string className() { return "hoot::ConflateCmd"; }

  ConflateCmd() {}

  virtual QString getName() const { return "conflate"; }

  void printStats(const QList<SingleStat>& stats)
  {
    QString sep = "\t";
    cout << "== Stats ==" << endl;
    for (int i = 0; i < stats.size(); i++)
    {
      cout << stats[i].name << sep << stats[i].value << endl;
    }
  }

  //The logic in this method won't allow ConflateCmd to have any inputs that have ';' in the file
  //name.
  int runMultiple(QStringList args)
  {
    const QStringList inputs = args[0].split(";");
    const QString output = args[1];

    OsmMapPtr map(new OsmMap());
    const QString sourceTagKey = "hoot:source";
    LOG_VARD(inputs.size());
    for (int i = 0; i < inputs.size(); i++)
    {
      if (i == 0)
      {
        loadMap(map, inputs[i], ConfigOptions().getConflateUseDataSourceIds(), Status::Unknown1);
        SetTagVisitor sourceTagVisitor(sourceTagKey, QString::number(i + 1));
        map->visitRw(sourceTagVisitor);
      }
      else
      {
        if (i == 1)
        {
          LOG_INFO("Conflating " << inputs[i - 1] << " with " << inputs[i] << "...");
        }
        else
        {
          LOG_INFO("Conflating cumulative map with " << inputs[i] << "...");
        }

        loadMap(map, inputs[i], ConfigOptions().getConflateUseDataSourceIds(), Status::Unknown2);
        if (i == 1 && Log::getInstance().isDebugEnabled())
        {
          LOG_DEBUG("Writing debug map.");
          OsmMapPtr debug(new OsmMap(map));
          MapProjector::projectToWgs84(debug);
          OsmMapWriterFactory::write(debug, ConfigOptions().getDebugMapFilename());
        }
        //keep a source tag history on the data for provenance; append to any existing source values
        //TODO: this shouldn't be modifying review relations
        SetTagVisitor sourceTagVisitor(sourceTagKey, QString::number(i + 1), true);
        map->visitRw(sourceTagVisitor);

        NamedOp(ConfigOptions().getConflatePreOps()).apply(map);

        UnifyingConflator().apply(map);

        //NamedOp(ConfigOptions().getConflatePostOps()).apply(map);

        if (i < inputs.size() - 1)
        {
          //Up until just before the last conflate job, set the status tag back to 1 so that the
          //accumulated data will conflate with the next dataset.
          //TODO: this shouldn't be modifying review relations
          SetTagVisitor statusTagVisitor(
            "hoot:status", QString("%1").arg(Status(Status::Unknown1).getEnum()));
          map->visitRw(statusTagVisitor);

//          if (Log::getInstance().isDebugEnabled())
//          {
//            LOG_DEBUG("Writing debug map.");
//            OsmMapPtr debug(new OsmMap(map));
//            MapProjector::projectToWgs84(debug);
//            OsmMapWriterFactory::write(debug, ConfigOptions().getDebugMapFilename());
//          }
        }
      }
    }

    NamedOp(ConfigOptions().getConflatePostOps()).apply(map);

    MapProjector::projectToWgs84(map);
    saveMap(map, output);

    return 0;
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

    //TODO: make this work with stats
    if (args.size() == 2 && args[0].split(";").size() > 1)
    {
      return runMultiple(args);
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

    LOG_INFO(
      "Conflating " << input1 << " with " << input2 << " and writing the output to " << output);

    double bytesRead = IoSingleStat(IoSingleStat::RChar).value;
    LOG_VAR(bytesRead);
    QList< QList<SingleStat> > allStats;

    // read input 1
    shared_ptr<OsmMap> map(new OsmMap());
    loadMap(map, input1, ConfigOptions().getConflateUseDataSourceIds(), Status::Unknown1);
    // read input 2
    if (!input2.isEmpty())
    {
      loadMap(map, input2, ConfigOptions().getConflateUseDataSourceIds(), Status::Unknown2);
    }
    double inputBytes = IoSingleStat(IoSingleStat::RChar).value - bytesRead;
    LOG_VAR(inputBytes);
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

    NamedOp(ConfigOptions().getConflatePreOps()).apply(map);

    stats.append(SingleStat("Apply Named Ops Time (sec)", t.getElapsedAndRestart()));

    shared_ptr<OsmMap> result = map;

    if (ConfigOptions().getConflateEnableOldRoads())
    {
      // call the old road conflation routine
      Conflator conflator;
      conflator.loadSource(map);
      conflator.conflate();
      result.reset(new OsmMap(conflator.getBestMap()));
      stats.append(SingleStat("Old Road Conflation Time (sec)", t.getElapsedAndRestart()));
    }

    {
      // call new conflation routine
      UnifyingConflator conflator;
      conflator.apply(result);
      stats.append(conflator.getStats());
      stats.append(SingleStat("Conflation Time (sec)", t.getElapsedAndRestart()));
    }

    // Apply any user specified operations.
    NamedOp(ConfigOptions().getConflatePostOps()).apply(result);

    MapProjector::projectToWgs84(result);
    stats.append(SingleStat("Project to WGS84 Time (sec)", t.getElapsedAndRestart()));

    saveMap(result, output);
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

    if (displayStats)
    {
      if (outputStatsFile.isEmpty())
      {
        allStats.append(stats);
        MapStatsWriter().writeStats(allStats, args);
        QString statsMsg = MapStatsWriter().statsToString( allStats, "\t" );
        cout << "stats = (stat) OR (input map 1 stat) (input map 2 stat) (output map stat)\n" << statsMsg << endl;
      }
      else
      {
        allStats.append(stats);
        MapStatsWriter().writeStatsToJson(allStats, outputStatsFile);
        cout << "stats = (stat) OR (input map 1 stat) (input map 2 stat) (output map stat) in file: " << outputStatsFile << endl;
      }
    }

    return 0;
  }
};

}

#endif // CONFLATECMD_H
