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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef PARALLELWAYCRITERION_H
#define PARALLELWAYCRITERION_H

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/filters/ElementCriterion.h>

namespace hoot
{

class Way;

class ParallelWayCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::ParallelWayCriterion"; }

  ParallelWayCriterion() {}
  ParallelWayCriterion(const ConstOsmMapPtr& map,
                       ConstWayPtr baseWay,
                       bool isParallel = true);

  virtual ~ParallelWayCriterion();

  Radians calculateDifference(const ConstWayPtr& w) const;

  void setThreshold(Degrees threshold) { _threshold = threshold; }

  virtual bool isSatisfied(const boost::shared_ptr<const Element> &e) const;

  ElementCriterionPtr clone()
  { return ElementCriterionPtr(new ParallelWayCriterion(_map, _baseWay, _isParallel)); }

private:

  ConstOsmMapPtr _map;
  ConstWayPtr _baseWay;
  bool _isParallel;

  // heading of baseWay at each coord
  std::vector<Radians> _headings;
  std::vector<geos::geom::Point*> _points;
  Degrees _threshold;
};

}

#endif // PARALLELWAYCRITERION_H
