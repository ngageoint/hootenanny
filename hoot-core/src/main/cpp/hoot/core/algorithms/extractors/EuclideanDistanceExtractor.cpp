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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "EuclideanDistanceExtractor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, EuclideanDistanceExtractor)

EuclideanDistanceExtractor::EuclideanDistanceExtractor() :
_requireAreaForPolygonConversion(true)
{
}

void EuclideanDistanceExtractor::setConfiguration(const Settings& conf)
{
  _requireAreaForPolygonConversion = ConfigOptions(conf).getConvertRequireAreaForPolygon();
}

double EuclideanDistanceExtractor::distance(
  const OsmMap& map, const ConstElementPtr& target, const ConstElementPtr& candidate) const
{
  ElementToGeometryConverter ec(map.shared_from_this());
  ec.setRequireAreaForPolygonConversion(_requireAreaForPolygonConversion);
  std::shared_ptr<Geometry> g1 = ec.convertToGeometry(target);
  std::shared_ptr<Geometry> g2 = ec.convertToGeometry(candidate);

  if (g1->isEmpty() || g2->isEmpty())
  {
    return nullValue();
  }

  g1.reset(GeometryUtils::validateGeometry(g1.get()));
  g2.reset(GeometryUtils::validateGeometry(g2.get()));

  if (g1.get() == nullptr || g2.get() == nullptr)
  {
    return nullValue();
  }

  return g1->distance(g2.get());
}

double EuclideanDistanceExtractor::distance(
  const OsmMap& map1, const OsmMap& map2, const ConstElementPtr& target,
  const ConstElementPtr& candidate) const
{
  ElementToGeometryConverter ec1(map1.shared_from_this());
  ec1.setRequireAreaForPolygonConversion(_requireAreaForPolygonConversion);
  std::shared_ptr<Geometry> g1 = ec1.convertToGeometry(target);
  if (g1->isEmpty())
  {
    return nullValue();
  }

  ElementToGeometryConverter ec2(map2.shared_from_this());
  ec2.setRequireAreaForPolygonConversion(_requireAreaForPolygonConversion);
  std::shared_ptr<Geometry> g2 = ec2.convertToGeometry(candidate);
  if (g2->isEmpty())
  {
    return nullValue();
  }

  g1.reset(GeometryUtils::validateGeometry(g1.get()));
  g2.reset(GeometryUtils::validateGeometry(g2.get()));

  if (g1.get() == nullptr || g2.get() == nullptr)
  {
    return nullValue();
  }

  LOG_VART(g1->toString());
  LOG_VART(g2->toString());

  return g1->distance(g2.get());
}

}
