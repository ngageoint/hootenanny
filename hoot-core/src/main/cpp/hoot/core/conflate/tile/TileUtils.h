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

#ifndef TILE_UTILS_H
#define TILE_UTILS_H

// GEOS
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Utilities for dealing with node density based tiles
 */
class TileUtils
{
public:

  static std::string className() { return "hoot::TileUtils"; }

  /**
   * Calculates a set of boundary tiles
   *
   * @param maxNodesPerTile the requested maximum number of nodes to be in any one tile
   * @param pixelSize the pixel size used to calculate the boundaries
   * @param map the map containing the nodes
   * @param minNodeCountInOneTile the smallest number of nodes actually found in any one tile
   * @param maxNodeCountInOneTile the largest number of nodes actually found in any one tile
   * @param nodeCounts a collection to store the number of node counts for each computed tile
   * @return a grid of tile bounding boxes
   */
  static std::vector<std::vector<geos::geom::Envelope>>  calculateTiles(
    const long maxNodesPerTile, const double pixelSize, OsmMapPtr map,
    long& minNodeCountInOneTile, long& maxNodeCountInOneTile,
    std::vector<std::vector<long>>& nodeCounts);

  /**
   * Retrieves a random tile index from a collection of tiles
   *
   * @param tiles the collection of tiles to calculate a random index for
   * @param randomSeed optional random number generator seed
   * @return an index
   */
  static int getRandomTileIndex(const std::vector<std::vector<geos::geom::Envelope>>& tiles,
                                int randomSeed);

  /**
   * Retrieves a random boundary tile from a collection of tiles
   *
   * @param tiles the collection of tiles from which to select
   * @param randomSeed optional random number generator seed
   * @return a random tile selected from the computed tiles
   */
  static geos::geom::Envelope getRandomTile(
    const std::vector<std::vector<geos::geom::Envelope>>& tiles, int randomSeed);

  /**
   * Writes boundary tiles to a GeoJSON output file
   *
   * @param tiles the collection of tiles to write
   * @param nodeCounts a collection of the number of node counts for each computed tile
   * @param outputPath the output file path
   * @param selectSingleRandomTile
   * @param randomSeed optional random number generator seed
   * @todo collapse with OSM writing method and refactor to TileFootprintWriter?
   * @todo refactor selectSingleRandomTile out?
   */
  static void writeTilesToGeoJson(const std::vector<std::vector<geos::geom::Envelope>>& tiles,
                                  const std::vector<std::vector<long>>& nodeCounts,
                                  const QString& outputPath,
                                  const QString& fileSource = "",
                                  const bool selectSingleRandomTile = false, int randomSeed = -1);

  /**
   * Writes boundary tiles to an OSM output file
   *
   * @param tiles the collection of tiles to write
   * @param nodeCounts a collection of the number of node counts for each computed tile
   * @param outputPath the output file path
   * @param selectSingleRandomTile
   * @param randomSeed optional random number generator seed
   * @todo collapse with GeoJSON writing method and refactor to TileFootprintWriter?
   * @todo refactor selectSingleRandomTile out?
   */
  static void writeTilesToOsm(
    const std::vector<std::vector<geos::geom::Envelope>>& tiles,
    const std::vector<std::vector<long>>& nodeCounts, const QString& outputPath,
    const bool selectSingleRandomTile = false, int randomSeed = -1);

private:

  /**
   * Convert boundary tiles to an OsmMap object
   *
   * @param tiles the collection of tiles to write
   * @param nodeCounts a collection of the number of node counts for each computed tile
   * @param randomTileIndex index of the random tile to use
   * @param selectSingleRandomTile
   */
  static OsmMapPtr tilesToOsmMap(const std::vector<std::vector<geos::geom::Envelope>>& tiles,
    const std::vector<std::vector<long>>& nodeCounts,
    int randomTileIndex, const bool selectSingleRandomTile = false);
};

}

#endif // TILE_UTILS_H
