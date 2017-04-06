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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ShapefileWriter.h>

namespace hoot
{

class Osm2ShpCmd : public BaseCommand
{
public:

  static string className() { return "hoot::Osm2ShpCmd"; }

  Osm2ShpCmd() { }

  virtual QString getName() const { return "osm2shp"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 2 || args.size() > 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two or three parameters.").arg(getName()));
    }
    int i = 0;
    QString columns;
    if (args.size() == 3)
    {
      columns = args[i++];
    }
    QString input = args[i++];
    QString output = args[i++];

    QStringList columnsArr = columns.split(",", QString::SkipEmptyParts);

    // open up both OSM files.
    boost::shared_ptr<OsmMap> map(new OsmMap());
    loadMap(map, input, true);

    ShapefileWriter writer;
    writer.setColumns(columnsArr);
    writer.write(map, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, Osm2ShpCmd)

}

