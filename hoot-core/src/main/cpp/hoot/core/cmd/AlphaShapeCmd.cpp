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

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/conflate/AlphaShapeGenerator.h>

namespace hoot
{
using namespace std;

class AlphaShapeCmd : public BaseCommand
{
public:
  static string className() { return "hoot::AlphaShapeCmd"; }

  AlphaShapeCmd() {}

  QString getHelp() const
  {
    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " (points) (alpha) (buffer) (output)\n"
        "  Reads in a OSM input file and uses the points to derives a polygon that\n"
        "  approximately represents the bounds as a alpha shape. The resulting\n"
        "  multipolygon is written to 'output'.\n"
        "  * points - The input OSM data to use for deriving the cookie cutter shape.\n"
        "             The alpha shape algorithm is used to derive a polygon representing\n"
        "             the bounds.\n"
        "  * alpha - The size in meters used for alpha. A larger value makes a smoother\n"
        "             shape and a smaller value will create a rough shape with more \n"
        "             holes. Value in meters.\n"
        "  * buffer - The buffer to add to the alpha shape before cutting. A negative\n"
        "             value will make the shape smaller. Value in meters.\n"
        "  * output - The output .osm data path.";
  }

  virtual QString getName() const { return "alpha-shape"; }

  int runSimple(QStringList args)
  {
    if (args.size() != 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes four parameters.").arg(getName()));
    }
    int i = 0;
    QString pointsPath = args[i++];
    double alpha = args[i++].toDouble();
    double buffer = args[i++].toDouble();
    QString outputPath = args[i++];

    shared_ptr<OsmMap> pointsMap(new OsmMap());
    loadMap(pointsMap, pointsPath, false, Status::Unknown1);
    OsmMapPtr result = AlphaShapeGenerator(alpha, buffer).generate(pointsMap);

    // reproject back into lat/lng
    MapProjector::reprojectToWgs84(result);

    // save out the result.
    if (outputPath.toLower().endsWith(".shp"))
    {
      ShapefileWriter writer;
      writer.writePolygons(result, outputPath);
    }
    else
    {
      saveMap(result, outputPath);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, AlphaShapeCmd)

}
