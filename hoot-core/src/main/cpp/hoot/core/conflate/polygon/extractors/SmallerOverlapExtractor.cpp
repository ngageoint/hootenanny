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
#include "SmallerOverlapExtractor.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, SmallerOverlapExtractor)

SmallerOverlapExtractor::SmallerOverlapExtractor()
{
}

double SmallerOverlapExtractor::extract(const OsmMap& map, const ConstElementPtr& target,
  const ConstElementPtr& candidate) const
{
  ElementConverter ec(map.shared_from_this());
  boost::shared_ptr<Geometry> g1 = ec.convertToGeometry(target);
  boost::shared_ptr<Geometry> g2 = ec.convertToGeometry(candidate);

  if (g1->isEmpty() || g2->isEmpty())
  {
    return nullValue();
  }

  auto_ptr<Geometry> overlap;
  try
  {
    overlap.reset(g1->intersection(g2.get()));
  }
  catch (geos::util::TopologyException& e)
  {
    g1.reset(GeometryUtils::validateGeometry(g1.get()));
    g2.reset(GeometryUtils::validateGeometry(g2.get()));
    overlap.reset(g2->intersection(g1.get()));
  }

  double a1 = g1->getArea();
  double a2 = g2->getArea();
  double overlapArea = overlap->getArea();

  if (a1 == 0.0 || a2 == 0.0)
  {
    return 0.0;
  }

  return std::min(1.0, overlapArea / min(a1, a2));
}

}
