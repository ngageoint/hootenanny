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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "DecomposeBuildingRelationsVisitor.h"

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/Factory.h>
#include <hoot/core/elements/Relation.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, DecomposeBuildingRelationsVisitor)

DecomposeBuildingRelationsVisitor::DecomposeBuildingRelationsVisitor()
{
}

void DecomposeBuildingRelationsVisitor::visit(const ConstElementPtr& e)
{
  if (e->getElementType() == ElementType::Relation)
  {
    const shared_ptr<Relation>& r = _map->getRelation(e->getId());
    if (r->getType() == "building")
    {
      _decomposeBuilding(r);
    }
  }
}

void DecomposeBuildingRelationsVisitor::_decomposeBuilding(const shared_ptr<Relation>& r)
{
  Tags baseTags = r->getTags();

  const vector<RelationData::Entry> members = r->getMembers();

  for (size_t i = 0; i < members.size(); ++i)
  {
    ElementId eid = members[i].getElementId();
    r->removeElement(eid);
    if (eid.getType() == ElementType::Node)
    {
      LOG_WARN("Unexpected node encountered in building relation. " << r->getElementId());
      continue;
    }
    // we're dropping the outline. We only care about the parts.
    else if (members[i].getRole() == "outline")
    {
      continue;
    }
    else if (members[i].getRole() != "part")
    {
      LOG_WARN("Encountered an unexpected role in a building relation. " << r->getElementId());
    }

    // ok, we've got a building part. Recompose it as a building.
    shared_ptr<Element> e = _map->getElement(members[i].getElementId());

    Tags t = baseTags;
    t.addTags(e->getTags());
    // don't need the building:part tag anymore.
    t.remove("building:part");

    if (!t.contains("building"))
    {
      t["building"] = "yes";
    }

    e->setTags(t);
  }

  // remove the building relation
  RecursiveElementRemover(r->getElementId()).apply(_map->shared_from_this());
}

}
