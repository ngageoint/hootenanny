/**
 * This is the conflate script for POI Conflation.
 */

exports.description = "Matches POIs";
exports.experimental = false;
exports.baseFeatureType = "POI";
exports.geometryType = "point";
exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.searchRadius = parseFloat(hoot.get("search.radius.poi"));
// This sets up custom search radii for selected feature types (see conf/core/PoiSearchRadii.json).
var searchRadii = hoot.PoiSearchRadius.getSearchRadii();
// This is needed for disabling superfluous conflate ops only. exports.isMatchCandidate handles
// culling match candidates.
exports.matchCandidateCriterion = "PoiCriterion";

// This matcher only sets match/miss/review values to 1.0. Therefore, we just use the default
// conflate thresholds and they're effectively ignored. If more custom values are ever required,
// then the generic score threshold configuration options used below should be replaced with custom
// score threshold configuration options.
exports.matchThreshold = parseFloat(hoot.get("conflate.match.threshold.default"));
exports.missThreshold = parseFloat(hoot.get("conflate.miss.threshold.default"));
exports.reviewThreshold = parseFloat(hoot.get("conflate.review.threshold.default"));

// name matchers
var soundexExtractor = new hoot.NameExtractor(
    new hoot.Soundex());
var translateMeanWordSetLevenshtein_1_5 = new hoot.NameExtractor(
    new hoot.MeanWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.LevenshteinDistance({"levenshtein.distance.alpha": 1.5})));
var translateMaxWordSetLevenshtein_1_15 = new hoot.NameExtractor(
    new hoot.MaxWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.ToEnglishTranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));
var translateMinWordSetLevenshtein_1_15 = new hoot.NameExtractor(
    new hoot.MinSumWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.ToEnglishTranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));
var weightedWordDistance = new hoot.NameExtractor(
    new hoot.WeightedWordDistance(
        {"token.separator": "[\\s-,';]+", "weighted.word.distance.probability": 0.5},
        new hoot.ToEnglishTranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.5}))));

// used for debugging
exports.writeDebugTags = hoot.get("writer.include.debug.tags");
// used for conflate provenance
exports.writeMatchedBy = hoot.get("writer.include.matched.by.tag");

function distance(e1, e2) {
    return Math.sqrt(Math.pow(e1.getX() - e2.getX(), 2) +
        Math.pow(e1.getY() - e2.getY(), 2));
}

function isSuperClose(e1, e2) {
    var r1 = exports.getSearchRadius(e1);
    var r2 = exports.getSearchRadius(e2);
    var r = Math.min(r1, r2);
    var d = distance(e1, e2);
    var result = false;
    if (d <= r / 5) {
        result = true;
    }
    return result;
}

exports.getSearchRadius = function(e) 
{
  if (exports.searchRadius == -1.0)
  {
    // If no search radius was defined by the config (it was set to default val of -1.0), then use
    // our distance table to calculate it.

    hoot.trace("Using custom search radius function...");
    
    var tags = e.getTags();
    var radius = e.getCircularError();
    //hoot.trace("radius start: " + radius);

    for (var i = 0; i < searchRadii.length; i++)
    {
      hoot.trace("searchRadii[i].key: " + searchRadii[i].key);
      hoot.trace("searchRadii[i].value: " + searchRadii[i].value);
      if (tags.contains(searchRadii[i].key) &&
          (searchRadii[i].value == undefined ||
           tags.get(searchRadii[i].key) == searchRadii[i].value))
      {
        hoot.trace("searchRadii[i].distance: " + searchRadii[i].distance);
        radius = Math.max(radius, searchRadii[i].distance);
      }
    }

    hoot.trace("calculated search radius: " + radius);
    return radius;
  }
  else
  {
    hoot.trace("Returning user defined search radius: " + exports.searchRadius + "...");
    return exports.searchRadius;
  }   
}

/**
 * Returns true if e is a candidate for a match. Implementing this method is optional, but may
   dramatically increase speed if you can cull some features early on. E.g. no need to check nodes
   for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e)
{
  return hoot.OsmSchema.isPoi(e);
};

/**
 * If this function returns true then all overlapping matches will be treated as a group. For now
   that means if two matches overlap then the whole group will be marked as needing review.
 *
 * If this function returns false the conflation routines will attempt to pick the best subset of
   matches that do not conflict.
 */
exports.isWholeGroup = function() {
    return true;
};

var typeTags = {
    related: {
        "cuisine": 1,
        "sport": 1,
        // power is accounted for in a unique way
        "power": 0
    },
    category: {
        "poi": 1
    }
};

/**
 * Returns true if one of the typeTags are found in the specified element.
 */
function hasTypeTag(t) {
    for (k in typeTags.related) {
        if (getRelatedTags(k, t).length > 0) {
            return true;
        }
    }
    for (k in typeTags.category) {
        if (getTagsByCategory(k, t).length > 0) {
            return true;
        }
    }

    return false;
}

function additiveScore(map, e1, e2) {
    var result = {};
    result.score = 0;
    result.reasons = [];

    var reason = result.reasons;

    var ignoreType = false;
    hoot.trace("hasName(e1): " + hoot.OsmSchema.hasName(e1));
    hoot.trace("hasName(e2): " + hoot.OsmSchema.hasName(e2));
    if (hoot.get("poi.ignore.type.if.name.present") == 'true' && hoot.OsmSchema.hasName(e1) && hoot.OsmSchema.hasName(e2))
    {
      ignoreType = true;
    }
    hoot.trace("ignoreType: " + ignoreType);

    var t1 = e1.getTags().toDict();
    var t2 = e2.getTags().toDict();

    // if there is no type information to compare the name becomes more
    // important
    var oneGeneric = hasTypeTag(t1) == false || hasTypeTag(t2) == false;
    if (oneGeneric)
    {
      hoot.trace("One element in the pair is generic.");
    }

    var e1SearchRadius = exports.getSearchRadius(e1);
    hoot.trace("e1SearchRadius: " + e1SearchRadius);
    var e2SearchRadius = exports.getSearchRadius(e2);
    hoot.trace("e2SearchRadius: " + e2SearchRadius);
    var searchRadius;
    if (oneGeneric)
    {
      searchRadius = Math.max(e1SearchRadius, e2SearchRadius);
    }
    else
    {
      searchRadius = Math.min(e1SearchRadius, e2SearchRadius);
    }
    hoot.trace("searchRadius: " + searchRadius);

    var d = distance(e1, e2);
    hoot.trace("d: " + d);

    if (d > searchRadius)
    {
      hoot.trace(
        "distance: " + d + " greater than search radius: " + searchRadius + "; returning score: " +
        result.score);
      return result;
    }

    var mean = translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
    var weightedWordDistanceScore = weightedWordDistance.extract(map, e1, e2);
    var weightedPlusMean = mean + weightedWordDistanceScore;
    var poiDistance = 1.0;
    var artworkTypeDistance = 1.0;
    var cuisineDistance = 1.0;
    var sportDistance = 1.0;
    if (!ignoreType)
    {
      poiDistance = getTagCategoryDistance("poi", map, e1, e2);
      artworkTypeDistance = getTagAncestorDistance("artwork_type", map, e1, e2);
      cuisineDistance = getTagAncestorDistance("cuisine", map, e1, e2);
      sportDistance = getTagAncestorDistance("sport", map, e1, e2);
    }

    var score = 0;

    if (!oneGeneric) {
        if (weightedPlusMean > 0.987403 && weightedPlusMean < 1.2) {
            score += 0.5;
            reason.push("similar names");
        } else if (weightedPlusMean >= 1.2) {
            score += 1;
            reason.push("very similar names");
        }
    } else {
        var min = translateMinWordSetLevenshtein_1_15.extract(map, e1, e2);

        // if there is no type information be very restrictive, but just the name can be enough
        // information for a match.
        if (min > 0.8 && weightedPlusMean >= 1.2) {
            score += 2;
            reason.push("very similar names and generic type");

        // with no type information just a similar name is enough to flag a review.
        } else if (weightedPlusMean > 0.987403) {
            score += 1;
            reason.push("similar names and generic type");
        }
    }

    var tags1 = e1.getTags();
    var tags2 = e2.getTags();

    if (isSuperClose(e1, e2)) 
    {
      // Adding a list here of things that don't normally have names and we want them to have a
      // better chance of matching if they are close together and their types match exactly.
      // Specifically, choosing to handle railway=level_crossing as a POI rather than as part of
      // railway conflation as its easier to implement and there are several other railway POI types
      // being used. You *could* handle it as part of railway conflation, though.
      if ((tags1.get("railway") == "level_crossing" && tags2.get("railway") == "level_crossing"))
      {
        score += 1.0;
      }
      else
      {
        score += 0.5;
      }
      reason.push("very close together");
    }

    var typeScore = 0;
    if (artworkTypeDistance <= 0.3) {
        typeScore += 1;
        reason.push("similar artwork type");
    }
    if (cuisineDistance <= 0.3) {
        typeScore += 1;
        reason.push("similar cuisine");
    }
    if (sportDistance <= 0.3) {
        typeScore += 1;
        reason.push("similar sport");
    }

    // we're unlikely to get more evidence than the fact that it is a tower or pole. If the power
    // tag matches exactly, give it 2 points of evidence; if not, just give it one.
    var powerDistance = 1.0;
    if (!ignoreType)
    {
      powerDistance = getTagDistance("power", t1, t2);
    }

    if (powerDistance == 0) {
        typeScore += 2;
        reason.push("same power (electrical) type");
    } else if (powerDistance <= 0.4) {
        typeScore += 1;
        reason.push("similar power (electrical) type");
    }

    // if at least one feature contains a place
    var placeCount = 0;
    if (!ignoreType)
    {
      placeCount = getTagsByAncestor("place", t1).length + getTagsByAncestor("place", t2).length;
    }

    // if at least one of the points has a place and neither of them are
    // generic poi types
    if (placeCount > 0 && oneGeneric == false) {
        var d = getTagDistance("place", t1, t2);
        // if the places don't match
        if (d == undefined) {
            // don't give name similarity or proximity a high weight
            score = Math.min(0.5, score);
            reason.push('no place match');
        // if the places are very dissimilar
        } else if (d > 0.8) {
            // don't give name similarity or proximity a high weight
            score = Math.min(0.5, score);
            reason.push('poor place match');
        // else if the places match, only increase score if the names match too.
        } else if (weightedPlusMean > 0.987403) {
            if (poiDistance <= 0.5) {
                score += 1;
                reason.push("similar name and place type");
            }
        }
    // if one is generic then we shouldn't match it outright.
    } else if (placeCount > 0 && oneGeneric) {
        score = Math.min(0.6, score);
        reason.push('generic type to place match');
    } else if (poiDistance <= 0.5) {
        score += 1;
        reason.push("similar POI type");
    // if the poi distance is very high, then they shouldn't be considered
    // for match based solely on name and proximity. See #6998
    } else if (poiDistance >= 0.99 && typeScore == 0 && oneGeneric == false) {
        score = 0;
        reason = ["similar names but no POI match"];
    }

    score = score + typeScore;

    result.score = score;
    result.reasons = reason;

    hoot.trace("score: " + result.score);
    hoot.trace("reasons: " + result.reasons);
    return result;
}

var totalCount = 0;

/**
 * Returns the match score for the three class relationships.
 * - match
 * - miss
 * - review
 *
 * The scores should always sum to one. If they don't you will be taunted mercilessly and we'll
   normalize it anyway. :P
 */
exports.matchScore = function(map, e1, e2) {
    totalCount += 1;
    var result = { miss: 1.0, explain:'Miss' };

    if (e1.getStatusString() == e2.getStatusString()) 
    {
      hoot.trace("same statuses: miss");
      return result;
    }

    hoot.trace("e1: " + e1.getElementId() + ", " + e1.getTags().get("name"));
    if (e1.getTags().get("note"))
    {
      hoot.trace("e1 note: " + e1.getTags().get("note"));
    }
    hoot.trace("e2: " + e2.getElementId() + ", " + e2.getTags().get("name"));
    if (e2.getTags().get("note"))
    {
      hoot.trace("e2 note: " + e2.getTags().get("note"));
    }

    var additiveResult = additiveScore(map, e1, e2);
    var score = additiveResult.score;
    hoot.trace("score: " + score);
    var reasons = additiveResult.reasons;
    hoot.trace("reasons: " + reasons);
    var d = "(" + prettyNumber(distance(e1, e2)) + "m)";

    var matchScore;
    var classification;
    if (score <= 0.5) {
        matchScore = {miss: 1, explain: 'Not much evidence of a match ' + d};
        classification = 'miss';
    } else if (score < 1.9) {
        matchScore = {review: 1, explain: "Somewhat similar " + d + " - " + reasons.join(", ") };
        classification = 'review';
    } else {
        matchScore = {match: 1, explain: "Very similar " + d + " - " + reasons.join(", ") };
        classification = 'match';
    }
    hoot.trace("explanation: " + matchScore.explain);
    hoot.trace("classification: " + classification);

    return matchScore;
};

exports.mergePair = function(map, e1, e2)
{
  // replace instances of e2 with e1 and merge tags
  mergeElements(map, e1, e2);
  e1.setStatusString("conflated");
  if (exports.writeDebugTags == "true" && exports.writeMatchedBy == "true")
  {
    // Technically, we should get this key from MetadataTags, but that's not integrated with hoot yet.
    e1.setTag("hoot:matchedBy", exports.baseFeatureType);
  }
  return e1;
};

/**
 * This method isn't being used yet. In future work we may use this method to generate a supervised
 * model.
 */
exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var fd = [];

  // before you use this list update it with more relevant features in the additiveScore() function.
  fd["soundex"] = soundexExtractor.extract(map, e1, e2);
  fd["mean"] = translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
  fd["max"] = translateMaxWordSetLevenshtein_1_15.extract(map, e1, e2);
  fd['weightedWordDistance'] = weightedWordDistance.extract(map, e1, e2);
  fd['weightedPlusMean'] = weightedWordDistance.extract(map, e1, e2) +
    translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
  fd['cuisine'] = getCuisineDistance(e1, e2);
  fd['additive'] = additiveScore(map, e1, e2).score;

  return fd;
};

/**
 * Given a number return a result that contains no more than 2 significant digits and no more than 1
   digit after the decimal place. For example:
 *
 * 123.456 -> 120
 * 1.234 -> 1.2
 * 0.123 -> 0.1
 * 1234567.89 -> 1200000
 */
function prettyNumber(n) {
    var digits = String(Math.round(n)).length;
    // a number that represents the number of digits we're rounding (e.g. for
    // 1234567.89 this will be 100000
    var f = Math.pow(10, digits - 2);
    // divide n by f, (12.3456789), round (12), then multiple by up, 1200000
    var r = Math.round(n / f) * f;
    var result;

    // I don't use the String conversion here b/c it sometimes gives weird
    // floating point errors.

    // if this is a small number remove the extra decimal places
    if (r < 10) {
        result = r.toFixed(1);
    } else {
        result = r.toFixed(0);
    }
    return result;
}

