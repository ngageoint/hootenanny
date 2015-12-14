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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapReprojector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

class Osm2OgrCmd : public BaseCommand
{
public:

  static string className() { return "hoot::Osm2OgrCmd"; }

  Osm2OgrCmd() { }

  virtual QString getHelp() const
  {
    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " (translation) (input.osm) (output)\n"
        "  * translation - JavaScript file name.\n"
        "  * input.osm - An OSM compatible input format (e.g. .osm or .osm.pbf)\n"
        "  * output - Output file name. The format is determined by extension. \n"
        "      FileGDB (*.gdb) and Shapefile (*.shp) have been tested but other\n"
        "      OGR compatible formats will likely work.\n";
  }

  virtual QString getName() const { return "osm2ogr"; }

  static QString opsKey() { return "osm2ogr.ops"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes exactly three parameters.").arg(getName()));
    }

    int a = 0;
    QString translation = args[a++];
    QString input = args[a++];
    QString output = args[a++];

    OgrWriter writer;
    writer.setScriptPath(translation);
    writer.open(output);

    shared_ptr<OsmMap> map(new OsmMap());

    loadMap(map, input, true);

    // Apply any user specified operations.
    NamedOp(ConfigOptions().getOsm2ogrOps()).apply(map);

    MapReprojector::reprojectToWgs84(map);

    writer.write(map);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, Osm2OgrCmd)

}

