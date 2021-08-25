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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/rnd/io/MultiaryIngester.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class MultiaryPoiIngestCmd : public BaseCommand
{
public:

  static QString className() { return "MultiaryPoiIngestCmd"; }

  QString getName() const override { return "multiary-poi-ingest"; }
  QString getDescription() const override
  { return "Ingests POIs for use by multiary-poi-conflate (experimental) "; }
  QString getType() const override { return "rnd"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() != 4)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes four parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    const QString input = args[0];
    const QString translation = args[1];
    const QString output = args[2];
    const QString changesetOutput = args[3];

    LOG_STATUS(
      "Ingesting multiary POI data from ..." << FileUtils::toLogFormat(input, 25) <<
      " using translation ..." << FileUtils::toLogFormat(translation, 25) <<
      " and writing output to ..." << FileUtils::toLogFormat(output, 25) <<
      " and changeset output to ..." << FileUtils::toLogFormat(changesetOutput, 25) << "...");
    MultiaryIngester().ingest(input, translation, output, changesetOutput);
    LOG_STATUS(
      "Ingest completed in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, MultiaryPoiIngestCmd)

}
