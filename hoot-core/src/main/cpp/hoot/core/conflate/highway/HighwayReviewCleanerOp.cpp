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
#include "HighwayReviewCleanerOp.h"

#include <hoot/core/Factory.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/ops/RemoveRelationOp.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, HighwayReviewCleanerOp)

HighwayReviewCleanerOp::HighwayReviewCleanerOp()
{
}

void HighwayReviewCleanerOp::apply(shared_ptr<OsmMap>& map)
{
  RelationMap relations = map->getRelationMap();

  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ElementId r = ElementId::relation(it->first);
    if (ReviewMarker().isReviewUid(map, r))
    {
      LOG_INFO("Looking at review");
      if (_isBadHighwayReview(map, r) == true)
      {
        LOG_INFO("Found bad review");
        // this relation shouldn't be part of any other relations so removing is safe. If by chance
        // it is in another relation it will simply be removed from that relation before being
        // removed from the map.
        RemoveRelationOp::removeRelation(map, r.getId());
      }
    }
  }
}

bool HighwayReviewCleanerOp::_isBadHighwayReview(shared_ptr<OsmMap>& map,
  ReviewMarker::ReviewUid review)
{
  if (ReviewMarker::getReviewType(map, review) == HighwayMatch::getHighwayMatchName())
  {
    LOG_INFO("Found highway review");
    set<ElementId> elements = ReviewMarker::getReviewElements(map, review);

    if (elements.size() == 2)
    {
      ElementId eid1 = *elements.begin();
      ElementId eid2 = *elements.rbegin();

      // By using the match factory instead of explicity calling the highway match we're more
      // robust to changes in the factory configuration that may occur at runtime.
      auto_ptr<Match> m(MatchFactory::getInstance().createMatch(map, eid1, eid2));
      // if we failed to find a match of any kind
      if (!m.get())
      {
        LOG_INFO("Found bad review");
        // this is a bad highway review
        return true;
      }
    }
  }

  return false;
}

}
