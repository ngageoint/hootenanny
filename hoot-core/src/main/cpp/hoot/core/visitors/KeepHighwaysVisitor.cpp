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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "KeepHighwaysVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/AreaCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, KeepHighwaysVisitor)

void KeepHighwaysVisitor::visit(const ConstElementPtr& e)
{
  ElementType type = e->getElementType();
  long id = e->getId();

  if (HighwayCriterion(_map->shared_from_this()).isSatisfied(e) == false ||
      AreaCriterion().isSatisfied(e))
  {
    // we don't want to accidentally delete a highway by deleting a relation that contains
    // highways.
    if (type == ElementType::Relation)
    {
      if (_map->getIndex().getParents(e->getElementId()).size() == 0)
      {
        RemoveRelationByEid::removeRelation(_map->shared_from_this(), id);
      }
    }
    else
    {
      RecursiveElementRemover(ElementId(type, id)).apply(_map->shared_from_this());
    }
  }
}

}
