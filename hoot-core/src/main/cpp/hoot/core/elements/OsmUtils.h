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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_UTILS_H
#define OSM_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/elements/ConstElementVisitor.h>

// GEOS
#include <geos/geom/Coordinate.h>

// Qt
#include <QString>

namespace hoot
{

class Node;
class Status;

/**
  Utilities for use with the OSM data model
  */
class OsmUtils
{
public:

  /**
    Prints a collection of nodes for debugging purposes (couldn't find a way to do this with the
    debug logger).

    @param nodeCollectionName
    @param nodes
    */
  static void printNodes(const QString& nodeCollectionName,
                         const QList<std::shared_ptr<const Node>>& nodes);

  /**
    Retrieves a collection of node ID's for a collection of nodes

    @param nodes a collection of nodes
    @return a collection of node ID's
    */
  static const QList<long> nodesToNodeIds(const QList<std::shared_ptr<const Node>>& nodes);

  /**
    Retrieves a collection of nodes given a collection of node ID's

    @param nodeIds a collection of node ID's
    @param map the map owning the nodes with the given ID's
    @return a collection of nodes
    */
  static QList<std::shared_ptr<const Node>> nodeIdsToNodes(const QList<long>& nodeIds,
                                                           const std::shared_ptr<const OsmMap>& map);

  /**
    Converts a OSM node to a coordinate

    @param node the node to convert
    @returns a coordinate
    */
  static geos::geom::Coordinate nodeToCoord(const std::shared_ptr<const Node>& node);

  /**
    Converts OSM nodes to a coordinates

    @param nodes the nodes to convert
    @returns coordinates
    */
  static QList<geos::geom::Coordinate> nodesToCoords(
    const QList<std::shared_ptr<const Node>>& nodes);

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
  static QList<std::shared_ptr<const Node>> coordsToNodes(
    const QList<geos::geom::Coordinate>& coords, const std::shared_ptr<const OsmMap>& map);

  /**
    Converts a 64-bit unsigned int timestamp (seconds from epoch) to a QString (utc zulu)

    @param timestamp quint64 time encoding in seconds from the epoch (1970-01-01T00:00:00Z)
    */
  static QString toTimeString(quint64 timestamp);

  /**
    Converts a utc zulu timestamp to time since the epoch in seconds.

    @param timestamp in utc zulu string to be convered to seconds from the epoch (1970-01-01 00:00:00)
    */
  static quint64 fromTimeString(QString timestamp);

  /**
   * Returns a time string for the current time
   *
   * @return
   */
  static QString currentTimeAsString();

  /**
   * Determines whether a map contains a minimum or a fixed amount of elements matching the
   * criterion type
   * Only objects of type ElementCriterion are allowed, all others will return false
   *
   * @param map the map to examine
   * @param minCount the minmal count of elements required (if exactCount == false)
   * @param exactCount if true, the count must be exactly minCount
   * @return true if the map meets the specified criteria; false otherwise
   */
  template<class C> static bool contains(const ConstOsmMapPtr& map, int minCount = 1,
                                         bool exactCount = false)
  {
    if (!std::is_base_of<ElementCriterion,C>::value) return false;

    const long count =
      (long)FilteredVisitor::getStat(
        ElementCriterionPtr(new C()),
        ConstElementVisitorPtr(new ElementCountVisitor()),
        map);
    LOG_VART(count);
    return exactCount ? (count == minCount) : (count >= minCount);
  }

  /**
   * Determines whether a collection of elements meet a criterion a minimum or a fixed amount of
   * times
   * Only objects of type ElementCriterion are allowed, all others will return false
   *
   * @param element the element to examine
   * @param minCount the minmal count of elements required (if exactCount == false)
   * @param exactCount if true, the count must be exactly minCount
   * @return true if the elements meet the specified criterion the specified number of times
   */
  template<class C> static bool isSatisfied(const std::vector<ConstElementPtr>& elements,
                                            int minCount = 1, bool exactCount = false)
  {
    if (!std::is_base_of<ElementCriterion,C>::value) return false;

    int count = 0;
    ElementCriterionPtr crit(new C());
    for (std::vector<ConstElementPtr>::const_iterator itr = elements.begin(); itr != elements.end();
         ++itr)
    {
      if (crit->isSatisfied(*itr))
      {
        count++;
      }
    }

    LOG_VART(count);
    return exactCount ? (count == minCount) : (count >= minCount);
  }

  /**
   * Get a more detailed string representation of a relation
   *
   * @param relation relation to get info from
   * @param map map owning the relation
   * @return a detailed relation string
   */
  static QString getRelationDetailedString(const ConstRelationPtr& relation, const ConstOsmMapPtr& map);

  /**
   * Get a detailed string representing a relation's members
   *
   * @param relation relation to get info from
   * @param map map owning the relation
   * @return a detailed relations members string
   */
  static QString getRelationMembersDetailedString(const ConstRelationPtr& relation,
                                                  const ConstOsmMapPtr& map);

  /**
   * Returns the first way ID from a set of relation members
   *
   * @param relation relation to check way ID for
   * @param map map owning the relation
   * @return a way ID
   */
  static long getFirstWayIdFromRelation(const ConstRelationPtr& relation, const OsmMapPtr& map);

  /**
   * Logs a detailed printout for an element
   *
   * @param element the element to log
   * @param map map owning the element
   * @param logLevel granularity at which to log the element
   * @param message optional message
   */
  static void logElementDetail(const ConstElementPtr& element, const ConstOsmMapPtr& map,
                               const Log::WarningLevel& logLevel = Log::Trace,
                               const QString& message = "");

  /**
   * Determines if two elements have conflicting one way street tags
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if their one way tags conflict; false otherwise
   */
  static bool oneWayConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Determines if a way has an explicitly negative one way tag (oneway=no, etc.)
   *
   * @param element the element to examine
   * @return true if the element contains a tag indicating it is not a one way street; false
   * otherwise
   */
  static bool explicitlyNotAOneWayStreet(const ConstElementPtr& element);

  /**
   * Determines if two elements have conflicting name tags
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if their name tags conflict; false otherwise
   */
  static bool nameConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Determines if there are specific highway type conflicts between elements
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if both have specific highway tags (other than highway=road) and they disagree;
   * false otherwise
   */
  static bool nonGenericHighwayConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2);

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
  static bool nodesAreContainedByTheSameWay(const long nodeId1, const long nodeId2,
                                            const ConstOsmMapPtr& map);
};

}

#endif // OSM_UTILS_H
