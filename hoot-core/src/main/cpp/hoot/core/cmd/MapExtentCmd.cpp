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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/util/GeometryUtils.h>

using namespace std;

namespace hoot
{

class MapExtentCmd : public BaseCommand
{

public:

  static string className() { return "hoot::MapExtentCmd"; }

  MapExtentCmd() {}

  virtual QString getName() const { return "map-extent"; }

  int runSimple(QStringList args)
  {
    if (args.size() < 1 || args.size() > 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes one or two parameters.").arg(getName()));
    }

    bool verbose = true;
    if (args.size() == 2 && args[1].toLower() == "false")
    {
      verbose = false;
    }
    LOG_VARD(verbose);

    const QString input = args[0];
    LOG_VARD(input);
    OsmMapPtr map(new OsmMap());
    loadMap(map, input, true, Status::Invalid);

    const QString bounds =
      GeometryUtils::envelopeToConfigString(CalculateMapBoundsVisitor::getGeosBounds(map));
    if (verbose)
    {
      cout << "Map extent (minx,miny,maxx,maxy): " << bounds << endl;
    }
    else
    {
      cout << bounds << endl;
    }

    return 0;
  }

};

HOOT_FACTORY_REGISTER(Command, MapExtentCmd)

}
