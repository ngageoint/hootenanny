/**
 * This script conflates all point features using Generic Conflation.
 */

"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches generic points";
exports.experimental = false;

// This matcher only sets match/miss/review values to 1.0, therefore the score thresholds aren't used. 
// If that ever changes, then the generic score threshold configuration options used below should 
// be replaced with custom score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

exports.searchRadius = parseFloat(hoot.get("search.radius.generic.point"));
exports.tagThreshold = parseFloat(hoot.get("generic.point.tag.threshold"));
exports.baseFeatureType = "Point";
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");
exports.geometryType = "point";

// This is needed for disabling superfluous conflate ops. In the future, it may also
// be used to replace exports.isMatchCandidate (see #3047).
exports.matchCandidateCriterion = "hoot::PointCriterion";

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
  //hoot.trace("e: " + e.getId());
  //hoot.trace("isPoint(map, e): " + isPoint(map, e));
  //hoot.trace("isSpecificallyConflatable(map, e, exports.geometryType): " + isSpecificallyConflatable(map, e, exports.geometryType));

  return isPoint(map, e) && !isSpecificallyConflatable(map, e, exports.geometryType);
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

  // If both features have types and they aren't just generic types, let's do a detailed type comparison and 
  // look for an explicit type mismatch. Otherwise, move on to the geometry comparison.
  var typeScorePassesThreshold = !explicitTypeMismatch(e1, e2, exports.tagThreshold);
  hoot.trace("typeScorePassesThreshold: " + typeScorePassesThreshold);
  if (!typeScorePassesThreshold)
  {
    return result;
  }

  var error1 = e1.getCircularError();
  var error2 = e2.getCircularError();
  // This is a simple check to see if the two points are within the worst CE of each other; could use EuclideanDistanceExtractor here instead
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


