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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef METADATA_TAGS_H
#define METADATA_TAGS_H

// Qt
#include <QString>

namespace hoot
{

/**
 * Metadata tag names that are used throughout the code.  This class centralizes them
 * into one place in the code base.  This class also includes a couple of key values too.
 *
 * These static methods cause the QString to be recreated from the ASCII version every time
 * the method is called. This does two things.
 * 1. The QString is recreated and duplicate RAM is allocated, used then freed
 * 2. The QString must parse the ASCII which takes time.
 *
 * In some non-critical places this isn't a big deal, however in other spots it can significantly
 * impact the code. If this is the case, please entertain creating a static variable and returning
 * a reference to that variable. @sa ErrorCircular() and ERROR_CIRCULAR
 */
class MetadataTags
{
public:
  inline static QString HootTagPrefix()                 { return "hoot:"; }

  static const QString ACCURACY;
  inline static const QString& Accuracy()               { return ACCURACY; }
  static const QString ERROR_CIRCULAR;
  inline static const QString& ErrorCircular()          { return ERROR_CIRCULAR; }
  inline static QString SourceDateTime()                { return "source:datetime"; }
  inline static QString SourceIngestDateTime()          { return "source:ingest:datetime"; }
  inline static QString SourceImagery()                 { return "source:imagery"; }

  inline static QString HootBuildingMatch()             { return "hoot:building:match"; }

  inline static QString HootActual()                    { return "hoot:actual"; }
  inline static QString HootExpected()                  { return "hoot:expected"; }
  inline static QString HootMismatch()                  { return "hoot:mismatch"; }
  inline static QString HootWrong()                     { return "hoot:wrong"; }
  inline static QString HootCorrectReview()             { return "hoot:correct_review"; }

  inline static QString HootEdge()                      { return "hoot:edge"; }
  inline static QString HootEdgeId()                    { return "hoot:edge:id"; }
  inline static QString HootEdgeScore()                 { return "hoot:edge:score"; }
  inline static QString HootEdgeScore12()               { return "hoot:edge:score12"; }
  inline static QString HootEdgeScore21()               { return "hoot:edge:score21"; }

  inline static QString HootVertex()                    { return "hoot:vertex"; }
  inline static QString HootVertexScore()               { return "hoot:vertex:score"; }
  inline static QString HootVertexScore12()             { return "hoot:vertex:score12"; }
  inline static QString HootVertexScore21()             { return "hoot:vertex:score21"; }

  static const QString HOOT_ID;
  inline static const QString& HootId()                 { return HOOT_ID; }
  inline static QString HootHash()                      { return "hoot:hash"; }
  inline static QString HootChangeType()                { return "hoot:change:type"; }
  inline static QString HootChangeExcludeDelete()       { return "hoot:change:exclude:delete"; }

  inline static QString HootLayername()                 { return "hoot:layername"; }

  // These are used by rubbersheeting only...could be a little better named.
  inline static QString HootMatchOrder()                { return "hoot:match:order"; }
  inline static QString HootMatchP()                    { return "hoot:match:p"; }
  inline static QString HootMatchScore()                { return "hoot:match:score"; }

  inline static QString HootMatchedBy()                 { return "hoot:matchedBy"; }

  inline static QString HootPertied()                   { return "hoot:pertied"; }

  // used by PoiPolygonMerger to keep track of the number of POIs merged into a polygon during
  // POI/Polygon Conflation
  inline static QString HootPoiPolygonPoisMerged()      { return "hoot:poipolygon:poismerged"; }
  inline static QString HootPoiPolygonPoisMerged1()     { return "hoot:poipolygon:poismerged:1"; }
  inline static QString HootPoiPolygonPoisMerged2()     { return "hoot:poipolygon:poismerged:2"; }

  inline static QString HootMergeTarget()               { return "hoot:merge:target"; }

  inline static QString HootReviewTagPrefix()           { return "hoot:review:"; }
  inline static QString HootReviewChoices()             { return "hoot:review:choices"; }
  inline static QString HootReviewMembers()             { return "hoot:review:members"; }
  inline static QString HootReviewNeeds()               { return "hoot:review:needs"; }
  inline static QString HootReviewNote()                { return "hoot:review:note"; }
  inline static QString HootReviewScore()               { return "hoot:review:score"; }
  inline static QString HootReviewSortOrder()           { return "hoot:review:sort_order"; }
  inline static QString HootReviewType()                { return "hoot:review:type"; }
  inline static QString HootReviewUuid()                { return "hoot:review:uuid"; }
  inline static QString HootReviewRoadCrossingPolygon() { return "road_crossing_polygon"; }

  inline static QString HootScoreMatch()                { return "hoot:score:match"; }
  inline static QString HootScoreMiss()                 { return "hoot:score:miss"; }
  inline static QString HootScoreReview()               { return "hoot:score:review"; }
  inline static QString HootScoreMatchThreshold()       { return "hoot:score:match:threshold"; }
  inline static QString HootScoreMissThreshold()        { return "hoot:score:miss:threshold"; }
  inline static QString HootScoreReviewThreshold()      { return "hoot:score:review:threshold"; }
  inline static QString HootScoreUuid()                 { return "hoot:score:uuid"; }
  inline static QString HootUserName()                  { return "hoot:user_name"; }
  inline static QString HootUserId()                    { return "hoot:user_id"; }

  // used with JOSM validation interop; match those used in JosmValidator.java
  inline static QString HootValidationError()           { return "hoot:validation:error"; }
  inline static QString HootValidationCleanStatus()     { return "hoot:validation:error:clean:status"; }
  inline static QString HootValidationSource()          { return "hoot:validation:error:source"; }

  static const QString HOOT_STATUS;
  inline static const QString& HootStatus()             { return HOOT_STATUS; }

  inline static QString HootSource()                    { return "hoot:source"; }

  inline static QString HootSpecial()                   { return "hoot:special"; }
  inline static QString RoundaboutCenter()              { return "roundabout_center"; }
  inline static QString RoundaboutConnector()           { return "roundabout_connector"; }

  inline static QString HootSplitParentId()             { return "hoot:split_parent_id"; }

  inline static QString HootStub()                      { return "hoot:stub"; }

  inline static QString HootFirstSeen()                 { return "hoot:first_seen"; }
  inline static QString HootLastSeen()                  { return "hoot:last_seen"; }
  inline static QString HootTotalObservations()         { return "hoot:total_observations"; }
  inline static QString HootTotalMisses()               { return "hoot:total_misses"; }
  inline static QString HootMaxConsecutive()            { return "hoot:max_consecutive"; }
  inline static QString HootMaxConsecutiveStart()       { return "hoot:max_consecutive_start"; }
  inline static QString HootHistogram()                 { return "hoot:histogram"; }

  inline static QString Ref1()                          { return "REF1"; }
  inline static QString Ref2()                          { return "REF2"; }
  inline static QString Review()                        { return "REVIEW"; }
  inline static QString Uuid()                          { return "uuid"; }

  inline static QString Unknown1()                      { return "Unknown1"; }
  inline static QString Unknown2()                      { return "Unknown2"; }

  inline static QString BuildingPart()                  { return "building:part"; }
  inline static QString BuildingHeight()                { return "height"; }
  inline static QString BuildingLevels()                { return "building:levels"; }

  inline static QString RelationBoundary()              { return "boundary"; }
  inline static QString RelationBuilding()              { return "building"; }
  inline static QString RelationCollection()            { return "collection"; }
  inline static QString RelationInner()                 { return "inner"; }
  inline static QString RelationMultilineString()       { return "multilinestring"; }
  inline static QString RelationMultiPolygon()          { return "multipolygon"; }
  inline static QString RelationMultiPoint()            { return "multipoint"; }
  inline static QString RelationNetwork()               { return "network"; }
  inline static QString RelationOuter()                 { return "outer"; }
  inline static QString RelationRestriction()           { return "restriction"; }
  inline static QString RelationReview()                { return "review"; }
  inline static QString RelationRoute()                 { return "route"; }
  inline static QString RelationRouteMaster()           { return "route_master"; }
  inline static QString RelationSite()                  { return "site"; }
  inline static QString RelationSuperRoute()            { return "superroute"; }
  inline static QString RelationWaterway()              { return "waterway"; }
  inline static QString RelationType()                  { return "type"; }

  inline static QString RoleInner()                     { return RelationInner(); }
  inline static QString RoleOuter()                     { return RelationOuter(); }
  inline static QString RoleOutline()                   { return "outline"; }
  inline static QString RolePart()                      { return "part"; }
  inline static QString RoleReviewee()                  { return "reviewee"; }
  // temp tag used by BuildingMerger
  inline static QString HootMultiPolyRole()             { return "hoot:multi_poly:role"; }

  inline static QString Length()                        { return "length"; }

  inline static QString Source()                        { return "source"; }
  inline static QString OsmApiDbScheme()                { return "osmapidb"; }
  inline static QString HootApiDbScheme()               { return "hootapidb"; }

  /**
   * This is used by ChangesetReplacementCreator to focus attention on input elements with missing
   * child references so that they may later be cleaned up.
   */
  inline static QString HootMissingChild()              { return "hoot:missing_child"; }

  /**
   * Used by ElementIdSynchronizer to mark elements whose IDs it synched
   */
  inline static QString HootIdSynchronized()            { return "hoot:synced:id"; }

  /**
   * Used to identify superfluous nodes without removing them for debugging purposes
   */
  inline static QString HootSuperfluous()               { return "hoot:superfluous"; }

  /**
   * Used to identify disconnected ways for debugging purposes
   */
  inline static QString HootDisconnected()              { return "hoot:disconnected"; }

  /**
   * Used to identify way end nodes for debugging purposes
   */
  inline static QString HootWayEndNode()                { return "hoot:way_end_node"; }

  /**
   * Used to identify empty ways for debugging purposes
   */
  inline static QString HootEmptyWay()                  { return "hoot:empty:way"; }

  /**
   * Used to identify superfluous nodes without removing them for debugging purposes
   */
  inline static QString HootDuplicate()                 { return "hoot:duplicate"; }

  /**
   * Used to mark roads that are divided highways
   */
  inline static QString HootDualHighway()               { return "hoot:dual_highway"; }

  /**
   * Used to mark roads that cross divided highways
   */
  inline static QString HootDualHighwayCrossing()       { return "hoot:dual_highway_crossing"; }

  /**
   * Identifies features snapped with UnconnectedWaySnapper
   *
   * values in use:
   *
   * to_way_node_source - source way node or way containing the source way node snapped to an
   *                      existing way node on another target way
   * to_way_node_target - existing target way node or way containing the existing target way node
   *                      that a source feature was snapped to
   * to_way_source      - source way node or way containing the source way node snapped to an
   *                      newly added way node on another target way
   * to_way_target      - newly added target way node or way containing the newly added target way
   *                      node that a source feature was snapped to
   */
  inline static QString HootSnapped()                   { return "hoot:snapped"; }

  /**
   * Identifies ways outside of bounds that are immediately connected to other ways within the
   * bounds
   */
  inline static QString HootConnectedWayOutsideBounds() { return "hoot:connected_way"; }

  /**
   * Identifies multilinestring relations hoot adds during conflation
   */
  inline static QString HootMultilineString()           { return "hoot:multilinestring"; }

  /**
   * Identifies ElementConflatableCriteria that consider an element conflatable
   */
  inline static QString HootConflatableCriteria()       { return "hoot:conflatable:criteria"; }

  /**
   * ID Unique to a training data set with multiary training data.
   * @sa MultiaryMatchComparator
   */
  inline static QString TrainingId()                    { return "ID"; }
  /**
   * Refers to a match between two or more elements in a multiary training data set.
   * @sa MultiaryMatchComparator
   */
  inline static QString TrainingMatch()                 { return "MATCH"; }
  /**
   * Refers to a review between two or more elements in a multiary training data set.
   * @sa MultiaryMatchComparator
   */
  inline static QString TrainingReview()                { return "REVIEW"; }

  // used by FindIntersectionsVisitor
  inline static QString HootIntersectionWayCount()      { return "hoot:intersection:way_count"; }
  inline static QString HootIntersectionMinAngle()      { return "hoot:intersection:min_angle"; }
  inline static QString HootIntersectionMaxAngle()      { return "hoot:intersection:max_angle"; }

  // used by FindStreetIntersectionsByName
  inline static QString HootIntersectionStreet1()       { return "hoot:intersection:street1"; }
  inline static QString HootIntersectionStreet2()       { return "hoot:intersection:street2"; }

  inline static QString HootWayNodeCount()              { return "hoot:way:node:count"; }

private:

  MetadataTags() = default;
};

}

#endif // METADATA_TAGS_H
