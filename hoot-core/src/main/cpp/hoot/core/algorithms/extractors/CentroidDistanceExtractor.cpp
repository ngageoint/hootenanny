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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "CentroidDistanceExtractor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, CentroidDistanceExtractor)

double CentroidDistanceExtractor::distance(const OsmMap& map,
  const std::shared_ptr<const Element>& target,
  const std::shared_ptr<const Element>& candidate) const
{
  ElementToGeometryConverter ec(map.shared_from_this());
  std::shared_ptr<Geometry> g1 = ec.convertToGeometry(target);
  std::shared_ptr<Geometry> g2 = ec.convertToGeometry(candidate);

  if (g1->isEmpty() || g2->isEmpty())
  {
    return -1;
  }

  g1.reset(GeometryUtils::validateGeometry(g1.get()));
  g2.reset(GeometryUtils::validateGeometry(g2.get()));
  std::shared_ptr<Point> tc(g1->getCentroid());
  std::shared_ptr<Point> cc(g2->getCentroid());

  if (tc.get() == nullptr || cc.get() == nullptr)
  {
    return nullValue();
  }

  return tc->distance(cc.get());
}

}
