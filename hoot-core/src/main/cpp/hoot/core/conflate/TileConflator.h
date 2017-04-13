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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TILECONFLATOR_H
#define TILECONFLATOR_H

// boost
#include <boost/shared_ptr.hpp>

// GEOS
#include <geos/geom/Envelope.h>

// hoot
#include <hoot/core/util/Units.h>

// Qt
#include <QString>

// Standard
#include <vector>

// TGS
#include <tgs/HashMap.h>

#include "TileWorker.h"

namespace hoot
{

using namespace boost;
using namespace geos::geom;
using namespace std;

class OsmMap;
class NodeReplacements;

/**
 * The TileConflator is a high level class that delegates the real work to TileWorkers. This class
 * makes no assumptions about the underlying file system. It is designed to use either HDFS or
 * the local file system, but that could conceivably be expanded on to support DBs or other storage
 * mechanisms.
 */
class TileConflator
{
public:
  TileConflator(boost::shared_ptr<TileWorker> worker);

  virtual ~TileConflator();

  void setBuffer(Degrees buffer) { _buffer = buffer; }

  void setMaxNodesPerBox(int n) { _maxNodesPerBox = n; }

  void setSources(QString input1, QString input2);

  void setWorkDir(QString workDir);

  void conflate(QString outputPath);

private:
  OGREnvelope _envelope;
  QString _input1, _input2;
  QString _tmp;

  /// The current input scratch space
  QString _scratchIn;
  /// The current output scratch space
  QString _scratchOut;
  /// a cache of the ways after "breakWays"
  QString _wayCache;

  // buffer in decimal degrees.
  Degrees _buffer;

  // [x][y]
  vector< vector<Envelope> > _tiles;

  int _mapPart;

  int _maxNodesPerBox;
  int _nodeReplacementPart;
  int _iteration;

  boost::shared_ptr<TileWorker> _worker;

  /**
   * The tile conflation works by creating and removing temporary copies of the input data. This
   * function eases that creation/deletion process by removing the old _scratchIn, assigning the
   * old _scratchOut to _scratchIn and creating a new _scratchOut.
   */
  void _createNextScratch();

  void _determineTileBounds(const cv::Mat& r1, const cv::Mat& r2);
};

}

#endif // TILECONFLATOR_H
