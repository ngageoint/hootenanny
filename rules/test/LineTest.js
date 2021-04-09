"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Line String Generic Test";
// This matcher only sets match/miss/review values to 1.0, therefore score thresholds aren't used. If that
// ever changes, then custom score threshold configuration options should be added for this matcher.
//exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
//exports.missThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
//exports.reviewThreshold = parseFloat(hoot.get("conflate.match.threshold.default"))
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.line"));
exports.baseFeatureType = "Line";
exports.geometryType = "line";

var angleHistogramExtractor = new hoot.AngleHistogramExtractor();
var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor();
var sublineMatcher = new hoot.MaximalSublineStringMatcher({
    "way.matcher.max.angle": hoot.get("generic.line.matcher.max.angle"),
    "way.subline.matcher": hoot.get("generic.line.subline.matcher")});

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 *
 * exports.matchCandidateCriterion takes precedence over this function and must
 * be commented out before using it.
 */
exports.isMatchCandidate = function(map, e)
{
  return hoot.OsmSchema.isLinear(e);
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
exports.matchScore = function(map, e1, e2) 
{
  var result = { miss: 1.0 };

  // extract the sublines needed for matching
  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);

  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;
    var angleHistogramValue = angleHistogramExtractor.extract(m, m1, m2);
    var weightedShapeDistanceValue = weightedShapeDistanceExtractor.extract(m, m1, m2);

    if (angleHistogramValue > 0.63 && weightedShapeDistanceValue > 0.9)
    {
      result = { match: 1.0 };
    }
  }

  return result;
};

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
  var result = new hoot.LinearSnapMerger().apply(sublineMatcher, map, pairs, replaced, exports.baseFeatureType);
  return result;
};

