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

#ifndef NODE_UTILS_H
#define NODE_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Various utilities useful when working with nodes but kept out of the Node class to avoid
 * circular dependencies
 */
class NodeUtils
{

public:

  /**
    Retrieves a collection of node IDs for a collection of nodes

    @param nodes a collection of nodes
    @return a collection of node IDs
    */
  static QList<long> nodesToNodeIds(const QList<std::shared_ptr<const Node>>& nodes);

  /**
   * Retrieves a collection of node IDs for a collection of nodes
   *
   * @param nodes a collection of nodes
   * @return a collection of node IDs
   */
  static std::vector<long> nodesToNodeIds(const std::vector<std::shared_ptr<const Node>>& nodes);

  /**
    Retrieves a collection of nodes given a collection of node IDs

    @param nodeIds a collection of node IDs
    @param map the map owning the nodes with the given IDs
    @return a collection of nodes
    */
  static QList<std::shared_ptr<const Node>> nodeIdsToNodes(const QList<long>& nodeIds,
                                                           const std::shared_ptr<const OsmMap>& map);

  /**
   * Retrieves a collection of nodes given a collection of node IDs
   *
   * @param nodeIds a collection of node IDs
   * @param map the map owning the nodes with the given IDs
   * @return a collection of nodes
   */
  static std::vector<std::shared_ptr<const Node>> nodeIdsToNodes(const std::vector<long>& nodeIds,
                                                                 const std::shared_ptr<const OsmMap>& map);

  /**
   * Determines if the coordinates from two collection of nodes match, given a configurable
   * tolerance
   *
   * @param nodes1 the first collection of nodes to compare
   * @param nodes2 the second collection of nodes to compare
   * @return true if the coordinates match; false otherwise
   */
  static bool nodeCoordsMatch(std::vector<std::shared_ptr<const Node>> nodes1,
                              std::vector<std::shared_ptr<const Node>> nodes2);

  /**
   * Returns a printable string for a collection of nodes
   *
   * @param nodes the nodes for which to create a string
   * @return a string
   */
  static QString nodeCoordsToString(const std::vector<ConstNodePtr>& nodes);

  /**
    Converts a OSM node to a coordinate

    @param node the node to convert
    @returns a coordinate
    */
  static geos::geom::Coordinate nodeToCoord(const std::shared_ptr<const Node>& node);

  /**
    Converts a coordinate to an OSM node

    @param coord the coordinate to convert
    @param map the map owning the node to be created
    @returns a node
    */
  static std::shared_ptr<const Node> coordToNode(const geos::geom::Coordinate& coord,
                                                 const std::shared_ptr<const OsmMap>& map);

  /**
    Converts coordinates to OSM nodes

    @param coords the coordinates to convert
    @param map the map owning the nodes to be created
    @returns nodes
    */
  static QList<std::shared_ptr<const Node>> coordsToNodes(const QList<geos::geom::Coordinate>& coords,
                                                          const std::shared_ptr<const OsmMap>& map);
};

}

#endif // NODE_UTILS_H
