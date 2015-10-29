/**
 * This little example loads a file, translates it and collects some simple
 * stats.
 */

//Define hoot, variables and functions
var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

hoot.Settings.set({"log.format": "%m%n"});
hoot.Log.init();

// translation file to convert from input (e.g. UFD, TDS, etc.) to OSM
var ufdTran = HOOT_HOME + "/translations-local/UFD.js"
var mgcpTran = HOOT_HOME + "/translations-local/MGCP_01.js"
var fileList = [];

//this function will recursively explore directories to find osm, mgcp and udf files
var walk = function(dir) {
    var dive = function(dir) {
        fs.readdir(dir, function(err, list) {
            list.forEach(function(file) {
                var path = dir + "/" + file;
                fs.stat(path, function(err, stat) {
                    if (stat && stat.isDirectory()) {
                        if (path.toLowerCase().indexOf("osm") > -1 || path.toLowerCase().indexOf("mgcp") > -1 || path.toLowerCase().indexOf("ufd") > -1) {
                            dive(path); // it's a directory, let's explore recursively
                        }
                    }
                    else {
                        if (path.split('.').pop().toLowerCase() === 'osm' || path.split('.').pop().toLowerCase() === 'shp' || path.split('.').pop().toLowerCase() === 'pbf') {
                            fileList.push(path)
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
if (fs.lstatSync(input).isDirectory()) {
    walk(input)
} else {
    fileList.push(input)
}

//Format output columns
var rows = [['Dataset', 'Buildings', 'POI\'s', 'Linear Highways','Linear Rivers','Others']]
var csvRows = [];

//Loop through file list
for (var filecount = 0; filecount < fileList.length; filecount++) {
    var inputFile = fileList[filecount];
    var tran = '';
    if (findFormat(inputFile) == 'mgcp') {
        tran = mgcpTran;
    } else if (findFormat(inputFile) == 'ufd') {
        tran = ufdTran;
    }

    //Create a new map and populate it with the input file
    var map = new hoot.OsmMap();
    hoot.loadMap(map, inputFile, false, 1);

    // Translate the file from the input format to OSM if input is not osm format
    if (tran.length > 0 ) {
        new hoot.TranslationOp({"translation.script":tran}).apply(map);
    }

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
}

// unquoted CSV row and push to an array
var rowCount = rows.length;
for(var i=0; i<rowCount; ++i) {
    csvRows.push(rows[i].join(','));
}

//if user passes the output file, write results to the file, else print in console
if (typeof(output) !== 'undefined') {
    fs.writeFile(output, csvRows.join("\n"))
} else {
    for (var i=0; i<rowCount; i++) {
        hoot.log(csvRows[i])
    }
}
