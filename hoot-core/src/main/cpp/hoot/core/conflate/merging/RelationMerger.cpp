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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "RelationMerger.h"

// hoot
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/elements/RelationMemberComparison.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigUtils.h>

namespace hoot
{

// ONLY ENABLE THIS DURING DEBUGGING; We don't want to tie it to debug.maps.write, as it may
// produce a very large number of output files.
const bool RelationMerger::WRITE_DETAILED_DEBUG_MAPS = false;

RelationMerger::RelationMerger() :
_mergeTags(true),
_deleteRelation2(true)
{
}

void RelationMerger::merge(
  const ElementId& elementId1, const ElementId& elementId2)
{
  if (elementId1 == elementId2)
  {
    return;
  }
  if (elementId1.getType() != ElementType::Relation ||
      elementId2.getType() != ElementType::Relation)
  {
    throw IllegalArgumentException(
      "Element types other than relation were passed to RelationMerger.");
  }

  RelationPtr relation1 = _map->getRelation(elementId1.getId());
  RelationPtr relation2 = _map->getRelation(elementId2.getId());

  if (!relation1 || !relation2)
  {
    return;
  }

  LOG_TRACE("Merging relations " << elementId1 << " and " << elementId2 << "...");

  if (relation1->contains(elementId2))
  {
    relation1->removeElement(elementId2);
  }

  if (_mergeTags)
  {
    LOG_TRACE("Merging tags...");
    Tags newTags =
      TagMergerFactory::mergeTags(
        relation1->getTags(), relation2->getTags(), ElementType::Relation);
    relation1->setTags(newTags);
  }

  // copy relation 2's members into 1
  bool allMembersCopied = false;
  if (relation2->getMemberCount() > 0)
  {
    allMembersCopied = _mergeMembers(relation1, relation2);
  }
  LOG_VART(allMembersCopied);

  if (_deleteRelation2)
  {
    // replace any references to relation 2 with a ref to relation 1
    LOG_TRACE("Replacing " << elementId2 << " with " << elementId1 << "...");
    ReplaceElementOp(elementId2, elementId1, true).apply(_map);
    // remove all instances of relation 2
    LOG_TRACE("Removing " << elementId2 << "...");
    RemoveRelationByEid(elementId2.getId()).apply(_map);
  }

  LOG_TRACE("Merged relations " << elementId1 << " and " << elementId2);

  if (WRITE_DETAILED_DEBUG_MAPS)
  {
    OsmMapWriterFactory::writeDebugMap(
      _map, "RelationMerger-" + elementId1.toString() + "-" + elementId2.toString());
  }
}

bool RelationMerger::_mergeMembers(RelationPtr replacingRelation, RelationPtr relationBeingReplaced)
{
  LOG_TRACE("Merging members...");

  const int numRelationBeingReplacedMembers = relationBeingReplaced->getMemberCount();
  int numMembersCopied = 0;
  // If a bounds was configured, we'll only merge in members that satisfy it.
  std::shared_ptr<InBoundsCriterion> boundsCrit = ConfigUtils::getBoundsFilter(_map);

  // Load up the relation members from both relations.

  const std::vector<RelationData::Entry> replacingRelationMembers = replacingRelation->getMembers();
  LOG_VART(replacingRelationMembers.size());
  const std::vector<RelationData::Entry> relationBeingReplacedMembers =
    relationBeingReplaced->getMembers();
  LOG_VART(relationBeingReplacedMembers.size());

  // If insertion ever proves too slow with these lists, then we can switch them over to linked
  // lists.
  QList<RelationMemberComparison> replacingRelationMemberComps;
  for (size_t i = 0; i < replacingRelationMembers.size(); i++)
  {
    const RelationData::Entry member = replacingRelationMembers[i];
    if (member.getElementId() != relationBeingReplaced->getElementId())
    {
      ConstElementPtr memberElement = _map->getElement(member.getElementId());
      if (memberElement)
      {
        ElementPtr memberElement2 = std::const_pointer_cast<Element>(memberElement);
        replacingRelationMemberComps.append(
          RelationMemberComparison(memberElement2, *_map, member.getRole(), true));
      }
    }
  }
  LOG_VART(replacingRelationMemberComps.size());

  QList<RelationMemberComparison> relationBeingReplacedMemberComps;
  for (size_t i = 0; i < relationBeingReplacedMembers.size(); i++)
  {
    const RelationData::Entry member = relationBeingReplacedMembers[i];
    if (member.getElementId() != replacingRelation->getElementId())
    {
      ConstElementPtr memberElement = _map->getElement(member.getElementId());
      if (memberElement)
      {
        ElementPtr memberElement2 = std::const_pointer_cast<Element>(memberElement);
        relationBeingReplacedMemberComps.append(
          RelationMemberComparison(memberElement2, *_map, member.getRole(), true));
      }
    }
  }
  LOG_VART(relationBeingReplacedMemberComps.size());

  for (int i = 0; i < relationBeingReplacedMemberComps.size(); i++)
  {
    const RelationMemberComparison currentMemberFromReplaced = relationBeingReplacedMemberComps[i];
    LOG_VART(currentMemberFromReplaced);

    // Skip copying any members to our target relation that our replacing relation already has.
    if (replacingRelationMemberComps.contains(currentMemberFromReplaced))
    {
      LOG_TRACE(
        "Skipping adding member being replaced that is already in the replacing relation: " <<
        currentMemberFromReplaced.getElement()->getElementId() << "...");
      LOG_TRACE("************************");
      continue;
    }
    // Don't add any relation members which aren't within the current bounds if one was specified.
    else if (boundsCrit && !boundsCrit->isSatisfied(currentMemberFromReplaced.getElement()))
    {
      LOG_TRACE(
        "Skipping adding member being replaced that is out of bounds: " <<
        currentMemberFromReplaced.getElement()->getElementId() << "...");
      LOG_TRACE("************************");
      continue;
    }

    // Determine which index in the target relation we want to insert the member from the replacing
    // relation.

    RelationMemberComparison beforeMemberFromReplaced;
    RelationMemberComparison afterMemberFromReplaced;
    if (i != 0)
    {
      // Find the member right before our current member in the relation being replaced.
      beforeMemberFromReplaced = relationBeingReplacedMemberComps[i - 1];
    }
    if (i != relationBeingReplacedMemberComps.size() - 1)
    {
      // Find the member right after our current member in the relation being replaced.
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

    // Use the relation size as the null value for size_t, since it can't be negative like the
    // not found index coming back from QList will be.
    int matchingFromReplacingBeforeIndex = relationBeingReplacedMemberComps.size();
    if (!beforeMemberFromReplaced.isNull())
    {
      // If there was a member directly preceding our current member in the relation being replaced,
      // see if it exists in the replacing relation. If so, record that member's index from the
      // replacing relation.
      const int index = replacingRelationMemberComps.indexOf(beforeMemberFromReplaced);
      if (index != -1)
      {
        matchingFromReplacingBeforeIndex = index;
      }
    }
    int matchingFromReplacingAfterIndex = relationBeingReplacedMemberComps.size();
    if (!afterMemberFromReplaced.isNull())
    {
      // If there was a member directly following our current member in the relation being replaced,
      // see if it exists in the replacing relation. If so, record that member's index from the
      // replacing relation.
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
        // If the replacing relation has matching directly preceding/following members for the
        // current member from the relation being replaced and they are consecutive, insert our
        // current member in between them in the replacing relation.
        LOG_TRACE(
          "Consecutive directly preceding and following members. Inserting current member: " <<
          currentMemberFromReplaced << " between indexes " <<
          matchingFromReplacingBeforeIndex << " and " << matchingFromReplacingAfterIndex << "...");
        replacingRelationMemberComps.insert(
          matchingFromReplacingAfterIndex, currentMemberFromReplaced);
      }
      else
      {
        // If the replacing relation has matching directly preceding/following members for the
        // current member from the relation being replaced and they are not consecutive,
        // arbitrarily insert our current member just after the preceding member.
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
      // If the replacing relation has a matching and directly preceding member but not a directly
      // following member, insert our current member from the relation being replaced just after the
      // preceding member in the replacing relation.
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
      // If the replacing relation has a matching and directly following member but not a directly
      // preceding member, insert our current member from the relation being replaced just before
      // the following member in the replacing relation.
      LOG_TRACE(
        "Directly following member only. Inserting current member: " <<
        currentMemberFromReplaced << " before index " <<  matchingFromReplacingAfterIndex << "...");
      replacingRelationMemberComps.insert(
        matchingFromReplacingAfterIndex - 1, currentMemberFromReplaced);
    }
    else
    {
      // If the replacing relation has no matching directly preceding/following members, arbitrarily
      // add the current member from the relation being replaced to the end of the replacing
      // relation.
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
    // Add the relation member to the relation we're keeping.
    modifiedMembers.emplace_back(
        currentMemberFromReplaced.getRole(),
        currentMemberFromReplaced.getElement()->getElementId());
    // Remove the member from the relation we may or may not be keeping.
    relationBeingReplaced->removeElement(currentMemberFromReplaced.getElement()->getElementId());
    numMembersCopied++;
  }
  if (!modifiedMembers.empty())
  {
    replacingRelation->setMembers(modifiedMembers);
  }

  return numRelationBeingReplacedMembers == numMembersCopied;
}

}
