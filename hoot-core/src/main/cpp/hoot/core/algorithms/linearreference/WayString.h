#ifndef WAYSTRING_H
#define WAYSTRING_H

// hoot
#include <hoot/core/algorithms/linearreference/WaySubline.h>

// Qt
#include <QList>

namespace hoot
{

/**
 * Represents an ordered collection of WaySublines. Each subline must match end to start and all
 * ways must be connected topogically (nodes, not just overlap).
 *
 * NOTE: Do you want to relax these simple rules? Make a new class.
 */
class WayString
{
public:
  WayString();

  void append(const WaySubline& subline);

  WaySubline& at(int i) { return _sublines[i]; }

  const WaySubline& back() const { return _sublines.back(); }

  Meters calculateDistanceOnString(const WayLocation &l) const;

  Meters calculateLength() const;

  WayLocation calculateLocationFromStart(Meters distance) const;

  int getSize() { return _sublines.size(); }

  QString toString() const { return hoot::toString(_sublines); }

  /**
   * Similar to the visitRw method in Element, this will visit all elements in the way string along
   * with all the children that make up the WayString.
   *
   * Only nodes that are part of the way string will be visited (e.g. if they intersect the
   * WaySubline).
   */
  void visitRo(const ElementProvider& map, ElementVisitor& v) const;

private:
  QList<WaySubline> _sublines;
};

typedef boost::shared_ptr<WayString> WayStringPtr;
typedef boost::shared_ptr<const WayString> ConstWayStringPtr;

}

#endif // WAYSTRING_H
