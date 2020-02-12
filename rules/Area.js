/**
 * This script conflates non-building area polygons (e.g. parks, parking lots, etc.; area as defined by the schema) using Generic Conflation.
 */

"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches areas";
// This matcher only sets match/miss/review values to 1.0, therefore the score thresholds aren't used. 
// If that ever changes, then the generic score threshold configuration options used below should 
// be replaced with custom score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));
exports.searchRadius = parseFloat(hoot.get("search.radius.area"));
exports.experimental = true;
exports.baseFeatureType = "Area";
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");
exports.geometryType = "polygon";
exports.matchCandidateCriterion = "hoot::AreaCriterion";

var sublineMatcher = new hoot.MaximalSublineStringMatcher();

/*
This matches areas to areas (area=yes), where an area is a non-building polygon.  

Some examples are: a park polygon which encloses several other park related POI 
nodes and polygons or a school polygon which encloses school buildings on the campus.
*/

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
  return isArea(map, e) && !isBuilding(map, e);
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

  if (e1.getStatusString() == e2.getStatusString())
  {
    return result;
  }

  // Do we need to be looking at tags too, since area is such a broad concept?

  var smallerOverlap = new hoot.SmallerOverlapExtractor().extract(map, e1, e2);
  var overlap = new hoot.OverlapExtractor().extract(map, e1, e2);
  var bufferedOverlap = new hoot.BufferedOverlapExtractor().extract(map, e1, e2);
  var edgeDist = new hoot.EdgeDistanceExtractor().extract(map, e1, e2);
  var angleHist = new hoot.AngleHistogramExtractor().extract(map, e1, e2);

  // This geometry matching model was derived against only one dataset using Weka, so may need more refinement.

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

exports.mergePair = function(map, e1, e2)
{
  // replace instances of e2 with e1 and merge tags
  mergeElements(map, e1, e2);
  e1.setStatusString("conflated");
  if (exports.writeMatchedBy == "true")
  {
    // Technically, we should get this key from MetadataTags, but that's not integrated with hoot yet.
    e1.setTag("hoot:matchedBy", exports.baseFeatureType);
  }
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
  featureDetails["attrScore"] = new hoot.AttributeScoreExtractor().extract(map, e1, e2);
  featureDetails["soundex"] = soundexExtractor.extract(map, e1, e2);
  featureDetails["mean"] = translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
  featureDetails["max"] = translateMaxWordSetLevenshtein_1_15.extract(map, e1, e2);
  featureDetails['weightedWordDistance'] = weightedWordDistance.extract(map, e1, e2);
  featureDetails['weightedPlusMean'] = weightedWordDistance.extract(map, e1, e2);
  
  return featureDetails;
};


