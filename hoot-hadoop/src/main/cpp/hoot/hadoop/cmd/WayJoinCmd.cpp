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
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/way-join/WayJoinDriver.h>
#include <hoot/core/cmd/BaseCommand.h>

// Qt
#include <QImage>

// Std
#include <iostream>
#include <string>

namespace hoot
{

class WayJoinCmd : public BaseCommand
{

public:

  static std::string className() { return "hoot::WayJoinCmd"; }

  QString getName() const { return "way-join"; }

  WayJoinCmd()
  {
  }

  QString getHelp() const
  {
    return "way-bbox (input.pbf) (output.pbf) (maxWaySize)\n"
      "  Reads the data from input.pbf, calculates the bbox for each way and write the result\n"
      "  including the optional bbox value into output.pbf. This uses Hadoop for distribution.\n"
      "  * input.pbf - Input .pbf directory -- must reside on HDFS.\n"
      "  * output.pbf - Output .pbf directory -- must reside on HDFS.\n"
      "  * maxWaySize - Max way size in degrees.";
  }

  int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes exactly three parameters.").arg(getName()));
    }
    QString in = args[0];
    QString out = args[1];
    bool ok;
    double maxWaySize = args[2].toDouble(&ok);
    if (!ok)
    {
      throw HootException(QString("Expected max way size as a double. Got: %1").arg(args[2]));
    }

    WayJoinDriver driver(maxWaySize);
    driver.calculateWayBounds(in, out);

    return 0;
  }

};

HOOT_FACTORY_REGISTER(Command, WayJoinCmd)

}
