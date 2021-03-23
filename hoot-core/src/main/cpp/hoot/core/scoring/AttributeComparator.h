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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef ATTRIBUTECOMPARATOR_H
#define ATTRIBUTECOMPARATOR_H

#include <hoot/core/scoring/BaseComparator.h>

namespace hoot
{

class AttributeComparator : public BaseComparator
{
public:

  AttributeComparator(const std::shared_ptr<OsmMap>& map1, const std::shared_ptr<OsmMap>& map2);

  ~AttributeComparator() = default;

  double compareMaps() override;

  double getConfidenceInterval() { return _ci; }

  double getMeanScore() { return _mean; }

  double getMedianScore() { return _median; }

  double getStandardDeviation() { return _s; }

  void setIterations(int i) { _iterations = i; }

private:

  int _iterations;
  geos::geom::Coordinate _r;
  double _median;
  double _mean;
  // confidence interval
  double _ci;
  // sampled standard deviation
  double _s;
};

}

#endif // ATTRIBUTECOMPARATOR_H
