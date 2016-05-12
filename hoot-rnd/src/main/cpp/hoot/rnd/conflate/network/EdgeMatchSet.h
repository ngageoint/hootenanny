#ifndef __EDGE_MATCH_SET_H__
#define __EDGE_MATCH_SET_H__

#include "EdgeMatch.h"

namespace hoot
{

class EdgeMatchSet
{
public:
  virtual ~EdgeMatchSet() {}

  virtual bool contains(const EdgeMatchPtr &em) const = 0;

  virtual QString toString() const = 0;

};

typedef shared_ptr<EdgeMatchSet> EdgeMatchSetPtr;
typedef shared_ptr<const EdgeMatchSet> ConstEdgeMatchSetPtr;

}

#endif // __EDGE_MATCH_SET_H__
