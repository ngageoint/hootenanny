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
#ifndef NODE_DENSITY_TASK_GRID_GENERATOR_H
#define NODE_DENSITY_TASK_GRID_GENERATOR_H

// Hoot
#include <hoot/core/algorithms/changeset/TaskGridGenerator.h>
#include <hoot/core/conflate/tile/NodeDensityTileBoundsCalculator.h>
#include <hoot/core/elements/OsmMap.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

/**
 * @brief The NodeDensityTaskGridGenerator class uses the node density of a map to create a task
 * grid made up of potentially irregularly sized rectangles.
 *
 * Node density calc requires reading in all of the replacement node data, so may not be feasible
 * when replacing extremely large amounts of data.
 */
class NodeDensityTaskGridGenerator : public TaskGridGenerator
{

public:

  NodeDensityTaskGridGenerator(const QStringList& inputs, const int maxNodesPerCell,
                               const QString& bounds = "", const QString& output = "");

  virtual ~NodeDensityTaskGridGenerator() = default;

  /**
   * @see TaskGridGenerator
   */
  TaskGrid generateTaskGrid() override;

  void setReadInputFullThenCrop(const bool readInputFullThenCrop)
  { _readInputFullThenCrop = readInputFullThenCrop; }

  void setPixelSize(const double pixelSize) { _boundsCalc.setPixelSize(pixelSize); }
  void setMaxNumTries(const int numTries) { _boundsCalc.setMaxNumTries(numTries); }
  void setMaxTimePerAttempt(const int maxTime) { _boundsCalc.setMaxTimePerAttempt(maxTime); }
  void setPixelSizeRetryReductionFactor(const int factor)
  { _boundsCalc.setPixelSizeRetryReductionFactor(factor); }
  void setWriteRandomCellOnly(const bool writeRandomOnly)
  { _writeRandomCellOnly = writeRandomOnly; }
  void setRandomSeed(const int randomSeed) { _randomSeed = randomSeed; }

private:

  // per task timer
  QElapsedTimer _subTaskTimer;

  // one or more input files to base the grid calc from
  QStringList _inputs;
  // area of the sum of all task grid cells;
  QString _bounds;
  // runtime optimization for large amounts of data at the expense of using extra memory
  bool _readInputFullThenCrop;
  // output location of the generated task grid file; useful for debugging, should be a *.osm file
  QString _output;

  // calculates the grid
  NodeDensityTileBoundsCalculator _boundsCalc;

  // creates a single randomly selected task grid cell, rather than an entire task grid
  bool _writeRandomCellOnly;
  // allows for pseudo-randomizing the random cell selected
  int _randomSeed;

  // This preps the input for node density calc based task grid generation.
  OsmMapPtr _getNodeDensityTaskGridInput();

  TaskGrid _calcNodeDensityTaskGrid(OsmMapPtr map);
};

}

#endif // NODE_DENSITY_TASK_GRID_GENERATOR_H
