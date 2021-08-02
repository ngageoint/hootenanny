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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "RemoveReviewUnknown2Visitor.h"

// hoot
#include <hoot/core/elements/Relation.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveReviewUnknown2Visitor)

void RemoveReviewUnknown2Visitor::visit(const std::shared_ptr<Element>& e)
{
  if (e->getElementType() == ElementType::Relation)
  {
    const Relation* r = dynamic_cast<Relation*>(e.get());
    OsmMapPtr map = _map->shared_from_this();

    //  Only look at review relations
    if (r->getType() == MetadataTags::RelationReview())
    {
      const std::vector<RelationData::Entry>& relationMembers = r->getMembers();
      //  Iterate all of the relation members
      foreach (RelationData::Entry member, relationMembers)
      {
        ElementId id = member.getElementId();
        std::shared_ptr<Element> element(map->getElement(id));
        //  Remove all UNKNOWN2 elements in this relation
        if (element->getStatus() == Status::Unknown2)
        {
          RemoveElementByEid::removeElement(map, id);
          _numElements++;
        }
      }
      //  Finally remove the review relation
      RemoveElementByEid::removeElement(map, r->getElementId());
      _numAffected++;
    }
  }
}

}
