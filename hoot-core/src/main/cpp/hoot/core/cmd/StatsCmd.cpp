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
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/io/MapStatsWriter.h>

namespace hoot
{

class NameSizesVisitor : public ElementVisitor, public OsmMapConsumer
{

};

class StatsCmd : public BaseCommand
{
public:
  static string className() { return "hoot::StatsCmd"; }

  StatsCmd() {}

  QString getHelp() const
  {
    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " [--quick] (input) [input2, ...]\n"
        "  Reads input and write out stats like node and way count. More may be added in\n"
        "  the future.\n"
        "  * --quick - If quick is specified then only a fast subset of the stats are \n"
        "    calculated.\n"
        "  * input - The input map path.";
  }

  virtual QString getName() const { return "stats"; }

  int runSimple(QStringList args)
  {
    if (args.size() < 1)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes one parameter.").arg(getName()));
    }

    bool quick = false;
    if (args.contains("--quick"))
    {
      args.removeOne("--quick");
      quick = true;
    }

    QString sep = "\t";
    // read the conflation status from the file.
    conf().set(ConfigOptions().getReaderUseFileStatusKey(), true);

    QList< QList<SingleStat> > allStats;

    for (int i = 0; i < args.size(); i++)
    {
      shared_ptr<OsmMap> map(new OsmMap());
      loadMap(map, args[i], true, Status::Invalid);

      MapProjector::reprojectToPlanar(map);

      shared_ptr<CalculateStatsOp> cso(new CalculateStatsOp());
      cso->setQuickSubset(quick);
      cso->apply(map);
      allStats.append(cso->getStats());
    }

    //MapStatsWriter().writeStats(allStats, args);
    cout << "Stat Name\t" << args.join(sep) << endl;
    cout << MapStatsWriter().statsToString(allStats, sep);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, StatsCmd)

}
