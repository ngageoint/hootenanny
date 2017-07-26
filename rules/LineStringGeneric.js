
var MATCH_OVERLAP_THRESHOLD = 0.75;
var MISS_OVERLAP_THRESHOLD = 0.15;

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Generic Line String";
exports.experimental = true;
exports.matchThreshold = parseFloat(hoot.get("generic.line.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("generic.line.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("generic.line.review.threshold"));
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.line"));

var angleHistogramExtractor = new hoot.AngleHistogramExtractor();
var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor();
var distanceScoreExtractor = new hoot.DistanceScoreExtractor();
var lengthScoreExtractor = new hoot.LengthScoreExtractor();
var sublineMatcher = new hoot.MaximalSublineStringMatcher({
    "way.matcher.max.angle": hoot.get("generic.line.matcher.max.angle"),
    "way.subline.matcher": hoot.get("generic.line.subline.matcher")});

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e) {
    return isLinear(e);
};

/**
 * If this function returns true then all overlapping matches will be treated
 * as a group. For now that means if two matches overlap then the whole group
 * will be marked as needing review.
 *
 * If this function returns false the conflation routines will attempt to 
 * pick the best subset of matches that do not conflict.
 */
exports.isWholeGroup = function() {
    return false;
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
exports.matchScore = function(map, e1, e2) {
    var result = { miss: 1.0 };

    if (e1.getStatusString() == e2.getStatusString()) {
        return result;
    }

    // extract the sublines needed for matching
    var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);

    if (sublines)
    {
        var m = sublines.map;
        var m1 = sublines.match1;
        var m2 = sublines.match2;
        var ds = distanceScoreExtractor.extract(m, m1, m2);
        var ps = weightedShapeDistanceExtractor.extract(m, m1, m2);
        var ls = lengthScoreExtractor.extract(m, m1, m2);

        if (ds * ps * ls > 0.4)
        {
//             hoot.log("Found Match!", e1.getTags().get('note'),
//                 e2.getTags().get('note'));
//             hoot.log(angleHistogramValue);
//             hoot.log(weightedShapeDistanceValue);
            result = { match: 1.0 };
        }
    }

    return result;
};

/*exports.mergePair = function(map, e1, e2)
{
    var newTags = mergeTags(e1, e2);
    e1.setTags(newTags);

    removeElement(map, e2);

    return e1;
};*/

/**
 * The internals of geometry merging can become quite complex. Typically this 
 * method will simply call another hoot method to perform the appropriate
 * merging of geometries.
 * 
 * If this method is exported then the mergePair method should not be exported.
 * 
 * @param map The map that is being conflated
 * @param pairs An array of ElementId pairs that will be merged.
 * @param replaced An empty array is passed in, the method should fill the array
 *      with all the replacements that occur during the merge process (e.g. if
 *      two elements (way:1 & way:2) are merged into one element (way:3), then
 *      the replaced array should contain [[way:1, way:3], [way:1, way:3]]
 *      where all the "way:*" objects are of the ElementId type.
 */
exports.mergeSets = function(map, pairs, replaced) 
{
    // snap the ways in the second input to the first input. Use the default tag 
    // merge method.
    var result = snapWays(sublineMatcher, map, pairs, replaced);
    return result;
};

