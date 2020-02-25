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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/MapStatsWriter.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class StatsCmd : public BaseCommand
{
public:

  static string className() { return "hoot::StatsCmd"; }

  StatsCmd() {}

  virtual QString getName() const override { return "stats"; }

  virtual QString getDescription() const override
  { return "Displays a default set of statistics for a map"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() < 1)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes one parameter.").arg(getName()));
    }

    QElapsedTimer timer;
    timer.start();

    const QString QUICK_SWITCH = "--brief";
    const QString OUTPUT_SWITCH = "--output=";

    QStringList inputs(args);

    bool quick = false;
    bool toFile = false;
    QString outputFilename = "";
    //  Capture any flags and remove them before processing inputs
    for (int i = 0; i < args.size(); i++)
    {
      if (args[i].startsWith(OUTPUT_SWITCH))
      {
        outputFilename = args[i];
        outputFilename.remove(OUTPUT_SWITCH);
        toFile = true;
        inputs.removeOne(args[i]);
      }
      else if (args[i] == QUICK_SWITCH)
      {
        quick = true;
        inputs.removeOne(args[i]);
      }
    }

    const QString sep = "\t";
    // read the conflation status from the file.
    conf().set(ConfigOptions::getReaderUseFileStatusKey(), true);

    QList<QList<SingleStat>> allStats;
    for (int i = 0; i < inputs.size(); i++)
    {
      QElapsedTimer timer2;
      timer2.restart();

      OsmMapPtr map(new OsmMap());
      // Tried using IoUtils::loadMap here, but it has extra logic beyond OsmMapReaderFactory::read
      // for reading in OGR layers. Using it causes the last part of Osm2OgrTranslationTest to fail.
      // Need to determine why either strictly use one reading method or the other.
      OsmMapReaderFactory::read(map, inputs[i], true, Status::Invalid);
      MapProjector::projectToPlanar(map);

      LOG_STATUS(
        "Calculating statistics for map " << i + 1 << " of " << inputs.size() << ": " <<
        inputs[i].right(25) << "...");

      std::shared_ptr<CalculateStatsOp> cso(new CalculateStatsOp());
      cso->setQuickSubset(quick);
      cso->apply(map);
      allStats.append(cso->getStats());

      LOG_STATUS(
        "Statistics calculated for map " << i + 1 << " of " << inputs.size() << ": " <<
        inputs[i].right(25) << " in " + StringUtils::millisecondsToDhms(timer2.elapsed()));
    }

    if (toFile)
    {
      LOG_STATUS("Writing statistics output to: " << outputFilename.right(25) << "...");
      if (outputFilename.endsWith(".json", Qt::CaseInsensitive))
        MapStatsWriter().writeStatsToJson(allStats, outputFilename);
      else
        MapStatsWriter().writeStatsToText(allStats, outputFilename);
    }
    else
    {
      LOG_STATUS("Writing statistics output to string...");
      cout << "Stat Name\t" << inputs.join(sep) << endl;
      cout << MapStatsWriter().statsToString(allStats, sep);
    }

    LOG_STATUS(
      "Map statistics calculated in " + StringUtils::millisecondsToDhms(timer.elapsed()) <<
      " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, StatsCmd)

}
