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
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmPbfWriter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Standard
#include <fstream>

namespace hoot
{
using namespace std;

class AddOsmPbfHeaderCmd : public BaseCommand
{
public:
  static string className() { return "hoot::AddOsmPbfHeaderCmd"; }

  AddOsmPbfHeaderCmd() {}

  virtual QString getName() const { return "add-pbf-header"; }

  int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }
    QString input = args[0];
    QString output = args[1];

    istream* is = NULL;
    ostream* os = NULL;

    if (input == "-")
    {
      is = &cin;
    }
    else
    {
      is = new fstream(input.toUtf8().constData(), ios::in | ios::binary);

      if (is->good() == false)
      {
        throw HootException(QString("Error reading %1").arg(input));
      }
    }

    if (output == "-")
    {
      os = &cout;
    }
    else
    {
      os = new fstream(output.toUtf8().constData(), ios::out | ios::binary);

      if (os->good() == false)
      {
        throw HootException(QString("Error reading %1").arg(output));
      }
    }

    OsmPbfWriter writer;

    writer.writeHeader(os, false, false);
    *os << is->rdbuf();
    os->flush();

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, AddOsmPbfHeaderCmd)

}
