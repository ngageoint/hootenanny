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
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class ExtentCmd : public BaseCommand
{

public:

  static QString className() { return "hoot::ExtentCmd"; }

  ExtentCmd() = default;

  QString getName() const override { return "extent"; }
  QString getDescription() const override { return "Calculates the bounds of a map"; }

  int runSimple(QStringList& args) override
  {
    bool recursive = false;
    const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);

    if (args.size() < 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least one parameter. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    conf().set(ConfigOptions::getWriterPrecisionKey(), 9);

    QStringList inputs;
    if (!recursive)
    {
      inputs = args;
    }
    else
    {
      inputs = IoUtils::getSupportedInputsRecursively(args, inputFilters);
    }

    QElapsedTimer timer;
    timer.start();

    LOG_STATUS("Calculating extent for ..." << FileUtils::toLogFormat(inputs, 25) << "...");

    OsmMapPtr map = std::make_shared<OsmMap>();
    IoUtils::loadMaps(map, inputs, false, Status::Invalid);

    const QString bounds =
      GeometryUtils::envelopeToString(CalculateMapBoundsVisitor::getGeosBounds(map));
    std::cout << "Map extent (minx,miny,maxx,maxy): " << bounds << std::endl;

    LOG_STATUS(
      "Map extent calculated in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ExtentCmd)

}
