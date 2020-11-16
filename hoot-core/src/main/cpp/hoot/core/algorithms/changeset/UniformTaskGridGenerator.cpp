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
#include "UniformTaskGridGenerator.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

namespace hoot
{

UniformTaskGridGenerator::UniformTaskGridGenerator(
  const QString& bounds, const int gridDimensionSize, const QString& output) :
_bounds(bounds),
_gridDimensionSize(gridDimensionSize),
_output(output)
{
}

UniformTaskGridGenerator::UniformTaskGridGenerator(
 const QStringList& inputs, const int gridDimensionSize, const QString& output) :
_gridDimensionSize(gridDimensionSize),
_output(output)
{
  OsmMapPtr map(new OsmMap());
  for (int i = 0; i < inputs.size(); i++)
  {
    IoUtils::loadMap(map, inputs.at(i), true, Status::Invalid);
  }
  _bounds = GeometryUtils::envelopeToString(CalculateMapBoundsVisitor::getGeosBounds(map));
}

TaskGrid UniformTaskGridGenerator::generateTaskGrid()
{
  LOG_INFO(
    "Creating uniform task grid with " << _gridDimensionSize << "x" << _gridDimensionSize <<
    " cells across bounds: " << _bounds << "...");

  TaskGrid taskGrid;

  const geos::geom::Envelope taskGridEnv = GeometryUtils::envelopeFromString(_bounds);
  const double widthPerCell = taskGridEnv.getWidth() / (double)_gridDimensionSize;
  const double heightPerCell = taskGridEnv.getHeight() / (double)_gridDimensionSize;

  const int rows = ceil(taskGridEnv.getHeight() / heightPerCell);
  const int cols = ceil(taskGridEnv.getWidth() / widthPerCell);

  const double cellXLeftOrigin = taskGridEnv.getMinX();
  const double cellXRightOrigin = taskGridEnv.getMinX() + widthPerCell;
  const double cellYTopOrigin = taskGridEnv.getMaxY();
  const double cellYBottomOrigin = taskGridEnv.getMaxY() - heightPerCell;

  int cellCtr = 1;
  // used to write boundaries to file after the grid is created
  QMap<int, geos::geom::Envelope> boundaries;
  double cellXLeft = cellXLeftOrigin;
  double cellXRight = cellXRightOrigin;
  for (int i = 0; i < cols; i++)
  {
    double cellYTop = cellYTopOrigin;
    double cellYBottom = cellYBottomOrigin;

    for (int j = 0; j < rows; j++)
    {
      TaskGrid::TaskGridCell taskGridCell;
      taskGridCell.id = cellCtr;
      // We don't know the individual cell node counts when creating a uniform grid.
      taskGridCell.replacementNodeCount = -1;
      geos::geom::Envelope taskGridCellEnv(cellXLeft, cellXRight, cellYBottom, cellYTop);
      taskGridCell.bounds = taskGridCellEnv;
      boundaries[taskGridCell.id] = taskGridCell.bounds;
      taskGrid.addCell(taskGridCell);

      cellYTop = cellYTop - heightPerCell;
      cellYBottom = cellYBottom - heightPerCell;

      cellCtr++;
    }

    cellXLeft = cellXLeft + widthPerCell;
    cellXRight = cellXRight + widthPerCell;
  }

  if (!_output.trimmed().isEmpty())
  {
    // write out task grid to file
    // TODO: There are some differences between how geojson files are written by this class and
    // NodeDensityTaskGridGenerator that may have to be dealt with at some point.
    OsmMapWriterFactory::write(GeometryUtils::createMapFromBoundsCollection(boundaries), _output);
  }

  LOG_STATUS(
    "Created uniform task grid with " << _gridDimensionSize << "x" << _gridDimensionSize <<
    " cells across bounds: " << _bounds << "...");
  return taskGrid;
}

}
