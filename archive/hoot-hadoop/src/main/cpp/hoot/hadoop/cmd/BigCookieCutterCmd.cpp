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
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/fourpass/FourPassManager.h>
#include <hoot/core/ops/OpList.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/UnionPolygonsVisitor.h>
#include <hoot/hadoop/HadoopTileWorker2.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class BigCookieCutterCmd : public BaseCommand
{
public:
  static std::string className() { return "hoot::BigCookieCutterCmd"; }

  BigCookieCutterCmd()
  {
  }

  void bufferGeometry(boost::shared_ptr<Geometry>& g, double b)
  {
    boost::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
    if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
    {
      throw HootException("Error creating EPSG:4326 projection.");
    }
    boost::shared_ptr<OGREnvelope> e(GeometryUtils::toOGREnvelope(*g->getEnvelopeInternal()));
    boost::shared_ptr<OGRSpatialReference> planar = MapProjector::createAeacProjection(*e);

    MapProjector::project(g, wgs84, planar);

    g.reset(g->buffer(b));

    MapProjector::project(g, planar, wgs84);
  }

  QString getName() const { return "big-cookie-cut"; }

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
    double pixelSize = ConfigOptions().getHootHadoopPixelSize();
    int maxNodeCount = ConfigOptions().getHootHadoopMaxNodeCount();

    if (local)
    {
      pp::Job::setDefaultJobTracker("local");
    }

    boost::shared_ptr<TileWorker2> worker(new HadoopTileWorker2());
    FourPassManager driver(worker);
    driver.setMaxNodesPerBox(maxNodeCount);
    driver.setBuffer(pixelSize);
    driver.setSource(in);

    OsmMapPtr cutterShapeMap(new OsmMap());
    loadMap(cutterShapeMap, cookieCutterPath, false, Status::Unknown1);

    UnionPolygonsVisitor v;
    cutterShapeMap->visitRo(v);
    boost::shared_ptr<Geometry> cookieCutter = v.getUnion();

    bufferGeometry(cookieCutter, buffer);

    if (cookieCutter->getArea() == 0.0)
    {
      throw HootException("Cutter area is zero. Try increasing the buffer size or check the input. "
        "Remember that buffer units are in meters.");
    }

    boost::shared_ptr<OpList> op(new OpList());
    op->addOp(boost::shared_ptr<OsmMapOperation>(new MapCropper(cookieCutter, !crop)));
    driver.setOperation(op);

    driver.apply(out);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BigCookieCutterCmd)

}
