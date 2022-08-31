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

#ifndef GEOMETRY_MERGER_H
#define GEOMETRY_MERGER_H

// GEOS
#include <geos/geom/Geometry.h>

// Standard
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace hoot
{

using GeometryPtr = std::shared_ptr<geos::geom::Geometry>;
using GeometryPair = std::pair<GeometryPtr, GeometryPtr>;

class GeometryMerger
{
public:
  /** Default constructor */
  GeometryMerger();
  /** Default destructor */
  ~GeometryMerger() = default;
  /**
   * @brief mergeGeometries Merge a vector of geometries all into one single geometry
   * @param geometries Vector of geos::geom::Geometry pointers that are to be merged together
   * @param envelope Bounding envelope used for sorting geometries by distance
   * @return Single geometry object that contains all geometries merged
   * (geos::geom::Geometry::union)
   */
  GeometryPtr mergeGeometries(std::vector<GeometryPtr> geometries, const geos::geom::Envelope& envelope);

private:
  /**
   * @brief mergeGeometryThread
   */
  void mergeGeometryThread();
  /** Number of threads to spawn to merge geometry */
  int _maxThreads;
  /** Atomic flag to tell all worker threads to terminate */
  std::atomic_bool _threadsWorking;
  /** Each round of merging geometries causes a resync of threads waiting for more work,
   * _finishedThreads counts the number of threads waiting for the next round of work.
   */
  std::atomic_int _finishedThreads;
  /** Condition variable that worker threads wait on between rounds */
  std::condition_variable _wait;
  /** Mutex for the condition variable */
  std::mutex _waitMutex;
  /** Stack of geometry pairs that need to be merged by the worker thread */
  std::stack<GeometryPair> _geometryStack;
  /** Mutex for geometry stack */
  std::mutex _geometryStackMutex;
  /** Vector of merged geometries (results) that are then fed back into the merging algorithm in the
   * next round */
  std::vector<GeometryPtr> _geometryReturn;
  /** Mutex for merged geometries */
  std::mutex _geometryReturnMutex;
  /** Total number of geometries to merge */
  int _geometryCount;
  /** Atomic counter for the number of geometries merged, used for reporting status */
  std::atomic_int _counter;
  /** Reporting interval from ConfigOptions
   *  note: because of the increasing complexity of the geometries, this interval is divided by 10
   *  when the remaining count is less than this interval
   */
  int _updateInterval;
};

}

#endif // GEOMETRY_MERGER_H
