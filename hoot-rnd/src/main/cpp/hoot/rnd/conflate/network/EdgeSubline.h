/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
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

 boost::shared_ptr<EdgeSubline> clone() const;

  bool contains(ConstNetworkVertexPtr v) const;

  bool contains(boost::shared_ptr<const EdgeSubline> es) const;

  bool contains(ConstEdgeLocationPtr el) const;

  static boost::shared_ptr<EdgeSubline> createFullSubline(ConstNetworkEdgePtr e);

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
  bool intersects(boost::shared_ptr<const EdgeSubline> other) const;
  bool intersects(ConstEdgeLocationPtr el) const;

  /**
   * Returns true if the other subline goes in the same dirction as this subline. Only
   * a valid test if sublines belong to the same edge.
   */
  bool isSameDirection(boost::shared_ptr<const EdgeSubline> other) const;

  bool isBackwards() const { return _end < _start; }

  bool isValid() const { return _start->isValid() && _end->isValid(); }

  bool isZeroLength() const { return _start == _end; }

  /**
   * Returns true if other overlaps (a subline in common). If they only touch at a point it is not
   * considered overlapping.
   */
  bool overlaps(boost::shared_ptr<const EdgeSubline> other) const;

  /**
   * swap start and end.
   */
  void reverse() { std::swap(_start, _end); }

  QString toString() const;

  /**
   * Returns a new subline that combines this and other. This and other must touch. If they are
   * going in different directions then the direction of this will be maintained.
   */
 boost::shared_ptr<EdgeSubline> unionSubline(boost::shared_ptr<const EdgeSubline> other) const;

private:
  ConstEdgeLocationPtr _start, _end;
};

typedef boost::shared_ptr<EdgeSubline> EdgeSublinePtr;
typedef boost::shared_ptr<const EdgeSubline> ConstEdgeSublinePtr;

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
