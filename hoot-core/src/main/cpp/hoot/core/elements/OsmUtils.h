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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_UTILS_H
#define OSM_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/util/GeometricRelationship.h>

// GEOS
#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>

// Qt
#include <QString>

namespace hoot
{

class Node;
class Status;

/**
  Utilities for use with the OSM data model

  @todo Parts of this class could be split out into, perhaps: TagUtils, RelationUtils,
        GeometryUtils, WayNodeUtils, MapUtils
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
   * Retrieves a collection of element IDs for a collection of elements
   *
   * @param elements a collection of elements
   * @return a collection of element IDs
   * @todo may be able to replace nodesToNodeIds with this more generic version
   */
  static QSet<ElementId> elementsToElementIds(const std::vector<ElementPtr>& elements);

  /**
    Retrieves a collection of nodes given a collection of node IDs

    @param nodeIds a collection of node IDs
    @param map the map owning the nodes with the given IDs
    @return a collection of nodes
    */
  static QList<std::shared_ptr<const Node>> nodeIdsToNodes(
    const QList<long>& nodeIds, const std::shared_ptr<const OsmMap>& map);

  /**
   * Retrieves a collection of nodes given a collection of node IDs
   *
   * @param nodeIds a collection of node IDs
   * @param map the map owning the nodes with the given IDs
   * @return a collection of nodes
   */
  static std::vector<std::shared_ptr<const Node>> nodeIdsToNodes(
    const std::vector<long>& nodeIds, const std::shared_ptr<const OsmMap>& map);

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
   * Determines if the way node coordinates from two ways match, given a configurable
   * tolerance
   *
   * @param way1 the first way with nodes to compare
   * @param way2 the second way with nodes to compare
   * @param map the map owning the ways
   * @return true if the way node coordinates match; false otherwise
   */
  static bool nodeCoordsMatch(const ConstWayPtr& way1, const ConstWayPtr& way2,
                              const ConstOsmMapPtr& map);

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

    @param timestamp in utc zulu string to be convered to seconds from the epoch
           (1970-01-01 00:00:00)
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
  static QString getRelationDetailedString(const ConstRelationPtr& relation,
                                           const ConstOsmMapPtr& map);

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
   * Get a detailed string respresenting a way's nodes
   *
   * @param way way to get info from
   * @param map map owning the way
   * @return a detailed way nodes string
   */
  static QString getWayNodesDetailedString(const ConstWayPtr& way, const ConstOsmMapPtr& map);

  /**
   * Returns the first way ID from a set of relation members
   *
   * @param relation relation to check way ID for
   * @param map map owning the relation
   * @return a way ID
   */
  static long getFirstWayIdFromRelation(const ConstRelationPtr& relation, const OsmMapPtr& map);

  /**
   * Constructs a detailed string for an element suitable for trace logging
   *
   * @param element the element to create a string for
   * @param map map owning the element
   * @return a string describing the element
   */
  static QString getElementDetailString(const ConstElementPtr& element, const ConstOsmMapPtr& map);

  /**
   *  Constructs a detailed string for collection of elements suitable for trace logging
   *
   * @param elements the elements to create a string for
   * @param map map owning the elements
   * @return a string describing the elements
   */
  static QString getElementsDetailString(const std::vector<ElementPtr>& elements,
                                         const ConstOsmMapPtr& map);

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
  static bool nonGenericHighwayConflictExists(const ConstElementPtr& element1,
                                              const ConstElementPtr& element2);

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
   * Returns a subset of elements from a map filtered by a criterion
   *
   * @param map map to copy elements from
   * @param filter filter to apply to the map
   * @return a copied subset map
   */
  static OsmMapPtr getMapSubset(const ConstOsmMapPtr& map, const ElementCriterionPtr& filter);

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

  /**
   * Determines if an element is contained by any relation in a map
   *
   * @param elementId the ID of the element to search for
   * @param map the map containing the element
   * @return true if any relation in the map contains the element; false otherwise
   */
  static bool elementContainedByAnyRelation(const ElementId& elementId, const ConstOsmMapPtr& map);

  /**
   * Determines if an element is contained by any way or relation in a map
   *
   * @param elementId the ID of the element to search for
   * @param map the map containing the element
   * @return true if any way or relation in the map contains the element; false otherwise
   */
  static bool isChild(const ElementId& elementId, const ConstOsmMapPtr& map);

  /**
   * Returns the number of elements with a changeset version less than one
   *
   * @param map the map to examine
   * @return the number of elements meeting the criteria
   */
  static int versionLessThanOneCount(const OsmMapPtr& map);

  /**
   * Logs a warning if any element in the map has a changeset version less than one
   *
   * @param map the map to examine
   * @return true if a warning was logged; false otherwise
   */
  static bool checkVersionLessThanOneCountAndLogWarning(const OsmMapPtr& map);

  /**
   * Returns the element IDs of all elements with a changeset version less than one
   *
   * @param map the map to examine
   * @return a set of element IDs
   */
  static std::set<ElementId> getIdsOfElementsWithVersionLessThanOne(const OsmMapPtr& map);

  /**
   * Determines if a map contains only nodes that are not way nodes
   *
   * @param map the map to examine
   * @return true if the map is made up of non-way node points only; false otherwise
   */
  static bool mapIsPointsOnly(const OsmMapPtr& map);

  // This group of allElements* methods really should probably be passing const elements and maps.

  /**
   * Determines if all elements in a specified collection have any tag key from a specified set of
   * keys
   *
   * @param tagKeys the tag keys to search for
   * @param elements the elements to examine
   * @return true if all elements from the input collection of elements contain at least one of the
   * tag keys specified in tagKeys; false otherwise
   */
  static bool allElementsHaveAnyTagKey(const QStringList& tagKeys,
                                       const std::vector<ElementPtr>& elements);

  /**
   * Determines if all elements in a specified collection have any tag key/value pair from a
   * specified set of kvps
   *
   * @param kvp the tag key/value pairs to search for
   * @param elements the elements to examine
   * @return true if all elements from the input collection of elements contain at least one of the
   * key/value pairs specified in kvps; false otherwise
   */
  static bool allElementsHaveAnyKvp(const QStringList& kvps,
                                    const std::vector<ElementPtr>& elements);

  /**
   * Determines if any elements in a specified collection have any tag key from a specified set of
   * keys
   *
   * @param tagKeys the tag keys to search for
   * @param elements the elements to examine
   * @return true if any elements from the input collection of elements contain at least one of the
   * tag keys specified in tagKeys; false otherwise
   */
  static bool anyElementsHaveAnyTagKey(const QStringList& tagKeys,
                                       const std::vector<ElementPtr>& elements);

  /**
   * Determines if any elements in a specified collection have any tag key/value pair from a
   * specified set of kvps
   *
   * @param kvps the tag key/value pairs to search for
   * @param elements the elements to examine
   * @return true if any elements from the input collection of elements contain at least one of the
   * key/value pairs specified in kvps; false otherwise
   */
  static bool anyElementsHaveAnyKvp(const QStringList& kvps,
                                    const std::vector<ElementPtr>& elements);

  /**
   * Determines if all elements in a specified collection have any tag key from a specified set of
   * keys
   *
   * @param tagKeys the tag keys to search for
   * @param elementIds IDs of the elements to examine
   * @param map the map containing the elements
   * @return true if all elements from the input collection of elements contain at least one of the
   * tag keys specified in tagKeys; false otherwise
   */
  static bool allElementsHaveAnyTagKey(const QStringList& tagKeys,
                                       const std::set<ElementId>& elementIds, OsmMapPtr& map);

  /**
   * Determines if all elements in a specified collection have any tag key/value pair from a
   * specified set of kvps
   *
   * @param kvp the tag key/value pairs to search for
   * @param elementIds IDs of the elements to examine
   * @param map the map containing the elements
   * @return true if all elements from the input collection of elements contain at least one of the
   * key/value pairs specified in kvps; false otherwise
   */
  static bool allElementsHaveAnyKvp(const QStringList& kvps,
                                    const std::set<ElementId>& elementIds, OsmMapPtr& map);

  /**
   * Determines if any elements in a specified collection have any tag key from a specified set of
   * keys
   *
   * @param tagKeys the tag keys to search for
   * @param elementIds IDs of the elements to examine
   * @param map the map containing the elements
   * @return true if any elements from the input collection of elements contain at least one of the
   * tag keys specified in tagKeys; false otherwise
   */
  static bool anyElementsHaveAnyTagKey(const QStringList& tagKeys,
                                       const std::set<ElementId>& elementIds, OsmMapPtr& map);

  /**
   * Determines if any elements in a specified collection have any tag key/value pair from a
   * specified set of kvps
   *
   * @param kvps the tag key/value pairs to search for
   * @param elementIds IDs of the elements to examine
   * @param map the map containing the elements
   * @return true if any elements from the input collection of elements contain at least one of the
   * key/value pairs specified in kvps; false otherwise
   */
  static bool anyElementsHaveAnyKvp(const QStringList& kvps,
                                    const std::set<ElementId>& elementIds, OsmMapPtr& map);

  /**
   * Determines if all element IDs in a map are positive
   *
   * @param map the map to examine
   * @return true if all elements in the input map have a positive ID; false otherwise
   */
  static bool allElementIdsPositive(const ConstOsmMapPtr& map);

  /**
   * Determines if all element IDs in a map are negative
   *
   * @param map the map to examine
   * @return true if all elements in the input map have a negative ID; false otherwise
   */
  static bool allElementIdsNegative(const ConstOsmMapPtr& map);

  /**
   * Determines if all hoot:id element tags in a map match their element IDs
   *
   * @param map the map to examine
   * @return true if all the hoot:id tag values match the owning element's ID; false otherwise
   */
  static bool allIdTagsMatchIds(const ConstOsmMapPtr& map);

  /**
   * Returns the distance between two elements
   *
   * @param element1 the first element to measure distance from
   * @param element2 the second element to measure distance from
   * @param map map owning the input elements
   * @return the distance between the two elements or -1.0 if the distance could not be calculated
   */
  static double getDistance(const ConstElementPtr& element1, const ConstElementPtr& element2,
                            ConstOsmMapPtr map);

  /**
   * Calculates the area of an element
   *
   * @param element the feature to calculate the area of
   * @param map map owning the input element
   * @return the area of the feature or -1.0 if the area could not be calculated
   */
  static double getArea(const ConstElementPtr& element, ConstOsmMapPtr map);

  /**
   * Determines two elements have a geometric relationship
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @param relationship the geometric relationship to check for between the two elements
   * @param map map owning the input elements
   * @return true if the two elements have the specified geometric relationship; false otherwise or
   * if the relationship could not be calculated
   * @todo should eventually back this with a cache, as is done in PoiPolygonInfoCache
   */
  static bool haveGeometricRelationship(
    const ConstElementPtr& element1, const ConstElementPtr& element2,
    const GeometricRelationship& relationship, ConstOsmMapPtr map);

  /**
   * Determines if an element has a given criterion; backed by a cache
   *
   * @param element the element to examine
   * @param criterionClassName class name of the ElementCriterion to determine membership of
   * @return true if the element has the criterion; false otherwise
   * @throws if the criterion class name is invalid
   */
  static bool hasCriterion(const ConstElementPtr& element, const QString& criterionClassName);

  /**
   * Determines if one element a child of another; e.g. way node or relation memeber
   *
   * @param parent the parent element
   * @param memberId the element ID of the child
   * @return true if parent has the element with memberId as a child; false otherwise
   */
  static bool containsMember(const ConstElementPtr& parent, const ElementId& memberId);

  /**
   * Determines if an element is a member of a relation
   *
   * @param map map owning the input element
   * @param childId element ID of the input element
   * @return true if the element is a member of a relation
   */
  static bool isMemberOfRelation(const ConstOsmMapPtr& map, const ElementId& childId);

  /**
   * Determines if an element is a member of a relation of a given type
   *
   * @param map map owning the input element
   * @param childId element ID of the input element
   * @param relationType type of relation to search for; if left blank, relation types are not
   * checked
   * @return true if the element is a member of a relation of the specified type OR if the element
   * is simply a member of a relation and the type is not specified; false otherwise
   */
  static bool isMemberOfRelationType(const ConstOsmMapPtr& map, const ElementId& childId,
                                     const QString& relationType = "");

  /**
   * Determines if an element is a member of a relation that is in a given schema category
   *
   * @param map map owning the input element
   * @param childId element ID of the input element
   * @param schemaCategory schema category to search for relation membership in; if left blank,
   * category membership for the relations is not checked
   * @return true if the element is a member of a relation that is in the specified schema category
   * OR if the element is simply a member of a relation and the category is not specified; false
   * otherwise
   */
  static bool isMemberOfRelationInCategory(const ConstOsmMapPtr& map, const ElementId& childId,
                                           const QString& schemaCategory = "");

  /**
   * Determines if an element is a member of a relation that has a given tag key
   *
   * @param map map owning the input element
   * @param childId element ID of the input element
   * @param tagKey tag key to search relations for; if left blank, tag keys on the relations are not
   * checked
   * @return true if the element is a member of a relation with the specified tag key OR if the
   * element is simply a member of a relation and a tag key is not specified
   */
  static bool isMemberOfRelationWithTagKey(const ConstOsmMapPtr& map, const ElementId& childId,
                                           const QString& tagKey = "");

  /**
   * TODO
   *
   * @param map
   * @param relation1
   * @param relation2
   * @return
   */
  static bool relationsHaveConnectedWayMembers(
    const ConstOsmMapPtr& map, const ConstRelationPtr& relation1,
    const ConstRelationPtr& relation2);

  /**
   * TODO
   *
   * @param way1
   * @param way2
   * @return
   */
  static QSet<long> getSharedWayNodes(const ConstWayPtr& way1, const ConstWayPtr& way2);

  /**
   * TODO
   *
   * @param way1
   * @param way2
   * @return
   */
  static bool waysShareNode(const ConstWayPtr& way1, const ConstWayPtr& way2);

  /**
   * TODO
   *
   * @param way1
   * @param way2
   * @param sameDirection
   * @return
   */
  static bool waysShareEndNode(
    const ConstWayPtr& way1, const ConstWayPtr& way2, const bool sameDirection = true);

private:

  static int _badGeomCount;

  static std::shared_ptr<geos::geom::Geometry> _getGeometry(
    const ConstElementPtr& element, ConstOsmMapPtr map);
  static ElementCriterionPtr _getCrit(const QString& criterionClassName);
};

}

#endif // OSM_UTILS_H
