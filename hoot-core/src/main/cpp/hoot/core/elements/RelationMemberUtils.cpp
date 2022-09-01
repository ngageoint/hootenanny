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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "RelationMemberUtils.h"

// Hoot
#include <hoot/core/conflate/ConflateInfoCache.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ElementIdUtils.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/CollectionUtils.h>

namespace hoot
{

QString RelationMemberUtils::getRelationMembersDetailString(const ConstRelationPtr& relation,
                                                            const ConstOsmMapPtr& map)
{
  QString str = "\nMember Detail:\n\n";
  const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
  LOG_VART(relationMembers.size());
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    str += "Member #" + QString::number(i + 1) + ":\n\n";
    ConstElementPtr member = map->getElement(relationMembers[i].getElementId());
    if (!member)
    {
      throw HootException(
        "Unable to retrieve relation member: " + relationMembers[i].getElementId().toString() +
        ". Skipping adding it to output...");
    }
    LOG_VART(member->getElementId());
    str += member->toString() + "\n\n";
  }
  return str;
}

bool RelationMemberUtils::isMemberOfRelationWithType(const ConstOsmMapPtr& map, const ElementId& childId,
                                                     const QString& relationType)
{
  LOG_VART(childId);
  LOG_VART(relationType);

  const std::set<ElementId> parentIds = map->getParents(childId);
  LOG_VART(parentIds.size());
  for (const auto& parentId : parentIds)
  {
    if (parentId.getType() == ElementType::Relation)
    {
      ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(map->getElement(parentId));
      if (relation && relation->getType() == relationType)
      {
        LOG_TRACE(childId << " member of relation: " << parentId << " with type: " << relationType);
        return true;
      }
    }
  }
  return false;
}

bool RelationMemberUtils::isMemberOfRelationInCategory(const ConstOsmMapPtr& map, const ElementId& childId,
                                                       const QString& schemaCategory)
{
  LOG_VART(childId);
  LOG_VART(schemaCategory);
  LOG_VART(map.get());

  const std::set<ElementId> parentIds = map->getParents(childId);
  LOG_VART(parentIds.size());
  for (const auto& parentId : parentIds)
  {
    if (parentId.getType() == ElementType::Relation)
    {
      ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(map->getElement(parentId));
      if (relation && OsmSchema::getInstance().hasCategory(relation->getTags(), schemaCategory))
      {
        LOG_TRACE(childId << " member of relation: " << parentId << " in category: " << schemaCategory);
        return true;
      }
    }
  }
  return false;
}

bool RelationMemberUtils::isMemberOfRelationWithTagKey(const ConstOsmMapPtr& map, const ElementId& childId,
                                                       const QString& tagKey)
{
  LOG_VART(childId);
  LOG_VART(tagKey);

  const std::set<ElementId> parentIds = map->getParents(childId);
  LOG_VART(parentIds.size());
  for (const auto& parentId : parentIds)
  {
    if (parentId.getType() == ElementType::Relation)
    {
      ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(map->getElement(parentId));
      if (relation && relation->getTags().contains(tagKey))
      {
        LOG_TRACE(childId << " member of relation: " << parentId << " with tag key: " << tagKey);
        return true;
      }
    }
  }
  return false;
}

bool RelationMemberUtils::elementContainedByAnyRelation(const ElementId& elementId, const ConstOsmMapPtr& map)
{
  return !map->getIndex().getElementToRelationMap()->getRelationByElement(elementId).empty();
}

bool RelationMemberUtils::containsMemberWithCriterion(const ConstRelationPtr& relation, const ElementCriterion& criterion,
                                                      const ConstOsmMapPtr& map)
{
  if (!map)
    throw IllegalArgumentException("Invalid map specified.");
  if (!relation)
    return false;

  const std::vector<RelationData::Entry>& relationMembers = relation->getMembers();
  for (const auto& m : relationMembers)
  {
    ConstElementPtr member = map->getElement(m.getElementId());
    if (member)
    {
      LOG_VART(member->getElementId());
      if (criterion.isSatisfied(member))
        return true;
    }
  }
  return false;
}

bool RelationMemberUtils::containsOnlyMembersWithCriterion(const ConstRelationPtr& relation, const ElementCriterion& criterion,
                                                           const ConstOsmMapPtr& map)
{
  if (!map)
    throw IllegalArgumentException("Invalid map specified.");
  if (!relation)
    return false;

  const std::vector<RelationData::Entry>& relationMembers = relation->getMembers();
  for (const auto& m : relationMembers)
  {
    ConstElementPtr member = map->getElement(m.getElementId());
    if (member)
    {
      LOG_VART(member->getElementId());
      if (!criterion.isSatisfied(member))
        return false;
    }
  }
  return true;
}

std::vector<ConstRelationPtr> RelationMemberUtils::getContainingRelationsConst(const ElementId& childId, const ConstOsmMapPtr& map,
                                                                               const bool ignoreReviewRelations)
{
  LOG_VART(map.get());
  LOG_VART(childId);

  std::vector<ConstRelationPtr> relations;
  const std::shared_ptr<ElementToRelationMap>& e2r = map->getIndex().getElementToRelationMap();
  LOG_VART(e2r.get());
  const std::set<long> relationIds = e2r->getRelationByElement(childId);
  LOG_VART(relationIds.size());
  for (auto relation_id : relationIds)
  {
    ConstRelationPtr relation = map->getRelation(relation_id);
    LOG_VART(relation.get());
    if (relation && (!ignoreReviewRelations || !relation->hasTag(MetadataTags::HootReviewNeeds())))
      relations.push_back(relation);
  }
  return relations;
}

std::vector<RelationPtr> RelationMemberUtils::getContainingRelations(
  const ElementId& childId, const ConstOsmMapPtr& map, const bool ignoreReviewRelations)
{
  std::vector<RelationPtr> relationsToReturn;
  std::vector<ConstRelationPtr> relations =
    getContainingRelationsConst(childId, map, ignoreReviewRelations);
  for (std::vector<ConstRelationPtr>::const_iterator itr = relations.begin();
       itr != relations.end(); ++itr)
  {
    relationsToReturn.push_back(std::const_pointer_cast<Relation>(*itr));
  }
  return relationsToReturn;
}

int RelationMemberUtils::getContainingRelationCount(const ElementId& childId, const ConstOsmMapPtr& map,
                                                    const bool ignoreReviewRelations)
{
  return (int)getContainingRelations(childId, map, ignoreReviewRelations).size();
}

bool RelationMemberUtils::isMemberOfRelationSatisfyingCriterion(const ElementId& childId, const ElementCriterion& criterion,
                                                                const ConstOsmMapPtr& map)
{
  LOG_VART(map.get());
  LOG_VART(childId);
  LOG_VART(criterion.toString());

  std::vector<ConstRelationPtr> containingRelations = getContainingRelationsConst(childId, map);
  LOG_VART(containingRelations.size());
  for (const auto& relation : containingRelations)
  {
    if (criterion.isSatisfied(relation))
      return true;
  }
  return false;
}

}
