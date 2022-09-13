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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RandomMapCropper.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class CropRandomCmd : public BaseCommand
{
public:

  static QString className() { return "CropRandomCmd"; }

  CropRandomCmd() = default;

  QString getName() const override { return "crop-random"; }
  QString getDescription() const override
  { return "Crops out a random section of a map (experimental)"; }
  QString getType() const override { return "advanced"; }

  int runSimple(QStringList& args) override
  {
    bool writeTileFootprints = false;
    if (args.contains("--write-tiles"))
    {
      writeTileFootprints = true;
      args.removeAll("--write-tiles");
    }

    bool ok = false;
    int randomSeed = -1;
    if (args.contains("--randomSeed"))
    {
      const int randomSeedIndex = args.indexOf("--randomSeed");
      ok = false;
      randomSeed = args[randomSeedIndex + 1].toInt(&ok);
      if (!ok || randomSeed < -1)
        throw HootException("Invalid random seed: " + args[randomSeedIndex]);
      args.removeAt(randomSeedIndex + 1);
      args.removeAt(randomSeedIndex);
    }

    if (args.size() < 4)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least four parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    QElapsedTimer timer;
    timer.start();

    const int outputIndex = args.size() - 3;
    const QString output = args[outputIndex].trimmed();
    args.removeAt(outputIndex);

    ok = false;
    const int maxNodesIndex = args.size() - 2;
    const int maxNodes = args[maxNodesIndex].toInt(&ok);
    if (!ok || maxNodes < 1)
      throw HootException("Invalid maximum node count: " + args[maxNodesIndex]);
    args.removeAt(maxNodesIndex);

    ok = false;
    const int pixelSizeIndex = args.size() - 1;
    double pixelSize = args[pixelSizeIndex].toDouble(&ok);
    if (!ok || pixelSize <= 0.0)
      throw HootException("Invalid pixel size value: " + args[pixelSizeIndex]);
    args.removeAt(pixelSizeIndex);

    // Everything left is an input.
    const QStringList inputs = IoUtils::expandInputs(args);

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

    if (!ConfigOptions().getCropBounds().trimmed().isEmpty())
    {
      LOG_WARN(
        getName() << " automatically calculates the crop bounds. The crop.bounds configuration " <<
        "option will be ignored.");
    }
    if (ConfigOptions().getCropInvert())
      throw IllegalArgumentException("The crop.invert configuration option is not supported by " + getName() + ".");

    LOG_STATUS(
      "Randomly cropping " << inputs.size() << " inputs and writing output to ..." <<
      FileUtils::toLogFormat(output, 25) << "...");

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

  OsmMapPtr _readInputs(const QStringList& inputs) const
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    for (const auto& input : qAsConst(inputs))
    {
      // tile alg expects inputs read in as Unknown1
      std::shared_ptr<OsmMapReader> reader = OsmMapReaderFactory::createReader(input, true, Status::Unknown1);

      reader->open(input);
      reader->read(map);
    }
    LOG_VARD(map->getNodeCount());

    OsmMapWriterFactory::writeDebugMap(map, className(), "");

    return map;
  }
};

HOOT_FACTORY_REGISTER(Command, CropRandomCmd)

}
