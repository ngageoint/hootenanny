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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonAlphaShapeDistanceExtractor.h"

// hoot
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/Factory.h>

// geos
#include <geos/geom/Geometry.h>
#include <geos/util/TopologyException.h>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonAlphaShapeDistanceExtractor)

double PoiPolygonAlphaShapeDistanceExtractor::extract(const OsmMap& map,
                                                      const ConstElementPtr& poi,
                                                      const ConstElementPtr& poly) const
{
  try
  {
    ElementToGeometryConverter elementToGeometryConverter(map.shared_from_this());
    std::shared_ptr<Geometry> polyGeom = elementToGeometryConverter.convertToGeometry(poly);
    if (polyGeom->isEmpty() ||
        QString::fromStdString(polyGeom->toString()).toUpper().contains("EMPTY"))
    {
      return -1.0;
    }
    std::shared_ptr<Geometry> poiGeom = elementToGeometryConverter.convertToGeometry(poi);
    if (poiGeom->isEmpty() ||
        QString::fromStdString(poiGeom->toString()).toUpper().contains("EMPTY"))
    {
      return -1.0;
    }

    ConstOsmMapPtr original = map.shared_from_this();
    OsmMapPtr polyMap = std::make_shared<OsmMap>();
    CopyMapSubsetOp(original, poly->getElementId()).apply(polyMap);
    AlphaShapeGenerator alphaShapeGenerator(1000.0, 0.0);
    std::shared_ptr<Geometry> polyAlphaShape = alphaShapeGenerator.generateGeometry(polyMap);
    // Oddly, even if the area is zero calc'ing the distance can have a positive effect. This may
    // be worth looking into at some point, but going with it for now.
    /*if (polyAlphaShape->getArea() == 0.0)
    {
      return -1.0;
    }*/
    return polyAlphaShape->distance(poiGeom.get());
  }
  catch (const geos::util::TopologyException& e)
  {
    LOG_TRACE(
      "Feature(s) passed to PoiPolygonMatchCreator caused topology exception on conversion "
      "to a geometry: " << poly->toString() << "\n" << poi->toString() << "\n" << e.what());
    return -1.0;
  }
}

}
