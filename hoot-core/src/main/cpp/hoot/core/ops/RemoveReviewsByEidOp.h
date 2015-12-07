#ifndef REMOVEREVIEWSBYEIDOP_H
#define REMOVEREVIEWSBYEIDOP_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ConstElementConsumer.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/filters/BaseFilter.h>

// Standard
#include <set>

#include "ConstOsmMapOperation.h"

namespace hoot
{
using namespace std;

/**
 *
 */
class RemoveReviewsByEidOp : public ConstOsmMapOperation, public ConstElementConsumer
{
public:
  static string className() { return "hoot::RemoveReviewsByEidOp"; }

  /**
   */
  RemoveReviewsByEidOp(ElementId eid, bool clearAndRemoveElement = false);

  /**
   * It is expected that the eid will be populated with addElement after construction. addElement
   * must be called exactly two times.
   */
  RemoveReviewsByEidOp() {}

  virtual ~RemoveReviewsByEidOp() {}

  /**
   * If the elements aren't specified in the constructor this must be called exactly two times. Once
   * for 'from' and a second time for 'to'.
   */
  virtual void addElement(const ConstElementPtr& e);

  virtual void apply(const shared_ptr<OsmMap>& map);

private:
  ElementId _eid;
  bool _clearAndRemove;
};

}

#endif // REMOVEREVIEWSBYEIDOP_H
