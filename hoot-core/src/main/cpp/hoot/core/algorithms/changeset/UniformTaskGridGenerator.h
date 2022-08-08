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
#ifndef UNIFORM_TASK_GRID_GENERATOR_H
#define UNIFORM_TASK_GRID_GENERATOR_H

// Hoot
#include <hoot/core/algorithms/changeset/TaskGridGenerator.h>

namespace hoot
{

/**
 * @brief The UniformTaskGridGenerator class dreates a task grid made up of uniformly distributed,
 * same sized rectangles.
 *
 * Shamelessly adapted for hoot from:
 *
 * https://pcjericks.github.io/py-gdalogr-cookbook/vector_layers.html#create-fishnet-grid
 */
class UniformTaskGridGenerator : public TaskGridGenerator
{

public:

  /**
   * @brief Constructor
   * @param bounds the bounding box over which to generate the task grid
   * @param gridDimensionSize dimension of the grid (number of columns or rows)
   * @param output optional output URL to write the task grid to
   */
  UniformTaskGridGenerator(const QString& bounds, const int gridDimensionSize = 2,
                           const QString& output = "");
  /**
   * @brief  Constructor
   * @param inputs one or more inputs to base the task grid off of; the extent of the task grid is
   * the sum of the extent of all input data
   * @param gridDimensionSize dimension of the grid (number of columns or rows)
   * @param output optional output URL to write the task grid to
   */
  UniformTaskGridGenerator(const QStringList& inputs, const int gridDimensionSize = 2,
                           const QString& output = "");
  virtual ~UniformTaskGridGenerator() = default;

  /**
   * @see TaskGridGenerator
   */
  TaskGrid generateTaskGrid() override;

private:

  // area of the sum of all task grid cells;
  QString _bounds;
  // the size of the uniform grid matrix to generate (number of rows/cols)
  int _gridDimensionSize;
  // output location of the generated task grid file; useful for debugging, should be a *.osm file
  QString _output;
};

}

#endif // UNIFORM_TASK_GRID_GENERATOR_H
