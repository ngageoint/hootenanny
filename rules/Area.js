"use strict";

var MATCH_OVERLAP_THRESHOLD = 0.75;
var MISS_OVERLAP_THRESHOLD = 0.15;

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("generic.polygon.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("generic.polygon.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("generic.polygon.review.threshold"));
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.polygon"));
exports.experimental = true;

var sublineMatcher = new hoot.MaximalSublineStringMatcher();

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
        {"token.separator": "[\\s-,';]+", "weighted.word.distance.p": 0.5},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.5}))));

/*
This matches areas to areas (area=yes), where an area is a non-building polygon.  

Some examples are: a park polygon which encloses several other park related POI 
nodes and polygons or a school polygon which encloses school buildings on the campus.
*/

/**
 * Runs before match creation occurs and provides an opportunity to perform custom initialization.
 */
exports.init = function(map) {

}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
    return isArea(e) && !isBuilding(e);
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
    return true;
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
  var result = { match: 0.0, miss: 1.0, review: 0.0 };

  var smallerOverlap = new hoot.SmallerOverlapExtractor().extract(map, e1, e2);
  var overlap = new hoot.OverlapExtractor().extract(map, e1, e2);
  var bufferedOverlap = new hoot.BufferedOverlapExtractor().extract(map, e1, e2);
  var edgeDist = new hoot.EdgeDistanceExtractor().extract(map, e1, e2);
  var angleHist = new hoot.AngleHistogramExtractor().extract(map, e1, e2);

  //REP - This was derived against only one dataset, so obviously needs more refinement.
  if (bufferedOverlap < 0.57)
  {
    if (overlap >= 0.18 && edgeDist >= 0.99)
    {
      result = { match: 1.0, miss: 0.0, review: 0.0 };
    }
  }
  else
  {
    if (edgeDist < 0.97)
    {
      if (angleHist < 0.98)
      {
        if (angleHist < 0.4)
        {
          result = { match: 1.0, miss: 0.0, review: 0.0 };
        }
        else if (smallerOverlap < 0.89)
        {
          result = { match: 1.0, miss: 0.0, review: 0.0 };
        }
      }
      else
      {
        result = { match: 1.0, miss: 0.0, review: 0.0 };
      }
    }
    else
    {
      result = { match: 1.0, miss: 0.0, review: 0.0 };
    }
  }

  return result;
};

/**
 * Simpler version of the mergeSets function. Maybe only support this at first.
 * It only supports merging two elements and the replaced list is determined
 * implicitly based on the result.
 */
exports.mergePair = function(map, e1, e2)
{
    var newTags = mergeTags(e1, e2);
    e1.setTags(newTags);

    removeElement(map, e2);

    return e1;
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var featureDetails = [];

  featureDetails["smallerOverlap"] = new hoot.SmallerOverlapExtractor().extract(map, e1, e2);
  featureDetails["overlap"] = new hoot.OverlapExtractor().extract(map, e1, e2);
  featureDetails["angleHist"] = new hoot.AngleHistogramExtractor().extract(map, e1, e2);
  featureDetails["edgeDist"] = new hoot.EdgeDistanceExtractor().extract(map, e1, e2);
  featureDetails["bufferedOverlap"] = new hoot.BufferedOverlapExtractor().extract(map, e1, e2);
  featureDetails["hausDist"] = new hoot.HausdorffDistanceExtractor().extract(map, e1, e2);
  featureDetails["attrDist"] = new hoot.AttributeDistanceExtractor().extract(map, e1, e2);
  featureDetails["attrScore"] = new hoot.AttributeScoreExtractor().extract(map, e1, e2);
  featureDetails["soundex"] = soundexExtractor.extract(map, e1, e2);
  featureDetails["mean"] = translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
  featureDetails["max"] = translateMaxWordSetLevenshtein_1_15.extract(map, e1, e2);
  featureDetails['weightedWordDistance'] = weightedWordDistance.extract(map, e1, e2);
  featureDetails['weightedPlusMean'] = weightedWordDistance.extract(map, e1, e2);
  
  return featureDetails;
};


