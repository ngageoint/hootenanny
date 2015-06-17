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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>

// Qt
#include <QTime>
#include <QDebug>

namespace hoot
{

class ReviewedTagCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ReviewedTagCmd"; }

  ReviewedTagCmd() { }

  virtual QString getHelp() const
  {
    return getName() + " (input) (output)\n"
        "  Prints the reviewed tags for input and writes to output.\n"
        "  * input - Input (e.g. .osm file).\n"
        "  * output - Output file (e.g. .osm file).";
  }

  virtual QString getName() const { return "reviewed-tag"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    QTime time;
    time.start();

    // open up both OSM files.
    OsmReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(args[0], map);

    const WayMap& ways = map->getWays();

    for (WayMap::const_iterator it = ways.begin(); it != ways.end(); it++)
    {
      const shared_ptr<Way>& w = it->second;
      w->setTag("tiger:reviewed", "no");
      w->removeTag("reviewed");
      w->removeTag("raw:GFID");
      w->removeTag("license");
      w->removeTag("classification");
    }

    OsmWriter writer;
    writer.setIncludeHootInfo(false);
    writer.setIncludeIds(false);
    writer.write(map, args[1]);

    qDebug() << "Elapsed: " << (double)time.elapsed() / 1000.0 << "sec";
    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ReviewedTagCmd)

}

