#include "HighwayReviewCleanerOp.h"

#include <hoot/core/conflate/ReviewMarker.h>

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
        map->removeRelation(r.getId());
      }
    }
  }
}

bool HighwayReviewCleanerOp::_isBadHighwayReview(shared_ptr<OsmMap>& map, ElementId review)
{
  #warning implement me
  return false;
}

}
