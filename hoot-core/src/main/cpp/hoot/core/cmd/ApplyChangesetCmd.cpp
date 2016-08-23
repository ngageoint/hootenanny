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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmApiDbSqlChangesetWriter.h>

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
    if (args.size() != 2 && args.size() != 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(
        QString("%1 takes two or four parameters and was given %2 parameters")
          .arg(getName())
          .arg(args.size()));
    }

    LOG_INFO("Applying changeset " << args[0] << " to " << args[1] << "...");

    if (args[0].endsWith(".osc"))
    {
      throw HootException(
        "XML changeset file writing is not currently supported by the apply-changeset command.");
    }
    else if (args[0].endsWith(".osc.sql"))
    {
      QUrl url(args[1]);
      OsmApiDbSqlChangesetWriter changesetWriter(url);

      if (args.size() == 4)
      {
        if (changesetWriter.conflictExistsInTarget(args[2], args[3]))
        {
          //Don't like throwing an exception here from the command line, but this error needs to
          //bubble up to the web service.
          throw HootException(
            "The changeset will not be written because conflicts exist in the target OSM API database.");
        }
      }

      QFile changesetSqlFile(args[0]);
      changesetWriter.write(changesetSqlFile);
      cout << changesetWriter.getChangesetStats();
    }
    else
    {
      throw HootException("Invalid changeset file format: " + args[0]);
    }

    return 0;
  }

};

HOOT_FACTORY_REGISTER(Command, ApplyChangesetCmd)

}

