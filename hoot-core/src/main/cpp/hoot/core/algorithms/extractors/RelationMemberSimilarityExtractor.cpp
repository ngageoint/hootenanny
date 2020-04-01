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
#include "RelationMemberSimilarityExtractor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/elements/RelationMemberComparison.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, RelationMemberSimilarityExtractor)

RelationMemberSimilarityExtractor::RelationMemberSimilarityExtractor() :
{
}

double RelationMemberSimilarityExtractor::extract(
  const OsmMap& map, const std::shared_ptr<const Element>& target,
  const std::shared_ptr<const Element>& candidate) const
{
  if (target->getElementType() != ElementType::Relation ||
      candidate->getElementType() != ElementType::Relation)
  {
    return 0.0;
  }

  const ConstRelationPtr targetRelation = std::dynamic_pointer_cast<const Relation>(target);
  const ConstRelationPtr candidateRelation =
    std::dynamic_pointer_cast<const Relation>(candidate);
  if (targetRelation && candidateRelation)
  {
    LOG_VART(targetRelation->getTags().get("name"));
    LOG_VART(candidateRelation->getTags().get("name"));
    LOG_VART(targetRelation->getType());
    LOG_VART(candidateRelation->getType());

    QSet<ElementId> targetMemberIds =
      CollectionUtils::stdSetToQSet(targetRelation->getMemberIds());
    LOG_VART(targetMemberIds.size());
    //LOG_VARD(targetMemberIds);
    QSet<ElementId> candidateMemberIds =
      CollectionUtils::stdSetToQSet(candidateRelation->getMemberIds());
    LOG_VART(candidateMemberIds.size());
    //LOG_VARD(candidateMemberIds);
    const int totalMembers = targetMemberIds.size() + candidateMemberIds.size();
    LOG_VART(totalMembers);

    if (targetMemberIds.size() == 0 || candidateMemberIds.size() == 0)
    {
      return 0.0;
    }

    int numSharedMembers = 0;

    QSet<RelationMemberComparison> targetMemberComps;
    const std::vector<RelationData::Entry> targetMembers = targetRelation->getMembers();
    for (size_t i = 0; i < targetMembers.size(); i++)
    {
      const RelationData::Entry member = targetMembers[i];
      ConstElementPtr memberElement = map.getElement(member.getElementId());
      if (memberElement)
      {
        ElementPtr memberElement2 = std::const_pointer_cast<Element>(memberElement);
        targetMemberComps.insert(RelationMemberComparison(memberElement2, map, member.getRole()));
      }
    }
    LOG_VART(targetMemberComps.size());

    QSet<RelationMemberComparison> candidateMemberComps;
    const std::vector<RelationData::Entry> candidateMembers = candidateRelation->getMembers();
    for (size_t i = 0; i < candidateMembers.size(); i++)
    {
      const RelationData::Entry member = candidateMembers[i];
      ConstElementPtr memberElement = map.getElement(member.getElementId());
      if (memberElement)
      {
        ElementPtr memberElement2 = std::const_pointer_cast<Element>(memberElement);
        candidateMemberComps.insert(
          RelationMemberComparison(memberElement2, map, member.getRole()));
      }
    }
    LOG_VART(candidateMemberComps.size());

    QSet<RelationMemberComparison> intersection =
      targetMemberComps.intersect(candidateMemberComps);
    LOG_VART(intersection);
    numSharedMembers = intersection.size();
    //if (numSharedMembers > 0)
    //{
      //LOG_VAR(targetMemberComps);
      //LOG_VAR(candidateMemberComps);
      //LOG_VAR(intersection);
    //}
  }

  LOG_VART(numSharedMembers);
  //if (numSharedMembers > 0)
  //{
    //LOG_INFO(numSharedMembers);
    //LOG_INFO(targetMemberIds);
    //LOG_INFO(candidateMemberIds);
  //}
  //LOG_VARD(intersection);

  // calculate Jaccard
  const double similarity = (double)numSharedMembers / (double)totalMembers;
  LOG_VART(similarity);
  return similarity;
}

}
