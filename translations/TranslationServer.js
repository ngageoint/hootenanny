/************************************************************************
This is Node js implementation of Hoot Translation Server.
The purpose of this module is to provide the hoot-ui fast way
to translate feature tags between OSM and supported schemas. See
docs/developer/ElementTranslationService.asciidoc
************************************************************************/
var http = require('http');
var url = require('url');
var fs = require('fs');
var serverPort = 8094;

var HOOT_HOME = process.env.HOOT_HOME;
if (typeof hoot === 'undefined') {
    hoot = require(HOOT_HOME + '/lib/HootJs');
    hoot.Log.setLogLevel("warn");
    hoot.Settings.set({"map.reader.add.child.refs.when.missing":"true"});
    hoot.Settings.set({"ogr.add.uuid": "false"});
    hoot.Settings.set({"ogr.esri.fcsubtype": "false"});
    hoot.Settings.set({"ogr.note.extra": "attribute"});
    hoot.Settings.set({"reader.add.source.datetime": "false"});
    hoot.Settings.set({"writer.include.circular.error.tags": "false"});
    hoot.Settings.set({"writer.thematic.structure":"false"});

    // LOTS of debug output on the console (journalctl -xe -u tomcat8)
    // hoot.Settings.set({"ogr.debug.dumptags": "true"});


    // Throw errors instead of returning partial translations/o2s_X features
    hoot.Settings.set({'ogr.throw.error':'true'});

    // Setting this here as a placeholder. The default is "true"
    hoot.Settings.set({"reader.drop.defaults": "true"});

    //Tests should set hashseedzero to true for consistent results
    if( typeof hashseedzero !== 'undefined' && hashseedzero == true ) hoot.Settings.set({"hash.seed.zero": "true"});
}

// Setup the lists of schema
var availableTrans = {};
var availableTranslations = [];
var availablePresets = {};
var schemaMap = {};
var fcodeLookup = {};

var translationsMap = {
    toogr: {},
    toosm: {}
};


// Loop through all "translations*" directories and load config files if they exist.
// The structure of the translation config file is very basic json array.
// NOTE: This is also used by the Hoot services for the list of available translations
// [
//   {
//     "name" : "smurf",
//     "description" : "The name of the format",
//     "canExport" : true,
//     "schema":"smurf_schema.js",
//     "path" : "Smurf.js"
//   },
//   {
//     "name" : "anotherSmurf",
//     "description" : "The name of the format",
//     "canExport" : true,
//     "schema":"anotherSmurf_schema.js",
//     "importPath" : "import_smurf.js",
//     "exportPath" : "export_smurf.js"
//   }
// ]

fs.readdirSync(HOOT_HOME,{withFileTypes:true}).filter(file => file.isDirectory() && (file.name.indexOf('translations') == 0)).forEach( function (file){
  var tLocal = {};
  var dirName = HOOT_HOME + '/' + file.name + '/';

  // Should this be in a config var?
  if (fs.existsSync(dirName + 'translationConfig.json'))
  {
    try {
          tLocal = require(dirName + 'translationConfig.json');

          tLocal.forEach(function (fmt) {
              // If we don't have a specified schema then we don't support schema switching in the UI
              if (fmt.schema)
              {
                availableTrans[fmt.name] = {"isavailable":true};
                availableTranslations.push(fmt.name);
                availablePresets[fmt.name] = ((dirName !== 'translations') ? dirName : '')
                    + fmt.name + '_preset.xml';
                schemaMap[fmt.name] = require(dirName + fmt.schema);

                // Either a "path" or "importPath" and "exportPath"
                if (fmt.path)
                {
                  fcodeLookup[fmt.name] = require(dirName + fmt.path);
                  translationsMap.toogr[fmt.name] = new hoot.SchemaTranslationOp({
                    'schema.translation.script': dirName + fmt.path,
                    'schema.translation.direction':'toogr'});
                  translationsMap.toosm[fmt.name] = new hoot.SchemaTranslationOp({
                    'schema.translation.script': dirName + fmt.path,
                    'schema.translation.direction':'toosm'});
                }
                else
                {
                  fcodeLookup[fmt.name] = require(dirName + fmt.importPath)
                  translationsMap.toogr[fmt.name] = new hoot.SchemaTranslationOp({
                    'schema.translation.script': dirName + fmt.exportPath,
                    'schema.translation.direction':'toogr'});
                  translationsMap.toosm[fmt.name] = new hoot.SchemaTranslationOp({
                    'schema.translation.script': dirName + fmt.importPath,
                    'schema.translation.direction':'toosm'});
                }
              }
        });
    } catch (e) {
      console.log('Translation Config Error: ' + e);
    }
  }
});

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
                try {
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
                } catch (err) {
                    var status = 500;
                    if (err.message.indexOf('Unsupported') > -1)
                        status = 400;
                    if (err.message.indexOf('Not found') > -1)
                        status = 404;
                    response.writeHead(status, header);
                    response.end(JSON.stringify({error: err.message}));
                }
            });

        } else if (request.method === 'GET') {
            var urlbits = url.parse(request.url, true);
            var params = request.params || urlbits.query;
            params.method = request.method;
            params.path = request.path || urlbits.pathname;
            var result = handleInputs(params);

            if (params.path === '/presets') {
                header['Content-Type'] = 'application/xml';
            } else {
                header['Content-Type'] = 'application/json';
                result = JSON.stringify(result);
            }
            response.writeHead(200, header);
            response.end(result);
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
        case '/translateTo':
            params.transMap = translationsMap;
            params.transDir = 'toogr';
            result = osm2ogr(params);
            break;
        case '/translateFrom':
            params.transMap = translationsMap;
            params.transDir = 'toosm';
            result = ogr2osm(params);
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
        case '/supportedGeometries':
            result = getSupportedGeometries(params);
            break;
        case '/translations':
            result = getTranslations(params);
            break;
        case '/presets':
            result = getPresets(params);
            break;
        case '/fieldMappings':
            result = getFieldMappings(params);
            break;
        case '/columns':
            result = getColumn(params);
            break;
        case '/version':
            result = {version: '0.0.5'};
            break;
        default:
            throw new Error('Not found');
    }
    return result;
};

var getSupportedGeometries = function(params) {
    if (params.method === 'GET') {
        var schema = (params.translation) ? schemaMap[params.translation].getDbSchema() : schemaMap['TDSv61'].getDbSchema();
        var fcode = params.fcode;
        // same as saying, filter schema for those with matching fcode, then mapping to be only geometry
        return schema.reduce(function(geoms, d) {
            if (d.fcode === fcode) geoms.push(d.geom);
            return geoms;
        }, []);
    } else {
        throw new Error('Unsupported method');
    }
}

var getCapabilities = function(params) {
    if (params.method === 'GET'){
        return availableTrans;
    } else {
        throw new Error('Unsupported method');
    }
};

var getTranslations = function(params) {
    if (params.method === 'GET'){
        return availableTranslations;
    } else {
        throw new Error('Unsupported method');
    }
};

var getPresets = function(params) {
    if (params.method === 'GET') {
        var presetFile = availablePresets[params.translation];
        if (fs.existsSync(presetFile)) {
            return fs.readFileSync(presetFile);
        } else {
            throw new Error('Not found');
        }
    } else {
        throw new Error('Unsupported method');
    }
};

// This is where all interesting things happen interfacing with hoot core lib directly
var postHandler = function(data) {
    if (data.osm === '')
        throw new Error('Unsupported payload cannot be empty');
    if (availableTranslations.indexOf(data.translation) === -1) {
        throw new Error('Unsupported translation schema ' + data.translation);
    }
    var translation = data.transMap[data.transDir][data.translation];
    var haveDropDefaults = data.dropDefaults && (data.dropDefaults === "true" || data.dropDefaults === "false");
    var dropDefaultsInitial = null;

    if (haveDropDefaults) {
        dropDefaultsInitial = hoot.Settings.get("reader.drop.defaults");
        hoot.Settings.set({"reader.drop.defaults": data.dropDefaults});
    }

    if (data.transDir === "toogr") {
        hoot.Settings.set({"map.writer.schema": data.translation});
    } else {
        hoot.Settings.set({"map.writer.schema": "OSM"});
    }

    var map = new hoot.OsmMap();
    // loadMapFrom(JSON)String arguments: map, XML, preserve ID's, hoot:status
    if (data.osm[0] === "{")
        hoot.loadMapFromJSONString(map, data.osm, true);
    else
        hoot.loadMapFromString(map, data.osm, true);

    translation.apply(map);

    if (haveDropDefaults) {
        hoot.Settings.set({"reader.drop.defaults": dropDefaultsInitial});
    }

    return hoot.OsmWriter.toString(map);
};


// OSM to Translated Schema request handler
var osm2ogr = function(params) {

    if(params.method === 'POST'){
        //translate tags in xml from osm to a supported schema
        return postHandler(params);
    } else if (params.method === 'GET'){
        //Get fields for F_CODE from schema
        var schema = (params.translation) ? schemaMap[params.translation].getDbSchema() : schemaMap['TDSv61'].getDbSchema();

        //geom type may be Vertex for tagged nodes that are members of ways
        var geom = params.geom.split(',').map(function(geom) {
            return geom === 'Vertex' ? 'Point' : geom;
        });
        var k = params.key || params.idelem;
        var v = params.value || params.idval;
        var match = schema.filter(function(d) {
            return d[k] === v && geom.indexOf(d.geom) > -1;
        });

        if (!match.length) {
            schemaError(params);
        } else if (match.length === 1) {
            return match[0]
        } else {
            // find intersections of matching...
            match = match.sort(function(schemaA, schemaB) { return schemaA.columns.length - schemaB.columns.length });
            var base = match.shift();
            var finColumns = []

            // for each column in the base schema...
            base.columns.forEach(function(column) {
                // first check to see if the current column name is found
                // in other schema columns
                var matches = [];
                match.forEach(function(schema) {
                    schema.columns.forEach(function(col) { if (col.name === column.name) matches.push(col) })
                })
                // if it is...
                if (matches.length) {
                    // and it's a non enumerated column, just add it to the finColumns
                    if (!matches[0].hasOwnProperty('enumerations')) {
                        finColumns.push(column)
                    } else {
                        // otherwise build the intersection of
                        // all matches' enumerations...
                        matches.push(column)
                        var enumerations = [].concat
                            .apply([], matches.map(function(column) { return column.enumerations }))
                            .filter(function(enumeration, index, self) {
                                return self.findIndex(function(e) {
                                    return e.name === enumeration.name
                                }) === index;
                            })

                        // and then only if that intersection
                        // exists, add the column to the final column
                        if (enumerations.length) {
                            column.enumerations = enumerations
                            finColumns.push(column)
                        }
                    }
                }
            })

            // final check to see if no
            // intersection was found amongst columns
            if (!finColumns.length) {
                schemaError(params);
            } else {
                return {
                    desc: match[0].desc,
                    fcode: match[0].fcode,
                    name: match[0].desc.toUpperCase(),
                    columns: finColumns
                }
            }
        }
    }
}

// Translated Schema to OSM handler
var ogr2osm = function(params) {

    if (params.method === 'POST') {
        //translate tags in xml from a supported schema to osm
        return postHandler(params);
    } else if (params.method === 'GET') {
        //Get OSM tags for F_CODE
        // var osm = translateToOsm.toosm[params.translation].toOSM({
        //     'FCODE': params.fcode
        createUuid = hoot.UuidHelper.createUuid;
        var osm = fcodeLookup[params.translation].toOsm({
            'FCODE': params.fcode
        }, '', params.geom || '');

        if (JSON.stringify(osm) === '{}') {
            return {'error':'Feature Code ' + params.fcode + ' is not valid for ' + params.translation};
        }

        return osm;
    }
}

var getFCodes = function(params) {
    if (params.method === 'POST') {
        throw new Error('Unsupported method');
    } else if (params.method === 'GET') {

        // get query params
        var geomType = params.geometry;
        var translation = params.translation;

        //Get valid FCODEs for this translation and geometry type
        var schema = schemaMap[translation].getDbSchema();
        var fcodes;
        if (geomType) {
            //Treat vertex geom type as point
            if (geomType.toLowerCase() === 'vertex') geomType = 'point';

            fcodes = schema
                .filter(function(d) {
                    return d.geom.toLowerCase() === geomType.toLowerCase();
                })
                .map(function(d) {
                    return {
                        fcode: d.fcode,
                        desc: d.desc
                    }
                });
        } else {

            fcodes = Object.values(schema
                .reduce(function(map, d) {
                    if (map[d.fcode]) {
                        map[d.fcode].geom.push(d.geom);
                    } else {
                        map[d.fcode] = {
                            fcode: d.fcode,
                            desc: d.desc,
                            geom: [d.geom]
                        };
                    }
                    return map;
                }, {}));
        }

        return fcodes.sort(function(a, b) {
                return a.fcode - b.fcode;
            });
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
        var maxLeinDistance = 1*params.maxleindst;
        var limitResult = 1*params.limit;

        return searchSchema({
            translation: translation,
            geomType: geomType,
            searchStr: searchStr,
            limitResult: limitResult,
            maxLeinDistance: maxLeinDistance
        });
    }
}

var getFieldMappings = function(options) {
    var translation = options.translation || 'TDSv61';
    var schema = schemaMap[translation].getDbSchema();
    var fieldMappings = [];
    var columnTracker = {};

    schema.forEach(function(fcode) {
        fcode.columns.forEach(function(col){
            var colName = col.name + '::' + col.desc;
            if (!columnTracker.hasOwnProperty(colName)) {
                columnTracker[colName] = true;
                fieldMappings.push(colName);
            }
        });
    });

    return fieldMappings;
}

var getColumn = function(options) {
    var translation = options.translation || 'TDSv61';
    var column = options.column;
    var schema = schemaMap[translation].getDbSchema();
    var columnSchema = [];

    schema.forEach(function(fcode) {
        fcode.columns.forEach(function(c) {
            if (c.name === column) {
                if (columnSchema.length === 0) {
                    columnSchema = c.type === 'enumeration' ? c.enumerations.map(function(e) { return e.name }) : ['Value'];
                } else if (c.type === 'enumeration') {
                    c.enumerations.forEach(function(e) {
                        if (columnSchema.indexOf(e.name) === -1) {
                            columnSchema.push(e.name);
                        }
                    });
                }
            }
        });
    });

    return columnSchema;
}

var searchSchema = function(options) {
    var translation = options.translation || 'TDSv61';
    var geomType = options.geomType || '';
    var searchStr = options.searchStr || '';
    var limitResult = options.limitResult || 20;
    var maxLeinDistance = options.maxLeinDistance || 200;
    var schema = schemaMap[translation].getDbSchema();
    var leinSearch = getLein(searchStr).toLocaleLowerCase();

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
        var limitLeft = limitResult;
            fcodeMatches = schemaMatches
                .filter(function(d) {
                    return d.fcode.toLowerCase().indexOf(searchStr.toLowerCase()) !== -1;
                })
                .map(function(d) {
                    return {
                        name: d.name,
                        fcode: d.fcode,
                        desc: d.desc,
                        geom: d.geom,
                        idx: Number(d.fcode.toLowerCase().replace(searchStr.toLowerCase(), ''))
                    };
                })
                .sort(function(a, b) { return a.idx - b.idx; })
                .slice(0, limitResult);

        result = result.concat(fcodeMatches);
        limitLeft = limitResult - fcodeMatches.length;

        // if partial exact fcode matches don't fill limit
        // add desc matches to results.
        if (limitLeft > 0) {

            var descMatches = schemaMatches
                    .filter(function(d) {
                        return d.desc.toLowerCase().indexOf(searchStr.toLowerCase()) !== -1 &&
                               d.fcode.toLowerCase().indexOf(searchStr.toLowerCase()) === -1;
                    })
                    .map(function(d) {
                        return {
                            name: d.name,
                            fcode: d.fcode,
                            desc: d.desc,
                            geom: d.geom,
                            idx: d.desc.toLowerCase().indexOf(searchStr.toLowerCase())
                        };
                    })
                    .sort(function(a, b) { return a.idx - b.idx; })
                    .slice(0, limitLeft);

            result = result.concat(descMatches);
            limitLeft = limitLeft - descMatches.length;

            // if no partial exact fcode or desc matches
            // use fuzzy matches (Lein code) on desc
            if (result.length === 0) {
                // make sure only matching on those
                // - valid geometry
                // - not a match in description or fcode
                var fuzzyMatches = schema
                        .filter(function(d) {
                            var validGeom = d.geom.toLowerCase().indexOf(geomType.toLowerCase()) !== -1,
                                notMatch = (
                                    d.fcode.toLowerCase().indexOf(searchStr.toLowerCase()) === -1  &&
                                    d.desc.toLowerCase().indexOf(searchStr.toLowerCase()) === -1
                                );

                            return validGeom && notMatch;
                        })
                        .map(function(d) {
                            var minDescDistance = Math.min.apply(
                                null, d.desc.toLowerCase().split(/\s+/).map(function(word) {
                                    var leinWord = getLein(word).toLocaleLowerCase();
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
                        .filter(function(d) { return d.idx <= maxLeinDistance })
                        .sort(function(a, b) { return a.idx - b.idx })
                        .slice(0, limitLeft)

                result = result.concat(fuzzyMatches);
                limitLeft = limitLeft - fuzzyMatches.length;

                // when traditional fuzzy matches still do not return anything, try to
                // use 'near by keys' as the lead character in string to find matches
                // motivation here is to catch things like 'vuilding' instead of building
                if (limitLeft > 0) {

                    // after nearby (keyboard) character replacement
                    // try exact matches on descriptions
                    var searchStrings = getFuzzyStrings(searchStr),
                        keyDescMatches = searchStrings.map(function(str) {
                            return schema
                                .filter(function(d) {
                                    return d.geom.toLowerCase().indexOf(geomType.toLowerCase()) !== -1  &&
                                        d.desc.toLowerCase().indexOf(str.toLowerCase()) !== -1;
                                })
                                .map(function(d) {
                                    return {
                                        name: d.name,
                                        fcode: d.fcode,
                                        desc: d.desc,
                                        geom: d.geom,
                                        idx: d.desc.toLowerCase().indexOf(str.toLowerCase())
                                    }
                                })
                                .sort(function(a, b) { return a.idx - b.idx })

                        });

                    // flatten, then concat keyDescMatches
                    keyDescMatches = [].concat.apply([], keyDescMatches)
                        .slice(0, limitLeft);

                    result = result.concat(keyDescMatches);
                    limitLeft = limitResult - keyDescMatches.length;

                    // try fuzzy matches on nearby (keyboard) character replaced descriptions
                    if (limitLeft > 0) {
                        var fuzzyKeyMatches = searchStrings.map(function(str) {
                            var leinStr = getLein(str);

                            return schema
                                .filter(function(d) {
                                    var validGeom = d.geom.toLowerCase().indexOf(geomType.toLowerCase()) !== -1,
                                        notMatch = (
                                            d.fcode.toLowerCase().indexOf(str.toLowerCase()) === -1,
                                            d.desc.toLowerCase().indexOf(str.toLowerCase()) === -1
                                        )

                                    return validGeom && notMatch;
                                }).
                                map(function(d) {
                                    var minDescDistance = Math.min.apply(
                                        null, d.desc.split(/\+s/g).map(function(word) {
                                            var leinWord = getLein(word);

                                            return leinWord[0] !== leinStr[0] ? Infinity : Math.abs(
                                                Number(leinStr.substr(1, leinStr.length)) -
                                                Number(leinWord.substr(1, leinWord.length))
                                            )
                                        })
                                    );

                                    return {
                                        name: d.name,
                                        fcode: d.fcode,
                                        desc: d.desc,
                                        geom: d.geom,
                                        idx: minDescDistance
                                    }
                                })
                                .filter(function(d) { return d.idx <= maxLeinDistance; })
                                .sort(function(a, b) { return a.idx - b.idx; })
                                .slice(0, 5)

                        });

                        // make sure best fuzzy match goes first before flattening them...
                        fuzzyKeyMatches = fuzzyKeyMatches.sort(function(a, b) {
                            var aMinLein = Math.min.apply(null, a.map(function(d) { return d.idx })),
                                bMinLein = Math.min.apply(null, a.map(function(d) { return d.idx }));

                            return aMinLein - bMinLein;
                        });

                        fuzzyKeyMatches = []
                            .concat.apply([], fuzzyKeyMatches)
                            .slice(0, limitLeft);

                        result = result.concat(fuzzyKeyMatches);
                    }
                }

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
                };
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

var getIntendedKeys = function(key) {
    var keyboard = {
        'q': ['a', 'w'],
        'w': ['q', 'e'],
        'e': ['w', 'r'],
        'r': ['e', 't'],
        't': ['r', 't'],
        'y': ['t', 'u'],
        'u': ['y', 'i'],
        'i': ['u', 'o'],
        'o': ['i', 'p'],
        'p': ['o', 'l'],'[': ['p'], '{': ['p'],
        'a': ['s', 'z'],
        's': ['a', 'd'],
        'd': ['s', 'f'],
        'f': ['d', 'g'],
        'g': ['f', 'h'],
        'h': ['g', 'j'],
        'j': ['h', 'k'],
        'l': ['k', 'o'], ';': ['p', 'l'], ':': ['p', 'l'],
        'z': ['a', 'x', 's'],
        'x': ['z', 'c', 's', 'd'],
        'c': ['x', 'd', 'f', 'v'],
        'v': ['c', 'f', 'g', 'b'],
        'b': ['v', 'g', 'h', 'n'],
        'n': ['b', 'h', 'j', 'm'],
        'm': ['k', 'n'],
        ',': ['m', 'k', 'l'], '<': ['m', 'k', 'l'],
        '.': ['l'], '>': ['l']
    }

    return keyboard[key.toLowerCase()];
}

var getFuzzyStrings = function(searchStr) {
    var tail = searchStr.substr(1, searchStr.length);

    return getIntendedKeys(searchStr[0])
        .map(function(key) {
            return key + tail
        });
}

var schemaError = function(params) {
    var msg = params.translation + ' for ' + params.geom + ' with ' + params.idelem + '=' + params.idval + ' not found';
    throw new Error(msg);
}

if (typeof exports !== 'undefined') {
    exports.getFCodes = getFCodes;
    exports.searchSchema = searchSchema;
    exports.handleInputs = handleInputs;
    exports.TranslationServer = TranslationServer;
    exports.getLein = getLein;
    exports.getIntendedKeys = getIntendedKeys;
    exports.getFuzzyStrings = getFuzzyStrings;
}
