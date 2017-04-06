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
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/Settings.h>

// Qt
#include <QDebug>
#include <QTime>

namespace hoot
{

class AccuracyDistributionCmd : public BaseCommand
{
public:

  static string className() { return "hoot::AccuracyDistributionCmd"; }

  AccuracyDistributionCmd() { }

  virtual QString getName() const { return "accuracy-dist"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 1)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes one parameter.").arg(getName()));
    }

    QTime time;
    time.start();

    // open up both OSM files.
    OsmXmlReader reader;
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(args[0], map);

    const WayMap& ways = map->getWays();

    std::map<Meters, int> m;
    for (WayMap::const_iterator it = ways.begin(); it != ways.end(); it++)
    {
      const boost::shared_ptr<Way>& w = it->second;
      m[w->getCircularError()]++;
    }

    for (std::map<Meters, int>::iterator it = m.begin(); it != m.end(); it++)
    {
      double p = (double)it->second / (double)ways.size();
      cout << it->first << " : " << it->second << " (" << p << ")" << endl;
    }

    qDebug() << "Elapsed: " << (double)time.elapsed() / 1000.0 << "sec";
    return 0;

  }
};

HOOT_FACTORY_REGISTER(Command, AccuracyDistributionCmd)

}

