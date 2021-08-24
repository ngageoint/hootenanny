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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/MapStatsWriter.h>
#include <hoot/core/io/OsmApiDbSqlChangesetApplier.h>
#include <hoot/core/io/OsmApiWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class ChangesetApplyCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::ChangesetApplyCmd"; }

  ChangesetApplyCmd() = default;

  QString getName() const override { return "changeset-apply"; }
  QString getDescription() const override { return "Writes an OSM changeset to an OSM data store"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

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
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least 2 parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    _progress =
      std::make_shared<Progress>(
        ConfigOptions().getJobId(), "Apply Changeset", Progress::JobState::Running);

    //  Write changeset/OSM XML to OSM API
    if (args[0].endsWith(".osc") || args[0].endsWith(".osm"))
    {
      _writeXmlChangeset(showProgress, showStats, args);
    }
    //  Write changeset SQL directly to the database
    else if (args[0].endsWith(".osc.sql"))
    {
      _writeSqlChangeset(args);
    }
    else
    {
      throw HootException("Invalid changeset file format: " + args[0]);
    }

    LOG_STATUS(
      "Changeset applied in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }

private:

  std::shared_ptr<Progress> _progress;

  void _writeXmlChangeset(
    const bool showProgress, const bool showStats, const QStringList& args) const
  {
    //  Get the endpoint URL
    const QString urlStr = args[args.size() - 1];
    if (!urlStr.toLower().startsWith("http://") && !urlStr.toLower().startsWith("https://"))
    {
      throw IllegalArgumentException(
        QString("XML changesets must be written to an OpenStreetMap compatible web service. ") +
        QString("Tried to write to: " + urlStr));
    }
    QUrl osm;
    osm.setUrl(args[args.size() - 1]);

    const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();

    //  Grab all the changeset files
    QList<QString> changesets;
    for (int i = 0; i < args.size() - 1; ++i)
    {
      _progress->set(
        0.0,
        "Adding changeset: ..." + FileUtils::toLogFormat(args[i], maxFilePrintLength) +
        " for application to: " + FileUtils::toLogFormat(osm.toString(), maxFilePrintLength) +
        "...", true);
      changesets.append(args[i]);
    }

    //  Get the error changeset pathname to pass to the writer
    QFileInfo path(args[0]);
    QString errorFilename =
      path.absolutePath() + QDir::separator() + path.baseName() + "-error." + path.completeSuffix();

    //  Do the actual splitting and uploading
    OsmApiWriter writer(osm, changesets);
    writer.setErrorPathname(errorFilename);
    writer.showProgress(showProgress);
    if (showProgress)
    {
      // OsmApiWriter is doing all the progress reporting but will pass a progress object in
      // anyway, just in case we ever add any extra job steps outside of it.
      writer.setProgress(*_progress);
    }
    writer.apply();

    _progress->set(
      1.0, writer.containsFailed() ? Progress::JobState::Failed : Progress::JobState::Successful,
      "Changeset(s) applied to: " + FileUtils::toLogFormat(osm.toString(), maxFilePrintLength) +
      "...");

    //  Output the last changeset ID in a status message
    LastElementInfo last = writer.getLastElementInfo();
    if (!last._id.isNull())
      LOG_STATUS("Last element pushed: " << last);

    //  Write out the failed changeset if there is one
    if (writer.containsFailed())
    {
      //  Output the errors from 'changeset.osc' to 'changeset-error.osc'
      LOG_ERROR(
        QString("Some changeset elements failed to upload. Stored in %1.").arg(errorFilename));
      writer.writeErrorFile();
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

  void _writeSqlChangeset(const QStringList& args) const
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(
        QString("%1 takes two parameters and was given %2 parameters")
          .arg(getName())
          .arg(args.size()));
    }

    LOG_STATUS("Applying changeset " << args[0] << " to " << args[1] << "...");

    QFile changesetSqlFile(args[0]);
    QUrl url(args[1]);
    OsmApiDbSqlChangesetApplier changesetWriter(url);
    changesetWriter.write(changesetSqlFile);
    //The tests rely on this being output, so leave it as a cout and not a log statement.
    cout << changesetWriter.getChangesetStatsSummary();
  }
};

HOOT_FACTORY_REGISTER(Command, ChangesetApplyCmd)

}
