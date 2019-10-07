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
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/util/MapProjector.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RandomMapCropper)

RandomMapCropper::RandomMapCropper() :
_maxNodeCount(1000),
_randomSeed(-1),
_pixelSize(0.001)
{
}

void RandomMapCropper::setConfiguration(const Settings& conf)
{
  _cropper.setConfiguration(conf);
  _cropper.setInvert(false);

  ConfigOptions confOpts = ConfigOptions(conf);
  _maxNodeCount = confOpts.getCropRandomMaxNodeCount();
  _pixelSize = confOpts.getCropRandomPixelSize();
  _randomSeed = confOpts.getRandomSeed();
}

void RandomMapCropper::apply(OsmMapPtr& map)
{
  //LOG_VARD(_maxNodeCount);
  if (_maxNodeCount < 1 || _maxNodeCount > (int)map->size())
  {
    throw IllegalArgumentException("Invalid max node count: " + _maxNodeCount);
  }

  LOG_INFO("Cropping to random map tile of max node size: " << _maxNodeCount << "...");
  LOG_VARD(_randomSeed);

  //MapProjector::projectToWgs84(map);

  const int startingMapSize = (int)map->size();
  // Could be an expensive operation. Keep as debug.
  geos::geom::Envelope startingBounds;
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    startingBounds = CalculateMapBoundsVisitor::getGeosBounds(map);
  }

  // compute tiles for the whole dataset, select one tile at random, and crop to the bounds of
  // the selected tile
  // TODO: Can we intelligently calculate pixel size here somehow?
  long minNodeCountInOneTile = 0;
  long maxNodeCountInOneTile = 0;
  const std::vector<std::vector<geos::geom::Envelope>> tiles =
    TileUtils::calculateTiles(
      _maxNodeCount, _pixelSize, map, minNodeCountInOneTile, maxNodeCountInOneTile);
  if (!_tileFootprintOutputPath.isEmpty())
  {
    if (_tileFootprintOutputPath.toLower().endsWith(".geojson"))
    {
      TileUtils::writeTilesToGeoJson(tiles, _tileFootprintOutputPath);
    }
    else
    {
      TileUtils::writeTilesToOsm(tiles, _tileFootprintOutputPath);
    }
    LOG_INFO("Wrote tile footprints to: " << _tileFootprintOutputPath);
  }
  _cropper.setBounds(TileUtils::getRandomTile(tiles, _randomSeed));
  _cropper.apply(map);

  // cleanup
  // TODO: This can be removed now, since its already happening in MapCropper, right?
  SuperfluousWayRemover::removeWays(map);
  SuperfluousNodeRemover::removeNodes(map);

  LOG_INFO("Starting map size: " << StringUtils::formatLargeNumber(startingMapSize));
  LOG_DEBUG("Starting map bounds: " << GeometryUtils::envelopeToConfigString(startingBounds));
  LOG_INFO("Cropped map size: " << StringUtils::formatLargeNumber(map->size()));
  LOG_DEBUG(
    "Cropped map bounds: " <<
    GeometryUtils::envelopeToConfigString(CalculateMapBoundsVisitor::getGeosBounds(map)));
  LOG_INFO("Minimum nodes in a single tile: " << minNodeCountInOneTile);
  LOG_INFO("Maximum nodes in a single tile: " << maxNodeCountInOneTile);
}

}
