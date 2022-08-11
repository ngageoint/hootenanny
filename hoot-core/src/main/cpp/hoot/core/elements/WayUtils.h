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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef WAY_UTILS_H
#define WAY_UTILS_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Utilities for use with ways
 */
class WayUtils
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
   * Returns the IDs of ways connected to the way with the specified ID
   *
   * @param wayId ID of the way to search connected ways for
   * @param map map owning the way
   * @return a collection of way IDs
   */
  static QSet<long> getConnectedWays(const long wayId, const ConstOsmMapPtr& map);

  /**
   * Returns the number of ways connected to the way with the specified ID
   *
   * @param wayId ID of the way to search connected ways for
   * @param map map owning the way
   * @return a way count
   */
  static int getNumberOfConnectedWays(const long wayId, const ConstOsmMapPtr& map);

  /**
   * Determines if the way with specified way ID is connected to any other ways
   *
   * @param wayId ID of the way to search connected ways for
   * @param map map owning the way
   * @return a way count
   */
  static bool hasConnectedWays(const long wayId, const ConstOsmMapPtr& map);

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
   * @return a valid coordinate or an empty coordinate if the closest coordinate could not be
   * calculated
   */
  static geos::geom::Coordinate closestWayCoordToNode(const ConstNodePtr& node, const ConstWayPtr& way, double& distance,
                                                      const double discretizationSpacing, const ConstOsmMapPtr& map);

  /**
   * Determines the ID of the closest way node on a way to another node not on the way
   *
   * @param node the node to find the closest way node to
   * @param way the way to find the closest way node on
   * @param map the map containing the input node and way
   * @return a way node ID
   */
  static long closestWayNodeIdToNode(const ConstNodePtr& node, const ConstWayPtr& way, const ConstOsmMapPtr& map);

  /**
   * Finds the closest way node index in a way to a specified node
   *
   * @param node node used for location comparison
   * @param way way to search for closest index
   * @param map map owning the node and way
   * @return closest index on the way to node or -1 if no index is found
   */
  static long closestWayNodeIndexToNode(const ConstNodePtr& node, const ConstWayPtr& way, const ConstOsmMapPtr& map);

  /**
   * Finds the closest way node insert index in a way to a specified node. The index can be used
   * directly in a call to Way::insertNode to insert a new node between two others
   *
   * @param node node used for location comparison
   * @param way way to search for closest index
   * @param map map owning the node and way
   * @return closest insert index on the way to node or -1 if no index is found
   */
  static long closestWayNodeInsertIndex(const ConstNodePtr& node, const ConstWayPtr& way, const ConstOsmMapPtr& map);

  /**
   * Determines whether the start or end of a way is closer to a specified node
   *
   * @param node node to check distance from input way
   * @param way way to check distance from input node
   * @param map map containing the inputs way and node
   * @return true if the node is closer to the end of the way; false otherwise
   */
  static bool endWayNodeIsCloserToNodeThanStart(const ConstNodePtr& node, const ConstWayPtr& way, const ConstOsmMapPtr& map);

  /**
   * Determines if two nodes belong to the same way
   *
   * @param nodeId1 the first node to examine
   * @param nodeId2 the second node to examine
   * @param map the map containing the nodes
   * @return true if there is at least one way that contains both nodes; false otherwise
   */
  static bool nodesAreContainedInTheSameWay(const long nodeId1, const long nodeId2, const ConstOsmMapPtr& map);

  /**
   * Returns the IDs of all ways containing an input node
   *
   * @param nodeId ID of the node to return containing ways for
   * @param map map which owns the input node
   * @param wayCriterion an optional ElementCriterion to further filter the containing ways
   * @return a collection of way IDs
   */
  static std::set<long> getContainingWayIds(const long nodeId, const ConstOsmMapPtr& map,
                                            const ElementCriterionPtr& wayCriterion = ElementCriterionPtr());

  /**
   * Returns all ways containing an input node
   *
   * @param nodeId ID of the node to return containing ways for
   * @param map map which owns the input node
   * @param wayCriterion an optional ElementCriterion to further filter the containing ways
   * @return a collection of const ways
   */
  static std::vector<ConstWayPtr> getContainingWaysConst(const long nodeId, const ConstOsmMapPtr& map,
                                                         const ElementCriterionPtr& wayCriterion = ElementCriterionPtr());

  /**
   * Returns all ways containing an input node
   *
   * @param nodeId ID of the node to return containing ways for
   * @param map map which owns the input node
   * @param wayCriterion an optional ElementCriterion to further filter the containing ways
   * @return a collection of ways
   */
  static std::vector<WayPtr> getContainingWays(const long nodeId, const ConstOsmMapPtr& map,
                                               const ElementCriterionPtr& wayCriterion = ElementCriterionPtr());

  /**
   * Returns the most specific feature types, as identified by the schema, for all ways containing
   * the specified node
   *
   * @param nodeId ID of the node to return containing way types for
   * @param map map which owns the input node
   * @return a unique collection of type key/value pair strings
   */
  static std::set<QString> getContainingWaysMostSpecificTypes(const long nodeId, const ConstOsmMapPtr& map);

  /**
   * Determines if a specified node is contained by a way, given a list of way IDs
   *
   * @param nodeId the ID of the node to search for
   * @param wayIds the IDs for the ways to search in
   * @param map the map containing the nodes/ways
   * @return true if any way in the ID list contains the node; false otherwise
   */
  static bool nodeContainedByAnyWay(const long nodeId, const std::set<long>& wayIds, const ConstOsmMapPtr& map);

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

  /**
   * Determines the ways intersecting an input way
   *
   * @param wayId the ID of the way to find intersecting ways for
   * @param map the map containing the input and potentially intersecting ways
   * @return a collection of ways
   */
  static std::vector<WayPtr> getIntersectingWays(const long wayId, const OsmMapPtr& map);

  /**
   * Determines the IDs of ways intersecting an input way
   *
   * @param wayId the ID of the way to find intersecting ways for
   * @param map the map containing the input and potentially intersecting ways
   * @return a collection of IDs
   */
  static std::vector<long> getIntersectingWayIds(const long wayId, const OsmMapPtr& map);

  /**
   * Determines the IDs of ways intersecting an input way
   *
   * @param wayId the ID of the way to find intersecting ways for
   * @param map the const map containing the input and potentially intersecting ways
   * @return a collection of IDs
   */
  static std::vector<long> getIntersectingWayIdsConst(const long wayId, const ConstOsmMapPtr& map);

  /**
   * This determines if a node belongs to a way that shares any nodes with another way.
   *
   * @param nodeId ID of the node to examine
   * @param wayId ID of the way to examine
   * @param map map owning the input elements
   * @return true if the node with ID nodeId belongs to a way that has any way nodes in common with
   * the way having ID wayId; false otherwise
   */
  static bool nodeContainedByWaySharingNodesWithAnotherWay(const long nodeId, const long wayId, const OsmMapPtr& map);
};

}

#endif // WAY_UTILS_H
