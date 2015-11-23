#ifndef NEEDSREVIEWCRITERION_H
#define NEEDSREVIEWCRITERION_H

#include "ElementCriterion.h"
#include "WayFilter.h"

#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/conflate/ReviewMarker.h>

namespace hoot
{

/**
 * Filters out everything except the specified status.
 */
class NeedsReviewCriterion : public ElementCriterion
{
public:

  NeedsReviewCriterion(ConstOsmMapPtr& map) : _map(map) { }

  virtual bool isSatisfied(const shared_ptr<const Element> &e) const
  {
    bool review = ReviewMarker().isNeedsReview(_map, e);
    return review;
  }

private:
  ConstOsmMapPtr& _map;
};

}

#endif // NEEDSREVIEWCRITERION_H
