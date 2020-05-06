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

#include "ElementIdUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/AttributeValueCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/criterion/IdTagMatchesId.h>

namespace hoot
{

QSet<ElementId> ElementIdUtils::elementsToElementIds(const std::vector<ElementPtr>& elements)
{
  QSet<ElementId> ids;
  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ElementPtr element = *it;
    if (element)
    {
      ids.insert(element->getElementId());
    }
  }
  return ids;
}

bool ElementIdUtils::allElementIdsPositive(const ConstOsmMapPtr& map)
{
  std::shared_ptr<AttributeValueCriterion> attrCrit(
    new AttributeValueCriterion(
      ElementAttributeType(ElementAttributeType::Id), 1, NumericComparisonType::LessThan));
  return
    (int)FilteredVisitor::getStat(
      attrCrit, std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map) == 0;
}

bool ElementIdUtils::allElementIdsNegative(const ConstOsmMapPtr& map)
{
  std::shared_ptr<AttributeValueCriterion> attrCrit(
    new AttributeValueCriterion(
      ElementAttributeType(ElementAttributeType::Id), -1, NumericComparisonType::GreaterThan));
  return
    (int)FilteredVisitor::getStat(
      attrCrit, std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map) == 0;
}

bool ElementIdUtils::allIdTagsMatchIds(const ConstOsmMapPtr& map)
{
  std::shared_ptr<IdTagMatchesId> idCrit(new IdTagMatchesId());
  return
    (int)FilteredVisitor::getStat(
      idCrit, std::shared_ptr<ElementCountVisitor>(new ElementCountVisitor()), map) ==
    (int)map->size();
}

}
