/**
 * This script conflates all point features using Generic Conflation.
 */

"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches generic points";
exports.experimental = false;
exports.matchThreshold = parseFloat(hoot.get("generic.point.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("generic.point.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("generic.point.review.threshold"));
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.point"));
exports.tagThreshold = parseFloat(hoot.get("generic.conflate.tag.threshold"));
exports.baseFeatureType = "Point";
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");
exports.geometryType = "point";

function distance(e1, e2) 
{
  return Math.sqrt(Math.pow(e1.getX() - e2.getX(), 2) +
         Math.pow(e1.getY() - e2.getY(), 2));
}

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
  return isPoint(map, e) && !isSpecificallyConflatable(map, e);
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

  var typeScore = getTypeScore(map, e1, e2);
  var typeScorePassesThreshold = false;
  if (typeScore >= exports.tagThreshold)
  {
    typeScorePassesThreshold = true;
  }
  hoot.trace("typeScore: " + typeScore);
  hoot.trace("typeScorePassesThreshold: " + typeScorePassesThreshold);

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


