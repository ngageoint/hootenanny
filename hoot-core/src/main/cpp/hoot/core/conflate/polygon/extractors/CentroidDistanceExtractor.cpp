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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CentroidDistanceExtractor.h"

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, CentroidDistanceExtractor)

double CentroidDistanceExtractor::distance(const OsmMap &map,
  const boost::shared_ptr<const Element>& target, const boost::shared_ptr<const Element> &candidate) const
{
  ElementConverter ec(map.shared_from_this());
  boost::shared_ptr<Geometry> g1 = ec.convertToGeometry(target);
  boost::shared_ptr<Geometry> g2 = ec.convertToGeometry(candidate);

  if (g1->isEmpty() || g2->isEmpty())
  {
    return -1;
  }

  g1.reset(GeometryUtils::validateGeometry(g1.get()));
  g2.reset(GeometryUtils::validateGeometry(g2.get()));
  auto_ptr<Point> tc(g1->getCentroid());
  auto_ptr<Point> cc(g2->getCentroid());

  if (tc.get() == 0 || cc.get() == 0)
  {
    return nullValue();
  }

  return tc->distance(cc.get());
}

}
