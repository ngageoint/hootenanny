/**
 * This script conflates all linear features using Generic Conflation.
 */

"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches generic lines";
exports.experimental = false;

// This matcher only sets match/miss/review values to 1.0, therefore the score thresholds aren't used. 
// If that ever changes, then the generic score threshold configuration options used below should 
// be replaced with custom score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

exports.searchRadius = parseFloat(hoot.get("search.radius.generic.line"));
exports.tagThreshold = parseFloat(hoot.get("generic.line.tag.threshold"));
exports.baseFeatureType = "Line";
exports.geometryType = "line";

// This is needed for disabling superfluous conflate ops. In the future, it may also
// be used to replace exports.isMatchCandidate (see #3047).
exports.matchCandidateCriterion = "hoot::LinearCriterion";

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
exports.isMatchCandidate = function(map, e)
{
  hoot.trace("e: " + e.getElementId());
  // Even though a route relation passes the linear crit, we want only highway or rail
  // conflation to conflate it.
  if (e.getElementId().getType() == "Relation" && e.getTags().contains("route"))
  {
    return false;
  }
  hoot.trace("isLinear: " + isLinear(map, e));
  hoot.trace("isSpecificallyConflatable: " + isSpecificallyConflatable(map, e, exports.geometryType));
  return isLinear(map, e) && !isSpecificallyConflatable(map, e, exports.geometryType);
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

  //hoot.trace("e1: " + e1.getElementId() + ", " + e1.getTags().get("name"));
  hoot.trace("e1: " + e1);
  if (e1.getTags().get("note"))
  {
    hoot.trace("e1 note: " + e1.getTags().get("note"));
  }
  //hoot.trace("e2: " + e2.getElementId() + ", " + e2.getTags().get("name"));
  hoot.trace("e2: " + e2);
  if (e2.getTags().get("note"))
  {
    hoot.trace("e2 note: " + e2.getTags().get("note"));
  }

  // If both features have types and they aren't just generic types, let's do a detailed type comparison and 
  // look for an explicit type mismatch. Otherwise, move on to the geometry comparison.
  var typeScorePassesThreshold = !explicitTypeMismatch(e1, e2, exports.tagThreshold);
  hoot.trace("typeScorePassesThreshold: " + typeScorePassesThreshold);
  if (!typeScorePassesThreshold)
  {
    return result;
  }

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
  // Snap the ways in the second input to the first input. Use the default tag 
  // merge method.
  return snapWays(sublineMatcher, map, pairs, replaced, exports.baseFeatureType);
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var featureDetails = [];
  return featureDetails;
};

