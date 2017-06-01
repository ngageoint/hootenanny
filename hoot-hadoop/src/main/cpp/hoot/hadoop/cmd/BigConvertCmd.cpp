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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/convert/WriteOsmSqlStatementsDriver.h>

// Qt
#include <QStringList>

using namespace std;

namespace hoot
{

//If we end up supporting more than one writer class with this, then this logic should be more
//in line with the convert command and all readers/writers used should implement appropriate
//interfaces.
class BigConvertCmd : public BaseCommand
{
public:

  static string className() { return "hoot::BigConvertCmd"; }

  BigConvertCmd() { }

  virtual QString getName() const { return "big-convert"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }
    if (!args[0].toLower().endsWith(".pbf"))
    {
      throw HootException(
        QString("%1 must take an OSM PBF file (.pbf or .osm.pbf) as input.").arg(getName()));
    }
    if (!args[1].toLower().endsWith(".sql") && !args[1].toLower().startsWith("osmapidb://"))
    {
      throw HootException(
        QString("%1 must take a SQL file (.sql) or an OSM API database (osmapidb://) as output.")
          .arg(getName()));
    }

    WriteOsmSqlStatementsDriver driver;
    driver.open(args[1]);
    driver.write(args[0]);
    driver.close();

    return 0;
  }

};

HOOT_FACTORY_REGISTER(Command, BigConvertCmd)

}
