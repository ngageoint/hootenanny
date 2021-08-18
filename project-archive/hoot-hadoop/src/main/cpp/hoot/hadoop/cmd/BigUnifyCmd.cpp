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
#include <hoot/core/conflate/TileConflator.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/fourpass/FourPassManager.h>
#include <hoot/core/ops/OpList.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/hadoop/HadoopTileWorker2.h>
#include <hoot/hadoop/paint-nodes/PaintNodesDriver.h>
#include <hoot/hadoop/stats/MapStatsDriver.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>

// Qt
#include <QImage>

using namespace std;

namespace hoot
{

class BigUnifyCmd : public BaseCommand
{
public:

  static string className() { return "hoot::BigUnifyCmd"; }

  BigUnifyCmd()
  {
  }

  virtual QString getName() const { return "big-unify"; }

  int runSimple(QStringList args)
  {
    if (args.size() < 3 || args.size() > 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three to four parameters.").arg(getName()));
    }
    QString in1 = args[0];
    QString in2 = args[1];
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
    driver.setSources(in1, in2);

    boost::shared_ptr<UnifyingConflator> unify(new UnifyingConflator());
    boost::shared_ptr<OpList> op(new OpList());
    op->addOp(unify);
    driver.setOperation(op);

    driver.apply(out);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BigUnifyCmd)

}
