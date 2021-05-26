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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "IntersectingWayCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/WayUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, IntersectingWayCriterion)

IntersectingWayCriterion::IntersectingWayCriterion(
  const QSet<long>& wayIds, ConstOsmMapPtr map, const ElementCriterionPtr& crit) :
_wayIds(wayIds),
_map(map),
_crit(crit)
{
}

bool IntersectingWayCriterion::isSatisfied(const ConstElementPtr& element) const
{
  if (element->getElementType() != ElementType::Way)
  {
    return false;
  }
  if (_crit && !_crit->isSatisfied(element))
  {
    return false;
  }
  // Don't consider ways in our list, as we only want to count other ways that intersect them.
  if (_wayIds.find(element->getId()) != _wayIds.end())
  {
    return false;
  }

  LOG_VART(element->getElementId());
  // Get all the ways that intersect the way we're examining.
  QSet<long> intersectingWayIdsSet =
    QVector<long>::fromStdVector(
      WayUtils::getIntersectingWayIdsConst(element->getId(), _map)).toList().toSet();
  LOG_VART(intersectingWayIdsSet);
  // Make a copy, so we don't modify _roadIds with the call to intersect.
  QSet<long> wayIds = _wayIds;
  // If any of the intersecting way IDs are in our input list (_wayIds), we have an intersecting
  // way.
  return wayIds.intersect(intersectingWayIdsSet).size() > 0;
}

}

