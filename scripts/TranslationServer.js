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
var fs = require('fs');
// default port
var serverPort = 8233;

var util = require('util');
var nCPU = os.cpus().length;

var availableTrans = {'TDSv40':{'isvailable':'true'}, 'TDSv61':{'isvailable':'true'}};
var HOOT_HOME = process.env.HOOT_HOME;
//Moving hoot init to the request handler allows stxxl temp file cleanup to happen properly.
//hoot = require(HOOT_HOME + '/lib/HootJs');

var osmToTdsMap = {};
var tdsToOsmMap = {};

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
			try
			{
				// we sends request based on requested path
				var subPath = url.parse(request.url).pathname; 

				if(subPath == '/osmtotds'){
					osmtotds(request, response);
				} else if(subPath == '/tdstoosm') {
					tdstoosm(request, response);
				} else if(subPath == '/taginfo/key/values') {
					getTaginfoKeyFields(request, response);
				} else if(subPath == '/taginfo/keys/all') {
					getTaginfoKeys(request, response);
				} else if(subPath == '/schema') {
					getFilteredSchema(request, response);
				} else if(subPath == '/capabilities') {
					getCapabilities(request, response);
				} else {
					response.writeHead(404, {"Content-Type": "text/plain", 'Access-Control-Allow-Origin' : '*'});
					response.write("404 Not Found\n");
					response.end();
				}
			} 
			catch (err)
			{
					response.writeHead(500, {"Content-Type": "text/plain", 'Access-Control-Allow-Origin' : '*'});
					response.write("500 Internal Error: " + err + "\n");
					response.end();
			}

		}
	).listen(serverPort);
}

var getCapabilities = function(request, response)
{
	if(request.method === "GET"){
		response.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'});
		response.end(JSON.stringify(availableTrans));
	} else {
		response.writeHead(404, {"Content-Type": "text/plain", 'Access-Control-Allow-Origin' : '*'});
		response.write("Post not supported\n");
		response.end();
	}	
}

var populateOsmToTdsmap = function()
{
	var hoot = require(HOOT_HOME + '/lib/HootJs');
	logVerbose = hoot.logVerbose;
	logWarn = hoot.logWarn;
	if(!osmToTdsMap['TDSv40']) {
		osmToTdsMap['TDSv40'] = new hoot.TranslationOp({
									'translation.script':HOOT_HOME + '/translations/OSM_to_englishTDS.js',
									'translation.direction':'toogr'});
	}

	if(!osmToTdsMap['TDSv61']) {
		osmToTdsMap['TDSv61'] = new hoot.TranslationOp({
					    	'translation.script':HOOT_HOME + '/translations/OSM_to_englishTDS61.js',
					    	'translation.direction':'toogr'});
	}
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
			populateOsmToTdsmap();
			
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
		
		//Gets the translation schema for field population for a fcode
		// TODO: In the future we should get this from caller
		var schemaMap = {};
		schemaMap['TDSv40'] = require(HOOT_HOME + '/plugins/tds40_full_schema.js');
		schemaMap['TDSv61'] = require(HOOT_HOME + '/plugins/tds61_full_schema.js');
		
		var schema = schemaMap['TDSv61'].getDbSchema();
		if(trns){
			var schModule = schemaMap[trns];
			if(schModule){
				schema = schModule.getDbSchema();
			}
		}
		for(var ii=0; ii<schema.length; ii++){
			var elem = schema[ii];
			if(elem[params.idelem] === params.idval){
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


var populateTdsToOsmMap = function() 
{
	var hoot = require(HOOT_HOME + '/lib/HootJs');
	logVerbose = hoot.logVerbose;
	logWarn = hoot.logWarn;
	if(!tdsToOsmMap['TDSv40'])
	{
		tdsToOsmMap['TDSv40'] = new hoot.TranslationOp({
					    	'translation.script':HOOT_HOME + '/translations/englishTDS_to_OSM.js',
					    	'translation.direction':'toosm'});
	}

	if(!tdsToOsmMap['TDSv61'])
	{
		tdsToOsmMap['TDSv61'] = new hoot.TranslationOp({
					    	'translation.script':HOOT_HOME + '/translations/englishTDS61_to_OSM.js',
					    	'translation.direction':'toosm'});
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
			populateTdsToOsmMap();
			
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

		hoot = require(HOOT_HOME + '/lib/HootJs');
		createUuid = hoot.UuidHelper.createUuid;
		logVerbose = hoot.logVerbose;
		logWarn = hoot.logWarn;

		var translationsMap = {};
		translationsMap['TDSv40'] = require(HOOT_HOME + '/plugins/etds_osm.js'); 
		translationsMap['TDSv61'] = require(HOOT_HOME + '/plugins/etds61_osm.js');

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

// TDS taginfo service
// This retrieves associated tag keys/fields for requested fcode
// http://localhost:8233/taginfo/key/values?fcode=AP030&filter=ways&key=SGCC&page=1&query=Clo&rp=25&sortname=count_ways&sortorder=desc&translation=TDSv61
var getTaginfoKeyFields = function(request, response)
{
	if(request.method === "POST"){
		response.writeHead(404, {"Content-Type": "text/plain", 'Access-Control-Allow-Origin' : '*'});
		response.write("Post not supported\n");
		response.end();
	} else if(request.method === "GET"){
		// When we get get request on  /osmtotds then produce fields based on supplied fcode
		var  url_parts = url.parse(request.url,true);
		var params = url_parts.query;
		var featWGeomMatchSchema = null;
		// Line, Point, Area
		var geom = [];

		if(params.filter == 'nodes') {
			geom = ['Point'];
		} else if(params.filter == 'ways') {
			geom = ['Line','Area'];
		} 

		var trns = params.translation;
		
		//Gets the translation schema for field population for a fcode
		// TODO: In the future we should get this from caller		
		var schemaMap = {};
		schemaMap['TDSv40'] = require(HOOT_HOME + '/plugins/tds40_full_schema.js');
		schemaMap['TDSv61'] = require(HOOT_HOME + '/plugins/tds61_full_schema.js');
		
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
				// We will take anything first and then if there is matching the geom
				// then it will take precedent
				for(var iii=0; iii<geom.length; iii++){
					var curGeom = geom[iii];
					if(curGeom === elem.geom){
						featWGeomMatchSchema = elem;
					}
				}
				
			}
		}

		var fData = [];
		if(featWGeomMatchSchema){
			var fields = featWGeomMatchSchema.columns;
			for(var j=0; j<fields.length; j++) {
				var f = fields[j];
				
				if(f.name == params.key && f.enumerations){
					
					for(var jj=0; jj<f.enumerations.length; jj++) {
						var nf = {};
						var fEnum = f.enumerations[jj];
						nf['value'] = fEnum.name;
						nf['count'] = 1;
						nf['fraction'] = 0.19;
						nf['in_wiki'] = false;
						nf['description'] = fEnum.name;
						nf['internal_val'] = fEnum.value;
						fData.push(nf);
					}
				}
			}
		}

		var output = {};
		output['page'] = 1;
		output['rp'] = fData.length;
		output['total'] = fData.length;
		output['url'] = '';
		output['data'] = fData;
		response.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'});
		response.end(JSON.stringify(output));
	}	
}

// TDS taginfo service
// This retrieves associated tag keys/fields for requested fcode
// http://localhost:8233/taginfo/keys/all?page=1&rp=10&sortname=count_ways&sortorder=desc&fcode=AP030&translation=TDSv61&geometry=Line
var getTaginfoKeys = function(request, response)
{
	if(request.method === "POST"){
		response.writeHead(404, {"Content-Type": "text/plain", 'Access-Control-Allow-Origin' : '*'});
		response.write("Post not supported\n");
		response.end();
	} else if(request.method === "GET"){
		// When we get get request on  /osmtotds then produce fields based on supplied fcode
		var  url_parts = url.parse(request.url,true);
		var params = url_parts.query;
		var featWGeomMatchSchema = null;
		var lastMatchingFeatureSchema = null;
		// Line, Point, Area
		var geom = params.rawgeom;
		var trns = params.translation;
		
		//Gets the translation schema for field population for a fcode
		// TODO: In the future we should get this from caller
		var schemaMap = {};
		schemaMap['TDSv40'] = require(HOOT_HOME + '/plugins/tds40_full_schema.js');
		schemaMap['TDSv61'] = require(HOOT_HOME + '/plugins/tds61_full_schema.js');
		
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
				if(geom === elem.geom){
					featWGeomMatchSchema = elem;
				}
				lastMatchingFeatureSchema = elem;
			}
		}

		// Sometimes Point is only availble in area (i.e. AK120) so if no match then use whatever available.
		if(!featWGeomMatchSchema) {
			featWGeomMatchSchema = lastMatchingFeatureSchema;
		}
		var fData = [];
		if(featWGeomMatchSchema){
			var fields = featWGeomMatchSchema.columns;
			for(var j=0; j<fields.length; j++) {
				var f = fields[j];
				var nf = {};
				
				nf['key'] = f.name;
				nf['count_all'] = 100001;
				nf['count_all_fraction'] = 0.1;
				nf['count_nodes'] = 100001;
				nf['count_nodes_fraction'] = 0.1;
				nf['count_ways'] = 100001;
				nf['count_ways_fraction'] = 0.1;
				nf['count_relations'] = 100001;
				nf['count_relations_fraction'] = 0.1;
				nf['values_all'] = 100;
				nf['users_all'] = 100;
				nf['in_wiki'] = false;
				nf['in_josm'] = false;

				fData.push(nf);
			}
		}

		var output = {};
		output['page'] = 1;
		output['rp'] = fData.length;
		output['total'] = fData.length;
		output['url'] = '';
		output['data'] = fData;
		response.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'});
		response.end(JSON.stringify(output));
	}	
}

// This is where all interesting things happen interfacing with hoot core lib directly
var postHandler = function(data, response, translatorMap)
{
	var hoot = require(HOOT_HOME + '/lib/HootJs');
	logVerbose = hoot.logVerbose;
	logWarn = hoot.logWarn;
	
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

var getFilteredSchema = function(request, response) {
	if(request.method === "POST"){
		response.writeHead(404, {"Content-Type": "text/plain", 'Access-Control-Allow-Origin' : '*'});
		response.write("Post not supported\n");
		response.end();
	} else if(request.method === "GET"){
		var  url_parts = url.parse(request.url,true);
		var params = url_parts.query;

		// get query params
		var geomType = params.geometry;
		var searchStr = params.searchstr;
		var translation = params.translation;
		var maxLevDistance = 1*params.maxlevdst;
		var limitResult = 1*params.limit;
		
		//Gets the translation schema for field population for a fcode
		// TODO: In the future we should get this from caller
		var schemaMap = {};
		schemaMap['TDSv40'] = require(HOOT_HOME + '/plugins/tds40_full_schema.js');
		schemaMap['TDSv61'] = require(HOOT_HOME + '/plugins/tds61_full_schema.js');

		var schema = schemaMap['TDSv61'].getDbSchema();
		if(translation){
			var schModule = schemaMap[translation];
			if(schModule){
				schema = schModule.getDbSchema();
			}
		}

		var levDistList = [];
		for(var ii=0; ii<schema.length; ii++){
			var elem = schema[ii];
			if(geomType === elem.geom.toLowerCase()){
				var targetStr = elem.desc;
				var levDist = 1*getLevenshteinDistance(searchStr, targetStr);

				if(levDist <= maxLevDistance) {
					var scoredElem = {};
					scoredElem['levdist'] = levDist;
					scoredElem['name'] = elem.name;
					scoredElem['fcode'] = elem.fcode;
					scoredElem['desc'] = elem.desc;
					scoredElem['geom'] = elem.geom;
					levDistList.push(scoredElem);
				}

				var targetFCode = elem.fcode;
				levDist = 1*getLevenshteinDistance(searchStr, targetFCode);

				if(levDist <= maxLevDistance) {
					var scoredElem = {};
					scoredElem['levdist'] = levDist;
					scoredElem['name'] = elem.name;
					scoredElem['fcode'] = elem.fcode;
					scoredElem['desc'] = elem.desc;
					scoredElem['geom'] = elem.geom;
					levDistList.push(scoredElem);
				}
			}	
		}

		var sorted = levDistList.sort(function(a,b){
			return (1*a.levdist) - (1*b.levdist);
		});
		
		var nRes = sorted.length;
		if(nRes > limitResult) {
			nRes = limitResult;
		}
		var result = [];
		for(var n=0; n<nRes; n++) {
			result.push(sorted[n]);
		}
		response.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'});
		response.end(JSON.stringify(result));
	}
}

//https://en.wikipedia.org/wiki/Levenshtein_distance#cite_note-5
//http://www.codeproject.com/Articles/13525/Fast-memory-efficient-Levenshtein-algorithm
var getLevenshteinDistance = function(s, t) {
	if(s === t){
		return 0;
	}

	if(s.length === 0) {
		return t.length;
	}

	if(t.length === 0) {
		return s.length;
	}

	var v0 = [];
	var v1 = [];

	var v0Len = t.length + 1;

	for(var i=0; i<v0Len; i++) {
		v0[i] = i;
	}

	for(var i=0; i<s.length; i++) {
		v1[0] = i + 1;

		for(var j=0; j<t.length; j++) {
			var cost = 1;
			if(s.charAt(j) === t.charAt(j)) {
				cost = 0;
			}

			v1[j+1] = Math.min(Math.min(v1[j] + 1, v0[j + 1] + 1), v0[j] + cost);
		}

		for(var j=0; j<v0.length; j++) {
			v0[j] = v1[j];
		}
	}

	return v1[t.length];
}

