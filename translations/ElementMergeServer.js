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
            var urlbits = url.parse(request.url, true);
            switch(urlbits.path) {
                case '/elementmerge':
                    //Lookin' good!
                    break;
                default:
                    throw new Error('Not found');
            }
            var payload = '';
            request.on('data', function(chunk){
                payload += chunk;
            });


            request.on('end', function() {

                var result;
                try {
                    result = postHandler(payload);
                } catch (err) {
                    var status = 400;
                    response.writeHead(status, header);
                    response.end(JSON.stringify({error: err}));
                }

                response.writeHead(200, header);
                response.end(result);
            });

        } else if (request.method === 'OPTIONS') {
            header["Access-Control-Allow-Methods"] = "POST, OPTIONS";
            header["Access-Control-Allow-Credentials"] = false;
            header["Access-Control-Max-Age"] = '86400'; // 24 hours
            header["Access-Control-Allow-Headers"] = "X-Requested-With, X-HTTP-Method-Override, Content-Type, Accept";
            response.writeHead(200, header);
            response.end();
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
        response.end(JSON.stringify({error: err.message}));
    }
}


var postHandler = function(data)
{
  // can't seem to get this to work
  //hoot.Log.setLogLevel('trace');

  var map = new hoot.OsmMap();
  hoot.loadMapFromStringPreserveIdAndStatus(map, data);
  var mergedMap = hoot.mergeElements(map);
  var xml = hoot.OsmWriter.toString(mergedMap);
  return xml;
}

if (typeof exports !== 'undefined') {
    exports.ElementMergeserver = ElementMergeserver;
}
