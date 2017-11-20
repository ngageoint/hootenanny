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
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/fourpass/FourPassManager.h>
#include <hoot/core/ops/OpList.h>
#include <hoot/core/perty/BigPertyOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/HadoopTileWorker2.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>

using namespace std;

namespace hoot
{

class BigPertyCmd : public BaseCommand
{
public:
  static std::string className() { return "hoot::BigPertyCmd"; }

  BigPertyCmd()
  {
  }

  QString getName() const { return "big-perty"; }

  int runSimple(QStringList args)
  {
    if (args.size() < 3 || args.size() > 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three to four parameters.").arg(getName()));
    }
    bool ok;
    Meters sigma = args[0].toDouble(&ok);
    if (!ok)
    {
      throw HootException("Expected a valid double for sigma, got: " + args[0]);
    }
    QString in = args[1];
    QString out = args[2];

    bool local = args.contains("--local");
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

    boost::shared_ptr<BigPertyOp> perty(new BigPertyOp());
    perty->setSigma(sigma);
    boost::shared_ptr<OpList> op(new OpList());
    op->addOp(perty);
    driver.setOperation(op);

    driver.apply(out);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BigPertyCmd)

}
