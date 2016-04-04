#ifndef ELEMENTSORTER_H
#define ELEMENTSORTER_H

// hoot
#include <hoot/core/OsmMap.h>

#include "ElementInputStream.h"

namespace hoot
{

class ElementSorter : public ElementInputStream
{
public:
  ElementSorter(ConstOsmMapPtr map);

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual ~ElementSorter() {}

  virtual void close() {}

  virtual bool hasMoreElements();

  virtual ElementPtr readNextElement();

private:
  ConstOsmMapPtr _source;

  vector<long> _nodeIds, _wayIds, _relationIds;
  size_t _nodeIndex, _wayIndex, _relationIndex;
};

typedef shared_ptr<ElementSorter> ElementSorterPtr;

}

#endif // ELEMENTSORTER_H
