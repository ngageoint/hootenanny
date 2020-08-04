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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/ApiDbReader.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/tile/NodeDensityTileBoundsCalculator.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/TileBoundsWriter.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class NodeDensityTilesCmd : public BaseCommand
{

public:

  static std::string className() { return "hoot::NodeDensityTilesCmd"; }

  NodeDensityTilesCmd() = default;

  virtual QString getName() const override { return "node-density-tiles"; }

  virtual QString getDescription() const override
  { return "Calculates bounding box partitions based on a map's node density"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() < 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least two parameters.").arg(getName()));
    }

    QStringList inputs;
    const QString input = args[0];
    LOG_VARD(input);
    if (!input.contains(";"))
    {
      inputs.append(input);
    }
    else
    {
      // multiple inputs
      inputs = input.split(";");
    }
    LOG_VARD(inputs);

    const QString output = args[1];
    if (!output.toLower().endsWith(".geojson") && !output.toLower().endsWith(".osm"))
    {
      throw IllegalArgumentException(
        "Invalid output file format: " + output + ". Only the GeoJSON (.geojson) and OSM " +
        "(.osm) output formats are supported.");
    }
    LOG_VARD(output);

    // optional parameters

    int maxNodesPerTile = 1000;
    if (args.contains("--maxNodesPerTile"))
    {
      const int optionNameIndex = args.indexOf("--maxNodesPerTile");
      bool parseSuccess = false;
      const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
      maxNodesPerTile = optionStrVal.toInt(&parseSuccess);
      if (!parseSuccess || maxNodesPerTile < 1)
      {
        throw IllegalArgumentException(
          "Invalid maximum node count per tile value: " + optionStrVal);
      }
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(maxNodesPerTile);

    double pixelSize = 0.001; // .1km?
    if (args.contains("--pixel-size"))
    {
      const int optionNameIndex = args.indexOf("--pixel-size");
      bool parseSuccess = false;
      const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
      pixelSize = optionStrVal.toDouble(&parseSuccess);
      if (!parseSuccess || pixelSize <= 0.0)
      {
        throw IllegalArgumentException("Invalid pixel size value: " + optionStrVal);
      }
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(pixelSize);

    int maxAttempts = 3;
    if (args.contains("--maxAttempts"))
    {
      const int optionNameIndex = args.indexOf("--maxAttempts");
      bool parseSuccess = false;
      const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
      maxAttempts = optionStrVal.toInt(&parseSuccess);
      if (!parseSuccess || maxAttempts < 1)
      {
        throw IllegalArgumentException("Invalid maximum attempts value: " + optionStrVal);
      }
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(maxAttempts);

    int maxTimePerAttempt = -1;
    if (args.contains("--maxTimePerAttempt"))
    {
      const int optionNameIndex = args.indexOf("--maxTimePerAttempt");
      bool parseSuccess = false;
      const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
      maxTimePerAttempt = optionStrVal.toInt(&parseSuccess);
      if (!parseSuccess || maxTimePerAttempt < -1)
      {
        throw IllegalArgumentException("Invalid maximum time per attempt value: " + optionStrVal);
      }
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(maxTimePerAttempt);

    int pixelSizeAutoReductionFactor = 10;
    if (args.contains("--pixelSizeReductionFactor"))
    {
      const int optionNameIndex = args.indexOf("--pixelSizeReductionFactor");
      bool parseSuccess = false;
      const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
      pixelSizeAutoReductionFactor = optionStrVal.toInt(&parseSuccess);
      if (!parseSuccess || pixelSizeAutoReductionFactor < 1)
      {
        throw IllegalArgumentException(
          "Invalid pixel size automatic reduction factor value: " + optionStrVal);
      }
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(pixelSizeAutoReductionFactor);

    int randomSeed = -1;
    if (args.contains("--random") && args.contains("--randomSeed"))
    { 
      const int optionNameIndex = args.indexOf("--randomSeed");
      bool parseSuccess = false;
      const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
      randomSeed = optionStrVal.toInt(&parseSuccess);
      if (!parseSuccess || randomSeed < -1)
      {
        throw IllegalArgumentException("Invalid random seed value: " + optionStrVal);
      }
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    LOG_VARD(randomSeed);

    conf().set(ConfigOptions().getIdGeneratorKey(), "hoot::PositiveIdGenerator");

    OsmMapPtr inputMap = _readInputs(inputs);

    NodeDensityTileBoundsCalculator tileCalc;
    tileCalc.setPixelSize(pixelSize);
    tileCalc.setMaxNodesPerTile(maxNodesPerTile);
    //tileCalc.setSlop(0.1); // tweak this?
    tileCalc.setMaxNumTries(maxAttempts);
    tileCalc.setMaxTimePerAttempt(maxTimePerAttempt);
    tileCalc.setPixelSizeRetryReductionFactor(pixelSizeAutoReductionFactor);
    tileCalc.calculateTiles(inputMap);
    const std::vector<std::vector<geos::geom::Envelope>> tiles = tileCalc.getTiles();
    const std::vector<std::vector<long>> nodeCounts = tileCalc.getNodeCounts();

    if (output.toLower().endsWith(".geojson"))
    {
      TileBoundsWriter::writeTilesToGeoJson(
        tiles, nodeCounts, output, inputMap->getSource(), args.contains("--random"), randomSeed);
    }
    else
    {
      TileBoundsWriter::writeTilesToOsm(
        tiles, nodeCounts, output, args.contains("--random"), randomSeed);
    }

    LOG_STATUS(
      "Node density tiles calculated in " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
    LOG_STATUS("Number of calculated tiles: " << StringUtils::formatLargeNumber(tiles.size()));
    LOG_STATUS(
      "Maximum node count in a single tile: " <<
      StringUtils::formatLargeNumber(tileCalc.getMaxNodeCountInOneTile()));
    LOG_STATUS(
      "Mininum node count in a single tile: " <<
      StringUtils::formatLargeNumber(tileCalc.getMinNodeCountInOneTile()));
    LOG_STATUS("Pixel size used: " << tileCalc.getPixelSize());
    LOG_STATUS(
      "Maximum node count per tile used: " <<
      StringUtils::formatLargeNumber(tileCalc.getMaxNodesPerTile()));

    return 0;
  }

private:

  OsmMapPtr _readInputs(const QStringList& inputs)
  {
    const bool bboxSpecified =
      !ConfigOptions().getConvertBoundingBox().trimmed().isEmpty() ||
      !ConfigOptions().getConvertBoundingBoxHootApiDatabase().trimmed().isEmpty() ||
      !ConfigOptions().getConvertBoundingBoxOsmApiDatabase().trimmed().isEmpty();

    OsmMapPtr map(new OsmMap());
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs.at(i);
      // IMPORTANT: has to be loaded as Unknown1 in order for the calculation to work
      std::shared_ptr<OsmMapReader> reader =
        OsmMapReaderFactory::createReader(input, true, Status::Unknown1);

      std::shared_ptr<Boundable> boundable = std::dynamic_pointer_cast<Boundable>(reader);
      if (bboxSpecified && !boundable)
      {
        // TODO: need a test for this
        throw IllegalArgumentException(
          "Reader for " + input + " must implement Boundable when the " +
          "convert.bounding.box configuration option is specified.");
      }

      std::shared_ptr<ApiDbReader> apiDbReader =
        std::dynamic_pointer_cast<ApiDbReader>(reader);
      if (apiDbReader)
      {
        // The tiles calculation is only concerned with nodes, and the only readers capable of
        // filtering down to nodes up front right now are the api db readers. More importantly,
        // setting this to true also prevents any features from being returned outside of
        // convert.bounding.box, if it was specified (ways partially inside the bounds, etc.)
        apiDbReader->setReturnNodesOnly(true);
      }

      reader->open(input);
      reader->read(map);
    }
    LOG_VARD(map->getNodeCount());

    OsmMapWriterFactory::writeDebugMap(map);

    return map;
  }
};

HOOT_FACTORY_REGISTER(Command, NodeDensityTilesCmd)

}
