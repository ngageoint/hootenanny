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
#include "NodeDensityTaskGridGenerator.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/conflate/tile/NodeDensityTileBoundsCalculator.h>
#include <hoot/core/io/TileBoundsWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/HootApiDbReader.h>

namespace hoot
{

NodeDensityTaskGridGenerator::NodeDensityTaskGridGenerator(
  const QString& bounds, const QString& input, const int maxNodesPerCell, const QString& output) :
_bounds(bounds),
_input(input),
_readInputFullThenCrop(false),
_maxNodesPerCell(maxNodesPerCell),
_output(output)
{
}

TaskGridGenerator::TaskGrid NodeDensityTaskGridGenerator::generateTaskGrid()
{
  return _calcNodeDensityTaskGrid(_getNodeDensityTaskGridInput());
}

OsmMapPtr NodeDensityTaskGridGenerator::_getNodeDensityTaskGridInput()
{
  // changeset derive will overwrite these later, if needed
  conf().set(ConfigOptions::getConvertBoundingBoxKey(), _bounds);
  conf().set(ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(), false);
  conf().set(
    ConfigOptions::getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
  conf().set(ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(), true);

  // TODO: replace the string truncation lengths with getProgressVarPrintLengthMax
  LOG_STATUS(
    "Preparing input data for task grid cell calculation from: ..." <<
    _output.right(25) << ", across bounds: " << _bounds << "...");
  assert(HootApiDbReader::isSupported(_input));

  // Getting large amounts of data via bbox can actually be slower than reading all of the data
  // and then cropping it, due to bounded query performance. #4192 improved this gap, but the
  // full read then crop can still be faster sometimes if the memory is available for it. All of
  // North Vegas takes 2:01 to read fully then crop to just the city bounds vs 2:30 to get via
  // bbox query.
  if (_readInputFullThenCrop)
  {
    conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), true);
  }

  // IMPORTANT: data used here must be unknown1 for node density calc to work
  OsmMapPtr map(new OsmMap());
  // small optimization since node density only needs nodes in the input; can only do this with
  // a db reader right now
  // TODO: I don't think the node only read is working correctly. Put a note in
  // NodeDensityTilesCmd.
//    std::shared_ptr<ApiDbReader> reader =
//      std::dynamic_pointer_cast<ApiDbReader>(
//        OsmMapReaderFactory::createReader(_output, true, Status::Unknown1));
//    reader->setReturnNodesOnly(true);
//    reader->open(_output);
//    reader->read(map);
  OsmMapReaderFactory::read(map, _input, true, Status::Unknown1);

  // Restore the default setting if it was changed, or the changeset replacement maps will be loaded
  // very slowly if loading from URL.
  conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), false);

  LOG_STATUS(
    StringUtils::formatLargeNumber(map->size()) << " task grid calc elements prepared " <<
    "in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  return map;
}

TaskGridGenerator::TaskGrid NodeDensityTaskGridGenerator::_calcNodeDensityTaskGrid(OsmMapPtr map)
{
  LOG_STATUS(
    "Calculating task grid cells for replacement data to: ..." << _output.right(25) << "...");
  NodeDensityTileBoundsCalculator boundsCalc;
  boundsCalc.setPixelSize(0.001);
  boundsCalc.setMaxNodesPerTile(_maxNodesPerCell);
  boundsCalc.setMaxNumTries(3);
  boundsCalc.setMaxTimePerAttempt(300);
  boundsCalc.setPixelSizeRetryReductionFactor(10);
  boundsCalc.setSlop(0.1);
  boundsCalc.calculateTiles(map);
  map.reset();
  const std::vector<std::vector<geos::geom::Envelope>> rawTaskGrid = boundsCalc.getTiles();
      const std::vector<std::vector<long>> nodeCounts = boundsCalc.getNodeCounts();
  if (!_output.trimmed().isEmpty())
  {
    TileBoundsWriter::writeTilesToOsm(rawTaskGrid, nodeCounts, _output);
  }

  // flatten the collection; use a list to maintain order
  QList<TaskGridCell> taskGrid;
  int cellCtr = 1;
  for (size_t tx = 0; tx < rawTaskGrid.size(); tx++)
  {
    for (size_t ty = 0; ty < rawTaskGrid[tx].size(); ty++)
    {
      TaskGridCell taskGridCell;
      taskGridCell.id = cellCtr;
      taskGridCell.replacementNodeCount = nodeCounts[tx][ty];
      taskGridCell.bounds = rawTaskGrid[tx][ty];
      taskGrid.append(taskGridCell);
      cellCtr++;
    }
  }
  assert(boundsCalc.getTileCount() == taskGrid.size());

  LOG_STATUS(
    "Calculated " << StringUtils::formatLargeNumber(taskGrid.size()) << " task grid cells in: " <<
    StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()) << ".");
  LOG_STATUS(
    "\tMaximum node count in any one tile: " <<
    StringUtils::formatLargeNumber(boundsCalc.getMaxNodeCountInOneTile()));
  LOG_STATUS(
    "\tMinimum node count in any one tile: " <<
    StringUtils::formatLargeNumber(boundsCalc.getMinNodeCountInOneTile()));
  LOG_INFO("Pixel size used: " << boundsCalc.getPixelSize());
  LOG_INFO(
    "\tMaximum node count per tile used: " <<
    StringUtils::formatLargeNumber(boundsCalc.getMaxNodesPerTile()));
  _subTaskTimer.restart();
  return taskGrid;
}

}
