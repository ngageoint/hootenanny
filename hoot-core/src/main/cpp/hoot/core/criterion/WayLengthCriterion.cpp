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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "WayLengthCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/ElementGeometryUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, WayLengthCriterion)

WayLengthCriterion::WayLengthCriterion() :
_comparisonLength(-1.0),
_numericComparisonType(NumericComparisonType::EqualTo)
{
}

WayLengthCriterion::WayLengthCriterion(
  const double comparisonLength, const NumericComparisonType& numericComparisonType,
  ConstOsmMapPtr map) :
_comparisonLength(comparisonLength),
_numericComparisonType(numericComparisonType),
_map(map)
{
}

bool WayLengthCriterion::isSatisfied(const ConstElementPtr& e) const
{
  // A comparison length of -1 effectively bypasses this crit.
  if (_comparisonLength == -1.0)
  {
    return true;
  }
  if (!_map)
  {
    throw IllegalArgumentException("WayLengthCriterion requires a map.");
  }

  if (e && e->getElementType() == ElementType::Way)
  {
    LOG_VART(e->getElementId());
    return
      _numericComparisonType.satisfiesComparison(
        ElementGeometryUtils::calculateLength(e, _map), _comparisonLength);
  }
  return false;
}

}
