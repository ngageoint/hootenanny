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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "TileBoundsWriter.h"

// hoot
#include <hoot/core/io/OsmGeoJsonWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/conflate/tile/TileUtils.h>

namespace hoot
{

void TileBoundsWriter::writeTilesToGeoJson(
  const std::vector<std::vector<geos::geom::Envelope>>& tiles,
  const std::vector<std::vector<long>>& nodeCounts, const QString& outputPath,
  const QString& fileSource, const bool selectSingleRandomTile, int randomSeed)
{
  LOG_VARD(outputPath);

  int randomTileIndex = -1;
  if (selectSingleRandomTile)
    randomTileIndex = TileUtils::getRandomTileIndex(tiles, randomSeed);

  OsmMapPtr boundaryMap =
    _tilesToOsmMap(tiles, nodeCounts, randomTileIndex, selectSingleRandomTile);
  boundaryMap->appendSource(fileSource);

  OsmGeoJsonWriter writer;
  Settings s;
  //  Output the source tags in the geojson writer
  s.set(ConfigOptions::getJsonOutputTaskingManagerAoiKey(), true);
  s.set(ConfigOptions::getWriterPrecisionKey(), 9);
  writer.setConfiguration(s);
  writer.open(outputPath);
  writer.write(boundaryMap);

  OsmMapWriterFactory::writeDebugMap(boundaryMap, "osm-tiles");
}

void TileBoundsWriter::writeTilesToOsm(
  const std::vector<std::vector<geos::geom::Envelope>>& tiles,
  const std::vector<std::vector<long>>& nodeCounts, const QString& outputPath,
  const bool selectSingleRandomTile, int randomSeed)
{
  LOG_VARD(outputPath);

  int randomTileIndex = -1;
  if (selectSingleRandomTile)
    randomTileIndex = TileUtils::getRandomTileIndex(tiles, randomSeed);

  OsmMapPtr boundaryMap =
    _tilesToOsmMap(tiles, nodeCounts, randomTileIndex, selectSingleRandomTile);

  OsmMapWriterFactory::write(boundaryMap, outputPath);
  OsmMapWriterFactory::writeDebugMap(boundaryMap, "osm-tiles");
}

OsmMapPtr TileBoundsWriter::_tilesToOsmMap(
  const std::vector<std::vector<geos::geom::Envelope>>& tiles,
  const std::vector<std::vector<long>>& nodeCounts,
  int randomTileIndex, const bool selectSingleRandomTile)
{
  OsmMapPtr boundaryMap(new OsmMap());
  int bboxCtr = 1;
  for (size_t tx = 0; tx < tiles.size(); tx++)
  {
    for (size_t ty = 0; ty < tiles[tx].size(); ty++)
    {
      //  Only create the bounding tile if we want all of them or
      //  if this is the randomly selected tile
      if (!selectSingleRandomTile ||
          (selectSingleRandomTile && (bboxCtr - 1) == randomTileIndex))
      {
        //  Create the way in the map
        const geos::geom::Envelope env = tiles[tx][ty];
        ElementId eid = GeometryUtils::createBoundsInMap(boundaryMap, env);
        //  Update the tags on the bounding rectangle
        WayPtr bbox = boundaryMap->getWay(eid.getId());
        bbox->setTag("node_count", QString::number(nodeCounts[tx][ty]));
        bbox->setTag("task", QString::number(bboxCtr));
      }
      bboxCtr++;
    }
  }
  return boundaryMap;
}

}
