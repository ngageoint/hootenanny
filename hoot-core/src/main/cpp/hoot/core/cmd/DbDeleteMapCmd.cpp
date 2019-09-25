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
 * @copyright Copyright (C) 2012, 2013, 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/HootApiDbWriter.h>

using namespace std;

namespace hoot
{

class DbDeleteMapCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DbDeleteMapCmd"; }

  DbDeleteMapCmd() {}

  virtual QString getName() const override { return "db-delete-map"; }

  virtual QString getDescription() const override
  { return "Deletes a map from the Hootenanny Web Services database"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() != 1)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes one parameter.").arg(getName()));
    }

    HootApiDbWriter().deleteMap(args[0]);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, DbDeleteMapCmd)

}
