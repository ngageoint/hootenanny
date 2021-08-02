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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/ElementIdSynchronizer.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class SynchronizeElementIdsCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::SynchronizeElementIdsCmd"; }

  SynchronizeElementIdsCmd() = default;

  QString getName() const override { return "sync-element-ids"; }
  QString getDescription() const override
  { return "Copies IDs for identical elements from one map to another (experimental)"; }
  QString getType() const override { return "rnd"; }

  int runSimple(QStringList& args) override
  {
    if (args.size() != 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes three parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    const QString input1 = args[0].trimmed();
    const QString input2 = args[1].trimmed();
    const QString output = args[2].trimmed();
    LOG_VARD(input1);
    LOG_VARD(input2);
    LOG_VARD(output);

    QElapsedTimer timer;
    timer.start();

    LOG_STATUS(
      "Synchronizing element IDs between ..." << FileUtils::toLogFormat(input1, 25) << " and ..." <<
      FileUtils::toLogFormat(input2, 25) << " to ..." << FileUtils::toLogFormat(output, 25) << "...");

    OsmMapPtr input1Map = std::make_shared<OsmMap>();
    IoUtils::loadMap(input1Map, input1, true, Status::Unknown1);
    OsmMapPtr input2Map = std::make_shared<OsmMap>();
    IoUtils::loadMap(input2Map, input2, true, Status::Unknown2);

    ElementIdSynchronizer idSynchronizer;
    idSynchronizer.synchronize(input1Map, input2Map);

    IoUtils::saveMap(input2Map, output);

    LOG_STATUS(
      "Synchronized " << idSynchronizer.getNumTotalFeatureIdsSynchronized() <<
      " element IDs from ..." << FileUtils::toLogFormat(input1, 25) <<
      " to ..." << FileUtils::toLogFormat(input2, 25) <<
      " and wrote them to ..." << FileUtils::toLogFormat(output, 25) << ".");
    LOG_STATUS("\t" << idSynchronizer.getNumNodeIdsSynchronized() << " nodes");
    LOG_STATUS("\t" << idSynchronizer.getNumWayIdsSynchronized() << " ways");
    LOG_STATUS("\t" << idSynchronizer.getNumRelationIdsSynchronized() << " relations");
    LOG_STATUS(
      "Synchronized element IDs in " << StringUtils::millisecondsToDhms(timer.elapsed()) <<
      " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, SynchronizeElementIdsCmd)

}
