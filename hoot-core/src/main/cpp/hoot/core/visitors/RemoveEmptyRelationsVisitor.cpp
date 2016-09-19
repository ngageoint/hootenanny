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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RemoveEmptyRelationsVisitor.h"

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/ops/RemoveRelationOp.h>

namespace hoot
{

RemoveEmptyRelationsVisitor::RemoveEmptyRelationsVisitor()
{
}

void RemoveEmptyRelationsVisitor::visit(const shared_ptr<Element>& e)
{
  if (e->getElementType() == ElementType::Relation)
  {
    Relation* r = dynamic_cast<Relation*>(e.get());
    assert(r != 0);

    if (r->getMembers().size() == 0)
    {
      RemoveRelationOp::removeRelation(_map->shared_from_this(), r->getId());
    }
  }
}

}
