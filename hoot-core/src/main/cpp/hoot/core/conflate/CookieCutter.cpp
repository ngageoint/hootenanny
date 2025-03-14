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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Maxar (http://www.maxar.com/)
 */

#include "CookieCutter.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/visitors/UnionPolygonsVisitor.h>

using namespace geos::geom;

namespace hoot
{

CookieCutter::CookieCutter(bool crop, double outputBuffer, bool keepEntireFeaturesCrossingBounds,
                           bool keepOnlyFeaturesInsideBounds, bool removeMissingElements)
  : _crop(crop),
    _outputBuffer(outputBuffer),
    _keepEntireFeaturesCrossingBounds(keepEntireFeaturesCrossingBounds),
    _keepOnlyFeaturesInsideBounds(keepOnlyFeaturesInsideBounds),
    _removeMissingElements(removeMissingElements)
{
}

void CookieCutter::cut(OsmMapPtr& cutterShapeOutlineMap, OsmMapPtr& doughMap) const
{
  LOG_VARD(cutterShapeOutlineMap->getNodeCount());
  LOG_VART(MapProjector::toWkt(cutterShapeOutlineMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(cutterShapeOutlineMap, className(), "cutter-shape-outline-map");
  LOG_VARD(doughMap->getNodeCount());
  LOG_VART(MapProjector::toWkt(doughMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(doughMap, className(), "dough-map");

  OGREnvelope env = CalculateMapBoundsVisitor::getBounds(cutterShapeOutlineMap);
  LOG_VARD(GeometryUtils::toEnvelope(env)->toString());
  env.Merge(CalculateMapBoundsVisitor::getBounds(doughMap));

  // reproject the dough and cutter into the same planar projection.
  MapProjector::projectToPlanar(doughMap, env);
  MapProjector::projectToPlanar(cutterShapeOutlineMap, env);

  // create a complex geometry representing the alpha shape
  UnionPolygonsVisitor v;
  cutterShapeOutlineMap->visitRo(v);
  std::shared_ptr<Geometry> cutterShape = v.getUnion();
  if (_outputBuffer != 0.0)
    cutterShape = cutterShape->buffer(_outputBuffer);
  // would rather this be thrown than a warning logged, as the warning may go unoticed by web
  // clients who are expecting the cookie cutting to occur...
  if (cutterShape->getArea() == 0.0)
    throw HootException("Cutter area is zero. Try increasing the buffer size or check the input.");
  LOG_VART(cutterShape->toString());

  // free up a little RAM
  cutterShapeOutlineMap.reset();

  // remove the cookie cutter portion from the dough
  MapCropper cropper;
  cropper.setConfiguration(conf());
  cropper.setBounds(cutterShape);
  cropper.setKeepEntireFeaturesCrossingBounds(_keepEntireFeaturesCrossingBounds);
  cropper.setKeepOnlyFeaturesInsideBounds(_keepOnlyFeaturesInsideBounds);
  cropper.setInvert(!_crop);
  cropper.setRemoveSuperflousFeatures(true);
  cropper.setRemoveMissingElements(_removeMissingElements);
  LOG_STATUS(cropper.getInitStatusMessage());
  cropper.apply(doughMap);
  LOG_DEBUG(cropper.getCompletedStatusMessage());

  OsmMapPtr cookieCutMap = doughMap;
  LOG_VARD(cookieCutMap->getNodeCount());
  LOG_VART(MapProjector::toWkt(cookieCutMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(cookieCutMap, className(), "cookie-cut-map");
}

}
