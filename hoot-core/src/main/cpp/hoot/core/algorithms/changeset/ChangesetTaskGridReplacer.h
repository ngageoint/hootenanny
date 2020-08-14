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
 * and application. Either a node density generated task grid or a task grid from an input file may
 * be used. Node density calc requires reading in all of the replacement data, so may not be
 * feasible when replacing very large amounts of data.
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
  void setPreloadMaps(bool preload) { _preloadMapsBeforeChangesetDerivation = preload; }
  void setTaskGridType(GridType gridType) { _gridType = gridType; }
  void setNodeDensityGridBounds(const QString& bounds) { _nodeDensityGridBounds = bounds; }
  void setNodeDensityMaxNodesPerCell(int maxNodes) { _maxNodeDensityNodesPerCell = maxNodes; }
  void setGridInput(const QString& input) { _gridInput = input; }
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

  // By passing in pre-loaded maps, we can cut down on I/O quite a bit (requires memory double the
  // combined size of both maps). Unfortunately, have seen bugs occur under certain circumstances
  // when enabling this that need to be tracked down.
  bool _preloadMapsBeforeChangesetDerivation;
  // cached maps
  OsmMapPtr _dataToReplaceFullMap;
  OsmMapPtr _replacementDataFullMap;

  // manner in which to generate the task grid
  GridType _gridType;
  // area of the sum of all task grid cells; needed for node density calc only
  QString _nodeDensityGridBounds;
  // allows for capping the max number of node density nodes per grid cell
  int _maxNodeDensityNodesPerCell;
  // output location of the generated node density task grid file; useful for debugging
  QString _nodeDensityTaskGridOutputFile;
  // path to a custom task grid
  QString _gridInput;
  // swap the order in which the task grid cells; useful for testing adjacency replacement issues
  bool _reverseTaskGrid;

  // derives the replacement changesets
  std::shared_ptr<ChangesetReplacementCreator> _changesetCreator;
  // applies the replacement changesets
  std::shared_ptr<OsmApiDbSqlChangesetApplier> _changesetApplier;
  // all changeset files generated are stored here
  QString _changesetsOutputDir;
  // allows for ending a large replacement early; useful for debugging; set to -1 to disable
  int _killAfterNumChangesetDerivations;
  int _numChangesetsDerived;
  // seconds
  double _totalChangesetDeriveTime;
  double _averageChangesetDeriveTime;

  QString _finalOutput;

  void _initConfig();

  void _cacheFullMaps();

  QMap<int, geos::geom::Envelope> _getTaskGrid();
  // This preps the input for node density calc based task grid generation.
  OsmMapPtr _getNodeDensityTaskGridInput();
  QMap<int, geos::geom::Envelope> _calcNodeDensityTaskGrid(OsmMapPtr map);

  void _replaceEntireTaskGrid(const QMap<int, geos::geom::Envelope>& taskGrid);
  void _replaceTaskGridCell(
    const int taskGridCellId, const int changesetNum, const geos::geom::Envelope& bounds,
    const int taskGridSize);

  // writes out all of the ref data; useful for debugging...expensive
  void _getUpdatedData(const QString& outputFile);
};

}

#endif // CHANGESET_TASK_GRID_REPLACER_H
