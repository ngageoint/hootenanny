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
#include <hoot/core/geometry/GeometryUtils.h>
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

    bool printStats = false;
    QString outputStatsFile;
    processStatsParams(args, printStats, outputStatsFile);

    // command workflow identification and input param error checking

    LOG_VARD(args);
    bool cutOnly = false;
    int input1Index = 0;
    int input2Index = 1;
    int boundsIndex = 2;
    int outputIndex = 3;
    int osmApiDbUrlIndex = 4;
    LOG_VARD(args[2]);
    if (!GeometryUtils::isEnvelopeString(args[2].trimmed()) &&
        !GeometryUtils::isPolygonString(args[2].trimmed()))
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
    conf().set(ConfigOptions::getConvertBoundsKey(), boundsStr);
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
    changesetCreator->setChangesetOptions(printStats, outputStatsFile, osmApiDbUrl);

    std::shared_ptr<geos::geom::Polygon> bounds =
      std::dynamic_pointer_cast<geos::geom::Polygon>(
        GeometryUtils::boundsFromString(boundsStr));
    if (!bounds)
    {
      throw IllegalArgumentException(
        "Invalid replacement bounds passed to changeset replacement derivation.");
    }

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


