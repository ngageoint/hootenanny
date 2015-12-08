/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// geos
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/fourpass/FourPassManager.h>
#include <hoot/core/ops/OpList.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/UnionPolygonsVisitor.h>
#include <hoot/hadoop/HadoopTileWorker2.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>

namespace hoot
{

class BigCookieCutterCmd : public BaseCommand
{
public:
  static std::string className() { return "hoot::BigCookieCutterCmd"; }

  BigCookieCutterCmd()
  {
  }

  void bufferGeometry(shared_ptr<Geometry>& g, double b)
  {
    shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
    if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
    {
      throw HootException("Error creating EPSG:4326 projection.");
    }
    auto_ptr<OGREnvelope> e(GeometryUtils::toOGREnvelope(*g->getEnvelopeInternal()));
    shared_ptr<OGRSpatialReference> planar = MapProjector::createAeacProjection(*e);

    MapProjector::reproject(g, wgs84, planar);

    g.reset(g->buffer(b));

    MapProjector::reproject(g, planar, wgs84);
  }

  QString getHelp() const
  {
    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " (cutter-shape) (dough.osm.pbf) (output.osm.pbf) [buffer] \n"
        "  [--local] [--crop]\n"
        "  Reads in a OSM input file as the 'cutter-shape', and removes the \n"
        "  cookie cutter shape from the contents of the 'dough' file. The\n"
        "  resulting geometries are written to 'output'. Very large inputs are\n"
        "  supported.\n"
        "  * cutter-shape - The input Polygons to use as the cutter shape. This must \n"
        "             reside on the local file system and must easily fit in RAM.\n"
        "  * dough.osm.pbf - The input OSM data to cut from. This is the data that will \n"
        "             be cut and written out. This must reside on HDFS.\n"
        "  * output.osm.pbf - The output OSM data path. This must reside on HDFS.\n"
        "  * buffer - Optional buffer value, if the buffer value is provided then the\n"
        "             shape is buffered by this many meters before cutting. The buffer\n"
        "             may be positive or negative.\n"
        "  * --local - Runs the job locally rather than using Hadoop.\n"
        "  * --crop - Crops based on the polygon rather than doing a cookie cut.";
  }

  QString getName() const { return "big-cookie-cutter"; }

  int runSimple(QStringList args)
  {
    if (args.size() < 3 || args.size() > 5)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three to five parameters.").arg(getName()));
    }
    QString cookieCutterPath = args[0];
    QString in = args[1];
    QString out = args[2];
    bool ok = false;
    double buffer = args.size() > 3 ? args[3].toDouble(&ok) : 0.0;
    if (!ok)
    {
      buffer = 0.0;
    }
    bool local = args.contains("--local");
    bool crop = args.contains("--crop");
    double pixelSize = conf().getDouble(HadoopTileWorker2::pixelSizeKey());
    int maxNodeCount = conf().getInt(HadoopTileWorker2::maxNodeCountKey());

    if (local)
    {
      pp::Job::setDefaultJobTracker("local");
    }

    shared_ptr<TileWorker2> worker(new HadoopTileWorker2());
    FourPassManager driver(worker);
    driver.setMaxNodesPerBox(maxNodeCount);
    driver.setBuffer(pixelSize);
    driver.setSource(in);

    shared_ptr<OsmMap> cutterShapeMap(new OsmMap());
    loadMap(cutterShapeMap, cookieCutterPath, false, Status::Unknown1);

    UnionPolygonsVisitor v;
    cutterShapeMap->visitRo(v);
    shared_ptr<Geometry> cookieCutter = v.getUnion();

    bufferGeometry(cookieCutter, buffer);

    if (cookieCutter->getArea() == 0.0)
    {
      throw HootException("Cutter area is zero. Try increasing the buffer size or check the input. "
        "Remember that buffer units are in meters.");
    }

    shared_ptr<OpList> op(new OpList());
    op->addOp(shared_ptr<OsmMapOperation>(new MapCropper(cookieCutter, !crop)));
    driver.setOperation(op);

    driver.apply(out);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BigCookieCutterCmd)

}
