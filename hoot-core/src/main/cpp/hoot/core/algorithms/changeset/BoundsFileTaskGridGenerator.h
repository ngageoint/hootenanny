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
#ifndef BOUNDS_FILE_TASK_GRID_GENERATOR_H
#define BOUNDS_FILE_TASK_GRID_GENERATOR_H

// Hoot
#include <hoot/core/algorithms/changeset/TaskGridGenerator.h>

namespace hoot
{

/**
 * @brief The BoundsFileTaskGridGenerator class generates a rectangular task grid from one or more
 * rectangular bounds file inputs.
 */
class BoundsFileTaskGridGenerator : public TaskGridGenerator
{

public:

  /**
   * @brief BoundsFileTaskGridGenerator Constructor
   * @param inputs one or more bounds file inputs
   */
  BoundsFileTaskGridGenerator(const QStringList& inputs);
  ~BoundsFileTaskGridGenerator() override = default;

  /**
   * @see TaskGridGenerator
   */
  TaskGrid generateTaskGrid() override;

private:

  // one or more paths to a custom task grid
  QStringList _inputs;
};

}

#endif // BOUNDS_FILE_TASK_GRID_GENERATOR_H
