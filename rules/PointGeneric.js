"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches points";
exports.matchThreshold = parseFloat(hoot.get("generic.point.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("generic.point.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("generic.point.review.threshold"));
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.point"));
exports.tagThreshold = parseFloat(hoot.get("generic.conflate.tag.threshold"));
exports.experimental = false;

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
  return isPoint(e);
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
  var result;

  if (e1.getStatusString() == e2.getStatusString()) 
  {
    result = { miss: 1.0, explain:'Miss' };
  }
    
  var tagScore = getTagScore(map, e1, e2);
  if (tagScore < exports.tagThreshold)
  {
    result = { match: 0.0, miss: 1.0, review: 0.0 };
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
  // replace instances of e2 with e1 and merge tags
  mergeElements(map, e1, e2);
  e1.setStatusString("conflated");
  return e1;
};


