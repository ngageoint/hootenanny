"use strict";

var MATCH_OVERLAP_THRESHOLD = 0.75;
var MISS_OVERLAP_THRESHOLD = 0.15;

exports.description = "Linear Waterway";
exports.experimental = false;
exports.baseFeatureType = "Waterway";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("waterway.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("waterway.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("waterway.review.threshold"));

var sublineMatcher =
  new hoot.MaximalSublineStringMatcher(
    { "way.matcher.max.angle": hoot.get("waterway.matcher.max.angle"),
      "way.subline.matcher": hoot.get("waterway.subline.matcher") });
var sampledAngleHistogramExtractor =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : hoot.get("waterway.angle.sample.distance"),
      "way.matcher.heading.delta" : hoot.get("waterway.matcher.heading.delta") });
var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor();

/**
 * Runs before match creation occurs and provides an opportunity to perform custom initialization.
 */
exports.calculateSearchRadius = function(map)
{
  var autoCalcSearchRadius = (hoot.get("waterway.auto.calc.search.radius") === 'true');
  if (autoCalcSearchRadius)
  {
    hoot.log("Calculating search radius for waterway conflation...");
    exports.searchRadius =
      parseFloat(
        calculateSearchRadiusUsingRubberSheeting(
          map,
          hoot.get("waterway.rubber.sheet.ref"),
          hoot.get("waterway.rubber.sheet.minimum.ties")));
  }
  else
  {
    exports.searchRadius = parseFloat(hoot.get("search.radius.waterway"));
    hoot.log("Using specified search radius for waterway conflation: " + exports.searchRadius);
  }
}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return isLinearWaterway(e);
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

  // extract the sublines needed for matching
  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    var sampledAngleHistogramValue = sampledAngleHistogramExtractor.extract(m, m1, m2);
    var weightedShapeDistanceValue = weightedShapeDistanceExtractor.extract(m, m1, m2);

    if (sampledAngleHistogramValue == 0 && weightedShapeDistanceValue > 0.861844)
    {
      hoot.trace("Found Match!");
      result = { match: 1.0, explain:"match" };
    }
    else if (sampledAngleHistogramValue > 0)
    {
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
  hoot.trace("Merging elements.");
  // snap the ways in the second input to the first input. Use the default tag
  // merge method.
  return snapWays(sublineMatcher, map, pairs, replaced);
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

    featureDetails["sampledAngleHistogramValue"] = sampledAngleHistogramExtractor.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceValue"] = weightedShapeDistanceExtractor.extract(m, m1, m2);
  }

  return featureDetails;
};

