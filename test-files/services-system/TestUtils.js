
assert = require("assert")
async = require('async');
express = require('express');
fs = require('fs');
htmlToText = require('html-to-text');
http = require('http');
path = require('path');
restler = require('restler');
childProcess = require('child_process');

var HOOT_HOME = process.env.HOOT_HOME
// disable hoot linking to save RAM
//var hoot = require(HOOT_HOME + '/lib/HootJs');


var util = exports;
// TOOD read this from the config files.

var props = {};

function loadProperties(str) {
    var lines = str.split("\n");
    for (var l in lines) {
        kvp = lines[l].split("=");
        if (kvp.length == 2) {
            props[kvp[0]] = kvp[1];
        }
    }
}

if (HOOT_HOME) {
    // load properties from config files
    loadProperties(fs.readFileSync(HOOT_HOME + 
      "/hoot-services/src/main/resources/conf/hoot-services.conf", 
      {'encoding':'utf8'}));
    loadProperties(fs.readFileSync(HOOT_HOME + 
      "/hoot-services/src/main/resources/conf/local.conf", 
      {'encoding':'utf8'}));

    var coreUrl = props['coreJobServerUrl'];
    exports.baseUrl = coreUrl + "/hoot-services"
    exports.dbUrl = 'postgresql://' + 
      props['dbUserId'] + ":" + props['dbPassword'] + "@" +
      props['dbHost'] + "/" + props['dbName'];
    exports.HOOT_HOME = HOOT_HOME;

    // Since coreJobServerUrl contains port like coreJobServerUrl=http://localhost:8080
    // try to replace port with translation server port
    exports.transUrlBase = "";
    var urlParts = coreUrl.split(":");
    for(var i=0; i<urlParts.length - 1; i++){
        exports.transUrlBase += urlParts[i] + ":";
    }

    exports.transUrlBase += props['translationServerPort'];
}

if (process.env.HOOT_SERVICES_URL) {
    exports.baseUrl = process.env.HOOT_SERVICES_URL;
}

if (process.env.HOOT_NODEJS_URL) {
    exports.transUrlBase = process.env.HOOT_NODEJS_URL;
}

exports.createRandomizedBaseUrl = function() {
    var url = exports.baseUrl;
    // avoid the time wait issue.
    if (url.indexOf("localhost") > -1) {
        var newIp = "127." + 
            Math.floor(Math.random() * 255) + "." +
            Math.floor(Math.random() * 255) + ".1";
        url = url.replace("localhost", newIp);
    }
    return url
}

/**
 * Convert gnarly server error messages from big long indescrernable HTML
 * into very brief indescernable text.
 */
exports.toText = function(html) {
    return htmlToText.fromString(html, {wordWrap:80});
}

exports.parseStatus = function(str) {
    if (str == undefined) return str;
    try {
        var json = JSON.parse(str);
        return json;
    } catch(e) {
        return undefined;
    }
}

/**
 * Given a job id poll the server every second until the job completes or
 * the job is no longer running. Calls done with an error if the job doesn't
 * complete, otherwise calls done with undefined and the job result object.
 * @param jobid - Job ID allocated by the services.
 * @param done - function(err, result)
 */
exports.waitForJob = function(jobid, done) {
    assert(done, "Need a done function");
    var jobPath = '/job/status/' + jobid;

    var timer = setInterval(function (jobid) {
        // for some reason restler doesn't handle the error situations well
        // this hackish looking code works in all the situations I could test
        // using ther error and fail events gave unusual situations where
        // the data couldn't be parsed. May be fixed in a more recent version
        // or simply shows my inability w/ JS. -JRS
        var req = restler.get(util.baseUrl + jobPath, {});
        req.on('complete', function(result, response) { 
            var d = util.parseStatus(response.raw);
            if (d != undefined) {
                if (d.status == 'complete')
                {
                    clearInterval(timer);
                    done(undefined, response);
                } else if (d.status != 'running') {
                    clearInterval(timer);
                    var error;
                    if (d.statusDetail) {
                        error = new Error("Job failed: " + d.statusDetail);
                    } else {
                        error = new Error("Job failed, unspecified error.");
                    }
                    done(error);
                }
            } else if (result instanceof Error) {
                done(result);
            }
        });
    }, 1000, jobid);
}

/**
 * Given an array of file paths create a dict of restler.file objects with
 * appropriate sizes populated.
 * @param done - function(err, result)
 */
exports.createFileObjects = function(files, done) {
    assert(done, "Need a done function");
    async.map(files, fs.stat, function(err, result) {
        if (err) {
            done(err);
            return;
        }
        var restlerFiles = {};
        for (var i in result)
        {
            var baseName = path.basename(files[i]);
            var f = restler.file(files[i], baseName, result[i].size, null);
            restlerFiles[baseName] = f;
        }
        done(err, restlerFiles);
    });
}

exports.downloadExport = function(jobid, outputName, done) {
    // http://localhost:8280/hoot-services/job/export/ex_508c2d573de749feb2f891d293edb1dd?outputname=DcGisRoads&removecache=false
    //var file = fs.createWriteStream('/tmp/' + outputName);
    var request = http.get(exports.baseUrl + '/job/export/' + jobid +
        '?outputname=' + outputName + '&removecache=false', function(res) {
            //res.pipe(file);
            var size = 0;
            var body = '';
            res.on('data', function(chunk) {
                size = size + chunk.length;
                body += chunk;
            });
            res.on('end', function() {
                done(size, body);
            });
            //file.on('finish', function() {
            //    file.close(done);
            //});
        }).
        on('error', function(e) {
            done(new Error(e.message));
        });
}

/**
 * @param outputType options: 'gdb'
 */
exports.exportMap = function(inputName, translation, outputName, outputType, done) {
    restler.post(util.baseUrl + '/job/export/execute',
        { data: JSON.stringify({
            input: inputName,
            inputtype: 'db',
            outputname: outputName,
            outputtype: outputType,
            removereview: 'false',
            translation: translation }) }).
        on('complete', function(result) {
            if (result instanceof Error) {
                done(result);
            } else {
                exports.waitForJob(result.jobid, function(response) {
                    exports.downloadExport(result.jobid, outputName, done);
                });
            }
        });
}

exports.getMapBounds = function(mapId, done) {
    restler.get(util.baseUrl + "/osm/api/0.6/map/mbr?mapId=" + mapId).
        on('complete', function(result) {
            done(result);
        });
}

exports.getMapIds = function(done) {
    restler.get(util.baseUrl + "/osm/api/0.6/map/layers").
        on('success', function(data, response) {
            done(data, response);
        }).
        on('fail', function(data, response) {
            done(new Error("failed getting map list."));
        }).
        on('error', function(err, response) {
            done(err);
        });
}

/**
 * Calls done with "exists" parameter. If it the map exists then exists is
 * set to true.
 */
exports.hasMapByName = function(name, done) {
    exports.getMapIds(function(data, respose) {
        for (var i in data.layers) {
            if (data.layers[i].name == name) {
                done(true);
                return;
            }
        }
        done(false);
    });
}

exports.conflate = function(input1, input2, output, done) {
    assert(done, "Need a done function");

    var getMapId = function(layers, name) {
        for (var i in layers) {
            if (layers[i].name == name) {
                return layers[i].id;
            }
        }
        done(new Error("Couldn't find map: " + name));
    }

    exports.getMapIds(function(data, response) {
        var mapId1 = getMapId(data.layers, input1);
        var mapId2 = getMapId(data.layers, input2);
        data = {'INPUT1_TYPE': 'DB',
            'INPUT1': String(mapId1),
            'INPUT2_TYPE': 'DB',
            'INPUT2': String(mapId2),
            'OUTPUT_NAME': output,
            'CONFLATION_TYPE': 'reference',
        };
        var url = exports.baseUrl + "/job/conflation/execute";
        restler.post(url, { 'data': JSON.stringify(data) }).
            on('success', function(data) {
                util.waitForJob(data.jobid, function(response) {
                    done(response, output);
                });
            }).
            on('fail', function(data, response) {
                done(new Error("failed conflating maps."), output);
            }).
            on('error', function(err, response) {
                done(err, output);
            });
        });
}

exports.deleteMap = function(mapName, done) {
    assert(done, "Need a done function");
    restler.post(util.baseUrl + "/osm/api/0.6/map/delete?mapId=" + mapName).
        on('success', function(data) {
            util.waitForJob(data.jobId, done);
        }).
        on('fail', function(data, response) {
            done(new Error("failed deleting map."));
        }).
        on('error', function(err, response) {
            done(err);
        });
}

/**
 * Given an array of file paths upload them into the map with the given name.
 */
exports.ingestFiles = function(files, translation, mapName, done) {
    var doIngest = function()
    {
        util.createFileObjects(files, function(err, files) {
            if (err) done(err);
            var inputType = 'UNSUPPORTED';

            restler.post(util.baseUrl + "/ingest/ingest/upload?TRANSLATION=" + 
                translation + "&INPUT_TYPE=" + inputType + "&INPUT_NAME=" + 
                mapName,
                { multipart: true, data: files }).
                on('success', function(data) {
                    util.waitForJob(data[0].jobid, done);
                }).
                on('fail', function(err, response) { 
                    done(new Error("failed ingesting files")); }).
                on('error', function(err, response) { done(err); });
        });
    }

    util.deleteMap(mapName, doIngest);
}

/**
 * Only ingest the given layer if it doesn't already exist.
 */
exports.ingestIfMissing = function(files, translation, mapName, done) {
    exports.hasMapByName(mapName, function(exists) {
        if (exists == false) {
            exports.ingestFiles(files, translation, mapName, done);
        } else {
            done();
        }
    });
}

exports.readMapElementCount = function(mapUrl, done) {
    var cmd = 'hoot stats -D services.db.reader.email=test@test.com --warn --quick ' + mapUrl;
    var cp = childProcess.exec(cmd,
        function(error, stdout, stderr) {
            if (error) {
                done(error);
            }
            lines = stdout.split('\n');
            count = 0;
            for (l in lines) {
                s = lines[l].split('\t');
                if (s[0] == "Node Count" || s[0] == "Way Count" || 
                    s[0] == "Relation Count") {
                    count += parseInt(s[1]);
                }
            }
            done(count);
        });
}

exports.translateMap = function(id, mapXml, done) {
    mapXml = mapXml.split('"').join("'");
    var req = { 
                command:'translate',
                translation: 'TDSv61',
                uid: id,
                input: mapXml 
            }
    var str = JSON.stringify(req);

    // get it directly from node server.
    restler.post(util.transUrlBase + '/osmtotds', {data:str}).
        on('complete', function(result)
        {
	    // Handle error messages: ECONREFUSED etc
	    if (result.errno)
            {
              done(result.errno);
            } else if (result instanceof Error)
              {
                done(result);
              } else
                {
                  done(null, result);
                }
        });
}


