/**
 * This script conflates groups of three or more POIs using Generic Conflation.
 */

poi = hoot.require("MultiaryPoiRules");

// poor man's inheritance. Is there a better practice in JS?
// use all the functions/parameters as defined in Poi.js, then we will override later.
Object.keys(poi).forEach(function(key) {
    exports[key] = poi[key];
});

exports.description = "Matches three or more POIs";
exports.experimental = true;
exports.geometryType = "point";

// this matcher will never cause a conflict. However, another matcher may conflict with us.
exports.neverCausesConflict = function() { return true; }

delete exports.mergePair;

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

    var s1 = e1.getTags().get("source");
    var s2 = e2.getTags().get("source");

//    // if either of the inputs are from wikimapia, allow intradataset conflation
//    if ((s1 && s1.indexOf("wikimapia") !== -1) || (s2 && s2.indexOf("wikimapia") !== -1)) {
//        // pass
//    }
//    // otherwise, if they aren't from wikimapia, return a miss on any intradataset comparison
//    // e.g. OSM vs OSM always returns a miss.
//    else if (e1.getStatusString() === e2.getStatusString()) {
//        return result;
//    }

    var additiveResult = additiveScore(map, e1, e2);
    var score = additiveResult.score;
    var reasons = additiveResult.reasons;
    var d = "(" + prettyNumber(distance(e1, e2)) + "m)";

    var matchScore;
    var classification;
    // convert the scores into faux-probabilities
    if (score <= 0.5) {
        matchScore = {
            miss: 1 - (score / 0.5 * 0.4),
            match: score / 0.5 * 0.4,
            explain: 'Not much evidence of a match ' + d};
        classification = 'miss';
    } else if (score < 1.9) {
        matchScore = {
            miss: 0.6 - ((score - 0.5) / 1.4 * 0.2),
            match: 0.4 + ((score - 0.5) / 1.4 * 0.2),
            explain: "Somewhat similar " + d + " - " + reasons.join(", ") };
        classification = 'review';
    } else if (score < 3) {
        matchScore = {
            miss: 0.4 - ((score - 1.9) / 2.1 * 0.6),
            match: 0.6 + ((score - 1.9) / 2.1 * 0.4),
            explain: "Somewhat similar " + d + " - " + reasons.join(", ") };
        classification = 'match';
    } else {
        matchScore = {
            miss: 0,
            match: 1,
            explain: "Somewhat similar " + d + " - " + reasons.join(", ") };
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

exports.mergeSets = function(map, pairs, replaced)
{
    var elements = {};

    // create a dictionary that only contains each element once.
    for (var i = 0; i < pairs.length; i++)
    {
        elements[pairs[i][0].toString()] = pairs[i][0];
        elements[pairs[i][1].toString()] = pairs[i][1];
    }

    var sorted = [];

    Object.keys(elements).forEach(function(key) { sorted.push(elements[key]); });

    // sort all the elements by input then element ID.
    sorted.sort(function(eid1, eid2) {
        var e1 = map.getElement(eid1);
        var e2 = map.getElement(eid2);
        var s1 = e1.getStatusInput();
        var s2 = e1.getStatusInput();
        if (eid1 === eid2)
        {
            return 0;
        }
        if (s1 < s2 || (s1 === s2 && e1 < e2))
        {
            return -1;
        }
        else
        {
            return 1;
        }
    });

    // merge so that the lower number inputs have more influence.
    var first = null;
    sorted.forEach(function(eid) {
        if (first == null)
        {
            first = map.getElement(eid);
        }
        else
        {
            mergeElements(map, first, map.getElement(eid));
        }
    });
    first.setStatusString("Conflated");

    var tags = first.getTags();

    if (tags.contains("hoot:hash") === false)
    {
        // should we automatically generate it if it doesn't exist? This would require projecting
        // the data and stuff. ugh.
        throw "All candidates for conflation must contain a 'hoot:hash' key value pair.";
    }

    tags.set("source:hash", tags.get("hoot:hash"));
    tags.set("hoot:hash", "");
    first.setTags(tags);
};

