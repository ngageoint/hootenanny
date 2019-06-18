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
#include "CookieCutConflator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/CookieCutterOp.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/visitors/CalculateHashVisitor2.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, CookieCutConflator)

CookieCutConflator::CookieCutConflator()
{
}

void CookieCutConflator::apply(std::shared_ptr<OsmMap>& map)
{
  CalculateHashVisitor2 hashVis;
  CookieCutterOp mapGenerator;

  LOG_DEBUG("Generating replacement map...");
  mapGenerator.setSwapInputs(false);
  //mapGenerator.setCrop(false);
  OsmMapPtr replacementMap(new OsmMap(map));
  mapGenerator.apply(replacementMap);
  replacementMap->visitRw(hashVis);
  MapProjector::projectToWgs84(replacementMap);
  OsmMapWriterFactory::writeDebugMap(replacementMap, "cookie-cut-conflate-replacement-map");

  LOG_DEBUG("Generating map to replace...");
  mapGenerator.setSwapInputs(true);
  //mapGenerator.setCrop(true);
  OsmMapPtr mapBeingReplaced(new OsmMap(map));
  mapGenerator.apply(mapBeingReplaced);
  mapBeingReplaced->visitRw(hashVis);
  MapProjector::projectToWgs84(mapBeingReplaced);
  OsmMapWriterFactory::writeDebugMap(mapBeingReplaced, "cookie-cut-conflate-being-replaced-map");

  LOG_DEBUG("Combining maps and conflating...");
  mapBeingReplaced->append(replacementMap);
  UnifyingConflator conflator;
  conflator.setConfiguration(conf());
  conflator.apply(mapBeingReplaced);
  map = mapBeingReplaced;
}

}
