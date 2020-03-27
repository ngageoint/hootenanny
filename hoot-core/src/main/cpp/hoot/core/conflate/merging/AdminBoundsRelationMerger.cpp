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
#include "AdminBoundsRelationMerger.h"

// hoot
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/ops/RemoveRelationByEid.h>

namespace hoot
{

AdminBoundsRelationMerger::AdminBoundsRelationMerger()
{
}

void AdminBoundsRelationMerger::merge(const ElementId& elementId1, const ElementId& elementId2)
{
  if (elementId1.getType() != ElementType::Relation ||
      elementId2.getType() != ElementType::Relation)
  {
    throw IllegalArgumentException("TODO");
  }

  RelationPtr relation1 = _map->getRelation(elementId1.getId());
  RelationPtr relation2 = _map->getRelation(elementId2.getId());

  Tags newTags =
    TagMergerFactory::mergeTags(relation1->getTags(), relation2->getTags(), ElementType::Relation);
  relation1->setTags(newTags);

  // copy relation 2's members into 1
  _mergeMembers(relation1, relation2);

  ReplaceElementOp(elementId2, elementId1, true).apply(_map);

  RemoveRelationByEid(elementId2.getId()).apply(_map);
}

void AdminBoundsRelationMerger::_mergeMembers(
  RelationPtr replacingRelation, RelationPtr relationBeingReplaced)
{
  const std::vector<RelationData::Entry> replacingRelationMembers = replacingRelation->getMembers();
  const std::vector<RelationData::Entry> relationBeingReplacedMembers =
    relationBeingReplaced->getMembers();
  // if insertion proves too slow with this list, then we can switch over to a linked list
  QList<RelationData::Entry> replacingRelationMembersList =
    QList<RelationData::Entry>::fromVector(
      QVector<RelationData::Entry>::fromStdVector(replacingRelationMembers));
  for (size_t i = 0; i < relationBeingReplacedMembers.size(); i++)
  {
    const RelationData::Entry currentMemberFromReplaced = relationBeingReplacedMembers[i];

    // skip copying members our replacing relation already has
    if (replacingRelationMembersList.contains(currentMemberFromReplaced))
    {
      continue;
    }

    RelationData::Entry beforeMemberFromReplaced;
    RelationData::Entry afterMemberFromReplaced;
    if (i != 0)
    {
      // find the member right before our current member in the relation being replaced
      beforeMemberFromReplaced = relationBeingReplacedMembers[i - 1];
    }
    if (i != relationBeingReplacedMembers.size() - 1)
    {
      // find the member right after our current member in the relation being replaced
      afterMemberFromReplaced = relationBeingReplacedMembers[i + 1];
    }

    size_t matchingFromReplacingBeforeIndex = relationBeingReplacedMembers.size();
    if (!beforeMemberFromReplaced.isNull())
    {
      // if there was a member directly preceding our current member in the relation being replaced,
      // see if it exists in the replacing relation; if so record that member's index from the
      // replacing relation
      matchingFromReplacingBeforeIndex =
        replacingRelationMembersList.indexOf(beforeMemberFromReplaced);
    }
    size_t matchingFromReplacingAfterIndex = relationBeingReplacedMembers.size();
    if (!afterMemberFromReplaced.isNull())
    {
      // if there was a member directly following our current member in the relation being replaced,
      // see if it exists in the replacing relation; if so record that member's index from the
      // replacing relation
      matchingFromReplacingAfterIndex =
        replacingRelationMembersList.indexOf(afterMemberFromReplaced);
    }

    if (matchingFromReplacingBeforeIndex != relationBeingReplacedMembers.size() &&
        matchingFromReplacingAfterIndex != relationBeingReplacedMembers.size())
    {
      if ((matchingFromReplacingAfterIndex - matchingFromReplacingBeforeIndex) == 1)
      {
        // if the replacing relation has matching directly preceding/following members for the
        // current member from the relation being replaced and they are consecutive, insert our
        // current member in between them in the replacing relation
        replacingRelationMembersList.insert(
          matchingFromReplacingAfterIndex, currentMemberFromReplaced);
      }
      else
      {
        // if the replacing relation has matching directly preceding/following members for the
        // current member from the relation being replaced and they are not consecutive,
        // arbitrarily insert our current member just after the preceding member
        replacingRelationMembersList.insert(
          matchingFromReplacingBeforeIndex + 1, currentMemberFromReplaced);
      }
    }
    else if (matchingFromReplacingAfterIndex == relationBeingReplacedMembers.size() &&
             matchingFromReplacingBeforeIndex != relationBeingReplacedMembers.size())
    {
      // if the replacing relation has a matching directly preceding member but not a directly
      // following member, insert our current member from the relation being replaced just after the
      // preceding member in the replacing relation
      replacingRelationMembersList.insert(
        matchingFromReplacingBeforeIndex + 1, currentMemberFromReplaced);
    }
    else if (matchingFromReplacingAfterIndex != relationBeingReplacedMembers.size() &&
             matchingFromReplacingBeforeIndex == relationBeingReplacedMembers.size())
    {
      // if the replacing relation has a matching directly following member but not a directly
      // preceding member, insert our current member from the relation being replaced just before
      // the following member in the replacing relation
      replacingRelationMembersList.insert(
        matchingFromReplacingAfterIndex - 1, currentMemberFromReplaced);
    }
    else
    {
      // if the replacing relation has no matching directly preceding/following members, arbitrarily
      // add the current member from the relation being replacd to the end of the replacing relation
      replacingRelationMembersList.append(currentMemberFromReplaced);
    }
  }
  replacingRelation->setMembers(replacingRelationMembersList.toVector().toStdVector());
}

}
