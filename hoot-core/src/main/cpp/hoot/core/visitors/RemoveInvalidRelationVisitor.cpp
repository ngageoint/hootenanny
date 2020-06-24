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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RemoveInvalidRelationVisitor.h"

//  hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/MetadataTags.h>

//  Standard library
#include <unordered_map>
#include <unordered_set>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveInvalidRelationVisitor)

RemoveInvalidRelationVisitor::RemoveInvalidRelationVisitor() :
_numMembersRemoved(0)
{
}

void RemoveInvalidRelationVisitor::visit(const ElementPtr& e)
{
  //  Ignore everything but relations
  if (e->getElementType() == ElementType::Relation)
  {
    RelationPtr r = dynamic_pointer_cast<Relation>(e);
    assert(r != 0);

    LOG_VART(r->getId());
    //LOG_VART(r);
    //  Only multilinestring or review relations
    if (r->getType() == MetadataTags::RelationReview())
      _removeDuplicates(r);
    else if (r->getType() == MetadataTags::RelationMultilineString())
    {
      _removeDuplicates(r);
      //  Any multilinestring that doesn't have 2 or more linestrings, isn't a multilinestring,
      //  remove it
      vector<RelationData::Entry> members = r->getMembers();
      if (members.size() < 2)
      {
        LOG_TRACE("Removing multilinestring relation with ID: " << r->getId());
        if (members.size() == 1)
        {
          //  Merge the relation tags back on to the single way before deleting the relation
          ElementPtr element = _map->getElement(members[0].getElementId());
          Tags merged =
            TagMergerFactory::getInstance().mergeTags(
              element->getTags(), r->getTags(), ElementType::Relation);
          element->setTags(merged);
        }
        //  Delete the relation
        RemoveRelationByEid::removeRelation(_map->shared_from_this(), r->getId());
        _numAffected++;
      }
    }
  }
}

void RemoveInvalidRelationVisitor::_removeDuplicates(const RelationPtr& r)
{
  //  Make sure that the relation is valid
  if (!r)
    return;
  vector<RelationData::Entry> members = r->getMembers();
  unordered_set<long> total_members;
  unordered_map<long, RelationData::Entry> membersMap;
  //  Iterate all of the members and remove the duplicates
  for (vector<RelationData::Entry>::const_iterator it = members.begin(); it != members.end(); ++it)
  {
    ElementId eid = it->getElementId();
    long id = eid.getId();
    if (total_members.find(id) == total_members.end())
      total_members.insert(id);
    else
    {
      LOG_TRACE("Removing multilinestring relation element with ID: " << eid.getId());
      //  Relation::removeElement removes ALL elements by that ElementId so we need to keep track
      //  of the ones that need to be added back in afterwards
      r->removeElement(eid);
      if (membersMap.find(id) == membersMap.end())
        membersMap[id] = *it;
      _numMembersRemoved++;
    }
  }
  //  Re-insert the members that were duplicates but all instances were deleted
  if (membersMap.size() > 0)
  {
    for (unordered_map<long, RelationData::Entry>::iterator it = membersMap.begin();
         it != membersMap.end(); ++it)
      r->addElement(it->second.getRole(), it->second.getElementId());
    //  Update the number of review members if we removed some of them
    if (r->getTags().contains(MetadataTags::HootReviewMembers()))
      r->setTag(MetadataTags::HootReviewMembers(), QString("%1").arg(r->getMembers().size()));
  }
}

}
