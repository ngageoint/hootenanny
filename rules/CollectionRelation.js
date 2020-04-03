/**
 * This script conflates collection relations (e.g route, administrative boundary relations, etc.).
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
// TODO: should this be line?
exports.geometryType = "polygon";

// This is needed for disabling superfluous conflate ops. In the future, it may also
// be used to replace exports.isMatchCandidate (see #3047).
exports.matchCandidateCriterion = "hoot::CollectionRelationCriterion";

var edgeDistanceExtractor = new hoot.EdgeDistanceExtractor();
var memberSimilarityExtractor = new hoot.RelationMemberSimilarityExtractor();
var nameExtractor = new hoot.NameExtractor();

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

function typeMismatch(e1, e2)
{
  var tags1 = e1.getTags();
  var tags2 = e2.getTags();
  var type1 = e1.getType();
  var type2 = e2.getType();
  
  hoot.trace("type1: " + type1);
  hoot.trace("type2: " + type2);
  hoot.trace("mostSpecificType 1: " + mostSpecificType(e1));
  hoot.trace("mostSpecificType 2: " + mostSpecificType(e2));

  if (type1 != type2)
  {    
    hoot.trace("type mismatch");
    return true;
  }
  else if (type1 == "boundary" && tags1.get("boundary") == "administrative" && tags2.get("boundary") == "administrative" && tags1.get("admin_level") != tags2.get("admin_level"))
  {
    hoot.trace("admin_level mismatch");
    return true;
  }
  else if ((type1 == "multipolygon" || type1 == "multilineString") && explicitTypeMismatch(e1, e2, exports.tagThreshold))
  {
    hoot.trace("multipoly/multilinestring type mismatch");
    return true;
  }
  else if (type1 == "restriction" && tags1.get("restriction") != tags2.get("restriction"))
  {
    hoot.trace("restriction mismatch");
    return true;
  }
  else if (type1 == "route" && tags1.get("route") != tags2.get("route"))
  {
    hoot.trace("route mismatch");
    return true;
  }
  return false;
}

function nameMismatch(map, e1, e2)
{
  var tags1 = e1.getTags();
  var tags2 = e2.getTags();
  var type1 = e1.getType();
  var type2 = e2.getType();

  var name = 1.0;
  // This may be too restrictive.
  if (type1 == "route" && tags1.get("route") == "road" && tags2.get("route") == "road")
  {
    var ref1 = tags1.get("ref");
    var ref2 = tags2.get("ref");
    hoot.trace("ref 1: " + ref1);
    hoot.trace("ref 2: " + ref2);
    if (ref1 != ref2)
    {
      hoot.trace("highway ref mismatch");
      name = 0.0;
    }
  }
  // only score the name if both have one
  else if (bothElementsHaveName(e1, e2))
  {
    hoot.trace("both elements have name");
    name = nameExtractor.extract(map, e1, e2);
  }

  if (name < exports.nameThreshold)
  {
    return true;
  }
  return false;
}

function geometryMismatch(map, e1, e2)
{
  // This can become a fairly expensive check against the collection relations if to many of
  // them are encountered because they aren't filtered out properly by type beforehand.
  var edgeDist = edgeDistanceExtractor.extract(map, e1, e2);
  hoot.trace("edgeDist: " + edgeDist);
  if (edgeDist < 0.97)
  { 
    hoot.trace("match failed on edge distance");
    return true;
  }

  // This hasn't panned out as being usable yet.
  //var memberSim = memberSimilarityExtractor.extract(map, e1, e2);

  return false;
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
  var result = { miss: 1.0 };

  if (e1.getStatusString() == e2.getStatusString()) 
  {
    return result;
  }

  var tags1 = e1.getTags();
  var tags2 = e2.getTags();

  hoot.trace("**********************************");
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
  
  // These were determined with a very small sample of test data and may need refinement. Also,
  // not all of the feature extractors support relations and, where possible, we may want to 
  // add support for them.

  if (typeMismatch(e1, e2))
  {
    return result;
  }
  else if (nameMismatch(map, e1, e2))
  {
    return result;
  }
  else if (geometryMismatch(map, e1, e2))
  {
    return result;
  }

  result = { match: 1.0, miss: 0.0, review: 0.0 };
  hoot.trace("match found");
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


