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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "PolygonCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/criterion/PolygonWayNodeCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, PolygonCriterion)

PolygonCriterion::PolygonCriterion(ConstOsmMapPtr map) :
_map(map)
{
  // Set this to allow any on poly child member to satisfy the crit.
  _relationCrit.setAllowMixedChildren(true);
  _relationCrit.setOsmMap(map.get());
}

void PolygonCriterion::setOsmMap(const OsmMap* map)
{
  _map = map->shared_from_this();
  _relationCrit.setOsmMap(map);
}

bool PolygonCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getElementId());

  if (e->getElementType() == ElementType::Node)
  {
    return false;
  }
  else if (e->getElementType() == ElementType::Relation)
  {
    // We use to define poly relations using a static list of relation types. Now, we look at the
    // member contents instead. If any member is a poly, then we call it a poly relation.
    const bool result = _relationCrit.isSatisfied(e);
    if (result)
    {
      LOG_TRACE("Relation has polygon members; crit satisified.");
    }
    return result;
  }
  else if (e->getElementType() == ElementType::Way)
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(e);
    LOG_VART(way->isValidPolygon());
    LOG_VART(way->isClosedArea());
    if (way->isValidPolygon() && way->isClosedArea())
    {
      LOG_TRACE("Way is valid closed area; crit satisfied.");
      return true;
    }
  }

  return false;
}

QStringList PolygonCriterion::getChildCriteria() const
{
  QStringList criteria;
  criteria.append(PolygonWayNodeCriterion::className());
  return criteria;
}

}

