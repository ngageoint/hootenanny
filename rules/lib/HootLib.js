"use strict";
/**
 * @file
 * @par JavaScript API
 *
 * HootLib allows for quickly adding reusable logic useful to multiple Javascript hoot clients.
 * If you are core developer, determine if all or some of your added logic would be more
 * appropriately placed in the hoot-core C++ code before adding it here in order to reduce code
 * duplication.

 * The comments within this file are used to generate user documentation. Please keep this in mind
 * when adding comments.
 */

/**
 * A simple method handy for determining if the library has been included
 * properly.
 */
function testAdd(v1, v2)
{
  return v1 + v2;
}

/////////////////GENERAL////////////////

function stringIsEmpty(str)
{
  return str === null || str === '' || str === 'undefined';
}

/////////////////MERGING////////////////

/**
 * This will merge elements 1 and 2. The geometry of 1 will be kept and 2 discarded. If element 2 is
   part of another element (e.g. in a way or relation) element 1 will take its place in that
   element.
 */
function mergeElements(map, e1, e2) 
{
  // Merge tags from e2 into e1 using default tag merging.
  var newTags = hoot.TagMergerFactory.mergeTags(e1.getTags(), e2.getTags());
  e1.setTags(newTags);

  new hoot.ReplaceElementOp(e2, e1).apply(map);
  // Remove the tags on e2 just in case we can't delete it.
  e2.setTags(new hoot.Tags());
  // Try to delete e2. This may silently fail if it is still part of another element. Failure in
  // this case isn't necessarily bad.
  new hoot.RecursiveElementRemover(e2).apply(map);
}

///////////////SCHEMA///////////////////////

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

/////////////////SEARCH RADIUS//////////////

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

/////////////////UTILITIES////////////////

// Would like to try to remove these one-liners but have been unable to make the translations in
// translations-local work with the "hoot." prefix.

/**
 * Wrapper for createUuid for backward compatibility.
 */
function createUuid()
{
  return hoot.UuidHelper.createUuid();
}

/**
 * Wrapper for print for backward compatibility.
 */
function print(e)
{
  hoot.print(e);
}

/**
 * Log a string using Hootenanny's logging mechanism. By default this will print the script location
   and the JSON version of the argument.
 *
 * @param s String to log.
 */
function log(s)
{
  hoot.log(s);
}

/**
 * Log a debug string using Hootenanny's logging mechanism. By default this will print the script
   location and the JSON version of the argument.
 *
 * @param s String to log.
 */
function debug(s)
{
  hoot.debug(s);
}
