#!/usr/bin/node

//  Add multiary manual matching tag stubs to a file.
// ./AddMultiaryTrainingTags.js (input.osm) (file #) (output.osm)
//
// Example:
// ./AddMultiaryTrainingTags.js input.osm 1 output.osm
//
// The file number is the number of the input and is mostly arbitrary just
// start at 1 and increase with each file that will be matched. Don't re-use
// a file number.

var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

var map = new hoot.OsmMap();
hoot.loadMap(map, process.argv[2], false, 1);
var num = process.argv[3]

var i = 0;
var zeros = "00000";

function addId(e) {
    var istr = String(i);
    var t = e.getTags();
    t.set("ID", num + "-" + zeros.substring(0, zeros.length - istr.length) + istr);
    t.set("MATCH", "todo");
    t.set("REVIEW", "none");
    e.setTags(t);
    i++;
}

map.visit(addId);

hoot.log("saving map1");
hoot.saveMap(map, process.argv[4]);
