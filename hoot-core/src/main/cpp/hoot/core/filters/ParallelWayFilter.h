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

#ifndef PARALLELWAYFILTER_H
#define PARALLELWAYFILTER_H

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Units.h>

#include "WayFilter.h"

namespace hoot
{
  using namespace geos::geom;
  class Way;

class ParallelWayFilter : public WayFilter
{
public:
  ParallelWayFilter(const ConstOsmMapPtr& map, boost::shared_ptr<const Way> baseWay,
    bool filterUnparallel = true);

  virtual ~ParallelWayFilter();

  Radians calculateDifference(const boost::shared_ptr<const Way>& w) const;

  void setThreshold(Degrees threshold) { _threshold = threshold; }

  virtual bool isFiltered(const boost::shared_ptr<const Way>& w) const;

private:
  boost::shared_ptr<const Way> _baseWay;
  bool _filterUnparallel;
  // heading of baseWay at each coord
  std::vector<Radians> _headings;
  ConstOsmMapPtr _map;
  std::vector<Point*> _points;
  Degrees _threshold;
};

}

#endif // PARALLELWAYFILTER_H
