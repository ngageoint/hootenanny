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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingMerger.h"

// hoot
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/ops/BuildingPartMergeOp.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/OverwriteTagMerger.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/BuildingPartCriterion.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/schema/PreserveTypesTagMerger.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, BuildingMerger)

class DeletableBuildingCriterion : public ElementCriterion
{

public:

  DeletableBuildingCriterion() {}

  virtual bool isSatisfied(const ConstElementPtr& e) const
  {
    bool result = false;

    if (e->getElementType() == ElementType::Node && e->getTags().getInformationCount() == 0)
    {
      result = true;
    }
    else if (e->getElementType() != ElementType::Node)
    {
      if (_buildingCrit.isSatisfied(e) || _buildingPartCrit.isSatisfied(e))
      {
        result = true;
      }
    }

    return !result;
  }

  virtual QString getDescription() const { return ""; }

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new DeletableBuildingCriterion()); }

private:

  BuildingCriterion _buildingCrit;
  BuildingPartCriterion _buildingPartCrit;
};

int BuildingMerger::logWarnCount = 0;

BuildingMerger::BuildingMerger() :
MergerBase()
{
}

BuildingMerger::BuildingMerger(const set<pair<ElementId, ElementId>>& pairs) :
_pairs(pairs),
_keepMoreComplexGeometryWhenAutoMerging(
  ConfigOptions().getBuildingKeepMoreComplexGeometryWhenAutoMerging()),
_mergeManyToManyMatches(ConfigOptions().getBuildingMergeManyToManyMatches()),
_manyToManyMatch(false)
{
  LOG_VART(_pairs);
}

void BuildingMerger::apply(const OsmMapPtr& map, vector<pair<ElementId, ElementId>>& replaced)
{
  set<ElementId> firstPairs;
  set<ElementId> secondPairs;
  set<ElementId> combined;

  LOG_VART(_pairs);

  //check if it is many to many
  for (set<pair<ElementId, ElementId>>::const_iterator sit = _pairs.begin(); sit != _pairs.end();
       ++sit)
  {
    firstPairs.insert(sit->first);
    secondPairs.insert(sit->second);
    combined.insert(sit->first);
    combined.insert(sit->second);
  }
  _manyToManyMatch = firstPairs.size() > 1 && secondPairs.size() > 1;

  ReviewMarker reviewMarker;
  if (_manyToManyMatch && !_mergeManyToManyMatches)
  {
    const QString note =
      "Merging multiple buildings from each data source is error prone and requires a human eye.";
    reviewMarker.mark(map, combined, note, "Building", 1);
  }
  else
  {
    std::shared_ptr<Element> e1 = _buildBuilding(map, true);
    if (e1.get())
    {
      OsmUtils::logElementDetail(e1, map, Log::Trace, "BuildingMerger: built building e1");
    }
    std::shared_ptr<Element> e2 = _buildBuilding(map, false);
    if (e2.get())
    {
      OsmUtils::logElementDetail(e2, map, Log::Trace, "BuildingMerger: built building e2");
    }

    std::shared_ptr<Element> keeper;
    std::shared_ptr<Element> scrap;
    LOG_VART(_keepMoreComplexGeometryWhenAutoMerging);
    if (_keepMoreComplexGeometryWhenAutoMerging)
    {
      // use node count as a surrogate for complexity of the geometry.
      int nodeCount1 = 0;
      if (e1.get())
      {
        LOG_VART(e1);
        nodeCount1 =
          (int)FilteredVisitor::getStat(
            new ElementTypeCriterion(ElementType::Node), new ElementCountVisitor(), map, e1);
      }
      LOG_VART(nodeCount1);

      int nodeCount2 = 0;
      if (e2.get())
      {
        nodeCount2 =
          (int)FilteredVisitor::getStat(
            new ElementTypeCriterion(ElementType::Node), new ElementCountVisitor(), map, e2);
      }
      LOG_VART(nodeCount2);

      //This will happen if a way/relation building is passed in with missing nodes.
      if (nodeCount1 == 0 || nodeCount2 == 0)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("One or more of the buildings to merge are empty.  Skipping merge...");
          if (e1.get())
          {
            LOG_VART(e1->getElementId());
          }
          else
          {
            LOG_TRACE("Building one null.");
          }
          if (e2.get())
          {
            LOG_VART(e2->getElementId());
          }
          else
          {
            LOG_TRACE("Building two null.");
          }
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;

        return;
      }

      if (nodeCount1 == nodeCount2)
      {
        keeper = e1;
        scrap = e2;
        LOG_TRACE(
          "Buildings have equally complex geometries.  Keeping the first building geometry: " <<
          keeper << "; scrap: " << scrap->getElementId() << "...");
      }
      else
      {
        if (nodeCount1 > nodeCount2)
        {
          keeper = e1;
          scrap = e2;
        }
        else
        {
          keeper = e2;
          scrap = e1;
        }
        LOG_TRACE(
          "Keeping the more complex building geometry: " << keeper << "; scrap: " <<
          scrap->getElementId() << "...");
      }
    }
    else
    {
      keeper = e1;
      scrap = e2;
      LOG_TRACE(
        "Keeping the first building geometry: " << keeper->getElementId() << "; scrap: " <<
        scrap->getElementId() << "...");
    }

    Tags newTags;
    LOG_TRACE("e1 tags before merging and after built building tag merge: " << e1->getTags());
    LOG_TRACE("e2 tags before merging and after built building tag merge: " << e2->getTags());
    if (_manyToManyMatch && _mergeManyToManyMatches)
    {
      // preserve type tags
      newTags = PreserveTypesTagMerger().mergeTags(e1->getTags(), e2->getTags(), ElementType::Way);
    }
    else
    {
      // use the default tag merging mechanism
      newTags = TagMergerFactory::mergeTags(e1->getTags(), e2->getTags(), ElementType::Way);
    }
    LOG_TRACE("tags after merging: " << newTags);

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

    OsmUtils::logElementDetail(keeper, map, Log::Trace, "BuildingMerger: keeper");
    OsmUtils::logElementDetail(scrap, map, Log::Trace, "BuildingMerger: scrap");

    //Check to see if we are removing a multipoly building relation.  If so, its multipolygon
    //relation members, need to be removed as well.
    const QSet<ElementId> multiPolyMemberIds = _getMultiPolyMemberIds(scrap);

    // remove the duplicate element
    DeletableBuildingCriterion crit;
    ReplaceElementOp(scrap->getElementId(), keeper->getElementId()).apply(map);
    RecursiveElementRemover(scrap->getElementId(), &crit).apply(map);
    scrap->getTags().clear();

    // delete any multipoly members
    LOG_TRACE("Removing multi-poly members: " << multiPolyMemberIds);
    for (QSet<ElementId>::const_iterator it = multiPolyMemberIds.begin();
         it != multiPolyMemberIds.end(); ++it)
    {
      RecursiveElementRemover(*it).apply(map);
    }

    set<pair<ElementId, ElementId>> replacedSet;
    for (set<pair<ElementId, ElementId>>::const_iterator it = _pairs.begin();
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

QSet<ElementId> BuildingMerger::_getMultiPolyMemberIds(const ConstElementPtr& element) const
{
  QSet<ElementId> relationMemberIdsToRemove;
  if (element->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(element);
    if (relation->getType() == MetadataTags::RelationMultiPolygon())
    {
      const vector<RelationData::Entry>& entries = relation->getMembers();
      for (size_t i = 0; i < entries.size(); i++)
      {
        const RelationData::Entry entry = entries[i];
        const QString role = entry.getRole();
        if (entry.getElementId().getType() == ElementType::Way &&
            (role == MetadataTags::RoleInner() || role == MetadataTags::RoleOuter()))
        {
          relationMemberIdsToRemove.insert(entry.getElementId());
        }
      }
    }
  }
  return relationMemberIdsToRemove;
}

std::shared_ptr<Element> BuildingMerger::buildBuilding(const OsmMapPtr& map,
                                                       const set<ElementId>& eid,
                                                       const bool preserveTypes)
{
  if (eid.size() > 0)
  {
    LOG_TRACE("Creating building for eid's: " << eid << "...");
  }

  if (eid.size() == 0)
  {
    throw IllegalArgumentException("No element ID passed to building builder.");
  }
  else if (eid.size() == 1)
  {
    return map->getElement(*eid.begin());
  }
  else
  {
    vector<std::shared_ptr<Element>> parts;
    vector<ElementId> toRemove;
    parts.reserve(eid.size());
    for (set<ElementId>::const_iterator it = eid.begin(); it != eid.end(); ++it)
    {
      std::shared_ptr<Element> e = map->getElement(*it);
      bool isBuilding = false;
      if (e && e->getElementType() == ElementType::Relation)
      {
        RelationPtr r = std::dynamic_pointer_cast<Relation>(e);
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
              std::shared_ptr<Element> buildingPart = map->getElement(m[i].getElementId());
              LOG_TRACE("Building part before tag update: " << buildingPart);
              // Push any non-conflicting tags in the parent relation down into the building part.
              buildingPart->setTags(
                OverwriteTagMerger().mergeTags(
                  buildingPart->getTags(), r->getTags(), buildingPart->getElementType()));
              LOG_TRACE("Building part after tag update: " << buildingPart);
              parts.push_back(buildingPart);
            }
          }

          toRemove.push_back(r->getElementId());
        }
      }

      if (!isBuilding)
      {
        OsmUtils::logElementDetail(e, map, Log::Trace, "BuildingMerger: Non-building part");
        parts.push_back(e);
      }
    }
    LOG_VART(parts.size());
    LOG_VART(parts);
    LOG_VART(toRemove.size());
    LOG_VART(toRemove);

    std::shared_ptr<Element> result =
      BuildingPartMergeOp(preserveTypes).combineBuildingParts(map, parts);
    LOG_TRACE("Combined building parts into: " << result);

    // likely create a crit that only matches buildings and building parts and pass that
    DeletableBuildingCriterion crit;
    for (size_t i = 0; i < toRemove.size(); i++)
    {
      if (map->containsElement(toRemove[i]))
      {
        ElementPtr willRemove = map->getElement(toRemove[i]);
        ReplaceElementOp(toRemove[i], result->getElementId()).apply(map);
        RecursiveElementRemover(toRemove[i], &crit).apply(map);
        // just in case it wasn't removed (e.g. part of another relation)
        willRemove->getTags().clear();
      }
    }

    return result;
  }
}

std::shared_ptr<Element> BuildingMerger::_buildBuilding(const OsmMapPtr& map,
                                                        const bool unknown1) const
{
  set<ElementId> eids;
  for (set<pair<ElementId, ElementId>>::const_iterator it = _pairs.begin();
    it != _pairs.end(); ++it)
  {
    if (unknown1)
    {
      eids.insert(it->first);
    }
    else
    {
      eids.insert(it->second);
    }
  }
  return buildBuilding(map, eids, _manyToManyMatch && _mergeManyToManyMatches);
}

void BuildingMerger::mergeBuildings(OsmMapPtr map, const ElementId& mergeTargetId)
{
  LOG_INFO("Merging buildings...");

  //The building merger by default uses geometric complexity (node count) to determine which
  //building geometry to keep.  Since the UI at this point will never pass in buildings with their
  //child nodes, we want to override the default behavior and make sure the building merger always
  //arbitrarily keeps the geometry of the first building passed in.  This is ok, b/c the UI workflow
  //lets the user select which building to keep and using complexity wouldn't make sense.
  LOG_VART(ConfigOptions().getBuildingKeepMoreComplexGeometryWhenAutoMergingKey());
  conf().set(
    ConfigOptions().getBuildingKeepMoreComplexGeometryWhenAutoMergingKey(), "false");
  LOG_VART(ConfigOptions().getBuildingKeepMoreComplexGeometryWhenAutoMerging());

  int buildingsMerged = 0;

  BuildingCriterion buildingCrit;

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const ConstWayPtr& way = wayItr->second;
    if (way->getElementId() != mergeTargetId && buildingCrit.isSatisfied(way))
    {
      LOG_VART(way);
      std::set<std::pair<ElementId, ElementId>> pairs;
      pairs.insert(std::pair<ElementId, ElementId>(mergeTargetId, way->getElementId()));
      BuildingMerger merger(pairs);
      LOG_VART(pairs.size());
      std::vector<std::pair<ElementId, ElementId>> replacedElements;
      merger.apply(map, replacedElements);
      buildingsMerged++;
    }
  }

  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator relItr = relations.begin(); relItr != relations.end(); ++relItr)
  {
    const ConstRelationPtr& relation = relItr->second;
    if (relation->getElementId() != mergeTargetId && buildingCrit.isSatisfied(relation))
    {
      LOG_VART(relation);
      std::set<std::pair<ElementId, ElementId>> pairs;
      pairs.insert(std::pair<ElementId, ElementId>(mergeTargetId, relation->getElementId()));
      BuildingMerger merger(pairs);
      LOG_VART(pairs.size());
      std::vector<std::pair<ElementId, ElementId>> replacedElements;
      merger.apply(map, replacedElements);
      buildingsMerged++;
    }
  }

  LOG_INFO("Merged " << buildingsMerged << " buildings.");
}

QString BuildingMerger::toString() const
{
  QString s = hoot::toString(_getPairs());
  return QString("BuildingMerger %1").arg(s);
}

}
