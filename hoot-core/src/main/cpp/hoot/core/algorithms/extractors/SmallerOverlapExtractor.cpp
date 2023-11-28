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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */
#include "SmallerOverlapExtractor.h"

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, SmallerOverlapExtractor)

SmallerOverlapExtractor::SmallerOverlapExtractor()
  : _requireAreaForPolygonConversion(true)
{
}

void SmallerOverlapExtractor::setConfiguration(const Settings& conf)
{
  _requireAreaForPolygonConversion = ConfigOptions(conf).getConvertRequireAreaForPolygon();
}

double SmallerOverlapExtractor::extract(const OsmMap& map, const ConstElementPtr& target,
                                        const ConstElementPtr& candidate) const
{
  LOG_VART(target->getElementId());
  LOG_VART(candidate->getElementId());

  ElementToGeometryConverter ec(map.shared_from_this());
  ec.setRequireAreaForPolygonConversion(_requireAreaForPolygonConversion);
  std::shared_ptr<Geometry> g1 = ec.convertToGeometry(target, false);
  if (g1->isEmpty())
    return nullValue();
  std::shared_ptr<Geometry> g2 = ec.convertToGeometry(candidate, false);
  if (g2->isEmpty())
    return nullValue();

  std::shared_ptr<Geometry> overlap;
  try
  {
    overlap = g1->intersection(g2.get());
  }
  catch (const std::runtime_error&)
  {
    //  Validate and intersect
    overlap = _validate(g1, g2);
  }

  if (!overlap)
    return 0.0;

  double a1 = g1->getArea();
  double a2 = g2->getArea();
  double overlapArea = overlap->getArea();

  LOG_VART(a1);
  LOG_VART(a2);
  LOG_VART(overlapArea);

  if (a1 == 0.0 || a2 == 0.0)
    return 0.0;

  return std::min(1.0, overlapArea / min(a1, a2));
}

std::shared_ptr<Geometry> SmallerOverlapExtractor::_validate(std::shared_ptr<Geometry>& g1, std::shared_ptr<Geometry>& g2) const
{
  try
  {
    g1.reset(GeometryUtils::validateGeometry(g1.get()));
    g2.reset(GeometryUtils::validateGeometry(g2.get()));
    return g2->intersection(g1.get());
  }
  catch (const std::runtime_error&)
  {
    return nullptr;
  }
}


}
