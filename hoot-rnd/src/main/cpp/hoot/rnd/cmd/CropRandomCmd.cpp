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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/rnd/ops/RandomMapCropper.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QFileInfo>
#include <QElapsedTimer>

namespace hoot
{

class CropRandomCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::CropRandomCmd"; }

  CropRandomCmd() = default;

  virtual QString getName() const override { return "crop-random"; }

  virtual QString getDescription() const override
  { return "Crops out a random section of a map (experimental)"; }

  virtual QString getType() const { return "rnd"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() < 4 || args.size() > 6)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes four to six parameters.").arg(getName()));
    }

    bool writeTileFootprints = false;
    if (args.contains("--write-tiles"))
    {
      writeTileFootprints = true;
      args.removeAll("--write-tiles");
    }
    LOG_VARD(writeTileFootprints);

    QStringList inputs;
    const QString input = args[0].trimmed();
    LOG_VARD(input);
    if (!input.contains(";"))
    {
      inputs.append(input);
    }
    else
    {
      //multiple inputs
      inputs = input.split(";");
    }
    LOG_VARD(inputs);

    const QString output = args[1].trimmed();
    LOG_VARD(output);

    bool ok = false;
    const int maxNodes = args[2].toLong(&ok);
    if (!ok || maxNodes < 1)
    {
      throw HootException("Invalid maximum node count: " + args[2]);
    }
    LOG_VARD(maxNodes);

    ok = false;
    double pixelSize = args[3].toDouble(&ok);
    if (!ok || pixelSize <= 0.0)
    {
      throw HootException("Invalid pixel size value: " + args[3]);
    }
    LOG_VARD(pixelSize);

    int randomSeed = -1;
    if (args.size() > 4)
    {
      ok = false;
      randomSeed = args[4].toLong(&ok);
      if (!ok || randomSeed < -1)
      {
        throw HootException("Invalid random seed: " + args[4]);
      }
    }
    LOG_VARD(randomSeed);

    QString tileOutputFootprintPath;
    if (writeTileFootprints)
    {
      QFileInfo outputFileInfo(output);
      tileOutputFootprintPath = output;
      tileOutputFootprintPath =
        tileOutputFootprintPath.replace(
          outputFileInfo.baseName() + "." + outputFileInfo.completeSuffix(),
          outputFileInfo.baseName() + "-tiles.osm");
    }
    LOG_VARD(tileOutputFootprintPath);

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

    OsmMapPtr map = _readInputs(inputs);

    RandomMapCropper cropper;
    cropper.setConfiguration(conf());
    cropper.setMaxNodeCount(maxNodes);
    cropper.setRandomSeed(randomSeed);
    cropper.setPixelSize(pixelSize);
    cropper.setTileFootprintOutputPath(tileOutputFootprintPath);
    LOG_INFO(cropper.getInitStatusMessage());
    cropper.apply(map);
    LOG_INFO(cropper.getCompletedStatusMessage());

    OsmMapWriterFactory::write(map, output);

    LOG_STATUS("Map cropped in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }

private:

  OsmMapPtr _readInputs(const QStringList& inputs)
  {
    OsmMapPtr map(new OsmMap());
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs.at(i);
      LOG_INFO("Reading " << input << "...");
      // tile alg expects inputs read in as Unknown1
      std::shared_ptr<OsmMapReader> reader =
        OsmMapReaderFactory::createReader(input, true, Status::Unknown1);

      reader->open(input);
      reader->read(map);
    }
    LOG_VARD(map->getNodeCount());

    OsmMapWriterFactory::writeDebugMap(map);

    return map;
  }
};

HOOT_FACTORY_REGISTER(Command, CropRandomCmd)

}
