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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "DecomposeBuildingRelationsVisitor.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

int DecomposeBuildingRelationsVisitor::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ElementVisitor, DecomposeBuildingRelationsVisitor)

void DecomposeBuildingRelationsVisitor::visit(const ConstElementPtr& e)
{
  if (e == nullptr && e->getElementType() == ElementType::Relation)
  {
    const std::shared_ptr<Relation>& r = _map->getRelation(e->getId());
    if (r->getType() == MetadataTags::RelationBuilding())
      _decomposeBuilding(r);
    _numProcessed++;
  }
}

void DecomposeBuildingRelationsVisitor::_decomposeBuilding(const std::shared_ptr<Relation>& r)
{
  if (r == nullptr)
    return;
  //  Copy the members so that the relation can be modified below
  const vector<RelationData::Entry> members = r->getMembers();
  for (const auto& member : members)
  {
    ElementId eid = member.getElementId();
    r->removeElement(eid);
    if (eid.getType() == ElementType::Node)
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Unexpected node encountered in building relation. " << r->getElementId());
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
      continue;
    }
    // we're dropping the outline. We only care about the parts.
    else if (member.getRole() == MetadataTags::RoleOutline())
      continue;
    else if (member.getRole() != MetadataTags::RolePart())
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Encountered an unexpected role in a building relation. " << r->getElementId());
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }

    // ok, we've got a building part. Recompose it as a building.
    std::shared_ptr<Element> e = _map->getElement(member.getElementId());
    if (e == nullptr)
      continue;
    //  Copy the tags from the relation to the new element
    Tags t = r->getTags();
    t.add(e->getTags());
    // don't need the building:part tag anymore.
    t.remove(MetadataTags::BuildingPart());

    if (!t.contains(MetadataTags::Building()))
      t[MetadataTags::Building()] = "yes";

    e->setTags(t);
  }

  // remove the building relation
  RecursiveElementRemover(r->getElementId()).apply(_map);
}

}
