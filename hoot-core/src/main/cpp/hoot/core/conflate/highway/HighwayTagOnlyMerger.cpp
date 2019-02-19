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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HighwayTagOnlyMerger.h"

// hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/BridgeCriterion.h>
#include <hoot/core/conflate/highway/HighwaySnapMerger.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/OsmUtils.h>
//#include <hoot/core/ops/RecursiveElementRemover.h>
//#include <hoot/core/ops/RemoveRelationOp.h>

namespace hoot
{

//int HighwayTagOnlyMerger::_numRelationsEncountered = 0;
//QList<int> HighwayTagOnlyMerger::_relationMemberSizeDistributionUnknown1;
//QList<int> HighwayTagOnlyMerger::_relationMemberSizeDistributionUnknown2;
//QList<int> HighwayTagOnlyMerger::_relationMemberSizeDistributionConflated;
//int HighwayTagOnlyMerger::_numUnknown1Relations = 0;
//int HighwayTagOnlyMerger::_numUnknown2Relations = 0;
//int HighwayTagOnlyMerger::_numConflatedRelations = 0;

HighwayTagOnlyMerger::HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs) :
HighwaySnapMerger(pairs, boost::shared_ptr<SublineStringMatcher>()),
_performBridgeGeometryMerging(false)
{
}

HighwayTagOnlyMerger::HighwayTagOnlyMerger(
  const std::set<std::pair<ElementId, ElementId>>& pairs,
  const boost::shared_ptr<SublineStringMatcher>& sublineMatcher) :
HighwaySnapMerger(pairs, sublineMatcher),
_performBridgeGeometryMerging(true)
{
  // Merging geometries for bridges is governed both by a config option and whether a subline
  // matcher gets passed in, since not all calling merger creators have a subline matcher available
  // to pass in at this point.
  if (!ConfigOptions().getAttributeConflationAllowRefGeometryChangesForBridges())
  {
    _performBridgeGeometryMerging = false;
  }
}

HighwayTagOnlyMerger::~HighwayTagOnlyMerger()
{
//  LOG_VARD(_numRelationsEncountered);
//  LOG_VARD(_numUnknown1Relations);
//  LOG_VARD(_numUnknown2Relations);
//  LOG_VARD(_numConflatedRelations);
//  LOG_VARD(_relationMemberSizeDistributionUnknown1);
//  LOG_VARD(_relationMemberSizeDistributionUnknown2);
//  LOG_VARD(_relationMemberSizeDistributionConflated);
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
    if (toRemove->getElementType() == ElementType::Way)
    {
      WayPtr wayWithTagsToRemove = boost::dynamic_pointer_cast<Way>(toRemove);
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
  LOG_VART(keeper->getElementId());
  LOG_VART(toRemove->getElementId());
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

  if (e1 && e2)
  {
    OsmUtils::logElementDetail(e1, map, Log::Trace, "HighwayTagOnlyMerger: e1");
    OsmUtils::logElementDetail(e2, map, Log::Trace, "HighwayTagOnlyMerger: e2");

    // If just one of the features is a bridge, we want the bridge feature to separate from the road
    // feature its being merged with.  So, use the normal geometry AND tag merger.

    std::vector<ConstElementPtr> elements;
    elements.push_back(e1);
    elements.push_back(e2);
    const bool onlyOneIsABridge = OsmUtils::isSatisfied<BridgeCriterion>(elements, 1, true);
    if (onlyOneIsABridge)
    {
      if (!_performBridgeGeometryMerging)
      {
        LOG_DEBUG(
          "Unable to perform geometric bridge merging due to invalid subline string matcher.  << "
          "Performing tag only merge...");
      }
      else
      {
        LOG_DEBUG("Using tag and geometry merger, since just one of the features is a bridge...");
        const bool needsReview = HighwaySnapMerger::_mergePair(map, eid1, eid2, replaced);
        if (needsReview)
        {
          LOG_DEBUG("HighwaySnapMerger returned review.");
        }
        LOG_VARD(map->getElement(eid1));
        LOG_VARD(map->getElement(eid2));
        return needsReview;
      }
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
    //LOG_VARD(elementWithTagsToKeep->getElementId());
    //LOG_VARD(elementWithTagsToRemove->getElementId());
    OsmUtils::logElementDetail(
      elementWithTagsToKeep, map, Log::Debug, "HighwayTagOnlyMerger: elementWithTagsToKeep");
    OsmUtils::logElementDetail(
      elementWithTagsToRemove, map, Log::Debug, "HighwayTagOnlyMerger: elementWithTagsToRemove");

    return
      _mergeWays(
        elementWithTagsToKeep, elementWithTagsToRemove, removeSecondaryElement, map, replaced);
  }

  return false;
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

  // merge the tags
  Tags mergedTags =
    TagMergerFactory::mergeTags(
      elementWithTagsToKeep->getTags(), elementWithTagsToRemove->getTags(), ElementType::Way);
  elementWithTagsToKeep->setTags(mergedTags);
  elementWithTagsToKeep->setStatus(Status::Conflated);
  OsmUtils::logElementDetail(
    elementWithTagsToKeep, map, Log::Debug, "HighwayTagOnlyMerger: keeper element");
  // mark element for replacement
  // TODO: The multilinestring relations marked for replacement aren't being removed.
  if (removeSecondaryElement)
  {
    LOG_DEBUG("Marking " << elementWithTagsToRemove->getElementId() << " for replacement...");
    replaced.push_back(
      std::pair<ElementId, ElementId>(
        elementWithTagsToRemove->getElementId(), elementWithTagsToKeep->getElementId()));
  }

  return true;
}

void HighwayTagOnlyMerger::_copyTagsToWayMembers(ConstElementPtr e1, ConstElementPtr e2,
                                                 const OsmMapPtr& map)
{
  // hope this isn't happening
  assert(!(e1->getElementType() == ElementType::Relation &&
           e2->getElementType() == ElementType::Relation));

  // handle relations coming from HighwaySnapMerger's previous handling of bridges
  if (e1->getElementType() == ElementType::Relation ||
      e2->getElementType() == ElementType::Relation)
  {
    _numRelationsEncountered++;

    ConstRelationPtr relation;
    if (e1->getElementType() == ElementType::Relation)
    {
      relation = boost::dynamic_pointer_cast<const Relation>(e1);
    }
    else
    {
      relation = boost::dynamic_pointer_cast<const Relation>(e2);
    }

//    if (relation->getStatus() == Status::Unknown1)
//    {
//      _numUnknown1Relations++;
//    }
//    else if (relation->getStatus() == Status::Unknown2)
//    {
//      _numUnknown2Relations++;
//    }
//    else if (relation->getStatus() == Status::Conflated)
//    {
//      _numConflatedRelations++;
//    }

    const std::vector<RelationData::Entry>& relationMembers = relation->getMembers();
    LOG_VARD(relationMembers.size());
    assert(relationMembers.size() >= 2);

//    if (relation->getStatus() == Status::Unknown1 &&
//        !_relationMemberSizeDistributionUnknown1.contains(relationMembers.size()))
//    {
//      _relationMemberSizeDistributionUnknown1.append(relationMembers.size());
//    }
//    else if (relation->getStatus() == Status::Unknown2 &&
//             !_relationMemberSizeDistributionUnknown2.contains(relationMembers.size()))
//    {
//      _relationMemberSizeDistributionUnknown2.append(relationMembers.size());
//    }
//    else if (relation->getStatus() == Status::Conflated &&
//             !_relationMemberSizeDistributionConflated.contains(relationMembers.size()))
//    {
//      _relationMemberSizeDistributionConflated.append(relationMembers.size());
//    }

    // Simply copy the tags that were pushed up from the way members to the parent relation,
    // presumably by the MultiLineStringSplitter.
    for (size_t i = 0; i < relationMembers.size(); i++)
    {
      ElementPtr member = map->getElement(relationMembers[i].getElementId());
      if (member && member->getElementType() == ElementType::Way)
      {
        WayPtr wayMember = boost::dynamic_pointer_cast<Way>(member);;
        wayMember->setTags(
          TagMergerFactory::mergeTags(wayMember->getTags(), relation->getTags(), ElementType::Way));
      }
    }
  }
}

bool HighwayTagOnlyMerger::_conflictExists(ConstElementPtr elementWithTagsToKeep,
                                           ConstElementPtr elementWithTagsToRemove) const
{
  if (OsmUtils::nameConflictExists(elementWithTagsToKeep, elementWithTagsToRemove))
  {
    LOG_DEBUG("Conflicting name tags.  Skipping merge.");
    return true;
  }

  // don't try to merge streets with conflicting one way info
  if (OsmUtils::oneWayConflictExists(elementWithTagsToKeep, elementWithTagsToRemove))
  {
    LOG_DEBUG("Conflicting one way street tags.  Skipping merge.");
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
    WayPtr wayWithTagsToKeep = boost::dynamic_pointer_cast<Way>(elementWithTagsToKeep);
    ConstWayPtr wayWithTagsToRemove =
      boost::dynamic_pointer_cast<const Way>(elementWithTagsToRemove);
    if (isAOneWayStreet.isSatisfied(wayWithTagsToRemove) &&
        // note the use of an alternative isSimilarDirection method
        !DirectionFinder::isSimilarDirection2(
           map->shared_from_this(), wayWithTagsToKeep, wayWithTagsToRemove))
    {
      LOG_DEBUG("Reversing " << wayWithTagsToKeep->getElementId() << "...");
      wayWithTagsToKeep->reverseOrder();
    }
  }
}

}
