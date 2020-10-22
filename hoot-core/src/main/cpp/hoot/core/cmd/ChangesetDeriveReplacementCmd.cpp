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
#include <hoot/core/algorithms/changeset/ChangesetReplacement.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/ChangesetStatsFormat.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QFileInfo>

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
    LOG_VARD(args);
    run(args);

    return 0;
  }

  void run(QStringList& args)
  {
    // process optional params

    // TODO: eventually this check can go away
    if (args.contains("--full-replacement"))
    {
      LOG_WARN(
        "All replacements are now done fully over the replacement bounds, so the " <<
        "--full-replacement option need no longer be specified to " << getName());
      args.removeAll("--full-replacement");
    }

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

    // TODO: eventually this check can go away
    if (args.contains("--strict-bounds"))
    {
      LOG_WARN(
        "Bounds handling is now determined automatically by the replacement workflow selected, " <<
        "so the --full-replacement option need no longer be specified to " << getName());
      args.removeAll("--strict-bounds");
    }

    bool printStats = false;
    QString outputStatsFile;
    processStatsParams(args, printStats, outputStatsFile);

    if (args.contains("--disable-way-snapping"))
    {
      LOG_WARN("--disable-way-snapping no longer supported by " << getName());
      args.removeAll("--disable-way-snapping");
    }

    // TODO: eventually this check can go away
    if (args.contains("--conflate"))
    {
      LOG_WARN("--conflate option is no longer supported by  " << getName() << ".");
      args.removeAll("--conflate");
    }
    if (args.contains("--disable-conflation"))
    {
      LOG_WARN("The --disable-conflation option is no longer supported by " << getName() << ".");
      args.removeAll("--disable-conflation");
    }

    // TODO: eventually this check can go away
    if (args.contains("--disable-cleaning"))
    {
      LOG_WARN(
        "Cleaning now done on an as-needed basis, so the --disable-cleaning option is no " <<
        "longer supported by " << getName());
      args.removeAll("--disable-cleaning");
    }

    // TODO: eventually this check can go away
    if (args.contains("--disable-oob-way-handling"))
    {
      LOG_WARN("The --disable-oob-way-handling option is no longer supported by " << getName());
      args.removeAll("--disable-oob-way-handling");
    }

    // command workflow identification and input param error checking

    LOG_VARD(args);
    bool cutOnly = false;
    int input1Index = 0;
    int input2Index = 1;
    int boundsIndex = 2;
    int outputIndex = 3;
    int osmApiDbUrlIndex = 4;
    LOG_VARD(args[2]);
    if (!GeometryUtils::isEnvelopeConfigString(args[2].trimmed()))
    {
      input2Index = -1;
      boundsIndex = 1;
      outputIndex = 2;
      osmApiDbUrlIndex = 3;
      cutOnly = true;
    }
    LOG_VARD(cutOnly);
    LOG_VARD(input1Index);
    LOG_VARD(input2Index);
    LOG_VARD(boundsIndex);
    LOG_VARD(outputIndex);
    LOG_VARD(osmApiDbUrlIndex);

    // do this here to get the --write-bounds param removed from args before the param error
    // checking and non-optional param parsing
    const QString boundsStr = args[boundsIndex].trimmed();
    conf().set(ConfigOptions::getConvertBoundingBoxKey(), boundsStr);
    BoundedCommand::runSimple(args);

    LOG_VARD(args);
    if (!cutOnly)
    {
      if (args.size() != 4 && args.size() != 5)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with two input maps takes four or five parameters.").arg(getName()));
      }
    }
    else
    {
      if (args.size() != 3 && args.size() != 4)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with one input map takes three or four parameters.").arg(getName()));
      }
    }

    // process non-optional params

    const QString input1 = args[input1Index].trimmed();
    LOG_VARD(input1);
    QString input2;
    if (!cutOnly)
    {
      input2 = args[input2Index].trimmed();
      LOG_VARD(input1);
    }
    const geos::geom::Envelope bounds = GeometryUtils::envelopeFromConfigString(boundsStr);
    LOG_VARD(bounds);
    const QString output = args[outputIndex].trimmed();
    LOG_VARD(output);
    QString osmApiDbUrl;
    LOG_VARD(output.toLower().endsWith(".osc.sql"));
    if (output.toLower().endsWith(".osc.sql"))
    {
      if (!cutOnly && args.size() != 5)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with two map inputs and a SQL output takes five parameters.").arg(getName()));
      }
      else if (cutOnly && args.size() != 4)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with one map input and a SQL output takes four parameters.").arg(getName()));
      }
      osmApiDbUrl = args[osmApiDbUrlIndex].trimmed();
    }
    LOG_VARD(osmApiDbUrl);

    // generate the changeset

    QString implementation = ConfigOptions().getChangesetReplacementImplementation();
    if (cutOnly)
    {
      implementation = ConfigOptions().getChangesetReplacementCutOnlyImplementation();
    }
    LOG_VARD(implementation);
    std::shared_ptr<ChangesetReplacement> changesetCreator(
      Factory::getInstance().constructObject<ChangesetReplacement>(implementation));
    changesetCreator->setFullReplacement(true);
    ChangesetReplacement::BoundsInterpretation boundInterpretation =
      ChangesetReplacement::BoundsInterpretation::Lenient;
    if (cutOnly)
    {
      boundInterpretation = ChangesetReplacement::BoundsInterpretation::Strict;
    }
    LOG_VARD(boundInterpretation);
    changesetCreator->setBoundsInterpretation(boundInterpretation);
    changesetCreator->setGeometryFilters(geometryFilters);
    // chain param must be set before the filters themselves
    changesetCreator->setChainReplacementFilters(chainReplacementFilters);
    changesetCreator->setReplacementFilters(replacementFilters);
    changesetCreator->setReplacementFilterOptions(replacementFilterOptions);
    changesetCreator->setChainRetainmentFilters(chainRetainmentFilters);
    changesetCreator->setRetainmentFilters(retainmentFilters);
    changesetCreator->setRetainmentFilterOptions(retainmentFilterOptions);
    changesetCreator->setConflationEnabled(false);
    changesetCreator->setChangesetOptions(printStats, outputStatsFile, osmApiDbUrl);
    changesetCreator->create(input1, input2, bounds, output);
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
};

HOOT_FACTORY_REGISTER(Command, ChangesetDeriveReplacementCmd)

}


