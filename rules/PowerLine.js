"use strict";

var MATCH_OVERLAP_THRESHOLD = 0.75;
var MISS_OVERLAP_THRESHOLD = 0.15;

exports.description = "Power Line";
exports.experimental = false;
exports.baseFeatureType = "PowerLine";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold"));

var sublineMatcher =
  new hoot.MaximalSublineStringMatcher(
    { "way.matcher.max.angle": hoot.get("way.matcher.max.angle"),
      "way.subline.matcher": hoot.get("power.line.subline.matcher") });

var angleHistogramExtractor = 
  new hoot.AngleHistogramExtractor({"angle.histogram.extractor.bins": 16, "angle.histogram.extractor.smoothing": 0.0}); //default

var distanceScoreExtractor = new hoot.DistanceScoreExtractor(new hoot.MeanAggregator());

var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor(new hoot.MeanAggregator(), {"edge.distance.extractor.spacing": 5.0}); //default

var euclideanDistanceExtractor = new hoot.EuclideanDistanceExtractor();

var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor();

var lengthScoreExtractor = new hoot.LengthScoreExtractor(new hoot.MeanAggregator());

var nameExtractor = new hoot.NameExtractor();
var translateMeanWordSetLevenshtein_1_5 = 
  new hoot.NameExtractor(
    new hoot.MeanWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.LevenshteinDistance({"levenshtein.distance.alpha": 1.5})));
var translateMaxWordSetLevenshtein_1_15 = 
  new hoot.NameExtractor(
    new hoot.MaxWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));
var translateMinWordSetLevenshtein_1_15 = 
  new hoot.NameExtractor(
    new hoot.MinSumWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.TranslateStringDistance(
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));
var weightedWordDistance = 
  new hoot.NameExtractor(
    new hoot.WeightedWordDistance(
        {"token.separator": "[\\s-,';]+", "weighted.word.distance.probability": 0.5},
        new hoot.TranslateStringDistance(
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.5}))));

var parallelScoreExtractor = new hoot.ParallelScoreExtractor();

var sampledAngleHistogramExtractor =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 20.0,
      "way.matcher.heading.delta" : 150.0 });

var weightedMetricDistanceExtractor = 
  new hoot.WeightedMetricDistanceExtractor(
    new hoot.MeanAggregator(),
    {"weighted.metric.distance.extractor.point.aggregator": "hoot::MinAggregator", "weighted.metric.distance.extractor.search.radius": -1.0});

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
    exports.searchRadius = parseFloat(hoot.get("search.radius.default"));
    hoot.debug("Using specified search radius for power line conflation: " + exports.searchRadius);
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

    var angleHistogramValue = angleHistogramExtractor.extract(m, m1, m2);
    var distanceScoreValue = distanceScoreExtractor.extract(m, m1, m2);
    var edgeDistanceValue = edgeDistanceExtractor.extract(m, m1, m2);
    var lengthScoreValue = lengthScoreExtractor.extract(m, m1, m2);
    //var sampledAngleHistogramValue = sampledAngleHistogramExtractor.extract(m, m1, m2);
    var weightedMetricDistanceValue = weightedMetricDistanceExtractor.extract(m, m1, m2);
    var weightedWordDistanceValue = weightedWordDistance.extract(m, m1, m2);

    //J48
    
    if (edgeDistanceValue > 0.992042 && lengthScoreValue > 0.998201 && distanceScoreValue > 0.999915)
    {
      result = { match: 1.0, explain:"match" };
    }

    /*if (edgeDistanceValue > 0.992042 && lengthScoreValue > 0.998201)
    {
      if (distanceScoreValue > 0.999915)
      {
        result = { match: 1.0, explain:"match" };
      }
      else if (lengthScoreValue <= 0.999702 && angleHistogramValue > 0.999492)
      {
        result = { match: 1.0, explain:"match" };
      }
      else if (lengthScoreValue > 0.999702)
      {
        if (distanceScoreValue > 0.846607)
        {
          result = { match: 1.0, explain:"match" };
        }
        else if (lengthScoreValue > 0.999826)
        {
          if (distanceScoreValue <= 0.60615)
          {
            result = { review: 1.0, explain:"review" };
          }
          else if (distanceScoreValue > 0.699304)
          {
            result = { match: 1.0, explain:"match" };
          }
        }
      }
    }*/

    /*if (edgeDistanceValue > 0.992042 && lengthScoreValue > 0.998201)
    {
      if (distanceScoreValue > 0.999915)
      {
        result = { match: 1.0, explain:"match" };
      }
      else if (lengthScoreValue > 0.999702)
      {
        if (weightedMetricDistanceValue <= 0.000034 && edgeDistanceValue > 0.999959)
        {
          result = { match: 1.0, explain:"match" };
        }
        else if (weightedMetricDistanceValue > 0.000034 && angleHistogramValue <= 0.999752)
        {
          result = { review: 1.0, explain:"review" };
        }
      }
    }*/
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

    featureDetails["angleHistogramExtractor"] = angleHistogramExtractor.extract(m, m1, m2);
    featureDetails["distanceScoreExtractor"] = distanceScoreExtractor.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor"] = edgeDistanceExtractor.extract(m, m1, m2);
    featureDetails["euclideanDistanceExtractor"] = euclideanDistanceExtractor.extract(m, m1, m2);
    featureDetails["hausdorffDistanceExtractor"] = hausdorffDistanceExtractor.extract(m, m1, m2);
    featureDetails["lengthScoreExtractor"] = lengthScoreExtractor.extract(m, m1, m2);
    featureDetails["parallelScoreExtractor"] = parallelScoreExtractor.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor"] = sampledAngleHistogramExtractor.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor"] = weightedMetricDistanceExtractor.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceExtractor"] = weightedShapeDistanceExtractor.extract(m, m1, m2);
    featureDetails["weightedWordDistance"] = weightedWordDistance.extract(m, m1, m2);
  }

  return featureDetails;
};

