#!/usr/bin/node

var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('debug');

var data = '<?xml version="1.0" encoding="UTF-8"?>\
<osm version="0.6" upload="true" generator="hootenanny">\
<node id="-1559816" lon="-104.71755649299132" lat="38.888511927381124" version="1">\
    <tag k="amenity" v="cafe" />\
    <tag k="error:circular" v="1000" />\
    <tag k="hoot:status" v="Input1" />\
    <tag k="name" v="Starbucks" />\
    <tag k="note" v="1-c" />\
    <tag k="poi" v="yes" />\
    <tag k="hoot:merge:target" v="yes" />\
</node>\
<node id="-1559910" lon="-104.71877032224545" lat="38.88870088854899" version="1">\
    <tag k="amenity" v="cafe" />\
    <tag k="error:circular" v="1000" />\
    <tag k="hoot:status" v="Input2" />\
    <tag k="name" v="Starbucks" />\
    <tag k="note" v="2-c" />\
    <tag k="poi" v="yes" />\
</node>\
</osm>';
var map = new hoot.OsmMap();
hoot.loadMapFromStringPreserveIdAndStatus(map, data);
var mergedMap = hoot.merge(map);
var mergedMapStr = hoot.OsmWriter.toString(mergedMap);
console.log("\n\nOutput:\n\n" + mergedMapStr);
