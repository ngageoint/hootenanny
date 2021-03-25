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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

//  Hoot
#include <hoot/core/algorithms/splitter/OsmMapSplitter.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class SplitCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::SplitCmd"; }

  SplitCmd() = default;

  QString getName() const override { return "split"; }

  QString getDescription() const override { return "Splits a map into tiles"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() != 3)
    {
      LOG_VARD(args);
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three parameters.").
                          arg(getName()));
    }

    //  Load the tile map ignoring the file IDs
    OsmMapPtr tile_map(new OsmMap());
    IoUtils::loadMap(tile_map, args[0], false);
    //  Don't introduce source:datetime or source:ingest:datetime
    conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), false);
    //  Load the actual map and use the file IDs
    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, args[1], true, Status::Unknown1);
    //  Split the map up into smaller maps
    OsmMapSplitter mapSplitter(map, tile_map);
    mapSplitter.apply();
    //  Don't include error:circular
    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), false);
    //  Write the maps out to disk
    mapSplitter.writeMaps(args[2]);

    LOG_STATUS("Map split in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, SplitCmd)

}
