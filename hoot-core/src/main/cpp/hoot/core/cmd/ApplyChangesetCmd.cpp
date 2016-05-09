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
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmApiDb.h>

// Qt
#include <QFile>

namespace hoot
{

class ApplyChangesetCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ApplyChangesetCmd"; }

  ApplyChangesetCmd() { }

  virtual QString getName() const { return "apply-changeset"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    if (args[0].endsWith(".osc"))
    {
      throw HootException(
        "XML changeset file writing is not supported by the apply-changeset command.");
    }
    else if (args[0].endsWith(".osc.sql"))
    {
      QFile changesetSqlFile(args[0]);
      OsmApiDb().writeChangeset(changesetSqlFile, QUrl(args[1]));
    }
    else
    {
      throw HootException("Invalid source file format: " + args[0]);
    }

    return 0;
  }

};

HOOT_FACTORY_REGISTER(Command, ApplyChangesetCmd)

}

