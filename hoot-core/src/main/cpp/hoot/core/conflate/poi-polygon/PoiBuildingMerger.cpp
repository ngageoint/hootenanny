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

void PoiBuildingMerger::merge(OsmMapPtr map)
{
  //there should be one poi node and one building poly, which is either a way or a relation

  int poiCount = 0;
  ElementId poiElementId;
  NodeMap::const_iterator nodeItr = map->getNodeMap().begin();
  while (nodeItr != map->getNodeMap().end())
  {
    const int nodeId = nodeItr->first;
    if (OsmSchema::getInstance().isPoi(*map->getNode(nodeId)))
    {
      poiElementId = ElementId::node(nodeId);
      poiCount++;
    }
    nodeItr++;
  }
  if (poiCount == 0)
  {
    throw IllegalArgumentException("No POI passed to POI/building merger.");
  }
  if (poiCount > 1)
  {
    throw IllegalArgumentException("More than one POI passed to POI/building merger.");
  }

  int buildingCount = 0;
  ElementId buildingElementId;
  WayMap::const_iterator wayItr = map->getWays().begin();
  while (wayItr != map->getWays().end())
  {
    const int wayId = wayItr->first;
    if (OsmSchema::getInstance().isBuilding(map->getWay(wayId)))
    {
      buildingElementId = ElementId::way(wayId);
      buildingCount++;
    }
    wayItr++;
  }
  if (buildingElementId.isNull())
  {
    RelationMap::const_iterator relItr = map->getRelationMap().begin();
    while (relItr != map->getRelationMap().end())
    {
      const int relationId = relItr->first;
      if (OsmSchema::getInstance().isBuilding(map->getRelation(relationId)))
      {
        buildingElementId = ElementId::relation(relationId);
        buildingCount++;
      }
      relItr++;
    }
  }
  if (buildingCount == 0)
  {
    throw IllegalArgumentException("No building passed to POI/building merger.");
  }
  if (buildingCount > 1)
  {
    throw IllegalArgumentException("More than one building passed to POI/building merger.");
  }

  std::set<std::pair<ElementId, ElementId> > pairs;
  pairs.insert(std::pair<ElementId, ElementId>(poiElementId, buildingElementId));
  PoiPolygonMerger merger(pairs);
  std::vector<std::pair<ElementId, ElementId> > replacedElements;
  merger.apply(map, replacedElements);
}

}

