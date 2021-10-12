/*
  This is the conflate script for areas, where an area is a non-building polygon with area=yes.

  Some examples are: a park polygon which encloses several other park related POI nodes and polygons
  or a school polygon which encloses school buildings on the campus.
*/

"use strict";

exports.description = "Matches areas";
exports.experimental = false;
exports.baseFeatureType = "Area";
exports.geometryType = "polygon";
exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.searchRadius = parseFloat(hoot.get("search.radius.area"));
// This is needed for disabling superfluous conflate ops only. exports.isMatchCandidate handles
// culling match candidates. This seems like it should probably be NonBuildingAreaCriterion instead
// but that has some affect on the stats that still needs to be investigated.
exports.matchCandidateCriterion = "AreaCriterion";

// This matcher only sets match/miss/review values to 1.0. Therefore, we just use the default
// conflate thresholds and they're effectively ignored. If more custom values are ever required,
// then the generic score threshold configuration options used below should be replaced with custom
// score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

// This is used only to determine type similarity and is independent of the other score
// thresholds.
exports.typeThreshold = parseFloat(hoot.get("area.type.threshold"));

// These are used to help with one to many area matching.
var overlapReviewThreshold = parseFloat(hoot.get("area.overlap.review.threshold"));
var overlapReviewTypeThreshold = parseFloat(hoot.get("area.type.overlap.review.threshold"));

// geometry matchers
var sublineMatcher = new hoot.MaximalSublineStringMatcher();
var smallerOverlapExtractor = new hoot.SmallerOverlapExtractor();
var overlapExtractor = new hoot.OverlapExtractor();
var bufferedOverlapExtractor = new hoot.BufferedOverlapExtractor();
var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor();
var angleHistogramExtractor = new hoot.AngleHistogramExtractor();

// used for debugging
exports.writeDebugTags = hoot.get("writer.include.debug.tags");
// used for conflate provenance
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");

/**
 * Returns true if e is a candidate for a match. Implementing this method is optional, but may
   dramatically increase speed if you can cull some features early on. E.g. no need to check nodes
   for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return hoot.OsmSchema.isNonBuildingArea(map, e);
};

/**
 * If this function returns true then all overlapping matches will be treated as a group. For now
   that means if two matches overlap then the whole group will be marked as needing review.
 *
 * If this function returns false the conflation routines will attempt to pick the best subset of
   matches that do not conflict.
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
 * The scores should always sum to one. If they don't you will be taunted mercilessly and we'll
   normalize it anyway. :P
 */
exports.matchScore = function(map, e1, e2)
{
  var result = { match: 0.0, miss: 1.0, review: 0.0 };

  if (e1.getStatusString() == e2.getStatusString())
  {
    return result;
  }

  hoot.trace("e1: " + e1.getElementId() + ", " + e1.getTags().get("name"));
  if (e1.getTags().get("note"))
  {
    hoot.trace("e1 note: " + e1.getTags().get("note"));
  }
  hoot.trace("e2: " + e2.getElementId() + ", " + e2.getTags().get("name"));
  if (e2.getTags().get("note"))
  {
    hoot.trace("e2 note: " + e2.getTags().get("note"));
  }

  // The geometry matching model was derived against only one training dataset using Weka and 
  // another without using Weka (review generation logic portion), so likely still needs more
  // refinement. The tag matching was derived manually after the fact outside of Weka and is the
  // same that is used with Generic Conflation. The original geometry matching model from Weka has
  // been updated to account for the fact that buffered overlap, edge distance, and overlap are
  // processing intensive (roughly in order from most to least).

  // If both features have types and they aren't just generic types, let's do a detailed type
  // comparison and look for an explicit type mismatch. Otherwise, move on to the geometry
  // comparison.
  var typeScorePassesThreshold = !hoot.OsmSchema.explicitTypeMismatch(e1, e2, exports.typeThreshold);
  hoot.trace("typeScorePassesThreshold: " + typeScorePassesThreshold);
  if (!typeScorePassesThreshold)
  {
    return result;
  }
  
  var angleHist = angleHistogramExtractor.extract(map, e1, e2);
  hoot.trace("angleHist: " + angleHist);
  var edgeDist = -1;
  var bufferedOverlap = -1;
  var smallerOverlap = -1;
  var overlap = -1;

  if (angleHist >= 0.98 || angleHist < 0.4)
  {
    edgeDist = edgeDistanceExtractor.extract(map, e1, e2);
    hoot.trace("edgeDist: " + edgeDist);
    if (edgeDist < 0.97)
    {
      bufferedOverlap = bufferedOverlapExtractor.extract(map, e1, e2);
      hoot.trace("bufferedOverlap: " + bufferedOverlap);
      if (bufferedOverlap >= 0.57)
      {
        hoot.trace("match");
        result = { match: 1.0, miss: 0.0, review: 0.0 };
        return result;
      }
    }
  }
  else if (angleHist >= 0.4)
  {
    smallerOverlap = smallerOverlapExtractor.extract(map, e1, e2);
    hoot.trace("smallerOverlap: " + smallerOverlap);
    if (smallerOverlap < 0.89)
    {
      edgeDist = edgeDistanceExtractor.extract(map, e1, e2);
      hoot.trace("edgeDist: " + edgeDist);
      if (edgeDist < 0.97)
      {
        bufferedOverlap = bufferedOverlapExtractor.extract(map, e1, e2);
        hoot.trace("bufferedOverlap: " + bufferedOverlap);
        if (bufferedOverlap >= 0.57)
        {
          hoot.trace("match");
          result = { match: 1.0, miss: 0.0, review: 0.0 };
          return result;
        }
      }
    }
  }

  overlap = overlapExtractor.extract(map, e1, e2);
  hoot.trace("overlap: " + overlap);
  if (overlap >= 0.18)
  {
    edgeDist = edgeDistanceExtractor.extract(map, e1, e2);
    hoot.trace("edgeDist: " + edgeDist);
    if (edgeDist >= 0.99)
    {
      bufferedOverlap = bufferedOverlapExtractor.extract(map, e1, e2);
      hoot.trace("bufferedOverlap: " + bufferedOverlap);
      if (bufferedOverlap < 0.57)
      {
        hoot.trace("match");
        result = { match: 1.0, miss: 0.0, review: 0.0 };
        return result;
      }
    }
  }

  edgeDist = edgeDistanceExtractor.extract(map, e1, e2);
  hoot.trace("edgeDist: " + edgeDist);
  if (edgeDist >= 0.97)
  {
    bufferedOverlap = bufferedOverlapExtractor.extract(map, e1, e2);
    hoot.trace("bufferedOverlap: " + bufferedOverlap);
    if (bufferedOverlap >= 0.57)
    {
      hoot.trace("match");
      result = { match: 1.0, miss: 0.0, review: 0.0 };
      return result;
    }
  }

  // Here, we're attempting to handle the many to one scenario for diff conflate and will mark this
  // as a review which will cause these features to drop out of the diff in the default config. See
  // tests area-3978-1 and area-4379-1.
  var typeScore = hoot.OsmSchema.scoreTypes(e1.getTags(), e2.getTags(), true);
  if (typeScore >= overlapReviewTypeThreshold)
  {
    if (smallerOverlap == -1) // don't calc it if it already has been
    {
      smallerOverlap = smallerOverlapExtractor.extract(map, e1, e2);
      hoot.trace("smallerOverlap: " + smallerOverlap);
    }
    if (smallerOverlap >= overlapReviewThreshold)
    {
      hoot.trace("review");
      result = { match: 0.0, miss: 0.0, review: 1.0 };
      return result;
    }
  }

  return result;
};

exports.mergePair = function(map, e1, e2)
{
  // replace instances of e2 with e1 and merge tags
  mergeElements(map, e1, e2);
  e1.setStatusString("conflated");
  if (exports.writeDebugTags == "true" && exports.writeMatchedBy == "true")
  {
    // Technically, we should get this key from MetadataTags, but that's not integrated with hoot
    // yet.
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


