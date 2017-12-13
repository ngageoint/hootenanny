/************************************************************************
This is a nodejs integration with Hootenanny element merging logic.
************************************************************************/
var http = require('http');
var url = require('url');
var serverPort = 8096;
var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');

if (require.main === module) {
    //I'm a running server

    // cluster for load balancing
    var cluster = require('cluster');
    var os = require('os');
    var nCPU = os.cpus().length;

    //Argument parser
    process.argv.forEach(function (val, index, array) {
        // port arg
        // Note that default port comes from serverPort var
        if (val.indexOf('port=') == 0) {
            var portArg = val.split('=');
            if (portArg.length == 2){
                serverPort = 1*portArg[1];
            }
        }

        // thread count arg
        // defaults to numbers of CPU
        if (val.indexOf('threadcount=') == 0) {
            var nThreadArg = val.split('=');
            if (nThreadArg.length == 2) {
                var nThreadCnt = 1*nThreadArg[1];
                if (nThreadCnt > 0){
                    nCPU = nThreadCnt;
                }
            }
        }
    });

    // This is when the cluster master gets invoked
    if (cluster.isMaster) {
        // Spawn off http server process by requested thread count
        for(var i=0; i<nCPU; i++) {
            cluster.fork();
        }

        // This is for if one child process dies then create new one
        cluster.on('exit', function(worker){
            cluster.fork();
        });
    } else {
        // We create child process http server
        // and we all listen on serverPort
        http.createServer(ElementMergeserver).listen(serverPort);
    }
} else {
    //I'm being called from a mocha test
    http.createServer(ElementMergeserver);
}

function ElementMergeserver(request, response) {
    try {
        var header = {'Accept': 'text/xml', 'Content-Type': 'text/xml', 'Access-Control-Allow-Origin': '*'};
        if (request.method === 'POST') {
            var payload = '';
            request.on('data', function(chunk){
                payload += chunk;
            });

            request.on('end', function() {
                var urlbits = url.parse(request.url, true);
                var params = {};
                params.osm = payload;
                params.method = request.method;
                params.path = urlbits.pathname;

                var result = handleInputs(params);

                response.writeHead(200, header);
                response.end(result);
            });

        } else if (request.method === 'GET') {
            var urlbits = url.parse(request.url, true);
            var params = urlbits.query;
            params.method = request.method;
            params.path = urlbits.pathname;

            var result = handleInputs(params);

            response.writeHead(200, header);
            response.end(JSON.Stringify(result));
        } else {
            throw new Error('Unsupported method');
        }

    } catch (err) {
        var status = 500;
        if (err.message.indexOf('Unsupported') > -1)
            status = 400;
        if (err.message.indexOf('Not found') > -1)
            status = 404;
        response.writeHead(status, header);
        response.end(JSON.stringify({error: err}));
    }
}

function handleInputs(params) {
    switch(params.path) {
        case '/elementmerge':
            return mergeElement(params);
        default:
            throw new Error('Not found');
    }
};

var mergeElement = function(payload)
{
    if (payload.method === 'POST') {
        return postHandler(payload.osm);
    } else if (payload.method === 'GET') {
        throw new Error('Unsupported method');
    }
}

// This is where all interesting things happen interfacing with hoot core lib directly
var postHandler = function(data)
{
    var map = new hoot.OsmMap();
    map.setIdGenerator(new hoot.DefaultIdGenerator());
    hoot.loadMapFromString(map, data);

    //Using a simple check here to determine whether the POI/POI merge service or
    //the POI/Poly merge service should be used (only two that currently exist).  Possibly as
    //more types of element merging are provided by this service, this check will need to become 
    //more robust.
    var mergedMap;
    if (data.indexOf('<way') === -1 && data.indexOf('<relation') === -1) {
      var script = 'PoiGeneric.js';
      mergedMap = hoot.poiMerge(script, map, -1);
    } else {
      //Unlike the hoot POI/POI merger which allows for merging more than two elements, the POI/poly 
      //merger always expects exactly two elements.
      mergedMap = hoot.poiPolyMerge(map);
    }

    var xml = hoot.OsmWriter.toString(mergedMap);

    return xml;
}

if (typeof exports !== 'undefined') {
    exports.handleInputs = handleInputs;
    exports.ElementMergeserver = ElementMergeserver;
}
