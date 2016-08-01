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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "EuclideanDistanceExtractor.h"

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, EuclideanDistanceExtractor)

double EuclideanDistanceExtractor::distance(const OsmMap &map,
  const shared_ptr<const Element>& target, const shared_ptr<const Element> &candidate) const
{
  ElementConverter ec(map.shared_from_this());
  shared_ptr<Geometry> g1 = ec.convertToGeometry(target);
  shared_ptr<Geometry> g2 = ec.convertToGeometry(candidate);

  if (g1->isEmpty() || g2->isEmpty())
  {
    return nullValue();
  }

  g1.reset(GeometryUtils::validateGeometry(g1.get()));
  g2.reset(GeometryUtils::validateGeometry(g2.get()));

  if (g1.get() == 0 || g2.get() == 0)
  {
    return nullValue();
  }

  return g1->distance(g2.get());
}

}
