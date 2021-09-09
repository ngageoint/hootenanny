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
// TODO: create custom options for these
//exports.nameThreshold = parseFloat(hoot.get("river.name.threshold"));
//exports.typeThreshold = parseFloat(hoot.get("river.type.threshold"));

// This is needed for disabling superfluous conflate ops only. exports.isMatchCandidate handles
// culling match candidates.
exports.matchCandidateCriterion = "RailwayCriterion";

// We're just using the default max recursions here for MaximalSubline. May need to come up with a
// custom value via empirical testing. This will not work if we ever end up needing to pass map in
// here for this data type.
var sublineStringMatcher = hoot.SublineStringMatcherFactory.getMatcher(exports.baseFeatureType);

var distanceScoreExtractor = new hoot.DistanceScoreExtractor();
// Use the default spacing of 5 meters.
var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor();
var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor();

var nameExtractor = new hoot.NameExtractor(
  new hoot.MaxWordSetDistance(
    { "token.separator": "[\\s-,';]+" },
    // runs just a little faster w/ tokenize off
    { "translate.string.distance.tokenize": "false" },
    new hoot.LevenshteinDistance( { "levenshtein.distance.alpha": 1.15 } )));

var oneToManyIdentifyingKeys = hoot.get("railway.one.to.many.identifying.keys").split(';');
var oneToManyTransferKeys = hoot.get("railway.one.to.many.transfer.keys").split(';');
var oneToManyMergeGeometries = (hoot.get("railway.one.to.many.reference.merge.geometries") === 'true');
var currentMergeIsOneToMany = false;
var defaultTagMerger = hoot.get("tag.merger.default");
var defaultLinearMerger = hoot.get("geometry.linear.merger.default");

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

function nameMismatch(map, e1, e2)
{
  // TODO: move this to HootLib.js

  hoot.trace("Processing name...");

  var tags1 = e1.getTags();
  var tags2 = e2.getTags();

  var nameScore = 1.0;

  // only score the name if both have one
  if (bothElementsHaveName(e1, e2))
  {
    nameScore = nameExtractor.extract(map, e1, e2);
  }

  if (nameScore < exports.nameThreshold)
  {
    hoot.trace("Explict name mismatch: " + e1.getTags().get("name") + ", " + e2.getTags().get("name"));
    return true;
  }

  return false;
}

function geometryMismatch(map, e1, e2, minDistanceScore, minHausdorffDistanceScore, minEdgeDistanceScore)
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
    if (distanceScore >= minDistanceScore)
    {
      var hausdorffDistance = hausdorffDistanceExtractor.extract(m, m1, m2);
      if (hausdorffDistance >= minHausdorffDistanceScore)
      {
        var edgeDistance  = edgeDistanceExtractor.extract(m, m1, m2);
        if (edgeDistance >= minEdgeDistanceScore)
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
  hoot.trace("mostSpecificType 1: " + hoot.OsmSchema.mostSpecificType(e1));
  hoot.trace("mostSpecificType 2: " + hoot.OsmSchema.mostSpecificType(e2));

  // TODO: disabling this for now due to the typing on the regression test secondary data
  // If both features have types and they aren't just generic types, let's do a detailed type
  // comparison and look for an explicit type mismatch.
  /*var typeScorePassesThreshold = !hoot.OsmSchema.explicitTypeMismatch(e1, e2, exports.typeThreshold);
  hoot.trace("typeScorePassesThreshold: " + typeScorePassesThreshold);
  if (!typeScorePassesThreshold)
  {
    return result;
  }*/
  /*This is probably eventually a good idea, but don't have any test data to back up needing it
  yet.*/
  /*if (nameMismatch(map, e1, e2))
  {
    return result;
  }*/

  var minDistanceScore = 0.577;
  var minHausdorffDistanceScore = 0.8;
  var minEdgeDistanceScore = 0.8;

  // TODO
  var oneToManyTagKey = tags2.getFirstMatchingKey(oneToManyIdentifyingKeys);
  hoot.trace("oneToManyTagKey: " + oneToManyTagKey);
  if (oneToManyTagKey !== "")
  {
    currentMergeIsOneToMany = true;
    minDistanceScore = 0.577;
    minHausdorffDistanceScore = 0.8;
    minEdgeDistanceScore = 0.8;
  }
  else
  {
    currentMergeIsOneToMany = false;
  }

  if (geometryMismatch(map, e1, e2, minDistanceScore, minHausdorffDistanceScore, minEdgeDistanceScore))
  {
    return result;
  }

  hoot.trace("match");
  result = { match: 1.0, explain:"match" };
  return result;
};

/**
 * The internals of geometry merging can become quite complex. Typically this method will simply
   call another hoot method to perform the appropriate merging of geometries.
 *
 * If this method is exported then the mergePair method should not be exported.
 *
 * @param map The map that is being conflated
 * @param pairs An array of ElementId pairs that will be merged.
 * @param replaced An empty array is passed in, the method should fill the array with all the
   replacements that occur during the merge process (e.g. if two elements (way:1 & way:2) are merged
   into one element (way:3), then the replaced array should contain [[way:1, way:3], [way:1, way:3]]
   where all the "way:*" objects are of the ElementId type.
 */
exports.mergeSets = function(map, pairs, replaced)
{
  hoot.trace("oneToManyMergeGeometries: " + oneToManyMergeGeometries);
  hoot.trace("currentMergeIsOneToMany: " + currentMergeIsOneToMany);

  if (currentMergeIsOneToMany)
  {
    hoot.trace("oneToManyTransferKeys: " + oneToManyTransferKeys);
    hoot.set({'tag.merger.default': 'SelectiveOverwriteTag1Merger'});
    hoot.set({'selected.overwrite.tag.merger.keys': oneToManyTransferKeys});

    if (!oneToManyMergeGeometries)
    {
      hoot.set({'geometry.linear.merger.default': 'LinearTagOnlyMerger'});
    }
    else
    {
      hoot.set({'geometry.linear.merger.default': defaultLinearMerger});
    }
  }
  else
  {
    hoot.set({'tag.merger.default': defaultTagMerger});
    var empty = [];
    hoot.set({'selected.overwrite.tag.merger.keys': empty});
    hoot.set({'geometry.linear.merger.default': defaultLinearMerger});
  }

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

    featureDetails["distanceScore"]         = distanceScoreExtractor.extract(m, m1, m2);
    featureDetails["edgeDistance"]          = edgeDistanceExtractor.extract(m, m1, m2);
    featureDetails["hausdorffDistance"]     = hausdorffDistanceExtractor.extract(m, m1, m2);
  }

  return featureDetails;
};
