#!/usr/bin/env node
/**
 * This little example loads a file, translates it and collects some simple
 * stats.
 */

//Define functions
//this function will recursively explore directories to find osm, mgcp and udf files,  and process files
var walk = function(dir) {
    var dive = function(dir) {
        fs.readdir(dir, function(err, list) {
            list.forEach(function(file) {
                var path = dir + "/" + file;
                fs.stat(path, function(err, stat) {
                    if (stat && stat.isDirectory()) {
                        dive(path); // it's a directory, let's explore recursively
                    } else {
                        if (findFormat(path) == "osm" || findFormat(path) == "mgcp" || findFormat(path) == "ufd") {
                            if (path.split('.').pop().toLowerCase() === 'osm' || path.split('.').pop().toLowerCase() === 'shp' || path.split('.').pop().toLowerCase() === 'pbf') {
                                var stats = fs.statSync(path)
                                var fileSizeInMBytes = stats["size"]/1000000.0;
                                if (path.split('.').pop().toLowerCase() === 'pbf' && fileSizeInMBytes > 50) {
                                    hoot.log(path + " is too big, it may cause memory problem. Please process individually.")
                                } else {
                                    processFile(path);
                                }
                            }
                        }
                    }
                });
            });
        });
    };
    // start exploration
    dive(dir);
};

//find file format
var findFormat = function(filename) {
    var format = '';
    var onlyPath = require('path').dirname(filename);
    var listPath = onlyPath.split("/");
    for (var i = 0; i < listPath.length; i++) {
        format = listPath[i];
        if (format.toLowerCase() == 'mgcp' || format.toLowerCase() == 'ufd' || format.toLowerCase() == 'osm') {
            break;
        }
    }
    return format.toLowerCase();
};

//Process file to get stats
var processFile = function(inputFile) {
    var tran = '';
    if (findFormat(inputFile) === 'mgcp') {
        tran = mgcpTran;
    } else if (findFormat(inputFile) === 'ufd') {
        tran = ufdTran;
    }

    //Create a new map and populate it with the input file
    var map = new hoot.OsmMap();
    try {
        //Load map
        hoot.loadMap(map, inputFile, false, 1);
        new hoot.ReprojectToPlanarOp().apply(map);

        // Translate the file from the input format to OSM if input is not osm format
        if (tran.length > 0 ) {
            new hoot.TranslationOp({"translation.script":tran}).apply(map);
        }

        var buildingPolygonCount = 0;
        var highwayCount = 0;
        var highwayLength = 0.0;
        var poiCount = 0;
        var linerRiverCount = 0;
        var riverLength = 0.0;
        var otherCount = 0;

        // Count the buildings, hightway,poi and river by visiting every element and
        // using the OsmSchema to determine.
        map.visit(function(e) {
            if (hoot.OsmSchema.isBuilding(e)) {
                buildingPolygonCount++;
            } else if (hoot.OsmSchema.isLinearHighway(e)) {
                highwayCount++;
                highwayLength += hoot.ElementConverter.calculateLength(map, e);
            } else if (hoot.OsmSchema.isLinearWaterway(e)) {
                linerRiverCount++;
                riverLength += hoot.ElementConverter.calculateLength(map, e);
            } else if (hoot.OsmSchema.isPoi(e)) {
                poiCount++;
            } else if (e.getTags().getInformationCount() > 0) {
                otherCount++;
            }
        });

        //write output
        //var inputFilename = inputFile.replace(/^.*[\\\/]/, '')
        var row = [inputFile,buildingPolygonCount,poiCount,highwayCount,highwayLength.toFixed(2), +
                   linerRiverCount,riverLength.toFixed(2),otherCount];
        if (typeof(output) !== 'undefined') {
            fs.appendFileSync(output, '\n');
            fs.appendFileSync(output, row.join(','));
        } else {
            hoot.log(row.join(','))
        }

        map = null;
    } catch (err) {
        hoot.warn(err.toString());
        map = null;
    }
}


//Define hoot, variables and functions
var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

hoot.Settings.set({"log.format": "%m%n"});
hoot.Log.init();

// translation file to convert from input (e.g. UFD, TDS, etc.) to OSM
var ufdTran = HOOT_HOME + "/translations-local/UFD.js";
var mgcpTran = HOOT_HOME + "/translations/MGCP_TRD4.js";

//Get user inputs
//input file (e.g. SHP) or input dir
var input = process.argv[2];
hoot.log("Input: " + input);

//optional output csv file
var output = process.argv[3];
if (typeof(output) !== 'undefined') {
    hoot.log("Output file: " + output);
}

//Nodejs file system
var fs = require('fs');

//Format output columns
var rowHeader = [['Dataset', 'Buildings', 'POI\'s', 'Linear Highways','Highway Length(meter)', +
                  'Linear Rivers','Rivers Length(meter)','Others']];
if (typeof(output) !== 'undefined') {
    fs.writeFile(output, rowHeader.join(','));
} else {
    hoot.log(rowHeader.join(','));
}

if (fs.lstatSync(input).isDirectory()) {
    walk(input);
} else {
    processFile(input);
}
