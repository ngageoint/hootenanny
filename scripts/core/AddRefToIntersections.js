#!/usr/bin/env node
/**
 * Loads two files. The first one has a REF1 tag added to each highway
 * intersection. The second file has a REF2 tag added to each highway
 * intersection.
 */
var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

var ref = process.argv[2];
var input = process.argv[3];
var output = process.argv[4];

hoot.log(process.argv.length);
if (process.argv.length != 5 ||
    (ref != "--ref1" && ref != "--ref2")) {
    console.log(process.argv[1] + " [--ref1|--ref2] <input.osm> <output.osm>");
    process.exit(-1);
}

// create a new map and populate it with two input files
var map1 = new hoot.OsmMap();
hoot.loadMap(map1, input, true, 1);

var refVisitor;
if (ref == "--ref1") {
    refVisitor = new hoot.AddRef1Visitor(map1,
        {"add.ref.visitor.information.only":false});
} else {
    refVisitor = new hoot.AddRef2Visitor(map1,
        {"add.ref.visitor.information.only":false});
}


// clean the data
new hoot.MapCleaner().apply(map1);
new hoot.VisitorOp(
    new hoot.FilteredVisitor(
        map1,
        new hoot.IntersectionCriterion(map1),
        refVisitor)).apply(map1);

hoot.saveMap(map1, output);

