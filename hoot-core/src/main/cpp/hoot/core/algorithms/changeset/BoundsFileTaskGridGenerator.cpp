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
#include "BoundsFileTaskGridGenerator.h"

// Hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

BoundsFileTaskGridGenerator::BoundsFileTaskGridGenerator(const QStringList& inputs) :
_inputs(inputs)
{
}

TaskGrid BoundsFileTaskGridGenerator::generateTaskGrid()
{
  LOG_INFO("Reading " << _inputs.size() << " task grid file(s)...");
  TaskGrid taskGrid;
  QMap<int, geos::geom::Envelope> taskGridTemp;
  for (int i = 0; i < _inputs.size(); i++)
  {
    const QString gridInput = _inputs.at(i);
    LOG_INFO("Reading task grid file: ..." << FileUtils::toLogFormat(gridInput, 25) << "...");
    taskGridTemp = GeometryUtils::readBoundsFileWithIds(gridInput);
    for (QMap<int, geos::geom::Envelope>::const_iterator taskGridTempItr = taskGridTemp.begin();
         taskGridTempItr != taskGridTemp.end(); ++taskGridTempItr)
    {
      TaskGrid::TaskGridCell taskGridCell;
      taskGridCell.id = taskGridTempItr.key();
      // We don't know the node count when reading the custom grid file.
      taskGridCell.replacementNodeCount = -1;
      taskGridCell.bounds = taskGridTempItr.value();
      taskGrid.addCell(taskGridCell);
    }
  }
  LOG_STATUS(
    "Read " << StringUtils::formatLargeNumber(taskGrid.size()) <<
    " task grid cells from " << _inputs.size() << " file(s).");
  return taskGrid;
}

}
