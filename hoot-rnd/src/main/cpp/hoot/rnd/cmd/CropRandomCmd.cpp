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
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/rnd/ops/RandomMapCropper.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

namespace hoot
{

class CropRandomCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::CropRandomCmd"; }

  CropRandomCmd() {}

  virtual QString getName() const override { return "crop-random"; }

  virtual QString getDescription() const override
  { return "Crops out a random section of data based on a given node size (experimental)"; }

  virtual QString getType() const { return "rnd"; }

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
      throw HootException("Invalid maximum node count: " + args[2]);
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

    OsmMapPtr map(new OsmMap());
    // tile alg expects inputs read in as Unknown1
    OsmMapReaderFactory::read(map, input, true, Status::Unknown1);
    LOG_DEBUG("Starting map size: " << StringUtils::formatLargeNumber(map->size()));
    LOG_DEBUG(
      "Starting map bounds: " <<
      GeometryUtils::envelopeToConfigString(CalculateMapBoundsVisitor::getGeosBounds(map)));

    RandomMapCropper cropper;
    cropper.setConfiguration(conf());
    cropper.setMaxNodeCount(maxNodes);
    cropper.setRandomSeed(randomSeed);
    LOG_INFO(cropper.getInitStatusMessage());
    cropper.apply(map);
    LOG_INFO(cropper.getCompletedStatusMessage());
    LOG_DEBUG("Cropped map size: " << StringUtils::formatLargeNumber(map->size()));
    LOG_DEBUG(
      "Cropped map bounds: " <<
      GeometryUtils::envelopeToConfigString(CalculateMapBoundsVisitor::getGeosBounds(map)));

    OsmMapWriterFactory::write(map, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, CropRandomCmd)

}
