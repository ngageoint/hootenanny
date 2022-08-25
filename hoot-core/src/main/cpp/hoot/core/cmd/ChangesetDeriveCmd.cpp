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
#include <hoot/core/algorithms/changeset/ChangesetCreator.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacement.h>
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/ChangesetStatsFormat.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

/**
 * Derives a set of changes given one or two map inputs
 */
class ChangesetDeriveCmd : public BoundedCommand
{
public:

  static QString className() { return "ChangesetDeriveCmd"; }

  ChangesetDeriveCmd() = default;

  QString getName() const override { return "changeset-derive"; }
  QString getDescription() const override
  { return "Creates a changeset representing the difference between two maps"; }

  int runSimple(QStringList& args) override
  {
    _timer.start();

    LOG_VARD(args);

    BoundedCommand::runSimple(args);

    // process optional params

    bool isReplacement = false;
    if (args.contains("--replacement"))
    {
      isReplacement = true;
      args.removeAll("--replacement");
    }
    LOG_VARD(isReplacement);

    bool enableWaySnapping = false;
    if (args.contains("--enable-way-snapping"))
    {
      enableWaySnapping = true;
      args.removeAll("--enable-way-snapping");
    }
    LOG_VARD(enableWaySnapping);

    if (!isReplacement && enableWaySnapping)
      throw IllegalArgumentException("The --enable-way-snapping option is only valid when the --replacement option is specified.");

    QString osmApiDbUrl;
    if (args.contains("--osmApiDatabaseUrl"))
    {
      const int osmApiDbUrlIndex = args.indexOf("--osmApiDatabaseUrl");
      osmApiDbUrl = args.at(osmApiDbUrlIndex + 1);
      args.removeAt(osmApiDbUrlIndex + 1);
      args.removeAt(osmApiDbUrlIndex);
    }

    bool printStats = false;
    QString outputStatsFile;
    _processStatsParams(args, printStats, outputStatsFile);

    LOG_VARD(args);

    if (args.size() != 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes three parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    // process required params

    const QString input1 = args[0].trimmed();
    const QString input2 = args[1].trimmed();
    const QString output = args[2].trimmed();

    if (output.endsWith(".osc.sql") && osmApiDbUrl.isEmpty())
      throw IllegalArgumentException(QString("Writing SQL changeset output (*.osc.sql) requires that the --osmApiDatabaseUrl parameter be specified."));

    if (!isReplacement)
      _deriveStandardChangeset(input1, input2, output, printStats, outputStatsFile, osmApiDbUrl);
    else
      _deriveReplacementChangeset(input1, input2, output, printStats, outputStatsFile, osmApiDbUrl, enableWaySnapping);

    return 0;
  }

private:

  QElapsedTimer _timer;

  void _processStatsParams(QStringList& args, bool& printStats, QString& outputStatsFile) const
  {
    if (args.contains("--stats"))
    {
      printStats = true;
      const int statsIndex = args.indexOf("--stats");
      // See similar note in ConflateCmd's parsing of --changeset-stats.
      if (statsIndex != -1 && statsIndex != (args.size() - 1) && !args[statsIndex + 1].startsWith("--"))
      {
        outputStatsFile = args[statsIndex + 1];
        QFileInfo statsInfo(outputStatsFile);
        if (!ChangesetStatsFormat::isValidFileOutputFormat(statsInfo.completeSuffix()))
          outputStatsFile = "";
        else
          args.removeAll(outputStatsFile);
      }
      args.removeAll("--stats");
    }
    LOG_VARD(printStats);
    LOG_VARD(outputStatsFile);
  }

  void _deriveStandardChangeset(const QString& input1, const QString& input2, const QString& output, const bool printStats,
                                const QString& outputStatsFile, const QString& osmApiDbUrl) const
  {
    const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
    LOG_STATUS(
      "Generating standard changeset for inputs: ..." <<
      FileUtils::toLogFormat(input1, maxFilePrintLength) << " and ..." <<
      FileUtils::toLogFormat(input2, maxFilePrintLength) << " and output: ..." <<
      FileUtils::toLogFormat(output, maxFilePrintLength));

    // Note that we may need to eventually further restrict this to only data with relation having
    // oob members due to full hydration (would then need to move this code to inside
    // ChangesetCreator).
    if (ConfigUtils::boundsOptionEnabled())
      _updateConfigOptionsForBounds();

    // printStats = true with an empty file means stdout only
    ChangesetCreator(printStats, outputStatsFile, osmApiDbUrl).create(output, input1, input2);

    LOG_STATUS("Changeset generated in " << StringUtils::millisecondsToDhms(_timer.elapsed()) << " total.");
  }

  void _deriveReplacementChangeset(const QString& input1, const QString& input2, const QString& output, const bool printStats,
                                   const QString& outputStatsFile, const QString& osmApiDbUrl, const bool enableWaySnapping) const
  {
    const bool isCutOnly = input2.isEmpty();
    QString implementation = ConfigOptions().getChangesetReplacementImplementation();
    if (isCutOnly)
      implementation = ConfigOptions().getChangesetReplacementCutOnlyImplementation();
    LOG_VARD(implementation);
    std::shared_ptr<ChangesetReplacement> changesetCreator =
      Factory::getInstance().constructObject<ChangesetReplacement>(implementation);
    changesetCreator->setFullReplacement(true);
    ChangesetReplacement::BoundsInterpretation boundInterpretation =
      ChangesetReplacement::BoundsInterpretation::Lenient;
    if (isCutOnly)
      boundInterpretation = ChangesetReplacement::BoundsInterpretation::Strict;
    LOG_VARD(boundInterpretation);
    changesetCreator->setBoundsInterpretation(boundInterpretation);
    changesetCreator->setEnableWaySnapping(enableWaySnapping);
    // printStats = true with an empty file means stdout only
    changesetCreator->setChangesetOptions(printStats, outputStatsFile, osmApiDbUrl);

    std::shared_ptr<geos::geom::Polygon> bounds;
    if (ConfigUtils::boundsOptionEnabled())
      bounds = std::dynamic_pointer_cast<geos::geom::Polygon>(ConfigUtils::getBounds());
    else
      bounds = GeometryUtils::polygonFromString("-180,-90,180,90");

    changesetCreator->create(input1, input2, bounds, output);
  }

  void _updateConfigOptionsForBounds() const
  {
    // If we're working within a bounds, we need to ensure that reference features outside of the
    // bounds don't get deleted.
    conf().set(ConfigOptions::getChangesetAllowDeletingReferenceFeaturesKey(), false);
  }
};

HOOT_FACTORY_REGISTER(Command, ChangesetDeriveCmd)

}


