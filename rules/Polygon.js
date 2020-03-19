/**
 * This script conflates all polygons using Generic Conflation.
 */

"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches polygons";
// This matcher only sets match/miss/review values to 1.0, therefore the score thresholds aren't used. 
// If that ever changes, then the generic score threshold configuration options used below should 
// be replaced with custom score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.polygon"));
exports.tagThreshold = parseFloat(hoot.get("generic.polygon.tag.threshold"));
exports.experimental = false;
exports.baseFeatureType = "Polygon";
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");
exports.geometryType = "polygon";
exports.matchCandidateCriterion = "hoot::PolygonCriterion";

var overlapExtractor = 
  new hoot.SmallerOverlapExtractor({ "convert.require.area.for.polygon": "false" });

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
  return isPolygon(e) && !isSpecificallyConflatable(map, e, exports.geometryType);
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
  var result = { miss: 1.0 };

  if (e1.getStatusString() == e2.getStatusString()) 
  {
    return result;
  }

  hoot.trace("e1: " + e1.getId() + ", " + e1.getTags().get("name"));
  if (e1.getTags().get("note"))
  {
    hoot.trace("e1 note: " + e1.getTags().get("note"));
  }
  hoot.trace("e2: " + e2.getId() + ", " + e2.getTags().get("name"));
  if (e2.getTags().get("note"))
  {
    hoot.trace("e2 note: " + e2.getTags().get("note"));
  }

  // TODO: Should we do anything with names?

  // If both features have types and they aren't just generic types, let's do a detailed type comparison and 
  // look for an explicit type mismatch. Otherwise, move on to the geometry comparison.
  var typeScorePassesThreshold = !explicitTypeMismatch(e1, e2, exports.tagThreshold);
  hoot.trace("typeScorePassesThreshold: " + typeScorePassesThreshold);
  if (!typeScorePassesThreshold)
  {
    return result;
  }

  // These geometry rules were derived by using training data in Weka with the
  // REPTree model w/ maxDepth set to 3. Note: This was taken directly from Building.js.

  var geometryMatch = false;
  var geometryReview = false;
  var overlap = overlapExtractor.extract(map, e1, e2);
  var hist = -1.0;
  if (overlap < 0.20)
  {
  }
  else 
  {
    if (overlap < 0.45)
    {
      hist = new hoot.AngleHistogramExtractor().extract(map, e1, e2);
      if (hist < 0.58)
      {
        geometryReview = true;
      }
      else
      {
        geometryMatch = true;
      }
    }
    else
    {
      geometryMatch = true;
    }
  }
  hoot.trace("overlap: " + overlap);
  hoot.trace("hist: " + hist);
  hoot.trace("geometryMatch: " + geometryMatch);

  var featureReview = false;
  var featureMatch = typeScorePassesThreshold && geometryMatch;
  if (featureMatch)
  {
    result = { match: 1.0 };
  }
  else
  {
    featureReview = typeScorePassesThreshold && geometryReview;
    if (featureReview)
    {
      result = { review: 1.0 };
    }
  }
  hoot.trace("featureReview: " + featureReview);
  hoot.trace("featureMatch: " + featureMatch);

  return result;
};

/**
 * Simpler version of the mergeSets function. Maybe only support this at first.
 * It only supports merging two elements and the replaced list is determined
 * implicitly based on the result.
 */
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
  return featureDetails;
};


