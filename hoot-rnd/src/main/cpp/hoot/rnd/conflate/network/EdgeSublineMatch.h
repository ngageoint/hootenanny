#ifndef EDGESUBLINEMATCH_H
#define EDGESUBLINEMATCH_H

#include "EdgeSubline.h"

namespace hoot
{

/**
 * This class represents a matching subline between two edges.
 *
 * This class parallels WaySublineMatch.
 */
class EdgeSublineMatch
{
public:
  EdgeSublineMatch(ConstEdgeSublinePtr es1, ConstEdgeSublinePtr es2) : _es1(es1), _es2(es2) {}

  const ConstEdgeSublinePtr& getSubline1() const { return _es1; }

  const ConstEdgeSublinePtr& getSubline2() const { return _es2; }

  QString toString() const;

private:
  ConstEdgeSublinePtr _es1, _es2;

};

typedef shared_ptr<EdgeSublineMatch> EdgeSublineMatchPtr;
typedef shared_ptr<const EdgeSublineMatch> ConstEdgeSublineMatchPtr;

// not implemented
bool operator<(ConstEdgeSublineMatchPtr, ConstEdgeSublineMatchPtr);

}

#endif // EDGESUBLINEMATCH_H
