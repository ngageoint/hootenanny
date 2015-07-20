
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
            new hoot.LevenshteinDistance(
                {"levenshtein.distance.alpha": 1.5}))));

var distances = [
    {k:'historic',                      match:100,      review:200},
    {k:'place',     v:'neighborhood',   match:1000,     review:2000},
    {k:'place',     v:'village',        match:2000,     review:3000},
    {k:'place',     v:'populated',      match:2000,     review:3000},
    {k:'place',     v:'locality',       match:2000,     review:3000},
    {k:'waterway',                      match:1000,     review:2000},
    {k:'amenity',                       match:100,      review:200},
    {k:'landuse',                       match:200,      review:500},
    {k:'leisure',                       match:100,      review:200},
    {k:'tourism',                       match:100,      review:200},
    {k:'shop',                          match:100,      review:200},
    {k:'station',                       match:100,      review:200},
    {k:'transport',                     match:100,      review:200},
    {k:'railway',                       match:500,      review:1000},
    {k:'natural',                       match:200,      review:500},
    {k:'building',  v:'hospital',       match:300,      review:500},
    {k:'barrier', v:'toll_booth',       match:25,       review:50},
];

var reviews = [
    {k1:'railway', v1:'halt', k2:'transport', v2:'station' },
    {k1:'building', v1:'train_station', k2:'transport', v2:'station' },
    {k1:'building', v1:'train_station', k2:'railway', v2:'station' },
    {k1:'railway', v1:'station', k2:'transport', v2:'station' },
    {k1:'station', v1:'light_rail', k2:'railway', v2:'station' },
    {k1:'leisure', v1:'recreation_ground', k2:'leisure', v2:'sports_centre' },
    {k1:'leisure', v1:'recreation_ground', k2:'leisure', v2:'stadium' },
    {k1:'building', v1:'church', k2:'amenity', v2:'place_of_worship' },
    {k1:'amenity', v1:'fuel', k2:'amenity', v2:'motor_vehicle_station' },
    {k1:'amenity', v1:'parking_entrance', k2:'amenity', v2:'parking' },
    {k1:'shop', v1:'car', k2:'shop', v2:'car_repair' },
    {k1:'poi', v1:'yes', k2:'place' },
    {k1:'office', v1:'company', k2:'building', v2:'yes'},
];

// things that are frequently similar, but shouldn't be merged
var dontMerge = [
    {k1:'amenity', v1:'fuel', k2:'amenity', v2:'car_wash'},
    {k1:'amenity', v1:'restaurant', k2:'amenity', v2:'bicycle_parking'},
    {k1:'amenity', v1:'fast_food', k2:'amenity', v2:'place_of_worship'},
    {k1:'amenity', v1:'pub', k2:'amenity', v2:'arts_centre' },
    {k1:'place', k2:'amenity'},
    {k1:'building', v1:'hospital', k2:'amenity', v2:'taxi'},
];

/**
 * Returns all the kvps that are related to cuisine (even a little).
 */
function getRelatedTags(relateToKvp, d) {
    var result = [];
    for (var k in d) {
        var kvp = k + '=' + d[k];
        if (kvp != "poi=yes") {
            if (hoot.OsmSchema.score(relateToKvp, kvp) > 0) {
                result.push(kvp);
            }
        }
    }
    return result;
}

/**
 * Returns all the kvps that are in a category.
 */
function getTagsByCategory(category, d) {
    var result = [];
    for (var k in d) {
        var kvp = k + '=' + d[k];
        if (kvp != "poi=yes") {
            if (hoot.OsmSchema.getCategories(kvp).indexOf(category) >= 0) {
                result.push(kvp);
            }
        }
    }
    return result;
}

/**
 * Given a common kvp, find the distance between e1 and e2 where common exists
 * between them.
 */
function getTagCategoryDistance(category, e1, e2) {
    var result = 1;
    var t1 = e1.getTags().toDict();
    var t2 = e2.getTags().toDict();
    var c1 = getTagsByCategory(category, t1);
    var c2 = getTagsByCategory(category, t2);

    if (c1.length == 0 || c2.length == 0) {
        return undefined;
    }

    // find the best match between the two cuisine types
    for (var i in c1) {
        for (var j in c2) {
            result = Math.min(1 - hoot.OsmSchema.score(c1[i], c2[j]), result);
        }
    }

    return result;
}



/**
 * Given a common kvp, find the distance between e1 and e2 where common exists
 * between them.
 */
function getTagDistance(commonKvp, e1, e2) {
    var result = 1;
    var t1 = e1.getTags().toDict();
    var t2 = e2.getTags().toDict();
    var c1 = getRelatedTags(commonKvp, t1);
    var c2 = getRelatedTags(commonKvp, t2);

    if (c1.length == 0 || c2.length == 0) {
        return undefined;
    }

    // find the best match between the two cuisine types
    for (var i in c1) {
        for (var j in c2) {
            result = Math.min(1 - hoot.OsmSchema.score(c1[i], c2[j]), result);
        }
    }

    return result;
}

function getPoiDistance(e1, e2) {
    return getTagDistance('poi', e1, e2);
}

function getRestaurantResult(map, e1, e2) {
    var result;
    var t1 = e1.getTags();
    var t2 = e2.getTags();
    if (t1.get("amenity") == 'restaurant' && 
        t2.get("amenity") == 'restaurant')
    {
        var weightedPlusMean = weightedWordDistance.extract(map, e1, e2) +
            translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
        if (weightedPlusMean > 1.2) {
            result = {match:1, explain:'Restaurants with similar names'};
        } else if (t1.get('cuisine') == t2.get('cuisine') &&
            t1.get('cuisine') != undefined) {
            result = {review:1, explain:'Restaurants with different names, but similar cuisine'};
        }
    }
    return result;
}

function isSimilar(r, t1, t2) {
    var result = false;

    var t1k1Match = false;
    var t1k2Match = false;
    var t2k1Match = false;
    var t2k2Match = false;

    if (r.v1 == undefined) {
        if (t1.contains(r.k1)) {
            t1k1Match = true;
        }
    } else if (t1.get(r.k1) == r.v1) {
        t1k1Match = true;
    }

    if (r.v2 == undefined) {
        if (t1.contains(r.k2)) {
            t1k2Match = true;
        }
    } else if (t1.get(r.k2) == r.v2) {
        t1k2Match = true;
    }

    if (r.v1 == undefined) {
        if (t2.contains(r.k1)) {
            t2k1Match = true;
        }
    } else if (t2.get(r.k1) == r.v1) {
        t2k1Match = true;
    }

    if (r.v2 == undefined) {
        if (t2.contains(r.k2)) {
            t2k2Match = true;
        }
    } else if (t2.get(r.k2) == r.v2) {
        t2k2Match = true;
    }

    return (t1k1Match && t2k2Match) || (t1k2Match && t2k1Match);
}

function isDifferent(e1, e2) {
    var result = 0;

    var t1 = e1.getTags();
    var t2 = e2.getTags();

    // if the tags are contradictory
    for (var i = 0; i < dontMerge.length; i++) {
        if (isSimilar(dontMerge[i], t1, t2)) {
            result = 1;
        }
    }

    return result;
}

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

function closeAmenity(e1, e2) {
    // Start with amenity distance and reduce as needed.
    var result = amenityDistance(e1, e2);

    var t1 = e1.getTags();
    var t2 = e2.getTags();
    // if the tags are similar
    for (var i = 0; i < reviews.length; i++) {
        if (isSimilar(reviews[i], t1, t2)) {
            result = Math.min(result, .25);
        }
    }

    result = Math.min(getPoiDistance(e1, e2), result);

    return result;
}

/**
 * Returns 0 if at least one amentiy type tag is the same. Otherwise, returns
 * 1.
 */
function amenityDistance(e1, e2) {
    var result = 1;

    if (isDifferent(e1, e2)) {
        return result;
    }
    return getTagDistance("poi", e1, e2);

    var poiKeys = [
        'poi',
        'aeroway',
        'amenity',
        'highway',
        'industrial',
        'landuse',
        'man_made',
        'mountain_pass',
        'office',
        'place',
        'shop',
        'slope',
        'sport',
        'station',
        'leisure',
        'tourism',
        'waterway'
    ];

    var t1 = e1.getTags();
    var t2 = e2.getTags();
    for (var i = 0; i < poiKeys.length; i++) {
        var k = poiKeys[i];
        if (t1.contains(k) && t2.contains(k)) {
            var v1 = t1.get(k);
            var v2 = t2.get(k);
            if (v1 == v2 && v1 != "" && v2 != "") {
                result = 0;
            } else {
                result = Math.min(result, 0.5);
            }
        }
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

    var searchRadius = Math.max(exports.getSearchRadius(e1), 
        exports.getSearchRadius(e2));

    var d = distance(e1, e2);

    if (d > searchRadius) {
        return 0;
    }

    var nameMultiplier = 1;
    // if there is no type information to compare the name becomes more 
    // important
    if (hasTypeTag(e1) == false || hasTypeTag(e2) == false) {
        nameMultiplier = 2;
    }

    var mean = translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
    var amenityDistanceScore = amenityDistance(e1, e2);
    var weightedWordDistanceScore = weightedWordDistance.extract(map, e1, e2);
    var weightedPlusMean = mean + weightedWordDistanceScore;
    var poiScore = getTagCategoryDistance("poi", e1, e2);
    var cuisineDistance = getTagDistance("cuisine", e1, e2);
    var sportDistance = getTagDistance("sport", e1, e2);

    var score = 0;
    hoot.debug(nameMultiplier);
    if (weightedPlusMean > 0.987403 && weightedPlusMean < 1.2) {
        score += 0.5 * nameMultiplier;
    }
    hoot.debug(score);
    if (weightedPlusMean >= 1.2) {
        score += 1 * nameMultiplier;
    }
    if (cuisineDistance <= 0.3) {
        score += 1;
    }
    if (sportDistance <= 0.3) {
        score += 1;
    }
    if (poiScore <= 0.5) {
        score += 1;
    }
    if (isSuperClose(e1, e2)) {
        score += 0.5;
    }
    hoot.debug(score);

    // we're unlikely to get more evidence than the fact that it is a tower
    // or pole. If the power tag matches exactly, give it 2 points of evidence
    // if not, just give it one.
    var powerDistance = getTagDistance("power", e1, e2);
    if (powerDistance == 0) {
        score += 2;
    } else if (powerDistance <= 0.4) {
        score += 1;
    }


    return score;
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
exports.matchScore = function(map, e1, e2) {
    var result = { miss: 1.0, explain:'Miss' };

    if (e1.getStatusString() == e2.getStatusString()) {
        return result;
    }

    var score = additiveScore(map, e1, e2);

    if (score <= 0.5) {
        return {miss: 1, explain: 'Not much evidence of match'};
    } else if (score < 1.9) {
        return {review: 1, explain: 'Close, but not close enough'};
    } else {
        return {match: 1, explain: 'Very similar'};
    }

/*
    var soundex = soundexExtractor.extract(map, e1, e2);
    var mean = translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
    var max = translateMaxWordSetLevenshtein_1_15.extract(map, e1, e2);
    var amenityDistanceScore = amenityDistance(e1, e2);
    var weightedWordDistanceScore = weightedWordDistance.extract(map, e1, e2);
    var weightedPlusMean = weightedWordDistance.extract(map, e1, e2) +
      translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
    var closeAmenityScore = closeAmenity(e1, e2);
    var restaurantResult = getRestaurantResult(map, e1, e2);
    var cuisineDistance = getCuisineDistance(e1, e2);

    //hoot.log(isDifferent(e1, e2));
    if (isDifferent(e1, e2) == 1) {
        result = { miss: 1, explain:"Types shouldn't be conflated." };
    // if either one doesn't have a name, but the amenity types match then call
    // it a match.
    } else if (weightedWordDistanceScore == null) {
        if (closeAmenityScore == 0) {
            result = { match: 0.8, miss: 0.2, explain:'Missing name, but same type.' };
        }
    } else {
        if (weightedPlusMean <= 0.987403) {
            if (cuisineDistance < 0.25) {
                result = { review: 1, explain: 'Different names, but similar cuisine.' };
            } else {
                result = { miss: 0.95, match: 0.05, explain: 'Names are too different' };
            }
        } else {
            if (closeAmenityScore <= 0.5) {
                result = { match: 0.935, miss: 0.065, explain: 'Very similar' };
            } else {
                if (weightedPlusMean <= 1.206584) {
                    result = { review: 1, explain: 'Similar, but names are not close enough.' };
                } else {
                    result = { match: 0.80, miss: 0.20, explain: 'Similar type and sufficiently similar names' };
                }
            }
        }
    }
*/
    //hoot.log(result);
    //hoot.log(mean);
    //hoot.log(weightedWordDistanceScore);
    //hoot.log(weightedPlusMean);
    //hoot.log(closeAmenityScore);

    return result;
};

function printTags(tags) {
    hoot.warn("Tags:");
    var keys = Object.keys(tags.toDict());
    keys.sort();
    for (var i = 0; i < keys.length; i++) {
        hoot.warn(keys[i] + ": " + tags.toDict()[keys[i]]);
    }
}

exports.mergePair = function(map, e1, e2)
{
    var newTags = mergeTags(e1, e2);
    e1.setTags(newTags);

    removeElement(map, e2);

    return e1;
};

exports.getMatchFeatureDetails = function(map, e1, e2)
{
  var fd = [];

  fd["soundex"] = soundexExtractor.extract(map, e1, e2);
  fd["mean"] = translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
  fd["max"] = translateMaxWordSetLevenshtein_1_15.extract(map, e1, e2);
  fd['amenityDistance'] = amenityDistance(e1, e2);
  fd['weightedWordDistance'] = weightedWordDistance.extract(map, e1, e2);
  fd['weightedPlusMean'] = weightedWordDistance.extract(map, e1, e2) +
    translateMeanWordSetLevenshtein_1_5.extract(map, e1, e2);
  fd['closeAmenity'] = closeAmenity(e1, e2);
  fd['different'] = isDifferent(e1, e2);
  fd['cuisine'] = getCuisineDistance(e1, e2);
  fd['additive'] = additiveScore(map, e1, e2);

  return fd;
};


///**
// * The internals of geometry merging can become quite complex. Typically this
// * method will simply call another hoot method to perform the appropriate
// * merging of geometries.
// *
// * If this method is exported then the mergePair method should not be exported.
// *
// * @param map The map that is being conflated
// * @param pairs An array of ElementId pairs that will be merged.
// * @param replaced An empty array is passed in, the method should fill the array
// *      with all the replacements that occur during the merge process (e.g. if
// *      two elements (way:1 & way:2) are merged into one element (way:3), then
// *      the replaced array should contain [[way:1, way:3], [way:1, way:3]]
// *      where all the "way:*" objects are of the ElementId type.
// */
//exports.mergeSets = function(map, pairs, replaced)
//{
//    // snap the ways in the second input to the first input. Use the default tag
//    // merge method.
//    var result = snapWays(sublineMatcher, map, pairs, replaced);
//    return result;
//};

