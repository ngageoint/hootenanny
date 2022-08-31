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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/algorithms/MapStitcher.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class StitchCmd : public BaseCommand
{
public:

  static QString className() { return "StitchCmd"; }

  StitchCmd() = default;

  QString getName() const override { return "stitch"; }
  QString getDescription() const override { return "Stitches cropped maps back together"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    LOG_VART(args.size());
    LOG_VART(args);

    if (args.size() != 3)
    { 
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least three parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    //  Output is the last param
    const int outputIndex = args.size() - 1;
    const QString output = args[outputIndex];
    args.removeAt(outputIndex);
    LOG_VARD(output);

    OsmMapPtr baseMap = std::make_shared<OsmMap>();
    OsmMapPtr secondaryMap = std::make_shared<OsmMap>();

    //  Load in the two maps for the map stitcher
    IoUtils::loadMap(baseMap, args[0], true, Status::Unknown1);
    IoUtils::loadMap(secondaryMap, args[1], true, Status::Unknown2);

    //  Run the stitcher on the two maps
    MapStitcher::stitchMaps(baseMap, secondaryMap);

    //  Save the output map
    IoUtils::saveMap(baseMap, output);

    LOG_STATUS("Data stitching completed in " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, StitchCmd)

}
