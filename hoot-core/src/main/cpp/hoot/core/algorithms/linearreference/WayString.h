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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
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

  WayLocation calculateLocationFromStart(Meters distance,
    ElementId preferredEid = ElementId()) const;

  /**
   * Simplifies this WayString from a complex collection of sublines into a single simple way. The
   * new way and all the associated nodes are put into destination.
   *
   * This is primarily useful when performing experiments (e.g. does this WayString match that
   * WayString).
   *
   * Some information may be lost as the new way is created (e.g. conflicting tags within child
   * ways).
   */
  WayPtr copySimplifiedWayIntoMap(const ElementProvider& map, OsmMapPtr destination);

  Meters getMaxCircularError() const;

  /** returns number of sublines in this string */
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
  static Meters _epsilon;

  Meters _aggregateCircularError() const;

  WayLocation _changeToPreferred(int index, const WayLocation& wl, ElementId preferredEid) const;
};

typedef boost::shared_ptr<WayString> WayStringPtr;
typedef boost::shared_ptr<const WayString> ConstWayStringPtr;

}

#endif // WAYSTRING_H
