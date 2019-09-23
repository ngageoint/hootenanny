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

class TileUtils
{
public:

  static std::string className() { return "hoot::TileUtils"; }

  /**
   * TODO
   *
   * @param maxNodesPerTile
   * @param pixelSize
   * @param map
   * @return
   */
  static std::vector<std::vector<geos::geom::Envelope>> calculateTiles(
    const long maxNodesPerTile, const double pixelSize, OsmMapPtr map);

  /**
   * TODO
   *
   * @param tiles
   * @param randomSeed
   * @return
   */
  static int getRandomTileIndex(const std::vector<std::vector<geos::geom::Envelope>>& tiles,
                                int randomSeed);

  /**
   * TODO
   *
   * @param tiles
   * @param randomSeed
   * @return
   */
  static geos::geom::Envelope getRandomTile(
    const std::vector<std::vector<geos::geom::Envelope>>& tiles, int randomSeed);
};

}

#endif // TILE_UTILS_H
