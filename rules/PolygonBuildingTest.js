/**
 * @file
 *
 * This rules conflation plugin is for testing purposes. Please do not make
 * changes to the file or it could impact system tests.
 */

var MATCH_OVERLAP_THRESHOLD = 0.75;
var MISS_OVERLAP_THRESHOLD = 0.01;

exports.description = "Polygon Building Test";
exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("building.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("building.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("building.review.threshold"));

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
    return isBuilding(e);
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
    if (isBuilding(e1) == false || isBuilding(e2) == false ||
        e1.getStatusString() == e2.getStatusString())
    {
        result = { match: 0.0, miss: 1.0, review: 0.0 };
    }
    else
    {
        var overlap = calculatePercentOverlap(map, e1, e2);
        if (overlap > MATCH_OVERLAP_THRESHOLD)
        {
            result = { match: 1.0, miss: 0.0, review: 0.0 };
        }
        else if (overlap < MISS_OVERLAP_THRESHOLD)
        {
            result = { match: 0.0, miss: 1.0, review: 0.0 };
        }
        else
        {
            result = { review: 1.0, 
                       explain: "Not sufficiently similar to call a match"};
        }
    }

    return result;
};

/**
 * Simpler version of the merge function. Maybe only support this at first.
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

