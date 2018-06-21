"use strict";

var MATCH_OVERLAP_THRESHOLD = 0.75;
var MISS_OVERLAP_THRESHOLD = 0.15;

exports.description = "Power Line";
exports.experimental = false;
exports.baseFeatureType = "PowerLine";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("power.line.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("power.line.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("power.line.review.threshold"));

var sublineMatcher =
  new hoot.MaximalSublineStringMatcher(
    { "way.matcher.max.angle": hoot.get("power.line.matcher.max.angle"),
      "way.subline.matcher": hoot.get("power.line.subline.matcher") });

var angleHistogramExtractor = new hoot.AngleHistogramExtractor(); 
var attributeScoreExtractor = new hoot.AttributeScoreExtractor(new hoot.MeanAggregator());
var bufferedOverlapExtractor = new hoot.BufferedOverlapExtractor(); //poly?
var centroidDistanceExtractor = new hoot.CentroidDistanceExtractor(); //poly?
var compactnessExtractor = new hoot.CompactnessExtractor(); //poly?
var distanceScoreExtractor = new hoot.DistanceScoreExtractor(new hoot.MeanAggregator());
var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor(new hoot.MeanAggregator());
var euclideanDistanceExtractor = new hoot.EuclideanDistanceExtractor();
var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor(); //poly?
var lengthScoreExtractor = new hoot.LengthScoreExtractor(new hoot.MeanAggregator());
var soundexExtractor = new hoot.NameExtractor(
    new hoot.Soundex());
var translateMeanWordSetLevenshtein_1_5 = new hoot.NameExtractor(
    new hoot.MeanWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.LevenshteinDistance({"levenshtein.distance.alpha": 1.5})));
var translateMaxWordSetLevenshtein_1_15 = new hoot.NameExtractor(
    new hoot.MaxWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));
var translateMinWordSetLevenshtein_1_15 = new hoot.NameExtractor(
    new hoot.MinSumWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));
var weightedWordDistance = new hoot.NameExtractor(
    new hoot.WeightedWordDistance(
        {"token.separator": "[\\s-,';]+", "weighted.word.distance.probability": 0.5},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.5}))));
var overlapExtractor = new hoot.OverlapExtractor(); //poly?
var parallelScoreExtractor = new hoot.ParallelScoreExtractor();
var sampledAngleHistogramExtractor =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : hoot.get("power.line.angle.sample.distance"),
      "way.matcher.heading.delta" : hoot.get("power.line.matcher.heading.delta") });
var smallerOverlapExtractor = new hoot.SmallerOverlapExtractor(); //poly?
var weightedMetricDistanceExtractor = new hoot.WeightedMetricDistanceExtractor(new hoot.MeanAggregator());
var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor(new hoot.MeanAggregator());

/**
 * Runs before match creation occurs and provides an opportunity to perform custom initialization.
 */
exports.calculateSearchRadius = function(map)
{
  var autoCalcSearchRadius = (hoot.get("power.line.auto.calc.search.radius") === 'true');
  if (autoCalcSearchRadius)
  {
    hoot.log("Calculating search radius for power line conflation...");
    exports.searchRadius =
      parseFloat(
        calculateSearchRadiusUsingRubberSheeting(
          map,
          hoot.get("power.line.rubber.sheet.ref"),
          hoot.get("power.line.rubber.sheet.minimum.ties")));
  }
  else
  {
    exports.searchRadius = parseFloat(hoot.get("search.radius.power.line"));
    hoot.log("Using specified search radius for power line conflation: " + exports.searchRadius);
  }
}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return isPowerLine(e);
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

  if (e1.getStatusString() == e2.getStatusString()) 
  {
    return result;
  }

  // extract the sublines needed for matching
  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    /*var angleHistogramValue = angleHistogramExtractor.extract(m, m1, m2);
    var attributeScoreValue = attributeScoreExtractor.extract(m, m1, m2);
    var bufferedOverlapValue = bufferedOverlapExtractor.extract(m, m1, m2);
    var centroidDistanceValue = centroidDistanceExtractor.extract(m, m1, m2);
    var compactnessValue = compactnessExtractor.extract(m, m1, m2);
    var distanceScoreValue = distanceScoreExtractor.extract(m, m1, m2);
    var edgeDistanceValue = edgeDistanceExtractor.extract(m, m1, m2);
    var euclideanDistanceValue = euclideanDistanceExtractor.extract(m, m1, m2);
    var hausdorffDistanceValue = hausdorffDistanceExtractor.extract(m, m1, m2);
    var lengthScoreValue = lengthScoreExtractor.extract(m, m1, m2);
    var nameValue1 = soundexExtractor.extract(m, m1, m2);
    var nameValue2 = translateMeanWordSetLevenshtein_1_5.extract(m, m1, m2);
    var nameValue3 = translateMaxWordSetLevenshtein_1_15.extract(m, m1, m2);
    var nameValue4 = translateMinWordSetLevenshtein_1_15.extract(m, m1, m2);
    var nameValue5 = weightedWordDistance.extract(m, m1, m2);
    var overlapValue = overlapExtractor.extract(m, m1, m2);
    var parallelScoreValue = parallelScoreExtractor.extract(m, m1, m2);
    var smallerOverlapValue = smallerOverlapExtractor.extract(m, m1, m2);
    var smallerOverlapValue = smallerOverlapExtractor.extract(m, m1, m2);
    var weightedMetricDistanceValue = weightedMetricDistanceExtractor.extract(m, m1, m2);*/

    var sampledAngleHistogramValue = sampledAngleHistogramExtractor.extract(m, m1, m2);
    if (sampledAngleHistogramValue > 0.1)
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

    featureDetails["angleHistogramValue"] = angleHistogramExtractor.extract(m, m1, m2);
    featureDetails["attributeScoreValue"] = attributeScoreExtractor.extract(m, m1, m2);
    featureDetails["bufferedOverlapValue"] = bufferedOverlapExtractor.extract(m, m1, m2);
    featureDetails["centroidDistanceValue"] = centroidDistanceExtractor.extract(m, m1, m2);
    featureDetails["compactnessValue"] = compactnessExtractor.extract(m, m1, m2);
    featureDetails["distanceScoreValue"] = distanceScoreExtractor.extract(m, m1, m2);
    featureDetails["edgeDistanceValue"] = edgeDistanceExtractor.extract(m, m1, m2);
    featureDetails["euclideanDistanceValue"] = euclideanDistanceExtractor.extract(m, m1, m2);
    featureDetails["hausdorffDistanceValue"] = hausdorffDistanceExtractor.extract(m, m1, m2);
    featureDetails["lengthScoreValue"] = lengthScoreExtractor.extract(m, m1, m2);
    featureDetails["nameValue1"] = soundexExtractor.extract(m, m1, m2);
    featureDetails["nameValue2"] = translateMeanWordSetLevenshtein_1_5.extract(m, m1, m2);
    featureDetails["nameValue3"] = translateMaxWordSetLevenshtein_1_15.extract(m, m1, m2);
    featureDetails["nameValue4"] = translateMinWordSetLevenshtein_1_15.extract(m, m1, m2);
    featureDetails["nameValue5"] = weightedWordDistance.extract(m, m1, m2);
    featureDetails["overlapValue"] = overlapExtractor.extract(m, m1, m2);
    featureDetails["parallelScoreValue"] = parallelScoreExtractor.extract(m, m1, m2);
    featureDetails["smallerOverlapValue"] = smallerOverlapExtractor.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramValue"] = sampledAngleHistogramExtractor.extract(m, m1, m2);
    featureDetails["smallerOverlapValue"] = smallerOverlapExtractor.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceValue"] = weightedMetricDistanceExtractor.extract(m, m1, m2); 
    featureDetails["weightedShapeDistanceValue"] = weightedShapeDistanceExtractor.extract(m, m1, m2);
  }

  return featureDetails;
};

