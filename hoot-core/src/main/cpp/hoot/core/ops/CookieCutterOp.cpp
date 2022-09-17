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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "CookieCutterOp.h"

// hoot
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, CookieCutterOp)

CookieCutterOp::CookieCutterOp()
  : _alpha(1000.0),
    _alphaShapeBuffer(0.0),
    _crop(false)
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
  LOG_VARD(map->getNodeCount());
  LOG_VARD(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, className(), "input-map");

  // This assumes that the incoming map has status Unknown1 for the replacement data and status
  // Unknown2 for the data being replaced.

  const Status cutterMapStatus = Status::Unknown1;
  const Status doughMapStatus = Status::Unknown2;

  // When working on Cut and Replace, it seemed like there were several places in here where data
  // was being projected properly. This class didn't end up being used to support that command, so
  // I've commented out the reprojection code that was temporarily needed. It may be worth taking
  // another look at CookieCutterOp to make sure its behaving correctly regarding the input/output
  // data projections

  // Remove elements with the dough status out of the full input map and create a new map, which
  // will be our cutter shape map.
  std::shared_ptr<OsmMap> cutterShapeMap = std::make_shared<OsmMap>(map);
  RemoveElementsVisitor doughRemover;
  doughRemover.setRecursive(true);
  doughRemover.addCriterion(std::make_shared<StatusCriterion>(doughMapStatus));
  cutterShapeMap->visitRw(doughRemover);
  LOG_VARD(cutterShapeMap->getNodes().size());
  LOG_VARD(MapProjector::toWkt(cutterShapeMap->getProjection()));
  OsmMapWriterFactory::writeDebugMap(cutterShapeMap, className(), "cutter-shape-map");

  // Create an alpha shape based on the map with the cutter shape data to get our cookie cutter
  // shape outline.
  std::shared_ptr<OsmMap> cutterShapeOutlineMap = AlphaShapeGenerator(_alpha, _alphaShapeBuffer).generateMap(cutterShapeMap);

  // Remove elements with the cutter shape status create a new map, which will be our dough map.
  std::shared_ptr<OsmMap> doughMap = std::make_shared<OsmMap>(map);
  RemoveElementsVisitor cutterShapeRemover;
  cutterShapeRemover.setRecursive(true);
  cutterShapeRemover.addCriterion(std::make_shared<StatusCriterion>(cutterMapStatus));
  doughMap->visitRw(cutterShapeRemover);

  // Cut the outline cutter shape obtained from the cutter shape map out of the dough map, leaving
  // a hole in it (or if _crop=false, drop what's around the outline shape instead).
  CookieCutter(_crop, 0.0).cut(cutterShapeOutlineMap, doughMap);
  std::shared_ptr<OsmMap> cookieCutMap = doughMap;

  // Combine the cutter shape map with the dough map by adding the cutter shape data back into the
  // hole created by cookie cutting the dough map.
  cutterShapeMap->append(cookieCutMap);
  std::shared_ptr<OsmMap> result = cutterShapeMap;
  map = std::make_shared<OsmMap>(result);
  LOG_VARD(map->getNodes().size());
  LOG_VARD(MapProjector::toWkt(map->getProjection()));
  OsmMapWriterFactory::writeDebugMap(map, className(), "final-combined-map");
}

}
