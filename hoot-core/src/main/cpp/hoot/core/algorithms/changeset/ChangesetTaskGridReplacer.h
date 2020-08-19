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
#ifndef CHANGESET_TASK_GRID_REPLACER_H
#define CHANGESET_TASK_GRID_REPLACER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class ChangesetReplacementCreator;
class OsmApiDbSqlChangesetApplier;

/**
 * This class can replace data in an OSM API database across multiple AOI's via changeset generation
 * and application.
 *
 * Either an auto node density generated task grid or a task grid from one or more bounds input
 * files may be used to partition the data replacements. Node density calc requires reading in all
 * of the replacement node data, so may not be feasible when replacing extremely large amounts of
 * data.
 */
class ChangesetTaskGridReplacer
{

public:

  // TODO: add the option to auto-generate a uniform grid
  enum GridType
  {
    NodeDensity = 0,
    InputFile
  };

  struct TaskGridCell
  {
    int replacementNodeCount;
    geos::geom::Envelope bounds;
  };

  ChangesetTaskGridReplacer();

  /**
   * Replaces data
   *
   * @param toReplace URL to the data to replace; must be an OSM API database
   * @param replacement URL to the replacement data; currently must be a Hoot API database, but
   * will update to support files again soon
   */
  void replace(const QString& toReplace, const QString& replacement);

  void setOriginalDataSize(int size) { _originalDataSize = size; }
  void setTaskGridType(GridType gridType) { _gridType = gridType; }
  void setNodeDensityGridBounds(const QString& bounds) { _nodeDensityGridBounds = bounds; }
  void setReadNodeDensityInputFullThenCrop(bool readFull)
  { _readNodeDensityInputFullThenCrop = readFull; }
  void setNodeDensityMaxNodesPerCell(int maxNodes) { _maxNodeDensityNodesPerCell = maxNodes; }
  void setGridInputs(const QStringList& inputs) { _gridInputs = inputs; }
  void setNodeDensityTaskGridOutputFile(const QString& output)
  { _nodeDensityTaskGridOutputFile = output; }
  void setReverseTaskGrid(bool reverse) { _reverseTaskGrid = reverse; }
  void setChangesetsOutputDir(const QString& dir)
  { _changesetsOutputDir = dir; }
  void setKillAfterNumChangesetDerivations(int numDerivations)
  { _killAfterNumChangesetDerivations = numDerivations; }
  void setWriteFinalOutput(QString output) { _finalOutput = output; }

private:

  // overall operation timer
  QElapsedTimer _opTimer;
  // per task timer
  QElapsedTimer _subTaskTimer;

  // data being replaced; must be osmapidb://
  QString _dataToReplaceUrl;
  // TODO: get rid of this; hacky
  int _originalDataSize;
  // replacement data
  QString _replacementUrl;

  // manner in which to generate the task grid
  GridType _gridType;
  // area of the sum of all task grid cells; needed for node density calc only
  QString _nodeDensityGridBounds;
  // runtime optimization for large amounts of data at the expense of using extra memory
  bool _readNodeDensityInputFullThenCrop;
  // allows for capping the max number of node density nodes per grid cell
  int _maxNodeDensityNodesPerCell;
  // output location of the generated node density task grid file; useful for debugging
  QString _nodeDensityTaskGridOutputFile;
  // one or more paths to a custom task grid
  QStringList _gridInputs;
  // swap the order in which the task grid cells; useful for testing adjacency replacement issues
  bool _reverseTaskGrid;

  // derives the replacement changesets
  std::shared_ptr<ChangesetReplacementCreator> _changesetCreator;
  // all changeset files generated are stored here
  QString _changesetsOutputDir;
  // allows for ending a large replacement early; useful for debugging; set to -1 to disable
  int _killAfterNumChangesetDerivations;
  int _numChangesetsDerived;
  // seconds
  double _totalChangesetDeriveTime;
  double _averageChangesetDeriveTime;

  // applies the replacement changesets
  std::shared_ptr<OsmApiDbSqlChangesetApplier> _changesetApplier;
  int _totalNodesCreated;
  int _totalNodesModified;
  int _totalNodesDeleted;
  int _totalWaysCreated;
  int _totalWaysModified;
  int _totalWaysDeleted;
  int _totalRelationsCreated;
  int _totalRelationsModified;
  int _totalRelationsDeleted;
  int _totalCreations;
  int _totalModifications;
  int _totalDeletions;

  QString _finalOutput;

  void _initConfig();

  QMap<int, TaskGridCell> _getTaskGrid();
  // This preps the input for node density calc based task grid generation.
  OsmMapPtr _getNodeDensityTaskGridInput();
  QMap<int, TaskGridCell> _calcNodeDensityTaskGrid(OsmMapPtr map);

  void _replaceEntireTaskGrid(const QMap<int, TaskGridCell>& taskGrid);
  void _replaceTaskGridCell(
    const int taskGridCellId, const int changesetNum, const geos::geom::Envelope& bounds,
    const int taskGridSize, const int numReplacementNodes = -1);
  void _printChangesetStats();

  // writes out all of the ref data; useful for debugging...expensive
  void _getUpdatedData(const QString& outputFile);
};

}

#endif // CHANGESET_TASK_GRID_REPLACER_H
