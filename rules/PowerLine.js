/**
 * This script conflates power lines using Generic Conflation.
 */

"use strict";

exports.description = "Matches power lines";
exports.experimental = false;
exports.baseFeatureType = "PowerLine";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("power.line.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("power.line.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("power.line.review.threshold"));
exports.matchCandidateCriterion = "hoot::PowerLineCriterion";
exports.geometryType = "line";

var sublineMatcher =
  new hoot.MaximalSublineStringMatcher(
    { "way.matcher.max.angle": hoot.get("power.line.matcher.max.angle"),
      "way.subline.matcher": hoot.get("power.line.subline.matcher") });

var centroidDistanceExtractor = new hoot.CentroidDistanceExtractor();
var edgeDistanceExtractor1 = new hoot.EdgeDistanceExtractor(new hoot.MeanAggregator());
var weightedMetricDistanceExtractor1 = new hoot.WeightedMetricDistanceExtractor(new hoot.MeanAggregator());
var weightedShapeDistanceExtractor7 = new hoot.WeightedShapeDistanceExtractor(new hoot.SigmaAggregator());

//for distance weighting
var distanceWeightCoeff = parseFloat(hoot.get("power.line.matcher.distance.weight.coefficient")) * -1.0;
var distanceScoreExtractor = new hoot.DistanceScoreExtractor();

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
          hoot.get("rubber.sheet.minimum.ties"),
          exports.matchCandidateCriterion));
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
 *
 * exports.matchCandidateCriterion takes precedence over this function and must
 * be commented out before using it.
 * 
 * @todo This must be left enabled for now despite exports.matchCandidateCriterion being enabled.  See #3047.
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
  //FOR MODEL BUILDING ONLY
  //var result = { match: 1.0, explain:"match" };
  //return result;

  var result = { miss: 1.0, explain:"miss" };

  hoot.trace("e1: " + e1.getTags());
  hoot.trace("e2: " + e2.getTags());
  hoot.trace("e1: " + e1.getStatusString());
  hoot.trace("e2: " + e2.getStatusString());

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
    var edgeDistanceExtractor1Val = edgeDistanceExtractor1.extract(m, m1, m2);
    var weightedMetricDistanceExtractor1Val = weightedMetricDistanceExtractor1.extract(m, m1, m2);
    var weightedShapeDistanceExtractor7Val = weightedShapeDistanceExtractor7.extract(m, m1, m2);

    /*hoot.trace("centroidDistanceExtractorVal: " + centroidDistanceExtractorVal);
    hoot.trace("edgeDistanceExtractor1Val: " + edgeDistanceExtractor1Val);
    hoot.trace("weightedMetricDistanceExtractor1Val: " + weightedMetricDistanceExtractor1Val);
    hoot.trace("weightedShapeDistanceExtractor7Val: " + weightedShapeDistanceExtractor7Val);*/

    if ((centroidDistanceExtractorVal > 0.61 && weightedMetricDistanceExtractor1Val < 1.4) || 
         (edgeDistanceExtractor1Val > 0.997 && weightedShapeDistanceExtractor7Val == 0.0))
    {
      //So far, voltage and location (underground vs overhead) seem to be the only tags useful to disambiguate power line matches.  We'll 
      //review when two features match and those tags disagree, and we'll ignore those tags completely if the values for either of them 
      //are unpopulated.

      var voltageStr1 = String(e1.getTags().get("voltage")).trim();
      var voltageStr2 = String(e2.getTags().get("voltage")).trim();
      if (voltageStr1 !== null && voltageStr1 !== '' && voltageStr2 !== null && voltageStr2 !== '')
      {
        var voltage1 = parseInt(voltageStr1);
        var voltage2 = parseInt(voltageStr2);
        if (!isNaN(voltage1) && !isNaN(voltage2) && voltage1 != voltage2)
        {
          var msg = "Explicit voltage mismatch between matching features: " + voltage1 + " " + voltage2
          hoot.trace(msg);
          result = { review: 1.0, explain:msg };
          return result;
        }
      }

      var location1 = String(e1.getTags().get("location")).trim();
      var location2 = String(e2.getTags().get("location")).trim();
      if (location1 !== 'undefined' && location1 !== null && location1 !== '' && 
          location2 !== 'undefined' && location2 !== null && location2 !== '' && location1 !== location2)
      {
        var msg = "Explicit location mismatch between matching features: " + location1 + " " + location2;
        hoot.trace(msg);
        result = { review: 1.0, explain:msg };
        return result;
      }

      //use distance weighting to slightly favor features that are closer together
      var distanceScoreValue = distanceScoreExtractor.extract(m, m1, m2);
      var delta = (1.0 - distanceScoreValue) * distanceWeightCoeff;
      result.match = 1.0 + delta;
      hoot.trace(result.match);
      result.miss = 0.0 - delta;
      hoot.trace(result.miss);

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

    featureDetails["centroidDistanceExtractorVal"] = centroidDistanceExtractor.extract(m, m1, m2);
    featureDetails["edgeDistanceExtractor1Val"] = edgeDistanceExtractor1.extract(m, m1, m2);
    featureDetails["weightedMetricDistanceExtractor1Val"] = weightedMetricDistanceExtractor1.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceExtractor7Val"] = weightedShapeDistanceExtractor7.extract(m, m1, m2);
  }

  return featureDetails;
};

