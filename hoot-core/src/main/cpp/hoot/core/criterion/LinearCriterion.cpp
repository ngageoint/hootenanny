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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "LinearCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/elements/Way.h>
//#include <hoot/core/criterion/RelationWithLinearMembersCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, LinearCriterion)

bool LinearCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getElementId());

  if (e->getElementType() == ElementType::Node)
  {
    return false;
  }
  else if (e->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(e);
    if (isLinearRelation(relation))
    {
      LOG_TRACE(e->getElementId() << " is linear relation and passes LinearCriterion.");
      return true;
    }
//    RelationWithLinearMembersCriterion crit(_map);
//    crit.setAllowMixedChildren(true);
//    return crit.isSatisfied(e);
  }
  else if (e->getElementType() == ElementType::Way)
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(e);

    if (way->isClosedArea())
    {
      LOG_TRACE("Way is a closed area, so fails LinearCriterion.");
      return false;
    }

    LOG_TRACE(e->getElementId() << " passes LinearCriterion.");
    return true;
  }

  // TODO: explain

  LOG_TRACE(e->getElementId() << " fails LinearCriterion.");
  return false;
}

bool LinearCriterion::isLinearRelation(const ConstRelationPtr& relation)
{
  // This list could get HUGE - see #4151.
  return relation->getType() == MetadataTags::RelationMultilineString() ||
         relation->getType() == MetadataTags::RelationRoute() ||
         relation->getType() == MetadataTags::RelationBoundary() ||
         relation->getType() == MetadataTags::RelationRouteMaster() ||
         relation->getType() == MetadataTags::RelationSuperRoute() ||
         relation->getType() == MetadataTags::RelationRestriction() ||
         relation->getType() == MetadataTags::RelationPublicTransport() ||
         relation->getType() == MetadataTags::RelationWaterway() ||
         relation->getType() == MetadataTags::RelationNetwork() ||
         relation->getType() == MetadataTags::RelationEnforcement() ||
         relation->getType() == MetadataTags::RelationConnectivity() ||
         relation->getType() == MetadataTags::RelationSite() ||
         relation->getType() == MetadataTags::RelationAssociatedStreet() ||
         relation->getType() == "level";
}

}

