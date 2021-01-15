/*Define dependencies.*/
var express = require('express');
var app = express();
var uuid = require('uuid');
var fs = require('fs');
var archiver = require("archiver");
var exec = require('child_process').exec;
var config = require('configure');
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

function getJobs() { return jobs; }
function getConfig() { return config; }
function waitSomeSeconds(seconds) {
    var someSecondsLater = new Date(new Date().getTime() + seconds * 1000);
    while(someSecondsLater > new Date()){}
}

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
    var fileExtension = req.params.datasource.toLowerCase() === 'overpass' ? '' : '.osm';

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
    writeExportFile(req, function(jobHash, input) {
        doExport(req, res, jobHash, input);
    })
});

/* Get export */
app.get('/export/:datasource/:schema/:format', function(req, res) {

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

exports.writeExportFile = writeExportFile

// ensures poly string in query params
//  1. follows structure lon_0,lat_0;...lon_n,lat_n;lon_0,lat_0
//  2. first and last match, meaning its a polygon
//  3. does not self intersect as deterimed by shamos-hoey alg
exports.validatePoly = function(poly, returnPolyArray = false) {
    var rings = [], ring = [];
    if (!poly) return null;
    var match = poly.split(';');

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

    if (match.length > 1) {
        for (m of match) {
            // gather capture groups to add to coordinates array
            // if we ever don't get a capture, exit early.
            // this is a way to make sure nothing that isn't a cooridnate can get injected into the hoot command
            var captures = /(-?\d+\.?\d*),(-?\d+\.?\d*)/g.exec(m);
            if (captures) {
                var coordinate = captures.splice(1).map(parseFloat);
                ring.push(coordinate);
                if (ring.length > 1 && matchingFirstLast(ring[0], coordinate)) {
                    rings.push(ring);
                    ring = [];
                }
            } else {
                return null;
            }
        }

        // if there is a ring with data, that tells us it's not matching first and last, and so invalid.
        if (ring.length) return null;

        for (var ring of rings) {
            if (!validCoordinates(ring) || !matchingFirstLast(ring[0], ring[ring.length - 1]) ||
                !noIntersection({ type: 'Polygon', coordinates: [ring] })
            ) {
                return null;
            }
        }

        return returnPolyArray ? rings : poly;
    }
    return null;
}

exports.isMultipolygon = function(poly) {
    var polygons = exports.validatePoly(poly, true);
    return polygons !== null && polygons.length > 1;
}

// create object of lat/lon that is sorted from min<max values then
// use order of list to return first values as min and last as max
exports.polyToBbox = function(polyString) {
    var coordinates = polyString.split(';').reduce(function(coordsMap, coord, idx) {
        var pair = /(-?\d+\.?\d*),(-?\d+\.?\d*)/g.exec(coord).splice(1).map(parseFloat)
        if (idx === 0 || coordsMap.lon.indexOf(pair[0]) === -1)
            coordsMap.lon.push(pair[0]);
        if (idx === 0 || coordsMap.lat.indexOf(pair[1]) === -1)
            coordsMap.lat.push(pair[1]);
        coordsMap.lon.sort(function(a,b) { return a - b })
        coordsMap.lat.sort(function(a,b) { return a - b })
        return coordsMap;
    }, { lon: [], lat: [] })

    return coordinates.lon[0] + ',' +
           coordinates.lat[0] + ',' +
           coordinates.lon[coordinates.lon.length - 1] + ',' +
           coordinates.lat[coordinates.lat.length - 1];
}
// hoot requires quotes for bounds so make sure they exist
exports.polyQuotes = function(polyString) {
    if (!polyString) return null
    polyString = polyString.trim()
    if (polyString[0] !== '"') polyString = '"' + polyString
    if (polyString[polyString.length - 1] !== '"') polyString = polyString + '"'
    return polyString;
}

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

function jobComplete(hash) {
    jobs[hash].status = 'complete';
    console.log(jobs[hash].id + ' complete');
}

function zipOutput(hash,output,outFile,outDir,outZip,isFile,format,cb) {
    var stream = fs.createWriteStream(outZip);
    stream.on('close', function() {
        jobComplete(hash);
        if (cb) cb();
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

function buildCommand(paramschema, queryOverrideTags, querybbox, querypoly, isFile, input, outDir, outFile, doCrop) {
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

    //if conn is url, write that response to a file
    if (input.substring(0,4) === 'http') {
        var cert_param = '';
        if (input.substring(0,5) === 'https' && fs.existsSync(appDir + 'key.pem') && fs.existsSync(appDir + 'cert.pem')) {
            cert_param = '--private-key=' + appDir + 'key.pem --certificate=' + appDir + 'cert.pem';
        }
        temp_file = outDir + '.osm';
        command += 'wget -q ' + cert_param + ' -O ' + temp_file + ' ' + input + '?bbox=' + (poly ? exports.polyToBbox(poly) : bbox) + ' && ';
        input = temp_file;
        //bbox not valid with osm file input source
        bbox = null;
    }
    //create command and run
    command += 'hoot convert -C NodeExport.conf';
    var bboxOption = '';
    if (bbox) bboxOption = ' -D ' + bbox_param + '=' + bbox;
    if (poly) bboxOption = ' -D ' + bbox_param + '=' + exports.polyQuotes(poly);
    if (bboxOption) command += bboxOption
    if (isFile) {
        if (input.substring(0,2) === 'PG') command += ' -D ogr.reader.bounding.box.latlng=true';
        if (overrideTags) {
            if (paramschema === 'OSM') {
                command += ' -D convert.ops=hoot::SchemaTranslationOp';
                command += ' -D translation.script=translations/OSM_Ingest.js';
            }
            command += ' -D schema.translation.override=' + overrideTags;
        }
        if (paramschema !== 'OSM' && getConfig().schemas[paramschema] !== '') {
            command += ' -D convert.ops=hoot::SchemaTranslationOp';
            command += ' -D schema.translation.script=' + getConfig().schemas[paramschema];
            command += ' -D schema.translation.direction=toogr';
            // Set per schema config options
            if (getConfig().schema_options[paramschema]) command += ' -D ' + getConfig().schema_options[paramschema];
        }
    } else {
        if (paramschema === 'OSM') command += ' -D writer.include.debug.tags=true';
        command += ' -D convert.ops=hoot::SchemaTranslationOp';
        command += ' -D schema.translation.script=' + getConfig().schemas[paramschema];
        if (overrideTags) command +=  ' -D schema.translation.override=' + overrideTags;
        if (input.substring(0,2) === 'PG') command += ' -D ogr.reader.bounding.box.latlng=true';
        // Set per schema getConfig() options
        if (getConfig().schema_options[paramschema]) command += ' -D ' + getConfig().schema_options[paramschema];
    }
    command += ' "' + input + '" '
        + outFile
        ;

    if (doCrop && poly) {
        command += ' && hoot crop ' + outFile + ' ' + outFile + ' ' + poly;
    }
    //if (!isFile) command += ' --trans ' + config.schemas[req.params.schema];

    //used for testing to simulate hoot export
    //command = 'dd bs=1024 count=1024 if=/dev/urandom of=' + outFile + ' > /dev/null 2>&1';
    console.log(command);
    return command;
};

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
                            if (fs.existsSync(job.outDir + '.osm')) {
                                fs.unlink(job.outDir + '.osm', function(err) {
                                    if (err) {
                                        console.error(err);
                                    } else {
                                        console.log('deleted ' + job.outDir + '.osm');
                                    }
                                });
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
        var child = null;
        var id = uuid.v4();
        var output = 'export_' + hash;
        var isFile = req.params.format === 'OSM XML';
        var outDir = appDir + output;
        var outFile = outDir + config.formats[req.params.format];
        if (req.params.format === 'File Geodatabase') outDir = outFile;
        var outZip = outDir + '.zip';
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
                var output = 'export_' + ringId;
                var outDir = appDir + output;
                var outFile = outDir + config.formats[req.params.format];
                if (req.params.format === 'File Geodatabase') outDir = outFile;
                multiCommand += buildCommand(req.params.schema, req.query.overrideTags, req.query.bbox, polyString, isFile, input, outDir, outFile, req.query.crop
                    ;
                if (i !== polygons.length - 1) multiCommand += ' && ';
                rings.push(outFile);
            }
            console.log(multiCommand);
            child = exec(multiCommand, function(error, stdout, stderr) {
                if (stderr || error) {
                    jobs[hash].status = error;
                } else {
                    // jobStatus(hash);

                   var command = 'hoot convert -C NodeExport.conf ' + rings.join(' ') + ' ' + outFile;
                   exec(command, {cwd: hootHome}, function(error, stdout, stderr) {
                       if (stderr || error) {
                           jobs[hash].status = stderr;
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
                    });
                }
            })
        } else {
            var command = buildCommand(req.params.schema, req.query.overrideTags, req.query.bbox, poly, isFile, input, outDir, outFile);
            child = exec(command, {cwd: hootHome},
                function(error, stdout, stderr) {
                    //setTimeout(function() { //used to simulate a long request
                    if (stderr || error) {
                        //res.send({command: command, stderr: stderr, error: error});
                        jobs[hash].status = stderr;

                    } else {
                        // if export specifies cropping to bounds or poly, do so.
                        if (req.query.crop) {

                            // if request did not provide a polygon or bounds to
                            // crop with, then go about zipping the output
                            var cropShape = exports.polyQuotes(exports.validatePoly(poly)) || exports.validateBbox(req.query.bbox);
                            if (cropShape) {
                                var cropCommand = 'hoot crop '
                                + outFile + ' '
                                + outFile + ' '
                                + cropShape
                                // following how I saw we logged convert command
                                // also proved helpful to just see if request handler calling crop or not when crop
                                // was present.
                                console.log(cropCommand)

                                // crop the output of the hoot convert, then write it to a zip
                                exec(cropCommand, {cwd: hootHome}, function(error, stdout, stderr) {
                                    if (stderr || error) {
                                        jobs[hash].status = stderr;
                                    } else {
                                        zipOutput(hash,output,outFile,outDir,outZip,isFile,req.params.format)
                                    }
                                })
                            } else {
                                zipOutput(hash,output,outFile,outDir,outZip,isFile,req.params.format)
                            }
                        } else {
                            zipOutput(hash,output,outFile,outDir,outZip,isFile,req.params.format)
                        }
                    }
                    //}, 10000); //used to simulate a long request
                }
            );
        }

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

/* Run the server. */
var server = app.listen(config.settings.port, function() {
    console.log("Listening on port " + config.settings.port);
});

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
