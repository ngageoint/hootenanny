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
#include <hoot/core/schema/TagMergerFactory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveInvalidMultilineStringMembersVisitor)

RemoveInvalidMultilineStringMembersVisitor::RemoveInvalidMultilineStringMembersVisitor()
{
}

void RemoveInvalidMultilineStringMembersVisitor::visit(const ElementPtr &e)
{
  //  Only look for relations
  if (e->getElementType() == ElementType::Relation)
  {
    Relation* r = dynamic_cast<Relation*>(e.get());
    assert(r != 0);
    //  Only multilinestring relations
    if (r->getType() == MetadataTags::RelationMultilineString())
    {
      vector<RelationData::Entry> multi_members = r->getMembers();
      Tags& tags = r->getTags();
      for (vector<RelationData::Entry>::iterator it = multi_members.begin(); it != multi_members.end(); ++it)
      {
        if (_map->getElement(it->getElementId())->getTags().getInformationCount() > 0)
          return;
      }
      const double expansion = r->getCircularError();
      OsmMapPtr map(_map->shared_from_this());
      //  Multiline strings that are a part of a review relation are what we are targetting for replacement
      set<ElementId> parents = map->getParents(r->getElementId());
      for (set<ElementId>::iterator it = parents.begin(); it != parents.end(); ++it)
      {
        ElementPtr p = map->getElement(*it);
        if (p->getElementType() == ElementType::Relation)
        {
          Relation* rev = dynamic_cast<Relation*>(p.get());
          if (rev->getType() == MetadataTags::RelationReview())
          {
            vector<RelationData::Entry> members = rev->getMembers();
            //  Iterate all of the members of the review looking for non-relation entities
            for (vector<RelationData::Entry>::iterator i = members.begin(); i != members.end(); i++)
            {
              ElementId id1 = i->getElementId();
              if (id1.getType() == ElementType::Relation)
                continue;
              //  Found a non-relation entity to test against
              ElementPtr element1 = map->getElement(id1);
              Envelope* env1 = element1->getEnvelope(map);
              env1->expandBy(expansion);
              for (vector<RelationData::Entry>::iterator j = multi_members.begin(); j != multi_members.end(); j++)
              {
                ElementId id2 = j->getElementId();
                ElementPtr element2 = _map->getElement(id2);
                Envelope* env2 = element2->getEnvelope(map);
                env2->expandBy(expansion);
                //  Add the element to the review if the two envelopes intersect
                if (env1->intersects(env2))
                  rev->addElement(MetadataTags::RoleReviewee(), id2);
              }
            }
            rev->removeElement(r->getElementId());
          }
        }
      }
      //  Copy tags from the multiline string tags to the children and remove from relation
      vector<RelationData::Entry> members = r->getMembers();
      TagMergerFactory& merger = TagMergerFactory::getInstance();
      for (vector<RelationData::Entry>::iterator i = members.begin(); i != members.end(); i++)
      {
        ElementId id = i->getElementId();
        ElementPtr element = _map->getElement(id);
        Tags merged = merger.mergeTags(element->getTags(), tags, id.getType());
        element->setTags(merged);
        r->removeElement(id);
      }
      RemoveRelationOp::removeRelation(map, r->getId());
    }
  }
}

}
