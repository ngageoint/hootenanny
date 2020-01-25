/**
 * This script conflates all linear features using Generic Conflation.
 */

"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches generic lines";
exports.experimental = false;
exports.matchThreshold = parseFloat(hoot.get("generic.line.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("generic.line.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("generic.line.review.threshold"));
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.line"));
exports.tagThreshold = parseFloat(hoot.get("generic.line.tag.threshold"));
exports.baseFeatureType = "Line";
exports.geometryType = "line";

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
 *
 * exports.matchCandidateCriterion takes precedence over this function and must
 * be commented out before using it.
 */
exports.isMatchCandidate = function(map, e)
{
  return isLinear(e) && !isSpecificallyConflatable(map, e);
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

  if (e1.getStatusString() == e2.getStatusString()) 
  {
    return result;
  }

  hoot.trace("e1: " + e1.getId() + ", " + e1.getTags().get("name"));
  if (e1.getTags().get("note"))
  {
    hoot.trace("e1 note: " + e1.getTags().get("note"));
  }
  hoot.trace("e2: " + e2.getId() + ", " + e2.getTags().get("name"));
  if (e2.getTags().get("note"))
  {
    hoot.trace("e2 note: " + e2.getTags().get("note"));
  }

  var typeScore = getTypeScore(map, e1, e2);
  var typeScorePassesThreshold = false;
  if (typeScore >= exports.tagThreshold)
  {
    typeScorePassesThreshold = true;
  }
  hoot.trace("typeScore: " + typeScore);
  hoot.trace("typeScorePassesThreshold: " + typeScorePassesThreshold);

  // extract the sublines needed for matching - Note that this was taken directly from Highway.js.
  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  var distanceScore = -1.0;
  var weightShapeDistanceScore = -1.0;
  var lengthScore = -1.0;
  var geometryMatch = false;
  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;
    distanceScore = distanceScoreExtractor.extract(m, m1, m2);
    weightShapeDistanceScore = weightedShapeDistanceExtractor.extract(m, m1, m2);
    lengthScore = lengthScoreExtractor.extract(m, m1, m2);

    // not sure where this originally came from
    if ((distanceScore * weightShapeDistanceScore * lengthScore) > 0.4)
    {
      geometryMatch = true;
    }
  }
  hoot.trace("distanceScore: " + distanceScore);
  hoot.trace("weightShapeDistanceScore: " + weightShapeDistanceScore);
  hoot.trace("lengthScore: " + lengthScore);
  hoot.trace("geometryMatch: " + geometryMatch);

  var featureMatch = typeScorePassesThreshold && geometryMatch;
  if (featureMatch)
  {
    result = { match: 1.0 };
  }
  hoot.trace("featureMatch: " + featureMatch);

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
  var result = snapWays(sublineMatcher, map, pairs, replaced, exports.baseFeatureType);
  return result;
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var featureDetails = [];
  return featureDetails;
};

