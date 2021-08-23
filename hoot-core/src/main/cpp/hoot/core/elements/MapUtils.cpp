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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "MapUtils.h"

// Hoot


#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/visitors/RemoveUnknownVisitor.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>

// Std


namespace hoot
{

OsmMapPtr MapUtils::getMapSubset(const ConstOsmMapPtr& map, const ElementCriterionPtr& filter,
                                 const bool copyChildren)
{
  CopyMapSubsetOp mapCopier(map, filter);
  mapCopier.setCopyChildren(copyChildren);
  OsmMapPtr output = std::make_shared<OsmMap>();
  mapCopier.apply(output);
  return output;
}

bool MapUtils::mapIsPointsOnly(const OsmMapPtr& map)
{
  std::shared_ptr<PointCriterion> pointCrit = std::make_shared<PointCriterion>();
  pointCrit->setOsmMap(map.get());
  return
    (int)FilteredVisitor::getStat(
      pointCrit, std::make_shared<ElementCountVisitor>(), map) == (int)map->getElementCount();
}

void MapUtils::combineMaps(const OsmMapPtr& map1, const OsmMapPtr& map2, const bool throwOutDupes)
{
  LOG_VART(map1.get());
  LOG_VART(map2.get());

  MapProjector::projectToWgs84(map1);
  MapProjector::projectToWgs84(map2);   // not exactly sure yet why this needs to be done

  if (map2->isEmpty())
  {
    LOG_DEBUG("Combined map size: " << map1->size());
    return;
  }

  LOG_INFO("Combining maps: " << map1->getName() << " and " << map2->getName() << "...");
  map1->append(map2, throwOutDupes);
  LOG_VART(MapProjector::toWkt(map1->getProjection()));
  LOG_DEBUG("Combined map size: " << map1->size());
}

ElementPtr MapUtils::getFirstElementWithNote(
  const OsmMapPtr& map, const QString& note, const ElementType& elementType)
{
  return getFirstElementWithTag(map, "note", note, elementType);
}

ElementPtr MapUtils::getFirstElementWithTag(
  const OsmMapPtr& map, const QString& tagKey, const QString& tagValue,
  const ElementType& elementType)
{
  TagCriterion tc(tagKey, tagValue);
  UniqueElementIdVisitor v;
  FilteredVisitor fv(tc, v);
  map->visitRo(fv);
  const std::set<ElementId> elementIds = v.getElementSet();

  if (elementIds.empty())
  {
    return ElementPtr();
  }

  if (elementType == ElementType::Unknown)
  {
    return map->getElement(*elementIds.begin());
  }
  else
  {
    for (std::set<ElementId>::const_iterator itr = elementIds.begin(); itr != elementIds.end();
         ++itr)
    {
      ElementPtr element = map->getElement(*itr);
      if (element->getElementType() == elementType)
      {
        return element;
      }
    }
  }
  return ElementPtr();
}

}
