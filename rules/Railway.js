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
// Use the default spacing of 5 meters.
var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor();
var hausdorffDistanceExtractor = new hoot.HausdorffDistanceExtractor();

// This contains the keys in a secondary feature that will trigger a one to many tag only transfer.
var oneToManyIdentifyingKeys = hoot.get("railway.one.to.many.identifying.keys").split(';');
// This contains the keys in a secondary feature that will be transferred to a reference feature
// one a one to many tag only transfer is triggered.
var oneToManyTransferKeys = hoot.get("railway.one.to.many.transfer.keys").split(';');
// Set the transfer keys on SelectiveOverwriteTag1Merger, as that's the tag merger we'll use for
// this.
hoot.set({'selective.overwrite.tag.merger.keys': oneToManyTransferKeys});
// Used to tag secondary features to identify them for removal post conflate; matches a var in
// MetadataTags
var oneToManySecondaryMatchTagKey = "hoot:railway:one:to:many:match:secondary";
// Determines whether the secondary feature gets deleted from output during merging.
var oneToManyDeleteSecondary = (hoot.get("railway.one.to.many.delete.secondary") === 'true');
if (oneToManyDeleteSecondary)
{
  hoot.prependToList({'conflate.post.ops': 'RailwayOneToManySecondaryMatchElementRemover'});
}
// Save off the default tag merger for merges not involving a one to many match.
var defaultTagMerger = hoot.get("tag.merger.default");
// The current one to many match identifying tag key.
var oneToManyTagKey;
// Bookkeeping for secondary match elements.
var oneToManySecondaryMatchElementIds = [];
// This maps secondary element IDs to the number of matches they're involved in.
var oneToManyMatches = {};

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

    /*
     Classifier derived using WEKA 3.8.2, then tweaked the numbers a bit. Classifier chosen was
     "REPTree".
    */
    var distanceScore = distanceScoreExtractor.extract(m, m1, m2);
    hoot.trace("distanceScore: " + distanceScore);
    if (distanceScore >= minDistanceScore)
    {
      var hausdorffDistance = hausdorffDistanceExtractor.extract(m, m1, m2);
      hoot.trace("hausdorffDistance: " + hausdorffDistance);
      if (hausdorffDistance >= minHausdorffDistanceScore)
      {
        var edgeDistance  = edgeDistanceExtractor.extract(m, m1, m2);
        hoot.trace("edgeDistance: " + edgeDistance);
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

  // No intra-dataset conflation is allowed.
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

  // Note: Attempts to do match filtering by type were initially unsuccessful, and no need has been
  // apparent *yet* to do match filtering via name.

  // These score thresholds were determined experimentally (see geometryMismatch) for the default
  // rail conflation routine.
  var minDistanceScore = 0.577;
  var minHausdorffDistanceScore = 0.8;
  var minEdgeDistanceScore = 0.8;

  // Check the secondary element to see if it has any of the one to many identifying tag keys.
  // Arbitrarily use the first one found (assumes that if more than one is found that they're in
  // agreement). If so, we'll perform a one to many tag transfer from the feature up to the number
  // of ref rails it matches with. Note that no geometry merging is done regardless of the type of
  // conflation being done. It *may* eventually make sense to only skip the geometry merging if
  // Attribute Conflation is selected and perform it when Reference Conflation is selected.
  var currentMatchAttemptIsOneToMany = false;
  oneToManyTagKey = tags2.getFirstMatchingKey(oneToManyIdentifyingKeys);
  hoot.trace("oneToManyTagKey: " + oneToManyTagKey);
  if (oneToManyTagKey !== "")
  {
    // Check the identifying tag to see how many rail tracks this single secondary feature represents.
    var totalMatchesAllowed = parseInt(String(tags2.get(oneToManyTagKey)));
    // If its a one to many and we've already made the total number of matches, then we won't make
    // anymore (may eventually find this check isn't needed if the rail data isn't super dense).
    if (parseInt(oneToManyMatches[e2.getElementId()]) >= totalMatchesAllowed)
    {
      hoot.trace(e2.getElementId() + " has already reached its maximum allowed matches: " + totalMatchesAllowed);
      return result;
    }

    currentMatchAttemptIsOneToMany = true;
    // If we're doing a one to many, we need to cover a wider swath to get all the matches, so
    // lessen the distance score. This value may need to be tweaked over time, as well as the other
    // score thresholds.
    minDistanceScore = 0.468;
  }

  // Check for a geometry match.
  if (geometryMismatch(map, e1, e2, minDistanceScore, minHausdorffDistanceScore, minEdgeDistanceScore))
  {
    return result;
  }

  hoot.trace("railway match");
  hoot.trace("currentMatchAttemptIsOneToMany: " + currentMatchAttemptIsOneToMany);
  if (currentMatchAttemptIsOneToMany)
  {
    // If we found a one to many match, increment our total matches for the secondary feature.
    var numTotalMatches = 1;
    if (e2.getElementId() in oneToManyMatches)
    {
      numTotalMatches = parseInt(oneToManyMatches[e2.getElementId()]) + 1;
    }
    oneToManyMatches[e2.getElementId()] = numTotalMatches;
    hoot.trace("total matches for " + e2.getElementId() + ": " + String(numTotalMatches));
    if (!oneToManySecondaryMatchElementIds.includes(String(e2.getElementId().toString())))
    {
      oneToManySecondaryMatchElementIds.push(String(e2.getElementId().toString()));
    }
    hoot.trace("oneToManySecondaryMatchElementIds: " + oneToManySecondaryMatchElementIds);
  }
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
  hoot.trace("Merging railways...");
  hoot.trace("oneToManySecondaryMatchElementIds: " + oneToManySecondaryMatchElementIds);
  var secondaryElementsIdsMerged = [];
  var oneToManyMergeOccurred = false;
  // If the current match is one to many, we're only bringing over tags from secondary to
  // reference in our specified list and we're doing no geometry merging.
  hoot.set({'tag.merger.default': 'SelectiveOverwriteTag1Merger'});
  hoot.trace("pairs.length: " + pairs.length);
  for (var i = 0; i < pairs.length; i++)
  {
    var elementIdPair = pairs[i];
    var element1 = map.getElement(elementIdPair[0]);
    var element2 = map.getElement(elementIdPair[1]);
    if (element1 && element2)
    {
      hoot.trace("element1.getElementId(): " + element1.getElementId());
      hoot.trace("element2.getElementId(): " + element2.getElementId());
    }
    if (element1 && element2 && oneToManySecondaryMatchElementIds.includes(String(element2.getElementId().toString())))
    {
      hoot.trace("Merging one to many tags for " + element1.getElementId() + " and " + element2.getElementId() + "...");
      var newTags = hoot.TagMergerFactory.mergeTags(element1.getTags(), element2.getTags());
      element1.setTags(newTags);
      element1.setStatusString("conflated");
      // Record the element merges.
      replaced.push(elementIdPair);
      // If we're deleting secondary match features, mark it for removal later.
      if (oneToManyDeleteSecondary)
      {
        element2.setTag(oneToManySecondaryMatchTagKey, "yes");
      }
      oneToManyMergeOccurred = true;
    }
  }
  hoot.trace("oneToManyMergeOccurred: " + oneToManyMergeOccurred);

  // *Think* this is the correct behavior...if we had any many to one merges earlier during this
  // merge call, then we shouldn't have any other types of merges to perform...not sure yet, though.
  if (!oneToManyMergeOccurred)
  {
    // If its not a one to many match, business as usual...use the conflation behavior built into
    // the linear merger in use. Snap the ways in the second input to the first input. Use the
    // default tag merge method.
    hoot.trace("Performing linear merge...");
    // Go back to the original default tag merger.
    hoot.set({'tag.merger.default': defaultTagMerger});
    return new hoot.LinearMerger().apply(sublineStringMatcher, map, pairs, replaced, exports.baseFeatureType);
  }
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var featureDetails = [];

  // Extract the sublines needed for matching.
  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    featureDetails["distanceScore"] = distanceScoreExtractor.extract(m, m1, m2);
    featureDetails["edgeDistance"] = edgeDistanceExtractor.extract(m, m1, m2);
    featureDetails["hausdorffDistance"] = hausdorffDistanceExtractor.extract(m, m1, m2);
  }

  return featureDetails;
};
