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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/ChangesetStatsFormat.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QFileInfo>
#include <QElapsedTimer>

namespace hoot
{

/**
 * Derives a set of replacement changes given two map inputs
 *
 * @todo move the input parsing to a separate method and assign the parsed inputs to member
 * variables
 * @todo command needs some input error handling tests
 */
class ChangesetDeriveReplacementCmd : public BoundedCommand
{
public:

  static std::string className() { return "hoot::ChangesetDeriveReplacementCmd"; }

  ChangesetDeriveReplacementCmd() = default;

  virtual QString getName() const { return "changeset-derive-replacement"; }

  virtual QString getDescription() const
  { return "Creates an OSM changeset that replaces features in a map"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();
    LOG_VARD(args);

    // If needed for testing, can manually load separate implementations with
    // ConfigOptions().getChangesetReplacementImplementation().
    run(args);

    LOG_STATUS(
      "Changeset generated in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }

  void run(QStringList& args)
  {
    // process optional params

    bool fullReplacement = false;
    if (args.contains("--full-replacement"))
    {
      fullReplacement = true;
      args.removeAll("--full-replacement");
    }
    LOG_VARD(fullReplacement);

    QStringList geometryFilters;
    if (args.contains("--geometry-filters"))
    {
      const int optionNameIndex = args.indexOf("--geometry-filters");
      geometryFilters = args.at(optionNameIndex + 1).trimmed().split(";");
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(geometryFilters);

    QStringList replacementFilters;
    if (args.contains("--replacement-filters"))
    {
      const int optionNameIndex = args.indexOf("--replacement-filters");
      replacementFilters = args.at(optionNameIndex + 1).trimmed().split(";");
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(replacementFilters);

    bool chainReplacementFilters = false;
    if (args.contains("--chain-replacement-filters"))
    {
      chainReplacementFilters = true;
      args.removeAll("--chain-replacement-filters");
    }
    LOG_VARD(chainReplacementFilters);

    QStringList replacementFilterOptions;
    if (args.contains("--replacement-filter-options"))
    {
      const int optionNameIndex = args.indexOf("--replacement-filter-options");
      replacementFilterOptions = args.at(optionNameIndex + 1).trimmed().split(";");
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(replacementFilterOptions);

    QStringList retainmentFilters;
    if (args.contains("--retainment-filters"))
    {
      const int optionNameIndex = args.indexOf("--retainment-filters");
      retainmentFilters = args.at(optionNameIndex + 1).trimmed().split(";");
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(retainmentFilters);

    bool chainRetainmentFilters = false;
    if (args.contains("--chain-retainment-filters"))
    {
      chainRetainmentFilters = true;
      args.removeAll("--chain-retainment-filters");
    }
    LOG_VARD(chainRetainmentFilters);

    QStringList retainmentFilterOptions;
    if (args.contains("--retainment-filter-options"))
    {
      const int optionNameIndex = args.indexOf("--retainment-filter-options");
      retainmentFilterOptions = args.at(optionNameIndex + 1).trimmed().split(";");
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(retainmentFilterOptions);

    if (args.contains("--strict-bounds") && args.contains("--hybrid-bounds"))
    {
      throw IllegalArgumentException(
        "Only one of '--strict-bounds' and '--hybrid-bounds' may be specified.");
    }
    ChangesetReplacementCreator::BoundsInterpretation boundsHandling =
      ChangesetReplacementCreator::BoundsInterpretation::Lenient;
    if (args.contains("--strict-bounds"))
    {
      boundsHandling = ChangesetReplacementCreator::BoundsInterpretation::Strict;
      args.removeAll("--strict-bounds");
    }
    else if (args.contains("--hybrid-bounds"))
    {
      boundsHandling = ChangesetReplacementCreator::BoundsInterpretation::Hybrid;
      args.removeAll("--hybrid-bounds");
    }

    bool printStats = false;
    QString outputStatsFile;
    processStatsParams(args, printStats, outputStatsFile);

    bool enableWaySnapping = true;
    if (args.contains("--disable-way-snapping"))
    {
      enableWaySnapping = false;
      args.removeAll("--disable-way-snapping");
    }
    LOG_VARD(enableWaySnapping);

    bool enableConflation = true;
    if (args.contains("--disable-conflation"))
    {
      enableConflation = false;
      args.removeAll("--disable-conflation");
    }
    LOG_VARD(enableConflation);

    bool enableCleaning = true;
    if (args.contains("--disable-cleaning"))
    {
      if (!enableConflation)
      {
        enableCleaning = false;
      }
      else
      {
        LOG_WARN(
          "Cleaning cannot be disabled unless conflation is disabled for changeset replacement " <<
          "derivation.");
      }
      args.removeAll("--disable-cleaning");
    }
    LOG_VARD(enableCleaning);

    bool tagOobConnectedWays = true;
    if (args.contains("--disable-oob-way-handling"))
    {
      tagOobConnectedWays = false;
      args.removeAll("--disable-oob-way-handling");
    }
    LOG_VARD(tagOobConnectedWays);

    QString boundsStr = "";
    if (args.size() >= 3)
    {
      boundsStr = args[2].trimmed();
      conf().set(ConfigOptions::getConvertBoundingBoxKey(), boundsStr);
      BoundedCommand::runSimple(args);
    }

    // param error checking
    checkParameterCount(args.size());

    // process non-optional params
    const QString input1 = args[0].trimmed();
    LOG_VARD(input1);
    const QString input2 = args[1].trimmed();
    LOG_VARD(input2);
    const geos::geom::Envelope bounds = GeometryUtils::envelopeFromConfigString(boundsStr);
    LOG_VARD(bounds);
    const QString output = args[3].trimmed();
    LOG_VARD(output);

    QString osmApiDbUrl;
    if (output.endsWith(".osc.sql"))
    {
      if (args.size() != 5)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with SQL output takes five parameters.").arg(getName()));
      }
      osmApiDbUrl = args[4].trimmed();
    }

    ChangesetReplacementCreator changesetCreator(printStats, outputStatsFile, osmApiDbUrl);
    changesetCreator.setFullReplacement(fullReplacement);
    changesetCreator.setBoundsInterpretation(boundsHandling);
    changesetCreator.setGeometryFilters(geometryFilters);
    // chain param must be set before the filters themselves
    changesetCreator.setChainReplacementFilters(chainReplacementFilters);
    changesetCreator.setReplacementFilters(replacementFilters);
    changesetCreator.setReplacementFilterOptions(replacementFilterOptions);
    changesetCreator.setChainRetainmentFilters(chainRetainmentFilters);
    changesetCreator.setRetainmentFilters(retainmentFilters);
    changesetCreator.setRetainmentFilterOptions(retainmentFilterOptions);
    changesetCreator.setWaySnappingEnabled(enableWaySnapping);
    changesetCreator.setConflationEnabled(enableConflation);
    changesetCreator.setCleaningEnabled(enableCleaning);
    changesetCreator.setTagOobConnectedWays(tagOobConnectedWays);
    changesetCreator.create(input1, input2, bounds, output);
  }

  void processStatsParams(QStringList& args, bool& printStats, QString& outputStatsFile)
  {
    if (args.contains("--stats"))
    {
      printStats = true;
      const int statsIndex = args.indexOf("--stats");
      // See similar note in ChangesetDeriveCmd.
      if (statsIndex != -1 && statsIndex != (args.size() - 1) &&
          !args[statsIndex + 1].startsWith("--"))
      {
        outputStatsFile = args[statsIndex + 1];
        QFileInfo statsInfo(outputStatsFile);
        if (!ChangesetStatsFormat::isValidFileOutputFormat(statsInfo.completeSuffix()))
        {
          outputStatsFile = "";
        }
        else
        {
          args.removeAll(outputStatsFile);
        }
      }
        args.removeAll("--stats");
    }
    LOG_VARD(printStats);
    LOG_VARD(outputStatsFile);
  }

  void checkParameterCount(int count)
  {
    if (count != 4 && count != 5)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes four or five parameters.").arg(getName()));
    }
  }
};

HOOT_FACTORY_REGISTER(Command, ChangesetDeriveReplacementCmd)

}


