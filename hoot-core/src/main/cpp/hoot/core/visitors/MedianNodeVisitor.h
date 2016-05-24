#ifndef MEDIANNODEVISITOR_H
#define MEDIANNODEVISITOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/ConstOsmMapConsumer.h>

namespace hoot
{

/**
 * Calculates the median node. This is very inefficient and shouldn't be used on large maps. E.g.
 * O(n^2).
 */
class MedianNodeVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:
  MedianNodeVisitor();

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e);

  ConstNodePtr calculateMedianNode() const;

private:
  const OsmMap* _map;
  QList<ConstNodePtr> _nodes;

  double _calculateRmsd(ConstNodePtr n) const;
};

}

#endif // MEDIANNODEVISITOR_H
