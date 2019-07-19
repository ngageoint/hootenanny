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
#include <hoot/core/io/ChangesetWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

/**
 * TODO
 */
class ChangesetDeriveReplacementCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::ChangesetDeriveReplacementCmd"; }

  ChangesetDeriveReplacementCmd() {}

  virtual QString getName() const { return "changeset-derive-replacement"; }

  virtual QString getDescription() const { return "TODO"; }

  virtual int runSimple(QStringList args)
  {
//    bool printStats = false;
//    if (args.contains("--stats"))
//    {
//      printStats = true;
//      args.removeAll("--stats");
//    }
//    bool writeBoundsFile = false;
//    if (args.contains("--write-bounds"))
//    {
//      writeBoundsFile = true;
//      args.removeAll("--write-bounds");
//    }

//    if (args.size() < 3 || args.size() > 4)
//    {
//      std::cout << getHelp() << std::endl << std::endl;
//      throw HootException(QString("%1 takes three or four parameters.").arg(getName()));
//    }

//    const QString input1 = args[0];
//    const QString input2 = args[1];
//    const QString output = args[2];
//    QString osmApiDbUrl;
//    if (output.endsWith(".osc.sql"))
//    {
//      if (args.size() != 4)
//      {
//        std::cout << getHelp() << std::endl << std::endl;
//        throw HootException(QString("%1 with SQL output takes four parameters.").arg(getName()));
//      }
//      osmApiDbUrl = args[3];
//    }

//    ChangesetWriter(printStats, osmApiDbUrl).write(output, input1, input2);

//    if (writeBoundsFile)
//    {
//      ConfigOptions opts;
//      const QString boundsStr = opts.getConvertBoundingBox().trimmed();
//      if (!boundsStr.isEmpty())
//      {
//        OsmMapWriterFactory::write(
//          GeometryUtils::createMapFromBounds(GeometryUtils::envelopeFromConfigString(boundsStr)),
//          opts.getBoundsOutputFile());
//      }
//    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ChangesetDeriveReplacementCmd)

}


