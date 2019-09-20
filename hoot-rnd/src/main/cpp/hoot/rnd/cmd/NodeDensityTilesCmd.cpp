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
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QTemporaryFile>

namespace hoot
{

class NodeDensityTilesCmd : public BaseCommand
{

public:

  static std::string className() { return "hoot::NodeDensityTilesCmd"; }

  NodeDensityTilesCmd() { }

  virtual QString getName() const override { return "node-density-tiles"; }

  virtual QString getDescription() const override
  { return "Calculates geospatial bounding box partitions based on a map's node density (experimental) "; }

  virtual QString getType() const override { return "rnd"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() < 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(
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
      //multiple inputs
      inputs = input.split(";");
    }
    LOG_VARD(inputs);

    const QString output = args[1];
    if (!output.toLower().endsWith(".geojson"))
    {
      throw HootException(
        "Invalid output file format: " + output + ".  Only the GeoJSON output format is " +
        "supported.");
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
        throw HootException("Invalid maximum nodes per tile value: " + args[2]);
      }
    }
    LOG_VARD(maxNodesPerTile);

    double pixelSize = 0.001; //.1km?
    if (args.size() > 2)
    {
      bool parseSuccess = false;
      pixelSize = args[3].toDouble(&parseSuccess);
      if (!parseSuccess || pixelSize <= 0.0)
      {
        throw HootException("Invalid pixel size value: " + args[3]);
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
        throw HootException("Invalid random seed value: " + args[4]);
      }
    }
    LOG_VARD(randomSeed);

    conf().set(ConfigOptions().getIdGeneratorKey(), "hoot::PositiveIdGenerator");

    OsmMapPtr inputMap = _readInputs(inputs);
    const std::vector<std::vector<geos::geom::Envelope>> tiles =
      GeometryUtils::calculateTiles(maxNodesPerTile, pixelSize, inputMap);
    _writeOutputAsGeoJson(tiles, output, args.contains("--random"), randomSeed);

    return 0;
  }

private:

  OsmMapPtr _readInputs(const QStringList inputs)
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

  void _writeOutputAsGeoJson(const std::vector<std::vector<geos::geom::Envelope>>& tiles,
                             const QString& outputPath, const bool selectSingleRandomTile,
                             int randomSeed)
  {
    //write out to temp osm and then use ogr2ogr to convert to geojson

    QTemporaryFile osmTempFile("tmp/tiles-calculate-temp-XXXXXX.osm");
    if (!osmTempFile.open())
    {
      throw HootException(
        "Unable to open OSM temp file: " + osmTempFile.fileName() + " for GeoJSON output.");
    }
    LOG_VARD(osmTempFile.fileName());
    _writeOutputAsOsm(tiles, osmTempFile.fileName(), selectSingleRandomTile, randomSeed);

    QFile outFile(outputPath);
    if (outFile.exists() && !outFile.remove())
    {
      throw HootException("Unable to open GeoJSON file: " + outputPath + " for writing.");
    }
    LOG_VARD(outputPath);

    //exporting as multipolygons, as that's what the Tasking Manager expects
    const QString cmd =
      "ogr2ogr -f GeoJSON -select \"name,boundary,osm_way_id\" " + outputPath + " " +
      osmTempFile.fileName() + " multipolygons";
    LOG_VARD(cmd);
    LOG_INFO("Writing output to " << outputPath);
    const int retval = std::system(cmd.toStdString().c_str());
    if (retval != 0)
    {
      throw HootException(
        "Failed converting " + osmTempFile.fileName() + " to GeoJSON: " + outputPath +
        ".  Status: " + QString::number(retval));
    }
  }

  void _writeOutputAsOsm(const std::vector<std::vector<geos::geom::Envelope>>& tiles,
                         const QString& outputPath, const bool selectSingleRandomTile,
                         int randomSeed)
  {
    LOG_VARD(outputPath);

    int randomTileIndex = -1;
    if (selectSingleRandomTile)
    {
      randomTileIndex = GeometryUtils::getRandomTileIndex(tiles, randomSeed);
    }

    OsmMapPtr boundaryMap(new OsmMap());
    int bboxCtr = 1;
    for (size_t tx = 0; tx < tiles.size(); tx++)
    {
      for (size_t ty = 0; ty < tiles[tx].size(); ty++)
      {
        // TODO: This code could possibly be replaced by GeometryUtils::createMapFromBounds.
        const geos::geom::Envelope env = tiles[tx][ty];
        const double circularError = ConfigOptions().getCircularErrorDefaultValue();

        NodePtr lowerLeft(
          new Node(
            Status::Unknown1,
            boundaryMap->createNextNodeId(),
            geos::geom::Coordinate(env.getMinX(), env.getMinY()),
            circularError));
        boundaryMap->addNode(lowerLeft);
        NodePtr upperRight(
          new Node(
            Status::Unknown1,
            boundaryMap->createNextNodeId(),
            geos::geom::Coordinate(env.getMaxX(), env.getMaxY()),
            circularError));
        boundaryMap->addNode(upperRight);
        NodePtr upperLeft(
          new Node(
            Status::Unknown1,
            boundaryMap->createNextNodeId(),
            geos::geom::Coordinate(env.getMinX(), env.getMaxY()),
            circularError));
        boundaryMap->addNode(upperLeft);
        NodePtr lowerRight(
          new Node(
            Status::Unknown1,
            boundaryMap->createNextNodeId(),
            geos::geom::Coordinate(env.getMaxX(), env.getMinY()),
            circularError));
        boundaryMap->addNode(lowerRight);

        WayPtr bbox(new Way(Status::Unknown1, boundaryMap->createNextWayId(), circularError));
        bbox->addNode(lowerLeft->getId());
        bbox->addNode(upperLeft->getId());
        bbox->addNode(upperRight->getId());
        bbox->addNode(lowerRight->getId());
        bbox->addNode(lowerLeft->getId());
        //gdal will recognize any closed way with the boundary tag as a polygon (tags
        //for features recognized as polys configurable in osmconf.ini), which is the type of
        //output we want
        bbox->setTag("boundary", "task_grid_cell");
        bbox->setTag("name", "Task Grid Cell #" + QString::number(bboxCtr));
        if (!selectSingleRandomTile ||
            (selectSingleRandomTile && (bboxCtr - 1) == randomTileIndex))
        {
          boundaryMap->addWay(bbox);
        }
        bboxCtr++;
      }
    }

    OsmMapWriterFactory::write(boundaryMap, outputPath);

  //      if (Log::getInstance().getLevel() <= Log::Debug)
  //      {
  //        const QString debugOutputPath = "tmp/calc-tiles-osm-debug.osm";
  //        QFile outFile(outputPath);
  //        LOG_DEBUG("writing debug output to " << debugOutputPath);
  //        outFile.copy(debugOutputPath);
  //      }
  }
};

HOOT_FACTORY_REGISTER(Command, NodeDensityTilesCmd)

}
