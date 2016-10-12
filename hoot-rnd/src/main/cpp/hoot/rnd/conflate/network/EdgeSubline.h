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

  EdgeSubline(EdgeLocationPtr start, EdgeLocationPtr end);

  EdgeSubline(ConstNetworkEdgePtr e, double start, double end);

  Meters calculateLength(const ConstElementProviderPtr& provider) const;

  shared_ptr<EdgeSubline> clone() const;

  bool contains(ConstNetworkVertexPtr v) const;

  bool contains(shared_ptr<const EdgeSubline> es) const;

  bool contains(ConstEdgeLocationPtr el) const;

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

  /**
   * Returns true if the sublines share any points.
   */
  bool intersects(shared_ptr<const EdgeSubline> other) const;
  bool intersects(ConstEdgeLocationPtr el) const;

  /**
   * Returns true if the other subline goes in the same dirction as this subline. Only
   * a valid test if sublines belong to the same edge.
   */
  bool isSameDirection(shared_ptr<const EdgeSubline> other) const;

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

  /**
   * Returns a new subline that combines this and other. This and other must touch. If they are
   * going in different directions then the direction of this will be maintained.
   */
  shared_ptr<EdgeSubline> unionSubline(shared_ptr<const EdgeSubline> other) const;

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
