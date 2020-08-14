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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/ElementIdSynchronizer.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/IoUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

/*
 * @todo needs command line test
 */
class SynchronizeElementIdsCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::SynchronizeElementIdsCmd"; }

  SynchronizeElementIdsCmd()
  {
  }

  virtual QString getName() const override { return "sync-element-ids"; }

  virtual QString getDescription() const override
  { return "Copies element IDs from one map to another when identical elements are found"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() != 3)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    const QString input1 = args[0].trimmed();
    const QString input2 = args[1].trimmed();
    const QString output = args[2].trimmed();
    LOG_VARD(input1);
    LOG_VARD(input2);
    LOG_VARD(output);

    LOG_STATUS(
      "Synchronizing element IDs between ..." << input1.right(25) << " and ..." <<
      input2.right(25) << " to ..." << output.right(25) << "...");

    OsmMapPtr input1Map(new OsmMap());
    IoUtils::loadMap(input1Map, input1, true, Status::Unknown1);
    OsmMapPtr input2Map(new OsmMap());
    IoUtils::loadMap(input2Map, input2, true, Status::Unknown2);

    ElementIdSynchronizer idSynchronizer;
    idSynchronizer.synchronize(input1Map, input2Map);

    IoUtils::saveMap(input2Map, output);

    LOG_STATUS(
      "Synchronized " << idSynchronizer.getNumTotalFeatureIdsSynchronized() <<
      " element IDs from ..." << input1.right(25) << " to ..." << input2.right(25) <<
      " and wrote them to ..." << output.right(25) << ".");
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
