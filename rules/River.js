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
exports.typeThreshold = parseFloat(hoot.get("waterway.type.threshold"));

// This is needed for disabling superfluous conflate ops. In the future, it may also
// be used to replace exports.isMatchCandidate (see #3047). 
exports.matchCandidateCriterion = "hoot::LinearWaterwayCriterion";

// used during subline matching
var sublineMatcherName = hoot.get("waterway.subline.matcher");
var sublineMatcher; // default subline matcher; gets set up in calculateSearchRadius function
var frechetSublineMatcher = // we'll switch over to this one if the default matcher runs too slowly
  new hoot.MaximalSublineStringMatcher(
    { "way.matcher.max.angle": hoot.get("waterway.matcher.max.angle"),
      "way.subline.matcher": "hoot::FrechetSublineMatcher" }); 

var sampledAngleHistogramExtractor =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : hoot.get("waterway.angle.sample.distance"),
      "way.matcher.heading.delta" : hoot.get("waterway.matcher.heading.delta"),
      "angle.histogram.extractor.process.relations" : "false" });
var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor();
var nameExtractor = new hoot.NameExtractor(
  new hoot.MaxWordSetDistance(
    { "token.separator": "[\\s-,';]+" },
    // runs just a little faster w/ tokenize off
    { "translate.string.distance.tokenize": "false" },
    new hoot.LevenshteinDistance( { "levenshtein.distance.alpha": 1.15 } )));

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

  // We need to configure the maximal subline matcher to not have runaway recursion when 
  // matching sublines. This is done based on the total length of all rivers in the input data.
  // This isn't the best place to put this, but there's nowhere convenient in the C++ to do it, 
  // and this is the only exported method that takes in a map and runs before the matching, 
  // so it will do.
  var maxRecursions = getRiverMaxSublineRecursions(map);
  sublineMatcher =
    new hoot.MaximalSublineStringMatcher(
      { "way.matcher.max.angle": hoot.get("waterway.matcher.max.angle"),
        "way.subline.matcher": sublineMatcherName,
        "maximal.subline.max.recursions": maxRecursions });
}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  hoot.trace("e: " + e.getElementId());
  hoot.trace("isLinearWaterway: " + isLinearWaterway(e));

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

function nameMismatch(map, e1, e2)
{
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

function geometryMismatch(map, e1, e2)
{
  hoot.trace("Processing geometry...");

  var sublines;
  // Try matching with our default subline matcher, which may be more accurate, but slower for complex features.
  hoot.trace("Extracting sublines with default...");
  sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  hoot.trace(sublines);
  if (sublines && sublines == "RecursiveComplexityException")
  {
    // If we receive the specfic string above from the matching routine, we know our subline matcher
    // hit the cap on the number of recursive calls we allow for it 
    // (see waterway.maximal.subline.max.recursive.complexity above; A little kludgy, but not sure 
    // how to handle hoot exceptions in a js script at this point). So, now we'll try a backup matcher
    // that may be a little less accurate but much faster. Previously tried tweaking the configuration 
    // of MaximalSublineMatcher for performance instead of using this approach, but it didn't help.
    hoot.trace("Extracting sublines with Frechet...");
    sublines = frechetSublineMatcher.extractMatchingSublines(map, e1, e2);
  }

  if (sublines)
  {
    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    var weightedShapeDist = -1;
    hoot.trace("Getting angleHist...");
    var angleHist = sampledAngleHistogramExtractor.extract(m, m1, m2);
    hoot.trace("angleHist: " + angleHist);
    if (angleHist == 0)
    {
      hoot.trace("Getting weightedShapeDist...");
      weightedShapeDist = weightedShapeDistanceExtractor.extract(m, m1, m2);
      hoot.trace("weightedShapeDist: " + weightedShapeDist);
      if (weightedShapeDist > 0.861844)
      {
        hoot.trace("geometry match");
        return false;
      }
    }
    if (angleHist > 0)
    {
      hoot.trace("geometry match");
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
  hoot.trace("mostSpecificType 1: " + mostSpecificType(e1));
  hoot.trace("mostSpecificType 2: " + mostSpecificType(e2));
  
  // If both features have types and they aren't just generic types, let's do a detailed type comparison and 
  // look for an explicit type mismatch.
  var typeScorePassesThreshold = !explicitTypeMismatch(e1, e2, exports.typeThreshold);
  hoot.trace("typeScorePassesThreshold: " + typeScorePassesThreshold);
  if (!typeScorePassesThreshold)
  {
    return result;
  }
  hoot.trace("mostSpecificType(e1): " + mostSpecificType(e1));
  hoot.trace("mostSpecificType(e2): " + mostSpecificType(e2));
  if (nameMismatch(map, e1, e2))
  {
    return result;
  }
  if (geometryMismatch(map, e1, e2))
  {
    return result;
  }

  result = { match: 1.0, miss: 0.0, review: 0.0 };
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
  // Snap the ways in the second input to the first input and use the default tag merge method. 

  // Feature matching also occurs during the merging phase. Since its not possible to know the 
  // original subline matcher used during matching, pass in both of the possible subline matchers 
  // that could have been used and use the same internal core logic that was used during matching to 
  // determine which one to use now.
  return snapWays2(sublineMatcher, map, pairs, replaced, exports.baseFeatureType, frechetSublineMatcher);
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

