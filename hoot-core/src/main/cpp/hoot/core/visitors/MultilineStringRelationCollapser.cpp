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

#include "MultilineStringRelationCollapser.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/criterion/TagKeyCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MultilineStringRelationCollapser)

MultilineStringRelationCollapser::MultilineStringRelationCollapser() :
_numRelationMembersModified(0)
{
}

void MultilineStringRelationCollapser::setConfiguration(const Settings& conf)
{
  _negateCriteria = false;
  _chainCriteria = true;

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

void MultilineStringRelationCollapser::visit(const ElementPtr& e)
{
  _numProcessed++;

  if (!e)
  {
    return;
  }

  if (!_criteriaSatisfied(e))
  {
    LOG_TRACE("Element did not satisfy criteria: " << e->getElementId() << ". Skipping...");
    return;
  }

  // This doesn't handle multiple matching types. Only grabs the first one.
  bool matchingTypeIsKey = false;
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
      LOG_VART(matchingType);
      LOG_VART(matchingTypeIsKey);

      bool anyMemberModified = false;
      const std::vector<RelationData::Entry>& members = relation->getMembers();
      for (std::vector<RelationData::Entry>::const_iterator membersItr = members.begin();
           membersItr != members.end(); ++membersItr)
      {
        RelationData::Entry member = *membersItr;
        ElementPtr memberElement = _map->getElement(member.getElementId());
        if (memberElement)
        {
          // update member tag
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
          LOG_TRACE(
            "Removing " << memberElement->getElementId() << " from " << relation->getElementId() <<
            "...");
          _numRelationMembersModified++;
          anyMemberModified = true;
        }
      }
      if (anyMemberModified)
      {
        _numAffected++;
      }

      LOG_VART(relation->getMemberCount());
      // remove relation
      LOG_TRACE("Non-recursively removing " << relation->getElementId() << "...");
      RemoveRelationByEid::removeRelation(_map, relation->getId());
    }
  }
}

}
