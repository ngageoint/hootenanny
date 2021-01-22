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

#include "MultilineStringMergeRelationCollapser.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/elements/RelationMemberUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MultilineStringMergeRelationCollapser)

MultilineStringMergeRelationCollapser::MultilineStringMergeRelationCollapser() :
_numRelationMembersModified(0)
{
  _relationMerger.setMergeTags(false);
  _relationMerger.setDeleteRelation2(false);
}

void MultilineStringMergeRelationCollapser::setOsmMap(OsmMap* map)
{
  _map = map->shared_from_this();
  _relationMerger.setOsmMap(_map.get());
}

void MultilineStringMergeRelationCollapser::setConfiguration(const Settings& conf)
{
  _negateCriteria = false;
  _chainCriteria = true;

  // We're only interested in modifying relations that were tagged during conflate merging.
  QStringList critNames;
  critNames.append("hoot::RelationCriterion");
  critNames.append("hoot::TagKeyCriterion");
  _addCriteria(critNames);

  for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
       it != _criteria.end(); ++it)
  {
    ElementCriterionPtr crit = *it;
    std::shared_ptr<TagKeyCriterion> tagCrit = std::dynamic_pointer_cast<TagKeyCriterion>(crit);
    if (tagCrit)
    {
      tagCrit->addKey(MetadataTags::HootMultilineString());
    }
  }
  LOG_VARD(_criteria.size());

  // Create a list of types to search for on the relations we process. Any types found will be
  // transferred to relation members.
  ConfigOptions opts(conf);
  const QStringList types = opts.getMultilinestringRelationCollapserTypes();
  for (int i = 0; i < types.size(); i++)
  {
    const QString type = types.at(i).trimmed();
    if (!type.isEmpty())
    {
      if (Tags::isValidKvp(type) && !_typeKvps.contains(type))
      {
        _typeKvps.append(type);
      }
      else if (!_typeKeys.contains(type))
      {
        _typeKeys.append(type);
      }
    }
  }
  LOG_VARD(_typeKvps);
  LOG_VARD(_typeKeys);
}

void MultilineStringMergeRelationCollapser::visit(const ElementPtr& e)
{
  _numProcessed++;

  if (!e)
  {
    return;
  }

  // The criteria used here was set up in setConfiguration.
  if (!_criteriaSatisfied(e))
  {
    LOG_TRACE(e->getElementId() << " did not satisfy criteria: " << toString() << ". Skipping...");
    return;
  }

  // Check the relation for any type tags matching the ones we added during configuration. This
  // doesn't handle multiple matching types. Only grabs the first one. No checking is done here to
  // verify the type passed in is contained in the schema...not sure yet if that needs to be done at
  // this point.
  bool matchingTypeIsKey = false;
  // Check to see whether its a key/value pair or just a type tag key.
  QString matchingType = e->getTags().getFirstKvp(_typeKvps);
  if (matchingType.isEmpty())
  {
    matchingType = e->getTags().getFirstKey(_typeKeys);
    matchingTypeIsKey = true;
  }
  if (!matchingType.isEmpty())
  {
    RelationPtr relation = _map->getRelation(e->getId());
    if (relation)
    {
      LOG_VART(relation->getElementId());
      LOG_VART(relation->getType());
      LOG_VART(relation->getTags().getName());
      LOG_VART(matchingType);
      LOG_VART(matchingTypeIsKey);

      // Get any relations which may own our relation being collapsed.
      const std::vector<ConstRelationPtr> relationsOwningMsRelation =
        RelationMemberUtils::getContainingRelations(_map, relation->getElementId());
      LOG_VART(relationsOwningMsRelation.size());

      // For all the members of our relation being collapsed,
      bool anyMemberModified = false;
      const std::vector<RelationData::Entry>& members = relation->getMembers();
      for (std::vector<RelationData::Entry>::const_iterator membersItr = members.begin();
           membersItr != members.end(); ++membersItr)
      {
        RelationData::Entry member = *membersItr;
        ElementPtr memberElement = _map->getElement(member.getElementId());
        if (memberElement)
        {
          // ...update each member's tag.
          if (matchingTypeIsKey)
          {
            LOG_TRACE(
              "Adding " << matchingType << "=" << relation->getTag(matchingType) << " to " <<
              memberElement->getElementId() << "...");
            memberElement->setTag(matchingType, relation->getTag(matchingType));
          }
          else
          {
            LOG_TRACE(
              "Adding " << matchingType << " to " << memberElement->getElementId() << "...");
            memberElement->setTag(Tags::kvpToKey(matchingType), Tags::kvpToVal(matchingType));
          }

          // Insert the member element into any relation that the relation being collapsed was a
          // part of at the same index.
          for (std::vector<ConstRelationPtr>::const_iterator owningRelationsItr =
                 relationsOwningMsRelation.begin();
               owningRelationsItr != relationsOwningMsRelation.end(); ++owningRelationsItr)
          {
            RelationPtr relationOwningMsRelation =
              std::const_pointer_cast<Relation>(*owningRelationsItr);
            if (relationOwningMsRelation)
            {
              // Use relation merger here, as will make the member insert indexes be correct.
              // Prevent the merger from deleting the merged ms relation, we may need to merge it
              // with multiple parent relations and will delete it outside of this loop.
              _relationMerger.merge(
                relationOwningMsRelation->getElementId(), relation->getElementId());
            }
          }

          _numRelationMembersModified++;
          anyMemberModified = true;
        }
      }
      LOG_VART(anyMemberModified);
      if (anyMemberModified)
      {
        _numAffected++;
      }
      LOG_VART(relation->getMemberCount());

      // Remove the ms relation we collapsed.
      LOG_TRACE("Non-recursively removing " << relation->getElementId() << "...");
      RemoveRelationByEid::removeRelation(_map, relation->getId());
    }
  }
}

}
