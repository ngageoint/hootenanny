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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef TILE_UTILS_H
#define TILE_UTILS_H

// GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * Utilities for dealing with node density based tiles
 */
class TileUtils
{
public:

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
};

}

#endif // TILE_UTILS_H
