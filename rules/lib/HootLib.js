"use strict";
/**
 * @file
 * @par Hoot JavaScript API
 *
 * The HootLib contains a number of convenience functions for wrapping internal
 * Hootenanny operations. This makes the code a little easier for the
 * non-developer and adds sugar coating, although a little naughty wrapping
 * for the more experienced developer.
 */

/*
 * The comments within this file are used to generate user documentation. Please
 * keep this in mind while adding comments.
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
 * Returns true if the specified element is a building.
 */
function isBuilding(e)
{
    return hoot.OsmSchema.isBuilding(e);
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
 * Wrapper for require for backward compatibility.
 */
/*
function require(e)
{
    return hoot.require(e);
}
*/

/**
 * Returns true if the specified element is an area element. The approach used
 * to determine area vs. linear is quite complex, but some example are below.
 * - building=yes
 * - area=yes
 *
 * See the OSM wiki for more information: 
 * http://wiki.openstreetmap.org/wiki/Key:area
 */
function isArea(e)
{
    return hoot.OsmSchema.isArea(e);
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
function isLinear(e)
{
    return hoot.OsmSchema.isLinear(e);
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
function mergeElements(map, e1, e2) {
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
 * Snaps the ways in the second input to the first input. The replaced array will
 * be updated appropriately to reflect the elements that were replaced.
 */
function snapWays(sublineMatcher, map, pairs, replaced)
{
    return new hoot.HighwaySnapMerger().apply(sublineMatcher, map, pairs, replaced);
}

/**
 * Uses the SearchRadiusCalculator to automatically calculate a search radius based on tie points found
 * in the two input datasets.
 * 
 * @param map Map that contains the input datasets to be conflated
 * @param rubberSheetRef If true, the source layer will be moved toward the reference layer during
 * the rubber sheeting required to automatically calculate the search radius.
 * @param rubberSheetMinTies The minimum number of tie points that need to be found during rubber
 * sheeting for the automatic search radius calculation to occur.
 */
function calculateSearchRadius(map, rubberSheetRef, rubberSheetMinTies)
{
  return new hoot.SearchRadiusCalculator(
      { "rubber.sheet.ref" : rubberSheetRef },
      { "rubber.sheet.minimum.ties" : rubberSheetMinTies })
      .applyAndGetResult(map);
}

/**
 * Merges a map that contains nothing but node objects that are POI objects that should be merged into one node
 *
 * @param map Map that contains the POIs that should be merged together
 *
 * @return Map that contains the single merged POI node
 */
function poiMerge(map)
{
  return new hoot.poiMerge('PoiGeneric.js', map);
}
