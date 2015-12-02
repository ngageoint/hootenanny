#ifndef COUNTUNIQUEREVIEWSVISITOR_H
#define COUNTUNIQUEREVIEWSVISITOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/OsmMapConsumer.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/elements/ElementVisitor.h>

#include "SingleStatistic.h"

namespace hoot
{
using namespace std;

/**
 * Sums the length of all the ways. The map projection is used so to get meters the map must be
 * first reprojected into meters.
 */
class CountUniqueReviewsVisitor : public ElementVisitor, public OsmMapConsumer, public SingleStatistic
{
public:

  static string className() { return "hoot::CountUniqueReviewsVisitors"; }

  CountUniqueReviewsVisitor() {}

  virtual ~CountUniqueReviewsVisitor() {}

  double getStat() const { return _reviews.size(); }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e)
  {
    set<ReviewMarker::ReviewUid> reviews = ReviewMarker().getReviewUids(_map->shared_from_this(),
      e);

    _reviews.insert(reviews.begin(), reviews.end());
  }

private:
  const OsmMap* _map;
  set<ReviewMarker::ReviewUid> _reviews;
};

}
#endif // COUNTUNIQUEREVIEWSVISITOR_H
