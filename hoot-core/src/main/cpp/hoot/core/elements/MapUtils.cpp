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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

#include "MapUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/visitors/RemoveUnknownVisitor.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

OsmMapPtr MapUtils::getMapSubset(const ConstOsmMapPtr& map, const ElementCriterionPtr& filter,
                                 const bool copyChildren)
{
  CopyMapSubsetOp mapCopier(map, filter);
  mapCopier.setCopyChildren(copyChildren);
  OsmMapPtr output(new OsmMap());
  mapCopier.apply(output);
  return output;
}

bool MapUtils::mapIsPointsOnly(const OsmMapPtr& map)
{
  std::shared_ptr<PointCriterion> pointCrit(new PointCriterion());
  pointCrit->setOsmMap(map.get());
  return
    (int)FilteredVisitor::getStat(
      pointCrit,
      ElementVisitorPtr(new ElementCountVisitor()), map) == (int)map->getElementCount();
}

void MapUtils::combineMaps(OsmMapPtr& map1, OsmMapPtr& map2, const bool throwOutDupes)
{
  LOG_VART(map1.get());
  LOG_VART(map2.get());

  MapProjector::projectToWgs84(map1);
  MapProjector::projectToWgs84(map2);   // not exactly sure yet why this needs to be done

  if (map2->size() == 0)
  {
    LOG_DEBUG("Combined map size: " << map1->size());
    return;
  }

  LOG_INFO("Combining maps: " << map1->getName() << " and " << map2->getName() << "...");
  map1->append(map2, throwOutDupes);
  LOG_VART(MapProjector::toWkt(map1->getProjection()));
  LOG_DEBUG("Combined map size: " << map1->size());
}


}
