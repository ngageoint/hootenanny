/**
 * This script conflates all generic point/polygon pairs which don't fit into a specific type
   category.
 */

"use strict";

exports.description = "Matches generic points with polygons";
exports.experimental = false;
// The baseFeatureType and geometryType aren't needed for Point/Polygon with stats due to it
// conflating different geometry types. Logic has been added to ScriptMatchCreator to handle this,
// in a custom fashion so they may remain empty.
exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.searchRadius = parseFloat(hoot.get("search.radius.generic.point.polygon"));
// This is needed for disabling superfluous conflate ops only. exports.isMatchCandidate handles
// culling match candidates.
exports.matchCandidateCriterion = "PointCriterion;PolygonCriterion";

// This matcher only sets match/miss/review values to 1.0. Therefore, we just use the default
// conflate thresholds and they're effectively ignored. If more custom values are ever required,
// then the generic score threshold configuration options used below should be replaced with custom
// score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

// This is used only to determine type similarity and are independent of the other score thresholds.
exports.typeThreshold = parseFloat(hoot.get("generic.point.polygon.type.threshold"));

// geometry matchers
var distanceExtractor = 
  new hoot.EuclideanDistanceExtractor({ "convert.require.area.for.polygon": "false" });

// used for debugging
exports.writeDebugTags = hoot.get("writer.include.debug.tags");
// used for conflate provenance
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  // We follow the same convention as POI/Polygon conflation here where all the match candidates are
  // just points (not polys) and we find polygon neighbors to match with inside of
  // ScriptMatchCreator. We're not getting the geometry type from exports.geometryType for the
  // reason described above.
  return hoot.OsmSchema.isPoint(map, e) && !hoot.OsmSchema.isSpecificallyConflatable(map, e, "point");
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
 * Simpler version of the mergeSets function. Maybe only support this at first. It only supports
   merging two elements and the replaced list is determined implicitly based on the result.
 */
exports.mergePair = function(map, e1, e2)
{
  hoot.trace("e1: " + e1.getId() + ", " + e1.getTags().get("name"));
  hoot.trace("e2: " + e2.getId() + ", " + e2.getTags().get("name"));

  // replace instances of e2 with e1 and merge tags
  
  // We always want to keep the poly and lose the point geometry.
  var keeper;
  var toReplace;
  if (hoot.OsmSchema.isPolygon(map, e1))
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
  if (exports.writeDebugTags == "true" && exports.writeMatchedBy == "true")
  {
    // Technically, we should get this key from MetadataTags, but that's not integrated with hoot
    // yet.
    keeper.setTag("hoot:matchedBy", "PointPolygon");
  }

  return keeper;
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var featureDetails = [];
  return featureDetails;
};


