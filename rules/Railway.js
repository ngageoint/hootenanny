/**
 * This script conflates railways using Generic Conflation.
 */

"use strict";

exports.description = "Matches railways";
exports.experimental = false;
exports.baseFeatureType = "Railway";
exports.geometryType = "line";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);

exports.searchRadius = parseFloat(hoot.get("search.radius.railway"));

// This matcher only sets match/miss/review values to 1.0, therefore the score thresholds aren't
// used. If that ever changes, then the generic score threshold configuration options used below
// should be replaced with custom score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

// This is needed for disabling superfluous conflate ops only. exports.isMatchCandidate handles
// culling match candidates.
exports.matchCandidateCriterion = "RailwayCriterion";

// We're just using the default max recursions here for MaximalSubline. May need to come up with a
// custom value via empirical testing. This will not work if we ever end up needing to pass map in
// here for this data type.
var sublineStringMatcher = hoot.SublineStringMatcherFactory.getMatcher(exports.baseFeatureType);

var distanceScoreExtractor = new hoot.DistanceScoreExtractor();
// Use default spacing of 5 meters.
var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor();
var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor();

//var oneToManyIdentifyingKeys = hoot.get("railway.one.to.many.identifying.keys").split(';');
//var oneToManyTransferKeys = hoot.get("railway.one.to.many.transfer.keys").split(';');
//var oneToManyMergeGeometries = hoot.get("railway.one.to.many.reference.merge.geometries");

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return hoot.OsmSchema.isRailway(e);
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
  return false;
};

function geometryMismatch(map, e1, e2)
{
  hoot.trace("Processing geometry...");

  var sublines;
  hoot.trace("Extracting sublines with default...");
  sublines = sublineStringMatcher.extractMatchingSublines(map, e1, e2);
  hoot.trace(sublines);
  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    // TODO: diff logic path if we find a one to many identifying key in e2

    /*
     Classifier derived using WEKA 3.8.2, then tweaked the numbers a bit. Classifier chosen was
     "REPTree".
    */
    var distanceScore = distanceScoreExtractor.extract(m, m1, m2);
    if (distanceScore >= 0.577)
    {
      var hausdorffDistance = hausdorffDistanceExtractor.extract(m, m1, m2);
      if (hausdorffDistance >= 0.8)
      {
        var edgeDistance  = edgeDistanceExtractor.extract(m, m1, m2);
        if (edgeDistance >= 0.8)
        {
          hoot.trace("Geometry match");
          return false;
        }
      }
    }
  }

  hoot.trace("Geometry mismatch");
  return true;
}

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
  var result = { miss: 1.0, explain:"miss" };

  if (e1.getStatusString() === e2.getStatusString())
  {
    return result;
  }

  var tags1 = e1.getTags();
  var tags2 = e2.getTags();

  hoot.trace("**********************************");
  hoot.trace("e1: " + e1.getElementId() + ", " + tags1.get("name"));
  if (tags1.get("note"))
  {
    hoot.trace("e1 note: " + tags1.get("note"));
  }
  hoot.trace("e2: " + e2.getElementId() + ", " + tags2.get("name"));
  if (tags2.get("note"))
  {
    hoot.trace("e2 note: " + tags2.get("note"));
  }

  // TODO: check for type and name mismatch

  if (geometryMismatch(map, e1, e2))
  {
    return result;
  }

  hoot.trace("match");
  result = { match: 1.0, explain:"match" };
  return result;
};

/**
 * The internals of geometry merging can become quite complex. Typically this
 * method will simply call another hoot method to perform the appropriate merging
 * of geometries.
 *
 * If this method is exported then the mergePair method should not be exported.
 *
 * @param map The map that is being conflated
 * @param pairs An array of ElementId pairs that will be merged.
 * @param replaced An empty array is passed in, the method should fill the array
 *      with all the replacements that occur during the merge process (e.g. if two
 *      elements (way:1 & way:2) are merged into one element (way:3), then the
 *      replaced array should contain [[way:1, way:3], [way:1, way:3]] where all
 *      the "way:*" objects are of the ElementId type.
 */
exports.mergeSets = function(map, pairs, replaced)
{
  // TODO: diff logic path if we have a one to many match; not sure yet if merging many to one
  // geometries is possible with the existing logic

  // Snap the ways in the second input to the first input. Use the default tag merge method.
  return new hoot.LinearMerger().apply(sublineStringMatcher, map, pairs, replaced, exports.baseFeatureType);
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var featureDetails = [];

  // extract the sublines needed for matching
  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    featureDetails["weightedShapeDistance"] = weightedShapeDistanceExtractor.extract(m, m1, m2);
    featureDetails["distanceScore"]         = distanceScoreExtractor.extract(m, m1, m2);
    featureDetails["edgeDistance"]          = edgeDistanceExtractor.extract(m, m1, m2);
    featureDetails["euclideanDistance"]     = euclideanDistanceExtractor.extract(m, m1, m2);
    featureDetails["hausdorffDistance"]     = hausdorffDistanceExtractor.extract(m, m1, m2);
    featureDetails["parallelScore"]         = parallelScoreExtractor.extract(m, m1, m2);
    featureDetails["lengthScore"]           = lengthScoreExtractor.extract(m, m1, m2);
  }

  return featureDetails;
};
