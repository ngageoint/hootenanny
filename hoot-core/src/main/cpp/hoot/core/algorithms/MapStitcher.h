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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#ifndef MAP_STITCHER_H
#define MAP_STITCHER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/ClosePointHash.h>
#include <hoot/core/util/Units.h>

// std
#include <map>
#include <unordered_set>

namespace hoot
{

/**
 * @brief
 *
 */
class MapStitcher
{
public:

  static QString className() { return "WayJoiner"; }

  MapStitcher(const OsmMapPtr& base_map);
  virtual ~MapStitcher() = default;

  void stitchMap(const OsmMapPtr& map);

  static void stitchMaps(const OsmMapPtr& base_map, const OsmMapPtr& source_map);

protected:

  Radians _getWayEndpointHeading(const OsmMapPtr& map, const WayPtr& way, long node_id) const;

  long _copyNodeToMap(const OsmMapPtr& dest_map, const NodePtr& node);
  void _copyWayToMap(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const WayPtr& way);
  void _copyRelationToMap(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const RelationPtr& relation);

  void _stitchWay(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const WayPtr& way);

  void _mergeWays(const OsmMapPtr& source_map, const WayPtr& source_way, const OsmMapPtr& dest_map, const WayPtr& dest_way);
  bool _mergeRelations(const OsmMapPtr& source_map, const RelationPtr& source_relation, const OsmMapPtr& dest_map, const RelationPtr& dest_relation);

  void _stitchPoly(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const WayPtr& poly);

  WayPtr _findStitchPointWay(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const WayPtr& way, bool first);

  void _joinWayPairs();
  void _findWayEndpoints(const OsmMapPtr& source_map, const WayPtr& source_way,
                         const OsmMapPtr& dest_map, const WayPtr& dest_way,
                         int& source_way_node_pos, int& dest_way_node_pos) const;

  OsmMapPtr _base_map;
/*
  int _numJoined;
  int _numProcessed;
  int _totalWays;

  int _taskStatusUpdateInterval;
*/
  /** Any changes of node IDs from secondary to base maps */
  std::map<long, long> _updated_node_ids;
  std::map<long, long> _updated_way_ids;
  std::map<long, long> _updated_relation_ids;
  /** Set of node IDs found in the buffer zone */
  std::unordered_set<long> _base_node_ids;
  std::unordered_set<long> _map_node_ids;

  std::vector<std::pair<long, long>> _way_join_pairs;

  double _stitch_buffer_size;
  double _node_buffer_threshold;
  ClosePointHash _cph;

};

}

#endif  //  MapStitcher
