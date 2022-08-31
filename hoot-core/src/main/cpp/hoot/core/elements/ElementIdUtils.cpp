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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ElementIdUtils.h"

// Hoot
#include <hoot/core/criterion/AttributeValueCriterion.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>

namespace hoot
{

QSet<ElementId> ElementIdUtils::elementsToElementIds(const std::vector<ElementPtr>& elements)
{
  QSet<ElementId> ids;
  for (const auto& element : elements)
  {
    if (element)
      ids.insert(element->getElementId());
  }
  return ids;
}

QSet<long> ElementIdUtils::elementIdsToIds(const std::set<ElementId>& elementIds)
{
  QSet<long> ids;
  const ElementType firstType = elementIds.begin()->getType();
  for (const auto& elementId : elementIds)
  {
    if (elementId.getType() != firstType)
      throw IllegalArgumentException("All element IDs must be of the same type.");
    ids.insert(elementId.getId());
  }
  return ids;
}

bool ElementIdUtils::containsElementId(const ElementId& id, const QList<ElementPtr>& elements)
{
  for (const auto& element : qAsConst(elements))
  {
    if (element && element->getElementId() == id)
      return true;
  }
  return false;
}

}
