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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "CookieCutter.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/conflate/SuperfluousWayRemover.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/visitors/UnionPolygonsVisitor.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

unsigned int CookieCutter::logWarnCount = 0;

CookieCutter::CookieCutter(bool crop, double outputBuffer) :
_crop(crop),
_outputBuffer(outputBuffer)
{
}

void CookieCutter::cut(OsmMapPtr cutterShapeMap, OsmMapPtr doughMap)
{
  OGREnvelope env = CalculateMapBoundsVisitor::getBounds(cutterShapeMap);
  env.Merge(CalculateMapBoundsVisitor::getBounds(doughMap));

  // reproject the dough and cutter into the same planar projection.
  MapProjector::projectToPlanar(doughMap, env);
  MapProjector::projectToPlanar(cutterShapeMap, env);

  // create a complex geometry representing the alpha shape
  UnionPolygonsVisitor v;
  cutterShapeMap->visitRo(v);
  boost::shared_ptr<Geometry> cutterShape = v.getUnion();

  if (_outputBuffer != 0.0)
  {
    cutterShape.reset(cutterShape->buffer(_outputBuffer));
  }

  if (cutterShape->getArea() == 0.0)
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("Cutter area is zero. Try increasing the buffer size or check the input.");
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  // free up a little RAM
  cutterShapeMap.reset();
  // remove the cookie cutter portion from the "dough"
  // if crop is true, then the cookie cutter portion is kept and the "dough" is dropped.
  MapCropper::crop(doughMap, cutterShape, !_crop);
  // clean up any ugly bits left over
  SuperfluousWayRemover::removeWays(doughMap);
  SuperfluousNodeRemover::removeNodes(doughMap);
}

}
