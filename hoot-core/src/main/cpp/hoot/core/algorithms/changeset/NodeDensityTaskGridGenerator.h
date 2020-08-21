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
#ifndef NODE_DENSITY_TASK_GRID_GENERATOR_H
#define NODE_DENSITY_TASK_GRID_GENERATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/changeset/TaskGridGenerator.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

/**
 * TODO
 */
class NodeDensityTaskGridGenerator : public TaskGridGenerator
{

public:

  NodeDensityTaskGridGenerator(const QString& bounds, const QString& input,
                               const int maxNodesPerCell,
                               const QString& output = "");

  virtual ~NodeDensityTaskGridGenerator() = default;

  /**
   * TODO
   *
   * @return
   */
  virtual TaskGrid generateTaskGrid();

  void setReadInputFullThenCrop(const bool readInputFullThenCrop)
  { _readInputFullThenCrop = readInputFullThenCrop; }

private:

  // per task timer
  QElapsedTimer _subTaskTimer;

  // area of the sum of all task grid cells;
  QString _bounds;
  // TODO
  QString _input;
  // runtime optimization for large amounts of data at the expense of using extra memory
  bool _readInputFullThenCrop;
  // allows for capping the max number of node density nodes per grid cell
  int _maxNodesPerCell;
  // output location of the generated task grid file; useful for debugging, should be a *.osm file
  QString _output;

  // This preps the input for node density calc based task grid generation.
  OsmMapPtr _getNodeDensityTaskGridInput();

  TaskGrid _calcNodeDensityTaskGrid(OsmMapPtr map);
};

}

#endif // NODE_DENSITY_TASK_GRID_GENERATOR_H
