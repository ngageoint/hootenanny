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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#ifndef PARALLEL_API_JOB_H
#define PARALLEL_API_JOB_H

//  Geos
#include <geos/geom/Geometry.h>

namespace hoot
{

/** Forward declarations for pointer typedef */
class ParallelApiJob;
using ParallelApiJobPtr = std::shared_ptr<ParallelApiJob>;

/** Job class representing a single job that is run through the ParallelBoundedReader,
 *  can be divided into multiple jobs.
 */
class ParallelApiJob
{
public:
  virtual std::vector<ParallelApiJobPtr> CreateInitialJob() const = 0;
  virtual std::vector<ParallelApiJobPtr> SplitJob() const = 0;
  virtual bool isBounded() const { return false; }
  virtual geos::geom::Envelope getBounds() const { geos::geom::Envelope e;  e.setToNull(); return e; }
};


/** Bounded job that initially splits the bounds by `grid_size` and then into four equal grids */
class BoundedParallelApiJob : public ParallelApiJob
{
public:
  BoundedParallelApiJob(const geos::geom::Envelope& env, double grid_size) : _envelope(env), _coordGridSize(grid_size) { }
  std::vector<ParallelApiJobPtr> CreateInitialJob() const override;
  std::vector<ParallelApiJobPtr> SplitJob() const override;
  bool isBounded() const override { return true; }
  geos::geom::Envelope getBounds() const override { return _envelope; }

  static void setBoundsIsPolygon(bool isPoly) { _boundsIsPolygon = isPoly; }
  static void setBoundingPolygon(const std::shared_ptr<geos::geom::Geometry>& poly) { _boundingPoly = poly; }
private:
  geos::geom::Envelope _envelope;
  double _coordGridSize;
  /** All bounded jobs have the same bounds so it is shared among all jobs */
  static bool _boundsIsPolygon;
  static std::shared_ptr<geos::geom::Geometry> _boundingPoly;
};

class QueryParallelApiJob : public ParallelApiJob
{
public:
  QueryParallelApiJob();
  std::vector<ParallelApiJobPtr> CreateInitialJob() const override;
  std::vector<ParallelApiJobPtr> SplitJob() const override;
  bool isBounded() const override { return false; }
private:
};

}

#endif  //  PARALLEL_API_JOB_H
