"use strict";

var MATCH_OVERLAP_THRESHOLD = 0.75;
var MISS_OVERLAP_THRESHOLD = 0.15;

exports.description = "Railways";
exports.experimental = false;
exports.baseFeatureType = "Railway";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("railway.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("railway.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("railway.review.threshold"));

var sublineMatcher =
  new hoot.MaximalSublineStringMatcher(
    { "way.matcher.max.angle": hoot.get("railway.matcher.max.angle"),
      "way.subline.matcher": hoot.get("railway.subline.matcher") });
var sampledAngleHistogramExtractor =
  new hoot.SampledAngleHistogramExtractor(
    { "way.angle.sample.distance" : hoot.get("railway.angle.sample.distance"),
      "way.matcher.heading.delta" : hoot.get("railway.matcher.heading.delta") });
var weightedShapeDistanceExtractor = new hoot.WeightedShapeDistanceExtractor();

/**
 * Runs before match creation occurs and provides an opportunity to perform custom initialization.
 */
exports.calculateSearchRadius = function(map)
{
  exports.searchRadius = parseFloat(hoot.get("search.radius.railway"));
  hoot.log("Using specified search radius for railway conflation: " + exports.searchRadius);
}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on.
 */
exports.isMatchCandidate = function(map, e)
{
  return isRailway(e);
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

  if (e1.getStatusString() === e2.getStatusString()) {
    return result;
  }

  // extract the sublines needed for matching
  var sublines = sublineMatcher.extractMatchingSublines(map, e1, e2);
  if (sublines) {
    //result = { match: 1.0, explain:"match" };
    //return result;

    var m = sublines.map;
    var m1 = sublines.match1;
    var m2 = sublines.match2;

    var sampledAngleHistogramValue = sampledAngleHistogramExtractor.extract(m, m1, m2);
    var weightedShapeDistanceValue = weightedShapeDistanceExtractor.extract(m, m1, m2);

    var attribs = [sampledAngleHistogramValue, weightedShapeDistanceValue];
    var classification = WekaClassifier.classify(attribs);
    if (1 === classification) {
      hoot.trace("Found Match!");
      result = { match: 1.0, explain:"match" };
    }
  }

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
  hoot.trace("Merging elements.");
  // snap the ways in the second input to the first input. Use the default tag
  // merge method.
  return snapWays(sublineMatcher, map, pairs, replaced);
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



/* Classifier derived using Weka 3.8
 * Using a manually-matched dataset which was probably too small
 * REPTree classifier
 *
 * === Summary ===
 * Correctly Classified Instances         369          79.3548 %
 * Incorrectly Classified Instances        96          20.6452 %
 * Kappa statistic                          0.1823
 * Mean absolute error                      0.192
 * Root mean squared error                  0.316
 * Relative absolute error                 86.5813 %
 * Root relative squared error             95.2511 %
 * Total Number of Instances              465
 *
 * Argument should be Object[2] = { Double sampledAngleHistogramValue,
 *                                  Double weightedShapeDistanceValue }
 */

class WekaClassifier {

  static classify(i) {
    var p = NaN;
    p = WekaClassifier.Nbf90f236(i);
    return p;
  }

  static Nbf90f236(i) {
    var p = NaN;
    // weightedShapeDistanceValue
    if (i[1] == null) {
      p = 1;
    } else if (i[1] < 0.9996969548964832) {
      p = 1;
    } else if (true) {
    p = WekaClassifier.Na1878427(i);
    }
    return p;
  }
  static Na1878427(i) {
    var p = NaN;
    // sampledAngleHistogramValue
    if (i[0] == null) {
      p = 1;
    } else if (i[0] < 0.9999867882954654) {
    p = WekaClassifier.N4649dd508(i);
    } else if (true) {
      p = 1;
    }
    return p;
  }
  static N4649dd508(i) {
    var p = NaN;
    // sampledAngleHistogramValue
    if (i[0] == null) {
      p = 0;
    } else if (i[0] < 0.9994555988547049) {
    p = WekaClassifier.N300d0e419(i);
    } else if (true) {
      p = 0;
    }
    return p;
  }
  static N300d0e419(i) {
    var p = NaN;
    // weightedShapeDistanceValue
    if (i[1] == null) {
      p = 1;
    } else if (i[1] < 0.9999996992930555) {
    p = WekaClassifier.N734e8f7410(i);
    } else if (true) {
      p = 1;
    }
    return p;
  }
  static N734e8f7410(i) {
    var p = NaN;
    // weightedShapeDistanceValue
    if (i[1] == null) {
      p = 1;
    } else if (i[1] < 0.9999980378189885) {
    p = WekaClassifier.N3add333711(i);
    } else if (true) {
      p = 0;
    }
    return p;
  }
  static N3add333711(i) {
    var p = NaN;
    // weightedShapeDistanceValue
    if (i[1] == null) {
      p = 1;
    } else if (i[1] < 0.9997304137129573) {
      p = 0;
    } else if (true) {
    p = WekaClassifier.N3276ca6112(i);
    }
    return p;
  }
  static N3276ca6112(i) {
    var p = NaN;
    // sampledAngleHistogramValue
    if (i[0] == null) {
      p = 1;
    } else if (i[0] < 0.6118379265697682) {
      p = 1;
    } else if (true) {
    p = WekaClassifier.N6ad1b56813(i);
    }
    return p;
  }
  static N6ad1b56813(i) {
    var p = NaN;
    // sampledAngleHistogramValue
    if (i[0] == null) {
      p = 1;
    } else if (i[0] < 0.9545753445646151) {
    p = WekaClassifier.N6de3b26914(i);
    } else if (true) {
      p = 1;
    }
    return p;
  }
  static N6de3b26914(i) {
    var p = NaN;
    // sampledAngleHistogramValue
    if (i[0] == null) {
      p = 0;
    } else if (i[0] < 0.908430154556233) {
      p = 1;
    } else if (true) {
      p = 0;
    }
    return p;
  }
}
