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
 * @copyright Copyright (C) 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RelationWithMembersOfTypeCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/RelationMemberUtils.h>

namespace hoot
{

RelationWithMembersOfTypeCriterion::RelationWithMembersOfTypeCriterion() :
_allowMixedChildren(false)
{
}

RelationWithMembersOfTypeCriterion::RelationWithMembersOfTypeCriterion(ConstOsmMapPtr map) :
_map(map),
_allowMixedChildren(false)
{
}

void RelationWithMembersOfTypeCriterion::_initCrit() const
{
  _crit =
    std::shared_ptr<ElementCriterion>(
      Factory::getInstance().constructObject<ElementCriterion>(getCriterion()));
  if (_map)
  {
    std::shared_ptr<ConstOsmMapConsumer> consumer =
      std::dynamic_pointer_cast<ConstOsmMapConsumer>(_crit);
    if (consumer.get())
    {
      consumer->setOsmMap(_map.get());
    }
  }
}

void RelationWithMembersOfTypeCriterion::setOsmMap(const OsmMap* map)
{
  _map = map->shared_from_this();
}

void RelationWithMembersOfTypeCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _allowMixedChildren = options.getRelationWithMembersOfTypeCriterionAllowMixedChildren();
}

bool RelationWithMembersOfTypeCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getElementId());

  bool result = false;
  if (e->getElementType() == ElementType::Relation)
  {
    if (!_map)
    {
      throw IllegalArgumentException("No map set on RelationWithMembersOfTypeCriterion.");
    }
    if (!_crit)
    {
      _initCrit();
    }

    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(e);
    if (_allowMixedChildren)
    {
      result = RelationMemberUtils::containsMemberWithCriterion(relation, *_crit, _map);
      if (result)
      {
        LOG_TRACE("Contains member with criterion.");
      }
      else
      {
        LOG_TRACE("Does not contain a member with criterion.");
      }
    }
    else
    {
      result = RelationMemberUtils::containsOnlyMembersWithCriterion(relation, *_crit, _map);
      if (result)
      {
        LOG_TRACE("Contains only members with criterion.");
      }
      else
      {
        LOG_TRACE("Does not contain only members with criterion.");
      }
    }
  }
  LOG_VART(result);
  return result;
}

}

