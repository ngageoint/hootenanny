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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CookieCutterOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, CookieCutterOp)

CookieCutterOp::CookieCutterOp() :
_alpha(1000.0),
_alphaShapeBuffer(0.0),
_crop(false),
_swapInputs(false)
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

void CookieCutterOp::apply(std::shared_ptr<OsmMap>& map)
{
  // This assumes that the incoming map has status Unknown1 for the replacement data and status
  // Unknown2 for the data being replaced. For inline changeset derivation with this op, this order
  // is reversed: use Unknown2 for the replacement data and Unknown1 for the data being replace
  // (TODO: this is weird).

  Status removeFromCutterMapStatus;
  Status removeFromDoughMapStatus;
//  if (!_swapInputs)  // TODO: probably remove this?
//  {
    removeFromCutterMapStatus = Status::Unknown2;
    removeFromDoughMapStatus = Status::Unknown1;
//  }
//  else
//  {
    //removeFromCutterMapStatus = Status::Unknown1;
    //removeFromDoughMapStatus = Status::Unknown2;
//  }

  // Remove Unknown2 out of the full input map and create a new map, which will be our cutter shape
  // map with data and having status = Unknown1.
  std::shared_ptr<OsmMap> cutterShapeMap(new OsmMap(map));
  RemoveElementsVisitor unknown2Remover;
  unknown2Remover.setRecursive(true);
  unknown2Remover.addCriterion(ElementCriterionPtr(new StatusCriterion(removeFromCutterMapStatus)));
  cutterShapeMap->visitRw(unknown2Remover);
  LOG_VARD(cutterShapeMap->getNodes().size());
  OsmMapWriterFactory::writeDebugMap(cutterShapeMap, "cookie-cutter-op-cutter-shape-map");

  // Create an alpha shape based on the map with the Unknown1 data to get our cookie cutter shape.
  std::shared_ptr<OsmMap> cutterShapeOutlineMap =
    AlphaShapeGenerator(_alpha, _alphaShapeBuffer).generateMap(cutterShapeMap);
  LOG_VARD(cutterShapeOutlineMap->getNodes().size());
  OsmMapWriterFactory::writeDebugMap(
    cutterShapeOutlineMap, "cookie-cutter-op-cutter-shape-outline-map");

  // Remove Unknown1 out of the full input map and create a new map, which will be our dough map
  // having status Unknown2.
  std::shared_ptr<OsmMap> doughMap(new OsmMap(map));
  RemoveElementsVisitor unknown1Remover;
  unknown1Remover.setRecursive(true);
  unknown1Remover.addCriterion(ElementCriterionPtr(new StatusCriterion(removeFromDoughMapStatus)));
  doughMap->visitRw(unknown1Remover);
  LOG_VARD(doughMap->getNodes().size());
  OsmMapWriterFactory::writeDebugMap(doughMap, "cookie-cutter-op-dough-map");

  // Cookie cut the outline shape obtained from the cutter shape map out of the dough map.
  CookieCutter(_crop, 0.0).cut(cutterShapeOutlineMap, doughMap);
  std::shared_ptr<OsmMap> cookieCutMap = doughMap;
  LOG_VARD(cookieCutMap->getNodes().size());
  OsmMapWriterFactory::writeDebugMap(cookieCutMap, "cookie-cutter-op-cookie-cut-map");

  // Combine the cutter shape map back into hole created by cookie cutting in the dough map.
  // Effectively, we've replaced all of the data in the dough map whose AOI coincides with the
  // cutter shape map with the cutter shape map's data (or vice versa for when running inline with
  // changeset derivation).
  cutterShapeMap->setProjection(cookieCutMap->getProjection()); // TODO: I think this is causing projection problems.
  cutterShapeMap->append(cookieCutMap);
  std::shared_ptr<OsmMap> result = cutterShapeMap;
  LOG_VARD(result->getNodes().size());
  map.reset(new OsmMap(result));
  OsmMapWriterFactory::writeDebugMap(map, "cookie-cutter-op-final-map");
}

}
