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
#ifndef TASK_GRID_H
#define TASK_GRID_H

namespace hoot
{

/**
 * @brief The TaskGrid class represents a subdivided area for performing work on a map.
 */
class TaskGrid
{

public:

  /**
   * Individual component of a task grid
   */
  struct TaskGridCell
  {
    int id;
    int replacementNodeCount;
    geos::geom::Envelope bounds;
  };

  TaskGrid() = default;

  /**
   * @brief addCell adds a task grid cell to this task grid; cell ordering will be maintainied.
   * @param cell task grid cell to add
   */
  void addCell(const TaskGridCell& cell) { _cells.append(cell); }

  QList<TaskGridCell> getCells() const { return _cells; }
  geos::geom::Envelope getBounds() const;
  int size() const { return _cells.size(); }

private:

  QList<TaskGridCell> _cells;
};

}

#endif // TASK_GRID_H
