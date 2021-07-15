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
#ifndef BOUNDS_STRING_TASK_GRID_GENERATOR_H
#define BOUNDS_STRING_TASK_GRID_GENERATOR_H

// Hoot
#include <hoot/core/algorithms/changeset/TaskGridGenerator.h>

// GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * @brief The BoundsStringTaskGridGenerator class generates a rectangular task grid with a single
 * cell from an envelope string.
 */
class BoundsStringTaskGridGenerator : public TaskGridGenerator
{

public:

  /**
   * @brief BoundsStringTaskGridGenerator Constructor
   * @param bounds a string of the form minx,miny,maxx,maxy
   * @param output optional output path for writing the bounds file
   */
  BoundsStringTaskGridGenerator(const QString& bounds, const QString& outputPath = "");
  virtual ~BoundsStringTaskGridGenerator() = default;

  /**
   * @see TaskGridGenerator
   */
  TaskGrid generateTaskGrid() override;

private:

  // the bounds for the grid
  geos::geom::Envelope _bounds;
  QString _outputPath;
};

}

#endif // BOUNDS_STRING_TASK_GRID_GENERATOR_H
