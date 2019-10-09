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

#include "TileUtils.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/tile/TileBoundsCalculator.h>
#include <hoot/core/util/RandomNumberUtils.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/TagRenameKeyVisitor.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>

// Tgs
#include <tgs/Statistics/Random.h>

// Qt
#include <QTemporaryFile>

namespace hoot
{

std::vector<std::vector<geos::geom::Envelope>> TileUtils::calculateTiles(
  const long maxNodesPerTile, const double pixelSize, OsmMapPtr map, long& minNodeCountInOneTile,
  long& maxNodeCountInOneTile, std::vector<std::vector<long>>& nodeCounts)
{
  TileBoundsCalculator tileBoundsCalculator(pixelSize);
  tileBoundsCalculator.setMaxNodesPerBox(maxNodesPerTile);
  //tbc.setSlop(0.1);
  cv::Mat r1, r2;
  tileBoundsCalculator.renderImage(map, r1, r2);
  //we're calculating for unknown1 only, so fill the second matrix with all zeroes
  cv::Mat zeros = cv::Mat::zeros(r1.size(), r1.type());
  tileBoundsCalculator.setImages(r1, zeros);

  const std::vector<std::vector<geos::geom::Envelope>> tiles =
    tileBoundsCalculator.calculateTiles();

  minNodeCountInOneTile = tileBoundsCalculator.getMinNodeCountInOneTile();
  maxNodeCountInOneTile = tileBoundsCalculator.getMaxNodeCountInOneTile();
  nodeCounts = tileBoundsCalculator.getNodeCounts();

  return tiles;
}

int TileUtils::getRandomTileIndex(const std::vector<std::vector<geos::geom::Envelope>>& tiles,
                                  int randomSeed)
{
  if (tiles.size() == 0)
  {
    throw IllegalArgumentException("No tiles passed to TileUtils::getRandomTileIndex.");
  }
  if (randomSeed == -1)
  {
    randomSeed = RandomNumberUtils::generateSeed();
  }
  LOG_VARD(tiles.size());
  LOG_VARD(randomSeed);
  Tgs::Random::instance()->seed(randomSeed);

  const size_t numBboxes = tiles.size() * tiles[0].size();
  LOG_VARD(numBboxes);
  return Tgs::Random::instance()->generateInt(numBboxes);
}

geos::geom::Envelope TileUtils::getRandomTile(
  const std::vector<std::vector<geos::geom::Envelope>>& tiles, int randomSeed)
{
  const int randomTileIndex = getRandomTileIndex(tiles, randomSeed);
  LOG_VARD(randomTileIndex);
  int bboxCtr = 1;
  for (size_t tx = 0; tx < tiles.size(); tx++)
  {
    for (size_t ty = 0; ty < tiles[tx].size(); ty++)
    {
      if ((bboxCtr - 1) == randomTileIndex)
      {
        LOG_DEBUG("Randomly selected tile: " << GeometryUtils::toConfigString(tiles[tx][ty]));
        return tiles[tx][ty];
      }
      bboxCtr++;
    }
  }
  // shouldn't ever get here
  return geos::geom::Envelope();
}

void TileUtils::writeTilesToGeoJson(
  const std::vector<std::vector<geos::geom::Envelope>>& tiles,
  const std::vector<std::vector<long>>& nodeCounts, const QString& outputPath,
  const bool selectSingleRandomTile, int randomSeed)
{
  // write out to temp osm and then use ogr2ogr to convert to geojson; trying to remember, but I
  // believe our geojson writer wouldn't quite give the geometry output format needed for this,
  // which is why ogr2ogr is used here...but that's worth verifying now.

  QTemporaryFile osmTempFile("/tmp/tiles-calculate-temp-XXXXXX.osm");
  if (!osmTempFile.open())
  {
    throw HootException(
      "Unable to open OSM temp file: " + osmTempFile.fileName() + " for GeoJSON output.");
  }
  LOG_VARD(osmTempFile.fileName());
  writeTilesToOsm(tiles, nodeCounts, osmTempFile.fileName(), selectSingleRandomTile, randomSeed);

  // hack - To get around having to modify osmconf.ini, hijacking the "amenity" field here since its
  // supported in the default configuration. Doing this allows the ogr2ogr select to bring back the
  // node counts. We can get away with this b/c there's no actual element data other than the tiles
  // in this file. If we decide we want to modify the default osmconf.ini as part of the build
  // process, then all of this "amenity" related logic can go away. These text replacements are
  // inefficient, but the tile files aren't generally going to get huge. Also our json I/O isn't
  // streamable, so it wouldn't be any more efficient, even it it could be used for this.
  FileUtils::replaceFully(
    osmTempFile.fileName(), QStringList("node_count"), QStringList("amenity"));
//  OsmMapPtr tempMap(new OsmMap());
//  OsmMapReaderFactory::read(tempMap, true, true, osmTempFile.fileName());
//  TagRenameKeyVisitor keyVis1("node_count", "amenity");
//  tempMap->visitWaysRw(keyVis1);
//  OsmMapWriterFactory::write(tempMap, osmTempFile.fileName());

  QFile outFile(outputPath);
  if (outFile.exists() && !outFile.remove())
  {
    throw HootException("Unable to open GeoJSON file: " + outputPath + " for writing.");
  }
  LOG_VARD(outputPath);

  // exporting as multipolygons, as that's what the Tasking Manager expects; The fields available
  // to select are configured in osmconf.ini. See note about "amenity" above.
//  const QString cmd =
//    "ogr2ogr -f GeoJSON -select \"name,boundary,amenity,osm_way_id\" " + outputPath + " " +
//    osmTempFile.fileName() + " multipolygons";
  const QString cmd =
    "ogr2ogr -f GeoJSON -select \"name,amenity,osm_way_id\" " + outputPath + " " +
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

  // see note above about amenity
  QStringList textsToReplace;
  textsToReplace.append("amenity");
  // for this one, just wanted to see it as "task" instead of "osm_way_id"; alternatively could have
  // hacked another ogr field for this, but not really gaining anything by doing that
  textsToReplace.append("osm_way_id");
  QStringList replacementTexts;
  replacementTexts.append("node_count");
  replacementTexts.append("task");
  FileUtils::replaceFully(outputPath, textsToReplace, replacementTexts);
//  tempMap.reset(new OsmMap());
//  OsmMapReaderFactory::read(tempMap, true, true, outputPath);
//  TagRenameKeyVisitor keyVis2("amenity", "node_count");
//  tempMap->visitWaysRw(keyVis2);
//  keyVis2.setOldKey("osm_way_id");
//  keyVis2.setNewKey("task");
//  tempMap->visitWaysRw(keyVis2);
//  RemoveTagsVisitor removeVis(QStringList("boundary"));
//  tempMap->visitWaysRw(removeVis);
//  OsmMapWriterFactory::write(tempMap, outputPath);
}

void TileUtils::writeTilesToOsm(
  const std::vector<std::vector<geos::geom::Envelope>>& tiles,
  const std::vector<std::vector<long>>& nodeCounts, const QString& outputPath,
  const bool selectSingleRandomTile, int randomSeed)
{
  LOG_VARD(outputPath);

  int randomTileIndex = -1;
  if (selectSingleRandomTile)
  {
    randomTileIndex = TileUtils::getRandomTileIndex(tiles, randomSeed);
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

      // If you want any tags added here to be seen in the geojson output, update the select
      // statement in writeTilesToGeoJson, if the field is already supported in osmconf.ini OR if
      // its not supported in osmconfi.ini, add custom handling as a workaround (see related note
      // in writeTilesToGeoJson).

      // gdal will recognize any closed way with the boundary tag as a polygon (tags
      // for features recognized as polys configurable in osmconf.ini), which is the type of
      // output we want
      bbox->setTag("boundary", "task_grid_cell");
      //const QString tileName = "Task Grid Cell #" + QString::number(bboxCtr);
      //bbox->setTag("name", tileName);
      const long nodeCount = nodeCounts[tx][ty];
      bbox->setTag("node_count", QString::number(nodeCount));

      if (!selectSingleRandomTile ||
          (selectSingleRandomTile && (bboxCtr - 1) == randomTileIndex))
      {
        boundaryMap->addWay(bbox);
      }
      bboxCtr++;
    }
  }

  OsmMapWriterFactory::write(boundaryMap, outputPath);
  OsmMapWriterFactory::writeDebugMap(boundaryMap, "osm-tiles");
}

}
