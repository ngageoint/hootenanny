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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmFileSorter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/GeoNamesReader.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmPbfWriter.h>

namespace hoot
{

class SortCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::SortCmd"; }

  SortCmd() { }

  virtual QString getName() const { return "sort"; }

  virtual QString getDescription() const { return "Sorts map data"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    const QString input = args[0];
    const QString output = args[1];
    LOG_VARD(input);
    LOG_VARD(output);

    //Technically, OsmFileSorter is already doing some input validation, but it was customized for
    //use with multiary ingest, so it also allows for an OGR input and then autoconverts it to
    //PBF...that seems kind of inappropriate for this command.  So, let's just do our own input
    //validation here for now.
    bool valid = false;
    //There is no GeoNamesWriter, so just doing a simple extension check.
    if (GeoNamesReader().isSupported(input) && output.toLower().endsWith(".geonames"))
    {
      valid = true;
    }
    else if (OsmPbfReader().isSupported(input) && OsmPbfWriter().isSupported(output))
    {
      valid = true;
    }
    else if (OsmXmlReader().isSupported(input) && OsmXmlWriter().isSupported(output))
    {
      valid = true;
    }
    LOG_VARD(valid);

    if (!valid)
    {
      throw HootException(
        "Invalid input or output format.  Input format: " + input + ", output format: " + output);
    }

    OsmFileSorter().sort(input, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, SortCmd)

}

