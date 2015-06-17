/************************************************************************
This is Node js implementation of Hoot Translation Server.
The purpose of this module is to provide the hoot-ui fast way
to translate OSM to TDS and TDS to OSM.
************************************************************************/
var http = require('http');
var url = require('url');
// cluster for load balancing
var cluster = require('cluster');
var os = require('os');
// default port
var serverPort = 8233;

var HOOT_HOME = process.env.HOOT_HOME;
hoot = require(HOOT_HOME + '/lib/HootJs');


createUuid = hoot.UuidHelper.createUuid;
logWarn = hoot.logWarn;

var translationsMap = {};
translationsMap['TDSv40'] = require(HOOT_HOME + '/plugins/etds_osm.js'); 
translationsMap['TDSv61'] = require(HOOT_HOME + '/plugins/etds61_osm.js'); 

var util = require('util');
var nCPU = os.cpus().length;


var osmToTdsMap = {};
osmToTdsMap['TDSv40'] = new hoot.TranslationOp({
		    	'translation.script':HOOT_HOME + '/translations/OSM_to_englishTDS.js',
		    	'translation.direction':'toogr'});;
osmToTdsMap['TDSv61'] = new hoot.TranslationOp({
		    	'translation.script':HOOT_HOME + '/translations/OSM_to_englishTDS61.js',
		    	'translation.direction':'toogr'});

var tdsToOsmMap = {};
tdsToOsmMap['TDSv40'] = new hoot.TranslationOp({
		    	'translation.script':HOOT_HOME + '/translations/englishTDS_to_OSM.js',
		    	'translation.direction':'toosm'});
tdsToOsmMap['TDSv61'] = new hoot.TranslationOp({
		    	'translation.script':HOOT_HOME + '/translations/englishTDS61_to_OSM.js',
		    	'translation.direction':'toosm'});

// Gets the translation schema for field population for a fcode
// TODO: In the future we should get this from caller
var schemaMap = {};
schemaMap['TDSv40'] = require(HOOT_HOME + '/plugins/tds40_schema.js');
schemaMap['TDSv61'] = require(HOOT_HOME + '/plugins/tds61_schema.js');


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

			if(subPath == '/osmtotds'){
				osmtotds(request, response);
			} else if(subPath == '/tdstoosm') {
				tdstoosm(request, response);
			} else {
				response.writeHead(404, {"Content-Type": "text/plain", 'Access-Control-Allow-Origin' : '*'});
				response.write("404 Not Found\n");
				response.end();
			}

		}
	).listen(serverPort);
}

// OSM to TDS request handler
var osmtotds = function(request, response)
{
	if(request.method === "POST"){
		var alldata = "";
		request.on('data', function(data){
			alldata += data;
		});

		request.on('end', function(data){
			postHandler(alldata, response, osmToTdsMap);
		});
	} else if(request.method === "GET"){
		// When we get get request on  /osmtotds then produce fields based on supplied fcode
		var  url_parts = url.parse(request.url,true);
		var params = url_parts.query;
		var featSchema = "";
		var featWGeomMatchSchema = null;
		var geom = params.geom;
		var trns = params.translation;
		var schema = schemaMap['TDSv61'].getDbSchema();
		if(trns){
			var schModule = schemaMap[trns];
			if(schModule){
				schema = schModule.getDbSchema();
			}
		}
		for(var ii=0; ii<schema.length; ii++){
			var elem = schema[ii];
			if(elem.fcode === params.fcode){
				featSchema = elem;
				// We will take anything first and then if there is matching the geom
				// then it will take precedent
				if(geom === elem.geom){
					featWGeomMatchSchema = elem;
				}
			}
		}

		if(featWGeomMatchSchema){
			featSchema = featWGeomMatchSchema;
		}

		response.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'});
		response.end(JSON.stringify(featSchema));
	}	
}

// TDS to OSM handler
var tdstoosm = function(request, response)
{
	if(request.method === "POST"){
		var alldata = "";
		request.on('data', function(data){
			alldata += data;
		});

		request.on('end', function(data){
			postHandler(alldata, response, tdsToOsmMap);
		});
	} else if(request.method === "GET"){
		/*response.writeHead(404, {"Content-Type": "text/plain", 'Access-Control-Allow-Origin' : '*'});
		response.write("Get not supported\n");
		response.end();*/

		var etdsToOSMDirect = null;

		var  url_parts = url.parse(request.url,true);
		var params = url_parts.query;

		var translation = params.translation;

		var transScript = translationsMap[translation];
		if(transScript) {
			etdsToOSMDirect = transScript;
		} else {
			etdsToOSMDirect = transScript['TDSv61'];
		}
		var attr = {};
		attr['Feature Code'] = params.fcode;
		var osm_out = etdsToOSMDirect.toOSM(attr,'','');

		response.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'});
		response.end(JSON.stringify(osm_out));
	}	
}


// This is where all interesting things happen interfacing with hoot core lib directly
var postHandler = function(data, response, translatorMap)
{
	var msg = JSON.parse(data);
    var resArr = [];
    var start = new Date().getTime();
    var result = {};

    if (msg.uid) {
        result.uid = msg.uid;
    }
    if (msg.command) {
        result.command = msg.command;
    }

    var cmdTrans = msg.translation; 

    var tran = translatorMap[cmdTrans];
    if (msg.command == 'translate') {
        var map = new hoot.OsmMap();
        hoot.loadMapFromString(map, msg.input);
        tran.apply(map);
        xml = hoot.OsmWriter.toString(map);
        result.output = xml;
    } else {
        result.status = -1;
        result.error = 'Unrecognized command';
    }

    result.elapsed = new Date().getTime() - start;

    response.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'});
	response.end(JSON.stringify(result));
}



