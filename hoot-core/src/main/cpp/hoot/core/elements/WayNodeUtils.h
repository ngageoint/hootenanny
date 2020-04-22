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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef WAY_NODE_UTILS_H
#define WAY_NODE_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Utilities for use with way nodes
 */
class WayNodeUtils
{

public:

  /**
   * Get a detailed string respresenting a way's nodes
   *
   * @param way way to get info from
   * @param map map owning the way
   * @return a detailed way nodes string
   */
  static QString getWayNodesDetailedString(const ConstWayPtr& way, const ConstOsmMapPtr& map);

  /**
   * Returns the IDs of all ways containing an input node
   *
   * @param nodeId ID of the node to return containing ways for
   * @param map map which owns the input node
   * @param wayCriterion an optional ElementCriterion to further filter the containing ways
   * @return a collection of way IDs
   */
  static std::set<long> getContainingWayIdsByNodeId(
    const long nodeId, const ConstOsmMapPtr& map,
    const ElementCriterionPtr& wayCriterion = ElementCriterionPtr());

  /**
   * Determines the coordinate on a way closest to another node not on the way
   *
   * @param node the node to find the closet way coordinate to
   * @param way the way to find the closest coordinate on
   * @param distance the distance that will be calculated between the node and the closest
   * coordinate on the way
   * @param discretizationSpacing the distance at which the way will be discretized; a smaller
   * value results in a more accurate distance value, generates more coordinates on the way, and
   * increase runtime
   * @param map the map containing the input node and way
   * @return a coordinate
   */
  static geos::geom::Coordinate closestWayCoordToNode(
    const ConstNodePtr& node, const ConstWayPtr& way, double& distance,
    const double discretizationSpacing, const ConstOsmMapPtr& map);

  /**
   * Determines the ID of the closest way node on a way to another node not on the way
   *
   * @param node the node to find the closest way node to
   * @param way the way to find the closest way node on
   * @param map the map containing the input node and way
   * @return a way node ID
   */
  static long closestWayNodeIdToNode(const ConstNodePtr& node, const ConstWayPtr& way,
                                     const ConstOsmMapPtr& map);

  /**
   * Finds the closest way node index in a way to a specified node
   *
   * @param node node used for location comparison
   * @param way way to search for closest index
   * @param map map owning the node and way
   * @return closest index on the way to node or -1 if no index is found
   */
  static long closestWayNodeIndexToNode(
    const ConstNodePtr& node, const ConstWayPtr& way, const ConstOsmMapPtr& map);

  /**
   * Finds the closest way node insert index in a way to a specified node. The index can be used
   * directly in a call to Way::insertNode to insert a new node between two others
   *
   * @param node node used for location comparison
   * @param way way to search for closest index
   * @param map map owning the node and way
   * @return closest insert index on the way to node or -1 if no index is found
   */
  static long closestWayNodeInsertIndex(const ConstNodePtr& node, const ConstWayPtr& way,
                                        const ConstOsmMapPtr& map);

  /**
   * Determines whether the start or end of a way is closer to a specified node
   *
   * @param node node to check distance from input way
   * @param way way to check distance from input node
   * @param map map containing the inputs way and node
   * @return true if the node is closer to the end of the way; false otherwise
   */
  static bool endWayNodeIsCloserToNodeThanStart(const ConstNodePtr& node, const ConstWayPtr& way,
                                                const ConstOsmMapPtr& map);

  /**
   * Determines if two nodes belong to the same way
   *
   * @param nodeId1 the first node to examine
   * @param nodeId2 the second node to examine
   * @param map the map containing the nodes
   * @return true if there is at least one way that contains both nodes; false otherwise
   */
  static bool nodesAreContainedInTheSameWay(const long nodeId1, const long nodeId2,
                                            const ConstOsmMapPtr& map);

  /**
   * Determines if a specified node is contained by a way, given a list of way IDs
   *
   * @param nodeId the ID of the node to search for
   * @param wayIds the IDs for the ways to search in
   * @param map the map containing the nodes/ways
   * @return true if any way in the ID list contains the node; false otherwise
   */
  static bool nodeContainedByAnyWay(const long nodeId, const std::set<long> wayIds,
                                    const ConstOsmMapPtr& map);

  /**
   * Determines if a node is contained by any way in a map
   *
   * @param nodeId the ID of the node to search for
   * @param map the map containing the nodes/ways
   * @return true if any way in the map contains the node; false otherwise
   */
  static bool nodeContainedByAnyWay(const long nodeId, const ConstOsmMapPtr& map);

  /**
   * Determines if a node is contained by more than one way in a map
   *
   * @param nodeId the ID of the node to search for
   * @param map the map containing the nodes/ways
   * @return true if more than one way contains the node; false otherwise
   */
  static bool nodeContainedByMoreThanOneWay(const long nodeId, const ConstOsmMapPtr& map);
};

}

#endif // WAY_NODE_UTILS_H
