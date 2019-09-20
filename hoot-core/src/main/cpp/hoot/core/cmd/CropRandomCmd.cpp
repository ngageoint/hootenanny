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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/ops/MapCropper.h>

namespace hoot
{

class CropRandomCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::CropRandomCmd"; }

  CropRandomCmd() {}

  virtual QString getName() const override { return "crop-random"; }

  virtual QString getDescription() const override
  { return "Crops out a random section of data"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() < 3 || args.size() > 4)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes three to four parameters.").arg(getName()));
    }

    const QString input = args[0].trimmed();
    const QString output = args[1].trimmed();
    bool ok = false;
    const int maxNodes = args[2].toLong(&ok);
    if (!ok || maxNodes < 1)
    {
      throw HootException("Invalid maximim node count: " + args[2]);
    }
    int randomSeed = -1;
    if (args.size() > 3)
    {
      bool ok = false;
      randomSeed = args[3].toLong(&ok);
      if (!ok || randomSeed < -1)
      {
        throw HootException("Invalid random seed: " + args[3]);
      }
    }

    if (!ConfigOptions().getCropBounds().trimmed().isEmpty())
    {
      LOG_WARN(
        getName() << " automatically calculates the crop bounds. The crop.bounds configuration " <<
        "option will be ignored.");
    }
    if (ConfigOptions().getCropInvert())
    {
      throw IllegalArgumentException(
        "The crop.invert configuration option is not supported by " + getName() + ".");
    }

    // read in all of the data
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, input);

    // compute tiles for the whole dataset, select one tile at random, and crop to the bounds of
    // the selected tile
    MapCropper cropper(
      GeometryUtils::getRandomTile(GeometryUtils::calculateTiles(maxNodes, 0.001, map), randomSeed));
    cropper.apply(map);
    OsmMapWriterFactory::write(map, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, CropRandomCmd)

}
