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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiBuildingMerger.h"

// hoot
#include "PoiPolygonMerger.h"
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

PoiBuildingMerger::PoiBuildingMerger()
{

}

void PoiBuildingMerger::merge(OsmMapPtr map, const int poiId, const int buildingId)
{
  //there should be one poi node and one building, either a way or a relation

  NodePtr poi = map->getNode(poiId);
  if (!poi.get() || !OsmSchema::getInstance().isPoi(*poi))
  {
    throw IllegalArgumentException(
      "Invalid POI passed to POI/building merger.  ID: " + poiId);
  }
  ElementId poiElementId = ElementId::node(poiId);

  //if a relation was passed in at all, the building must be that relation
  ElementId buildingElementId;
  ElementPtr building;
  if (map->getRelationMap().size() > 0)
  {
    building = map->getRelation(buildingId);
    buildingElementId = ElementId::relation(buildingId);
  }
  //otherwise, its a way
  else
  {
    building = map->getWay(buildingId);
    buildingElementId = ElementId::way(buildingId);
  }
  if (!building.get() || !OsmSchema::getInstance().isBuilding(building))
  {
     throw IllegalArgumentException(
       "Invalid building passed to POI/building merger.  ID: " + poiId);
  }

  std::set<std::pair<ElementId, ElementId> > pairs;
  pairs.insert(std::pair<ElementId, ElementId>(poiElementId, buildingElementId));
  PoiPolygonMerger merger(pairs);
  //OsmMapPtr mergedMap(map);
  std::vector<std::pair<ElementId, ElementId> > replacedElements;
  merger.apply(/*mergedMap*/map, replacedElements);

  //TODO: fix - I think the poi's always get merged into the building poly's, so only
  //the building poly's would need their id's updated here, but I need to verify
  LOG_VARD(replacedElements.size());
  LOG_VARD(replacedElements);
  /*if (replacedElements.size() == 1)
  {
    mergedMap->replacedElements(
      replacedElements[0].first.getId(), replacedElements[0].second.getId());
  }*/
}

}

