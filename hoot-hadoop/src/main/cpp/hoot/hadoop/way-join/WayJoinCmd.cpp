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

#include "WayJoinCmd.h"

// Hoot
#include <hoot/core/util/Factory.h>

// Qt
#include <QImage>

#include "WayJoinDriver.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(Command, WayJoinCmd)

WayJoinCmd::WayJoinCmd()
{
}

QString WayJoinCmd::getHelp() const
{
  return "way-bbox (input.pbf) (output.pbf) (maxWaySize)\n"
      "  Reads the data from input.pbf, calculates the bbox for each way and write the result\n"
      "  including the optional bbox value into output.pbf. This uses Hadoop for distribution.\n"
      "  * input.pbf - Input .pbf directory -- must reside on HDFS.\n"
      "  * output.pbf - Output .pbf directory -- must reside on HDFS.\n"
      "  * maxWaySize - Max way size in degrees.";
}

int WayJoinCmd::run(char *argv[], int argc)
{
  if (argc != 5)
  {
    cout << getHelp() << endl << endl;
    throw HootException(QString("%1 takes exactly three parameters.").arg(getName()));
  }
  QString in = argv[2];
  QString out = argv[3];
  bool ok;
  double maxWaySize = QString(argv[4]).toDouble(&ok);
  if (!ok)
  {
    throw HootException(QString("Expected max way size as a double. Got: %1").arg(argv[4]));
  }

  WayJoinDriver driver(maxWaySize);
  driver.calculateWayBounds(in, out);

  return 0;
}

}
