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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/io/TileBoundsWriter.h>

namespace hoot
{

class NodeDensityTilesCmd : public BaseCommand
{

public:

  static std::string className() { return "hoot::NodeDensityTilesCmd"; }

  NodeDensityTilesCmd() { }

  virtual QString getName() const override { return "node-density-tiles"; }

  virtual QString getDescription() const override
  { return "Calculates bounding box partitions based on a map's node density"; }

  virtual int runSimple(QStringList& args) override
  {
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
        "Invalid output file format: " + output + ".  Only the GeoJSON (.geojson) and OSM " +
        "(.osm) output formats are supported.");
    }
    LOG_VARD(output);

    //if either of max nodes per tile or pixel size is specified, then both must be specified

    long maxNodesPerTile = 1000;
    if (args.size() > 2)
    {
      bool parseSuccess = false;
      maxNodesPerTile = args[2].toLong(&parseSuccess);
      if (!parseSuccess || maxNodesPerTile < 1)
      {
        throw IllegalArgumentException("Invalid maximum nodes per tile value: " + args[2]);
      }
    }
    LOG_VARD(maxNodesPerTile);

    double pixelSize = 0.001; // .1km?
    if (args.size() > 2)
    {
      bool parseSuccess = false;
      pixelSize = args[3].toDouble(&parseSuccess);
      if (!parseSuccess || pixelSize <= 0.0)
      {
        throw IllegalArgumentException("Invalid pixel size value: " + args[3]);
      }
    }
    LOG_VARD(pixelSize);

    int randomSeed = -1;
    if (args.contains("--random") && args.size() > 4)
    {
      bool parseSuccess = false;
      randomSeed = args[4].toInt(&parseSuccess);
      if (!parseSuccess || randomSeed < -1)
      {
        throw IllegalArgumentException("Invalid random seed value: " + args[4]);
      }
    }
    LOG_VARD(randomSeed);

    conf().set(ConfigOptions().getIdGeneratorKey(), "hoot::PositiveIdGenerator");

    OsmMapPtr inputMap = _readInputs(inputs);

    NodeDensityTileBoundsCalculator tileCalc;
    tileCalc.setPixelSize(pixelSize);
    tileCalc.setMaxNodesPerTile(maxNodesPerTile);
    //tileCalc.setSlop(0.1); // TODO: tweak this?
    tileCalc.setMaxNodePerTileIncreaseFactor(10);
    tileCalc.setMaxNumTries(1); // TODO: change
    tileCalc.setPixelSizeRetryReductionFactor(10.0);
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

    LOG_INFO("Number of calculated tiles: " << tiles.size());
    LOG_INFO("Maximum node count in a single tile: " << tileCalc.getMaxNodeCountInOneTile());
    LOG_INFO("Mininum node count in a single tile: " << tileCalc.getMinNodeCountInOneTile());
    LOG_INFO("Pixel size used: " << tileCalc.getPixelSize());
    LOG_INFO("Maximum node count per tile used: " << tileCalc.getMaxNodesPerTile());

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
        //the tiles calculation is only concerned with nodes, and the only readers capable of
        //filtering down to nodes up front right now are the api db readers; more importantly,
        //setting this to true also prevents any features from being returned outside of
        //convert.bounding.box, if it was specified (ways partially inside the bounds, etc.)
        apiDbReader->setReturnNodesOnly(true);
      }

      reader->open(input);
      reader->read(map);
    }
    LOG_VARD(map->getNodeCount());

//    OGREnvelope envelope = CalculateMapBoundsVisitor::getBounds(map);
//    std::shared_ptr<geos::geom::Envelope> tempEnv(GeometryUtils::toEnvelope(envelope));
//    LOG_VARD(tempEnv->toString());
    OsmMapWriterFactory::writeDebugMap(map);

    return map;
  }
};

HOOT_FACTORY_REGISTER(Command, NodeDensityTilesCmd)

}
