"use strict";
/**
 * @file
 * @par Hoot JavaScript API
 *
 * The HootLib contains a number of convenience functions for wrapping internal Hootenanny
 * operations. This makes the code a little easier for the non-developer and adds sugar coating,
 * although a little naughty wrapping for the more experienced developer.  The comments within this
 * file are used to generate user documentation. Please keep this in mind while adding comments.
 */

/**
 * A simple method handy for determining if the library has been included
 * properly.
 */
function testAdd(v1, v2)
{
  return v1 + v2;
}

/**
 * Wrapper for createUuid for backward compatibility.
 */
function createUuid()
{
  return hoot.UuidHelper.createUuid();
}

/**
 * Wrapper for getHootConfig for backward compatibility.
 */
function getHootConfig(e)
{
  return hoot.get(e);
}

/**
 * Returns all the kvps that are related to a specified kvp (even a little).
 */
function getRelatedTags(relateToKvp, d) {
    var result = [];
    for (var k in d) {
        var kvp = k + '=' + d[k];
        // This would need to be updated for features other than POI before it could be used outside
        // of Poi.js.
        if (kvp != "poi=yes" && kvp != "place=locality") {
            if (hoot.OsmSchema.score(relateToKvp, kvp) > 0) {
                result.push(kvp);
            }
        }
    }
    return result;
}

/**
 * Returns all the kvps that share a common ancestor
 */
function getTagsByAncestor(ancestorKvp, d) {
    var result = [];
    for (var k in d) {
        var kvp = k + '=' + d[k];
        if (hoot.OsmSchema.isAncestor(kvp, ancestorKvp)) {
            result.push(kvp);
        }
    }
    return result;
}

/**
 * Returns all the kvps that are in a category.
 */
function getTagsByCategory(category, d) {
    var result = [];
    for (var k in d) {
        var kvp = k + '=' + d[k];
        // if it is not a generic type
        // This would need to be updated for features other than POI before it could be used outside
        // of Poi.js.
        if (kvp != "poi=yes" && kvp != "place=locality") {
            if (hoot.OsmSchema.getCategories(kvp).indexOf(category) >= 0) {
                result.push(kvp);
            }
        }
    }
    return result;
}

// maintain a cache of diff classes
var tagAncestorDiffs = {};

/**
 * Given a common kvp, find the distance between the tags in e1 and e2 where common exists
 * between them.
 */
function getTagAncestorDistance(ancestorKvp, map, e1, e2) {
    var differ;
    if (ancestorKvp in tagAncestorDiffs == false) {
        differ = new hoot.TagAncestorDifferencer(
            {"tag.ancestor.differencer.name":ancestorKvp});
        tagAncestorDiffs[ancestorKvp] = differ;
    } else {
        differ = tagAncestorDiffs[ancestorKvp];
    }

    return differ.diff(map, e1, e2);
}

// maintain a cache of diff classes
var tagCategoryDiffs = {};

/**
 * Given a common kvp, find the distance between the tags in e1 and e2 where common exists
 * between them.
 */
function getTagCategoryDistance(category, map, e1, e2) {
    var differ;
    if (category in tagCategoryDiffs == false) {
        differ = new hoot.TagCategoryDifferencer(
            {"tag.category.differencer.name":category});
        tagCategoryDiffs[category] = differ;
    } else {
        differ = tagCategoryDiffs[category];
    }

    return differ.diff(map, e1, e2);
}

/**
 * Given a common kvp, find the distance between e1 and e2 where common exists
 * between them.
 */
function getTagDistance(commonKvp, t1, t2) {
    var result = 1;
    var c1 = getRelatedTags(commonKvp, t1);
    var c2 = getRelatedTags(commonKvp, t2);

    if (c1.length == 0 || c2.length == 0) {
        return undefined;
    }

    // find the best match between the two tag types
    for (var i in c1) {
        for (var j in c2) {
            result = Math.min(1 - hoot.OsmSchema.score(c1[i], c2[j]), result);
        }
    }

    return result;
}

/**
 * Determines if both features have a populated name
 */
function bothElementsHaveName(e1, e2)
{
  var name1 = String(e1.getTags().get("name")).trim();
  var name2 = String(e2.getTags().get("name")).trim();
  var bothHaveName = false;
  if (name1 !== 'undefined' && name1 !== null && name1 !== '' && name2 !== 'undefined' && name2 !== null && name2 !== '')
  {
    bothHaveName = true;
  }
  return bothHaveName;
}

/**
 * Determines if an element is a member of relation that has a specified type
 */
function isMemberOfRelationWithType(map, childElementId, relationType)
{
  return hoot.RelationMemberUtils.isMemberOfRelationWithType(childElementId, relationType, map);
}

/**
 * Determines if an element is a member of relation that is in a specified schema category
 */
function isMemberOfRelationInCategory(map, childElementId, schemaCategory)
{
  return hoot.RelationMemberUtils.isMemberOfRelationInCategory(map, childElementId, schemaCategory);
}

/**
 * Determines if an element is a member of a relation containing a specified tag key
 */
function isMemberOfRelationWithTagKey(map, childElementId, tagKey)
{
  return hoot.RelationMemberUtils.isMemberOfRelationWithTagKey(map, childElementId, tagKey);
}

/*
 * Determines if an element is a member of a relation that satisifies specified criteria
 */
function isMemberOfRelationSatisfyingCriterion(map, childElementId, criterionClassName)
{
  return hoot.RelationMemberUtils.isMemberOfRelationSatisfyingCriterion(map, childElementId, criterionClassName);
}

/**
 * Determines if two features mismatch on non-generic types
 */
function explicitTypeMismatch(e1, e2, minTypeScore)
{
  return hoot.OsmSchema.explicitTypeMismatch(e1, e2, minTypeScore);
}

/**
 * Returns the most specific type tag found as determined by the hoot schema. 
   If the element has more than one specific type, only the first will be returned.
 */
function mostSpecificType(e)
{
  return hoot.OsmSchema.mostSpecificType(e);
}

/**
 * Scores the similarity between two feature types
 */
function getTypeScore(e1, e2, ignoreGenericTypes)
{
  return hoot.OsmSchema.scoreTypes(e1.getTags(), e2.getTags(), ignoreGenericTypes);
}

/**
 * Determines if a feature has a generic type (e.g. has building=yes and no other type tags)
 */
function isGeneric(e)
{
  return hoot.OsmSchema.isGeneric(e);
}

/**
 * Determines if a feature has a specific type
 */
function hasType(e)
{
  return hoot.OsmSchema.hasType(e);
}

/**
 * Wrapper for logWarn for backward compatibility.
 */
function logWarn(e)
{
  return hoot.logWarn(e);
}

/**
 * Wrapper for logError for backward compatibility.
 */
function logError(e)
{
  return hoot.logError(e);
}

/**
 * Wrapper for print for backward compatibility.
 */
function print(e)
{
  hoot.print(e);
}

/**
 * Returns true if the specified element has a name
 */
function hasName(e)
{
  return hoot.OsmSchema.hasName(e);
}

/**
 * Log a string using Hootenanny's logging mechanism. By default this will
 * print the script location and the JSON version of the argument.
 *
 * @param s String to log.
 */
function log(s)
{
  hoot.log(s);
}

/**
 * Log a debug string using Hootenanny's logging mechanism. By default this will
 * print the script location and the JSON version of the argument.
 *
 * @param s String to log.
 */
function debug(s)
{
  hoot.debug(s);
}

/**
 * Merge the tags in the two elements into a resulting set of tags using the
 * default mechanism. Depending on the default mechanism order may matter.
 *
 * @param e1 First element
 * @param e2 Second element
 * @return Returns a merged set of tags
 */
function mergeTags(e1, e2)
{
  return hoot.TagMergerFactory.mergeTags(e1.getTags(), e2.getTags());
}

/**
 * Calculates the percent overlap of two elements. If the elements are of a
 * non-area type then the overlap will always be 0.
 *
 * @param map Map that contains the two elements.
 * @param e1 The first element to compare
 * @param e2 The second element to compare
 * @return The percentage overlap represented as a value from 0 to 1.
 */
function calculatePercentOverlap(map, e1, e2)
{
  return new hoot.OverlapExtractor().extract(map, e1, e2);
}

/**
 * This will merge elements 1 and 2. The geometry of 1 will be kept and 2
 * discarded. If element 2 is part of another element (e.g. in a way or
 * relation) element 1 will take its place in that element.
 */
function mergeElements(map, e1, e2) 
{
  // merge tags from e2 into e1 using default tag merging
  var newTags = mergeTags(e1, e2);
  e1.setTags(newTags);

  new hoot.ReplaceElementOp(e2, e1).apply(map);
  // remove the tags on e2 just in case we can't delete it.
  e2.setTags(new hoot.Tags());
  // try to delete e2. This may silently fail if it is still part of another
  // element. Failure in this case isn't necessarily bad.
  new hoot.RecursiveElementRemover(e2).apply(map);
}

/**
 * Remove a specified element and all unused child elements of the element.
 * For example, if you remove a way then the way and all nodes that make up
 * the way and are not being used by another element are removed.
 *
 * @param map Map that contains the element e
 * @param e Element to remove
 */
function removeElement(map, e)
{
  new hoot.RecursiveElementRemover(e).apply(map);
}

/**
 * Merges two relations
 */
function mergeRelations(map, elementId1, elementId2)
{
  return hoot.RelationMerger.mergeRelations(map, elementId1, elementId2);
}

/**
 * Recursively returns the total number of nodes contained with a relation
 */
function getNumRelationMemberNodes(map, relationId)
{
  return hoot.RelationMemberUtils.getNumRelationMemberNodes(map, relationId);
}

/**
 * Determines if two relations have at least one connected way member
 */
function relationsHaveConnectedWayMembers(map, relationId1, relationId2)
{
  return hoot.RelationMemberUtils.relationsHaveConnectedWayMembers(map, relationId1, relationId2);
}

/**
 * Snaps the ways in the second input to the first input. The replaced array will
 * be updated appropriately to reflect the elements that were replaced.
 */
function snapWays(sublineMatcher, map, pairs, replaced, matchedBy)
{
  return new hoot.LinearSnapMerger().apply(sublineMatcher, map, pairs, replaced, matchedBy);
}

/**
 * Another approach to snapping ways, which allows for using multiple subline matchers. See related
   notes in the mergeSets method of River.js.
 */
function snapWays2(sublineMatcher, map, pairs, replaced, matchedBy, sublineMatcher2)
{
  return new hoot.LinearSnapMerger().apply(sublineMatcher, map, pairs, replaced, matchedBy, sublineMatcher2);
}

/**
 * Uses the SearchRadiusCalculator to automatically calculate a search radius based on tie points
   found in the two input datasets.
 *
 * @param map Map that contains the input datasets to be conflated
 * @param rubberSheetRef If true, the source layer will be moved toward the reference layer during
 * the rubber sheeting required to automatically calculate the search radius.
 * @param rubberSheetMinTies The minimum number of tie points that need to be found during rubber
 * sheeting for the automatic search radius calculation to occur.
 * @param matchCandidateCriterion criterion used to filter match candidates
 */
function calculateSearchRadiusUsingRubberSheeting(map, rubberSheetRef, rubberSheetMinTies, matchCandidateCriterion)
{
  return new hoot.SearchRadiusCalculator(
      { "rubber.sheet.ref" : rubberSheetRef },
      { "rubber.sheet.minimum.ties" : rubberSheetMinTies },
      { "search.radius.calculator.element.criterion" : matchCandidateCriterion })
      .applyAndGetResult(map);
}

/**
 * Returns true if the feature is conflatable by any geometry non-generic conflation algorithm .
 */
function isSpecificallyConflatable(map, e, geometryTypeFilter)
{
  return hoot.OsmSchema.isSpecificallyConflatable(map, e, geometryTypeFilter);
}

// All of these 'is*' methods can go away if #3047 is completed.

/**
 * Returns true if the specified element is an area element. The approach used
 * to determine area vs. linear is quite complex, but some example are below.
 * - building=yes
 * - area=yes
 *
 * See the OSM wiki for more information:
 * http://wiki.openstreetmap.org/wiki/Key:area
 */
function isArea(map, e)
{
  return hoot.OsmSchema.isArea(map, e);
}

function isNonBuildingArea(map, e)
{
  return hoot.OsmSchema.isNonBuildingArea(map, e);
}

function isCollectionRelation(e)
{
  return hoot.OsmSchema.isCollectionRelation(e);
}

function isHighway(map, e)
{
  return hoot.OsmSchema.isHighway(map, e);
}

/**
 * Returns true if the specified element is a polygon element.
 */
function isPolygon(map, e)
{
  return hoot.OsmSchema.isPolygon(map, e);
}

/**
 * Returns true if the specified element is a point element.
 */
function isPoint(map, e)
{
  return hoot.OsmSchema.isPoint(map, e);
}

/**
 * Returns true if the specified element is a building.
 */
function isBuilding(map, e)
{
  return hoot.OsmSchema.isBuilding(map, e);
}

/**
 * Returns true if the specified element is an linear element. The approach used
 * to determine area vs. linear is quite complex, but some example are below.
 * - highway=road
 * - waterway=river
 *
 * See the OSM wiki for more information:
 * http://wiki.openstreetmap.org/wiki/Key:area
 */
function isLinear(d)
{
  return hoot.OsmSchema.isLinear(d);
}

/**
 * Returns true if the specified element is an linear waterway element.
 *
 * See the OSM wiki for more information:
 * http://wiki.openstreetmap.org/wiki/River
 */
function isLinearWaterway(e)
{
  return hoot.OsmSchema.isLinearWaterway(e);
}

/**
 * Returns true if the specified element is in the poi category in `schema.json` and the element is
 * a node type.
 */
function isPoi(e)
{
  return hoot.OsmSchema.isPoi(e);
}

/**
 * Returns true if the specified element is an railway element.
 *
 * See the OSM wiki for more information:
 * http://wiki.openstreetmap.org/wiki/Railway
 */
function isRailway(e)
{
  return hoot.OsmSchema.isRailway(e);
}

/**
 * Returns true if the specified element is an power line.
 *
 * See the OSM wiki for more information:
 * https://wiki.openstreetmap.org/wiki/Power
 */
function isPowerLine(e)
{
  return hoot.OsmSchema.isPowerLine(e);
}

/*
 * Returns the length of the feature in meters
 */
function getLength(map, e)
{
  return hoot.ElementToGeometryConverter.calculateLength(map, e);
}

/*
 * Returns the POI match/review distances used by POI to POI Conflation
 */
function getPoiSearchRadii()
{
  return hoot.PoiSearchRadius.getSearchRadii();
}

/*
 * Returns the maximum amount of match method recursions that should be allowed for river 
 * conflation based on the input data
 */
function getRiverMaxSublineRecursions(map)
{
  return hoot.RiverMaximalSublineSettingOptimizer.getFindBestMatchesMaxRecursions(map);
}

/*
 * Determines if an element has a particular geometric relationship with a bounds
 */
//function haveGeometricRelationship(e, bounds, relationship, map)
//{
//  return hoot.ElementGeometryUtils.haveGeometricRelationship(e, bounds, relationship, map);
//}
