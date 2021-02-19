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

#include "RandomMapCropper.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/conflate/tile/NodeDensityTileBoundsCalculator.h>
#include <hoot/core/io/NodeDensityTaskGridWriter.h>
#include <hoot/core/conflate/tile/TileUtils.h>

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


void RandomMapCropper::setTileFootprintOutputPath(QString path)
{
  _tileFootprintOutputPath = path;

  if (!OsmXmlWriter().isSupported(_tileFootprintOutputPath))
  {
    throw IllegalArgumentException("RandomMapCropper supports .osm tile footprint outputs only.");
  }
}

void RandomMapCropper::apply(OsmMapPtr& map)
{
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

  // TODO: integrate this with the node density retry params if they end up being useful with
  // the task-grid --node-density command
  NodeDensityTileBoundsCalculator tileCalc;
  tileCalc.setPixelSize(_pixelSize);
  tileCalc.setMaxNodesPerTile(_maxNodeCount);
  tileCalc.calculateTiles(map);
  const std::vector<std::vector<geos::geom::Envelope>> tiles = tileCalc.getTiles();
  const std::vector<std::vector<long>> nodeCounts = tileCalc.getNodeCounts();

  if (!_tileFootprintOutputPath.isEmpty())
  {
    NodeDensityTaskGridWriter::writeTilesToOsm(tiles, nodeCounts, _tileFootprintOutputPath);
    LOG_INFO("Wrote tile footprints to: " << _tileFootprintOutputPath);
  }
  _cropper.setBounds(TileUtils::getRandomTile(tiles, _randomSeed));
  _cropper.apply(map);

  // cleanup
  // TODO: This can be removed now, since its already happening in MapCropper, right?
  SuperfluousWayRemover::removeWays(map);
  SuperfluousNodeRemover::removeNodes(map);

  LOG_INFO("Starting map size: " << StringUtils::formatLargeNumber(startingMapSize));
  LOG_DEBUG("Starting map bounds: " << GeometryUtils::envelopeToString(startingBounds));
  LOG_INFO("Cropped map size: " << StringUtils::formatLargeNumber(map->size()));
  LOG_DEBUG(
    "Cropped map bounds: " <<
    GeometryUtils::envelopeToString(CalculateMapBoundsVisitor::getGeosBounds(map)));
  LOG_INFO("Minimum nodes in a single tile: " << tileCalc.getMinNodeCountInOneTile());
  LOG_INFO("Maximum nodes in a single tile: " << tileCalc.getMaxNodeCountInOneTile());
}

}
