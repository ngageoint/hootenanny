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
 * @copyright Copyright (C) 2012, 2013, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QStringList>
#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class CropCmd : public BoundedCommand
{
public:

  static QString className() { return "hoot::CropCmd"; }

  CropCmd() = default;

  QString getName() const override { return "crop"; }
  QString getDescription() const override { return "Crops a map to a bounds"; }

  int runSimple(QStringList& args) override
  {      
    if (args.size() < 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at least three parameters.").arg(getName()));
    }

    int boundsIndex;
    if (args.contains("--write-bounds"))
    {
      boundsIndex = args.size() - 2;
    }
    else
    {
      boundsIndex = args.size() - 1;
    }
    _env = GeometryUtils::boundsFromString(args.at(boundsIndex));
    args.removeAt(boundsIndex);
    // This has to be done after we get the envelope.
    BoundedCommand::runSimple(args);

    const QString output = args.last();
    args.removeLast();

    // Everything left is an input.
    const QStringList inputs = args;

    QElapsedTimer timer;
    timer.start();

    LOG_STATUS(
      "Cropping ..." << FileUtils::toLogFormat(inputs, 25) << " and writing output to ..." <<
      FileUtils::toLogFormat(output, 25) << "...");

    OsmMapPtr map(new OsmMap());
    if (inputs.size() == 1)
    {
      IoUtils::loadMap(map, inputs.at(0), true);
    }
    else
    {
      // Avoid ID conflicts across multiple inputs.
      IoUtils::loadMaps(map, inputs, false);
    }

    MapCropper cropper;
    cropper.setBounds(_env);
    cropper.setConfiguration(Settings::getInstance());
    cropper.apply(map);

    IoUtils::saveMap(map, output);

    LOG_STATUS("Map cropped in: " << StringUtils::millisecondsToDhms(timer.elapsed()) + " total.");

    return 0;
  }

private:

  std::shared_ptr<geos::geom::Geometry> _env;

  void _writeBoundsFile() override
  {
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(*(_env->getEnvelopeInternal())),
      ConfigOptions().getBoundsOutputFile());
  }
};

HOOT_FACTORY_REGISTER(Command, CropCmd)

}
