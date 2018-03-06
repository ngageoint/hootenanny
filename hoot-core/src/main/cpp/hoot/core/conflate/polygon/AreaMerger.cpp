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
  //there should be exactly two area polygons passed in, one ref and one secondary

  LOG_INFO("Merging two areas...");

  const int featureCount = map->getElementCount();
  if (featureCount != 2)
  {
    throw IllegalArgumentException(
      "The Area Merger expects two areas passed to it.  Number passed: " +
      QString::number(featureCount));
  }

  ElementPtr refArea;
  ElementPtr secArea;

  //find the areas as ways
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const long wayId = wayItr->first;
    const ConstWayPtr way = map->getWay(wayId);
    //Buildings are also defined as areas, and we want this to work independently of the building
    //conflation process.  So, use isNonBuildingArea instead of isArea here.
    if (way.get() && OsmSchema::getInstance().isNonBuildingArea(way))
    {
      if (way->getStatus() == Status::Unknown1)
      {
        if (!refArea.get())
        {
          refArea.reset(new Way(*way));
        }
      }
      else if (way->getStatus() == Status::Unknown2)
      {
        if (!secArea.get())
        {
          secArea.reset(new Way(*way));
        }
      }
      else
      {
        throw IllegalArgumentException(
          "Passed a way with an invalid status to the Area Merger: " + way->getStatus().toString());
      }
    }
  }

  if (!refArea.get() || !secArea.get())
  {
    //find the areas as relations
    const RelationMap& relations = map->getRelations();
    for (RelationMap::const_iterator relItr = relations.begin(); relItr != map->getRelations().end();
         ++relItr)
    {
      const long relationId = relItr->first;
      const ConstRelationPtr relation = map->getRelation(relationId);
      //see comment above regarding use of isNonBuildingArea here
      if (relation.get() && OsmSchema::getInstance().isNonBuildingArea(relation))
      {
        if (relation->getStatus() == Status::Unknown1)
        {
          if (!refArea.get())
          {
            refArea.reset(new Relation(*relation));
          }
        }
        else if (relation->getStatus() == Status::Unknown2)
        {
          if (!secArea.get())
          {
            secArea.reset(new Relation(*relation));
          }
        }
        else
        {
          throw IllegalArgumentException(
            "Passed a relation with an invalid status to the Area Merger: " +
            relation->getStatus().toString());
        }
      }
    }
  }

  if (!refArea.get())
  {
    throw IllegalArgumentException("No reference area was passed to the Area Merger.");
  }
  if (!secArea.get())
  {
    throw IllegalArgumentException("No secondary area was passed to the Area Merger.");
  }

  assert(refArea.get() && secArea.get());
  LOG_VART(refArea);
  LOG_VART(secArea);

  //simply keeping the ref geometry and tags right now; may need a more robust way to merge
  //eventually
  const Tags mergedTags =
    TagMergerFactory::mergeTags(refArea->getTags(), secArea->getTags(), ElementType::Unknown);
  refArea->setTags(mergedTags);
  RecursiveElementRemover(secArea->getElementId()).apply(map);
  LOG_VART(map->getElementCount());

  return refArea->getElementId();
}

}
