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
 * Replaces all instances of one element with another element. In some cases we may not be able
 * to replace all instances of the "from" element. E.g. if "from" is a node and "to" is a way. If
 * "from" is part of a way, then we can't do the replacement. In this case it won't be replaced and
 * will still be part of the parent way.
 *
 * In many cases you may want to follow this call by clearing tags and then attempting to remove
 * "from". E.g.
 *
 *    ReplaceElementOp(from, to).apply(map);
 *    from->getTags().clear();
 *    RecursiveElementRemover(from->getElementId()).apply(map);
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
