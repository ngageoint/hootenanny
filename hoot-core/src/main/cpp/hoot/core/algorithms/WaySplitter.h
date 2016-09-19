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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef WAYSPLITTER_H
#define WAYSPLITTER_H

// GEOS
#include <geos/geom/Coordinate.h>

// Standard
#include <vector>

// TGS
#include <tgs/SharedPtr.h>

// Hoot
#include <hoot/core/Units.h>
#include <hoot/core/util/FindNodesInWayFactory.h>

#include "linearreference/WayLocation.h"
#include "linearreference/LocationOfPoint.h"

namespace hoot
{
  // Standard
  using namespace std;

  class Way;
  class WaySubline;

class WaySplitter
{
public:

  /**
   * Find the subline in a based on b
   */
  WaySplitter(const OsmMapPtr& map, shared_ptr<Way> a);

  /**
   * Creates a split for each way location (+1) and returns the new ways. The first result in the
   * vector refers to the way before wl[0], the next one is wl[0] -> wl[1], etc.
   *
   * @note The returned vector may contain empty entries if the split size is zero. E.g. two way
   * locations that are in the same location, or a way location at the start/end of the way.
   *
   * The old way is not modified and the new ways are not added to the map.
   *
   * @param wl A sorted list of split points. There may be duplicate split points.
   */
  vector<WayPtr> createSplits(const vector<WayLocation>& wl);

  /**
   * Given an input subline, break the way up into up to 3 pieces where one is the way that covers
   * the subline and the remaining pieces are put into the scraps vector.
   */
  WayPtr createSubline(const WaySubline& subline, vector<WayPtr>& scraps);

  /**
   * Given a split point on a way break the way into two smaller ways and return those ways as
   * an array. The old way will be removed from the source map and the two new ones will be added.
   * No nodes will be removed or replaced, but a new node may be added.
   */
  vector< shared_ptr<Way> > split(WayLocation& splitPoint);

  /**
   * Splits way into smaller ways no bigger than maxSize. If a is smaller than maxSize already
   * then nothing is done. There are no guarantees about the size, but the child ways should be
   * approximately equal.The resulting ways will be placed in a's parent OsmMap.
   * @param w The way to split.
   * @param maxSize the maximum size of the way in map units.
   */
  static void split(const OsmMapPtr& map, const shared_ptr<Way>& w, double maxSize);

  static vector< shared_ptr<Way> > split(const OsmMapPtr& map, shared_ptr<Way> a,
    WayLocation& splitPoint);

private:
  OsmMapPtr _map;
  shared_ptr<Way> _a;
  auto_ptr<FindNodesInWayFactory> _nf;

  shared_ptr<Node> _createNode(const Coordinate& c);
};

}

#endif // WAYSPLITTER_H
