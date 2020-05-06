/**
 * This script conflates railways using Generic Conflation.
 */

"use strict";

exports.description = "Matches railways";
exports.experimental = false;
exports.baseFeatureType = "Railway";
exports.geometryType = "line";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);

// This matcher only sets match/miss/review values to 1.0, therefore the score thresholds aren't used. 
// If that ever changes, then the generic score threshold configuration options used below should 
// be replaced with custom score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

// This is needed for disabling superfluous conflate ops. In the future, it may also
// be used to replace exports.isMatchCandidate (see #3047).
exports.matchCandidateCriterion = "hoot::RailwayCriterion";

var sublineMatcher =
  new hoot.MaximalSublineStringMatcher(
    { "way.matcher.max.angle": hoot.get("railway.matcher.max.angle"),
      "way.subline.matcher": hoot.get("railway.subline.matcher") });

var distanceScoreExtractor = new hoot.DistanceScoreExtractor();
// Use default spacing, 5 meters
var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor();
var euclideanDistanceExtractor = new hoot.EuclideanDistanceExtractor();
var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor();
var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor();
var parallelScoreExtractor = new hoot.ParallelScoreExtractor();
var lengthScoreExtractor = new hoot.LengthScoreExtractor();

/**
 * Runs before match creation occurs and provides an opportunity to perform custom initialization.
 */
exports.calculateSearchRadius = function(map)
{
  exports.searchRadius = parseFloat(hoot.get("search.radius.railway"));
  hoot.log("Using specified search radius for railway conflation: " + exports.searchRadius);
}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return isRailway(e);
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
  var result = { miss: 1.0, explain:"miss" };

  if (e1.getStatusString() === e2.getStatusString()) {
    return result;
  }

  // extract the sublines needed for matching
  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  if (sublines) {
    //result = { match: 1.0, explain:"match" };
    //return result;

    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    var distanceScore = distanceScoreExtractor.extract(m, m1, m2);
    var edgeDistance  = edgeDistanceExtractor.extract(m, m1, m2);
    var hausdorffDistance = hausdorffDistanceExtractor.extract(m, m1, m2);

    var attribs = [distanceScore, edgeDistance, hausdorffDistance];
    var classification = WekaClassifier.classify(attribs);
    if (0 === classification) {
      hoot.trace("Found Match!");
      result = { match: 1.0, explain:"match" };
    }
  }

  return result;
};

/**
 * The internals of geometry merging can become quite complex. Typically this
 * method will simply call another hoot method to perform the appropriate merging
 * of geometries.
 *
 * If this method is exported then the mergePair method should not be exported.
 *
 * @param map The map that is being conflated
 * @param pairs An array of ElementId pairs that will be merged.
 * @param replaced An empty array is passed in, the method should fill the array
 *      with all the replacements that occur during the merge process (e.g. if two
 *      elements (way:1 & way:2) are merged into one element (way:3), then the
 *      replaced array should contain [[way:1, way:3], [way:1, way:3]] where all
 *      the "way:*" objects are of the ElementId type.
 */
exports.mergeSets = function(map, pairs, replaced)
{
  // snap the ways in the second input to the first input. Use the default tag
  // merge method.
  return snapWays(sublineMatcher, map, pairs, replaced, exports.baseFeatureType);
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var featureDetails = [];

  // extract the sublines needed for matching
  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    featureDetails["weightedShapeDistance"] = weightedShapeDistanceExtractor.extract(m, m1, m2);
    featureDetails["distanceScore"]         = distanceScoreExtractor.extract(m, m1, m2);
    featureDetails["edgeDistance"]          = edgeDistanceExtractor.extract(m, m1, m2);
    featureDetails["euclideanDistance"]     = euclideanDistanceExtractor.extract(m, m1, m2);
    featureDetails["hausdorffDistance"]     = hausdorffDistanceExtractor.extract(m, m1, m2);
    featureDetails["parallelScore"]         = parallelScoreExtractor.extract(m, m1, m2);
    featureDetails["lengthScore"]           = lengthScoreExtractor.extract(m, m1, m2);
  }

  return featureDetails;
};

/*
Classifier derived using WEKA 3.8.2
... And then I tweaked the numbers a bit
Classifier chosen was "REPTree"

Invoke WekaClassifier.classify(i) with:
i[0] = distanceScore
i[1] = edgeDistance
i[2] = hausdorffDistance
*/
class WekaClassifier {

  static classify(i) {
    var p = NaN;
    p = WekaClassifier.N1ddbfdb32(i);
    return p;
  }

  static N1ddbfdb32(i) {
    var p = NaN;
    /* distanceScore */
    if (i[0] == null) {
      p = 1;
    } else if (i[0] < 0.577) {
      p = 1;
    } else if (true) {
    p = WekaClassifier.N4ee8593a3(i);
    }
    return p;
  }
  static N4ee8593a3(i) {
    var p = NaN;
    /* hausdorffDistance */
    if (i[2] == null) {
      p = 1;
    } else if (i[2] < 0.800) {
      p = 1;
    } else if (true) {
    p = WekaClassifier.N296715a14(i);
    }
    return p;
  }
  static N296715a14(i) {
    var p = NaN;
    /* edgeDistance */
    if (i[1] == null) {
      p = 1;
    } else if (i[1] < 0.800) {
      p = 1;
    } else if (true) {
      p = 0;
    }
    return p;
  }
}
