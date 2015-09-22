#include "HighwayReviewCleanerOp.h"

#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/MatchFactory.h>

namespace hoot
{

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
      if (_isBadHighwayReview(map, r) == true)
      {
        // this relation shouldn't be part of any other relations so removing is safe. If by chance
        // it is in another relation it will simply be removed from that relation before being
        // removed from the map.
        map->removeRelation(r.getId());
      }
    }
  }
}

bool HighwayReviewCleanerOp::_isBadHighwayReview(shared_ptr<OsmMap>& map,
  ReviewMarker::ReviewUid review)
{
  bool result = false;

  if (ReviewMarker::getReviewType(map, review) == HighwayMatch::getHighwayMatchName())
  {
    set<ElementId> elements = ReviewMarker::getReviewElements(map, review);

    if (elements.size() == 2)
    {
      ElementId eid1 = *elements.begin();
      ElementId eid2 = *elements.rbegin();

      // By using the match factory instead of explicity calling the highway match we're more
      // robust to changes in the factory configuration that may occur at runtime.
      if (MatchFactory::getInstance().createMatch(map, eid1, eid2))
      {
        result = true;
      }
    }
  }

  return result;
}

}
