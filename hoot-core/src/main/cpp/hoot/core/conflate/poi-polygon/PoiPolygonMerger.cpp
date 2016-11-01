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
#include "PoiPolygonMerger.h"

// hoot
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>

#include "PoiPolygonMatch.h"

namespace hoot
{

PoiPolygonMerger::PoiPolygonMerger(const set< pair<ElementId, ElementId> >& pairs) :
_pairs(pairs)
{
  assert(_pairs.size() >= 1);
}

void PoiPolygonMerger::apply(const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  ////
  /// See "Hootenanny - POI to Building" powerpoint for more details.
  ////

  // merge all POI tags first, but keep Unknown1 and Unknown2 separate. It is implicitly assumed
  // that since they're in a single group they all represent the same entity.
  Tags poiTags1 = _mergePoiTags(map, Status::Unknown1);
  Tags poiTags2 = _mergePoiTags(map, Status::Unknown2);

  // Get all the building parts for each status
  vector<ElementId> buildings1 = _getBuildingParts(map, Status::Unknown1);
  vector<ElementId> buildings2 = _getBuildingParts(map, Status::Unknown2);

  // Merge all the building parts together into a single building entity using the typical building
  // merge process.
  ElementId finalBuildingEid = _mergeBuildings(map, buildings1, buildings2, replaced);
  //LOG_VARD(finalBuildingEid);

  ElementPtr finalBuilding = map->getElement(finalBuildingEid);
  if (!finalBuilding.get())
  {
    //building merger must not have been able to merge...maybe need an earlier check for this
    //and also handle it differently...
    return;
  }
  assert(finalBuilding.get());

  Tags finalBuildingTags = finalBuilding->getTags();
  if (poiTags1.size())
  {
    finalBuildingTags = TagMergerFactory::getInstance().mergeTags(poiTags1, finalBuildingTags,
      finalBuilding->getElementType());
  }
  if (poiTags2.size())
  {
    finalBuildingTags = TagMergerFactory::getInstance().mergeTags(finalBuildingTags,
      poiTags2, finalBuilding->getElementType());
  }
  finalBuilding->setTags(finalBuildingTags);

  // do some book keeping to remove the POIs and mark them as replaced.
  for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin(); it != _pairs.end();
       ++it)
  {
    const pair<ElementId, ElementId>& p = *it;
    if (p.first.getType() == ElementType::Node)
    {
      replaced.push_back(pair<ElementId, ElementId>(p.first, finalBuildingEid));
      // clear the tags just in case it is part of a way
      if (map->containsElement(p.first))
      {
        map->getElement(p.first)->getTags().clear();
        RecursiveElementRemover(p.first).apply(map);
      }
    }

    if (p.second.getType() == ElementType::Node)
    {
      replaced.push_back(pair<ElementId, ElementId>(p.second, finalBuildingEid));
      // clear the tags just in case it is part of a way
      if (map->containsElement(p.second))
      {
        map->getElement(p.second)->getTags().clear();
        RecursiveElementRemover(p.second).apply(map);
      }
    }
  }
}

Tags PoiPolygonMerger::_mergePoiTags(const OsmMapPtr& map, Status s) const
{
  Tags result;

  for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin(); it != _pairs.end();
       ++it)
  {
    const pair<ElementId, ElementId>& p = *it;
    ElementPtr e1 = map->getElement(p.first);
    ElementPtr e2 = map->getElement(p.second);
    if (e1->getStatus() == s && e1->getElementType() == ElementType::Node)
    {
      result = TagMergerFactory::getInstance().mergeTags(result, e1->getTags(),
        e1->getElementType());
    }
    if (e2->getStatus() == s && e2->getElementType() == ElementType::Node)
    {
      result = TagMergerFactory::getInstance().mergeTags(result, e2->getTags(),
        e2->getElementType());
    }
  }

  return result;
}

vector<ElementId> PoiPolygonMerger::_getBuildingParts(const OsmMapPtr& map, Status s) const
{
  vector<ElementId> result;

  for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin(); it != _pairs.end();
       ++it)
  {
    const pair<ElementId, ElementId>& p = *it;
    ElementPtr e1 = map->getElement(p.first);
    ElementPtr e2 = map->getElement(p.second);
    if (e1->getStatus() == s && e1->getElementType() != ElementType::Node)
    {
      result.push_back(e1->getElementId());
    }
    if (e2->getStatus() == s && e2->getElementType() != ElementType::Node)
    {
      result.push_back(e2->getElementId());
    }
  }

  return result;
}

ElementId PoiPolygonMerger::_mergeBuildings(const OsmMapPtr& map,
  vector<ElementId>& buildings1, vector<ElementId>& buildings2,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  set< pair<ElementId, ElementId> > pairs;

  assert(buildings1.size() != 0 || buildings2.size() != 0);
  // if there is only one set of buildings then there is no need to merge.
  if (buildings1.size() == 0)
  {
    // group all the building parts into a single building
    set<ElementId> eids;
    eids.insert(buildings2.begin(), buildings2.end());
    //LOG_VARD(eids);
    return BuildingMerger::buildBuilding(map, eids)->getElementId();
  }
  else if (buildings2.size() == 0)
  {
    // group all the building parts into a single building
    set<ElementId> eids;
    eids.insert(buildings1.begin(), buildings1.end());
    return BuildingMerger::buildBuilding(map, eids)->getElementId();
  }

  // the exact pairing doesn't matter for the building merger, it just breaks it back out into
  // two groups
  for (size_t i = 0; i < max(buildings1.size(), buildings2.size()); i++)
  {
    size_t i1 = min(i, buildings1.size() - 1);
    size_t i2 = min(i, buildings2.size() - 1);

    pair<ElementId, ElementId> p(buildings1[i1], buildings2[i2]);
    pairs.insert(p);
  }

  assert(replaced.size() == 0);
  //assert(pairs.size() == 0);  //This fails on a poi-building case test.
  BuildingMerger(pairs).apply(map, replaced);
  assert(replaced.size() > 0);

  set<ElementId> newElement;
  for (size_t i = 0; i < replaced.size(); i++)
  {
    newElement.insert(replaced[i].second);
  }
  assert(newElement.size() == 1);

  return *newElement.begin();
}

void PoiPolygonMerger::merge(OsmMapPtr map)
{
  //there should be one poi node and one building/area poly, which is either a way or a relation
  //in the input map

  int poiCount = 0;
  ElementId poiElementId;
  NodeMap::const_iterator nodeItr = map->getNodeMap().begin();
  while (nodeItr != map->getNodeMap().end())
  {
    const int nodeId = nodeItr->first;
    if (PoiPolygonMatch::isPoi(*map->getNode(nodeId)))
    {
      poiElementId = ElementId::node(nodeId);
      poiCount++;
    }
    nodeItr++;
  }
  if (poiCount == 0)
  {
    throw IllegalArgumentException("No POI passed to POI/Polygon merger.");
  }
  if (poiCount > 1)
  {
    throw IllegalArgumentException("More than one POI passed to POI/Polygon merger.");
  }

  int polyCount = 0;
  ElementId polyElementId;
  WayMap::const_iterator wayItr = map->getWays().begin();
  while (wayItr != map->getWays().end())
  {
    const int wayId = wayItr->first;
    if (PoiPolygonMatch::isPoly(*map->getWay(wayId)))
    {
      polyElementId = ElementId::way(wayId);
      polyCount++;
    }
    wayItr++;
  }
  if (polyElementId.isNull())
  {
    RelationMap::const_iterator relItr = map->getRelationMap().begin();
    while (relItr != map->getRelationMap().end())
    {
      const int relationId = relItr->first;
      if (PoiPolygonMatch::isPoly(*map->getRelation(relationId)))
      {
        polyElementId = ElementId::relation(relationId);
        polyCount++;
      }
      relItr++;
    }
  }
  if (polyCount == 0)
  {
    throw IllegalArgumentException("No polygon passed to POI/Polygon merger.");
  }
  if (polyCount > 1)
  {
    throw IllegalArgumentException("More than one polygon passed to POI/Polygon merger.");
  }

  std::set<std::pair<ElementId, ElementId> > pairs;
  pairs.insert(std::pair<ElementId, ElementId>(poiElementId, polyElementId));
  PoiPolygonMerger merger(pairs);
  std::vector<std::pair<ElementId, ElementId> > replacedElements;
  merger.apply(map, replacedElements);
}

QString PoiPolygonMerger::toString() const
{
  QString s = hoot::toString(getPairs());
  return QString("PoiPolygonMerger %1").arg(s);
}

}
