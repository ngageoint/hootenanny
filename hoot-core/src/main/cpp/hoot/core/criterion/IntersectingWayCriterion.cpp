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
#include "IntersectingWayCriterion.h"

// hoot
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

const QString IntersectingWayCriterion::EMPTY_SOURCE_IDS_ERROR_MESSAGE = "No way IDs were passed to IntersectingWayCriterion.";

HOOT_FACTORY_REGISTER(ElementCriterion, IntersectingWayCriterion)

IntersectingWayCriterion::IntersectingWayCriterion(const QSet<long>& sourceWayIds, ConstOsmMapPtr map)
  : ConstOsmMapConsumerImpl(map),
    _sourceWayIds(sourceWayIds)
{
  if (_sourceWayIds.empty())
    throw IllegalArgumentException(EMPTY_SOURCE_IDS_ERROR_MESSAGE);
}

ElementCriterionPtr IntersectingWayCriterion::clone()
{
  if (!_sourceWayIds.empty())
    return std::make_shared<IntersectingWayCriterion>(_sourceWayIds, _map);
  else
    return std::make_shared<IntersectingWayCriterion>(_map);
}

void IntersectingWayCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  const QStringList idStrs = opts.getIntersectingWayCriterionSourceWayIds();
  for (const auto& idStr : qAsConst(idStrs))
  {
    bool converted = false;
    const long id = idStr.toLong(&converted);
    if (!converted)
      throw IllegalArgumentException("Invalid way id passed to IntersectingWayCriterion: " + idStr);
    _sourceWayIds.insert(id);
  }
  if (_sourceWayIds.empty())
    throw IllegalArgumentException(EMPTY_SOURCE_IDS_ERROR_MESSAGE);
}

bool IntersectingWayCriterion::isSatisfied(const ConstElementPtr& element) const
{
  if (_sourceWayIds.empty())
    throw IllegalArgumentException(EMPTY_SOURCE_IDS_ERROR_MESSAGE);
  if (element->getElementType() != ElementType::Way)
    return false;
  // Don't consider any ways in our input list, as we only want to find other ways that intersect them.
  if (_sourceWayIds.find(element->getId()) != _sourceWayIds.end())
    return false;

  LOG_VART(element->getElementId());
  // Get all the ways that intersect the way we're examining.
  QSet<long> intersectingWayIdsSet =
    QVector<long>::fromStdVector(WayUtils::getIntersectingWayIdsConst(element->getId(), _map)).toList().toSet();
  LOG_VART(intersectingWayIdsSet);
  // Make a copy of our input way IDs so we don't modify them with the set intersection call.
  QSet<long> sourceWayIds = _sourceWayIds;
  // If any of the intersecting way IDs are in our input list (_wayIds), we have an intersecting way.
  return !sourceWayIds.intersect(intersectingWayIdsSet).empty();
}

}
