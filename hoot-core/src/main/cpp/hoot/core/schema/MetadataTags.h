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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
  inline static const QString HootTagPrefix()           { return "hoot:"; }

  static const QString ACCURACY;
  inline static const QString& Accuracy()               { return ACCURACY; }
  static const QString ERROR_CIRCULAR;
  inline static const QString& ErrorCircular()          { return ERROR_CIRCULAR; }
  inline static const QString SourceDateTime()          { return "source:datetime"; }
  inline static const QString SourceIngestDateTime()    { return "source:ingest:datetime"; }

  inline static const QString HootBuildingMatch()       { return "hoot:building:match"; }

  inline static const QString HootActual()              { return "hoot:actual"; }
  inline static const QString HootExpected()            { return "hoot:expected"; }
  inline static const QString HootMismatch()            { return "hoot:mismatch"; }
  inline static const QString HootWrong()               { return "hoot:wrong"; }
  inline static const QString HootCorrectReview()       { return "hoot:correct_review"; }

  inline static const QString HootEdge()                { return "hoot:edge"; }
  inline static const QString HootEdgeId()              { return "hoot:edge:id"; }
  inline static const QString HootEdgeScore()           { return "hoot:edge:score"; }
  inline static const QString HootEdgeScore12()         { return "hoot:edge:score12"; }
  inline static const QString HootEdgeScore21()         { return "hoot:edge:score21"; }

  inline static const QString HootVertex()              { return "hoot:vertex"; }
  inline static const QString HootVertexScore()         { return "hoot:vertex:score"; }
  inline static const QString HootVertexScore12()       { return "hoot:vertex:score12"; }
  inline static const QString HootVertexScore21()       { return "hoot:vertex:score21"; }

  static const QString HOOT_ID;
  inline static const QString& HootId()                 { return HOOT_ID; }
  inline static const QString HootHash()                { return "hoot:hash"; }
  inline static const QString HootChangeType()          { return "hoot:change:type"; }
  inline static const QString HootChangeExcludeDelete() { return "hoot:change:exclude:delete"; }

  inline static const QString HootLayername()           { return "hoot:layername"; }

  inline static const QString HootMatchOrder()          { return "hoot:match:order"; }
  inline static const QString HootMatchP()              { return "hoot:match:p"; }
  inline static const QString HootMatchScore()          { return "hoot:match:score"; }
  inline static const QString HootMatchedBy()           { return "hoot:matchedBy"; }

  inline static const QString HootPertied()             { return "hoot:pertied"; }

  inline static const QString HootPoiPolygonPoisMerged(){ return "hoot:poipolygon:poismerged"; }
  inline static const QString HootMergeTarget()         { return "hoot:merge:target"; }

  inline static const QString HootReviewTagPrefix()     { return "hoot:review:"; }
  inline static const QString HootReviewChoices()       { return "hoot:review:choices"; }
  inline static const QString HootReviewMembers()       { return "hoot:review:members"; }
  inline static const QString HootReviewNeeds()         { return "hoot:review:needs"; }
  inline static const QString HootReviewNote()          { return "hoot:review:note"; }
  inline static const QString HootReviewScore()         { return "hoot:review:score"; }
  inline static const QString HootReviewSortOrder()     { return "hoot:review:sort_order"; }
  inline static const QString HootReviewType()          { return "hoot:review:type"; }
  inline static const QString HootReviewUuid()          { return "hoot:review:uuid"; }

  inline static const QString HootScoreMatch()          { return "hoot:score:match"; }
  inline static const QString HootScoreMiss()           { return "hoot:score:miss"; }
  inline static const QString HootScoreReview()         { return "hoot:score:review"; }
  inline static const QString HootScoreUuid()           { return "hoot:score:uuid"; }

  // used with JOSM validation interop; match those used in JosmValidator.java
  inline static const QString HootValidationError()     { return "hoot:validation:error"; }
  inline static const QString HootValidationCleanStatus()
  { return "hoot:validation:error:clean:status"; }
  inline static const QString HootValidationSource()    { return "hoot:validation:error:source"; }

  static const QString HOOT_STATUS;
  inline static const QString& HootStatus()             { return HOOT_STATUS; }

  inline static const QString HootSource()              { return "hoot:source"; }

  inline static const QString HootSpecial()             { return "hoot:special"; }
  inline static const QString RoundaboutCenter()        { return "roundabout_center"; }
  inline static const QString RoundaboutConnector()     { return "roundabout_connector"; }

  inline static const QString HootSplitParentId()       { return "hoot:split_parent_id"; }

  inline static const QString HootStub()                { return "hoot:stub"; }

  inline static const QString Ref1()                    { return "REF1"; }
  inline static const QString Ref2()                    { return "REF2"; }
  inline static const QString Review()                  { return "REVIEW"; }
  inline static const QString Uuid()                    { return "uuid"; }

  inline static const QString Unknown1()                { return "Unknown1"; }
  inline static const QString Unknown2()                { return "Unknown2"; }

  inline static const QString BuildingPart()            { return "building:part"; }
  inline static const QString BuildingHeight()          { return "height"; }
  inline static const QString BuildingLevels()          { return "building:levels"; }

  inline static const QString RelationBoundary()        { return "boundary"; }
  inline static const QString RelationBuilding()        { return "building"; }
  inline static const QString RelationCollection()      { return "collection"; }
  inline static const QString RelationInner()           { return "inner"; }
  inline static const QString RelationMultilineString() { return "multilinestring"; }
  inline static const QString RelationMultiPolygon()    { return "multipolygon"; }
  inline static const QString RelationMultiPoint()      { return "multipoint"; }
  inline static const QString RelationNetwork()         { return "network"; }
  inline static const QString RelationOuter()           { return "outer"; }
  inline static const QString RelationRestriction()     { return "restriction"; }
  inline static const QString RelationReview()          { return "review"; }
  inline static const QString RelationRoute()           { return "route"; }
  inline static const QString RelationRouteMaster()     { return "route_master"; }
  inline static const QString RelationSite()            { return "site"; }
  inline static const QString RelationSuperRoute()      { return "superroute"; }
  inline static const QString RelationWaterway()        { return "waterway"; }
  inline static const QString RelationType()            { return "type"; }

  inline static const QString RoleInner()               { return RelationInner(); }
  inline static const QString RoleOuter()               { return RelationOuter(); }
  inline static const QString RoleOutline()             { return "outline"; }
  inline static const QString RolePart()                { return "part"; }
  inline static const QString RoleReviewee()            { return "reviewee"; }
  // temp tag used by BuildingMerger
  inline static const QString HootMultiPolyRole()       { return "hoot:multi_poly:role"; }

  inline static const QString Length()                  { return "length"; }

  inline static const QString Source()                  { return "source"; }
  inline static const QString OsmApiDbScheme()          { return "osmapidb"; }
  inline static const QString HootApiDbScheme()         { return "hootapidb"; }
  /**
   * Identifies features snapped with UnconnectedWaySnapper
   */
  inline static const QString HootSnapped()             { return "hoot:snapped"; }

  /**
   * Identifies ways outside of convert.bounding.box that are immediately connected to other ways
   * within the bounds
   */
  inline static const QString HootConnectedWayOutsideBounds() { return "hoot:connected_way"; }

  /**
   * Identifies multilinestring relations hoot adds during conflation
   */
  inline static const QString HootMultilineString()     { return "hoot:multilinestring"; }

  /**
   * Identifies ElementConflatableCriteria that consider an element conflatable
   */
  inline static const QString HootConflatableCriteria() { return "hoot:conflatable:criteria"; }

  /**
   * ID Unique to a training data set with multiary training data.
   * @sa MultiaryMatchComparator
   */
  inline static const QString TrainingId()              { return "ID"; }
  /**
   * Refers to a match between two or more elements in a multiary training data set.
   * @sa MultiaryMatchComparator
   */
  inline static const QString TrainingMatch()           { return "MATCH"; }
  /**
   * Refers to a review between two or more elements in a multiary training data set.
   * @sa MultiaryMatchComparator
   */
  inline static const QString TrainingReview()          { return "REVIEW"; }

private:

  MetadataTags() { }
};

}

#endif // METADATA_TAGS_H
