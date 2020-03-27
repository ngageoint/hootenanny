/**
 * This script conflates administrative boundary relations.
 */

"use strict";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.description = "Matches administrative boundaries";

// This matcher only sets match/miss/review values to 1.0, therefore the score thresholds aren't used. 
// If that ever changes, then the generic score threshold configuration options used below should 
// be replaced with custom score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

exports.searchRadius = parseFloat(hoot.get("search.radius.administrative.boundary"));
exports.tagThreshold = parseFloat(hoot.get("administrative.boundary.tag.threshold"));
exports.experimental = false;
exports.baseFeatureType = "AdministrativeBoundary";
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");
exports.geometryType = "polygon";

// This is needed for disabling superfluous conflate ops. In the future, it may also
// be used to replace exports.isMatchCandidate (see #3047).
exports.matchCandidateCriterion = "hoot::AdministrativeBoundaryCriterion";

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

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return isAdministrativeBoundary(e);
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

  // other possible tags to look at: border_type, name, place
  if (tags1.get("admin_level") !=  tags2.get("admin_level"))
  {
    return result;
  }
  if (tags1.get("type") != tags2.get("type"))
  {
    return result;
  }

  var smallerOverlap = smallerOverlapExtractor.extract(map, e1, e2);
  var overlap = overlapExtractor.extract(map, e1, e2);
  var bufferedOverlap = bufferedOverlapExtractor.extract(map, e1, e2);

  var edgeDist = edgeDistanceExtractor.extract(map, e1, e2);
  var weightedShapDist = weightedShapeDistanceExtractor.extract(map, e1, e2);
  var dist = distanceScoreExtractor.extract(map, e1, e2);
  var length = lengthScoreExtractor.extract(map, e1, e2);
  var weightedMetricDist = weightedMetricDistanceExtractor.extract(map, e1, e2);
  var hausdorffDist = hausdorffDistanceExtractor.extract(map, e1, e2);

  var parallel = parallelScoreExtractor.extract(map, e1, e2);
  var angleHist = angleHistogramExtractor.extract(map, e1, e2);
  var sampledAngleHist = sampledAngleHistogramExtractor.extract(map, e1, e2);

  var name = nameExtractor.extract(map, e1, e2);

  hoot.trace("smallerOverlap: " + smallerOverlap);
  hoot.trace("overlap: " + overlap);
  hoot.trace("bufferedOverlap: " + bufferedOverlap);
  hoot.trace("edgeDist: " + edgeDist);
  hoot.trace("weightedShapDist: " + weightedShapDist);
  hoot.trace("dist: " + dist);
  hoot.trace("length: " + length);
  hoot.trace("weightedMetricDist: " + weightedMetricDist);
  hoot.trace("hausdorffDist: " + hausdorffDist);
  hoot.trace("parallel: " + parallel);
  hoot.trace("angleHist: " + angleHist);
  hoot.trace("sampledAngleHist: " + sampledAngleHist);
  hoot.trace("name: " + name);

  if (name == 1.0)
  {
    result = { match: 1.0, miss: 0.0, review: 0.0 };
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
  mergeAdminBoundsRelations(map, e1, e2);

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


