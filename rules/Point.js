/**
 * This script conflates all generic points which don't fit into a specific type category.
 */

"use strict";

exports.description = "Matches generic points";
exports.experimental = false;
exports.baseFeatureType = "Point";
exports.geometryType = "point";
exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.point"));
// This is needed for disabling superfluous conflate ops only. exports.isMatchCandidate handles
// culling match candidates.
exports.matchCandidateCriterion = "PointCriterion";

// This matcher only sets match/miss/review values to 1.0. Therefore, we just use the default
// conflate thresholds and they're effectively ignored. If more custom values are ever required,
// then the generic score threshold configuration options used below should be replaced with custom
// score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

// This is used only to determine type similarity and is independent of the other score thresholds.
exports.typeThreshold = parseFloat(hoot.get("generic.point.type.threshold"));

// used for debugging
exports.writeDebugTags = hoot.get("writer.include.debug.tags");
// used for conflate provenance
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");

function distance(e1, e2) 
{
  return Math.sqrt(Math.pow(e1.getX() - e2.getX(), 2) +
         Math.pow(e1.getY() - e2.getY(), 2));
}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return hoot.OsmSchema.isPoint(map, e) && !hoot.OsmSchema.isSpecificallyConflatable(map, e, exports.geometryType);
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
  var result = { miss: 1.0 };

  if (e1.getStatusString() == e2.getStatusString()) 
  {
    hoot.trace("same statuses: miss");
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

  // If both features have types and they aren't just generic types, let's do a detailed type
  // comparison and look for an explicit type mismatch. Otherwise, move on to the geometry
  // comparison.
  var typeScorePassesThreshold = !hoot.OsmSchema.explicitTypeMismatch(e1, e2, exports.typeThreshold);
  hoot.trace("typeScorePassesThreshold: " + typeScorePassesThreshold);
  if (!typeScorePassesThreshold)
  {
    return result;
  }

  var error1 = e1.getCircularError();
  var error2 = e2.getCircularError();
  // This is a simple check to see if the two points are within the worst CE of each other; could
  // use EuclideanDistanceExtractor here instead.
  var distanceBetweenFeatures = distance(e1, e2);
  var searchRadius = Math.max(error1, error2);
  var geometryMatch = false;
  if (distanceBetweenFeatures <= searchRadius)
  {
    geometryMatch = true;
  }
  hoot.trace("distanceBetweenFeatures: " + distanceBetweenFeatures);
  hoot.trace("geometryMatch: " + geometryMatch);

  var featureMatch = typeScorePassesThreshold && geometryMatch;
  if (featureMatch)
  {
    result = { match: 1.0 };
  }
  hoot.trace("featureMatch: " + featureMatch);

  return result;
};

/**
 * Simpler version of the mergeSets function. Maybe only support this at first. It only supports
   merging two elements and the replaced list is determined implicitly based on the result.
 */
exports.mergePair = function(map, e1, e2)
{
  // replace instances of e2 with e1 and merge tags
  mergeElements(map, e1, e2);
  e1.setStatusString("conflated");
  if (exports.writeDebugTags == "true" && exports.writeMatchedBy == "true")
  {
    // Technically we should get this key from MetadataTags, but that's not integrated with hoot
    // yet.
    e1.setTag("hoot:matchedBy", exports.baseFeatureType);
  }
  return e1;
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var featureDetails = [];
  return featureDetails;
};


