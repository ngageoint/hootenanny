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

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/fourpass/FourPassManager.h>
#include <hoot/core/ops/OpList.h>
#include <hoot/core/ops/MergeNearbyNodes.h>
#include <hoot/core/ops/ReprojectToGeographicOp.h>
#include <hoot/core/ops/ReprojectToPlanarOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/HadoopTileWorker2.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>

using namespace std;

namespace hoot
{

class BigMergeNodesCmd : public BaseCommand
{
public:
  static std::string className() { return "hoot::BigMergeNodesCmd"; }

  BigMergeNodesCmd()
  {
  }

  QString getName() const { return "big-merge-nodes"; }

  int runSimple(QStringList args)
  {
    if (args.size() < 2 || args.size() > 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two to three parameters.").arg(getName()));
    }
    QString in = args[0];
    QString out = args[1];
    bool local = args.contains("--local");
    double pixelSize = ConfigOptions().getHootHadoopPixelSize();
    int maxNodeCount = ConfigOptions().getHootHadoopMaxNodeCount();

    if (local)
    {
      pp::Job::setDefaultJobTracker("local");
    }

    boost::shared_ptr<TileWorker2> worker(new HadoopTileWorker2());
    FourPassManager driver(worker);
    driver.setBuffer(pixelSize);
    driver.setMaxNodesPerBox(maxNodeCount);
    driver.setSource(in);

    boost::shared_ptr<OpList> op(new OpList());
    // this will automatically reproject to planar as needed.
    op->addOp(boost::shared_ptr<OsmMapOperation>(new MergeNearbyNodes()));
    // after the op is done the reducer will automatically reproject to WGS84 as needed.
    driver.setOperation(op);

    driver.apply(out);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BigMergeNodesCmd)

}
