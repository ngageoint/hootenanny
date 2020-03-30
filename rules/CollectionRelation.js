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
exports.experimental = false;
exports.baseFeatureType = "CollectionRelation";
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");
exports.geometryType = "polygon";

// This is needed for disabling superfluous conflate ops. In the future, it may also
// be used to replace exports.isMatchCandidate (see #3047).
exports.matchCandidateCriterion = "hoot::CollectionRelationCriterion";

var smallerOverlapExtractor = new hoot.SmallerOverlapExtractor();
var overlapExtractor = new hoot.OverlapExtractor();
var bufferedOverlapExtractor = new hoot.BufferedOverlapExtractor();

var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor();
var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor();
var distanceScoreExtractor = new hoot.DistanceScoreExtractor();
var lengthScoreExtractor = new hoot.LengthScoreExtractor();
var weightedMetricDistanceExtractor = new hoot.WeightedMetricDistanceExtractor();
var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor();

var parallelScoreExtractor = new hoot.ParallelScoreExtractor();
var angleHistogramExtractor = new hoot.AngleHistogramExtractor();
var sampledAngleHistogramExtractor = new hoot.SampledAngleHistogramExtractor();

var nameExtractor = new hoot.NameExtractor();

var smallerOverlapMin = 999999;
var smallerOverlapMax = 0;
var overlapMin = 999999;
var overlapMax = 0;
var bufferedOverlapMin = 999999;
var bufferedOverlapMax = 0;
var edgeDistMin = 999999;
var edgeDistMax = 0;
var weightedShapeDistMin = 999999;
var weightedShapeDistMax = 0;
var distMin = 999999;
var distMax = 0;
var lengthMin = 999999;
var lengthMax = 0;
var weightedMetricDistMin = 999999;
var weightedMetricDistMax = 0;
var hausdorffDistMin = 999999;
var hausdorffDistMax = 0;
var parallelMin = 999999;
var parallelMax = 0;

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
  // other possible tags to look at for admin bounds: border_type, name, place; or just do a 
  // tag distance comp
  if (type1 == "boundary" && tags1.get("boundary") == "administrative" && 
      tags2.get("boundary") == "administrative" && 
      tags1.get("admin_level") != tags2.get("admin_level"))
  {
    hoot.trace("admin_level mismatch");
    return result;
  }
  if (type1 == "multipolygon" && explicitTypeMismatch(e1, e2, exports.tagThreshold))
  {
    hoot.trace("multipoly type mismatch");
    return result;
  }

  var name = nameExtractor.extract(map, e1, e2);

  /*var powerDistance = 1.0;
  if (!ignoreType)
  {
    powerDistance = getTagDistance("power", t1, t2);
  }*/

  hoot.trace("name: " + name);

  if (name == 1.0)
  {
    result = { match: 1.0, miss: 0.0, review: 0.0 };

  var smallerOverlap = smallerOverlapExtractor.extract(map, e1, e2);
  var overlap = overlapExtractor.extract(map, e1, e2);
  var bufferedOverlap = bufferedOverlapExtractor.extract(map, e1, e2);

  var edgeDist = edgeDistanceExtractor.extract(map, e1, e2);
  var weightedShapeDist = weightedShapeDistanceExtractor.extract(map, e1, e2);
  var dist = distanceScoreExtractor.extract(map, e1, e2);
  var length = lengthScoreExtractor.extract(map, e1, e2);
  var weightedMetricDist = weightedMetricDistanceExtractor.extract(map, e1, e2);
  var hausdorffDist = hausdorffDistanceExtractor.extract(map, e1, e2);

  var parallel = parallelScoreExtractor.extract(map, e1, e2);
  //var angleHist = angleHistogramExtractor.extract(map, e1, e2);
  //var sampledAngleHist = sampledAngleHistogramExtractor.extract(map, e1, e2);

  hoot.debug("smallerOverlap: " + smallerOverlap);
  hoot.debug("overlap: " + overlap);
  hoot.debug("bufferedOverlap: " + bufferedOverlap);
  hoot.debug("edgeDist: " + edgeDist);
  hoot.debug("weightedShapeDist: " + weightedShapeDist);
  hoot.debug("dist: " + dist);
  hoot.debug("length: " + length);
  hoot.debug("weightedMetricDist: " + weightedMetricDist);
  hoot.debug("hausdorffDist: " + hausdorffDist);
  hoot.debug("parallel: " + parallel);
  //hoot.debug("angleHist: " + angleHist);
  //hoot.debug("sampledAngleHist: " + sampledAngleHist);

  if (smallerOverlap != null && smallerOverlap < smallerOverlapMin)
  {
    smallerOverlapMin = smallerOverlap;
  }
  if (smallerOverlap != null && smallerOverlap > smallerOverlapMax)
  {
    smallerOverlapMax = smallerOverlap;
  }
  hoot.debug("smallerOverlapMin: " + smallerOverlapMin);
  hoot.debug("smallerOverlapMax: " + smallerOverlapMax);
  if (overlap != null && overlap < overlapMin)
  {
    overlapMin = overlap;
  }
  if (overlap != null && overlap > overlapMax)
  {
    overlapMax = overlap;
  }
  hoot.debug("overlapMin: " + overlapMin);
  hoot.debug("overlapMax: " + overlapMax);
  if (bufferedOverlap != null && bufferedOverlap < bufferedOverlapMin)
  {
    bufferedOverlapMin = bufferedOverlap;
  }
  if (bufferedOverlap != null && bufferedOverlap > bufferedOverlapMax)
  {
    bufferedOverlapMax = bufferedOverlap;
  }
  hoot.debug("bufferedOverlapMin: " + bufferedOverlapMin);
  hoot.debug("bufferedOverlapMax: " + bufferedOverlapMax);
  if (edgeDist != null && edgeDist < edgeDistMin)
  {
    edgeDistMin = edgeDist;
  }
  if (edgeDist != null && edgeDist > edgeDistMax)
  {
    edgeDistMax = edgeDist;
  }
  hoot.debug("edgeDistMin: " + edgeDistMin);
  hoot.debug("edgeDistMax: " + edgeDistMax);
  if (weightedShapeDist != null && weightedShapeDist < weightedShapeDistMin)
  {
    weightedShapeDistMin = weightedShapeDist;
  }
  if (weightedShapeDist != null && weightedShapeDist > weightedShapeDistMax)
  {
    weightedShapeDistMax = weightedShapeDist;
  }
  hoot.debug("weightedShapeDistMin: " + weightedShapeDistMin);
  hoot.debug("weightedShapeDistMax: " + weightedShapeDistMax);
  if (dist != null && dist < distMin)
  {
    distMin = dist;
  }
  if (dist != null && dist > distMax)
  {
    distMax = dist;
  }
  hoot.debug("distMin: " + distMin);
  hoot.debug("distMax: " + distMax);
  if (length != null && length < lengthMin)
  {
    lengthMin = length;
  }
  if (length != null && length > lengthMax)
  {
    lengthMax = length;
  }
  hoot.debug("lengthMin: " + lengthMin);
  hoot.debug("lengthMax: " + lengthMax);
  if (weightedMetricDist != null && weightedMetricDist < weightedMetricDistMin)
  {
    weightedMetricDistMin = weightedMetricDist;
  }
  if (weightedMetricDist != null && weightedMetricDist > weightedMetricDistMax)
  {
    weightedMetricDistMax = weightedMetricDist;
  }
  hoot.debug("weightedMetricDistMin: " + weightedMetricDistMin);
  hoot.debug("weightedMetricDistMax: " + weightedMetricDistMax);
  if (hausdorffDist != null && hausdorffDist < hausdorffDistMin)
  {
    hausdorffDistMin = hausdorffDist;
  }
  if (hausdorffDist != null && hausdorffDist > hausdorffDistMax)
  {
    hausdorffDistMax = hausdorffDist;
  }
  hoot.debug("hausdorffDistMin: " + hausdorffDistMin);
  hoot.debug("hausdorffDistMax: " + hausdorffDistMax);
  if (parallel != null && parallel < parallelMin)
  {
    parallelMin = parallel;
  }
  if (parallel != null && parallel > parallelMax)
  {
    parallelMax = parallel;
  }
  hoot.debug("parallelMin: " + parallelMin);
  hoot.debug("paralleMax: " + parallelMax);
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


