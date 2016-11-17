/*Define dependencies.*/
var express = require('express');
var app = express();
var uuid = require('node-uuid');
var fs = require('fs');
var archiver = require("archiver");
var exec = require('child_process').exec;
var config = require('configure');
var _ = require('lodash');
var rmdir = require('rimraf');
var done = false;
var dir;
var jobs = {};
var runningStatus = 'running',
    completeStatus = 'complete';

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

/* Get export */
app.get('/export/:datasource/:schema/:format', function(req, res) {

    //Build a hash for the input params using base64
    var params = req.params.datasource
        + req.params.schema
        + req.params.format
        + req.query.bbox;
    var hash = new Buffer(params).toString('base64')

    //Check existing jobs
    var job = jobs[hash];

    var id;
    if (job) {
        id = job.id;
    } else {
        id = uuid.v1();
    }
    var output = 'export_' + id;
    var hootHome = process.env['HOOT_HOME'];
    var isFile = req.params.format === 'OSM XML';
    var appDir = hootHome + '/node-export-server/';
    var outDir = appDir + output;
    var outFile = outDir + config.formats[req.params.format];
    if (req.params.format === 'File Geodatabase') outDir = outFile;
    var outzip = outDir + '.zip';
    var downloadFile = req.params.datasource.replace(' ', '_')
        + '_' + req.params.schema.replace(' ', '_')
        + '_' + req.params.format.replace(' ', '_')
        + '.zip';

    if (job) {
        if (job.status === completeStatus) { //if complete, return file
            res.download(outzip, downloadFile, function(err) {
                if (jobs[hash].timeout)
                    clearTimeout(jobs[hash].timeout);

                // clean up export files after configured delay
                jobs[hash].timeout = setTimeout(function() {
                    if (jobs[hash]) {
                        delete jobs[hash];
                        //delete export files
                        if (isFile) {
                            fs.unlink(outFile, function(err) {
                                if (err) {
                                    console.error(err);
                                } else {
                                    console.log('deleted ' + outFile);
                                }
                            });
                        } else {
                            rmdir(outDir, function(err) {
                                if (err) {
                                    console.error(err);
                                } else {
                                    console.log('deleted ' + outDir);
                                }
                            });
                        }
                        fs.unlink(outzip, function(err) {
                            if (err) {
                                console.error(err);
                            } else {
                                console.log('deleted ' + outzip);
                            }
                        });
                        if (fs.existsSync(outDir + '.osm')) {
                            fs.unlink(outDir + '.osm', function(err) {
                                if (err) {
                                    console.error(err);
                                } else {
                                    console.log('deleted ' + outDir + '.osm');
                                }
                            });
                        }
                    }
                }, config.settings.cleanupDelay);
            });
        } else { //if present, return status
            if (job.status !== runningStatus) {
                res.status(500);
                //remove error'd job
                if (req.method === 'GET') delete jobs[hash];
            }
            res.send(job.status);
        }
    } else { //if missing, run job
        var command = '';
        //if conn is url, write that response to a file
        var input = config.datasources[req.params.datasource].conn;
        //handle different flavors of bbox param
        var bbox_param = 'convert.bounding.box';
        var bbox = exports.validateBbox(req.query.bbox);
        if (input.substring(0,2) === 'PG') bbox_param = 'ogr.reader.bounding.box.latlng';

        if (input.substring(0,4) === 'http') {
            var cert_param = '';
            if (input.substring(0,5) === 'https' && fs.existsSync(appDir + 'key.pem') && fs.existsSync(appDir + 'cert.pem')) {
                cert_param = '--private-key=' + appDir + 'key.pem --certificate=' + appDir + 'cert.pem';
            }
            temp_file = outDir + '.osm';
            command += 'wget -q ' + cert_param + ' -O ' + temp_file + ' ' + input + '?bbox=' + bbox + ' && ';
            input = temp_file;
        }

        //create command and run
        command += 'hoot';
        if (isFile) {
            command += ' convert';
            command += ' -D ' + bbox_param + '=' + bbox
            if (config.schemas[req.params.schema] !== '') {
                command += ' -D convert.ops=hoot::TranslationOp -D translation.script=' + config.schemas[req.params.schema] + ' -D translation.direction=toogr';
            }
        } else {
            command += ' osm2ogr';
            command += ' -D ' + bbox_param + '=' + bbox
            command += ' ' + config.schemas[req.params.schema]
        }
        command += ' "' + input + '" '
            + outFile
            ;

        //used for testing to simulate hoot export
        //command = 'dd bs=1024 count=1024 if=/dev/urandom of=' + outFile + ' > /dev/null 2>&1';
        console.log(command);

        //hoot osm2ogr -D ogr.reader.bounding.box=106.851,-6.160,107.052,-5.913 translations/TDSv61.js "PG:dbname='osmsyria' host='192.168.33.12' port='5432' user='vagrant' password=''" osm.shp
        //setTimeout(function() { //used to simulate a long request
        var child = exec(command, {cwd: hootHome},
            function(error, stdout, stderr) {
                if (stderr || error) {
                    //res.send({command: command, stderr: stderr, error: error});
                    jobs[hash].status = stderr;

                } else {
                    var stream = fs.createWriteStream(outzip);
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
                        zip.directory(outDir, output + config.formats[req.params.format]);
                    }
                    zip.finalize();

                }
            }
        );
        //}, 5000); //used to simulate a long request

        //create new job entry
        jobs[hash] = {
                        id: id,
                        status: runningStatus,
                        process: child
        };

        //return job status
        //if (job.status !== runningStatus) res.status(500);
        res.send(jobs[hash].status)
    }
});

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
