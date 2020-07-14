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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HighwayTagOnlyMerger.h"

// hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/conflate/highway/HighwaySnapMerger.h>
#include <hoot/core/criterion/BridgeCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/conflate/highway/HighwayUtils.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/elements/TagUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, HighwayTagOnlyMerger)

HighwayTagOnlyMerger::HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs,
                                           std::shared_ptr<PartialNetworkMerger> networkMerger)
  : HighwaySnapMerger(pairs, std::shared_ptr<SublineStringMatcher>()),
      _performBridgeGeometryMerging(
      ConfigOptions().getAttributeConflationAllowRefGeometryChangesForBridges()),
      _networkMerger(networkMerger)
{
  _removeTagsFromWayMembers = false;
  _markAddedMultilineStringRelations = true;
}

HighwayTagOnlyMerger::HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs,
                                           const std::shared_ptr<SublineStringMatcher>& sublineMatcher)
  : HighwaySnapMerger(pairs, sublineMatcher),
      _performBridgeGeometryMerging(
      ConfigOptions().getAttributeConflationAllowRefGeometryChangesForBridges())
{
  _removeTagsFromWayMembers = false;
  _markAddedMultilineStringRelations = true;
}

void HighwayTagOnlyMerger::_determineKeeperFeature(ElementPtr element1, ElementPtr element2,
                                                   ElementPtr& keeper, ElementPtr& toRemove,
                                                   bool& removeSecondaryElement)
{
  removeSecondaryElement = true;
  if (element1->getStatus() == Status::Conflated && element2->getStatus() == Status::Conflated)
  {
    keeper = element1;
    toRemove = element2;
    // TODO: This is ignoring the contents of multilinestring relations.
    if (toRemove->getElementType() == ElementType::Way)
    {
      WayPtr wayWithTagsToRemove = std::dynamic_pointer_cast<Way>(toRemove);
      wayWithTagsToRemove->setPid(element1->getElementId().getId());
      removeSecondaryElement = false;
    }
  }
  // TODO: Should pid's be getting set in these two cases as well?
  else if (element1->getStatus() == Status::Unknown1 || element1->getStatus() == Status::Conflated)
  {
    keeper = element1;
    toRemove = element2;
  }
  else if (element1->getStatus() == Status::Unknown2 || element2->getStatus() == Status::Conflated)
  {
    keeper = element2;
    toRemove = element1;
  }
}

bool HighwayTagOnlyMerger::_mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
  std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  ElementPtr e1 = map->getElement(eid1);
  ElementPtr e2 = map->getElement(eid2);

  if (!e1 || !e2)
  {
    LOG_TRACE("One element missing.  Marking for review...");
    return HighwayMergerAbstract::_mergePair(map, eid1, eid2, replaced);
  }

  LOG_TRACE("HighwayTagOnlyMerger: e1\n" << OsmUtils::getElementDetailString(e1, map));
  LOG_TRACE("HighwayTagOnlyMerger: e2\n" << OsmUtils::getElementDetailString(e2, map));

  // If just one of the features is a bridge, we want the bridge feature to separate from the road
  // feature its being merged with.  So, use a geometry AND tag merger.

  std::vector<ConstElementPtr> elements;
  elements.push_back(e1);
  elements.push_back(e2);
  const bool onlyOneIsABridge =
    CriterionUtils::containsSatisfyingElements<BridgeCriterion>(elements, 1, true);
  if (onlyOneIsABridge)
  {
    LOG_TRACE("Using tag and geometry merger, since just one of the features is a bridge...");

    bool needsReview = false;
    std::string mergerName;
    if (!_networkMerger)
    {
      mergerName = HighwaySnapMerger::className();
      needsReview = HighwaySnapMerger::_mergePair(map, eid1, eid2, replaced);
    }
    else
    {
      mergerName = PartialNetworkMerger::className();
      _networkMerger->apply(map, replaced);
    }
    if (needsReview)
    {
      LOG_TRACE(mergerName << " returned review.");
    }
    LOG_VART(map->getElement(eid1));
    LOG_VART(map->getElement(eid2));
    return needsReview;
  }

  // Otherwise, proceed with tag only merging.

  // copy relation tags back to their way members
  _copyTagsToWayMembers(e1, e2, map);

  // Merge the ways, bringing the secondary feature's attributes over to the reference feature.

  ElementPtr elementWithTagsToKeep;
  ElementPtr elementWithTagsToRemove;
  bool removeSecondaryElement;
  _determineKeeperFeature(
    e1, e2, elementWithTagsToKeep, elementWithTagsToRemove, removeSecondaryElement);
  LOG_VART(elementWithTagsToKeep->getElementId());
  LOG_VART(elementWithTagsToRemove->getElementId());
//  OsmUtils::logElementDetail(
//    elementWithTagsToKeep, map, Log::Trace, "HighwayTagOnlyMerger: elementWithTagsToKeep");
//  OsmUtils::logElementDetail(
//    elementWithTagsToRemove, map, Log::Trace, "HighwayTagOnlyMerger: elementWithTagsToRemove");

  return
    _mergeWays(
      elementWithTagsToKeep, elementWithTagsToRemove, removeSecondaryElement, map, replaced);
}

bool HighwayTagOnlyMerger::_mergeWays(ElementPtr elementWithTagsToKeep,
                                      ElementPtr elementWithTagsToRemove,
                                      const bool removeSecondaryElement, const OsmMapPtr& map,
                                      std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  if (_conflictExists(elementWithTagsToKeep, elementWithTagsToRemove))
  {
    return false;
  }

  // Reverse the way if way to remove is one way and the two ways aren't in similar directions
  _handleOneWayStreetReversal(elementWithTagsToKeep, elementWithTagsToRemove, map);

  // TODO: This is ignoring the contents of multilinestring relations.
  // TODO: I think we need to bring information nodes from secondary ways here like we do in ref
  // with HighwaySnapMerger...not exactly sure why the call in _mergePair to HighwaySnapMerger
  // doesn't already do this.

  // merge the tags
  Tags mergedTags =
    TagMergerFactory::mergeTags(
      elementWithTagsToKeep->getTags(), elementWithTagsToRemove->getTags(), ElementType::Way);
  // sanity check to prevent the multilinestring tag, later used to remove multilinestring
  // relations added during conflation, from being added to anything other than relations
  if (elementWithTagsToKeep->getElementType() != ElementType::Relation)
  {
    mergedTags.remove(MetadataTags::HootMultilineString());
  }
  elementWithTagsToKeep->setTags(mergedTags);
  elementWithTagsToKeep->setStatus(Status::Conflated);
  ConfigOptions conf;
  if (conf.getWriterIncludeDebugTags() && conf.getWriterIncludeMatchedByTag())
  {
    elementWithTagsToKeep->setTag(MetadataTags::HootMatchedBy(), HighwayMatch::MATCH_NAME);
  }
  LOG_TRACE(
    "HighwayTagOnlyMerger: keeper element\n" <<
    OsmUtils::getElementDetailString(elementWithTagsToKeep, map));

  map->getIdSwap()->add(
    elementWithTagsToRemove->getElementId(), elementWithTagsToKeep->getElementId());
  // mark element for replacement
  // TODO: The multilinestring relations marked for replacement aren't being removed.
  if (removeSecondaryElement)
  {
    LOG_TRACE("Marking " << elementWithTagsToRemove->getElementId() << " for replacement...");
    replaced.push_back(
      std::pair<ElementId, ElementId>(
        elementWithTagsToRemove->getElementId(), elementWithTagsToKeep->getElementId()));
  }

  return true;
}

void HighwayTagOnlyMerger::_copyTagsToWayMembers(ElementPtr e1, ElementPtr e2, const OsmMapPtr& map)
{
  // hope this isn't happening
  assert(!(e1->getElementType() == ElementType::Relation &&
           e2->getElementType() == ElementType::Relation));

  // handle relations coming from HighwaySnapMerger's previous handling of bridges
  if (e1->getElementType() == ElementType::Relation ||
      e2->getElementType() == ElementType::Relation)
  {
    RelationPtr relation;
    if (e1->getElementType() == ElementType::Relation)
    {
      relation = std::dynamic_pointer_cast<Relation>(e1);
    }
    else
    {
      relation = std::dynamic_pointer_cast<Relation>(e2);
    }

    const std::vector<RelationData::Entry>& relationMembers = relation->getMembers();
    LOG_VART(relationMembers.size());
    assert(relationMembers.size() >= 2);

    // Simply copy the tags that were pushed up from the way members to the parent relation,
    // presumably by the MultiLineStringSplitter.
    for (size_t i = 0; i < relationMembers.size(); i++)
    {
      ElementPtr member = map->getElement(relationMembers[i].getElementId());
      if (member && member->getElementType() == ElementType::Way)
      {
        WayPtr wayMember = std::dynamic_pointer_cast<Way>(member);
        LOG_TRACE(
          "Copying tags from: " << relation->getElementId() << " to member: " <<
          wayMember->getElementId() << "...");
        wayMember->setTags(
          TagMergerFactory::mergeTags(wayMember->getTags(), relation->getTags(), ElementType::Way));
        // safety check to make sure we didn't mark any ways as added multilinestring relations
        wayMember->getTags().remove(MetadataTags::HootMultilineString());
      }
    }
  }
}

bool HighwayTagOnlyMerger::_conflictExists(ConstElementPtr elementWithTagsToKeep,
                                           ConstElementPtr elementWithTagsToRemove) const
{
  if (TagUtils::nameConflictExists(elementWithTagsToKeep, elementWithTagsToRemove))
  {
    LOG_TRACE("Conflicting name tags.  Skipping merge.");
    return true;
  }

  // don't try to merge streets with conflicting one way info
  // TODO: This is ignoring the contents of multilinestring relations.
  if (HighwayUtils::oneWayConflictExists(elementWithTagsToKeep, elementWithTagsToRemove))
  {
    LOG_TRACE("Conflicting one way street tags.  Skipping merge.");
    return true;
  }

  // If two roads disagree in highway type and aren't generic, don't merge.
  if (HighwayUtils::nonGenericHighwayConflictExists(elementWithTagsToKeep, elementWithTagsToRemove))
  {
    LOG_TRACE("Conflicting highway type tags.  Skipping join.")
    return true;
  }

  return false;
}

void HighwayTagOnlyMerger::_handleOneWayStreetReversal(ElementPtr elementWithTagsToKeep,
                                                       ConstElementPtr elementWithTagsToRemove,
                                                       const OsmMapPtr& map)
{
  // TODO: This is ignoring the contents of multilinestring relations.
  OneWayCriterion isAOneWayStreet;
  if (elementWithTagsToKeep->getElementType() == ElementType::Way &&
      elementWithTagsToRemove->getElementType() == ElementType::Way)
  {
    WayPtr wayWithTagsToKeep = std::dynamic_pointer_cast<Way>(elementWithTagsToKeep);
    ConstWayPtr wayWithTagsToRemove =
      std::dynamic_pointer_cast<const Way>(elementWithTagsToRemove);
    if (isAOneWayStreet.isSatisfied(wayWithTagsToRemove) &&
        // note the use of an alternative isSimilarDirection method
        !DirectionFinder::isSimilarDirection2(
           map->shared_from_this(), wayWithTagsToKeep, wayWithTagsToRemove))
    {
      LOG_TRACE("Reversing " << wayWithTagsToKeep->getElementId() << "...");
      wayWithTagsToKeep->reverseOrder();
    }
  }
}

}
