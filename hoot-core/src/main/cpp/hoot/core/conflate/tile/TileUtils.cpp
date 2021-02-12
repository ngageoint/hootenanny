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
 * @copyright Copyright (C) 2019, 2020 Maxar (http://www.maxar.com/)
 */

#include "TileUtils.h"

// hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/RandomNumberUtils.h>

// Tgs
#include <tgs/Statistics/Random.h>

namespace hoot
{

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

}
