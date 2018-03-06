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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "AreaMerger.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>

namespace hoot
{

AreaMerger::AreaMerger()
{
}

ElementId AreaMerger::merge(OsmMapPtr map)
{
  //there should be exactly two area polygons
  LOG_INFO("Merging two areas...");

  const int featureCount = map->getElementCount();
  if (featureCount != 2)
  {
    throw IllegalArgumentException("Two features were not passed to the area merger.");
  }

  ElementPtr element1;
  ElementPtr element2;
  int parsedAreaCount = 0;

  //find the area(s) as ways
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const int wayId = wayItr->first;
    //Buildings are also defined as areas, and we want this to work independently of the building
    //conflation process.
    if (OsmSchema::getInstance().isNonBuildingArea(map->getWay(wayId)))
    {
      if (!element1.get())
      {
        element1 = map->getElement(ElementType::Way, wayId);
      }
      else
      {
        element2 = map->getElement(ElementType::Way, wayId);
      }
      parsedAreaCount++;
    }
  }

  if (parsedAreaCount < 2)
  {
    //find the area(s) as relations
    const RelationMap& relations = map->getRelations();
    for (RelationMap::const_iterator relItr = relations.begin(); relItr != map->getRelations().end();
         ++relItr)
    {
      const int relationId = relItr->first;
      if (OsmSchema::getInstance().isNonBuildingArea(map->getRelation(relationId)))
      {
        if (!element1.get())
        {
          element1 = map->getElement(ElementType::Relation, relationId);
        }
        else
        {
          element2 = map->getElement(ElementType::Relation, relationId);
        }
        parsedAreaCount++;
      }
    }
  }

  if (parsedAreaCount != 2)
  {
    throw IllegalArgumentException("Two areas were not passed to the area merger.");
  }

  LOG_VART(element1);
  LOG_VART(element2);

  //simply keeping the ref geometry and tags; may need something more robust eventually
  const Tags mergedTags =
    TagMergerFactory::mergeTags(element1->getTags(), element2->getTags(), ElementType::Unknown);
  element1->setTags(mergedTags);
  RecursiveElementRemover(element2->getElementId()).apply(map);

  //TODO: should determine ref from passed in element statuses instead
  return element1->getElementId();
}

}
