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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef WAYSPLITTER_H
#define WAYSPLITTER_H

// GEOS
#include <geos/geom/Coordinate.h>

// Standard
#include <vector>

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

class WaySubline;
class FindNodesInWayFactory;
class WayLocation;

/**
 * Splits ways
 *
 * @todo Not all of the methods add the split parent ID...more may need to.
 */
class WaySplitter
{
public:

  WaySplitter(const OsmMapPtr& map, WayPtr way);

  /**
   * Creates a split for each way location (+1) and returns the new ways.
   *
   * The old way is not modified, and the new ways are not added to the map.
   *
   * @param wl A sorted list of split points. There may be duplicate split points.
   * @return The first result in the vector refers to the way before wl[0], the next one is
   * wl[0] -> wl[1], etc. The returned vector may contain empty entries if the split size is zero.
   * E.g. two way locations that are in the same location, or a way location at the start/end of the
   * way.
   */
  std::vector<WayPtr> createSplits(const std::vector<WayLocation>& wl) const;

  /**
   * Given an input subline, breaks the way up into up to 3 pieces
   *
   * @param subline to split
   * @param populated with scraps from the split
   * @return the way that covers the subline
   */
  WayPtr createSubline(const WaySubline& subline, std::vector<WayPtr>& scraps) const;

  /**
   * Given a split point on a way, breaks the way into two smaller ways
   *
   * The old way will be removed from the source map and the two new ones will be added. No nodes
   * will be removed or replaced, but a new node may be added.
   *
   * @param splitPoint point at which to split the way
   * @return split way parts, which are added to the map
   */
  std::vector<WayPtr> split(WayLocation& splitPoint) const;

  /**
   * Splits way into smaller ways no bigger than maxSize. If a is smaller than maxSize already
   * then nothing is done. There are no guarantees about the size, but the child ways should be
   * approximately equal.The resulting ways will be placed in a's parent OsmMap.
   *
   * @param map map owning the way being split
   * @param w the way to split
   * @param maxSize the maximum size of the way in map units
   */
  static void split(const OsmMapPtr& map, const WayPtr& w, double maxSize);

  /**
   * Splits a way into smaller ways
   *
   * @param map map owning the way
   * @param way the way to split
   * @param splitPoint the point at which to split the way
   * @return the split ways
   */
  static std::vector<WayPtr> split(const OsmMapPtr& map, WayPtr way, WayLocation& splitPoint);

private:

  OsmMapPtr _map;
  WayPtr _way;
  std::shared_ptr<FindNodesInWayFactory> _nf;

  NodePtr _createNode(const geos::geom::Coordinate& c);
};

}

#endif // WAYSPLITTER_H
