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
 * @copyright Copyright (C) 2012, 2013, 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>

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

  static string className() { return "hoot::CropCmd"; }

  CropCmd() {}

  virtual QString getName() const override { return "crop"; }

  virtual QString getDescription() const override { return "Crops a map to a bounds"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() < 3 || args.size() > 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three or four parameters.").arg(getName()));
    }

    QElapsedTimer timer;
    timer.start();
    QString in = args[0];
    QString out = args[1];
    _env.reset(new Envelope(GeometryUtils::envelopeFromConfigString(args[2])));

    BoundedCommand::runSimple(args);

    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, in, true);

    MapCropper cropper(*_env);
    cropper.setConfiguration(Settings::getInstance());
    cropper.apply(map);

    IoUtils::saveMap(map, out);

    LOG_INFO("Cropping completed in: " + StringUtils::millisecondsToDhms(timer.elapsed()) + ".");

    return 0;
  }

protected:

  std::shared_ptr<Envelope> _env;

  virtual void _writeBoundsFile() override
  {
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(*_env), ConfigOptions().getBoundsOutputFile());
  }
};

HOOT_FACTORY_REGISTER(Command, CropCmd)

}
