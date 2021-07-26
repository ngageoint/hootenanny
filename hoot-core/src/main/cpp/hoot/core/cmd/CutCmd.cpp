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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{
using namespace std;

class CutCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::CutCmd"; }

  CutCmd() = default;

  QString getName() const override { return "cut"; }
  QString getDescription() const override { return "Cuts out a portion from a map"; }

  int runSimple(QStringList& args) override
  {
    bool crop = false;
    if (args.contains("--crop"))
    {
      crop = true;
      args.removeAt(args.indexOf("--crop"));
    }

    double buffer = 0.0;
    if (args.contains("--buffer"))
    {
      const int bufferIndex = args.indexOf("--buffer");
      bool ok = false;
      buffer = args.at(bufferIndex + 1).toDouble(&ok);
      if (!ok)
      {
        throw IllegalArgumentException("Invalid buffer value: " + args.at(bufferIndex));
      }
      args.removeAt(bufferIndex + 1);
      args.removeAt(bufferIndex);
    }

    LOG_VARD(args);

    if (args.size() != 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at three parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    const QString cutterShapePath = args[0].trimmed();
    const QString doughPath = args[1].trimmed();
    const QString outputPath = args[2].trimmed();

    QElapsedTimer timer;
    timer.start();

    LOG_STATUS(
      "Cutting ..." << FileUtils::toLogFormat(cutterShapePath, 25) << " out of ..." <<
      FileUtils::toLogFormat(doughPath, 25) << " and writing output to ..." <<
      FileUtils::toLogFormat(outputPath, 25) << "...");

    // Load up the shape being cut out.
    OsmMapPtr cutterShapeMap = std::make_shared<OsmMap>();
    IoUtils::loadMap(cutterShapeMap, cutterShapePath, true, Status::Unknown1);

    // Load up the dough.
    OsmMapPtr doughMap = std::make_shared<OsmMap>();
    IoUtils::loadMap(doughMap, doughPath, true, Status::Unknown1);

    // Cut the cutter shape out of the dough.
    CookieCutter(crop, buffer).cut(cutterShapeMap, doughMap);
    OsmMapPtr result = doughMap;

    MapProjector::projectToWgs84(result);
    IoUtils::saveMap(result, outputPath);

    LOG_STATUS(
      "Map cookie cut in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, CutCmd)

}
