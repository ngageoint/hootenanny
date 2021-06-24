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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef PROBABILITYOFMATCH_H
#define PROBABILITYOFMATCH_H

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * @brief The ProbabilityOfMatch class (Singleton)
 */
class ProbabilityOfMatch
{
public:

  static ProbabilityOfMatch& getInstance();

  double attributeScore(const ConstOsmMapPtr &map, const ConstWayPtr& w1,
    const ConstWayPtr &w2) const;

  double distanceScore(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
    const ConstWayPtr& w2);
  double distanceScore(const hoot::ConstOsmMapPtr &map, const ConstWayPtr &w1,
    const std::shared_ptr<const geos::geom::LineString> &ls2, Meters circularError);

  double lengthScore(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
    const ConstWayPtr& w2) const;

  double parallelScore(const ConstOsmMapPtr& map, const ConstWayPtr& w1,
    const ConstWayPtr& w2) const;

  double expertProbability(const ConstOsmMapPtr &map, const ConstWayPtr& w1,
    const ConstWayPtr &w2);

  double zipperScore(const ConstWayPtr& w1, const ConstWayPtr& w2) const;

private:

  double _parallelExp;
  double _dMax;

  ProbabilityOfMatch();
  /** Default destructor */
  ~ProbabilityOfMatch() = default;
  /** Delete copy constructor and assignment operator */
  ProbabilityOfMatch(const ProbabilityOfMatch&) = delete;
  ProbabilityOfMatch& operator=(const ProbabilityOfMatch&) = delete;
};

}

#endif // PROBABILITYOFMATCH_H
