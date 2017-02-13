
/*
 * This generic conflation script supports conflation of POI data
 */

exports.description = "POI Generic";
exports.experimental = false;
exports.baseFeatureType = "POI";

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("poi.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("poi.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("poi.review.threshold"));
exports.searchRadius = -1.0;

var soundexExtractor = new hoot.NameExtractor(
    new hoot.Soundex());
var translateMeanWordSetLevenshtein_1_5 = new hoot.NameExtractor(
    new hoot.MeanWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.LevenshteinDistance({"levenshtein.distance.alpha": 1.5})));
var translateMaxWordSetLevenshtein_1_15 = new hoot.NameExtractor(
    new hoot.MaxWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));
var translateMinWordSetLevenshtein_1_15 = new hoot.NameExtractor(
    new hoot.MinSumWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.15}))));
var weightedWordDistance = new hoot.NameExtractor(
    new hoot.WeightedWordDistance(
        {"token.separator": "[\\s-,';]+", "weighted.word.distance.probability": 0.5},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.5}))));

var distances = [

    {k:'amenity',                             match:100,      review:200},
    {k:'amenity',  v:'grave_yard',            match:500,      review:1000},
    {k:'building',                            match:100,      review:200},
    {k:'building',  v:'hospital',             match:300,      review:500},
    {k:'building',  v:'train_station',        match:500,      review:1000},
    {k:'barrier',   v:'toll_booth',           match:25,       review:50},
    {k:'barrier',   v:'border_control',       match:50,       review:100},
    {k:'historic',                            match:100,      review:200},
    {k:'landuse',                             match:500,      review:1000},
    {k:'landuse',   v:'built_up_area',        match:2000,     review:3000},
    {k:'leisure',                             match:250,      review:500},
    {k:'man_made',                            match:100,      review:200},
    {k:'natural',                             match:500,      review:1000},
    {k:'place',                               match:500,      review:1000},
    {k:'place',     v:'built_up_area',        match:2000,     review:3000},
    {k:'place',     v:'locality',             match:2000,     review:3000},
    {k:'place',     v:'populated',            match:2000,     review:3000},
    {k:'place',     v:'region',               match:1000,     review:2000},
    {k:'place',     v:'village',              match:2000,     review:3000},
    {k:'power',                               match:25,       review:50},
    {k:'railway',                             match:250,      review:500},
    {k:'railway',   v:'station',              match:500,      review:1000},
    {k:'shop',                                match:100,      review:200},
    {k:'sport',                               match:50,       review:100},
    {k:'station',                             match:100,      review:200},
    {k:'station',   v:'light_rail',           match:500,      review:1000},
    {k:'tourism',                             match:100,      review:200},
    // hotel campuses can be quite large
    {k:'tourism',   v:'hotel',                match:200,      review:400},
    {k:'transport',  v:'station',             match:500,      review:1000},

];

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

exports.getSearchRadius = function(e) {
    var tags = e.getTags();

    var radius = e.getCircularError();
    hoot.trace("radius start: " + radius);

    for (var i = 0; i < distances.length; i++) {
        if (tags.contains(distances[i].k) &&
            (distances[i].v == undefined ||
             tags.get(distances[i].k) == distances[i].v)) {
            //hoot.debug("distances[i].review: " + distances[i].review);
            radius = Math.max(radius, distances[i].review);
        }
    }

    hoot.trace("radius final: " + radius);

    return radius;
}

/**
 * Returns true if e is a candidate for a match. Implementing this method is
 * optional, but may dramatically increase speed if you can cull some features
 * early on. E.g. no need to check nodes for a polygon to polygon match.
 */
exports.isMatchCandidate = function(map, e) {
    return isPoi(e);
};

/**
 * If this function returns true then all overlapping matches will be treated
 * as a group. For now that means if two matches overlap then the whole group
 * will be marked as needing review.
 *
 * If this function returns false the conflation routines will attempt to
 * pick the best subset of matches that do not conflict.
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
function hasTypeTag(e1) {
    for (k in typeTags.related) {
        if (getRelatedTags(k, e1.getTags().toDict()).length > 0) {
            return true;
        }
    }
    for (k in typeTags.category) {
        if (getTagsByCategory(k, e1.getTags().toDict()).length > 0) {
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

    var t1 = e1.getTags().toDict();
    var t2 = e2.getTags().toDict();

    // if there is no type information to compare the name becomes more
    // important
    var oneGeneric = hasTypeTag(e1) == false || hasTypeTag(e2) == false;
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

    var d = distance(e1, e2);

    if (d > searchRadius)
    {
        hoot.trace("e1: " + e1.getId() + ", " + e1.getTags().get("name"));
        hoot.trace("e2: " + e2.getId() + ", " + e2.getTags().get("name"));
        hoot.trace(
          "distance: " + d + " greater than search radius: " + searchRadius + "; returning score: " +
          result.score);
        return result;
    }

    var mean = translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
    var weightedWordDistanceScore = weightedWordDistance.extract(map, e1, e2);
    var weightedPlusMean = mean + weightedWordDistanceScore;
    var poiDistance = getTagCategoryDistance("poi", map, e1, e2);
    var artworkTypeDistance = getTagAncestorDistance("artwork_type", map, e1, e2);
    var cuisineDistance = getTagAncestorDistance("cuisine", map, e1, e2);
    var sportDistance = getTagAncestorDistance("sport", map, e1, e2);

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


    if (isSuperClose(e1, e2)) {
        score += 0.5;
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

    // we're unlikely to get more evidence than the fact that it is a tower
    // or pole. If the power tag matches exactly, give it 2 points of evidence
    // if not, just give it one.
    var powerDistance = getTagDistance("power", e1, e2);
    if (powerDistance == 0) {
        typeScore += 2;
        reason.push("same power (electrical) type");
    } else if (powerDistance <= 0.4) {
        typeScore += 1;
        reason.push("similar power (electrical) type");
    }


    // if at least one feature contains a place
    var placeCount = getTagsByAncestor("place", t1).length +
        getTagsByAncestor("place", t2).length;

    // if at least one of the points has a place and neither of them are
    // generic poi types
    if (placeCount > 0 && oneGeneric == false) {
        var d = getTagDistance("place", e1, e2);
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

    hoot.trace("e1: " + e1.getId() + ", " + e1.getTags().get("name"));
    if (e1.getTags().get("note"))
    {
      hoot.trace("e1 note: " + e1.getTags().get("note"));
    }
    hoot.trace("e2: " + e2.getId() + ", " + e2.getTags().get("name"));
    if (e2.getTags().get("note"))
    {
      hoot.trace("e2 note: " + e2.getTags().get("note"));
    }
    hoot.trace("reason: " + reason);
    hoot.trace("score: " + score);

    return result;
}

var totalCount = 0;

/**
 * Returns the match score for the three class relationships.
 * - match
 * - miss
 * - review
 *
 * The scores should always sum to one. If they don't you will be taunted
 * mercilessly and we'll normalize it anyway. :P
 */
exports.matchScore = function(map, e1, e2) {
    totalCount += 1;
    var result = { miss: 1.0, explain:'Miss' };

    if (e1.getStatusString() == e2.getStatusString()) {
        return result;
    }

    var additiveResult = additiveScore(map, e1, e2);
    var score = additiveResult.score;
    var reasons = additiveResult.reasons;
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

    hoot.trace("***POI MATCH DETAIL***");
    hoot.trace("e1: " + e1.getId() + ", " + e1.getTags().get("name"));
    if (e1.getTags().get("note"))
    {
      hoot.trace("e1 note: " + e1.getTags().get("note"));
    }
    hoot.trace("e2: " + e2.getId() + ", " + e2.getTags().get("name"));
    if (e2.getTags().get("note"))
    {
      hoot.trace("e2 note: " + e2.getTags().get("note"));
    }
    hoot.trace("score: " + score);
    hoot.trace("explanation: " + matchScore.explain);
    hoot.trace("classification: " + classification);
    hoot.trace("***END POI MATCH DETAIL***");

    return matchScore;
};

exports.mergePair = function(map, e1, e2)
{
    // replace instances of e2 with e1 and merge tags
    mergeElements(map, e1, e2);
    e1.setStatusString("conflated");

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
 * Given a number return a result that contains no more than 2 significant
 * digits and no more than 1 digit after the decimal place. For example:
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

