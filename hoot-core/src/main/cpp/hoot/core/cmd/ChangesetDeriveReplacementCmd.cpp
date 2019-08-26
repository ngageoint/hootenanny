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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/IoUtils.h>

namespace hoot
{

/**
 * Derives a set of replacement changes given two map inputs
 */
class ChangesetDeriveReplacementCmd : public BoundedCommand
{
public:

  static std::string className() { return "hoot::ChangesetDeriveReplacementCmd"; }

  ChangesetDeriveReplacementCmd() {}

  virtual QString getName() const { return "changeset-derive-replacement"; }

  virtual QString getDescription() const
  { return "Creates an OSM replacement changeset (experimental)"; }

  virtual int runSimple(QStringList& args) override
  {
    const QString boundsStr = args[2].trimmed();
    conf().set(ConfigOptions::getConvertBoundingBoxKey(), boundsStr);
    BoundedCommand::runSimple(args);

    // process optional params

    QStringList geometryFilters;
    if (args.contains("--geometry-filters"))
    {
      const int optionNameIndex = args.indexOf("--geometry-filters");
      LOG_VARD(optionNameIndex);
      geometryFilters = args.at(optionNameIndex + 1).trimmed().split(";");
      LOG_VARD(geometryFilters);
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    QStringList additionalFilters;
    if (args.contains("--additional-filters"))
    {
      const int optionNameIndex = args.indexOf("--additional-filters");
      LOG_VARD(optionNameIndex);
      additionalFilters = args.at(optionNameIndex + 1).trimmed().split(";");
      LOG_VARD(additionalFilters);
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    bool chainAdditionalFilters = false;
    if (args.contains("--chain-additional-filters"))
    {
      chainAdditionalFilters = true;
      args.removeAll("--chain-additional-filters");
    }
    LOG_VARD(chainAdditionalFilters);
    bool lenientBounds = true;
    if (args.contains("--strict-bounds"))
    {
      lenientBounds = false;
      args.removeAll("--strict-bounds");
    }
    LOG_VARD(lenientBounds);
    bool printStats = false;
    if (args.contains("--stats"))
    {
      printStats = true;
      args.removeAll("--stats");
    }
    LOG_VARD(printStats);

    LOG_VARD(args.size());
    LOG_VARD(args);

    // param error checking

    if (ConfigOptions().getConvertOps().size())
    {
      throw IllegalArgumentException(getName() + " command does not support convert operations.");
    }

    if (args.size() < 4 || args.size() > 5)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes four or five parameters.").arg(getName()));
    }

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

    ChangesetReplacementCreator changesetCreator(printStats, osmApiDbUrl);
    changesetCreator.setLenientBounds(lenientBounds);
    changesetCreator.setGeometryFilters(geometryFilters);
    changesetCreator.setAdditionalFilters(additionalFilters);
    changesetCreator.setChainAdditionalFilters(chainAdditionalFilters);
    changesetCreator.create(input1, input2, bounds, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ChangesetDeriveReplacementCmd)

}


