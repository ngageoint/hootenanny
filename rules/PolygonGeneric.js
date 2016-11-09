"use strict";

var MATCH_OVERLAP_THRESHOLD = 0.75;
var MISS_OVERLAP_THRESHOLD = 0.15;

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("generic.polygon.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("generic.polygon.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("generic.polygon.review.threshold"));
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.polygon"));
exports.experimental = true;

/**
 * Runs before match creation occurs and provides an opportunity to perform custom initialization.
 */
exports.init = function(map) {

}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
    return isArea(e);
};

/**
 * If this function returns true then all overlapping matches will be treated
 * as a group. For now that means if two matches overlap then the whole group
 * will be marked as needing review.
 *
 * If this function returns false the conflation routines will attempt to 
 * pick the best subset of matches that do not conflict.
 */
exports.isWholeGroup = function()
{
    return true;
};

/**
 * Returns the match score for the three class relationships.
 * - match
 * - miss
 * - review
 *
 * The scores should always sum to one. If they don't you will be taunted 
 * mercilessly and we'll normalize it anyway. :P
 */
exports.matchScore = function(map, e1, e2)
{
    var result;
    if (isArea(e1) == false || isArea(e2) == false)
    {
        result = { match: 0.0, miss: 1.0, review: 0.0 };
    }
    else
    {
        // These rules were derived by using training data in Weka with the
        // REPTree model w/ maxDepth set to 3. 
        var overlap = new hoot.SmallerOverlapExtractor().extract(map, e1, e2);
        if (overlap < 0.20)
        {
            result = { match: 0.0, miss: 1.0, review: 0.0 };
        }
        else 
        {
            if (overlap < 0.45)
            {
                var hist = new hoot.AngleHistogramExtractor().
                    extract(map, e1, e2);
                if (hist < 0.58)
                {
                    result = { match: 0.0, miss: 0.0, review: 1.0 };
                }
                else
                {
                    result = { match: 1.0, miss: 0.0, review: 0.0 };
                }
            }
            else
            {
                result = { match: 1.0, miss: 0.0, review: 0.0 };
            }
        }
    }
    return result;
};

/**
 * Simpler version of the mergeSets function. Maybe only support this at first.
 * It only supports merging two elements and the replaced list is determined
 * implicitly based on the result.
 */
exports.mergePair = function(map, e1, e2)
{
    var newTags = mergeTags(e1, e2);
    e1.setTags(newTags);

    removeElement(map, e2);

    return e1;
};

/**
 * The internals of geometry merging can become quite complex. Rather than expose all those
 * geometries out to the user for merging two sets, let them return the merge method being used
 * along with some tunable parameters.
 *
 * This should handle a large number of scenarios by simply implementing more internal, complex
 * merge methods.
 */
exports.mergeSets = function(map, set1, set2) {

    // snap the ways in set2 on to set1. Use the default merge method for merging the tags while
    // snapping.
    return new hoot.SnapWays(mergeTags);
};


