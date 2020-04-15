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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CollectionRelationMerger.h"

// hoot
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/elements/RelationMemberComparison.h>

namespace hoot
{

CollectionRelationMerger::CollectionRelationMerger()
{
}

void CollectionRelationMerger::merge(const ElementId& elementId1, const ElementId& elementId2)
{
  if (elementId1.getType() != ElementType::Relation ||
      elementId2.getType() != ElementType::Relation)
  {
    throw IllegalArgumentException(
      "Element types other than relation were passed to CollectionRelationMerger.");
  }

  LOG_TRACE("Merging relations " << elementId1 << " and " << elementId2 << "...");

  RelationPtr relation1 = _map->getRelation(elementId1.getId());
  RelationPtr relation2 = _map->getRelation(elementId2.getId());

  LOG_TRACE("Merging tags...");
  Tags newTags =
    TagMergerFactory::mergeTags(relation1->getTags(), relation2->getTags(), ElementType::Relation);
  relation1->setTags(newTags);

  // copy relation 2's members into 1
  _mergeMembers(relation1, relation2);

  LOG_TRACE("Replacing " << elementId2 << " with " << elementId1 << "...");
  ReplaceElementOp(elementId2, elementId1, true).apply(_map);

  LOG_TRACE("Removing " << elementId2 << "...");
  RemoveRelationByEid(elementId2.getId()).apply(_map);

  LOG_TRACE("Merged relations " << elementId1 << " and " << elementId2);
}

void CollectionRelationMerger::_mergeMembers(
  RelationPtr replacingRelation, RelationPtr relationBeingReplaced)
{
  LOG_TRACE("Merging members...");

  const std::vector<RelationData::Entry> replacingRelationMembers = replacingRelation->getMembers();
  LOG_VART(replacingRelationMembers.size());
  const std::vector<RelationData::Entry> relationBeingReplacedMembers =
    relationBeingReplaced->getMembers();
  LOG_VART(relationBeingReplacedMembers.size());

  // if insertion ever proves too slow with these lists, then we can switch them over to linked
  // lists
  QList<RelationMemberComparison> replacingRelationMemberComps;
  for (size_t i = 0; i < replacingRelationMembers.size(); i++)
  {
    const RelationData::Entry member = replacingRelationMembers[i];
    ConstElementPtr memberElement = _map->getElement(member.getElementId());
    if (memberElement)
    {
      ElementPtr memberElement2 = std::const_pointer_cast<Element>(memberElement);
      replacingRelationMemberComps.append(
        RelationMemberComparison(memberElement2, *_map, member.getRole(), true));
    }
  }
  LOG_VART(replacingRelationMemberComps.size());

  QList<RelationMemberComparison> relationBeingReplacedMemberComps;
  for (size_t i = 0; i < relationBeingReplacedMembers.size(); i++)
  {
    const RelationData::Entry member = relationBeingReplacedMembers[i];
    ConstElementPtr memberElement = _map->getElement(member.getElementId());
    if (memberElement)
    {
      ElementPtr memberElement2 = std::const_pointer_cast<Element>(memberElement);
      relationBeingReplacedMemberComps.append(
        RelationMemberComparison(memberElement2, *_map, member.getRole(), true));
    }
  }
  LOG_VART(relationBeingReplacedMemberComps.size());

  for (int i = 0; i < relationBeingReplacedMemberComps.size(); i++)
  {
    const RelationMemberComparison currentMemberFromReplaced = relationBeingReplacedMemberComps[i];
    LOG_VART(currentMemberFromReplaced);

    // skip copying members our replacing relation already has
    if (replacingRelationMemberComps.contains(currentMemberFromReplaced))
    {
      LOG_TRACE(
        "Skipping adding member being replaced already in the replacing relation: " <<
        currentMemberFromReplaced << "...");
      LOG_TRACE("************************");
      continue;
    }

    RelationMemberComparison beforeMemberFromReplaced;
    RelationMemberComparison afterMemberFromReplaced;
    if (i != 0)
    {
      // find the member right before our current member in the relation being replaced
      beforeMemberFromReplaced = relationBeingReplacedMemberComps[i - 1];
    }
    if (i != relationBeingReplacedMemberComps.size() - 1)
    {
      // find the member right after our current member in the relation being replaced
      afterMemberFromReplaced = relationBeingReplacedMemberComps[i + 1];
    }
    if (!beforeMemberFromReplaced.isNull())
    {
      LOG_VART(beforeMemberFromReplaced);
    }
    if (!afterMemberFromReplaced.isNull())
    {
      LOG_VART(afterMemberFromReplaced);
    }

    // use the relation size as the null value for size_t, since it can't be negative like the
    // not found index coming back from QList will be
    int matchingFromReplacingBeforeIndex = relationBeingReplacedMemberComps.size();
    if (!beforeMemberFromReplaced.isNull())
    {
      // if there was a member directly preceding our current member in the relation being replaced,
      // see if it exists in the replacing relation; if so record that member's index from the
      // replacing relation
      const int index = replacingRelationMemberComps.indexOf(beforeMemberFromReplaced);
      if (index != -1)
      {
        matchingFromReplacingBeforeIndex = index;
      }
    }
    int matchingFromReplacingAfterIndex = relationBeingReplacedMemberComps.size();
    if (!afterMemberFromReplaced.isNull())
    {
      // if there was a member directly following our current member in the relation being replaced,
      // see if it exists in the replacing relation; if so record that member's index from the
      // replacing relation
      const int index = replacingRelationMemberComps.indexOf(afterMemberFromReplaced);
      if (index != -1)
      {
        matchingFromReplacingAfterIndex = index;
      }
    }
    if (matchingFromReplacingBeforeIndex != relationBeingReplacedMemberComps.size())
    {
      LOG_VART(matchingFromReplacingBeforeIndex);
    }
    if (matchingFromReplacingAfterIndex != relationBeingReplacedMemberComps.size())
    {
      LOG_VART(matchingFromReplacingAfterIndex);
    }

    if (matchingFromReplacingBeforeIndex != relationBeingReplacedMemberComps.size() &&
        matchingFromReplacingAfterIndex != relationBeingReplacedMemberComps.size())
    {
      if ((matchingFromReplacingAfterIndex - matchingFromReplacingBeforeIndex) == 1)
      {
        // if the replacing relation has matching directly preceding/following members for the
        // current member from the relation being replaced and they are consecutive, insert our
        // current member in between them in the replacing relation
        LOG_TRACE(
          "Consecutive directly preceding and following members. Inserting current member: " <<
          currentMemberFromReplaced << " between indexes " <<
          matchingFromReplacingBeforeIndex << " and " << matchingFromReplacingAfterIndex << "...");
        replacingRelationMemberComps.insert(
          matchingFromReplacingAfterIndex, currentMemberFromReplaced);
      }
      else
      {
        // if the replacing relation has matching directly preceding/following members for the
        // current member from the relation being replaced and they are not consecutive,
        // arbitrarily insert our current member just after the preceding member
        LOG_TRACE(
          "Non-consecutive directly preceding and following members. Inserting current member: " <<
          currentMemberFromReplaced << " after index " << matchingFromReplacingBeforeIndex <<
          "...");
        replacingRelationMemberComps.insert(
          matchingFromReplacingBeforeIndex + 1, currentMemberFromReplaced);
      }
    }
    else if (matchingFromReplacingAfterIndex == relationBeingReplacedMemberComps.size() &&
             matchingFromReplacingBeforeIndex != relationBeingReplacedMemberComps.size())
    {
      // if the replacing relation has a matching directly preceding member but not a directly
      // following member, insert our current member from the relation being replaced just after the
      // preceding member in the replacing relation
      LOG_TRACE(
        "Directly preceding member only. Inserting current member: " <<
        currentMemberFromReplaced << " after index " <<  matchingFromReplacingBeforeIndex <<
        "...");
      replacingRelationMemberComps.insert(
        matchingFromReplacingBeforeIndex + 1, currentMemberFromReplaced);
    }
    else if (matchingFromReplacingAfterIndex != relationBeingReplacedMemberComps.size() &&
             matchingFromReplacingBeforeIndex == relationBeingReplacedMemberComps.size())
    {
      // if the replacing relation has a matching directly following member but not a directly
      // preceding member, insert our current member from the relation being replaced just before
      // the following member in the replacing relation
      LOG_TRACE(
        "Directly following member only. Inserting current member: " <<
        currentMemberFromReplaced << " before index " <<  matchingFromReplacingAfterIndex << "...");
      replacingRelationMemberComps.insert(
        matchingFromReplacingAfterIndex - 1, currentMemberFromReplaced);
    }
    else
    {
      // if the replacing relation has no matching directly preceding/following members, arbitrarily
      // add the current member from the relation being replacd to the end of the replacing relation
      LOG_TRACE(
        "No directly preceding or following members. Inserting current member: " <<
        currentMemberFromReplaced << " at the end of the relation...");
      replacingRelationMemberComps.append(currentMemberFromReplaced);
    }
    LOG_TRACE("************************");
  }
  LOG_VART(replacingRelationMemberComps);

  std::vector<RelationData::Entry> modifiedMembers;
  for (int i = 0; i < replacingRelationMemberComps.size(); i++)
  {
    const RelationMemberComparison currentMemberFromReplaced = replacingRelationMemberComps[i];
    modifiedMembers.push_back(
      RelationData::Entry(
        currentMemberFromReplaced.getRole(),
        currentMemberFromReplaced.getElement()->getElementId()));
  }
  replacingRelation->setMembers(modifiedMembers);
}

}
