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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

#include "RemoveInvalidReviewRelationsVisitor.h"

//hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/RemoveRelationByEid.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveInvalidReviewRelationsVisitor)

void RemoveInvalidReviewRelationsVisitor::visit(const ElementPtr& e)
{
  if (e->getElementType() == ElementType::Relation)
  {
    Relation* r = dynamic_cast<Relation*>(e.get());
    assert(r != 0);

    LOG_VART(r->getId());
    if (r->getType() == MetadataTags::RelationReview())
    {
      bool invalidRelation = false;
      const bool hasMemberCountTag = r->getTags().contains(MetadataTags::HootReviewMembers());
      if (hasMemberCountTag &&
          (int)r->getMembers().size() != r->getTags().get(MetadataTags::HootReviewMembers()).toInt())
      {
        invalidRelation = true;
      }
      // in case the review member count tag didn't get added for some reason, go ahead and at least
      // remove empty relations
      else if (!hasMemberCountTag && r->getMembers().size() == 0)
      {
        invalidRelation = true;
      }
      else
      {
        // In the cut and replace workflow (ChangesetReplacementCreator), there's a possibility that
        // we could have a relation with all references to members not actually in the data. If all
        // the members don't exist, let's drop the review.
        int nullMemberCount = 0;
        const std::vector<RelationData::Entry> relationMembers = r->getMembers();
        for (size_t i = 0; i < relationMembers.size(); i++)
        {
          ConstElementPtr member = _map->getElement(relationMembers[i].getElementId());
          if (!member)
          {
            nullMemberCount++;
          }
        }
        if (nullMemberCount == (int)relationMembers.size())
        {
          invalidRelation = true;
        }
      }

      if (invalidRelation)
      {
        LOG_TRACE("Removing review relation with ID: " << r->getId());
        RemoveRelationByEid::removeRelation(_map->shared_from_this(), r->getId());
        _numAffected++;
      }
    }
  }
}

}
