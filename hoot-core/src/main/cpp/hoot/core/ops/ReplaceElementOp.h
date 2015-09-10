#ifndef REPLACEELEMENTOP_H
#define REPLACEELEMENTOP_H

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
 * Replaces all instances of one element with another element. After the element has been replaced
 * all tags are removed and then an attempt is made to remove the element. No error is thrown
 * if the element isn't removed for some reason.
 *
 * An element may not be removed for instance if the "from" is a node and the "to" is a non-node.
 * If "from" is part of a way you can't replace it with "to". So rather than delete "from" we
 * simply remove its tags.
 */
class ReplaceElementOp : public ConstOsmMapOperation, public ConstElementConsumer
{
public:
  static string className() { return "hoot::ReplaceElementOp"; }

  /**
   * @param from Replace this element.
   * @param to Use this to replace the specified element.
   */
  ReplaceElementOp(ElementId from, ElementId to);

  /**
   * It is expected that the eid will be populated with addElement after construction. addElement
   * must be called exactly two times.
   */
  ReplaceElementOp() {}

  virtual ~ReplaceElementOp() {}

  /**
   * If the elements aren't specified in the constructor this must be called exactly two times. Once
   * for 'from' and a second time for 'to'.
   */
  virtual void addElement(const ConstElementPtr& e);

  virtual void apply(const shared_ptr<OsmMap>& map);

private:
  ElementId _from, _to;
};

}

#endif // REPLACEELEMENTOP_H
