/************************************************************************
This is Node js implementation of Hoot Translation Server.
The purpose of this module is to provide the hoot-ui fast way
to translate feature tags between OSM and supported schemas.
************************************************************************/
var http = require('http');
var url = require('url');
var serverPort = 8094;
var availableTrans = {
    TDSv40: {isavailable: true},
    TDSv61: {isavailable: true},
    MGCP: {isavailable: true},
    GGDMv30: {isavailable: true}
};
var HOOT_HOME = process.env.HOOT_HOME;
if (typeof hoot === 'undefined') {
    hoot = require(HOOT_HOME + '/lib/HootJs');
}

// //Getting schema for fcode, geom type
var schemaMap = {
    TDSv40: require(HOOT_HOME + '/plugins/tds40_full_schema.js'),
    TDSv61: require(HOOT_HOME + '/plugins/tds61_full_schema.js'),
    MGCP: require(HOOT_HOME + '/plugins/mgcp_schema.js'),
    GGDMv30: require(HOOT_HOME + '/plugins/ggdm30_schema.js')
};

//Getting osm tags for fcode
var fcodeLookup = {
    TDSv40: require(HOOT_HOME + '/plugins/etds40_osm.js'),
    TDSv61: require(HOOT_HOME + '/plugins/etds61_osm.js'),
    MGCP: require(HOOT_HOME + '/plugins/emgcp_osm.js'),
    GGDMv30: require(HOOT_HOME + '/plugins/eggdm30_osm.js')
};

var translationsMap = {
    toogr: {
        TDSv40: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/TDSv40.js',
            'translation.direction': 'toogr'
        }),
        TDSv61: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/TDSv61.js',
            'translation.direction': 'toogr'
        }),
        MGCP: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/MGCP_TRD4.js',
            'translation.direction': 'toogr'
        }),
        GGDMv30: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/GGDMv30.js',
            'translation.direction': 'toogr'
        })
    },
    toosm: {
        TDSv40: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/TDSv40.js',
            'translation.direction': 'toosm'
        }),
        TDSv61: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/TDSv61.js',
            'translation.direction': 'toosm'
        }),
        MGCP: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/MGCP_TRD4.js',
            'translation.direction': 'toosm'
        }),
        GGDMv30: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/GGDMv30.js',
            'translation.direction': 'toosm'
        })
    }
};

var osmToTdsMap = {
    toogr: {
        TDSv40: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/OSM_to_englishTDS.js',
            'translation.direction': 'toogr'
        }),
        TDSv61: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/OSM_to_englishTDS61.js',
            'translation.direction': 'toogr'
        }),
        MGCP: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/OSM_to_englishMGCP.js',
            'translation.direction': 'toogr'
        }),
        GGDMv30: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/OSM_to_englishGGDM30.js',
            'translation.direction': 'toogr'
        })
    }
};

var tdsToOsmMap = {
    toosm: {
        TDSv40: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/englishTDS_to_OSM.js',
            'translation.direction': 'toosm'
        }),
        TDSv61: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/englishTDS61_to_OSM.js',
            'translation.direction': 'toosm'
        }),
        MGCP: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/englishMGCP_to_OSM.js',
            'translation.direction': 'toosm'
        }),
        GGDMv30: new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/englishGGDM30_to_OSM.js',
            'translation.direction': 'toosm'
        })
    }
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
        if (val.indexOf('port=') === 0) {
            var portArg = val.split('=');
            if (portArg.length === 2) {
                serverPort = 1*portArg[1];
            }
        }

        // thread count arg
        // defaults to number of CPU
        if (val.indexOf('threadcount=') === 0) {
            var nThreadArg = val.split('=');
            if (nThreadArg.length === 2) {
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
        for (var i=0; i<nCPU; i++) {
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
                var params = request.params || urlbits.query;
                params.method = request.method;
                params.path = request.path || urlbits.pathname;
                params.osm = payload;
                header['Accept'] = 'text/xml';
                header['Content-Type'] = 'text/xml';
                var result = handleInputs(params);
                response.writeHead(200, header);
                response.end(result);
            });

        } else if (request.method === 'GET') {
            var urlbits = url.parse(request.url, true);
            var params = request.params || urlbits.query;
            params.method = request.method;
            params.path = request.path || urlbits.pathname;
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
        case '/fcodes':
            result = getFCodes(params);
            break;
        case '/capabilities':
            result = getCapabilities(params);
            break;
        case '/version':
            result = {version: '0.0.3'};
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

// This is where all interesting things happen interfacing with hoot core lib directly
var postHandler = function(data) {
    if (!availableTrans[data.translation] || !availableTrans[data.translation].isavailable) {
        throw new Error('Unsupported translation schema');
    }
    var translation = data.transMap[data.transDir][data.translation];
    hoot.Settings.set({"ogr.esri.fcsubtype": "false"});
    hoot.Settings.set({"ogr.note.extra": "attribute"});
    if (data.transDir === "toogr") {
        hoot.Settings.set({"osm.map.writer.schema": data.translation});
    } else {
        hoot.Settings.set({"osm.map.writer.schema": "OSM"});
    }
    var map = new hoot.OsmMap();
    // loadMapFromString arguments: map, XML, preserve ID's, hoot:status
    hoot.loadMapFromString(map, data.osm, true);
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


        //geom type may be Vertex for tagged nodes that are members of ways
        var geom = params.geom;
        if (geom === 'Vertex') geom = 'Point';

        var match = schema.filter(function(d) {
            return d[params.idelem] === params.idval && d.geom === geom;
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
        createUuid = hoot.UuidHelper.createUuid;
        var osm = fcodeLookup[params.translation].toOSM({
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

var getFCodes = function(params) {
    if (params.method === 'POST') {
        throw new Error('Unsupported method');
    } else if (params.method === 'GET') {

        // get query params
        var geomType = params.geometry;
        var translation = params.translation;

        //Treat vertex geom type as point
        if (geomType.toLowerCase() === 'vertex') geomType = 'point';

        //Get valid FCODEs for this translation and geometry type
        var schema = schemaMap[translation].getDbSchema();

        console.log(geomType + ', ' + translation + ', ' + schema.length);
        var fcodes = schema
            .filter(function(d) {
                return d.geom.toLowerCase() === geomType.toLowerCase();
            })
            .map(function(d) {
                return {
                    fcode: d.fcode,
                    desc: d.desc
                }
            })
            .sort(function(a, b) {
                return a.fcode - b.fcode;
            });

        return fcodes;
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
    var leinSearch = getLein(searchStr);

    //Treat vertex geom type as point
    if (geomType.toLowerCase() === 'vertex') geomType = 'point';

    // get desc and fcode matching results
    var schemaMatches = schema
        .filter(function(d) {
            return d.geom.toLowerCase().indexOf(geomType.toLowerCase()) !== -1  &&
                (  d.desc.toLowerCase().indexOf(searchStr.toLowerCase()) !== -1 ||
                   d.fcode.toLowerCase().indexOf(searchStr.toLowerCase()) !== -1 
                );
        });

    var result = [];
    if (searchStr.length > 0) {
        // find exact fcode matches and sort.
        var fcodeMatches = schemaMatches
            .filter(function(d) { 
                return d.fcode.toLowerCase().indexOf(searchStr.toLowerCase()) !== -1;
            })
            .map(function(d) {
                return {
                    name: d.name,
                    fcode: d.fcode,
                    desc: d.desc,
                    geom: d.geom,
                    idx: Number(d.fcode.toLowerCase().replace(searchStr, ''))
                }
            })
            .sort(function(a, b) { return a.idx - b.idx })
            .slice(0, limitResult);

        result = result.concat(fcodeMatches);
       
        // if fcode matches below result limit,
        // add desc matches to results.
        if (fcodeMatches.length < limitResult) {

            var limitLeft = limitResult - fcodeMatches.length;
                descMatches = schemaMatches
                    .filter(function(d) { 
                        return d.desc.toLowerCase().indexOf(searchStr.toLowerCase()) !== -1 &&
                               d.fcode.toLowerCase().indexOf(searchStr.toLowerCase()) === -1;
                    })
                    .filter(function(d) {
                        return {
                            name: d.name,
                            fcode: d.fcode,
                            desc: d.desc,
                            geom: d.geom,
                            idx: d.desc.toLowerCase().indexOf(searchStr.toLowerCase())
                        } 
                    })
                    .sort(function(a, b) { return a.idx - b.idx })
                    .slice(0, limitLeft);

            result = result.concat(descMatches);

            // if limit result still unmet,
            // add in fuzzy matches tags
            if ((descMatches.length + fcodeMatches.length) < limitLeft) {
                limitLeft = limitResult - (descMatches.length + fcodeMatches.length);

                // make sure only matching on those 
                // - valid geometry
                // - not a match in desc or fcode
                var fuzzyMatches = schema
                        .filter(function(d) {
                            var validGeom = d.geom.toLowerCase().indexOf(geomType.toLowerCase()) !== -1,
                                notMatch = (
                                    d.fcode.toLowerCase().indexOf(searchStr.toLowerCase()) === -1  &&
                                    d.desc.toLowerCase().indexOf(searchStr.toLowerCase()) === -1
                                );

                            return validGeom && notMatch;
                        }).map(function(d) {
                            var minDescDistance = Math.min.apply(
                                null, d.desc.toLowerCase().split(/\s+/).map(function(word) {
                                    var leinWord = getLein(word);
                                    return leinWord[0] !== leinSearch[0] ? Infinity : Math.abs(
                                        Number(leinSearch.substr(1, leinSearch.length)) -
                                        Number(leinWord.substr(1, leinWord.legth))
                                    );
                                })
                            )

                            return {
                                name: d.name,
                                fcode: d.fcode,
                                desc: d.desc,
                                geom: d.geom,
                                idx: minDescDistance
                            }
                        })
                        .sort(function(a, b) { return a.idx - b.idx })
                        .slice(0, limitLeft)

                result = result.concat(fuzzyMatches)
                    .slice(0, limitResult)
            }

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

// src: talisam
// https://github.com/Yomguithereal/talisman/blob/master/src/phonetics/lein.js
var getLein = function(name) {
    if (typeof name !== 'string') {
        throw Error('talisman/phonetics/lein: the given name is not a string.');
    }

    // helpers start //
    var pad = function(code) {
        return (code + '0000').slice(0, 4);
    };
    var seq = function(target) {
        return typeof target === 'string' ? target.split('') : target;
    };
    var squeeze = function (target) {
        var isString = typeof target === 'string',
            sequence = seq(target), 
            squeezed = [sequence[0]];
        
        for (var i = 1, l = sequence.length; i < l; i++) {
            if (sequence[i] !== sequence[i - 1]) {
                squeezed.push(sequence[i]);
            }
        }
        
        return isString ? squeezed.join('') : squeezed;
    }
    var translation = function(first, second) {
        var index = {};
        
        first = first.split(''),
        second = second.split('');
      
        if (first.length !== second.length)
          throw Error('talisman/helpers#translation: given strings don\'t have the same length.');
      
        for (var i = 0, l = first.length; i < l; i++)
          index[first[i]] = second[i];
      
        return index;
    }
    // helpers end //
      
    // constants start //
    var DROPPED = /[AEIOUYWH]/g;
    var TRANSLATION = translation('DTMNLRBFPVCJKGQSXZ', '112233444455555555');
    // constants end //

    if (typeof name !== 'string') {
        throw Error('talisman/phonetics/lein: the given name is not a string.');
    }
    var code = name
        .toUpperCase()
        .replace(/[^A-Z\s]/g, '');

    // 1-- Keeping the first letter
    var first = code[0];
    code = code.slice(1);

    // 2-- Dropping vowels and Y, W & H
    code = code.replace(DROPPED, '');

    // 3-- Dropping consecutive duplicates and truncating to 4 characters
    code = squeeze(code).slice(0, 4);

    // 4-- Translations
    var backup = code;
    code = '';

    for (var i = 0, l = backup.length; i < l; i++) {
        code += TRANSLATION[backup[i]] || backup[i];
    }

    return pad(first + code); 
}

var schemaError = function(params) {
    var msg = params.translation + ' for ' + params.geom + ' with ' + params.idelem + '=' + params.idval + ' not found';
    console.error(msg);
    throw new Error(msg);
}

if (typeof exports !== 'undefined') {
    exports.getFCodes = getFCodes;
    exports.searchSchema = searchSchema;
    exports.handleInputs = handleInputs;
    exports.TranslationServer = TranslationServer;
    exports.getLein = getLein;
}
