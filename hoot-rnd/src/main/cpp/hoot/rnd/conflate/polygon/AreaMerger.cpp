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
#include "AreaMerger.h"

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>


namespace hoot
{

AreaMerger::AreaMerger()
{
}

void AreaMerger::merge(OsmMapPtr map)
{
  //var newTags = hoot.TagMergerFactory.mergeTags(e1.getTags(), e2.getTags());
  //e1.setTags(newTags);
  //new hoot.RecursiveElementRemover(e2).apply(map);

  //there should be exactly two area polygons

  int areaCount = map->getElementCount();
  if (areaCount != 2)
  {
    throw IllegalArgumentException("Two areas were not passed to the area merger.");
  }

  ElementPtr element1;
  ElementPtr element2;
  int parsedElementCount = 0;

  WayMap::const_iterator wayItr = map->getWays().begin();
  while (wayItr != map->getWays().end())
  {
    const int wayId = wayItr->first;
    if (PoiPolygonMatch::isArea(*map->getWay(wayId)))
    {
      if (!element1.get())
      {
        element1 = map->getElement(ElementType::Way, wayId);
      }
      else
      {
        element2 = map->getElement(ElementType::Way, wayId);
      }
      parsedElementCount++;
    }
    wayItr++;
  }

  if (parsedElementCount < 2)
  {
    RelationMap::const_iterator relItr = map->getRelationMap().begin();
    while (relItr != map->getRelationMap().end())
    {
      const int relationId = relItr->first;
      if (PoiPolygonMatch::isArea(*map->getRelation(relationId)))
      {
        if (!element1.get())
        {
          element1 = map->getElement(ElementType::Relation, relationId);
        }
        else
        {
          element2 = map->getElement(ElementType::Relation, relationId);
        }
        parsedElementCount++;
      }
      relItr++;
    }
  }

  Tags mergedTags =
    TagMergerFactory::mergeTags(element1->getTags(), element2->getTags(), ElementType::Unknown);
  element1->setTags(mergedTags);
  RecursiveElementRemover(element2->getElementId()).apply(map);
}

}
