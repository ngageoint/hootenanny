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
    { "way.matcher.max.angle": hoot.get("power.line.matcher.max.angle"),
      "way.subline.matcher": hoot.get("power.line.subline.matcher") });

var distanceWeightCoeff = parseFloat(hoot.get("power.line.distance.weight.coefficient")) * -1.0;
var distanceScoreExtractor = new hoot.DistanceScoreExtractor();

var angleHistogramExtractor5 = new hoot.AngleHistogramExtractor({"angle.histogram.extractor.smoothing": 1.57});

var centroidDistanceExtractor = new hoot.CentroidDistanceExtractor();

var distanceScoreExtractor7 = new hoot.DistanceScoreExtractor(new hoot.SigmaAggregator());

var edgeDistanceExtractor1 = new hoot.EdgeDistanceExtractor(new hoot.MeanAggregator());

var euclideanDistanceExtractor = new hoot.EuclideanDistanceExtractor();

var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor();

var lengthScoreExtractor1 = new hoot.LengthScoreExtractor(new hoot.MeanAggregator());
var lengthScoreExtractor7 = new hoot.LengthScoreExtractor(new hoot.SigmaAggregator());

var nameExtractor = new hoot.NameExtractor();
var soundexExtractor = new hoot.NameExtractor(new hoot.Soundex());
var translateMinWordSetLevenshtein_1_15 = 
  new hoot.NameExtractor(
    new hoot.MinSumWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));;

var parallelScoreExtractor = new hoot.ParallelScoreExtractor();

var weightedMetricDistanceExtractor1 = new hoot.WeightedMetricDistanceExtractor(new hoot.MeanAggregator());

var weightedShapeDistanceExtractor1 = new hoot.WeightedShapeDistanceExtractor(new hoot.MeanAggregator());
var weightedShapeDistanceExtractor7 = new hoot.WeightedShapeDistanceExtractor(new hoot.SigmaAggregator());

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
          hoot.get("rubber.sheet.ref"),
          hoot.get("rubber.sheet.minimum.ties")));
  }
  else
  {
    exports.searchRadius = parseFloat(hoot.get("search.radius.default"));
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

  hoot.trace("e1: " + e1.getTags());
  hoot.trace("e2: " + e2.getTags());

  if (e1.getStatusString() == e2.getStatusString()) 
  {
    return result;
  }

  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    var centroidDistanceExtractorVal = centroidDistanceExtractor.extract(m, m1, m2);
    hoot.trace("centroidDistanceExtractorVal: " + centroidDistanceExtractorVal);
    var angleHistogramExtractor5Val = angleHistogramExtractor5.extract(m, m1, m2);
    hoot.trace("angleHistogramExtractor5Val: " + angleHistogramExtractor5Val);
    var distanceScoreExtractor7Val = distanceScoreExtractor7.extract(m, m1, m2);
    hoot.trace("distanceScoreExtractor7Val: " + distanceScoreExtractor7Val);
    var edgeDistanceExtractor1Val = edgeDistanceExtractor1.extract(m, m1, m2);
    hoot.trace("edgeDistanceExtractor1Val: " + edgeDistanceExtractor1Val);
    var euclideanDistanceExtractorVal = euclideanDistanceExtractor.extract(m, m1, m2);
    hoot.trace("euclideanDistanceExtractorVal: " + euclideanDistanceExtractorVal);
    var hausdorffDistanceExtractorVal = hausdorffDistanceExtractor.extract(m, m1, m2);
    hoot.trace("hausdorffDistanceExtractorVal: " + hausdorffDistanceExtractorVal);
    var lengthScoreExtractor1Val = lengthScoreExtractor1.extract(m, m1, m2);
    hoot.trace("lengthScoreExtractor1Val: " + lengthScoreExtractor1Val);
    var lengthScoreExtractor7Val = lengthScoreExtractor7.extract(m, m1, m2);
    hoot.trace("lengthScoreExtractor7Val: " + lengthScoreExtractor7Val);
    //var nameScoreExtractorVal = nameExtractor.extract(m, m1, m2);
    //hoot.trace("nameScoreExtractorVal: " + nameScoreExtractorVal);
    var parallelScoreExtractorVal = parallelScoreExtractor.extract(m, m1, m2);
    hoot.trace("parallelScoreExtractorVal: " + parallelScoreExtractorVal);
    //var soundexExtractorVal = soundexExtractor.extract(m, m1, m2);
    //hoot.trace("soundexExtractorVal: " + soundexExtractorVal);
    //var translateMinWordSetLevenshtein_1_15Val = translateMinWordSetLevenshtein_1_15.extract(m, m1, m2);
    //hoot.trace("translateMinWordSetLevenshtein_1_15Val: " + translateMinWordSetLevenshtein_1_15Val);
    var weightedMetricDistanceExtractor1Val = weightedMetricDistanceExtractor1.extract(m, m1, m2);
    hoot.trace("weightedMetricDistanceExtractor1Val: " + weightedMetricDistanceExtractor1Val);
    var weightedShapeDistanceExtractor1Val = weightedShapeDistanceExtractor1.extract(m, m1, m2);
    hoot.trace("weightedShapeDistanceExtractor1Val: " + weightedShapeDistanceExtractor1Val);
    var weightedShapeDistanceExtractor7Val = weightedShapeDistanceExtractor7.extract(m, m1, m2); 
    hoot.trace("weightedShapeDistanceExtractor7Val: " + weightedShapeDistanceExtractor7Val);

    if (/*angleHistogramExtractor5Val > 0.95 &&*/ centroidDistanceExtractorVal > 0.61 && /*distanceScoreExtractor7Val < 0.552 &&
        edgeDistanceExtractor1Val > 0.64 && euclideanDistanceExtractorVal > 0.64 && hausdorffDistanceExtractorVal > 0.14 &&
        lengthScoreExtractor1Val > 0.41 && lengthScoreExtractor7Val < 0.155 && nameScoreExtractorVal < 0.451 &&
        parallelScoreExtractorVal > 0.76 && soundexExtractorVal < 0.75 && translateMinWordSetLevenshtein_1_15Val <= 0.5 &&*/ 
        weightedMetricDistanceExtractor1Val < /*0.59*/1.20 /*&& weightedShapeDistanceExtractor1Val > 0.64 &&
        weightedShapeDistanceExtractor7Val < 0.49*/)
    {
      //So far, voltage and location (underground vs overhead) seem to be the most consistent tags to disambiguate matches.  We'll 
      //review when features match and those tags disagree, and we'll ignore them if the values for either of them are unpopulated.

      var voltageStr1 = String(e1.getTags().get("voltage")).trim();
      var voltageStr2 = String(e2.getTags().get("voltage")).trim();
      if (voltageStr1 !== null && voltageStr1 !== '' && voltageStr2 !== null && voltageStr2 !== '')
      {
        var voltage1 = parseInt(voltageStr1);
        var voltage2 = parseInt(voltageStr2);
        if (!isNaN(voltage1) && !isNaN(voltage2) && voltage1 != voltage2)
        {
          hoot.trace("Explicit voltage mismatch between matching features: " + voltage1 + " " + voltage2);
          result = { review: 1.0, explain:"review" };
          return result;
        }
      }

      var location1 = String(e1.getTags().get("location")).trim();
      var location2 = String(e2.getTags().get("location")).trim();
      if (location1 !== 'undefined' && location1 !== null && location1 !== '' && 
          location2 !== 'undefined' && location2 !== null && location2 !== '' && location1 !== location2)
      {
        hoot.trace("Explicit location mismatch between matching features: " + location1 + " " + location2);
        result = { review: 1.0, explain:"review" };
        return result;
      }

      //use distance weighting to slightly favor features that are closer together
      var distanceScoreValue = distanceScoreExtractor.extract(m, m1, m2);
      var delta = (1.0 - distanceScoreValue) * distanceWeightCoeff;
      result.match = 1.0 + delta;
      result.miss = 0.0 - delta;

      return result;
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

    //featureDetails["angleHistogramExtractor1"] = angleHistogramExtractor1.extract(m, m1, m2);
  }

  return featureDetails;
};

