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
 * @brief MapStitcher class stitches two cropped datasets together along the boundary between the two.
 * The two datasets must have similar shapes along the boundary in order to work optimally, for example
 * +-----+   +-----+   +-----------+
 * |     | + |     | = |           |
 * +-----+   +-----+   +-----------+
 *
 * and
 *
 * +-----+
 * |     |   +-----+
 * +-----+   |     |
 *    +    = |     |
 * +-----+   |     |
 * |     |   +-----+
 * +-----+
 *
 * but this is undefined
 *
 * +-----------+
 * |           |
 * |     +-----+   +-----+
 * |     |       + |     |
 * +-----+         +-----+
 *
 * Currently the rectangular bounds of each map are used to determine the buffer
 */
class MapStitcher
{
public:

  static QString className() { return "WayJoiner"; }

  /**
   * @brief MapStitcher constructor
   * @param base_map Base map will contain all map data after the stitch operation
   */
  MapStitcher(const OsmMapPtr& base_map);
  virtual ~MapStitcher() = default;

  /**
   * @brief stitchMap Stitch two cropped map datasets together, joining/merging elements along
   * the adjacent boundaries
   * @param map Map to pull into base map from constructor
   */
  void stitchMap(const OsmMapPtr& map);

  /**
   * @brief stitchMaps Stitch two cropped map datasets together, joining/merging elements along
   * the adjacent boundaries
   * @param base_map Base map will contain all map data after the stitch operation
   * @param source_map Map to pull into base map
   */
  static void stitchMaps(const OsmMapPtr& base_map, const OsmMapPtr& source_map);

protected:

  /**
   * @brief _getWayEndpointHeading Get the heading of the way from the endpoint specified
   * @param map Map containing the way
   * @param way Way used to calculate the heading
   * @param node_id Node ID in the way to calculate the heading from
   * @param front_to_back True to calculate heading towards node_id endpoint
   *                      False to calculate heading away from node_id endpoint
   * @return The heading in radians
   */
  Radians _getWayEndpointHeading(const OsmMapPtr& map, const WayPtr& way, long node_id, bool towards_node_id) const;

  /**
   * @brief _copyNode/Way/RelationToMap Copy element from source map to destination without merging or joining
   * @param source_map Source map that contains the element to be copied
   * @param dest_map Base map that gets the element copied in
   * @param node/way/relation The element to be copied
   * @return Node ID of the new node
   */
  long _copyNodeToMap(const OsmMapPtr& dest_map, const NodePtr& node);
  void _copyWayToMap(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const WayPtr& way);
  void _copyRelationToMap(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const RelationPtr& relation);

  /**
   * @brief _stitchWay Stitch a single way from the source map with corresponding way in destination
   * @param source_map Source map that contains the element to be stitched
   * @param dest_map Destination map containing ways
   * @param way Way to stitch into a destination way
   */
  void _stitchWay(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const WayPtr& way);

  /**
   * @brief _stitchPoly Stitch a single polygon way from the source map with corresponding polygon way
   * @param source_map Source map that contains the polygon to be stitched
   * @param dest_map Destination map containing ways
   * @param poly Polygon way to stitch into a destination polygon
   */
  void _stitchPoly(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const WayPtr& poly);

  /**
   * @brief _mergeWays Does the actual merging of two ways
   * @param source_map Source map that contains the way to be merged
   * @param source_way Source way to merge into destination
   * @param dest_map Base map that gets the way to be merged
   * @param dest_way Destination way to receive the merge
   */
  void _mergeWays(const OsmMapPtr& source_map, const WayPtr& source_way, const OsmMapPtr& dest_map, const WayPtr& dest_way);

  /**
   * @brief _mergeRelations Does the actual merging of two relations, only merges relations that have the same positive ID
   * @param source_relation Relation from source map to be merged in
   * @param dest_relation Relation fromm destination map to receive the source relation's members
   * @return True if a merge happened
   */
  bool _mergeRelations(const RelationPtr& source_relation, const RelationPtr& dest_relation);

  /**
   * @brief _findStitchPointWay Find a way from the destination map that will stitch with 'way' at selected endpoint
   * @param source_map Source map that contains way
   * @param dest_map Destination map containing ways that match the endpoint of 'way'
   * @param way Way from source map used in matching destination ways
   * @param first True for matching `way->getFirstNodeId()`, false for matching `way->getLastNodeId()`
   * @return Pointer to the corresponding stitch way
   */
  WayPtr _findStitchPointWay(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const WayPtr& way, bool first);
  WayPtr _findStitchPointWay(const OsmMapPtr& source_map, const OsmMapPtr& dest_map, const WayPtr& way, const NodePtr& node);

  /**
   * @brief _joinWayPairs Join two ways in the base map, fixes the over and back issue
   */
  void _joinWayPairs();

  /**
   * @brief _findWayEndpoints Get the position (index) of the node endpoints of each of the two ways that match,
   * must be a matching set
   * @param source_map Source map
   * @param source_way Source way
   * @param dest_map Destination map
   * @param dest_way Destination way
   * @param source_way_node_pos Position index of matching endpoint for source way (0 or size() - 1)
   * @param dest_way_node_pos Position index of matching endpoint for destination way (0 or size() - 1)
   */
  void _findWayEndpoints(const OsmMapPtr& source_map, const WayPtr& source_way,
                         const OsmMapPtr& dest_map, const WayPtr& dest_way,
                         int& source_way_node_pos, int& dest_way_node_pos) const;

  /** Base map that will receive all elements during the stitching operation */
  OsmMapPtr _base_map;
  /** Any changes of node IDs from secondary to base maps */
  std::map<long, long> _updated_node_ids;
  std::map<long, long> _updated_way_ids;
  std::map<long, long> _updated_relation_ids;
  /** Set of node IDs found in the buffer zone */
  std::unordered_set<long> _base_node_ids;
  std::unordered_set<long> _map_node_ids;
  /** Base map way pairs that need to be joined and not merged */
  std::vector<std::pair<long, long>> _way_join_pairs;
  /** Size of buffer in meters on either side of the shared boundary */
  Meters _stitch_buffer_size;
  /** Threshold distance between two points within the buffer that are considered matching */
  Meters _node_buffer_threshold;
  /** Used for detecting matching endpoints between the two maps */
  ClosePointHash _cph;
};

}

#endif  //  MapStitcher
