#!/usr/bin/nodejs
/**
 * This is a small example that loads, cleans and conflates two files. There
 * is also a little bonus ways splitting on just one of the input files.
 * 
 * This doesn't address a specific use case, but demonstrates some of the 
 * functionality exposed to node.js via hootenanny.
 */
var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

var input = process.argv[2]

// create a new map and populate it with two input files
var map = new hoot.OsmMap();
hoot.loadMap(map, input, false, 1);

var unPoiTags = {};

function getTagsByCategory(category, d) {
    var result = [];
    for (var k in d) {
        var kvp = k + '=' + d[k];
        // if it is not a generic POI type
        if (kvp != "poi=yes" && kvp != "place=locality") {
            if (hoot.OsmSchema.getCategories(kvp).indexOf(category) >= 0) {
                result.push(kvp);
            }
        }
    }
    return result;
}

map.visit(function(e) {
if (getTagsByCategory("poi", e.getTags().toDict()).length == 0) {
    var t = e.getTags().toDict();
    for (var k in t) {
        if (k in unPoiTags == false) {
            unPoiTags[k] = {};
        }
        if (t[k] in unPoiTags[k] == false) {
            unPoiTags[k][t[k]] = 1;
        } else {
            unPoiTags[k][t[k]] = unPoiTags[k][t[k]] + 1;
        }
    }
}
});

ignoreKeys = ["ref", "source", "ele", "population", "source:datetime", "source", "exit_to", "description", "note", "operator", "is_in", "created_by", "postal_code", "addr:city", "source:date", "addr:street", "addr:postcode", "fixme", "addr:country", "addr:housenumber", "FIXME"];

for (k in unPoiTags) {
    for (v in unPoiTags[k]) {
        if (unPoiTags[k][v] > 1 && hoot.OsmSchema.isMetaData(k, v) == false &&
            ignoreKeys.indexOf(k) == -1 && key.indexOf("addr") == -1) {
            console.log(k + "=" + v);
        }
    }
}

