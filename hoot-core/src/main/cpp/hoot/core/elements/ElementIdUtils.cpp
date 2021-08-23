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
#include <hoot/core/util/HootException.h>

#include <hoot/core/criterion/AttributeValueCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>

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

QSet<long> ElementIdUtils::elementIdsToIds(const std::set<ElementId>& elementIds)
{
  QSet<long> ids;
  const ElementType firstType = elementIds.begin()->getType();
  for (std::set<ElementId>::const_iterator itr = elementIds.begin(); itr != elementIds.end(); ++itr)
  {
    const ElementId elementId = *itr;
    if (elementId.getType() != firstType)
    {
      throw IllegalArgumentException("All element IDs must be of the same type.");
    }
    ids.insert(elementId.getId());
  }
  return ids;
}

bool ElementIdUtils::containsElementId(const ElementId& id, const QList<ElementPtr>& elements)
{
  int ctr = 0;
  for (QList<ElementPtr>::const_iterator itr = elements.begin(); itr != elements.end(); ++itr)
  {
    ElementPtr element = *itr;
    if (element && element->getElementId() == id)
    {
      return true;
    }
    ctr++;
  }
  return false;
}

}
