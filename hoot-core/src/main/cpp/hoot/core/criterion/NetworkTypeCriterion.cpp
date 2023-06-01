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
 * @copyright Copyright (C) 2021-2023 Maxar (http://www.maxar.com/)
 */
#include "NetworkTypeCriterion.h"

// hoot
#include <hoot/core/conflate/matching/NodeMatcher.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, NetworkTypeCriterion)

bool NetworkTypeCriterion::isSatisfied(const ConstElementPtr& element) const
{
  if (element->getElementType() != ElementType::Way)
    return false;

  if (NodeMatcher::isNetworkFeatureType(element))
    return true;

  // If the element itself wasn't a network type, see if it belongs to a relation that is.
  const std::set<long>& relations = _map->getIndex().getElementToRelationMap()->getRelationByElement(element->getElementId());
  LOG_VART(relations.size());
  for (auto relationId : relations)
  {
    if (NodeMatcher::isNetworkFeatureType(_map->getRelation(relationId)))
      return true;
  }

  return false;
}

}

