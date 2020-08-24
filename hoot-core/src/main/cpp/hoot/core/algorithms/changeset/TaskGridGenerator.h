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
#ifndef TASK_GRID_GENERATOR_H
#define TASK_GRID_GENERATOR_H

// GEOS
#include <geos/geom/Envelope.h>

// Qt
#include <QList>

namespace hoot
{

/**
 * Allows for creating a task grid for performing distributed map operations
 */
class TaskGridGenerator
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

  typedef QList<TaskGridCell> TaskGrid;

  /**
   * Generates a task grid
   *
   * @return a task grid
   */
  virtual TaskGrid generateTaskGrid() = 0;
};

}

#endif // TASK_GRID_GENERATOR_H
