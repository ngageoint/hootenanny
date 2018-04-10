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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RemoveInvalidMultilineStringVisitor.h"

//  hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/ops/RemoveRelationOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MetadataTags.h>

//  Standard library
#include <unordered_set>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, RemoveInvalidMultilineStringVisitor)

RemoveInvalidMultilineStringVisitor::RemoveInvalidMultilineStringVisitor()
{
}

void RemoveInvalidMultilineStringVisitor::visit(const ElementPtr& e)
{
  //  Ignore everything but relations
  if (e->getElementType() == ElementType::Relation)
  {
    Relation* r = dynamic_cast<Relation*>(e.get());
    assert(r != 0);

    LOG_VART(r->getId());
    //  Only multilinestring relations
    if (r->getType() == MetadataTags::RelationMultilineString())
    {
      vector<RelationData::Entry> members = r->getMembers();
      unordered_set<long> total_members;
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
          r->removeElement(eid);
        }
      }
      //  Get a new copy of the relation members
      members = r->getMembers();
      //  Any multilinestring that doesn't have 2 or more linestrings, isn't a multilinestring, remove it
      if (members.size() < 2)
      {
        LOG_TRACE("Removing multilinestring relation with ID: " << r->getId());
        if (members.size() == 1)
        {
          //  Merge the relation tags back on to the single way before deleting the relation
          ElementPtr element = _map->getElement(members[0].getElementId());
          Tags merged = TagMergerFactory::getInstance().mergeTags(element->getTags(), r->getTags(), ElementType::Relation);
          element->setTags(merged);
        }
        //  Delete the relation
        RemoveRelationOp::removeRelation(_map->shared_from_this(), r->getId());
      }
    }
  }
}

}
