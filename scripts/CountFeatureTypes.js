/**
 * This little example loads a file, translates it and collects some simple
 * stats.
 */
var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

hoot.Settings.set({"log.format": "%m%n"});
hoot.Log.init();

//format output columns
var rows = [['Dataset', 'Buildings', 'POI\'s', 'Linear Highways','Linear Rivers','Others']]
var csvRows = [];

// translation file to convert from input (e.g. UFD, TDS, etc.) to OSM
var tran = process.argv[2]
hoot.log("Translation file: " + tran);

// Input file (e.g. SHP)
var input = process.argv[3];
hoot.log("Input file: " + input);

var output = process.argv[4];
if (typeof(output) !== 'undefined') {
    hoot.log("Output file: " + output);
}

// create a new map and populate it with the input file
var map = new hoot.OsmMap();
hoot.loadMap(map, input, false, 1);

// Translate the file from the input format to OSM
new hoot.TranslationOp({"translation.script":tran}).apply(map);

var buildingPolygonCount = 0;
var highwayCount = 0;
var poiCount = 0;
var linerRiverCount = 0;
var otherCount = 0;

// Count the buildings, hightway,poi and river by visiting every element and
// using the OsmSchema to determine.
map.visit(function(e) {
    if (hoot.OsmSchema.isBuilding(e)) {
        buildingPolygonCount++;
    } else if (hoot.OsmSchema.isLinearHighway(e)) {
    	highwayCount++;
    } else if (hoot.OsmSchema.isLinearWaterway(e)) {
    	linerRiverCount++;
    } else if (hoot.OsmSchema.isPoi(e)) {
    	poiCount++;
    } else {
    	otherCount++;
    }
});

//insert the count info into rows
var inputFilename = input.replace(/^.*[\\\/]/, '')
rows.push([inputFilename,buildingPolygonCount,poiCount,highwayCount,linerRiverCount,otherCount])

// unquoted CSV row and push to an array
var rowCount = rows.length;
for(var i=0; i<rowCount; ++i) {
    csvRows.push(rows[i].join(','));
}

//if user passes the output file, write results to the file, else print in console
if (typeof(output) !== 'undefined') {
    var fs = require('fs')
    fs.writeFile(output, csvRows.join("\n"))
} else {
    for (var i=0; i<rowCount; i++) {
        hoot.log(csvRows[i])
    }
}


