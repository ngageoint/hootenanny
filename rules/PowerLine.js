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

var angleHistogramExtractor1 = new hoot.AngleHistogramExtractor();
var angleHistogramExtractor2 = new hoot.AngleHistogramExtractor({"angle.histogram.extractor.bins": 8});
var angleHistogramExtractor3 = new hoot.AngleHistogramExtractor({"angle.histogram.extractor.bins": 32});
var angleHistogramExtractor4 = new hoot.AngleHistogramExtractor({"angle.histogram.extractor.smoothing": 0.09});
var angleHistogramExtractor5 = new hoot.AngleHistogramExtractor({"angle.histogram.extractor.smoothing": 1.57});

var attributeScoreExtractor1 = new hoot.AttributeScoreExtractor(new hoot.MeanAggregator());
var attributeScoreExtractor2 = new hoot.AttributeScoreExtractor({"attribute.score.extractor.use.weight": "true"});
var attributeScoreExtractor3 = new hoot.AttributeScoreExtractor(new hoot.MaxAggregator());
var attributeScoreExtractor4 = new hoot.AttributeScoreExtractor(new hoot.MaxAggregator(), {"attribute.score.extractor.use.weight": "true"});
var attributeScoreExtractor5 = new hoot.AttributeScoreExtractor(new hoot.MinAggregator());
var attributeScoreExtractor6 = new hoot.AttributeScoreExtractor(new hoot.MinAggregator(), {"attribute.score.extractor.use.weight": "true"});
var attributeScoreExtractor7 = new hoot.AttributeScoreExtractor(new hoot.QuantileAggregator());
var attributeScoreExtractor8 = new hoot.AttributeScoreExtractor(new hoot.QuantileAggregator(), {"attribute.score.extractor.use.weight": "true"});
var attributeScoreExtractor9 = new hoot.AttributeScoreExtractor(new hoot.QuantileAggregator({"quantile.aggregator.quantile": 0.5}));
var attributeScoreExtractor10 = new hoot.AttributeScoreExtractor(new hoot.RmseAggregator());
var attributeScoreExtractor11 = new hoot.AttributeScoreExtractor(new hoot.RmseAggregator(), {"attribute.score.extractor.use.weight": "true"});
var attributeScoreExtractor12 = new hoot.AttributeScoreExtractor(new hoot.SigmaAggregator());
var attributeScoreExtractor13 = new hoot.AttributeScoreExtractor(new hoot.SigmaAggregator(), {"attribute.score.extractor.use.weight": "true"});

var bufferedOverlapExtractor1 = new hoot.BufferedOverlapExtractor();
var bufferedOverlapExtractor2 = new hoot.BufferedOverlapExtractor({"buffered.overlap.extractor.portion": 0.2});

var centroidDistanceExtractor = new hoot.CentroidDistanceExtractor();

var compactnessExtractor = new hoot.CompactnessExtractor();

var distanceScoreExtractor1 = new hoot.DistanceScoreExtractor(new hoot.MeanAggregator());
var distanceScoreExtractor2 = new hoot.DistanceScoreExtractor(new hoot.MaxAggregator());
var distanceScoreExtractor3 = new hoot.DistanceScoreExtractor(new hoot.MinAggregator());
var distanceScoreExtractor4 = new hoot.DistanceScoreExtractor(new hoot.QuantileAggregator());
var distanceScoreExtractor5 = new hoot.DistanceScoreExtractor(new hoot.QuantileAggregator({"quantile.aggregator.quantile": 0.5}));
var distanceScoreExtractor6 = new hoot.DistanceScoreExtractor(new hoot.RmseAggregator());
var distanceScoreExtractor7 = new hoot.DistanceScoreExtractor(new hoot.SigmaAggregator());

var edgeDistanceExtractor1 = new hoot.EdgeDistanceExtractor(new hoot.MeanAggregator());
var edgeDistanceExtractor2 = new hoot.EdgeDistanceExtractor(new hoot.MeanAggregator(), {"edge.distance.extractor.spacing": 10.0});
var edgeDistanceExtractor3 = new hoot.EdgeDistanceExtractor(new hoot.MaxAggregator());
var edgeDistanceExtractor4 = new hoot.EdgeDistanceExtractor(new hoot.MaxAggregator(), {"edge.distance.extractor.spacing": 10.0});
var edgeDistanceExtractor5 = new hoot.EdgeDistanceExtractor(new hoot.MinAggregator());
var edgeDistanceExtractor6 = new hoot.EdgeDistanceExtractor(new hoot.MinAggregator(), {"edge.distance.extractor.spacing": 10.0});
var edgeDistanceExtractor7 = new hoot.EdgeDistanceExtractor(new hoot.QuantileAggregator());
var edgeDistanceExtractor8 = new hoot.EdgeDistanceExtractor(new hoot.QuantileAggregator({"quantile.aggregator.quantile": 0.5}));
var edgeDistanceExtractor9 = new hoot.EdgeDistanceExtractor(new hoot.QuantileAggregator(), {"edge.distance.extractor.spacing": 10.0});
var edgeDistanceExtractor10 = new hoot.EdgeDistanceExtractor(new hoot.RmseAggregator());
var edgeDistanceExtractor11 = new hoot.EdgeDistanceExtractor(new hoot.RmseAggregator(), {"edge.distance.extractor.spacing": 10.0});
var edgeDistanceExtractor12 = new hoot.EdgeDistanceExtractor(new hoot.SigmaAggregator());
var edgeDistanceExtractor13 = new hoot.EdgeDistanceExtractor(new hoot.SigmaAggregator(), {"edge.distance.extractor.spacing": 10.0});

var euclideanDistanceExtractor = new hoot.EuclideanDistanceExtractor();

var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor();

var lengthScoreExtractor1 = new hoot.LengthScoreExtractor(new hoot.MeanAggregator());
var lengthScoreExtractor2 = new hoot.LengthScoreExtractor(new hoot.MaxAggregator());
var lengthScoreExtractor3 = new hoot.LengthScoreExtractor(new hoot.MinAggregator());
var lengthScoreExtractor4 = new hoot.LengthScoreExtractor(new hoot.QuantileAggregator());
var lengthScoreExtractor5 = new hoot.LengthScoreExtractor(new hoot.QuantileAggregator({"quantile.aggregator.quantile": 0.5}));
var lengthScoreExtractor6 = new hoot.LengthScoreExtractor(new hoot.RmseAggregator());
var lengthScoreExtractor7 = new hoot.LengthScoreExtractor(new hoot.SigmaAggregator());

var nameExtractor = new hoot.NameExtractor();
var soundexExtractor = new hoot.NameExtractor(
    new hoot.Soundex());
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
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));
var weightedWordDistance = 
  new hoot.NameExtractor(
    new hoot.WeightedWordDistance(
        {"token.separator": "[\\s-,';]+", "weighted.word.distance.probability": 0.5},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.5}))));

var overlapExtractor = new hoot.OverlapExtractor();

var parallelScoreExtractor = new hoot.ParallelScoreExtractor();

var sampledAngleHistogramExtractor1 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 10.0,
      "way.matcher.heading.delta" : 5.0 });
var sampledAngleHistogramExtractor2 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 25.0,
      "way.matcher.heading.delta" : 5.0 });
var sampledAngleHistogramExtractor3 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 50.0,
      "way.matcher.heading.delta" : 5.0 });
var sampledAngleHistogramExtractor4 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 100.0,
      "way.matcher.heading.delta" : 5.0 });
var sampledAngleHistogramExtractor5 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 150.0,
      "way.matcher.heading.delta" : 5.0 });
var sampledAngleHistogramExtractor6 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 10.0,
      "way.matcher.heading.delta" : 10.0 });
var sampledAngleHistogramExtractor7 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 10.0,
      "way.matcher.heading.delta" : 25.0 });
var sampledAngleHistogramExtractor8 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 10.0,
      "way.matcher.heading.delta" : 50.0 });
var sampledAngleHistogramExtractor9 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 10.0,
      "way.matcher.heading.delta" : 90.0 });
var sampledAngleHistogramExtractor10 =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : 20.0,
      "way.matcher.heading.delta" : 150.0 });

var smallerOverlapExtractor = new hoot.SmallerOverlapExtractor();

var weightedMetricDistanceExtractor1 = new hoot.WeightedMetricDistanceExtractor(new hoot.MeanAggregator());
var weightedMetricDistanceExtractor2 = new hoot.WeightedMetricDistanceExtractor(new hoot.MaxAggregator());
var weightedMetricDistanceExtractor3 = new hoot.WeightedMetricDistanceExtractor(new hoot.MinAggregator());
var weightedMetricDistanceExtractor4 = new hoot.WeightedMetricDistanceExtractor(new hoot.QuantileAggregator());
var weightedMetricDistanceExtractor5 = new hoot.WeightedMetricDistanceExtractor(new hoot.QuantileAggregator({"quantile.aggregator.quantile": 0.5}));
var weightedMetricDistanceExtractor6 = new hoot.WeightedMetricDistanceExtractor(new hoot.RmseAggregator());
var weightedMetricDistanceExtractor7 = new hoot.WeightedMetricDistanceExtractor(new hoot.SigmaAggregator());
var weightedMetricDistanceExtractor8 = 
  new hoot.WeightedMetricDistanceExtractor(
    new hoot.MeanAggregator(), {"weighted.metric.distance.extractor.point.aggregator": "hoot::MaxAggregator"});
var weightedMetricDistanceExtractor9 = 
  new hoot.WeightedMetricDistanceExtractor(
    new hoot.MeanAggregator(), {"weighted.metric.distance.extractor.point.aggregator": "hoot::MinAggregator"});
var weightedMetricDistanceExtractor10 = 
  new hoot.WeightedMetricDistanceExtractor(
    new hoot.MeanAggregator(), {"weighted.metric.distance.extractor.point.aggregator": "hoot::QuantileAggregator"});
var weightedMetricDistanceExtractor11 = 
  new hoot.WeightedMetricDistanceExtractor(
    new hoot.MeanAggregator(), {"weighted.metric.distance.extractor.point.aggregator": "hoot::QuantileAggregator", "quantile.aggregator.quantile": 0.5});
var weightedMetricDistanceExtractor12 = 
  new hoot.WeightedMetricDistanceExtractor(
    new hoot.MeanAggregator(), {"weighted.metric.distance.extractor.point.aggregator": "hoot::RmseAggregator"});
var weightedMetricDistanceExtractor13 = 
  new hoot.WeightedMetricDistanceExtractor(
    new hoot.MeanAggregator(), {"weighted.metric.distance.extractor.point.aggregator": "hoot::SigmaAggregator"});
var weightedMetricDistanceExtractor14 = new hoot.WeightedMetricDistanceExtractor(new hoot.MeanAggregator(), {"weighted.metric.distance.extractor.search.radius": 25.0});
var weightedMetricDistanceExtractor15 = new hoot.WeightedMetricDistanceExtractor(new hoot.MaxAggregator(), {"weighted.metric.distance.extractor.search.radius": 25.0});
var weightedMetricDistanceExtractor16 = new hoot.WeightedMetricDistanceExtractor(new hoot.MinAggregator(), {"weighted.metric.distance.extractor.search.radius": 25.0});
var weightedMetricDistanceExtractor17 = new hoot.WeightedMetricDistanceExtractor(new hoot.QuantileAggregator(), {"weighted.metric.distance.extractor.search.radius": 25.0});
var weightedMetricDistanceExtractor18 = new hoot.WeightedMetricDistanceExtractor(new hoot.QuantileAggregator({"quantile.aggregator.quantile": 0.5}), {"weighted.metric.distance.extractor.search.radius": 25.0});
var weightedMetricDistanceExtractor19 = new hoot.WeightedMetricDistanceExtractor(new hoot.RmseAggregator(), {"weighted.metric.distance.extractor.search.radius": 25.0});
var weightedMetricDistanceExtractor20 = new hoot.WeightedMetricDistanceExtractor(new hoot.SigmaAggregator(), {"weighted.metric.distance.extractor.search.radius": 25.0});

var weightedShapeDistanceExtractor1 = new hoot.WeightedShapeDistanceExtractor(new hoot.MeanAggregator());
var weightedShapeDistanceExtractor2 = new hoot.WeightedShapeDistanceExtractor(new hoot.MaxAggregator());
var weightedShapeDistanceExtractor3 = new hoot.WeightedShapeDistanceExtractor(new hoot.MinAggregator());
var weightedShapeDistanceExtractor4 = new hoot.WeightedShapeDistanceExtractor(new hoot.QuantileAggregator());
var weightedShapeDistanceExtractor5 = new hoot.WeightedShapeDistanceExtractor(new hoot.QuantileAggregator({"quantile.aggregator.quantile": 0.5}));
var weightedShapeDistanceExtractor6 = new hoot.WeightedShapeDistanceExtractor(new hoot.RmseAggregator());
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
          hoot.get("power.line.rubber.sheet.ref"),
          hoot.get("power.line.rubber.sheet.minimum.ties")));
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

    //var sampledAngleHistogramValue = sampledAngleHistogramExtractor1.extract(m, m1, m2);
    //if (sampledAngleHistogramValue > 0.1)
    //{
      //result = { match: 1.0, explain:"match" };
    //}

    //var edgeDistanceExtractor8Val = edgeDistanceExtractor8.extract(m, m1, m2);
    //var weightedMetricDistanceExtractor10Val = weightedMetricDistanceExtractor10.extract(m, m1, m2);
    //var lengthScoreExtractor2Val = lengthScoreExtractor2.extract(m, m1, m2);
    //var weightedMetricDistanceExtractor16Val = weightedMetricDistanceExtractor16.extract(m, m1, m2);
    //var sampledAngleHistogramExtractor9Val = sampledAngleHistogramExtractor9.extract(m, m1, m2);
    //var weightedMetricDistanceExtractor8Val = weightedMetricDistanceExtractor8.extract(m, m1, m2);
    //var weightedMetricDistanceExtractor12Val = weightedMetricDistanceExtractor12.extract(m, m1, m2);
    //var edgeDistanceExtractor4Val = edgeDistanceExtractor4.extract(m, m1, m2);
    //var lengthScoreExtractor3Val = lengthScoreExtractor3.extract(m, m1, m2);
    //var weightedMetricDistanceExtractor9Val = weightedMetricDistanceExtractor9.extract(m, m1, m2);
    //var weightedWordDistanceVal = weightedWordDistance.extract(m, m1, m2);
    //var lengthScoreExtractor5Val = lengthScoreExtractor5.extract(m, m1, m2);

    var centroidDistanceExtractorVal = centroidDistanceExtractor.extract(m, m1, m2);
    var angleHistogramExtractor5Val = angleHistogramExtractor5.extract(m, m1, m2);
    var distanceScoreExtractor7Val = distanceScoreExtractor7.extract(m, m1, m2);
    var edgeDistanceExtractor1Val = edgeDistanceExtractor1.extract(m, m1, m2);
    var euclideanDistanceExtractorVal = euclideanDistanceExtractor.extract(m, m1, m2);
    var hausdorffDistanceExtractorVal = hausdorffDistanceExtractor.extract(m, m1, m2);
    var lengthScoreExtractor1Val = lengthScoreExtractor1.extract(m, m1, m2);
    var lengthScoreExtractor7Val = lengthScoreExtractor7.extract(m, m1, m2);
    var nameScoreExtractorVal = nameExtractor.extract(m, m1, m2);
    var parallelScoreExtractorVal = parallelScoreExtractor.extract(m, m1, m2);
    var soundexExtractorVal = soundexExtractor.extract(m, m1, m2);
    var translateMinWordSetLevenshtein_1_15Val = translateMinWordSetLevenshtein_1_15.extract(m, m1, m2);
    var weightedMetricDistanceExtractor1Val = weightedMetricDistanceExtractor1.extract(m, m1, m2);
    var weightedShapeDistanceExtractor1Val = weightedShapeDistanceExtractor1.extract(m, m1, m2);
    var weightedShapeDistanceExtractor7Val = weightedShapeDistanceExtractor7.extract(m, m1, m2);

    //#1
    //1 - no change
    //2 - 0.1
    //3 - .02
    //4 - .094
    /*if (edgeDistanceExtractor8Val > 0.992155 && weightedMetricDistanceExtractor10Val <= 0.500659 && lengthScoreExtractor2Val > 0.998201)
    {
      if (weightedMetricDistanceExtractor16Val <= 0.000045)
      {
        result = { match: 1.0, explain:"match" };
        return result;
      }
      else
      {
        if (lengthScoreExtractor2Val <= 0.999702)
        {
          if (sampledAngleHistogramExtractor9Val <= 0.991983)
          {
            if (weightedMetricDistanceExtractor10Val <= 0.000061)
            {
              result = { match: 1.0, explain:"match" };
              return result;
            }
            else if (weightedMetricDistanceExtractor8Val > 1.60789)
            {
              result = { match: 1.0, explain:"match" };
              return result;
            }
          } 
        }
        else
        {
          if (centroidDistanceExtractorVal <= 0.999946)
          {
            if (weightedMetricDistanceExtractor12Val > 0.57756)
            {
              result = { review: 1.0, explain:"review" };
              return result;
            }
          }
          else
          {
            if (edgeDistanceExtractor4Val <= 0.999938)
            {
              if (weightedMetricDistanceExtractor10Val <= 0.000034)
              {
                if (lengthScoreExtractor3Val > 0.999802)
                {
                  result = { match: 1.0, explain:"match" };
                  return result;
                }
              }
              else
              {
                result = { review: 1.0, explain:"review" };
                return result;
              }
            }
            else
            {
              result = { match: 1.0, explain:"match" };
              return result;
            }
          }
        }
      }*/

      //#2
      //1 - 0.72
      //2 - .11
      //3 - .04
      //4 - .118
      /*if (centroidDistanceExtractorVal > 0.974276 && weightedMetricDistanceExtractor9Val <= 0.785178 && lengthScoreExtractor2Val > 0.998201)
      {
        if (edgeDistanceExtractor4Val <= 0.999938)
        {
          if (weightedWordDistanceVal <= 0.066098)
          {
            if (lengthScoreExtractor2Val <= 0.999702)
            {
              if (sampledAngleHistogramExtractor9Val <= 0.991983)
              {
                result = { match: 1.0, explain:"match" };
                return result;
              }
            }
            else
            {
              if (centroidDistanceExtractorVal <= 0.999946)
              {
                result = { review: 1.0, explain:"review" };
                return result;
              }
              else if (weightedMetricDistanceExtractor9Val <= 0.000101)
              {
                result = { match: 1.0, explain:"match" };
                return result;
              }
            }
          }
        }
        else if (lengthScoreExtractor2Val > 0.99897)
        {
          result = { match: 1.0, explain:"match" };
          return result;
        }
      }*/

      //#3
      //1 - .704
      //2 - .05
      //3 - .03
      //4 - .10
      /*if (centroidDistanceExtractorVal > 0.974276 && lengthScoreExtractor2Val > 0.998201)
      {
        if (edgeDistanceExtractor4Val <= 0.999938)
        {
          if (sampledAngleHistogramExtractor9Val <= 0.990007)
          {
            if (edgeDistanceExtractor4Val <= 0.998996)
            {
              if (lengthScoreExtractor2Val > 0.998986)
              {
                result = { match: 1.0, explain:"match" };
                return result;
              }
              else
              {
                if (centroidDistanceExtractorVal <= 0.999805 || edgeDistanceExtractor4Val > 0.999833)
                {
                  result = { review: 1.0, explain:"review" };
                  return result;
                }
                else if (centroidDistanceExtractorVal > 0.999946 && weightedMetricDistanceExtractor9Val <= 0.000101)
                {
                  if (weightedMetricDistanceExtractor9Val <= 0.000034)
                  {
                    if (lengthScoreExtractor2Val <= 0.999842 && sampledAngleHistogramExtractor9Val > 0.999985)
                    {
                      result = { match: 1.0, explain:"match" };
                      return result;
                    }
                    else if (lengthScoreExtractor2Val > 0.999842)
                    {
                      result = { match: 1.0, explain:"match" };
                      return result;
                    }
                  }
                  else
                  {
                    result = { review: 1.0, explain:"review" };
                    return result;
                  }
                }
              }
            }
            else
            {
              if (weightedMetricDistanceExtractor9Val <= 0.000062)
              {
                result = { match: 1.0, explain:"match" };
                return result;
              }
              else if (sampledAngleHistogramExtractor9Val > 0.94127)
              {
                if (edgeDistanceExtractor4Val <= 0.999094 || edgeDistanceExtractor4Val > 0.999227)
                {
                  result = { match: 1.0, explain:"match" };
                  return result;
                }
              }
            }
          }
        }
        else if (lengthScoreExtractor2Val > 0.99897)
        {
          result = { match: 1.0, explain:"match" };
          return result;
        }
      }*/

      //#4
      //1 - .705
      //2 - .03
      //3 - 0
      //4 - 0
      /*if (centroidDistanceExtractorVal > 0.974276 && lengthScoreExtractor2Val > 0.998201)
      {
        if (edgeDistanceExtractor4Val <= 0.999938 && lengthScoreExtractor2Val > 0.999702 && centroidDistanceExtractorVal <= 0.999946)
        {
          if (centroidDistanceExtractorVal <= 0.999805 || edgeDistanceExtractor4Val > 0.999833)
          {
            result = { review: 1.0, explain:"review" };
            return result;
          }
          else if (centroidDistanceExtractorVal > 0.999946 && weightedMetricDistanceExtractor9Val <= 0.000101)
          {
            result = { match: 1.0, explain:"match" };
            return result;
          }
        }
        else if (edgeDistanceExtractor4Val > 0.999938 && lengthScoreExtractor2Val > 0.99897)
        {
          result = { match: 1.0, explain:"match" };
          return result;
        }
      }*/

      //#5
      //1 - .803
      //2 - .10
      //3 - .018
      //4 - .01
      /*if (centroidDistanceExtractorVal > 0.974276)
      {
        if (lengthScoreExtractor5Val <= 0.999423)
        {
          if (edgeDistanceExtractor4Val > 0.999981 && lengthScoreExtractor5Val > 0.998113)
          {
            if (lengthScoreExtractor5Val <= 0.99897 && lengthScoreExtractor5Val <= 0.998433)
            {
              result = { match: 1.0, explain:"match" };
              return result; 
            }
            else if (lengthScoreExtractor5Val > 0.99897)
            {
              result = { match: 1.0, explain:"match" };
              return result;
            }
          }
        }
        else
        {
          if (edgeDistanceExtractor4Val <= 0.999938)
          {
            if (lengthScoreExtractor5Val <= 0.999612)
            {
              result = { match: 1.0, explain:"match" };
              return result;
            }
            else
            {
              if (centroidDistanceExtractorVal <= 0.999946)
              {
                if (lengthScoreExtractor5Val > 0.999702)
                {
                  if (centroidDistanceExtractorVal <= 0.999805 || edgeDistanceExtractor4Val > 0.999833)
                  {
                    result = { review: 1.0, explain:"review" };
                    return result;
                  }
                }
              }
              else
              {
                result = { match: 1.0, explain:"match" };
                return result;
              }
            }
          }
          else 
          {
            result = { match: 1.0, explain:"match" };
            return result;
          }
        }
      }*/

      //#6
      //1 - passed
      //2 - .075
      //3 - .027
      //4 - .025
      /*if (centroidDistanceExtractorVal > 0.996138 && lengthScoreExtractor5Val > 0.998459 && centroidDistanceExtractorVal > 0.999299)
      {
        if (centroidDistanceExtractorVal <= 0.999554)
        {
          result = { match: 1.0, explain:"match" };
          return result;
        }
        else
        {
          if (centroidDistanceExtractorVal <= 0.999967 && lengthScoreExtractor5Val <= 0.998485)
          {
            if (centroidDistanceExtractorVal <= 0.999786)
            {
              result = { review: 1.0, explain:"review" };
              return result;
            }
            else
            {
              result = { match: 1.0, explain:"match" };
              return result;
            }
          }
          else if (centroidDistanceExtractorVal > 0.999967 && lengthScoreExtractor5Val > 0.99897)
          {
            result = { match: 1.0, explain:"match" };
            return result;
          }
        }
      }*/

      //mine
      //1 - .93
      //2 - .42
      //3 - .65
      //4 - .616
      if (/*angleHistogramExtractor5Val > 0.95 && */centroidDistanceExtractorVal > 0.61 && distanceScoreExtractor7Val < 0.552 &&
          edgeDistanceExtractor1Val > 0.64 && euclideanDistanceExtractorVal > 0.64 && hausdorffDistanceExtractorVal > 0.14 &&
          lengthScoreExtractor1Val > 0.41 && lengthScoreExtractor7Val < 0.155 && nameScoreExtractorVal < 0.451 && 
          parallelScoreExtractorVal > 0.76 && soundexExtractorVal < 0.75 && translateMinWordSetLevenshtein_1_15Val <= 0.5 && 
          weightedMetricDistanceExtractor1Val < 0.57 && weightedShapeDistanceExtractor1Val > 0.64 && 
          weightedShapeDistanceExtractor7Val < 0.49)
      {
        result = { match: 1.0, explain:"match" };
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

    //hoot.log("angleHistogramExtractor");
    featureDetails["angleHistogramExtractor1"] = angleHistogramExtractor1.extract(m, m1, m2);
    featureDetails["angleHistogramExtractor2"] = angleHistogramExtractor2.extract(m, m1, m2); 
    featureDetails["angleHistogramExtractor3"] = angleHistogramExtractor3.extract(m, m1, m2);
    featureDetails["angleHistogramExtractor4"] = angleHistogramExtractor4.extract(m, m1, m2);
    featureDetails["angleHistogramExtractor5"] = angleHistogramExtractor5.extract(m, m1, m2);

    //hoot.log("attributeScoreExtractor");
    featureDetails["attributeScoreExtractor1"] = attributeScoreExtractor1.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor2"] = attributeScoreExtractor2.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor3"] = attributeScoreExtractor3.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor4"] = attributeScoreExtractor4.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor5"] = attributeScoreExtractor5.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor6"] = attributeScoreExtractor6.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor7"] = attributeScoreExtractor7.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor8"] = attributeScoreExtractor8.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor9"] = attributeScoreExtractor9.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor10"] = attributeScoreExtractor10.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor11"] = attributeScoreExtractor11.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor12"] = attributeScoreExtractor12.extract(m, m1, m2);
    featureDetails["attributeScoreExtractor13"] = attributeScoreExtractor13.extract(m, m1, m2);

    //hoot.log("bufferedOverlapExtractor");
    featureDetails["bufferedOverlapExtractor1"] = bufferedOverlapExtractor1.extract(m, m1, m2);
    featureDetails["bufferedOverlapExtractor2"] = bufferedOverlapExtractor2.extract(m, m1, m2);

    //hoot.log("centroidDistanceExtractor");
    featureDetails["centroidDistanceExtractor"] = centroidDistanceExtractor.extract(m, m1, m2);

    //hoot.log("compactnessExtractor");
    featureDetails["compactnessExtractor"] = compactnessExtractor.extract(m, m1, m2);

    //hoot.log("distanceScoreExtractor");
    featureDetails["distanceScoreExtractor1"] = distanceScoreExtractor1.extract(m, m1, m2);
    featureDetails["distanceScoreExtractor2"] = distanceScoreExtractor2.extract(m, m1, m2);
    featureDetails["distanceScoreExtractor3"] = distanceScoreExtractor3.extract(m, m1, m2);
    featureDetails["distanceScoreExtractor4"] = distanceScoreExtractor4.extract(m, m1, m2);
    featureDetails["distanceScoreExtractor5"] = distanceScoreExtractor5.extract(m, m1, m2);
    featureDetails["distanceScoreExtractor6"] = distanceScoreExtractor6.extract(m, m1, m2);
    featureDetails["distanceScoreExtractor7"] = distanceScoreExtractor7.extract(m, m1, m2);

    //hoot.log("edgeDistanceExtractor");
    featureDetails["edgeDistanceExtractor1"] = edgeDistanceExtractor1.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor2"] = edgeDistanceExtractor2.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor3"] = edgeDistanceExtractor3.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor4"] = edgeDistanceExtractor4.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor5"] = edgeDistanceExtractor5.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor6"] = edgeDistanceExtractor6.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor7"] = edgeDistanceExtractor7.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor8"] = edgeDistanceExtractor8.extract(m, m1, m2);
    featureDetails["edgeDistanceeExtractor9"] = edgeDistanceExtractor9.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor10"] = edgeDistanceExtractor10.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor11"] = edgeDistanceExtractor11.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor12"] = edgeDistanceExtractor12.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor13"] = edgeDistanceExtractor13.extract(m, m1, m2);

    //hoot.log("euclideanDistanceExtractor");
    featureDetails["euclideanDistanceExtractor"] = euclideanDistanceExtractor.extract(m, m1, m2);

    //hoot.log("hausdorffDistanceExtractor");
    featureDetails["hausdorffDistanceExtractor"] = hausdorffDistanceExtractor.extract(m, m1, m2);

    //hoot.log("lengthScoreExtractor");
    featureDetails["lengthScoreExtractor1"] = lengthScoreExtractor1.extract(m, m1, m2);
    featureDetails["lengthScoreExtractor2"] = lengthScoreExtractor2.extract(m, m1, m2);
    featureDetails["lengthScoreExtractor3"] = lengthScoreExtractor3.extract(m, m1, m2);
    featureDetails["lengthScoreExtractor4"] = lengthScoreExtractor4.extract(m, m1, m2);
    featureDetails["lengthScoreExtractor5"] = lengthScoreExtractor5.extract(m, m1, m2);
    featureDetails["lengthScoreExtractor6"] = lengthScoreExtractor6.extract(m, m1, m2);
    featureDetails["lengthScoreExtractor7"] = lengthScoreExtractor7.extract(m, m1, m2);

    //hoot.log("nameExtractor");
    featureDetails["nameExtractor"] = nameExtractor.extract(m, m1, m2);
    featureDetails["soundexExtractor"] = soundexExtractor.extract(m, m1, m2);
    featureDetails["translateMaxWordSetLevenshtein_1_15"] = translateMaxWordSetLevenshtein_1_15.extract(m, m1, m2);
    featureDetails["translateMinWordSetLevenshtein_1_15"] = translateMinWordSetLevenshtein_1_15.extract(m, m1, m2);
    featureDetails["weightedWordDistance"] = weightedWordDistance.extract(m, m1, m2);

    //hoot.log("overlapExtractor");
    featureDetails["overlapExtractor"] = overlapExtractor.extract(m, m1, m2);

    //hoot.log("parallelScoreExtractor");
    featureDetails["parallelScoreExtractor"] = parallelScoreExtractor.extract(m, m1, m2);
   
    //hoot.log("sampledAngleHistogramExtractor");
    featureDetails["sampledAngleHistogramExtractor1"] = sampledAngleHistogramExtractor1.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor2"] = sampledAngleHistogramExtractor2.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor3"] = sampledAngleHistogramExtractor3.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor4"] = sampledAngleHistogramExtractor4.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor5"] = sampledAngleHistogramExtractor5.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor6"] = sampledAngleHistogramExtractor6.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor7"] = sampledAngleHistogramExtractor7.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor8"] = sampledAngleHistogramExtractor8.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor9"] = sampledAngleHistogramExtractor9.extract(m, m1, m2);
    featureDetails["sampledAngleHistogramExtractor10"] = sampledAngleHistogramExtractor10.extract(m, m1, m2);

    //hoot.log("smallerOverlapExtractor");
    featureDetails["smallerOverlapExtractor"] = smallerOverlapExtractor.extract(m, m1, m2);

    //hoot.log("weightedMetricDistanceExtractor");
    featureDetails["weightedMetricDistanceExtractor1"] = weightedMetricDistanceExtractor1.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor2"] = weightedMetricDistanceExtractor2.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor3"] = weightedMetricDistanceExtractor3.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor4"] = weightedMetricDistanceExtractor4.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor5"] = weightedMetricDistanceExtractor5.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor6"] = weightedMetricDistanceExtractor6.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor7"] = weightedMetricDistanceExtractor7.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor8"] = weightedMetricDistanceExtractor8.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor9"] = weightedMetricDistanceExtractor9.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor10"] = weightedMetricDistanceExtractor10.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor11"] = weightedMetricDistanceExtractor11.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor12"] = weightedMetricDistanceExtractor12.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor13"] = weightedMetricDistanceExtractor13.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor14"] = weightedMetricDistanceExtractor14.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor15"] = weightedMetricDistanceExtractor15.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor16"] = weightedMetricDistanceExtractor16.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor17"] = weightedMetricDistanceExtractor17.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor18"] = weightedMetricDistanceExtractor18.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor19"] = weightedMetricDistanceExtractor19.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor20"] = weightedMetricDistanceExtractor20.extract(m, m1, m2);

    //hoot.log("weightedShapeDistanceExtractor1");
    featureDetails["weightedShapeDistanceExtractor1"] = weightedShapeDistanceExtractor1.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceExtractor2"] = weightedShapeDistanceExtractor2.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceExtractor3"] = weightedShapeDistanceExtractor3.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceExtractor4"] = weightedShapeDistanceExtractor4.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceExtractor5"] = weightedShapeDistanceExtractor5.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceExtractor6"] = weightedShapeDistanceExtractor6.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceExtractor7"] = weightedShapeDistanceExtractor7.extract(m, m1, m2);
  }

  return featureDetails;
};

