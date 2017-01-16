/************************************************************************
This is Node js implementation of Hoot Translation Server.
The purpose of this module is to provide the hoot-ui fast way
to translate OSM to TDS and TDS to OSM.
************************************************************************/
var http = require('http');
var url = require('url');
var serverPort = 8094;
var availableTrans = {TDSv40: {isavailable: true}, TDSv61: {isavailable: true}, MGCP: {isavailable: true}};
var HOOT_HOME = process.env.HOOT_HOME;

var schemaMap = {
    TDSv40: require(HOOT_HOME + '/plugins/tds40_full_schema.js'),
    TDSv61: require(HOOT_HOME + '/plugins/tds61_full_schema.js'),
    MGCP: require(HOOT_HOME + '/plugins/mgcp_schema.js')
};

var translationsMap = {
    TDSv40: '/translations/TDSv40.js',
    TDSv61: '/translations/TDSv61.js',
    MGCP: '/translations/MGCP_TRD4.js'
};

var osmToTdsMap = {
    TDSv40: '/translations/OSM_to_englishTDS.js',
    TDSv61: '/translations/OSM_to_englishTDS61.js',
    MGCP: '/translations/OSM_to_englishMGCP.js'
};

var tdsToOsmMap = {
    TDSv40: '/translations/englishTDS_to_OSM.js',
    TDSv61: '/translations/englishTDS61_to_OSM.js',
    MGCP: '/translations/englishMGCP_to_OSM.js'
};

var englishTranslationsMap = {
    TDSv40: '/plugins/etds40_osm.js',
    TDSv61: '/plugins/etds61_osm.js',
    MGCP: '/plugins/emgcp_osm.js'
};


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
                if (nThreadCnt > 0) {
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
        http.createServer(TranslationServer).listen(serverPort);
    }
} else {
    //I'm being called from a mocha test
    http.createServer(TranslationServer);
}


function TranslationServer(request, response) {
    try {
        var header = {'Access-Control-Allow-Origin': '*'};
        if (request.method === 'POST') {
            var payload = '';
            request.on('data', function(chunk) {
                payload += chunk;
            });

            request.on('end', function() {
                var urlbits = url.parse(request.url, true);
                var params = urlbits.query;
                params.method = request.method;
                params.path = urlbits.pathname;

                if (params.path === '/translate') {
                    params.tags = JSON.parse(payload);
                    header['Accept'] = 'application/json';
                    header['Content-Type'] = 'application/json';
                } else {
                    params.osm = payload;
                    header['Accept'] = 'text/xml';
                    header['Content-Type'] = 'text/xml';
                }
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
            header['Content-Type'] = 'application/json';
            response.writeHead(200, header);
            response.end(JSON.stringify(result));
        } else if (request.method === 'OPTIONS') {
            header["Access-Control-Allow-Methods"] = "POST, GET, OPTIONS";
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

function handleInputs(params) {
    var result;
    switch(params.path) {
        case '/translate':
            result = JSON.stringify(translate(params));
            break;
        case '/osmtotds':
            params.transMap = osmToTdsMap;
            params.transDir = 'toogr';
            result = osmtotds(params);
            break;
        case '/translateToEnglish':
            params.transMap = osmToTdsMap;
            params.transDir = 'toogr';
            result = osmtotds(params);
            break;
        case '/translateTo':
            params.transMap = translationsMap;
            params.transDir = 'toogr';
            result = osmtotds(params);
            break;
        case '/tdstoosm':
            params.transMap = tdsToOsmMap;
            params.transDir = 'toosm';
            result = tdstoosm(params);
            break;
        case '/translateFrom':
            params.transMap = translationsMap;
            params.transDir = 'toosm';
            result = tdstoosm(params);
            break;
        case '/taginfo/key/values':
            result = getTaginfoKeyFields(params);
            break;
        case '/taginfo/keys/all':
            result = getTaginfoKeys(params);
            break;
        case '/schema':
            result = getFilteredSchema(params);
            break;
        case '/capabilities':
            result = getCapabilities(params);
            break;
        case '/version':
            result = {version: '0.0.2'};
            break;
        default:
            throw new Error('Not found');
    }
    return result;
};

var getCapabilities = function(params) {
    if (params.method === 'GET'){
        return availableTrans;
    } else {
        throw new Error('Unsupported method');
    }
};

var translate = function(data) {
    data.translation = data.to || data.from;
    if (!availableTrans[data.translation] || !availableTrans[data.translation].isavailable) {
        throw new Error('Unsupported translation schema');
    }
    hoot = require(HOOT_HOME + '/lib/HootJs');
    createUuid = hoot.UuidHelper.createUuid;
    var trans = require(HOOT_HOME + englishTranslationsMap[data.translation]);
    var result;
    if (data.to) {
        if (data.english) {
            result = trans.OSMtoEnglish(data.tags, '', data.geom);
        } else {
            result = trans.OSMtoRaw(data.tags, '', data.geom);
        }
    } else if (data.from) {
        if (data.english) {
            result = trans.EnglishtoOSM(data.tags, '', data.geom);
        } else {
            result = trans.RawtoOSM(data.tags, '', data.geom);
        }
    }
    return result;
};

// This is where all interesting things happen interfacing with hoot core lib directly
var postHandler = function(data) {
    if (!availableTrans[data.translation] || !availableTrans[data.translation].isavailable) {
        throw new Error('Unsupported translation schema');
    }
    var hoot = require(HOOT_HOME + '/lib/HootJs');
    var result = {};
    var translation = new hoot.TranslationOp({
        'translation.script': HOOT_HOME + data.transMap[data.translation],
        'translation.direction': data.transDir});

    var map = new hoot.OsmMap();
    hoot.loadMapFromString(map, data.osm);
    translation.apply(map);
    return hoot.OsmWriter.toString(map);
};

// OSM to TDS request handler
var osmtotds = function(params) {

    if(params.method === 'POST'){
        //translate tags in xml from osm to a supported schema
        return postHandler(params);
    } else if (params.method === 'GET'){
        //Get fields for F_CODE from schema
        var schema = (params.translation) ? schemaMap[params.translation].getDbSchema() : schemaMap['TDSv61'].getDbSchema();

        var match = schema.filter(function(d) {
            return d[params.idelem] === params.idval && d.geom === params.geom;
        });

        if (match.length !== 1) {
            schemaError(params);
        }

        return match[0];
    }
};

// TDS to OSM handler
var tdstoosm = function(params) {

    if (params.method === 'POST') {
        //translate tags in xml from a supported schema to osm
        return postHandler(params);
    } else if (params.method === 'GET') {
        //Get OSM tags for F_CODE
        hoot = require(HOOT_HOME + '/lib/HootJs');
        createUuid = hoot.UuidHelper.createUuid;
        var englishToOsmMap = {
            TDSv40: require(HOOT_HOME + '/plugins/etds40_osm.js'),
            TDSv61: require(HOOT_HOME + '/plugins/etds61_osm.js'),
            MGCP: require(HOOT_HOME + '/plugins/emgcp_osm.js')
        };

        var osm = englishToOsmMap[params.translation].toOSM({
            'Feature Code': params.fcode
        }, '', '');

        return osm;
    }
}

// TDS taginfo service
// This retrieves associated tag keys/fields for requested fcode
// http://localhost:8233/taginfo/key/values?fcode=AP030&filter=ways&key=SGCC&page=1&query=Clo&rp=25&sortname=count_ways&sortorder=desc&translation=TDSv61
var getTaginfoKeyFields = function(params)
{
    if (params.method === 'POST') {
        throw new Error('Unsupported method');
    } else if (params.method === 'GET') {

        // Line, Point, Area
        var geom = [];

        if (params.filter == 'nodes') {
            geom = ['Point'];
        } else if (params.filter == 'ways') {
            geom = ['Line','Area'];
        }

        var schema = (params.translation) ? schemaMap[params.translation].getDbSchema() : schemaMap['TDSv61'].getDbSchema();
        var match = schema.filter(function(d) {
            return d.fcode === params.fcode && geom.indexOf(d.geom) > -1;
        });
        if (match.length !== 1) {
            schemaError({
                translation: params.translation,
                idelem: 'fcode',
                idval: params.fcode,
                geom: geom
            });
        }

        var data = match[0].columns.filter(function(c) {
            return c.name === params.key && c.enumerations;
        }).map(function(d) {
            return d.enumerations.map(function(e) {
                return {
                    value: e.name,
                    count: 1,
                    fraction: 0.19,
                    in_wiki: false,
                    description: e.name,
                    internal_val: e.value
                };
            });
        }).reduce(function(a, b) {
            return a.concat(b);
        }, []);

        return {
            page: 1,
            rp: data.length,
            total: data.length,
            url: '',
            data: data
        };
    }
}

// TDS taginfo service
// This retrieves associated tag keys/fields for requested fcode
// http://localhost:8233/taginfo/keys/all?page=1&rp=10&sortname=count_ways&sortorder=desc&fcode=AP030&translation=TDSv61&geometry=Line
var getTaginfoKeys = function(params)
{
    if (params.method === 'POST') {
        throw new Error('Unsupported method');
    } else if (params.method === 'GET') {

        // Line, Point, Area
        params.geom = params.rawgeom;

        var schema = (params.translation) ? schemaMap[params.translation].getDbSchema() : schemaMap['TDSv61'].getDbSchema();

        var match = schema.filter(function(d) {
            return d.fcode === params.fcode;
        });

        if (match.length > 1) {
            match = match.filter(function(d) {
                return d.geom === params.geom;
            })
        }

        if (match.length !== 1) {
            schemaError(params);
        }

        var data = match[0].columns.map(function(d) {
            return {
                key: d.name,
                count_all: 100001,
                count_all_fraction: 0.1,
                count_nodes: 100001,
                count_nodes_fraction: 0.1,
                count_ways: 100001,
                count_ways_fraction: 0.1,
                count_relations: 100001,
                count_relations_fraction: 0.1,
                values_all: 100,
                users_all: 100,
                in_wiki: false,
                in_josm: false,
            };
        });

        return {
            page: 1,
            rp: data.length,
            total: data.length,
            url: '',
            data: data
        };
    }
}

var getFilteredSchema = function(params) {
    if (params.method === 'POST') {
        throw new Error('Unsupported method');
    } else if (params.method === 'GET') {

        // get query params
        var geomType = params.geometry;
        var searchStr = params.searchstr;
        var translation = params.translation;
        var maxLevDistance = 1*params.maxlevdst;
        var limitResult = 1*params.limit;

        return searchSchema({
            translation: translation,
            geomType: geomType,
            searchStr: searchStr,
            limitResult: limitResult,
            maxLevDistance: maxLevDistance
        });
    }
}

var searchSchema = function(options) {
    var translation = options.translation || 'TDSv61';
    var geomType = options.geomType || '';
    var searchStr = options.searchStr || '';
    var limitResult = options.limitResult || 1000;
    var maxLevDistance = options.maxLevDistance || 20;
    var schema = schemaMap[translation].getDbSchema();

    var result = [];
    if (searchStr.length > 0) {

        //Check for search string in the description or fcode
        var exactMatches = schema
            .filter(function(d) {
                return d.geom.toLowerCase().indexOf(geomType.toLowerCase()) !== -1 &&
                ( d.desc.toLowerCase().indexOf(searchStr.toLowerCase()) !== -1 ||
                    d.fcode.toLowerCase().indexOf(searchStr.toLowerCase()) !== -1 )
                ;
            })
            .map(function(d) {
                return {
                    name: d.name,
                    fcode: d.fcode,
                    desc: d.desc,
                    geom: d.geom,
                    idx: Math.min( d.desc.toLowerCase().indexOf(searchStr.toLowerCase()),
                        d.fcode.toLowerCase().indexOf(searchStr.toLowerCase()) )
                }
            })
            .sort(function(a, b) {
                return a.idx - b.idx;
            })
            .slice(0, limitResult);

        result = result.concat(exactMatches);

        //If more results are needed to reach the limit, calculate fuzzy levenshtein matches
        if (exactMatches.length < limitResult) {

            var fuzzyMatches = schema
                .filter(function(d) {
                    return (! exactMatches.some(function(e) { //filter out exact matches
                        return e.desc === d.desc;
                    }) ) &&
                    d.geom.toLowerCase().indexOf(geomType.toLowerCase()) !== -1 &&
                    ( getLevenshteinDistance(searchStr.toLowerCase(), d.desc.toLowerCase()) <= maxLevDistance ||
                        getLevenshteinDistance(searchStr.toLowerCase(), d.fcode.toLowerCase()) <= maxLevDistance )
                    ;
                })
                .map(function(d) {
                    return {
                        name: d.name,
                        fcode: d.fcode,
                        desc: d.desc,
                        geom: d.geom,
                        idx: Math.min( getLevenshteinDistance(searchStr.toLowerCase(), d.desc.toLowerCase()),
                            getLevenshteinDistance(searchStr.toLowerCase(), d.fcode.toLowerCase()) )
                    }
                })
                .sort(function(a, b) {
                    return a.idx - b.idx;
                })
                .slice(0, limitResult);

            result = result.concat(fuzzyMatches)
                .slice(0, limitResult);
        }

    } else {
        // Return the first N elements of the schema
        result = schema.filter(function(d) {
                return d.geom.toLowerCase().indexOf(geomType.toLowerCase()) !== -1
            })
            .slice(0, limitResult)
            .map(function(d) {
                return {
                    name: d.name,
                    fcode: d.fcode,
                    desc: d.desc,
                    geom: d.geom
                }
            });
    }

    return result;
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

var schemaError = function(params) {
    throw new Error(params.translation + ' for ' + params.geom + ' with ' + params.idelem + '=' + params.idval + ' not found');
}

if (typeof exports !== 'undefined') {
    exports.searchSchema = searchSchema;
    exports.handleInputs = handleInputs;
    exports.TranslationServer = TranslationServer;
    exports.translate = translate;
}
