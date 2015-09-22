#ifndef HIGHWAYREVIEWCLEANEROP_H
#define HIGHWAYREVIEWCLEANEROP_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * This class will go through all highway reviews and look for reviews that are impossible due to
 * a lack of string match. If the review is impossible it will be deleted. This is handy due to the
 * way multilinestrings are generated during the conflation process.
 */
class HighwayReviewCleanerOp : public OsmMapOperation
{
public:

  static string className() { return "hoot::HighwayReviewCleanerOp"; }

  HighwayReviewCleanerOp();

  virtual void apply(shared_ptr<OsmMap>& map);

  virtual QString toString() const { return QString::fromStdString(className()); }

private:

  bool _isBadHighwayReview(shared_ptr<OsmMap>& map, hoot::ReviewMarker::ReviewUid review);
};

}

#endif // HIGHWAYREVIEWCLEANEROP_H
