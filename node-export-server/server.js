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
var done = false;
var dir;
var jobs = {};
var runningStatus = 'running',
    completeStatus = 'complete';

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
        + req.query.overrideTags;
    var hash = crypto.createHash('sha1').update(params).digest('hex');
    var input = config.datasources[req.params.datasource].conn;
    doExport(req, res, hash, input);

});

exports.writeExportFile = writeExportFile
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

function doExport(req, res, hash, input) {
    //Check existing jobs
    var job = jobs[hash];

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
        var id = uuid.v1();
        var output = 'export_' + id;
        var hootHome = process.env['HOOT_HOME'];
        var isFile = req.params.format === 'OSM XML';
        var appDir = hootHome + '/node-export-server/';
        var outDir = appDir + output;
        var outFile = outDir + config.formats[req.params.format];
        if (req.params.format === 'File Geodatabase') outDir = outFile;
        var outZip = outDir + '.zip';
        var downloadFile = req.params.datasource.replace(' ', '_')
            + '_' + req.params.schema.replace(' ', '_')
            + '_' + req.params.format.replace(' ', '_')
            + '.zip';

        var command = '';
        var overrideTags = null;
        //if there is a override tags query param
        if (req.query.overrideTags) {
            if (req.query.overrideTags === 'true') { //if it's true
                //use the default overrides from config
                overrideTags = "'" + JSON.stringify(config.tagOverrides) + "'";
            } else { //assume it's json
                //use user submitted overrides
                overrideTags = "'" + JSON.stringify(JSON.parse(req.query.overrideTags)) + "'";
            }
        }
        //if conn is url, write that response to a file
        //handle different flavors of bbox param
        var bbox_param = 'convert.bounds';
        var bbox = exports.validateBbox(req.query.bbox);

        if (input.substring(0,4) === 'http') {
            var cert_param = '';
            if (input.substring(0,5) === 'https' && fs.existsSync(appDir + 'key.pem') && fs.existsSync(appDir + 'cert.pem')) {
                cert_param = '--private-key=' + appDir + 'key.pem --certificate=' + appDir + 'cert.pem';
            }
            temp_file = outDir + '.osm';
            command += 'wget -q ' + cert_param + ' -O ' + temp_file + ' ' + input + '?bbox=' + bbox + ' && ';
            input = temp_file;
            //bbox not valid with osm file input source
            bbox = null;
        }

        //create command and run
        command += 'hoot convert -C NodeExport.conf';
        if (isFile) {
            if (bbox) command += ' -D ' + bbox_param + '=' + bbox;
            if (input.substring(0,2) === 'PG') command += ' -D ogr.reader.bounding.box.latlng=true';
            if (overrideTags) {
                if (req.params.schema === 'OSM') {
                    command += ' -D convert.ops=hoot::SchemaTranslationOp';
                    command += ' -D translation.script=translations/OSM_Ingest.js';
                }
                command += ' -D schema.translation.override=' + overrideTags;
            }
            if (req.params.schema !== 'OSM' && config.schemas[req.params.schema] !== '') {
                command += ' -D convert.ops=hoot::SchemaTranslationOp';
                command += ' -D schema.translation.script=' + config.schemas[req.params.schema];
                command += ' -D schema.translation.direction=toogr';
                // Set per schema config options
                if (config.schema_options[req.params.schema]) command += ' -D ' + config.schema_options[req.params.schema];
            }
        } else {
            if (req.params.schema === 'OSM') command += ' -D writer.include.debug.tags=true';
            command += ' -D convert.ops=hoot::SchemaTranslationOp';
            command += ' -D schema.translation.script=' + config.schemas[req.params.schema];
            if (overrideTags) command +=  ' -D schema.translation.override=' + overrideTags;
            if (bbox) command += ' -D ' + bbox_param + '=' + bbox;
            if (input.substring(0,2) === 'PG') command += ' -D ogr.reader.bounding.box.latlng=true';
            // Set per schema config options
            if (config.schema_options[req.params.schema]) command += ' -D ' + config.schema_options[req.params.schema];
        }
        command += ' "' + input + '" '
            + outFile
            ;

        //if (!isFile) command += ' --trans ' + config.schemas[req.params.schema];

        //used for testing to simulate hoot export
        //command = 'dd bs=1024 count=1024 if=/dev/urandom of=' + outFile + ' > /dev/null 2>&1';
        console.log(command);

        //hoot convert -D ogr.reader.bounding.box=106.851,-6.160,107.052,-5.913 translations/TDSv61.js "PG:dbname='osmsyria' host='192.168.33.12' port='5432' user='vagrant' password=''" osm.shp
        var child = exec(command, {cwd: hootHome},
            function(error, stdout, stderr) {
                //setTimeout(function() { //used to simulate a long request
                if (stderr || error) {
                    //res.send({command: command, stderr: stderr, error: error});
                    jobs[hash].status = stderr;

                } else {
                    var stream = fs.createWriteStream(outZip);
                    stream.on('close', function() {
                        jobs[hash].status = 'complete';
                        console.log(jobs[hash].id + ' complete');
                    });

                    var zip = archiver('zip');
                    zip.on('error', function(err) {
                        throw err;
                    })
                    zip.pipe(stream);

                    if (isFile) {
                        zip.file(outFile, { name: output + config.formats[req.params.format]});
                    } else {
                        //Don't include file extension for shapefile
                        var ext = (req.params.format === 'Shapefile') ? '' : config.formats[req.params.format];
                        zip.directory(outDir, output + ext);
                    }
                    zip.finalize();

                }
                //}, 10000); //used to simulate a long request
            }
        );

        //create new job entry
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

        //return job status
        //if (job.status !== runningStatus) res.status(500);
        res.send(hash);
    }
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
