/**
 * This script conflates rivers using Generic Conflation.
 */

"use strict";

exports.description = "Matches linear waterways";
exports.experimental = false;
exports.baseFeatureType = "Waterway";
exports.geometryType = "line";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);

// This matcher only sets match/miss/review values to 1.0, therefore the score thresholds aren't used. 
// If that ever changes, then the generic score threshold configuration options used below should 
// be replaced with custom score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));
exports.nameThreshold = parseFloat(hoot.get("waterway.name.threshold"));

// This is needed for disabling superfluous conflate ops. In the future, it may also
// be used to replace exports.isMatchCandidate (see #3047). 
exports.matchCandidateCriterion = "hoot::LinearWaterwayCriterion";

// used during subline matching
var longWayThreshold = parseInt(hoot.get("waterway.long.way.threshold"));
var sublineMatcherName = hoot.get("waterway.subline.matcher");

var sublineMatcher =
  new hoot.MaximalSublineStringMatcher(
    { "way.matcher.max.angle": hoot.get("waterway.matcher.max.angle"),
      "way.subline.matcher": sublineMatcherName });
var frechetSublineMatcher =
  new hoot.MaximalSublineStringMatcher(
    { "way.matcher.max.angle": hoot.get("waterway.matcher.max.angle"),
      "way.subline.matcher": "hoot::FrechetSublineMatcher" });
var sampledAngleHistogramExtractor =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : hoot.get("waterway.angle.sample.distance"),
      "way.matcher.heading.delta" : hoot.get("waterway.matcher.heading.delta") });
var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor();;

var nameExtractor = new hoot.NameExtractor(
  new hoot.MaxWordSetDistance(
    {"token.separator": "[\\s-,';]+"},
    // runs just a little faster w/ tokenize off
    {"translate.string.distance.tokenize": "false"},
    new hoot.LevenshteinDistance(
      {"levenshtein.distance.alpha": 1.15})));

/**
 * Runs before match creation occurs and provides an opportunity to perform custom initialization.
 */
exports.calculateSearchRadius = function(map)
{
  var autoCalcSearchRadius = (hoot.get("waterway.auto.calc.search.radius") === 'true');
  if (autoCalcSearchRadius)
  {
    hoot.log("Calculating search radius for waterway conflation...");
    exports.searchRadius =
      parseFloat(
        calculateSearchRadiusUsingRubberSheeting(
          map,
          hoot.get("waterway.rubber.sheet.ref"),
          hoot.get("waterway.rubber.sheet.minimum.ties"),
          exports.matchCandidateCriterion));
  }
  else
  {
    exports.searchRadius = parseFloat(hoot.get("search.radius.waterway"));
    hoot.debug("Using specified search radius for waterway conflation: " + exports.searchRadius);
  }
}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return isLinearWaterway(e);
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

function explicitTypeMismatch(e1, e2)
{
  var tags1 = e1.getTags();
  var tags2 = e2.getTags();

  // This isn't foolproof as there could be other untranslated river identifying tags involved.
  var waterway1 = tags1.get("waterway");
  var waterway2 = tags2.get("waterway");
  if (waterway1 != 'undefined' && waterway1 != null && waterway1 != '' && 
      waterway2 != 'undefined' && waterway2 != null && waterway2 != '' && 
      waterway1 != waterway2)
  {
    return true;
  }

  return false;
}

function nameMismatch(map, e1, e2)
{
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
    return true;
  }

  return false;
}

function geometryMismatch(map, e1, e2)
{
  var longWays = false;
  var length1 = getLength(map, e1);
  // Let's cache this tag as metadata so we can use it during merging.
  
  var length2 = -1;
  if (length1 > exports.longWayThreshold)
  {
    longWays = true;
  }
  else
  {
    length2 = getLength(map, e2);
    if (length2 > exports.longWayThreshold)
    {
      longWays = true;
    }
  }

  var sublines;
  var sublineMatcherUsed =sublineMatcherName;
  if (!longWays)
  {
    sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  }
  else
  {
    // Longer ways are can be very computationally expensive with the default subline matching. 
    // Frechet subline matching is a little less accurate but much faster, so we'll
    // switch over to it for longer ways. If for some reason Frechet can't return any sublines,
    // we'll go ahead and do maximal subline matching...this *shouldn't* happen too often so
    // won't penalize runtime performance much.
    hoot.trace("Processing longer ways of length: " + length1 + ", " + length2 + "...");
    sublines = frechetSublineMatcher.extractMatchingSublines(map, e1, e2);
    if (!sublines)
    {
      sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
    }
    else
    {
      sublineMatcherUsed = "hoot::FrechetSublineMatcher";
    }
  }

  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    var weightedShapeDist = -1;
    var angleHist = sampledAngleHistogramExtractor.extract(m, m1, m2);
    if (angleHist == 0)
    {
      weightedShapeDist = weightedShapeDistanceExtractor.extract(m, m1, m2);
      if (weightedShapeDist > 0.861844)
      {
        //e1.getTags().set("hoot:subline:matcher:used", sublineMatcherUsed);
        //e2.getTags().set("hoot:subline:matcher:used", sublineMatcherUsed);
        return false;
      }
    }
    if (angleHist > 0)
    {
      //e1.getTags().set("hoot:subline:matcher:used", sublineMatcherUsed);
      //e2.getTags().set("hoot:subline:matcher:used", sublineMatcherUsed);
      return false;
    }
  }
  
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
  hoot.trace("mostSpecificType 1: " + mostSpecificType(e1));
  hoot.trace("mostSpecificType 2: " + mostSpecificType(e2));
  
  // This type and name checks are mostly here to help cull down the potential matches and avoid 
  // costly geometric comparisons for long features. The geometric comparison itself is fairly
  // accurate.
  if (explicitTypeMismatch(e1, e2))
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

function getSublineMatcherUsed(map, pairs)
{
  for (var i = 0; i < pairs.length; i++)
  { 
    var elementsStr = String(pairs[i]); // Way(-1),Way(-2)...
    var elementsStrParts = elementsStr.split(",");
    for (var j = 0; j < elementsStrParts.length; j++)
    {
      var elementIdStr = String(elementsStrParts[j]); // Way(-1)
      var element = map.getElement(elementIdStr);
      var length = 0;
      if (element)
      {
        var sublineMatcherUsed = element.getTags().get("hoot:subline:matcher:used");
        if (sublineMatcherUsed != '' && sublineMatcherUsed != null && sublineMatcherUsed != 'undefined')
        {
          return sublineMatcherUsed;
        }
      }
    }
  } 
  return "";
}

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
  // Snap the ways in the second input to the first input. Use the default tag
  // merge method.
  var snapSublineMatcher = sublineMatcher;
  // Pass the appropriate subline matcher based on what was used during matching.
  // TODO: Unfortunately this is crashing CollectionRelationMergeTest for an yet 
  // undetermined reason. Oddly, when only a river matcher is configured it doesn't crash.
  /*if (getSublineMatcherUsed(map, pairs) == "hoot::FrechetSublineMatcher")
  {
    snapSublineMatcher = frechetSublineMatcher
  }*/
  return snapWays(snapSublineMatcher, map, pairs, replaced, exports.baseFeatureType);
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

    featureDetails["sampledAngleHistogramValue"] = sampledAngleHistogramExtractor.extract(m, m1, m2);
    featureDetails["weightedShapeDistanceValue"] = weightedShapeDistanceExtractor.extract(m, m1, m2);
  }

  return featureDetails;
};

