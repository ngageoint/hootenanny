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
 * @copyright Copyright (C) 2017-2023 Maxar (http://www.maxar.com/)
 */

#include "RemoveInvalidMultilineStringMembersVisitor.h"

//  hoot
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveInvalidMultilineStringMembersVisitor)

RemoveInvalidMultilineStringMembersVisitor::RemoveInvalidMultilineStringMembersVisitor()
  : _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void RemoveInvalidMultilineStringMembersVisitor::visit(const ElementPtr& e)
{
  if (!e)
    return;

  // Only look for relations
  if (e->getElementType() == ElementType::Relation)
  {
    Relation* r = dynamic_cast<Relation*>(e.get());
    LOG_VART(r->getElementId());
    assert(r != 0);
    LOG_VART(r->getType());
    // Only multilinestring relations
    if (r->getType() == MetadataTags::RelationMultilineString())
    {
      const vector<RelationData::Entry>& multi_members = r->getMembers();
      const Tags& tags = r->getTags();
      //  Empty tags on multilinestring relations can be removed
      if (tags.getInformationCount() > 0)
      {
        for (const auto& member : multi_members)
        {
          ElementPtr element = _map->getElement(member.getElementId());
          if (element)
          {
            const Tags& member_tags = element->getTags();
            LOG_VART(member_tags.getInformationCount());
            if (member_tags.getInformationCount() > 0 && member_tags != tags)
                return;
          }
        }
      }

      const double expansion = r->getCircularError() / 2.0;
      OsmMapPtr map(_map->shared_from_this());
      // Multiline strings that are a part of a review relation are what we are targeting for
      // replacement
      set<ElementId> parents = map->getParents(r->getElementId());
      for (const auto& eid : parents)
      {
        ElementPtr parent = map->getElement(eid);
        if (!parent)
          continue;
        LOG_VART(parent->getElementId());
        if (parent->getElementType() == ElementType::Relation)
        {
          Relation* rev = dynamic_cast<Relation*>(parent.get());
          LOG_VART(rev->getType());

          if (rev->getType() == MetadataTags::RelationReview())
          {
            // Copy members here so that the original can be modified later
            vector<RelationData::Entry> members = rev->getMembers();
            // Iterate all of the members of the review looking for non-relation entities
            for (const auto& member : members)
            {
              ElementId id1 = member.getElementId();
              LOG_VART(id1);
              if (id1.getType() == ElementType::Relation)
                continue;
              // Found a non-relation entity to test against
              ElementPtr element1 = map->getElement(id1);
              std::shared_ptr<Envelope> env1 = element1->getEnvelope(map);
              env1->expandBy(expansion);
              for (const auto& multi_member : multi_members)
              {
                ElementId id2 = multi_member.getElementId();
                LOG_VART(id2);
                ElementPtr element2 = _map->getElement(id2);
                std::shared_ptr<Envelope> env2 = element2->getEnvelope(map);
                env2->expandBy(expansion);
                //  Add the element to the review if the two envelopes intersect
                if (env1->intersects(env2.get()))
                {
                  LOG_TRACE("Adding element as reviewee: " << id2 << "...");
                  rev->addElement(MetadataTags::RoleReviewee(), id2);
                }
              }
            }

            // There is a one-to-one relation here, replace the multilinestring with the only way
            if (rev->getMemberCount() == 1 && r->getMemberCount() == 1)
            {
              LOG_TRACE("Adding element as reviewee: " << r->getMembers()[0].getElementId() << "...");
              rev->addElement(MetadataTags::RoleReviewee(), r->getMembers()[0].getElementId());
            }

            // Don't remove multilinestring relations that are members of a review relation
            // only contains one member that is the original multilinestring
            if (rev->getMemberCount() > 1)
            {
              LOG_TRACE("Removing: " << r->getElementId() << "...");
              rev->removeElement(r->getElementId());
              _numAffected++;
            }
          }
        }
      }

      // Don't remove multilinestring relations that are members of a review relation
      // only contains one member that is the original multilinestring
      LOG_VART(map->getParents(r->getElementId()).size());
      if (!map->getParents(r->getElementId()).empty())
        return;

      // Copy tags from the multiline string tags to the children and remove from relation
      vector<RelationData::Entry> members = r->getMembers();
      for (const auto& member : members)
      {
        ElementId id = member.getElementId();
        ElementPtr element = _map->getElement(id);
        if (element)
        {
          Tags merged = TagMergerFactory::mergeTags(element->getTags(), tags, id.getType());
          element->setTags(merged);
        }
        LOG_TRACE("Removing: " << id << "...");
        r->removeElement(id);
      }
      LOG_TRACE("Removing: " << r->getElementId() << "...");
      RemoveRelationByEid::removeRelation(map, r->getId());
      _numAffected++;
    }
  }

  _numProcessed++;
  if (_numProcessed % (_taskStatusUpdateInterval * 10) == 0)
  {
    PROGRESS_INFO(
      "\tRemoved " << StringUtils::formatLargeNumber(_numAffected) << " invalid " <<
      "relations / " << StringUtils::formatLargeNumber(_map->getRelationCount()) <<
      " total relations.");
  }
}

}
