/**
 * This script conflates collection relations (e.g route, administrative boundary relations, etc.). 
 * It is meant to be run after all other matchers.
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
var angleHistExtractor = new hoot.AngleHistogramExtractor();
// We may eventually want to try something other than the default name extractor here.
var nameExtractor = new hoot.NameExtractor();

var memberSimilarityExtractor = new hoot.RelationMemberSimilarityExtractor();

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

  // If the collection relations aren't filtered out properly by type beforehand the
  // geometry checks afterward can become very expensive.

  if (type1 != type2)
  {    
    hoot.trace("type mismatch; type1: " + type1 + "; type2: " + type2);
    hoot.trace("mostSpecificType 1: " + mostSpecificType(e1));
    hoot.trace("mostSpecificType 2: " + mostSpecificType(e2));
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

  var nameScore = 1.0;

  // This may end up being too restrictive...
  if (type1 == "route" && tags1.get("route") == "road" && tags2.get("route") == "road")
  {
    var ref1 = tags1.get("ref");
    var ref2 = tags2.get("ref");
    if (ref1 != ref2)
    {
      hoot.trace("highway ref mismatch; ref1: " + ref1 + ", ref2: " + ref2);
      nameScore = 0.0;
    }
  }
  // only score the name if both have one
  else if (bothElementsHaveName(e1, e2))
  {
    nameScore = nameExtractor.extract(map, e1, e2);
  }

  if (nameScore < exports.nameThreshold)
  {
    hoot.trace("name mismatch; score: " + nameScore);
    return true;
  }

  return false;
}

function geometryMismatch(map, e1, e2)
{
  // This is a little convoluted and may need further adjustment. Edge distance is pretty accurate 
  // for this but gets expensive as the relations get larger. Angle hist is a little less accurate
  // overall but runs faster and seems to be working ok for the largeer relations. For matching
  // of disjointed relations (relations with different but connecting ways) a further check is
  // needed (which also has the potential to be very expensive O(n^2)) and is only done for the
  // larger relations when the geometry check fails.

  // TODO: Should we be extracting sublines first and passing those to the extractors?

  var numRelationMemberNodes = getNumRelationMemberNodes(map, e1.getElementId()) + getNumRelationMemberNodes(map, e2.getElementId());
  if (numRelationMemberNodes < 2000) // Threshold determined off of one dataset...may need tweaking.
  {
    // This can become a fairly expensive check for relations with a lot of total nodes.
    var edgeDist = edgeDistanceExtractor.extract(map, e1, e2);
    if (edgeDist < 0.97)
    { 
      hoot.trace("match failed on edgeDist: " + edgeDist);
      return true;
    }
  }
  else
  {
    //hoot.trace("numRelationMemberNodes: " + numRelationMemberNodes);
    var angleHist = angleHistExtractor.extract(map, e1, e2);
    if (angleHist < 0.73)
    { 
      if (relationsHaveConnectedWayMembers(map, e1.getElementId(), e2.getElementId()))
      {
        hoot.trace("match failed on angleHist: " + angleHist + " but there are connected ways.");
      }
      else
      {
        hoot.trace("match failed on angleHist: " + angleHist);
        return true;
      }  
    }
  }
  return false;
}

function memberSimilarityMismatch(map, e1, e2)
{
  // This hasn't panned out as being useful yet. This is meant to recognize relations with almost 
  // identical members. We do encounter those, but the other checks (name, geometry) usually help 
  // identify them beforehand and makes this check unnecessary.

  var memberSim = memberSimilarityExtractor.extract(map, e1, e2);
  if (memberSim < 0.75)
  {
    hoot.trace("match failed on memberSim: " + memberSim);
    return true;
  }
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
  
  // These checks were determined with a very small sample of test data and no model was used, so 
  // may need further refinement.

  if (typeMismatch(e1, e2))
  {
    return result;
  }
  if (nameMismatch(map, e1, e2))
  {
    return result;
  }
  if (geometryMismatch(map, e1, e2))
  {
    return result;
  }

  hoot.trace("match");
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


