var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js');
var httpMocks = require('node-mocks-http');
var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

//Comment out for now due to:
//     TypeError: Cannot call method 'getDbSchema' of undefined

    // describe('translate', function() {
    //     //MGCP
    //     it('should translate OSMtoRaw', function() {
    //         var data = server.translate({
    //             tags: {
    //                 'error:circular': '5',
    //                 'highway': 'road',
    //                 'hoot:status': '1',
    //                 'name': '23RD ST NW',
    //                 'hoot': 'DcGisRoadsCucumber'
    //             },
    //             to: 'MGCP',
    //             geom: 'Line'
    //         });
    //         assert.equal(data[0].attrs.NAM, '23RD ST NW');
    //         assert.equal(data[0].attrs.FCODE, 'AP030');
    //         assert.equal(data[0].attrs.TXT, '<OSM>{"highway":"road"}</OSM>');
    //     });

    //     it('should translate OSMtoEnglish', function() {
    //         var data = server.translate({
    //             tags: {
    //                 'error:circular': '5',
    //                 'highway': 'road',
    //                 'hoot:status': '1',
    //                 'name': '23RD ST NW',
    //                 'hoot': 'DcGisRoadsCucumber'
    //             },
    //             to: 'MGCP',
    //             geom: 'Line',
    //             english: true
    //         });
    //         assert.equal(data.attrs['Name'], '23RD ST NW');
    //         assert.equal(data.attrs['Feature Code'], 'AP030:Road');
    //         assert.equal(data.attrs['Associated Text'], '<OSM>{"highway":"road"}</OSM>');
    //     });

    //     it('should translate EnglishtoOSM', function() {
    //         var data = server.translate({
    //             tags: {
    //                 //'error:circular': '5',
    //                 'Feature Code': 'AP030:Road',
    //                 //'hoot:status': '1',
    //                 'Name': '23RD ST NW',
    //                 'Thoroughfare Class': 'Unknown'
    //                 //'hoot': 'DcGisRoadsCucumber'
    //             },
    //             from: 'MGCP',
    //             geom: 'Line',
    //             english: true
    //         });
    //         assert.equal(data.attrs.highway, 'road');
    //         assert.equal(data.attrs.name, '23RD ST NW');
    //     });

    //     it('should translate RawtoOSM', function() {
    //         var data = server.translate({
    //             tags: {
    //                 //'error:circular': '5',
    //                 'FCODE': 'AP030',
    //                 //'hoot:status': '1',
    //                 'NAM': '23RD ST NW',
    //                 'HCT': '0'
    //                 //'hoot': 'DcGisRoadsCucumber'
    //             },
    //             from: 'MGCP',
    //             geom: 'Line'
    //         });
    //         assert.equal(data.highway, 'road');
    //         assert.equal(data.name, '23RD ST NW');
    //     });

    //     //TDSv61
    //     it('should translate OSMtoRaw', function() {
    //         var data = server.translate({
    //             tags: {
    //                 'error:circular': '5',
    //                 'highway': 'road',
    //                 'hoot:status': '1',
    //                 'name': '23RD ST NW',
    //                 'hoot': 'DcGisRoadsCucumber'
    //             },
    //             to: 'TDSv61',
    //             geom: 'Line'
    //         });
    //         assert.equal(data[0].attrs.NAM, '23RD ST NW');
    //         assert.equal(data[0].attrs.FCODE, 'AP030');
    //         assert.equal(data[0].attrs.TXT, '<OSM>{"highway":"road"}</OSM>');
    //     });

    //     it('should translate OSMtoEnglish', function() {
    //         var data = server.translate({
    //             tags: {
    //                 'error:circular': '5',
    //                 'highway': 'road',
    //                 'hoot:status': '1',
    //                 'name': '23RD ST NW',
    //                 'hoot': 'DcGisRoadsCucumber'
    //             },
    //             to: 'TDSv61',
    //             geom: 'Line',
    //             english: true
    //         });
    //         assert.equal(data.attrs['Geographic Name Information : Full Name'], '23RD ST NW');
    //         assert.equal(data.attrs['Feature Code'], 'AP030:Road');
    //         assert.equal(data.attrs['Associated Text'], '<OSM>{"highway":"road"}</OSM>');
    //     });

    //     it('should translate EnglishtoOSM', function() {
    //         var data = server.translate({
    //             tags: {
    //                 //'error:circular': '5',
    //                 'Feature Code': 'AP030:Road',
    //                 //'hoot:status': '1',
    //                 'Geographic Name Information : Full Name': '23RD ST NW'
    //                 //'hoot': 'DcGisRoadsCucumber'
    //             },
    //             from: 'TDSv61',
    //             geom: 'Line',
    //             english: true
    //         });
    //         assert.equal(data.attrs.highway, 'road');
    //         assert.equal(data.attrs.name, '23RD ST NW');
    //     });

    //     it('should translate RawtoOSM', function() {
    //         var data = server.translate({
    //             tags: {
    //                 'error:circular': '5',
    //                 'FCODE': 'AP030',
    //                 'hoot:status': '1',
    //                 'ZI005_FNA': '23RD ST NW',
    //                 'hoot': 'DcGisRoadsCucumber'
    //             },
    //             from: 'TDSv61',
    //             geom: 'Line'
    //         });
    //         assert.equal(data.highway, 'road');
    //         assert.equal(data.name, '23RD ST NW');
    //     });

    // });

    describe('searchSchema', function() {

        var defaults = {};
        // Updated to reflect returning all geometries when none specified
        var defaultsResult = {
                                "name": "AERATION_BASIN_S",
                                "fcode": "AB040",
                                "desc": "Aeration Basin",
                                "geom": "Area"
                            };

        var MgcpPointBui = {
                                translation: 'MGCP',
                                geomType: 'point',
                                searchStr: 'Bui'
                            };
        var MgcpResult = [{
                                name: "PAL015",
                                fcode: "AL015",
                                desc: "General Building",
                                geom: "Point",
                                idx: -1
                            },
                            {
                                name: "PAL020",
                                fcode: "AL020",
                                desc: "Built-Up Area",
                                geom: "Point",
                                idx: -1
                            }];

        it('should search for default options', function(){
            assert.equal(JSON.stringify(server.searchSchema(defaults)[0]), JSON.stringify(defaultsResult));
        });

        it('should search for "Bui" point feature types in the MGCP schema', function(){
            assert.equal(JSON.stringify(server.searchSchema(MgcpPointBui).slice(0,2)), JSON.stringify(MgcpResult));
        });
    });

    describe('handleInputs', function() {

        it('should handle osmtotds GET', function() {
            //http://localhost:8094/osmtotds?idval=AL015&geom=Point&translation=MGCP&idelem=fcode
            var schema = server.handleInputs({
                idval: 'AL015',
                geom: 'Point',
                translation: 'MGCP',
                idelem: 'fcode',
                method: 'GET',
                path: '/osmtotds'
            });
            assert.equal(schema.desc, 'General Building');
            assert.equal(schema.columns[0].name, 'ACC');
            assert.equal(schema.columns[0].enumerations[0].name, 'Accurate');
            assert.equal(schema.columns[0].enumerations[0].value, '1');
        });

        it('should handle no matches osmtotds GET for MGCP', function() {
            assert.throws(function error() {
                server.handleInputs({
                    idval: 'FB123',
                    geom: 'Area',
                    translation: 'TDSv61',
                    idelem: 'fcode',
                    method: 'GET',
                    path: '/osmtotds'
                })
            }, Error, 'TDSv61 for Area with fcode=FB123 not found');
        });

        it('should handle tdstoosm GET for TDSv61', function() {
            //http://localhost:8094/tdstoosm?fcode=AL013&translation=TDSv61
            var attrs = server.handleInputs({
                fcode: 'AL013',
                translation: 'TDSv61',
                method: 'GET',
                path: '/tdstoosm'
            }).attrs;
            assert.equal(attrs.building, 'yes');
        });

        it('should handle tdstoosm GET for TDSv40', function() {
            //http://localhost:8094/tdstoosm?fcode=AL013&translation=TDSv61
            var attrs = server.handleInputs({
                fcode: 'AP030',
                translation: 'TDSv40',
                method: 'GET',
                path: '/tdstoosm'
            }).attrs;
            assert.equal(attrs.highway, 'road');
        });

        it('should handle tdstoosm GET for MGCP', function() {
            //http://localhost:8094/tdstoosm?fcode=AL013&translation=TDSv61
            var attrs = server.handleInputs({
                fcode: 'BH140',
                translation: 'MGCP',
                method: 'GET',
                path: '/tdstoosm'
            }).attrs;
            assert.equal(attrs.waterway, 'river');
        });

        it('should handle invalid F_CODE in tdstoosm GET for MGCP', function() {
            var attrs = server.handleInputs({
                fcode: 'ZZTOP',
                translation: 'MGCP',
                method: 'GET',
                path: '/tdstoosm'
            }).attrs;
            assert.equal(attrs.error, 'Feature Code ZZTOP is not valid for MGCP');
        });

        it('should handle osmtotds POST', function() {
            //http://localhost:8094/osmtotds
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="JOSM"><node id="-1" lon="-105.21811763904256" lat="39.35643172777992" version="0"><tag k="building" v="yes"/><tag k="uuid" v="{bfd3f222-8e04-4ddc-b201-476099761302}"/></node></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/osmtotds'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.node[0].tag[0].$.k, "Feature Code");
                assert.equal(result.osm.node[0].tag[0].$.v, "AL013:Building");
                assert.equal(result.osm.node[0].tag[1].$.k, "Unique Entity Identifier");
                assert.equal(result.osm.node[0].tag[1].$.v, "bfd3f222-8e04-4ddc-b201-476099761302");
            });
        });

        it('should handle osmtotds POST of building area feature', function() {
            //http://localhost:8094/osmtotds
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="building" v="yes"/><tag k="uuid" v="{d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1}"/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/osmtotds'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "Feature Code");
                assert.equal(result.osm.way[0].tag[0].$.v, "AL015:General Building");
                assert.equal(result.osm.way[0].tag[1].$.k, "MGCP Feature universally unique identifier");
                assert.equal(result.osm.way[0].tag[1].$.v, "d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1");
            });
        });

        it('should handle osmtotds POST of road line feature with width', function() {
            //http://localhost:8094/osmtotds
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-8" version="0"><nd ref="-21"/><nd ref="-24"/><nd ref="-27"/><tag k="highway" v="road"/><tag k="uuid" v="{8cd72087-a7a2-43a9-8dfb-7836f2ffea13}"/><tag k="width" v="20"/><tag k="lanes" v="2"/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/osmtotds'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "Feature Code");
                assert.equal(result.osm.way[0].tag[0].$.v, "AP030:Road");
                assert.equal(result.osm.way[0].tag[1].$.k, "MGCP Feature universally unique identifier");
                assert.equal(result.osm.way[0].tag[1].$.v, "8cd72087-a7a2-43a9-8dfb-7836f2ffea13");
                assert.equal(result.osm.way[0].tag[2].$.k, "Thoroughfare Class");
                assert.equal(result.osm.way[0].tag[2].$.v, "Unknown");
                assert.equal(result.osm.way[0].tag[3].$.k, "Route Minimum Travelled Way Width");
                assert.equal(result.osm.way[0].tag[3].$.v, "20");
                assert.equal(result.osm.way[0].tag[4].$.k, "Track or Lane Count");
                assert.equal(result.osm.way[0].tag[4].$.v, "2");
            });
        });

        // it('should return error message for invalide F_CODE/geom combination in osmtotds POST', function() {
        //     var osm2trans = server.handleInputs({
        //         osm: '<osm version="0.6" upload="true" generator="hootenanny"><node id="72" lon="-104.878690508945" lat="38.8618557942463" version="1"><tag k="poi" v="yes"/><tag k="hoot:status" v="1"/><tag k="name" v="Garden of the Gods"/><tag k="leisure" v="park"/><tag k="error:circular" v="1000"/><tag k="hoot" v="AllDataTypesACucumber"/></node></osm>',
        //         method: 'POST',
        //         translation: 'MGCP',
        //         path: '/osmtotds'
        //     });
        //     //console.log(osm2trans);
        //     assert.equal(attrs.error, 'Point geometry is not valid for AK120 in MGCP TRD4');
        // });

        it('should handle bad translation schema value in osmtotds POST', function() {
            assert.throws(function error() {
                var osm2trans = server.handleInputs({
                    osm: '<osm version="0.6" upload="true" generator="JOSM"><node id="-1" lon="-105.21811763904256" lat="39.35643172777992" version="0"><tag k="building" v="yes"/><tag k="uuid" v="{bfd3f222-8e04-4ddc-b201-476099761302}"/></node></osm>',
                    method: 'POST',
                    translation: 'TDv61',
                    path: '/osmtotds'
                });
            }, Error, 'Unsupported translation schema');
        });

        // it('should throw an error for unable to translate in osmtotds POST', function(){
        //     //http://localhost:8094/osmtotds
        //     var osm2trans = server.handleInputs({
        //         command: 'translate',
        //         input: '<osm version="0.6" upload="true" generator="JOSM"><node id="-1" lon="-105.21811763904256" lat="39.35643172777992" version="0"><tag k="poi" v="yes"/><tag k="leisure" v="park"/><tag k="name" v="Garden of the Gods"/><tag k="uuid" v="{bfd3f222-8e04-4ddc-b201-476099761302}"/></node></osm>',
        //         translation: 'MGCP',
        //         method: 'POST',
        //         path: '/osmtotds'
        //     });
        //     var output = xml2js.parseString(osm2trans.output, function(err, result) {
        //         if (err) console.error(err);
        //         assert.equal(result.osm.node[0].tag[0].$.k, "Feature Code");
        //         assert.equal(result.osm.node[0].tag[0].$.v, "AL013:Building");
        //         assert.equal(result.osm.node[0].tag[1].$.k, "Unique Entity Identifier");
        //         assert.equal(result.osm.node[0].tag[1].$.v, "bfd3f222-8e04-4ddc-b201-476099761302");
        //     });
        // });

        it('should handle tdstoosm POST', function() {
            //http://localhost:8094/tdstoosm
            var trans2osm = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="JOSM"><node id="-9" lon="-104.907037158172" lat="38.8571566428667" version="0"><tag k="Horizontal Accuracy Category" v="Accurate"/><tag k="Built-up Area Density Category" ve="Unknown"/><tag k="Commercial Copyright Notice" v="UNK"/><tag k="Feature Code" v="AL020:Built-Up Area"/><tag k="Functional Use" v="Other"/><tag k="Condition of Facility" v="Unknown"/><tag k="Name" v="Manitou Springs"/><tag k="Named Feature Identifier" v="UNK"/><tag k="Name Identifier" v="UNK"/><tag k="Relative Importance" v="Unknown"/><tag k="Source Description" v="N_A"/><tag k="Source Date and Time" v="UNK"/><tag k="Source Type" v="Unknown"/><tag k="Associated Text" v="&lt;OSM&gt;{&quot;poi&quot;:&quot;yes&quot;}&lt;/OSM&gt;"/><tag k="MGCP Feature universally unique identifier" v="c6df0618-ce96-483c-8d6a-afa33541646c"/></node></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/tdstoosm'
            });
            var output = xml2js.parseString(trans2osm, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.node[0].tag[0].$.k, "use");
                assert.equal(result.osm.node[0].tag[0].$.v, "other");
                assert.equal(result.osm.node[0].tag[1].$.k, "place");
                assert.equal(result.osm.node[0].tag[1].$.v, "yes");
                assert.equal(result.osm.node[0].tag[2].$.k, "poi");
                assert.equal(result.osm.node[0].tag[2].$.v, "yes");
                assert.equal(result.osm.node[0].tag[3].$.k, "uuid");
                assert.equal(result.osm.node[0].tag[3].$.v, "{c6df0618-ce96-483c-8d6a-afa33541646c}");
                assert.equal(result.osm.node[0].tag[4].$.k, "name");
                assert.equal(result.osm.node[0].tag[4].$.v, "Manitou Springs");
                assert.equal(result.osm.node[0].tag[5].$.k, "landuse");
                assert.equal(result.osm.node[0].tag[5].$.v, "built_up_area");
                assert.equal(result.osm.node[0].tag[6].$.k, "source:accuracy:horizontal:category");
                assert.equal(result.osm.node[0].tag[6].$.v, "accurate");
            });
        });

        it('should handle /taginfo/key/values GET with NO enums', function() {
            //http://localhost:8094/taginfo/key/values?fcode=AP030&filter=ways&key=SGCC&page=1&query=Clo&rp=25&sortname=count_ways&sortorder=desc&translation=TDSv61
//http://localhost:8094/taginfo/key/values?fcode=AA040&filter=nodes&key=ZSAX_RX3&page=1&query=undefined&rp=25&sortname=count_nodes&sortorder=desc&translation=TDSv61
            var enums = server.handleInputs({
                fcode: 'AA040',
                filter: 'ways',
                key: 'ZSAX_RX3',
                page: '1',
                query: 'undefined',
                rp: '25',
                sortname: 'count_nodes',
                sortorder: 'desc',
                translation: 'TDSv61',
                method: 'GET',
                path: '/taginfo/key/values'
            });
            assert.equal(enums.data.length, 0);
        });

        it('should handle /taginfo/key/values GET with enums', function() {
//http://localhost:8094/taginfo/key/values?fcode=AA040&filter=nodes&key=FUN&page=1&query=Damaged&rp=25&sortname=count_nodes&sortorder=desc&translation=MGCP
            var enums = server.handleInputs({
                fcode: 'AA040',
                filter: 'nodes',
                key: 'FUN',
                page: '1',
                query: 'Damaged',
                rp: '25',
                sortname: 'count_nodes',
                sortorder: 'desc',
                translation: 'MGCP',
                method: 'GET',
                path: '/taginfo/key/values'
            });
            assert.equal(enums.data.length, 7);
        });

        it('should handle /taginfo/keys/all GET with enums', function() {
//http://localhost:8094/taginfo/keys/all?fcode=AA040&page=1&query=&rawgeom=Point&rp=10&sortname=count_nodes&sortorder=desc&translation=MGCP

            var enums = server.handleInputs({
                fcode: 'AA040',
                rawgeom: 'Point',
                key: 'FUN',
                page: '1',
                rp: '10',
                sortname: 'count_nodes',
                sortorder: 'desc',
                translation: 'MGCP',
                method: 'GET',
                path: '/taginfo/keys/all'
            });
            assert.equal(enums.data.length, 15);
        });

        it('should handle /taginfo/keys/all GET with enums', function() {

            var enums = server.handleInputs({
                fcode: 'EC030',
                rawgeom: 'Area',
                key: 'FUN',
                page: '1',
                rp: '10',
                sortname: 'count_ways',
                sortorder: 'desc',
                translation: 'MGCP',
                method: 'GET',
                path: '/taginfo/keys/all'
            });
            assert.equal(enums.data.length, 14);
        });

        it('should handle /capabilities GET', function() {

            var capas = server.handleInputs({
                method: 'GET',
                path: '/capabilities'
            });
            assert.equal(capas.TDSv61.isavailable, true);
            assert.equal(capas.TDSv40.isavailable, true);
            assert.equal(capas.MGCP.isavailable, true);
        });

        it('should handle /schema GET', function() {
//http://localhost:8094/schema?geometry=point&translation=MGCP&searchstr=Buil&maxlevdst=20&limit=12
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'TDSv40',
                searchstr: 'river',
                maxlevdst: 20,
                limit: 12,
                method: 'GET',
                path: '/schema'
            });
            assert.equal(schm[0].name, 'RIVER_C');
            assert.equal(schm[0].fcode, 'BH140');
            assert.equal(schm[0].desc, 'River');
        });

//Checking the use of limit param
        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'TDSv40',
                searchstr: 'river',
                maxlevdst: 10,
                limit: 1,
                method: 'GET',
                path: '/schema'
            });
            assert.equal(schm.length, 1);
        });

        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'TDSv40',
                searchstr: 'ri',
                maxlevdst: 50,
                limit: 33,
                method: 'GET',
                path: '/schema'
            });
            assert.equal(schm.length, 33);
        });

        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'TDSv40',
                searchstr: 'ri',
                maxlevdst: 50,
                limit: 100,
                method: 'GET',
                path: '/schema'
            });
            assert(schm.length <= 100, 'Schema search results greater than requested');
        });

//Checking the use of limit param with no search string
        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'TDSv40',
                searchstr: '',
                maxlevdst: 10,
                limit: 1,
                method: 'GET',
                path: '/schema'
            });
            assert.equal(schm.length, 1);
        });

        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'point',
                translation: 'TDSv61',
                searchstr: '',
                limit: 33,
                method: 'GET',
                path: '/schema'
            });
            assert.equal(schm.length, 33);
        });

        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'MGCP',
                searchstr: '',
                maxlevdst: 0, //This shouldn't be used when searchstr is ''
                limit: 100,
                method: 'GET',
                path: '/schema'
            });
            assert(schm.length <= 100, 'Schema search results greater than requested');
            assert(schm.some(function(d) {
                return d.desc === 'Railway';
            }));
        });

        it('throws error if url not found', function() {
            assert.throws(function error() {
                server.handleInputs({
                    idval: 'AL015',
                    geom: 'Point',
                    translation: 'TDSv40',
                    idelem: 'fcode',
                    method: 'GET',
                    path: '/foo'
                })
            }, Error, 'Not found');
        });

        it('throws error if unsupported method', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'POST',
                    path: '/schema'
                })
            }, Error, 'Unsupported method');
        });

        it('throws error if unsupported method', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'POST',
                    path: '/taginfo/key/values'
                })
            }, Error, 'Unsupported method');
        });

        it('throws error if unsupported method', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'POST',
                    path: '/taginfo/keys/all'
                })
            }, Error, 'Unsupported method');
        });

        it('throws error if unsupported method', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'POST',
                    path: '/capabilities'
                })
            }, Error, 'Unsupported method');
        });

    });

    describe('capabilities', function () {
      it('should return 200', function (done) {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/capabilities'
        });
        var response = httpMocks.createResponse();
        server.TranslationServer(request, response);
        assert.equal(response.statusCode, '200');
        done();
      });
    });

    describe('schema', function () {
      it('should return 200', function (done) {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/schema?geometry=point&translation=MGCP&earchstr=Buil&maxlevdst=20&limit=12'
        });
        var response = httpMocks.createResponse();
        server.TranslationServer(request, response);
        assert.equal(response.statusCode, '200');
        done();
      });
    });

    describe('osmtotds', function () {
      it('should return 200', function (done) {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/osmtotds?idval=AP030&translation=MGCP&geom=Line&idelem=fcode'
        });
        var response = httpMocks.createResponse();
        server.TranslationServer(request, response);
        assert.equal(response.statusCode, '200');
        done();
      });
    });

    // describe('tdstoosm', function () {
    //   it('should return 200', function (done) {
    //     var request  = httpMocks.createRequest({
    //         method: 'GET',
    //         url: '/tdstoosm',
    //         query: {
    //             fcode: 'AL013',
    //             translation: 'TDSv61'
    //         }
    //     });
    //     var response = httpMocks.createResponse();
    //     server.TranslationServer(request, response);
    //     assert.equal(response.statusCode, '200');
    //   });
    // });

    // describe('taginfo/key/values', function () {
    //   it('should return 200', function (done) {
    //     var request  = httpMocks.createRequest({
    //         method: 'GET',
    //         url: '/taginfo/key/values',
    //         query: {
    //             fcode: 'AP030',
    //             translation: 'TDSv61',
    //             filter: 'ways',
    //             key: 'SGCC',
    //             page: 1,
    //             key: 'SGCC',
    //             query: 'Clo',
    //             rp: 25,
    //             sortname: 'count_ways',
    //             sortorder: 'desc'
    //         }
    //     });
    //     var response = httpMocks.createResponse();
    //     server.TranslationServer(request, response);
    //     assert.equal(response.statusCode, '200');
    //   });
    // });

    // describe('taginfo/keys/all', function () {
    //   it('should return 200', function (done) {
    //     http.get('http://localhost:8094/taginfo/keys/all?page=1&rp=10&sortname=count_ways&sortorder=desc&fcode=AP030&translation=TDSv61&geometry=Line', function (res) {
    //       assert.equal(200, res.statusCode);
    //       done();
    //     });
    //   });
    // });

    // describe('not found', function () {
    //   it('should return 404', function (done) {
    //     http.get('http://localhost:8094/foo', function (res) {
    //         console.log(res.statusCode);
    //       assert.equal(404, res.statusCode);
    //       done();
    //     });
    //   });
    // });

});

