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
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/IoUtils.h>
namespace hoot
{

/**
 * Derives a set of replacement changes given one or two map inputs
 *
 * TODO: implement progress
 */
class ChangesetDeriveReplacementCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::ChangesetDeriveReplacementCmd"; }

  ChangesetDeriveReplacementCmd() {}

  virtual QString getName() const { return "changeset-derive-replacement"; }

  virtual QString getDescription() const
  { return "Creates an OSM replacement changeset (experimental)"; }

  virtual int runSimple(QStringList args)
  {
    // process optional params

    bool lenientBounds = false;
    if (args.contains("--lenient-bounds"))
    {
      lenientBounds = true;
      args.removeAll("--lenient-bounds");
    }
    bool printStats = false;
    if (args.contains("--stats"))
    {
      printStats = true;
      args.removeAll("--stats");
    }
    bool writeBoundsFile = false;
    if (args.contains("--write-bounds"))
    {
      writeBoundsFile = true;
      args.removeAll("--write-bounds");
    }

    // param error checking

    if (ConfigOptions().getConvertOps().size())
    {
      throw IllegalArgumentException(getName() + " command does not support convert operations.");
    }

    if (args.size() < 5 || args.size() > 6)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes five or six parameters.").arg(getName()));
    }

    // process non-optional params

    const QString input1 = args[0].trimmed();
    const QString input2 = args[1].trimmed();
    const QString boundsStr = args[2].trimmed();
    const geos::geom::Envelope bounds = GeometryUtils::envelopeFromConfigString(boundsStr);
    const QString critClassName = args[3].trimmed();
    const QString output = args[4].trimmed();

    QString osmApiDbUrl;
    if (output.endsWith(".osc.sql"))
    {
      if (args.size() != 6)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with SQL output takes six parameters.").arg(getName()));
      }
      osmApiDbUrl = args[5].trimmed();
    }

    ChangesetReplacementCreator(printStats, osmApiDbUrl)
      .create(input1, input2, bounds, critClassName, lenientBounds, output);

    if (writeBoundsFile)
    {
      OsmMapWriterFactory::write(
        GeometryUtils::createMapFromBounds(bounds), ConfigOptions().getBoundsOutputFile());
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ChangesetDeriveReplacementCmd)

}


