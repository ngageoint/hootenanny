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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RemoveInvalidMultilineStringMembersVisitor.h"

//  hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/ops/RemoveRelationOp.h>
//#include <hoot-rnd/src/main/cpp/hoot/rnd/conflate/frechet/FrechetDistance.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveInvalidMultilineStringMembersVisitor)

RemoveInvalidMultilineStringMembersVisitor::RemoveInvalidMultilineStringMembersVisitor()
{
}

void RemoveInvalidMultilineStringMembersVisitor::visit(const ElementPtr &e)
{
  const double expansion = 15.0;
  if (e->getElementType() == ElementType::Relation)
  {
    Relation* r = dynamic_cast<Relation*>(e.get());
    assert(r != 0);

    bool removeRelation = false;
    if (r->getType() == Relation::MULTILINESTRING)
    {
      OsmMapPtr map(_map->shared_from_this());
      //  Remove members that are within the threshold distance from each other
      if (r->getMembers().size() > 1)
      {
        vector<ElementId> removeIds;
        vector<RelationData::Entry> members = r->getMembers();
        for (vector<RelationData::Entry>::iterator i = members.begin(); i != members.end(); i++)
        {
          bool keepElement1 = false;
          ElementId id1 = i->getElementId();
          ElementPtr element1 = _map->getElement(id1);
          Envelope* env1 = element1->getEnvelope(map);
          env1->expandBy(expansion);
          for (vector<RelationData::Entry>::iterator j = members.begin(); j != members.end(); j++)
          {
            if (i == j)
              continue;
            ElementId id2 = j->getElementId();
            ElementPtr element2 = _map->getElement(id2);
            Envelope* env2 = element2->getEnvelope(map);
            env2->expandBy(expansion);
            if (element1->getElementType() == ElementType::Way && element2->getElementType() == ElementType::Way)
            {
              if (env1->contains(env2))
                keepElement1 = true;
            }
          }
          if (!keepElement1)
            removeIds.push_back(id1);
        }
        //  Remove all elements from relation that need to be removed
        for (vector<ElementId>::iterator it = removeIds.begin(); it != removeIds.end(); it++)
          r->removeElement(*it);
      }
      //  Remove multilinestring relations with only one member, above operation could
      //  remove members and bring the count down to below two
      if (r->getMembers().size() < 2)
        removeRelation = true;
    }

    if (removeRelation)
    {
      LOG_TRACE("Removing multilinestring relation with ID: " << r->getId());
      RemoveRelationOp::removeRelation(_map->shared_from_this(), r->getId());
    }
  }
}

}
