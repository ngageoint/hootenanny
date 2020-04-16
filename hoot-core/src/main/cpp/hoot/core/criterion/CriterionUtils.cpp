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

#include "CriterionUtils.h"

// Hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

bool CriterionUtils::hasCriterion(const ConstElementPtr& element, const QString& criterionClassName)
{
  if (!element || criterionClassName.trimmed().isEmpty())
  {
    throw IllegalArgumentException(
      "The input element is null or the criterion class name is empty.");
  }

  return _getCrit(criterionClassName)->isSatisfied(element);
}

ElementCriterionPtr CriterionUtils::_getCrit(const QString& criterionClassName)
{
  if (criterionClassName.trimmed().isEmpty())
  {
    throw IllegalArgumentException("The criterion class name is empty.");
  }

  ElementCriterionPtr crit =
    ElementCriterionPtr(
      Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
  if (!crit)
  {
    throw IllegalArgumentException(
      "Invalid criterion passed to PoiPolygonInfoCache::hasCriterion: " + criterionClassName);
  }
  return crit;
}

}
