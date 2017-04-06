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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef SMALLWAYMERGER_H
#define SMALLWAYMERGER_H

// TGS
#include <tgs/SharedPtr.h>

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/ops/OsmMapOperation.h>

// Std
#include <set>

namespace hoot
{
  class OsmMap;
  class Way;
  class TagDifferencer;
  class NodeToWayMap;

  using namespace std;

/**
 * Searches for itty bitty ways that connect end to end. For some reason some files provide little
 * way segments of 2m or less in length. Silliness. If:
 *   - the attributes are essentially identical
 *   - there are only two ways meeting (not an intersection)
 *   - and one is less than the specified threshold
 *   - OsmSchema::isHighway returns true
 * The ways will be merged.
 *
 * Assumptions:
 *  - All intersections have been split. (IntersectionSplitter)
 *  - All duplicate ways have been removed. (DuplicateWayRemover)
 */
class SmallWayMerger : public OsmMapOperation
{
public:
  static string className() { return "hoot::SmallWayMerger"; }

  SmallWayMerger(Meters threshold = -1);

  void apply(boost::shared_ptr<OsmMap>& map);

  /**
   * Remove parts of ways that are duplicates.
   */
  static void mergeWays(boost::shared_ptr<OsmMap> map, Meters threshold);

protected:

  boost::shared_ptr<OsmMap> _map;

  double _threshold;
  NodeToWayMap* _n2w;
  auto_ptr<TagDifferencer> _diff;

  void _mergeNeighbors(boost::shared_ptr<Way> w);

  void _mergeWays(const set<long>& ids);
};

}

#endif // SMALLWAYMERGER_H
