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
 * @copyright Copyright (C) 2016-2023 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonDistanceExtractor.h"

// hoot
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonDistanceExtractor)

PoiPolygonDistanceExtractor::PoiPolygonDistanceExtractor(PoiPolygonInfoCachePtr infoCache)
  : _infoCache(infoCache)
{
}

double PoiPolygonDistanceExtractor::extract(const OsmMap& map, const ConstElementPtr& poi,
                                            const ConstElementPtr& poly) const
{
  if (!_infoCache)
    _infoCache = make_shared<PoiPolygonInfoCache>(map.shared_from_this());

  LOG_VART(poi->getElementId());
  LOG_VART(poly->getElementId());

  // to suppress the ElementToGeometryConverter poly warnings...warnings worth looking into at some
  // point
  //DisableLog dl(Log::Warn);

  return _infoCache->getDistance(poly, poi);
}

}
