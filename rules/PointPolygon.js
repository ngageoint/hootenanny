/**
 * This script conflates all points with all polygons using Generic Conflation.
 */

"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches generic points with polygons";
exports.experimental = false;
exports.matchThreshold = parseFloat(hoot.get("generic.point.polygon.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("generic.point.polygon.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("generic.point.polygon.review.threshold"));
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.point.polygon"));
exports.tagThreshold = parseFloat(hoot.get("generic.conflate.tag.threshold"));
exports.writeDebugTags = hoot.get("writer.include.debug.tags");
//exports.baseFeatureType = ""; // 
//exports.geometryType = "";
// The baseFeatureType and geometryType vars don't work for Point/Polygon due to it conflating different geometry types.
// Logic has been added to ScriptMatchCreator to handle this, so they can remain empty.

var distanceExtractor = 
  new hoot.EuclideanDistanceExtractor({ "convert.require.area.for.polygon": "false" });

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
  // We follow the same convention as POI/Polygon conflation here where all the match candidates are just points (not polys) and
  // we find polygon neighbors to match with inside of ScriptMatchCreator.
  return isPoint(map, e)  && !isSpecificallyConflatable(map, e);
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

  // This is a simple check to see if the two features are within the worst CE of each other.
  var error1 = e1.getCircularError();
  var error2 = e2.getCircularError();
  var distanceBetweenFeatures = distanceExtractor.extract(map, e1, e2);
  var searchRadius = Math.max(error1, error2);
  var geometryMatch = false;
  if (distanceBetweenFeatures <= searchRadius)
  {
    geometryMatch = true;
  }
  hoot.trace("searchRadius: " + searchRadius);
  hoot.trace("distanceBetweenFeatures: " + distanceBetweenFeatures);
  hoot.trace("geometryMatch: " + geometryMatch);

  var featureMatch = typeScorePassesThreshold && geometryMatch;
  if (featureMatch)
  {
    var result = { match: 1.0 };
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
  hoot.trace("e1: " + e1.getId() + ", " + e1.getTags().get("name"));
  hoot.trace("e2: " + e2.getId() + ", " + e2.getTags().get("name"));

  // replace instances of e2 with e1 and merge tags
  
  // We always want to keep the poly and lose the point geometry.
  var keeper;
  var toReplace;
  if (isPolygon(e1))
  {
    keeper = e1;
    toReplace = e2;
  }
  else
  {
    keeper = e2;
    toReplace = e1;
  }
  hoot.trace("keeper: " + keeper.getId() + ", " + keeper.getTags().get("name"));
  hoot.trace("toReplace: " + toReplace.getId() + ", " + toReplace.getTags().get("name"));

  mergeElements(map, keeper, toReplace);

  keeper.setStatusString("conflated");
  if (exports.writeDebugTags == "true")
  {
    // Technically, we should get this key from MetadataTags, but that's not integrated with hoot yet.
    keeper.setTag("hoot:matchedBy", "PointPolygon");
    hoot.trace("keeper: " + keeper.getId() + ", " + keeper.getTags().get("hoot:matchedBy"));
  }

  return keeper;
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var featureDetails = [];
  return featureDetails;
};


