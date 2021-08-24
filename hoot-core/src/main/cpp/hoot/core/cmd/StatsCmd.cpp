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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/MapStatsWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class StatsCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::StatsCmd"; }

  StatsCmd() = default;

  QString getName() const override { return "stats"; }
  QString getDescription() const override
  { return "Calculates a pre-configured set of statistics for a map"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    bool quick = false;
    if (args.contains("--brief"))
    {
      quick = true;
      args.removeAt(args.indexOf("--brief"));
    }

    QString output;
    if (args.contains("--output"))
    {
      const int outputIndex = args.indexOf("--output");
      output = args.at(outputIndex + 1).trimmed();

      QStringList supportedOutputFormats;
      supportedOutputFormats.append(".json");
      supportedOutputFormats.append(".txt");
      if (!StringUtils::endsWithAny(output, supportedOutputFormats))
      {
        throw IllegalArgumentException(
          "Invalid output format: ..." + FileUtils::toLogFormat(output, 25));
      }

      args.removeAt(outputIndex + 1);
      args.removeAt(outputIndex);
    }

    bool recursive = false;
    const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);

    if (args.size() < 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least one parameter. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    // Everything left is an input.
    QStringList inputs;
    if (!recursive)
    {
      inputs = IoUtils::expandInputs(args);
    }
    else
    {
      inputs = IoUtils::getSupportedInputsRecursively(args, inputFilters);
    }

    const QString sep = "\t";
    conf().set(ConfigOptions::getReaderUseFileStatusKey(), true);

    QList<QList<SingleStat>> allStats;
    for (int i = 0; i < inputs.size(); i++)
    {
      QElapsedTimer timer2;
      timer2.restart();

      OsmMapPtr map = std::make_shared<OsmMap>();
      // Tried using IoUtils::loadMap here, but it has extra logic beyond OsmMapReaderFactory::read
      // for reading in OGR layers. Using it causes the last part of Osm2OgrTranslationTest to fail.
      // Need to determine why either strictly use one reading method or the other.
      LOG_STATUS("Reading from: ..." << FileUtils::toLogFormat(inputs[i], 25) << "...");
      OsmMapReaderFactory::read(map, inputs[i], true, Status::Invalid);
      MapProjector::projectToPlanar(map);

      LOG_STATUS(
        "Calculating statistics for map " << i + 1 << " of " << inputs.size() << ": " <<
        inputs[i].right(25) << "...");

      std::shared_ptr<CalculateStatsOp> cso = std::make_shared<CalculateStatsOp>();
      cso->setQuickSubset(quick);
      cso->apply(map);
      allStats.append(cso->getStats());

      LOG_STATUS(
        "Statistics calculated for map " << i + 1 << " of " << inputs.size() << ": " <<
        inputs[i].right(25) << " in " + StringUtils::millisecondsToDhms(timer2.elapsed()));
    }

    if (!output.isEmpty())
    {
      LOG_STATUS(
        "Writing statistics output to: " << FileUtils::toLogFormat(output, 25) << "...");
      if (output.endsWith(".json", Qt::CaseInsensitive))
        MapStatsWriter().writeStatsToJson(allStats, output);
      else
        MapStatsWriter().writeStatsToText(allStats, output);
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
