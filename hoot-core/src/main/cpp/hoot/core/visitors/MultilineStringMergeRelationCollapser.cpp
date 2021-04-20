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

#include "MultilineStringMergeRelationCollapser.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/elements/RelationMemberUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MultilineStringMergeRelationCollapser)

MultilineStringMergeRelationCollapser::MultilineStringMergeRelationCollapser() :
_mergeAllTypes(false),
_numRelationMembersModified(0)
{
  _relationMerger.setMergeTags(false);
  // We don't want RelationMerger to auto-delete the relation we're merging in.
  _relationMerger.setDeleteRelation2(false);
}

void MultilineStringMergeRelationCollapser::setOsmMap(OsmMap* map)
{
  _map = map->shared_from_this();
  _relationMerger.setOsmMap(_map.get());
}

void MultilineStringMergeRelationCollapser::setTypes(const QStringList& types)
{
  for (int i = 0; i < types.size(); i++)
  {
    const QString type = types.at(i).trimmed();
    if (!type.isEmpty())
    {
      // Each list entry can either be:

      // a ';' delimited kvp,
      if (Tags::isValidKvp(type) && !_typeKvps.contains(type))
      {
        _typeKvps.append(type);
      }
      // or just a type key.
      else if (!_typeKeys.contains(type))
      {
        _typeKeys.append(type);
      }
    }
  }
  LOG_VART(_typeKvps);
  LOG_VART(_typeKeys);
}

void MultilineStringMergeRelationCollapser::setConfiguration(const Settings& conf)
{
  _negateCriteria = false;
  _chainCriteria = true;

  // We're only interested in modifying relations that were tagged during conflate merging.
  QStringList critNames;
  critNames.append(RelationCriterion::className());
  critNames.append(TagKeyCriterion::className());
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
  LOG_VART(_criteria.size());

  ConfigOptions opts(conf);

  // Create a list of types to search for on the relations we process. Any types found will be
  // transferred to relation members.
  const QStringList types = opts.getMultilinestringRelationCollapserTypes();
  LOG_VART(types);
  if (types.size() == 1 && types.contains("*"))
  {
    _mergeAllTypes = true;
  }
  else
  {
    setTypes(opts.getMultilinestringRelationCollapserTypes());
  }
  LOG_VART(_mergeAllTypes);
}

void MultilineStringMergeRelationCollapser::visit(const ElementPtr& e)
{
  if (!_mergeAllTypes && _typeKeys.isEmpty() && _typeKvps.isEmpty())
  {
    throw IllegalArgumentException(
      "MultilineStringMergeRelationCollapser has not been configured with any feature types.");
  }

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

  LOG_VART(e->getElementId());

  // Check the relation for any type tags matching the ones we added during configuration. This
  // doesn't handle multiple matching types. It only grabs the first one. No checking is done here
  // to verify the type passed in is contained in the schema...not sure yet if that needs to be done
  // at this point.
  bool matchingTypeIsKey = false;
  QString matchingType;
  if (_mergeAllTypes)
  {
    // returns a kvp
    matchingType = OsmSchema::getInstance().getFirstType(e->getTags(), true);
  }
  else
  {
    // Check to see whether its a key/value pair or just a type tag key.
    matchingType = e->getTags().getFirstMatchingKvp(_typeKvps);
    if (matchingType.isEmpty())
    {
      matchingType = e->getTags().getFirstMatchingKey(_typeKeys);
      matchingTypeIsKey = true;
    }
  }

  if (!matchingType.isEmpty())
  {
    RelationPtr relation = _map->getRelation(e->getId());
    LOG_VART(relation.get());
    if (relation)
    {
      LOG_VART(relation->getElementId());
      LOG_VART(relation->getType());
      LOG_VART(relation->getTags().getName());
      LOG_VART(matchingType);
      LOG_VART(matchingTypeIsKey);

      // Get any relations which may own our relation being collapsed.
      const std::vector<ConstRelationPtr> relationsOwningMsRelation =
        RelationMemberUtils::getContainingRelations(relation->getElementId(), _map);
      LOG_VART(relationsOwningMsRelation.size());

      // For all the members of our relation being collapsed,
      QSet<ElementId> parsedOwningRelationIds;
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

          _numRelationMembersModified++;
        }
      }

      // Merge the relation we're removing with any relations that own it.
      for (std::vector<ConstRelationPtr>::const_iterator owningRelationsItr =
             relationsOwningMsRelation.begin();
           owningRelationsItr != relationsOwningMsRelation.end(); ++owningRelationsItr)
      {
        RelationPtr relationOwningMsRelation =
          std::const_pointer_cast<Relation>(*owningRelationsItr);
        LOG_VART(relationOwningMsRelation.get());
        LOG_VART(parsedOwningRelationIds);
        if (relationOwningMsRelation &&
            !parsedOwningRelationIds.contains(relationOwningMsRelation->getElementId()))
        {
          LOG_VART(relationOwningMsRelation->getElementId());
          // Use relation merger here, as will make the member insert indexes be correct. Prevent
          // the merger from deleting the merged ms relation, as we may need to merge it with
          // multiple parent relations within this loop. It will be deleted at the end.
          _relationMerger.merge(
            relationOwningMsRelation->getElementId(), relation->getElementId());
          parsedOwningRelationIds.insert(relationOwningMsRelation->getElementId());
        }
      }
      LOG_VART(relation->getMemberCount());

      // Remove the ms relation we collapsed, since we didn't have RelationMerger remove it.
      LOG_TRACE("Non-recursively removing " << relation->getElementId() << "...");
      RemoveRelationByEid::removeRelation(_map, relation->getId());

      _numAffected++;
    }
  }
}

}
