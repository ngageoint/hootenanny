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

#include "RandomMapCropper.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/rnd/util/TileUtils.h>
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RandomMapCropper)

RandomMapCropper::RandomMapCropper()
{
}

void RandomMapCropper::setConfiguration(const Settings& conf)
{
  _cropper.setConfiguration(conf);
  _cropper.setInvert(false);

  ConfigOptions confOpts = ConfigOptions(conf);
  _maxNodeCount = confOpts.getRandomCropMaxNodeCount();
  _randomSeed = confOpts.getRandomSeed();
}

void RandomMapCropper::apply(OsmMapPtr& map)
{
  if (_maxNodeCount < 1 || _maxNodeCount > (int)map->size())
  {
    throw IllegalArgumentException("Invalid max node count: " + _maxNodeCount);
  }
  LOG_VARD(_maxNodeCount);
  LOG_VARD(_randomSeed);

  // compute tiles for the whole dataset, select one tile at random, and crop to the bounds of
  // the selected tile
  geos::geom::Envelope randomTile =
    TileUtils::getRandomTile(
      TileUtils::calculateTiles(_maxNodeCount, 0.001, map), _randomSeed);
  LOG_DEBUG("Randomly selected tile: " << GeometryUtils::toConfigString(randomTile));
  _cropper.setBounds(randomTile);
  _cropper.apply(map);
}

}
