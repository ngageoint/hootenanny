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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CompactnessExtractor.h"

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/ElementConverter.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, CompactnessExtractor)

CompactnessExtractor::CompactnessExtractor()
{
}

double CompactnessExtractor::characteristic(Geometry* g) const
{
  double area = g->getArea();
  double perimeter = g->getLength();

  if (perimeter == 0.0)
  {
    return 1.0;
  }

  return 4 * M_PI * area / pow(perimeter, 2);
}

double CompactnessExtractor::extract(const OsmMap& map, const ConstElementPtr& target,
  const ConstElementPtr& candidate) const
{
  /*if (candidate->getElementType() == ElementType::Node)
  {
    LOG_DEBUG("candidate: " << candidate->toString());
  }
  if (target->getElementType() == ElementType::Node)
  {
    LOG_DEBUG("target: " << target->toString());
  }*/
  ElementConverter ec(map.shared_from_this());
  shared_ptr<Geometry> g1 = ec.convertToGeometry(target);
  shared_ptr<Geometry> g2 = ec.convertToGeometry(candidate);

  double result;
  if (g1->isEmpty() || g2->isEmpty())
  {
    result = nullValue();
  }
  else
  {
    result = 1 - fabs(characteristic(g1.get()) - characteristic(g2.get()));
  }

  return result;
}

}
