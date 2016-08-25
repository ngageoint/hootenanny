#ifndef EDGESUBLINE_H
#define EDGESUBLINE_H

#include "EdgeLocation.h"

namespace hoot
{

/**
 * This class represents a subline on an edge.
 *
 * This parallels WaySubline.
 */
class EdgeSubline
{
public:
  EdgeSubline(ConstEdgeLocationPtr start, ConstEdgeLocationPtr end);

  EdgeSubline(ConstNetworkEdgePtr e, double start, double end);

  shared_ptr<EdgeSubline> clone() const;

  bool contains(ConstNetworkVertexPtr v) const;

  static shared_ptr<EdgeSubline> createFullSubline(ConstNetworkEdgePtr e);

  const ConstNetworkEdgePtr& getEdge() const { return _start->getEdge(); }

  const ConstEdgeLocationPtr& getEnd() const { return _end; }

  /**
   * Returns the way location that is closer to the beginning of the way regardless of the
   * isBackwards() result.
   */
  ConstEdgeLocationPtr getFormer() const { return isBackwards() ? _end : _start; }

  /**
   * Returns the way location that is closer to the end of the way regardless of the
   * isBackwards() result.
   */
  ConstEdgeLocationPtr getLatter() const { return isBackwards() ? _start : _end; }

  const ConstEdgeLocationPtr& getStart() const { return _start; }

  bool isBackwards() const { return _end < _start; }

  bool isValid() const { return _start->isValid() && _end->isValid(); }

  bool isZeroLength() const { return _start == _end; }

  /**
   * Returns true if other overlaps (a subline in common). If they only touch at a point it is not
   * considered overlapping.
   */
  bool overlaps(shared_ptr<const EdgeSubline> other) const;

  /**
   * swap start and end.
   */
  void reverse() { std::swap(_start, _end); }

  QString toString() const;

private:
  ConstEdgeLocationPtr _start, _end;
};

typedef shared_ptr<EdgeSubline> EdgeSublinePtr;
typedef shared_ptr<const EdgeSubline> ConstEdgeSublinePtr;

bool operator==(ConstEdgeSublinePtr, ConstEdgeSublinePtr);
inline bool operator!=(ConstEdgeSublinePtr es1, ConstEdgeSublinePtr es2)
{
  return !(es1 == es2);
}

// not implemented
bool operator<(ConstEdgeSublinePtr, ConstEdgeSublinePtr);

inline uint qHash(const ConstEdgeSublinePtr& es)
{
//  LOG_VARW(qHash(es->getStart()));
//  LOG_VARW(qHash(es->getEnd()));
  return ::qHash(Tgs::cantorPairing(qHash(es->getStart()), qHash(es->getEnd())));
}

}

#endif // EDGESUBLINE_H
