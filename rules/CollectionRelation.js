/**
 * This script conflates collection relations (e.g route or administrative boundary relations).
 */

"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches collection relations";

// This matcher only sets match/miss/review values to 1.0, therefore the score thresholds aren't used. 
// If that ever changes, then the generic score threshold configuration options used below should 
// be replaced with custom score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

exports.searchRadius = parseFloat(hoot.get("search.radius.collection.relation"));
exports.tagThreshold = parseFloat(hoot.get("collection.relation.tag.threshold"));
exports.nameThreshold = parseFloat(hoot.get("collection.relation.name.threshold"));
exports.experimental = false;
exports.baseFeatureType = "CollectionRelation";
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");
exports.geometryType = "polygon";

// This is needed for disabling superfluous conflate ops. In the future, it may also
// be used to replace exports.isMatchCandidate (see #3047).
exports.matchCandidateCriterion = "hoot::CollectionRelationCriterion";

var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor();
//var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor();
var angleHistExtractor = new hoot.SampledAngleHistogramExtractor();

var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor();
var distanceScoreExtractor = new hoot.DistanceScoreExtractor();
var lengthScoreExtractor = new hoot.LengthScoreExtractor();
var weightedMetricDistanceExtractor = new hoot.WeightedMetricDistanceExtractor();
var parallelScoreExtractor = new hoot.ParallelScoreExtractor();

var memberSimilarityExtractor = new hoot.RelationMemberSimilarityExtractor();

var nameExtractor = new hoot.NameExtractor();

/////////////////////

var memberSimMin = 99999;
var memberSimMax = 0;

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return isCollectionRelation(e);
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

  var tags1 = e1.getTags();
  var tags2 = e2.getTags();

  hoot.trace("e1: " + e1.getId() + ", " + tags1.get("name"));
  if (tags1.get("note"))
  {
    hoot.trace("e1 note: " + tags1.get("note"));
  }
  hoot.trace("e2: " + e2.getId() + ", " + tags2.get("name"));
  if (tags2.get("note"))
  {
    hoot.trace("e2 note: " + tags2.get("note"));
  }

  var type1 = e1.getType();
  hoot.trace("type1: " + type1);
  var type2 = e2.getType();
  hoot.trace("type2: " + type2);
  if (type1 != type2)
  {    
    hoot.trace("type mismatch");
    return result;
  }
  else if (type1 == "boundary" && tags1.get("boundary") == "administrative" && tags2.get("boundary") == "administrative" && tags1.get("admin_level") != tags2.get("admin_level"))
  {
    hoot.trace("admin_level mismatch");
    return result;
  }
  else if ((type1 == "multipolygon" || type1 == "multilineString") && explicitTypeMismatch(e1, e2, exports.tagThreshold))
  {
    hoot.trace("multipoly/multilinestring type mismatch");
    return result;
  }

  // These were determined with a very small sample of test data and may need refinement. Also,
  // not all of the feature extractors support relations and, where possible, we may want to 
  // add support for them.

  var name = 1.0;
  if (bothElementsHaveName(e1, e2))
  {
    // only score the name if both have one
    hoot.trace("both elements have name");
    name = nameExtractor.extract(map, e1, e2);
  }
  if (name < exports.nameThreshold)
  {
    return result;
  }

  /*var angleHist = angleHistExtractor.extract(map, e1, e2);
  if (angleHist < 0.072)
  {
    hoot.trace("match failed on angle hist");
    return result;
  }
  var weightedShapeDist = weightedShapeDistanceExtractor.extract(map, e1, e2);
  if (weightedShapeDist < 0.99)
  {
    hoot.trace("match failed weighted shape dist");
    return result;
  }
  var dist = distanceScoreExtractor.extract(map, e1, e2);
  if (dist < 0.96)
  {
    hoot.trace("match failed on dist");
    return result;
  }
  var length = lengthScoreExtractor.extract(map, e1, e2);
  if (length < 0.81 || length > 0.97)
  {
    hoot.trace("match failed on length");
    return result;
  }
  var weightedMetricDist = weightedMetricDistanceExtractor.extract(map, e1, e2);
  if (weightedMetricDist > 0.04)
  {
    hoot.trace("match failed weighted metric dist");
    return result;
  }
  var parallel = parallelScoreExtractor.extract(map, e1, e2);
  if (parallel < 0.99)
  {
    hoot.trace("match failed on parallel");
    return result;
  }*/
  // This is a fairly expensive check against these relations.
  var edgeDist = edgeDistanceExtractor.extract(map, e1, e2);
  hoot.trace("edgeDist: " + edgeDist);
  if (edgeDist < 0.97)
  { 
    hoot.trace("match failed on edge distance");
    return result;
  }
  // Seeing if we can get by w/o this for now.
  /*var hausdorffDist = hausdorffDistanceExtractor.extract(map, e1, e2);
  hoot.trace("hausdorffDist: " + hausdorffDist);
  if (hausdorffDist < 0.21)
  {
    hoot.trace("match failed on hausdorff");
    return result;
  }*/

  /*var memberSim = memberSimilarityExtractor.extract(map, e1, e2);
  if (memberSim < memberSimMin)
  {
    memberSimMin = memberSim;
  }
  hoot.debug("memberSimMin: " + memberSimMin);
  if (memberSim > memberSimMax)
  {
    memberSimMax = memberSim;
  }
  hoot.debug("memberSimMax: " + memberSimMax);*/

  result = { match: 1.0, miss: 0.0, review: 0.0 };

  return result;
};

/**
 * Simpler version of the mergeSets function. Maybe only support this at first.
 * It only supports merging two elements and the replaced list is determined
 * implicitly based on the result.
 */
exports.mergePair = function(map, e1, e2)
{
  mergeCollectionRelations(map, e1.getElementId(), e2.getElementId());

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


