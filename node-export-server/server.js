/*Define dependencies.*/
var express = require('express');
var app = express();
var uuid = require('uuid');
var fs = require('fs');
var archiver = require("archiver");
var exec = require('child_process').exec;

// The rest of the config is loaded below.
var config = require('./config.json');
var _ = require('lodash');
var rmdir = require('rimraf');
var crypto = require('crypto');
var noIntersection = require('shamos-hoey');
var done = false;
var dir;
var jobs = {};
var hootHome = process.env['HOOT_HOME'];
var appDir = hootHome + '/node-export-server/';
var runningStatus = 'running',
    completeStatus = 'complete';

// Loop through all "hootHome/translations*" directories and load config files if they exist.
fs.readdirSync(hootHome,{withFileTypes:true}).filter(file => file.isDirectory() && (file.name.indexOf('translations') == 0)).forEach( function (file){
  var tLocal = {};
  var dirName = hootHome + '/' + file.name + '/';

  // The same file is used by the translationServer and hoot-services
  if (fs.existsSync(dirName + 'translationConfig.json'))
  {
    try {
          tLocal = require(dirName + 'translationConfig.json');

          tLocal.forEach(function (fmt) {
              // If we don't have a specified schema then we don't support schema switching in the UI
              if (fmt.nodeExport) {
                // Either a "path" or "importPath" and "exportPath"
                if (fmt.path) {
                  config.schemas[fmt.name] = file.name + '/' + fmt.path;
                } else {
                  config.schemas[fmt.name] = file.name + '/' + fmt.exportPath;
                }

                if (fmt.exportOptions) {
                    config.schema_options[fmt.name] = fmt.exportOptions;
                }
              }
        });
    } catch (e) {
      console.log('Translation Config Error: ' + e);
    }
  }
});

// pipes request data to temp file with name that matches posted data format.
function writeExportFile(req, done) {
    //Build a hash for the input params used in file name
    var params = (new Date()).getTime()
        + req.params.datasource
        + req.params.schema
        + req.params.format;

    var fileNameHash = crypto.createHash('sha1').update(params, 'utf-8').digest('hex');

    //Write payload to file
    var input = 'export_' + fileNameHash;
    // we know overpass can give us json or osm, otherwise we expect osm format.
    var fileExtension = req.params.datasource.toLowerCase() === 'overpass' ? '' : 'osm';

    req.on('data', function(chunk) {
        if (fileExtension === '') { // if overpass, figure out file type to guide reader in hoot command
            var firstChar = chunk.toString().trim()[0];
            if (firstChar === '{') {
                fileExtension = 'json';
            } else if (firstChar === '<') {
                fileExtension = 'osm';
            }
        }
    });

    var writeStream = fs.createWriteStream(input, { flags : 'w' });
    req.pipe(writeStream);
    writeStream.on('error', function(err) {
        if(err) {
            return console.log(err);
        }
    }).on('close', function () {
        var inputWithExtension = input + '.' + fileExtension;
        // give file extension for reader.
        fs.rename(input, inputWithExtension, function() {
            //Calc sha1 for input file contents
            var read = fs.createReadStream(inputWithExtension);
            var hash = crypto.createHash('sha1');
            hash.setEncoding('hex');
            read.pipe(hash);
            read.on('end', function () {
                hash.end();
                var fileHash = hash.read();
                var jobParams = fileHash
                    + req.params.schema
                    + req.params.format;
                //Create job hash from input file content and export params
                var jobHash = crypto.createHash('sha1').update(jobParams, 'utf-8').digest('hex');
                done(jobHash, __dirname + '/' + inputWithExtension);
            });
        })
    });
}

/* Configure CORS. */

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers",
                    "Origin, X-Requested-With, Content-Type, Accept");
    next();
});

/* Handling routes. */

/* Get service options */
app.get('/options', function(req, res) {
    res.json({
        Datasource: _.keys(config.datasources),
        Schema: _.keys(config.schemas),
        Format: _.keys(config.formats)
    });
});

app.get('/tagoverrides', function(req, res) {
    res.json(config.tagOverrides);
});

/* Get job status*/
app.get('/job/:hash', function(req, res) {
    var hash = req.params.hash;
    var job = jobs[hash];
    if (job) {
        var input = job.input;
        doExport(req, res, hash, __dirname + '/' + input);
    } else {
        res.status(404).send('Job ' + hash + ' not found.');
    }
});

/* Post export */
// export/Overpass/OSM/Shapefile
app.post('/export/:datasource/:schema/:format', function(req, res) {
    //validate the params
    if (!validateExportParams(req, res)) return;

    writeExportFile(req, function(jobHash, input) {
        doExport(req, res, jobHash, input);
    })
});

/* Validate export params */
function validateExportParams(req, res) {
    var message;
    // check the datasource if request is GET
    // POST requests supplying own data can send whatever datasource string they want
    if (req.method === 'GET' && !config.datasources[req.params.datasource]) {
        message = `Datasource "${req.params.datasource}" not found`;
    }
    // check the schema
    if (!config.schemas[req.params.schema]) {
        message = `Schema "${req.params.schema}" not found`;
    }
    // check the format
    if (!config.formats[req.params.format]) {
        message = `Format "${req.params.format}" not found`;
    }
    if (message) {
        res.status(400);
        res.send(message);
        return false;
    } else {
        return true;
    }
}

/* Get export */
app.get('/export/:datasource/:schema/:format', function(req, res) {

    //validate the params
    if (!validateExportParams(req, res)) return;

    //Build a hash for the input params using base64
    var params = req.params.datasource
        + req.params.schema
        + req.params.format
        + req.query.bbox
        + req.query.crop
        + req.query.poly
        + req.query.overrideTags;
    var hash = crypto.createHash('sha1').update(params).digest('hex');
    var input = config.datasources[req.params.datasource].conn;
    doExport(req, res, hash, input);

});
exports.validateExportParams = validateExportParams
exports.writeExportFile = writeExportFile

/**
 * Validates that provided poly parameter is a valid polygon/multipolygon string.
 * If at any point the string is determined invalid, will return null.
 * Otherwise returns the original string by default or optionally an array of polygon coordinates
 * parsed from the input string.
 *
 * Input strings are determined valid when each polygon in the input string...
 * 1. follows the lon_0,lat_0;...lon_n,lat_n;lon_0,lat_0
 * 2. has a first and last coordinate that match
 * 3. does not self intersect as deterimed as determined by the shamos-hoey alg
 */
exports.validatePoly = function(poly, returnPolyArray = false) {
    var rings = [], ring = [];
    if (!poly) return null;
    var match = poly.split(';');

    //if the input array is empty, it lacks semi colons, or there is only 1 coordinate, then we
    //certainly do not have a polygon and so it is invalid
    if (match.length <= 1)
        return null;

    function validCoordinates(coordinates) {
        for (var c of coordinates) {
            if (c[0] < -180 || c[0] > 180 || c[1] < -90 || c[1] > 90) {
                return false;
            }
        }
        return true;
    }

    function matchingFirstLast(first, other) {
        return first[0] === other[0] && first[1] === other[1]
    }

    //try to parse each coordinate string.
    for (var m of match) {
        var captures = /(-?\d+\.?\d*),(-?\d+\.?\d*)/g.exec(m);
        // something about the polygon is invalid.
        if (!captures)
            return null;

        var coordinate = captures.splice(1).map(parseFloat); ring.push(coordinate);
        //once we've added a coordinate to the ring array, see if the first and last match
        //when they do we've found a polygon so we can add the ring (polygon) array to the rings array
        //then clear it out and follow the same process.
        if (ring.length > 1 && matchingFirstLast(ring[0], coordinate)) {
            rings.push(ring);
            ring = [];
        }
    }

    //if there is a ring with data, that tells us we did not find a matching first and last and so the input string does not
    //represent polygon data.
    if (ring.length) return null;

    //if we get passed the previous if statement we know that each ring atleast has a matching first and last polygon.
    //so, we only need to check to make sure that all coordinates are within bounds of geo-coordinate system and see that
    //no intersections are present.
    for (var ring of rings) {
        if (!validCoordinates(ring) || !noIntersection({ type: 'Polygon', coordinates: [ring] })) {
            return null;
        }
    }

    //return the polygon string by default or if specified the polygons array
    return returnPolyArray ? rings : poly;
}

/**
 * Using the validatePoly function, returns true when that function does not return null and
 * it returns more than one polygon array
 */
exports.isMultipolygon = function(poly) {
    var polygons = exports.validatePoly(poly, true);
    return polygons !== null && polygons.length > 1;
}

/**
 * Transforms a polygon string into a bounding box string.
 * Does so by sorting longitude and latitude from smallest to largest,
 * then returns the a string with form 'smallest_lon,smallest_lat,largest_lat,largest_lon'
 */
exports.polyToBbox = function(polyString) {
    var coordinates = polyString.split(';').reduce(function(coordsMap, coord, idx) {
        var pair = /(-?\d+\.?\d*),(-?\d+\.?\d*)/g.exec(coord).splice(1).map(parseFloat);

        //adds longitude and latitude not yet in the array.
        if (coordsMap.lon.indexOf(pair[0]) === -1)
            coordsMap.lon.push(pair[0]);
        if (coordsMap.lat.indexOf(pair[1]) === -1)
            coordsMap.lat.push(pair[1]);

        //sort longitude and latitude arrays from smallest->largest
        coordsMap.lon.sort(function(a,b) { return a - b })
        coordsMap.lat.sort(function(a,b) { return a - b })

        return coordsMap;
    }, { lon: [], lat: [] })

    return coordinates.lon[0] + ',' +
           coordinates.lat[0] + ',' +
           coordinates.lon[coordinates.lon.length - 1] + ',' +
           coordinates.lat[coordinates.lat.length - 1];
}

/**
 * Adds quotes around polyString when missing since hoot requires quotes
 */
exports.polyQuotes = function(polyString) {
    if (!polyString)
        return null

    polyString = polyString.trim()

    if (polyString[0] !== '"')
        polyString = '"' + polyString
    if (polyString[polyString.length - 1] !== '"')
        polyString = polyString + '"'

    return polyString;
}

/**
 * Ensures bbox string is a valid bounding box by checking...
 * 1. that it is a string of 4 floating point numbers
 * 2. the smaller longitude and latitude come first
 * 3. all longitude and latitude are within max bounds
 */
exports.validateBbox = function(bbox) {
    //38.4902,35.7982,38.6193,35.8536
    var regex = /(-?\d+\.?\d*),(-?\d+\.?\d*),(-?\d+\.?\d*),(-?\d+\.?\d*$)/;
    var match = regex.exec(bbox);
    if (match != null) { //matches pattern
        var minx = parseFloat(match[1]),
            miny = parseFloat(match[2]),
            maxx = parseFloat(match[3]),
            maxy = parseFloat(match[4]);
        if (minx < maxx && miny < maxy) { //passes min/max order
            if (minx >= -180 && miny >= -90 && maxx <= 180 && maxy <= 90) { //passes max bounds
                return bbox;
            }
        }
    }
    return null;
}

/**
 * updates job with hash's status
 * We do this in a couple places so figured a function would reduce writing smame things twice.
 */
function jobComplete(hash) {
    jobs[hash].status = 'complete';
    console.log('Job id: ' + hash + ' complete, output to ' + jobs[hash].outFile);
}

/**
 * Creates a zip archive of outFile.
 */
function zipOutput(hash,output,outFile,outDir,outZip,isFile,format,cb) {
    var stream = fs.createWriteStream(outZip);
    stream.on('close', function() {
        jobComplete(hash);
        if (cb) cb(); // optional callback function. used now in the multipolygon case so we can unlink each of the input files once the zip is generated.
    });

    var zip = archiver('zip');
    zip.on('error', function(err) {
        throw err;
    })
    zip.pipe(stream);

    if (isFile) {
        zip.file(outFile, { name: output + config.formats[format]});
    } else {
        //Don't include file extension for shapefile
        var ext = (format === 'Shapefile') ? '' : config.formats[format];
        zip.directory(outDir, output + ext);
    }
    zip.finalize();
}

/**
 * Builds the hootenanny command(s) from parts of provided request.
 */
function buildCommand(params, queryOverrideTags, querybbox, querypoly, isFile, input, outDir, outFile, doCrop, ignoreSourceIds, ignoreConf) {
    var paramschema = params.schema;
    var command = '', overrideTags = null;
    if (queryOverrideTags) {
        if (queryOverrideTags === 'true') { //if it's true
            //use the default overrides from config
            overrideTags = "'" + JSON.stringify(config.tagOverrides) + "'";
        } else { //assume it's json
            //use user submitted overrides
            overrideTags = "'" + JSON.stringify(JSON.parse(queryOverrideTags)) + "'";
        }
    }

    //if there is a override tags query param
    //handle different flavors of bbox param
    var bbox_param = 'bounds';
    var bbox = exports.validateBbox(querybbox);
    var poly = exports.validatePoly(querypoly);
    var willCrop = doCrop == 1 && (poly || bbox) !== null;

    //create command and run
    command += 'hoot convert'

    if (!ignoreConf)
        command += ' -C NodeExport.conf';

    if (params.datasource
        && config.datasources[params.datasource]
        && config.datasources[params.datasource].overrideConfig)
        command += ' -C ' + config.datasources[params.datasource].overrideConfig

    var convertOpts = [];
    var bboxOption = '';
    if (bbox) bboxOption = ' -D ' + bbox_param + '=' + bbox;
    if (poly) bboxOption = ' -D ' + bbox_param + '=' + exports.polyQuotes(poly);
    if (bboxOption) command += bboxOption

    if (isFile) {
        if (input.substring(0,2) === 'PG') command += ' -D ogr.reader.bounding.box.latlng=true';
        if (overrideTags) {
            if (paramschema === 'OSM') {
                convertOpts.push('SchemaTranslationOp');
                command += ' -D translation.script=' + hootHome  + '/translations/OSM_Ingest.js';
            }
            command += ' -D schema.translation.override=' + overrideTags;
        }
        if (paramschema !== 'OSM' && config.schemas[paramschema] !== '') {
            convertOpts.push('SchemaTranslationOp')
            command += ' -D schema.translation.script=' + hootHome + '/' + config.schemas[paramschema];
            command += ' -D schema.translation.direction=toogr';
            // Set per schema config options
            if (config.schema_options[paramschema]) command += ' -D ' + config.schema_options[paramschema];
        }
    } else {
        if (paramschema === 'OSM') command += ' -D writer.include.debug.tags=true';
        convertOpts.push('SchemaTranslationOp');
        command += ' -D schema.translation.script=' + hootHome + '/' + config.schemas[paramschema];
        if (overrideTags) command +=  ' -D schema.translation.override=' + overrideTags;
        if (input.substring(0,2) === 'PG') command += ' -D ogr.reader.bounding.box.latlng=true';
        // Set per schema config options
        if (config.schema_options[paramschema]) command += ' -D ' + config.schema_options[paramschema];
    }

    //prevent negative id collision when merging multipolygon rings
    if (ignoreSourceIds)
        command += ' -D reader.use.data.source.ids=false'

    //hard clip data to bounds of or bbox or polygon
    if (willCrop) {
        command += ' -D crop.bounds="' + (bbox || poly) + '"';
        convertOpts.push('MapCropper');
    }

    if (convertOpts.length > 0)
        command += ' -D convert.ops="' + convertOpts.join(';') + '"';

    command += ' ' + input + ' ' + outFile;

    //if (!isFile) command += ' --trans ' + config.schemas[req.params.schema];

    //used for testing to simulate hoot export
    //command = 'dd bs=1024 count=1024 if=/dev/urandom of=' + outFile + ' > /dev/null 2>&1';
    return command;
};

exports.buildCommand = buildCommand

/**
 * Manages request to the node-export service.
 * If provided job does not yet exist, kicks off the job and returns the hash to the user.
 * Otherwise, returns the job output, job has if running, or error message if an error occured.
 */
function doExport(req, res, hash, input) {
    //Check existing jobs
    var job = jobs[hash];
    var isFile = req.params.format === 'OSM XML';
    if (job) {
        if (job.status === completeStatus) { //if complete, return file
            if (req.method === 'POST') {
                res.send(hash);
            } else {
                res.download(job.outZip, job.downloadFile, function(err) {
                    // write stream has closed but for larger zips maybe file
                    // not yet ready for download so log err so we can troubleshoot
                    if (err) {
                        console.error('Error with download');
                        console.error(err);
                    }

                    if (jobs[hash]) {
                        if (jobs[hash].timeout)
                            clearTimeout(jobs[hash].timeout);

                        // clean up export files after configured delay
                        jobs[hash].timeout = setTimeout(function() {
                            if (jobs[hash]) {
                                var job = jobs[hash];
                                //delete export files
                                if (isFile) {
                                    fs.unlink(job.outFile, function(err) {
                                        if (err) {
                                            console.error(err);
                                        } else {
                                            console.log('deleted ' + job.outFile);
                                        }
                                    });
                                } else {
                                    rmdir(job.outDir, function(err) {
                                        if (err) {
                                            console.error(err);
                                        } else {
                                            console.log('deleted ' + job.outDir);
                                        }
                                    });
                                }
                                fs.unlink(job.outZip, function(err) {
                                    if (err) {
                                        console.error(err);
                                    } else {
                                        console.log('deleted ' + job.outZip);
                                    }
                                });
                                //Attempt to remove superfluous POST payload file if job is already running
                                if (fs.existsSync(input)) {
                                    fs.unlink(input, function(err) {
                                        if (err) {
                                            console.error(err);
                                        } else {
                                            console.log('deleted ' + input);
                                        }
                                    });
                                }
                                //This deletes the input files used by the job
                                if (fs.existsSync(job.input)) {
                                    fs.unlink(job.input, function(err) {
                                        if (err) {
                                            console.error(err);
                                        } else {
                                            console.log('deleted ' + job.input);
                                        }
                                    });
                                }
                                delete jobs[hash];
                            }
                        }, config.settings.cleanupDelay);
                    }
                });
            }
        } else { //if present, return status or hash
            if (job.status !== runningStatus) {
                res.status(500);
                res.send(job.status);
                //remove error'd job
                if (req.method === 'GET') delete jobs[hash];
            } else {
                res.send(hash);
            }
            //Attempt to remove superfluous POST payload file if job is already running
            if (fs.existsSync(input)) {
                fs.unlink(input, function(err) {
                    if (err) {
                        console.error(err);
                    } else {
                        console.log('deleted ' + input);
                    }
                });
            }

        }
    } else { //if missing, run job
        console.log('Running job id: ' + hash);
        var child = null;
        var id = uuid.v4();
        var output = 'export_' + id;
        var isFile = ['OSM XML', 'GeoPackage'].includes(req.params.format);
        var outDir = appDir + output;
        var outFile = outDir + config.formats[req.params.format];
        if (req.params.format === 'File Geodatabase') outDir = outFile;
        var outZip = outDir + '.zip';
        var doCrop = req.query.crop || Number(req.query.crop);
        var tempOsmFile = outDir + '.osm';
        var downloadFile = req.params.datasource.replace(' ', '_')
            + '_' + req.params.schema.replace(' ', '_')
            + '_' + req.params.format.replace(' ', '_')
            + '.zip';

        if (exports.isMultipolygon(req.query.poly)) {
            var polygons = exports.validatePoly(req.query.poly,true), multiCommand = '', rings = [];
            for (var i = 0; i < polygons.length; i++) {
                var poly = polygons[i];
                var polyString = poly.join(';');
                var ringId = uuid.v4();
                // disregard output format for rings. since we are going to merge these into a single file,
                // dealing with a list of osm files (versus directories for shp or gdb) is easier.
                var ringOutput = 'export_' + ringId;
                var ringOutDir = appDir + ringOutput;
                var ringOutFile = ringOutDir + '.osm';

                multiCommand += buildCommand({schema:'OSM'}, req.query.overrideTags, null,  polyString, isFile, input, ringOutDir, ringOutFile, doCrop);
                multiCommand += ' && ';

                rings.push(ringOutFile);
            }

            //here we merge the rings into a single osm file and create a new id sequence (to avoid negative id collisions).
            multiCommand += buildCommand({schema:'OSM'}, req.query.overrideTags, null, null, true, rings.join(' '), outDir, tempOsmFile, false, true);
            //if we need to make a shapefile/geodatabase or our ourput schema is not OSM, then we add a second command that'll do so.
            if (!isFile || req.params.schema !== 'OSM')
                multiCommand += ' && ' + buildCommand(req.params, req.query.overrideTags, null, null, isFile, tempOsmFile, outDir, outFile, false, false, true);

            console.log(multiCommand);
            child = exec(multiCommand, function(error, stdout, stderr) {
                if (stderr) {
                    jobs[hash].status = stderr;
                    console.error('Error with job id: ' + hash);
                    console.error(stderr);
                } else {
                    //zip the merged output then remove the rings individual export/crop output files.
                    zipOutput(hash,output,outFile,outDir,outZip,isFile,req.params.format, function() {
                        rings.forEach(function(ring) {
                            fs.unlink(ring, function(err) {
                                if (err) {
                                    console.error(err);
                                } else {
                                    console.log('deleted ' + ring);
                                }
                            });
                        });
                    })
                }
            })
        } else {
            var command = buildCommand(req.params, req.params.tagOverrides, req.query.bbox, req.query.poly, isFile, input, outDir, outFile, Number(req.query.crop));
            console.log(command);
            child = exec(command, {cwd: hootHome},
                function(error, stdout, stderr) {
                    //setTimeout(function() { //used to simulate a long request
                    if (stderr) {
                        //res.send({command: command, stderr: stderr, error: error});
                        jobs[hash].status = stderr;
                        console.error('Error with job id: ' + hash);
                        console.error(stderr);
                    } else {
                        zipOutput(hash,output,outFile,outDir,outZip,isFile,req.params.format)
                    }
                    //}, 10000); //used to simulate a long request
                }
            );
        }
        jobs[hash] = {
                    id: id,
                    status: runningStatus,
                    process: child,
                    input: input,
                    isFile: isFile,
                    outFile: outFile,
                    outDir: outDir,
                    outZip: outZip,
                    downloadFile: downloadFile
        };

        res.send(hash);
    }
}

/* Run the server. */
var server;
if (!process.env['NO_SERVER']) {
    server = app.listen(config.settings.port, function() {
        console.log("Listening on port " + config.settings.port);
    });
}

//http://glynnbird.tumblr.com/post/54739664725/graceful-server-shutdown-with-nodejs-and-express
//this function is called when you want the server to die gracefully
//i.e. wait for existing connections
var gracefulShutdown = function() {
    console.log("Received kill signal, shutting down gracefully.");
    server.close(function() {
        console.log("Closed out remaining connections.");
        cleanup();
    });

    // if after
    setTimeout(function() {
        console.error("Could not close connections in time, forcefully shutting down.");
        cleanup();
    }, 10*1000);
}

var cleanup = function() {
    //kill child processes
    console.log("Killing remaining child processes.");
    _.forOwn(jobs, function(job) {
        job.process.kill('SIGINT');
    });

    //clean up any export data incomplete or not yet downloaded
    console.log("Removing any orphaned export data.");
    exec('rm -rf export_*', {cwd: process.cwd()}, function() {
        process.exit();
    });
}

//listen for TERM signal e.g. kill
process.on('SIGTERM', gracefulShutdown);

//listen for INT signal e.g. Ctrl-C
process.on('SIGINT', gracefulShutdown);
