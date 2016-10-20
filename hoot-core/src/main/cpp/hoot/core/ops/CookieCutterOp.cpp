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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CookieCutterOp.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/conflate/AlphaShapeGenerator.h>
#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, CookieCutterOp)

CookieCutterOp::CookieCutterOp()
{
  setConfiguration(conf());
}

void CookieCutterOp::setConfiguration(const Settings& conf)
{
  ConfigOptions config(conf);
  setAlpha(config.getCookieCutterAlpha());
  setAlphaShapeBuffer(config.getCookieCutterAlphaShapeBuffer());
  setCrop(config.getCookieCutterOutputCrop());
}

void CookieCutterOp::apply(shared_ptr<OsmMap>& map)
{
  //remove unknown2 out of the input map and create a new map, which will be our ref map
  OsmMapPtr refMap(new OsmMap(map));
  RemoveElementsVisitor unknown2Remover(ElementCriterionPtr(new StatusCriterion(Status::Unknown2)));
  unknown2Remover.setRecursive(true);
  refMap->visitRw(unknown2Remover);
  LOG_VARD(refMap->getNodeMap().size());

  //create an alpha shape based on the ref map (unknown1)
  OsmMapPtr cutShapeMap = AlphaShapeGenerator(_alpha, _alphaShapeBuffer).generateMap(refMap);
  LOG_VARD(cutShapeMap->getNodeMap().size());

  //remove unknown1 out of the input and create a new map, which will be our source map (unknown2)
  OsmMapPtr doughMap(new OsmMap(map));
  RemoveElementsVisitor unknown1Remover(ElementCriterionPtr(new StatusCriterion(Status::Unknown1)));
  unknown1Remover.setRecursive(true);
  doughMap->visitRw(unknown1Remover);
  LOG_VARD(doughMap->getNodeMap().size());

  //cookie cut the alpha shape obtained from the ref map out of the source map
  CookieCutter(_crop, 0.0).cut(cutShapeMap, doughMap);
  OsmMapPtr cookieCutMap = doughMap;
  LOG_VARD(cookieCutMap->getNodeMap().size());

  //combine the ref map back with the source map; Effectively, we've replaced all of the data in the
  //source map whose AOI coincides with the ref map with the ref map's data.
  refMap->setProjection(cookieCutMap->getProjection());
  refMap->append(cookieCutMap);
  OsmMapPtr result = refMap;
  LOG_VARD(result->getNodeMap().size());
  map.reset(new OsmMap(result));
}

}
