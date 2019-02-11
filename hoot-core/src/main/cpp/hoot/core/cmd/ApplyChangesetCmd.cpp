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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/MapStatsWriter.h>
#include <hoot/core/io/OsmApiDbSqlChangesetApplier.h>
#include <hoot/core/io/OsmApiWriter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QFile>
#include <QFileInfo>

using namespace std;

namespace hoot
{

class ApplyChangesetCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ApplyChangesetCmd"; }

  ApplyChangesetCmd() { }

  virtual QString getName() const { return "changeset-apply"; }

  virtual QString getDescription() const { return "Writes an OSM changeset"; }

  virtual int runSimple(QStringList args) override
  {
    bool showStats = false;
    bool showProgress = false;
    //  Check for stats flag
    if (args.contains("--stats"))
    {
      showStats = true;
      args.removeAll("--stats");
    }
    //  Check for progress flag
    if (args.contains("--progress"))
    {
      showProgress = true;
      args.removeAll("--progress");
    }
    //  Make sure that there are at least two other arguments
    if (args.size() < 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(
        QString("%1 takes at least two parameters and was given %2 parameters")
          .arg(getName())
          .arg(args.size()));
    }
    //  Write changeset/OSM XML to OSM API
    if (args[0].endsWith(".osc") || args[0].endsWith(".osm"))
    {
      //  Get the endpoint URL
      QUrl osm;
      osm.setUrl(args[args.size() - 1]);
      //  Create a URL without user info for logging
      QUrl printableUrl = osm;
      printableUrl.setUserInfo("");
      //  Grab all the changeset files
      QList<QString> changesets;
      for (int i = 0; i < args.size() - 1; ++i)
      {
        LOG_INFO("Applying changeset " << args[i] << " to " << printableUrl.toString() << "...");
        changesets.append(args[i]);
      }
      //  Do the actual splitting and uploading
      OsmApiWriter writer(osm, changesets);
      writer.showProgress(showProgress);
      writer.apply();
      //  Write out the failed changeset if there is one
      if (writer.containsFailed())
      {
        //  Output the errors from 'changeset.osc' to 'changeset-error.osc'
        QFileInfo path(args[0]);
        QString errorFilename =
          path.absolutePath() + QDir::separator() +
          path.baseName() + "-error." + path.completeSuffix();
        LOG_ERROR(QString("Some changeset elements failed to upload. Stored in %1.").arg(errorFilename));
        FileUtils::writeFully(errorFilename, writer.getFailedChangeset());
      }
      //  Output the stats if requested
      if (showStats)
      {
        //  Jump through a few hoops to use the MapStatsWriter
        QList<QList<SingleStat>> allStats;
        allStats.append(writer.getStats());
        QString statsMsg = MapStatsWriter().statsToString(allStats, "\t");
        cout << "stats = (stat)\n" << statsMsg << endl;
      }
    }
    //  Write changeset SQL directly to the database
    else if (args[0].endsWith(".osc.sql"))
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

      QUrl url(args[1]);
      OsmApiDbSqlChangesetApplier changesetWriter(url);

      if (args.size() == 4)
      {
        if (changesetWriter.conflictExistsInTarget(args[2], args[3]))
        {
          //Don't like throwing an exception here from the command line, but this error needs to
          //bubble up to the web service.
          //The better thing to do here would be to return an error code and have the services
          //scripts look for it, I think.
          throw HootException(
            "The changeset will not be written because conflicts exist in the target OSM API database.");
        }
      }

      QFile changesetSqlFile(args[0]);
      changesetWriter.write(changesetSqlFile);
      //The tests rely on this being output, so leave it as a cout and not a log statement.
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

