#ifndef ELEMENTSORTER_H
#define ELEMENTSORTER_H

// hoot
#include <hoot/core/OsmMap.h>

#include "ElementInputStream.h"

namespace hoot
{

/**
 * An element stream that returns elements in the order of node, way, then relation
 */
class ElementSorter : public ElementInputStream
{

public:

  ElementSorter(ConstOsmMapPtr map);

  /**
   * @see ElementInputStream
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual ~ElementSorter() {}

  /**
   * @see ElementInputStream
   */
  virtual void close() {}

  /**
   * @see ElementInputStream
   */
  virtual bool hasMoreElements();

  /**
   * @see ElementInputStream
   */
  virtual ElementPtr readNextElement();

private:

  ConstOsmMapPtr _source;

  vector<long> _nodeIds, _wayIds, _relationIds;
  size_t _nodeIndex, _wayIndex, _relationIndex;

};

typedef shared_ptr<ElementSorter> ElementSorterPtr;

}

#endif // ELEMENTSORTER_H
