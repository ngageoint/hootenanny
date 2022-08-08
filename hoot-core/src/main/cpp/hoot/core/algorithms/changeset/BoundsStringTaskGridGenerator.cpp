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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "BoundsStringTaskGridGenerator.h"

// Hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

BoundsStringTaskGridGenerator::BoundsStringTaskGridGenerator(
  const QString& bounds, const QString& outputPath) :
_bounds(GeometryUtils::envelopeFromString(bounds)),
_outputPath(outputPath)
{
}

TaskGrid BoundsStringTaskGridGenerator::generateTaskGrid()
{
  LOG_INFO(
    "Generating task grid for bounds: " << GeometryUtils::envelopeToString(_bounds) << "...");

  TaskGrid taskGrid;
  TaskGrid::TaskGridCell taskGridCell;
  taskGridCell.id = 1;
  // We don't know the node count when creating from bounds string.
  taskGridCell.replacementNodeCount = -1;
  taskGridCell.bounds = _bounds;
  taskGrid.addCell(taskGridCell);

  if (!_outputPath.isEmpty())
  {
    OsmMapWriterFactory::write(GeometryUtils::createMapFromBounds(_bounds), _outputPath);
  }

  return taskGrid;
}

}
