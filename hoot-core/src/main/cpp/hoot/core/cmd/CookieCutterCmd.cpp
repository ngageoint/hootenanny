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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapReprojector.h>
#include <hoot/core/algorithms/AlphaShape.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/conflate/SuperfluousWayRemover.h>
#include <hoot/core/io/PbfWriter.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/visitors/UnionPolygonsVisitor.h>

// Standard
#include <fstream>

namespace hoot
{
using namespace std;

class CookieCutterCmd : public BaseCommand
{
public:
  static string className() { return "hoot::CookieCutterCmd"; }

  CookieCutterCmd() {}

  QString getHelp() const
  {
    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " (cutter-shape) (dough) (output) [buffer] [--crop]\n"
        "  Reads in a OSM input file as the 'cutter-shape', and removes the \n"
        "  cookie cutter shape from the contents of the 'dough' file. The\n"
        "  resulting geometries are written to 'output'. The dough input is limited by\n"
        "  the available RAM.\n"
        "  * cutter-shape - The input Polygons to use as the cutter shape.\n"
        "  * dough - The input OSM data to cut from.\n"
        "  * output - The output OSM data path.\n"
        "  * buffer - Optional buffer value, if the buffer value is provided then the\n"
        "             shape is buffered by this many meters before cutting. The buffer\n"
        "             may be positive or negative.\n"
        "  * --crop - Crops based on the polygon rather than doing a cookie cut.";
  }

  virtual QString getName() const { return "cookie-cutter"; }

  int runSimple(QStringList args)
  {
    if (args.size() < 3 || args.size() > 5)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three to five parameters.").arg(getName()));
    }
    int i = 0;
    QString cutterShapePath = args[i++];
    QString doughPath = args[i++];
    QString outputPath = args[i++];
    bool ok = false;
    double buffer = args.size() > i ? args[i].toDouble(&ok) : 0.0;
    if (ok)
    {
      i++;
    }
    else
    {
      buffer = 0.0;
    }
    bool crop = false;
    if (args.size() > i)
    {
      if (args[i++] == "--crop")
      {
        crop = true;
      }
      else
      {
        throw HootException(QString("Expected --crop as the last argument, but got %1.").
          arg(args[i - 1]));
      }
    }

    shared_ptr<OsmMap> cutterShapeMap(new OsmMap());
    loadMap(cutterShapeMap, cutterShapePath, true, Status::Unknown1);
    OGREnvelope env = cutterShapeMap->calculateBounds();

    // load up the "dough"
    shared_ptr<OsmMap> doughMap(new OsmMap());
    loadMap(doughMap, doughPath, true, Status::Unknown1);
    env.Merge(doughMap->calculateBounds());

    // reproject the dough and cutter into the same planar projection.
    MapReprojector::reprojectToPlanar(doughMap, env);
    MapReprojector::reprojectToPlanar(cutterShapeMap, env);

    // create a complex geometry representing the alpha shape
    UnionPolygonsVisitor v;
    cutterShapeMap->visitRo(v);
    shared_ptr<Geometry> cutterShape = v.getUnion();

    if (buffer != 0.0)
    {
      cutterShape.reset(cutterShape->buffer(buffer));
    }

    if (cutterShape->getArea() == 0.0)
    {
      LOG_WARN("Cutter area is zero. Try increasing the buffer size or check the input.");
    }

    shared_ptr<OsmMap> result;

    // free up a little RAM
    cutterShapeMap.reset();
    // remove the cookie cutter portion from the "dough"
    // if crop is true, then the cookie cutter portion is kept and the "dough" is dropped.
    MapCropper::crop(doughMap, cutterShape, !crop);
    // clean up any ugly bits left over
    SuperfluousWayRemover::removeWays(doughMap);
    doughMap = SuperfluousNodeRemover::removeNodes(doughMap);

    result = doughMap;

    // reproject back into lat/lng
    MapReprojector::reprojectToWgs84(result);

    // save out the result.
    saveMap(result, outputPath);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, CookieCutterCmd)

}
