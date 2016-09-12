/************************************************************************
This is Node js implementation of Hoot Poi to Poi merge Server.
The purpose of this module is to provide the hoot-ui fast way
to merge Poi to Poi.
************************************************************************/
var http = require('http');
var url = require('url');
// cluster for load balancing
var cluster = require('cluster');
var os = require('os');
// default port
var serverPort = 8096;

var HOOT_HOME = process.env.HOOT_HOME;
//Moving hoot init to the request handler allows stxxl temp file cleanup to happen properly.
//hoot = require(HOOT_HOME + '/lib/HootJs');

var nCPU = os.cpus().length;

// Argument parser
process.argv.forEach(function (val, index, array) {
	// port arg
	// Note that default port comes from serverPort var
  if(val.indexOf('port=') == 0){
  	var portArg = val.split('=');
  	if(portArg.length == 2){
  		serverPort = 1*portArg[1];
  	}
  }

  // thread count arg
  // defaults to numbers of CPU
  if(val.indexOf('threadcount=') == 0){
  	var nThreadArg = val.split('=');
  	if(nThreadArg.length == 2){
		var nThreadCnt = 1*nThreadArg[1];
		if(nThreadCnt > 0){
			nCPU = nThreadCnt;
		}
  	}
  }
});

// This is when the cluster master gets invoked
if(cluster.isMaster){
	// Spawn off http server process by requested thread count
	for(var i=0; i<nCPU; i++) {
		cluster.fork();
	}

	// This is for if one child process dies then create new one
	cluster.on('exit', function(worker){
    	cluster.fork();
	})
} else {
	// We create child process http server
	// and we all listen on serverPort
	http.createServer(

		function(request, response){

			// we sends request based on requested path
			var subPath = url.parse(request.url).pathname;

			if(subPath == '/p2pmerge'){
				mergeP2P(request, response);
			} else {
				response.writeHead(404, {"Content-Type": "text/plain", 'Access-Control-Allow-Origin' : '*'});
				response.write("404 Not Found\n");
				response.end();
			}

		}
	).listen(serverPort);
}

// request handler
var mergeP2P = function(request, response)
{
	if(request.method === "POST"){
		var alldata = "";
		request.on('data', function(data){
			alldata += data;
		});

		request.on('end', function(data){
			postHandler(alldata, response);
		});
	}
	else if(request.method === "GET"){
	}
}

// This is where all interesting things happen interfacing with hoot core lib directly
var postHandler = function(data, response)
{
	var hoot = require(HOOT_HOME + '/lib/HootJs');

	var result = {};
	/*var input = "<?xml version='1.0' encoding='UTF-8'?>\
	<osm version='0.6' upload='true' generator='JOSM'>\
	<node id='-3200079' visible='true' lat='48.0479399' lon='11.7012814'>\
	<tag k='amenity' v='post_office' />\
	<tag k='error:circular' v='15' />\
	<tag k='hoot:status' v='1' />\
	</node>\
	<node id='-3200083' visible='true' lat='48.04854' lon='11.70194'>\
	<tag k='amenity' v='post_office' />\
	<tag k='error:circular' v='150' />\
	<tag k='name' v='POST, JÄGER-VON-FALL-STRASSE' />\
	<tag k='hoot:status' v='2' />\
	</node>\
	</osm>";*/

	var map = new hoot.OsmMap();
    map.setIdGenerator(new hoot.DefaultIdGenerator());
    hoot.loadMapFromString(map, data);

	var script = 'PoiGeneric.js';
    var mergedMap = hoot.poiMerge(script, map, -1);
	var xml = hoot.OsmWriter.toString(mergedMap);
	result.input = data;
	result.output = xml;

    response.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'});
	response.end(JSON.stringify(result));
}
