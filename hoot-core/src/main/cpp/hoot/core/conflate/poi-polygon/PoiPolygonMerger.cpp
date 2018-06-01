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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonMerger.h"

// hoot
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPoiCriterion.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPolyCriterion.h>
#include <hoot/core/visitors/ElementIdSetVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/StatusUpdateVisitor.h>
#include <hoot/core/schema/PreserveTypesTagMerger.h>
#include <hoot/core/util/ConfigOptions.h>

using namespace std;

namespace hoot
{

unsigned int PoiPolygonMerger::logWarnCount = 0;

PoiPolygonMerger::PoiPolygonMerger(const set< pair<ElementId, ElementId> >& pairs) :
_pairs(pairs),
_autoMergeManyPoiToOnePolyMatches(ConfigOptions().getPoiPolygonAutoMergeManyPoiToOnePolyMatches())
{
  assert(_pairs.size() >= 1);
}

void PoiPolygonMerger::apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
{
  /// See
  /// https://github.com/ngageoint/hootenanny/files/607197/Hootenanny.-.POI.to.Polygon.2016-11-15.pptx
  /// for more details.

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
  LOG_VART(finalBuildingEid);

  ElementPtr finalBuilding = map->getElement(finalBuildingEid);
  if (!finalBuilding.get())
  {
    //building merger must not have been able to merge...maybe need an earlier check for this
    //and also handle it differently...

    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Building merger unable to merge.");
      LOG_VART(buildings1);
      LOG_VART(buildings2);
      LOG_VART(replaced);
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    return;
  }
  assert(finalBuilding.get());

  Tags finalBuildingTags = finalBuilding->getTags();
  boost::shared_ptr<const TagMerger> tagMerger;
  if (_autoMergeManyPoiToOnePolyMatches)
  {
    tagMerger.reset(new PreserveTypesTagMerger());
  }
  else
  {
    tagMerger = TagMergerFactory::getInstance().getDefaultPtr();
  }
  if (poiTags1.size())
  {
    LOG_TRACE("Merging POI tags with building tags for POI status Unknown1...");
    finalBuildingTags =
      tagMerger->mergeTags(poiTags1, finalBuildingTags, finalBuilding->getElementType());
  }
  if (poiTags2.size())
  {
    LOG_TRACE("Merging POI tags with building tags for POI status Unknown2...");
    finalBuildingTags =
      tagMerger->mergeTags(finalBuildingTags, poiTags2, finalBuilding->getElementType());
  }

  // do some book keeping to remove the POIs and mark them as replaced.
  long poisMerged = 0;
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
      poisMerged++;
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
      poisMerged++;
    }
  }
  LOG_VART(poisMerged);

  if (poisMerged > 0)
  {
    finalBuildingTags.set(MetadataTags::HootPoiPolygonPoisMerged(), QString::number(poisMerged));
    finalBuilding->setStatus(Status::Conflated);
  }

  finalBuilding->setTags(finalBuildingTags);
  LOG_VART(finalBuilding);
}

Tags PoiPolygonMerger::_mergePoiTags(const OsmMapPtr& map, Status s) const
{
  LOG_TRACE("Merging POI tags for status: " << s << "...");

  Tags result;

  boost::shared_ptr<const TagMerger> tagMerger;
  if (_autoMergeManyPoiToOnePolyMatches)
  {
    tagMerger.reset(new PreserveTypesTagMerger());
  }
  else
  {
    tagMerger = TagMergerFactory::getInstance().getDefaultPtr();
  }

  for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin(); it != _pairs.end();
       ++it)
  {
    const pair<ElementId, ElementId>& p = *it;
    ElementPtr e1 = map->getElement(p.first);
    ElementPtr e2 = map->getElement(p.second);
    LOG_VART(e1->getElementId());
    LOG_VART(e1->getStatus());
    LOG_VART(e1->getElementType());
    LOG_VART(e1->getTags().get("name"));
    if (e1->getStatus() == s && e1->getElementType() == ElementType::Node)
    {
      result = tagMerger->mergeTags(result, e1->getTags(), e1->getElementType());
    }
    LOG_VART(e2->getElementId());
    LOG_VART(e2->getStatus());
    LOG_VART(e2->getElementType());
    LOG_VART(e2->getTags().get("name"));
    if (e2->getStatus() == s && e2->getElementType() == ElementType::Node)
    {
      result = tagMerger->mergeTags(result, e2->getTags(), e2->getElementType());
    }
  }

  LOG_VART(result);
  return result;
}

vector<ElementId> PoiPolygonMerger::_getBuildingParts(const OsmMapPtr& map, Status s) const
{
  LOG_TRACE("Getting building parts for status: " << s << "...");

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

  LOG_VART(result);
  return result;
}

ElementId PoiPolygonMerger::_mergeBuildings(const OsmMapPtr& map,
  vector<ElementId>& buildings1, vector<ElementId>& buildings2,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  LOG_TRACE("Merging buildings...");

  LOG_VART(buildings1.size());
  LOG_VART(buildings2.size());
  LOG_VART(replaced.size());

  set< pair<ElementId, ElementId> > pairs;

  assert(buildings1.size() != 0 || buildings2.size() != 0);
  // if there is only one set of buildings then there is no need to merge.  group all the building
  //parts into a single building
  if (buildings1.size() == 0)
  {
    set<ElementId> eids;
    eids.insert(buildings2.begin(), buildings2.end());
    LOG_VART(eids.size());
    return BuildingMerger::buildBuilding(map, eids)->getElementId();
  }
  else if (buildings2.size() == 0)
  {
    set<ElementId> eids;
    eids.insert(buildings1.begin(), buildings1.end());
    LOG_VART(eids.size());
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

  BuildingMerger(pairs).apply(map, replaced);

  set<ElementId> newElement;
  for (size_t i = 0; i < replaced.size(); i++)
  {
    newElement.insert(replaced[i].second);
  }

  return *newElement.begin();
}

ElementId PoiPolygonMerger::mergePoiAndPolygon(OsmMapPtr map)
{
  //Trying to merge more than one POI into the polygon has proven problematic due to the building
  //merging logic.  Merging more than one POI isn't a requirement, so only supporting 1:1 merging
  //at this time.

  LOG_INFO("Merging one POI and one polygon...");

  StatusUpdateVisitor statusVisitor(Status::Unknown1, true);
  PoiPolygonPoiCriterion poiFilter;
  PoiPolygonPolyCriterion polyFilter;

  //If the features involved in the merging don't have statuses, let's arbitrarily set them to
  //Unknown1, since poi/poly requires it.  Setting them artificially *shouldn't* have a negative
  //effect on the poi/poly merging, though.

  FilteredVisitor filteredVis1(poiFilter, statusVisitor);
  map->visitRw(filteredVis1);

  FilteredVisitor filteredVis2(polyFilter, statusVisitor);
  map->visitRw(filteredVis2);

  //get our poi id
  ElementIdSetVisitor idSetVis1;
  FilteredVisitor filteredVis3(poiFilter, idSetVis1);
  map->visitRo(filteredVis3);
  const std::set<ElementId>& poiIds = idSetVis1.getElementSet();
  if (poiIds.size() != 1)
  {
    throw IllegalArgumentException(
      "Exactly one POI should be passed to PoiPolygonMerger::mergePoiAndPolygon.");
  }
  const ElementId poiId = *poiIds.begin();
  LOG_VART(poiId);

  //get our poly id
  ElementIdSetVisitor idSetVis2;
  FilteredVisitor filteredVis4(polyFilter, idSetVis2);
  map->visitRo(filteredVis4);
  const std::set<ElementId>& polyIds = idSetVis2.getElementSet();
  if (polyIds.size() != 1)
  {
    throw IllegalArgumentException(
      "Exactly one polygon should be passed to PoiPolygonMerger::mergePoiAndPolygon.");
  }
  const ElementId polyId = *polyIds.begin();
  LOG_VART(polyId);

  std::set<std::pair<ElementId, ElementId> > pairs;
  //Ordering doesn't matter here, since the poi is always merged into the poly.
  pairs.insert(std::pair<ElementId, ElementId>(polyId, poiId));
  PoiPolygonMerger merger(pairs);
  std::vector<std::pair<ElementId, ElementId> > replacedElements;
  merger.apply(map, replacedElements);

  LOG_INFO("Merged the POI into the polygon.");

  return polyId;
}

QString PoiPolygonMerger::toString() const
{
  QString s = hoot::toString(getPairs());
  return QString("PoiPolygonMerger %1").arg(s);
}

}
