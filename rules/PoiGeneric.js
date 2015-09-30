
/*
 * This generic conflation script supports conflation of POI data
 */

exports.candidateDistanceSigma = 1.0; // 1.0 * (CE95 + Worst CE95);
exports.matchThreshold = parseFloat(hoot.get("poi.match.threshold"));
exports.missThreshold = parseFloat(hoot.get("poi.miss.threshold"));
exports.reviewThreshold = parseFloat(hoot.get("poi.review.threshold"));
exports.searchRadius = -1.0;

hoot.Settings.set({"ogr.log.limit":1000});

var soundexExtractor = new hoot.NameExtractor(
    new hoot.Soundex());
var translateMeanWordSetLevenshtein_1_5 = new hoot.NameExtractor(
    new hoot.MeanWordSetDistance(
        {"token.separator": "[\\s-,';]+"},
        new hoot.LevenshteinDistance({"levenshtein.distance.alpha": 1.5})));
var translateMaxWordSetLevenshtein_1_15 = new hoot.NameExtractor(
    new hoot.MaxWordSetDistance(
        new hoot.LevenshteinDistance({"levenshtein.distance.alpha": 1.15})));
var weightedWordDistance = new hoot.NameExtractor(
    new hoot.WeightedWordDistance(
        {"token.separator": "[\\s-,';]+", "weighted.word.distance.p": 0.5},
        new hoot.TranslateStringDistance(
            // runs just a little faster w/ tokenize off
            {"translate.string.distance.tokenize": "false"},
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.5}))));

var distances = [
    {k:'historic',                      match:100,      review:200},
    {k:'place',                         match:500,      review:1000},
    {k:'place',     v:'built_up_area',  match:1000,     review:2000},
    {k:'place',     v:'city',           match:2500,     review:5000},
    {k:'place',     v:'locality',       match:2000,     review:3000},
    {k:'place',     v:'neighborhood',   match:1000,     review:2000},
    {k:'place',     v:'populated',      match:2000,     review:3000},
    {k:'place',     v:'suburb',         match:1000,     review:2000},
    {k:'place',     v:'village',        match:2000,     review:3000},
    {k:'waterway',                      match:1000,     review:2000},
    {k:'amenity',                       match:100,      review:200},
    {k:'landuse',                       match:200,      review:600},
    {k:'leisure',                       match:100,      review:200},
    {k:'tourism',                       match:100,      review:200},
    // hotel campuses can be quite large
    {k:'tourism',   v:'hotel',          match:200,      review:400},
    {k:'shop',                          match:100,      review:200},
    {k:'station',                       match:100,      review:200},
    {k:'transport',                     match:100,      review:200},
    {k:'railway',                       match:500,      review:1000},
    {k:'natural',                       match:1000,     review:1500},
    {k:'building',  v:'hospital',       match:300,      review:500},
    {k:'barrier', v:'toll_booth',       match:25,       review:50},
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
    for (var i = 0; i < distances.length; i++) {
        if (tags.contains(distances[i].k) &&
            (distances[i].v == undefined ||
             tags.get(distances[i].k) == distances[i].v)) {
            radius = Math.max(distances[i].review);
        }
    }

    return radius;
}

/**
 * Runs before match creation occurs and provides an opportunity to perform custom initialization.
 */
exports.init = function(map) {
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

    var searchRadius = Math.max(exports.getSearchRadius(e1), 
        exports.getSearchRadius(e2));

    var d = distance(e1, e2);

    if (d > searchRadius) {
        return result;
    }

    var nameMultiplier = 1;
    // if there is no type information to compare the name becomes more 
    // important
    var oneGeneric = hasTypeTag(e1) == false || hasTypeTag(e2) == false;
    if (oneGeneric) {
        nameMultiplier = 2;
    }

    var t1 = e1.getTags().toDict();
    var t2 = e2.getTags().toDict();

    var mean = translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
    var weightedWordDistanceScore = weightedWordDistance.extract(map, e1, e2);
    var weightedPlusMean = mean + weightedWordDistanceScore;
    var placeScore = getTagCategoryDistance("place", e1, e2);
    var poiDistance = getTagCategoryDistance("poi", e1, e2);
    var artworkTypeDistance = getTagAncestorDistance("artwork_type", e1, e2);
    var cuisineDistance = getTagAncestorDistance("cuisine", e1, e2);
    var sportDistance = getTagAncestorDistance("sport", e1, e2);

    var score = 0;

    if (weightedPlusMean > 0.987403 && weightedPlusMean < 1.2) {
        score += 0.5 * nameMultiplier;
        reason.push("similar names");
    }
    if (weightedPlusMean >= 1.2) {
        score += 1 * nameMultiplier;
        reason.push("very similar names");
    }
    if (isSuperClose(e1, e2)) {
        score += 0.5;
        reason.push("very close together");
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
    } else if (poiDistance >= 0.99) {
        score = 0;
    }

    if (artworkTypeDistance <= 0.3) {
        score += 1;
        reason.push("similar artwork type");
    }
    if (cuisineDistance <= 0.3) {
        score += 1;
        reason.push("similar cuisine");
    }
    if (sportDistance <= 0.3) {
        score += 1;
        reason.push("similar sport");
    }

    // we're unlikely to get more evidence than the fact that it is a tower
    // or pole. If the power tag matches exactly, give it 2 points of evidence
    // if not, just give it one.
    var powerDistance = getTagDistance("power", e1, e2);
    if (powerDistance == 0) {
        score += 2;
        reason.push("same power (electrical) type");
    } else if (powerDistance <= 0.4) {
        score += 1;
        reason.push("similar power (electrical) type");
    }

    result.score = score;
    result.reasons = reason;

    hoot.debug(reason);
    hoot.debug(score);

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

    if (score <= 0.5) {
        return {miss: 1, explain: 'Not much evidence of a match'};
    } else if (score < 1.9) {
        return {review: 1, explain: "Somewhat similar - " + reasons.join(", ") };
    } else {
        return {match: 1, explain: "Very similar - " + reasons.join(", ") };
    }
};

exports.mergePair = function(map, e1, e2)
{
    var newTags = mergeTags(e1, e2);
    e1.setTags(newTags);
    e1.setStatusString("conflated");

    removeElement(map, e2);

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

