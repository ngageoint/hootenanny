/**
 * This little example loads a file, translates it and collects some simple
 * stats.
 */
var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

hoot.Settings.set({"log.format": "%m%n"});
hoot.Log.init();

// print hello world to gain confidence
console.log(hoot.hello());

// translation file to convert from input (e.g. UFD, TDS, etc.) to OSM
var tran = process.argv[2]
// Input file (e.g. SHP)
var input = process.argv[3];
hoot.log(input);

// create a new map and populate it with the input file
var map = new hoot.OsmMap();
hoot.loadMap(map, input, false, 1);

// Translate the file from the input format to OSM
new hoot.TranslationOp({"translation.script":tran}).apply(map);

var buildingPolygonCount = 0;

// Count the buildings by visiting every element and using the OsmSchema to
// determine if it is a building or not.
map.visit(function(e) {
    if (hoot.OsmSchema.isBuilding(e)) {
        buildingPolygonCount++;
    }
});

hoot.log(buildingPolygonCount);

