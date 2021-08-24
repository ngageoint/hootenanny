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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "NodeDensityTaskGridGenerator.h"

// Hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/io/NodeDensityTaskGridWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

NodeDensityTaskGridGenerator::NodeDensityTaskGridGenerator(
  const QStringList& inputs, const int maxNodesPerCell, const QString& bounds,
  const QString& output) :
_inputs(inputs),
_bounds(bounds),
_readInputFullThenCrop(false),
_output(output),
_writeRandomCellOnly(false),
_randomSeed(-1)
{
  _boundsCalc.setMaxNodesPerTile(maxNodesPerCell);

  _boundsCalc.setPixelSize(0.001);
  _boundsCalc.setMaxNumTries(3);
  _boundsCalc.setMaxTimePerAttempt(300);
  _boundsCalc.setPixelSizeRetryReductionFactor(10);
  _boundsCalc.setSlop(0.1);
}

TaskGrid NodeDensityTaskGridGenerator::generateTaskGrid()
{
  return _calcNodeDensityTaskGrid(_getNodeDensityTaskGridInput());
}

OsmMapPtr NodeDensityTaskGridGenerator::_getNodeDensityTaskGridInput()
{
  // changeset derive will overwrite these later, if needed
  if (!_bounds.trimmed().isEmpty())
  {
    conf().set(ConfigOptions::getBoundsKey(), _bounds);
    conf().set(ConfigOptions::getBoundsKeepEntireFeaturesCrossingBoundsKey(), false);
    conf().set(
      ConfigOptions::getBoundsKeepImmediatelyConnectedWaysOutsideBoundsKey(), false);
    conf().set(ConfigOptions::getBoundsKeepOnlyFeaturesInsideBoundsKey(), true);
  }

  // TODO: replace the string truncation lengths with getProgressVarPrintLengthMax
  QString msg =
    "Preparing input data for task grid cell calculation from: ..." + FileUtils::toLogFormat(_output, 25);
  if (!_bounds.trimmed().isEmpty())
  {
    msg += ", across bounds: " + _bounds;
  }
  msg += "...";
  LOG_STATUS(msg);
  //assert(HootApiDbReader::isSupported(_input));

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
  OsmMapPtr map = std::make_shared<OsmMap>();
  // small optimization since node density only needs nodes in the input; can only do this with
  // a db reader right now
  // TODO: I don't think the node only read is working correctly.
//    std::shared_ptr<ApiDbReader> reader =
//      std::dynamic_pointer_cast<ApiDbReader>(
//        OsmMapReaderFactory::createReader(_output, true, Status::Unknown1));
//    reader->setReturnNodesOnly(true);
//    reader->open(_output);
//    reader->read(map);
  for (int i = 0; i < _inputs.size(); i++)
  {
    OsmMapReaderFactory::read(map, _inputs.at(i), /*true*/false, Status::Unknown1);
  }

  // Restore the default setting if it was changed, or the changeset replacement maps will be loaded
  // very slowly if loading from URL.
  conf().set(ConfigOptions::getApidbReaderReadFullThenCropOnBoundedKey(), false);

  LOG_STATUS(
    StringUtils::formatLargeNumber(map->size()) << " task grid calc elements prepared " <<
    "in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()));
  _subTaskTimer.restart();

  return map;
}

TaskGrid NodeDensityTaskGridGenerator::_calcNodeDensityTaskGrid(OsmMapPtr map)
{
  LOG_STATUS(
    "Calculating task grid cells for replacement data to: ..." << FileUtils::toLogFormat(_output, 25) << "...");

  _boundsCalc.calculateTiles(map);

  const std::vector<std::vector<geos::geom::Envelope>> rawTaskGrid = _boundsCalc.getTiles();
      const std::vector<std::vector<long>> nodeCounts = _boundsCalc.getNodeCounts();
  if (_output.toLower().endsWith(".geojson"))
  {
    NodeDensityTaskGridWriter::writeTilesToGeoJson(
      rawTaskGrid, nodeCounts, _output, map->getSource(), _writeRandomCellOnly, _randomSeed);
  }
  else
  {
    NodeDensityTaskGridWriter::writeTilesToOsm(
      rawTaskGrid, nodeCounts, _output, _writeRandomCellOnly, _randomSeed);
  }
  map.reset();

  // flatten the collection; use a list to maintain order
  TaskGrid taskGrid;
  int cellCtr = 1;
  for (size_t tx = 0; tx < rawTaskGrid.size(); tx++)
  {
    for (size_t ty = 0; ty < rawTaskGrid[tx].size(); ty++)
    {
      TaskGrid::TaskGridCell taskGridCell;
      taskGridCell.id = cellCtr;
      taskGridCell.replacementNodeCount = nodeCounts[tx][ty];
      taskGridCell.bounds = rawTaskGrid[tx][ty];
      taskGrid.addCell(taskGridCell);
      cellCtr++;
    }
  }
  assert(_boundsCalc.getTileCount() == taskGrid.size());

  LOG_STATUS(
    "Calculated " << StringUtils::formatLargeNumber(taskGrid.size()) <<
    " task grid cells in: " << StringUtils::millisecondsToDhms(_subTaskTimer.elapsed()) << ".");
  LOG_STATUS(
    "\tMaximum node count in any one tile: " <<
    StringUtils::formatLargeNumber(_boundsCalc.getMaxNodeCountInOneTile()));
  LOG_STATUS(
    "\tMinimum node count in any one tile: " <<
    StringUtils::formatLargeNumber(_boundsCalc.getMinNodeCountInOneTile()));
  LOG_INFO("Pixel size used: " << _boundsCalc.getPixelSize());
  LOG_INFO(
    "\tMaximum node count per tile used: " <<
    StringUtils::formatLargeNumber(_boundsCalc.getMaxNodesPerTile()));
  _subTaskTimer.restart();
  return taskGrid;
}

}
