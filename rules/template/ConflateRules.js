
/**
 * @file
 * @par JavaScript Conflation Reference
 *
 * The Conflation API provides the ability to perform simple matches and 
 * merges. Such as merging two points, or merging two buildings. This does
 * not provide the facilities to merge complex geometries such as one to many
 * or many to many merges.
 *
 * The methods below describe the methods that are recognized by Hootenanny
 * when loading and using a plugin. Some are required and some are optional.
 * For example conflation plugins see the $HOOT_HOME/rules/ directory.
 */

/**
 * If the searchRadius is not specified then the candidateDistanceSigma value
 * will be used to determine if two features are candidates for merging.
 *
 * The candidateDistanceSigma is a number that defines how far out potential
 * candidates will be accepted. For efficiency you may get some feature slightly
 * farther than the specified distance so you will have to filter further in the
 * matchScore method if you want to set a firm distance threshold.
 *
 * The default value is 1.0 and can generally be used unchanged.
 *
 * Please see the Calculating Search Radius section for details.
 */
var candidateDistanceSigma = 1.0;
exports['candidateDistanceSigma'] = candidateDistanceSigma;

/**
 * An exact search radius can be specified for use during match creation by
 * populating this variable.  If the variable is left with its default value,
 * it will not be used during match creation.
 *
 * Two features are considered candidates for matching if the maximum of the
 * two search radii is greater than or equal to the distance between the
 * features.
 *
 * Please see the Calculating Search Radius section for details.
 */
var searchRadius = -1.0;
exports['searchRadius'] = searchRadius;

/**
 * If implemented the user can override all search radius calculations on a
 * feature by returning a search radius appropriate to the feature.
 *
 * Please see the Calculating Search Radius section for details.
 */
function getSearchRadius(element) {}
exports['getSearchRadius'] = getSearchRadius;

/**
 * Runs before match creation occurs and provides an opportunity to perform
 * custom initialization.
 */
function init(map) {}
exports['init'] = init;

/**
 * If this function returns true then all overlapping matches will be treated
 * as a group. For now that means if two matches overlap then the whole group
 * will be marked as needing review.
 *
 * If this function returns false the conflation routines will attempt to 
 * pick the best subset of matches that do not conflict.
 *
 * @return True if the match should be treated as part of a group, false 
 *  otherwise.
 */
function isWholeGroup() {}
exports['isWholeGroup'] = isWholeGroup;

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 *
 * @param map A map that contains e will be passed in.
 * @param e The element that is a candidate for match with other elements.
 * @return True if the element is a match candidate, otherwise false.
 */
function isMatchCandidate(map, e) {}
exports['isMatchCandidate'] = isMatchCandidate;

/**
 * Returns the match score for the three class relationships.
 *
 * - match
 * - miss
 * - review
 *
 * The scores should always sum to one. If they don't you will be taunted 
 * mercilessly and we'll normalize it anyway. :P
 *
 * The resulting match is an object with at least one of the match, miss and
 * review scores. Values that are omitted are assumed to be zero. If the result
 * is a review then an explanation must be given in the 'explain' key. The
 * following are all valid results:
 * 
 * - return {"match": 1};
 * - return {"match": 0.5, "miss": 0.5, "review": 0};
 * - return {"review": 1, "explain": "This doesn't look right."};
 *
 * The explain text will be presented to the user when a review occurs.
 *
 * This method should consistently give the same result. Failure to do so may
 * give unexpected results. In other words don't rely on external stimulus that
 * may change the results such as random values or times.
 */
function matchScore(map, e1, e2) { }
exports['matchScore'] = matchScore;

/**
 * Simpler version of the merge function. Maybe only support this at first.
 * It only supports merging two elements and the replaced list is determined
 * implicitly based on the result.
 *
 * If this method is exported then the mergePair method should not be exported.
 *
 * This method should consistently give the same result. Failure to do so may
 * give unexpected results. In other words don't rely on external stimulus that
 * may change the results such as random values or times.
 */
function mergePair(map, e1, e2) { }
exports['mergePair'] = mergePair;

/**
 * The internals of geometry merging can become quite complex. Typically this 
 * method will simply call another hoot method to perform the appropriate
 * merging of geometries.
 *
 * If this method is exported then the mergePair method should not be exported.
 *
 * This method should consistently give the same result. Failure to do so may
 * give unexpected results. In other words don't rely on external stimulus that
 * may change the results such as random values or times.
 *
 * @param map The map that is being conflated
 * @param pairs An array of ElementId pairs that will be merged.
 * @param replaced An empty array is passed in, the method should fill the array
 *      with all the replacements that occur during the merge process (e.g. if
 *      two elements (way:1 & way:2) are merged into one element (way:3), then
 *      the replaced array should contain [ [way:1, way:3], [way:1, way:3] ]
 *      where all the "way:*" objects are of the ElementId type.
 */
function mergeSets(map,  pairs, replaced) { }
exports['mergeSets'] = mergeSets;


