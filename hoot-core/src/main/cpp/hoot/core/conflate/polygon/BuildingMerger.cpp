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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingMerger.h"

// hoot
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/filters/BaseFilter.h>
#include <hoot/core/filters/ElementTypeCriterion.h>
#include <hoot/core/ops/BuildingPartMergeOp.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/OverwriteTagMerger.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>

using namespace std;

namespace hoot
{

class DeletableBuildingPart : public BaseFilter
{

public:

  DeletableBuildingPart() : BaseFilter(Filter::FilterMatches) {}

  bool isMatch(const Element &e) const
  {
    bool result = false;

    if (e.getElementType() == ElementType::Node && e.getTags().getInformationCount() == 0)
    {
      result = true;
    }
    else if (e.getElementType() != ElementType::Node)
    {
      if (OsmSchema::getInstance().isBuilding(e.getTags(), e.getElementType()) ||
          OsmSchema::getInstance().isBuildingPart(e.getTags(), e.getElementType()))
      {
        result = true;
      }
    }

    return result;
  }

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new DeletableBuildingPart()); }
};

unsigned int BuildingMerger::logWarnCount = 0;

BuildingMerger::BuildingMerger(const set< pair<ElementId, ElementId> >& pairs) :
_pairs(pairs)
{
}

void BuildingMerger::apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
{
  //check if it is many to many
  set<ElementId> firstPairs;
  set<ElementId> secondPairs;
  set<ElementId> combined;
  ReviewMarker reviewMarker;
  for (set< pair<ElementId, ElementId> >::iterator sit = _pairs.begin(); sit != _pairs.end(); ++sit)
  {
    firstPairs.insert(sit->first);
    secondPairs.insert(sit->second);
    combined.insert(sit->first);
    combined.insert(sit->second);
  }
  if (firstPairs.size() > 1 && secondPairs.size() > 1) //it is many to many
  {
    QString note =
      "Merging multiple buildings from each data source is error prone and requires a human eye.";
    reviewMarker.mark(map, combined, note, "Building", 1);
  }
  else
  {
    // use node count as a surrogate for complexity of the geometry.
    int nodeCount1 = 0;
    boost::shared_ptr<Element> e1 = _buildBuilding1(map);
    LOG_VART(e1.get());
    //in #2034, encountering a situation where the second building is empty;
    //didn't think that was possible here...added checks here for both
    if (e1.get())
    {
      LOG_VART(e1);
      nodeCount1 =
        (int)FilteredVisitor::getStat(
          new ElementTypeCriterion(ElementType::Node), new ElementCountVisitor(), map, e1);
    }

    int nodeCount2 = 0;
    boost::shared_ptr<Element> e2 = _buildBuilding2(map);
    LOG_VART(e2.get());
    if (e2.get())
    {
      nodeCount2 =
        (int)FilteredVisitor::getStat(
          new ElementTypeCriterion(ElementType::Node), new ElementCountVisitor(), map, e2);
    }
    LOG_VART(nodeCount1);
    LOG_VART(nodeCount2);

    //don't think this should be occurring...needs more investigation
    if (nodeCount1 == 0 || nodeCount2 == 0)
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("One or more of the buildings to merge are empty.");
        if (e1.get())
        {
          LOG_VARD(e1->getElementId());
        }
        else
        {
          LOG_DEBUG("Building one null.");
        }
        if (e2.get())
        {
          LOG_VARD(e2->getElementId());
        }
        else
        {
          LOG_DEBUG("Building two null.");
        }
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;

      return;
    }

    boost::shared_ptr<Element> keeper;
    boost::shared_ptr<Element> scrap;
    // keep the more complex building geometry.
    if (nodeCount1 >= nodeCount2)
    {
      keeper = e1;
      scrap = e2;
    }
    else
    {
      keeper = e2;
      scrap = e1;
    }
    LOG_TRACE("Keeping the more complex building geometry: " << keeper << "...");

    // use the default tag merging mechanism
    Tags newTags = TagMergerFactory::mergeTags(e1->getTags(), e2->getTags(), ElementType::Way);

    QStringList ref1;
    e1->getTags().readValues(MetadataTags::Ref1(), ref1);
    QStringList ref2;
    e2->getTags().readValues(MetadataTags::Ref2(), ref2);

    ref1.sort();
    ref2.sort();

    if (ref1.size() != 0 || ref2.size() != 0)
    {
      if (ref1 == ref2)
      {
        newTags[MetadataTags::HootBuildingMatch()] = "true";
      }
      else
      {
        newTags[MetadataTags::HootBuildingMatch()] = "false";
      }
    }

    keeper->setTags(newTags);
    keeper->setStatus(Status::Conflated);

    LOG_VART(keeper->getElementId());
    LOG_VART(scrap->getElementId());
    const ElementId scrapId = scrap->getElementId();
    const Status scrapStatus = scrap->getStatus();

    // remove the duplicate element
    DeletableBuildingPart filter;
    ReplaceElementOp(scrap->getElementId(), keeper->getElementId()).apply(map);
    RecursiveElementRemover(scrap->getElementId(), &filter).apply(map);
    scrap->getTags().clear();

    // see comments for similar functionality in HighwaySnapMerger::_mergePair
    if (scrapStatus == Status::Unknown1 &&
        ConfigOptions().getPreserveUnknown1ElementIdWhenModifyingFeatures())
    {
      LOG_TRACE(
        "Retaining reference ID by mapping unknown1 ID: " << scrapId << " to ID: " <<
        keeper->getElementId() << "...");
      _unknown1Replacements.insert(pair<ElementId, ElementId>(scrapId, keeper->getElementId()));
    }

    set< pair<ElementId, ElementId> > replacedSet;
    for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin();
         it != _pairs.end(); ++it)
    {
      // if we replaced the second group of buildings
      if (it->second != keeper->getElementId())
      {
        replacedSet.insert(pair<ElementId, ElementId>(it->second, keeper->getElementId()));
      }
      if (it->first != keeper->getElementId())
      {
        replacedSet.insert(pair<ElementId, ElementId>(it->first, keeper->getElementId()));
      }
    }
    replaced.insert(replaced.end(), replacedSet.begin(), replacedSet.end());
  }
}

boost::shared_ptr<Element> BuildingMerger::buildBuilding(const OsmMapPtr& map,
                                                         const set<ElementId>& eid)
{
  LOG_TRACE("Build the building...");
  LOG_VART(eid);

  assert(eid.size() > 0);

  if (eid.size() == 1)
  {
    return map->getElement(*eid.begin());
  }
  else
  {
    vector< boost::shared_ptr<Element> > parts;
    vector< ElementId > toRemove;
    parts.reserve(eid.size());
    for (set<ElementId>::const_iterator it = eid.begin(); it != eid.end(); ++it)
    {
      boost::shared_ptr<Element> e = map->getElement(*it);
      bool isBuilding = false;
      if (e && e->getElementType() == ElementType::Relation)
      {
        RelationPtr r = boost::dynamic_pointer_cast<Relation>(e);
        if (r->getType() == MetadataTags::RelationBuilding())
        {
          LOG_VART(r);
          isBuilding = true;

          // This is odd. Originally I had a const reference to the result, but that was causing
          // an obscure segfault. I changed it to a copy and everything is happy. I don't know
          // when/where the reference would be changing, but I also don't think this will be
          // a significant optimization issue.
          vector<RelationData::Entry> m = r->getMembers();
          for (size_t i = 0; i < m.size(); ++i)
          {
            if (m[i].getRole() == MetadataTags::RolePart())
            {
              boost::shared_ptr<Element> em = map->getElement(m[i].getElementId());
              // push any non-conflicing tags in the parent relation down into the building part.
              em->setTags(OverwriteTagMerger().mergeTags(em->getTags(), r->getTags(),
                em->getElementType()));
              parts.push_back(em);
            }
          }

          toRemove.push_back(r->getElementId());
        }
      }

      if (!isBuilding)
      {
        parts.push_back(e);
      }
    }

    boost::shared_ptr<Element> result = BuildingPartMergeOp().combineParts(map, parts);
    LOG_VART(result);

    DeletableBuildingPart filter;

    // likely create a filter that only matches buildings and building parts and pass that to the
    for (size_t i = 0; i < toRemove.size(); i++)
    {
      if (map->containsElement(toRemove[i]))
      {
        ElementPtr willRemove = map->getElement(toRemove[i]);
        ReplaceElementOp(toRemove[i], result->getElementId()).apply(map);
        RecursiveElementRemover(toRemove[i], &filter).apply(map);
        // just in case it wasn't removed (e.g. part of another relation)
        willRemove->getTags().clear();
      }
    }

    return result;
  }
}

boost::shared_ptr<Element> BuildingMerger::_buildBuilding1(const OsmMapPtr& map) const
{
  set<ElementId> e;

  for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin();
    it != _pairs.end(); ++it)
  {
    e.insert(it->first);
  }

  return buildBuilding(map, e);
}

boost::shared_ptr<Element> BuildingMerger::_buildBuilding2(const OsmMapPtr& map) const
{
  set<ElementId> e;

  for (set< pair<ElementId, ElementId> >::const_iterator it = _pairs.begin();
    it != _pairs.end(); ++it)
  {
    e.insert(it->second);
  }

  return buildBuilding(map, e);
}

QString BuildingMerger::toString() const
{
  QString s = hoot::toString(getPairs());
  return QString("BuildingMerger %1").arg(s);
}

ElementId BuildingMerger::merge(OsmMapPtr map)
{
  LOG_INFO("Merging two buildings...");

  //This logic will work whether constituent way nodes/relation members are passed in or not.  See
  //additional notes in the method description.

  //TODO: fix

  LOG_VART(map->getElementCount());

//  int polyCount = 0;
//  ElementId polyElementId;
//  const WayMap& ways = map->getWays();
//  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
//  {
//    const int wayId = wayItr->first;
//    WayPtr way = map->getWay(wayId);
//    if (OsmSchema::getInstance().isBuilding(way))
//    {
//      LOG_VART(way);
//      polyElementId = ElementId::way(wayId);
//      polyCount++;
//    }
//  }
//  if (polyElementId.isNull())
//  {
//    const RelationMap& relations = map->getRelations();
//    for (RelationMap::const_iterator relItr = relations.begin(); relItr != relations.end(); ++relItr)
//    {
//      const int relationId = relItr->first;
//      RelationPtr relation = map->getRelation(relationId);
//      if (OsmSchema::getInstance().isBuilding(relation))
//      {
//        LOG_VART(relation);
//        polyElementId = ElementId::relation(relationId);
//        polyCount++;
//      }
//    }
//  }
//  if (polyCount == 0)
//  {
//    throw IllegalArgumentException("No polygon passed to POI/Polygon merger.");
//  }
//  if (polyCount > 1)
//  {
//    throw IllegalArgumentException("More than one polygon passed to POI/Polygon merger.");
//  }

//  LOG_VART(polyElementId);

//  std::set<std::pair<ElementId, ElementId> > pairs;
//  pairs.insert(std::pair<ElementId, ElementId>(poiElementId, polyElementId));
//  BuildingMerger merger(pairs);
//  LOG_VART(pairs.size());
//  std::vector<std::pair<ElementId, ElementId> > replacedElements;
//  merger.apply(map, replacedElements);

//  return polyElementId;

  return ElementId();
}

}
