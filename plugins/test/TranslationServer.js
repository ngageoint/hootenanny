var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js');
var server = require('../TranslationServer.js');

var defaults = {};
var defaultsResult = {
                        name: "BLAST_FURNACE_P",
                        fcode: "AC010",
                        desc: "Blast-furnace",
                        geom: "Point"
                    };

var MgcpPointBui = {
                        translation: 'MGCP',
                        geomType: 'point',
                        searchStr: 'Bui'
                    };
var MgcpResult = [{
                        name: "PAL015",
                        fcode: "AL015",
                        desc: "General Building Point Feature",
                        geom: "Point",
                        idx: -1
                    },
                    {
                        name: "PAL020",
                        fcode: "AL020",
                        desc: "Built-Up Area Point Feature",
                        geom: "Point",
                        idx: -1
                    }];

if (server.cluster.isMaster) {
    describe('TranslationServer', function () {

        describe('searchSchema', function() {

            it('should search for default options', function(){
                assert.equal(JSON.stringify(server.searchSchema(defaults)[0]), JSON.stringify(defaultsResult));
            });

            it('should search for "Bui" point feature types in the MGCP schema', function(){
                assert.equal(JSON.stringify(server.searchSchema(MgcpPointBui).slice(0,2)), JSON.stringify(MgcpResult));
            });
        });

        describe('handleInputs', function() {

/*
        case '/osmtotds':
            payload.transMap = osmToTdsMap;
            payload.trandDir = 'toogr';
            result = osmtotds(payload);
            break;
        case '/tdstoosm':
            payload.transMap = tdsToOsmMap;
            payload.trandDir = 'toosm';
            result = tdstoosm(payload);
            break;
        case '/taginfo/key/values':
            result = getTaginfoKeyFields(payload);
            break;
        case '/taginfo/keys/all':
            result = getTaginfoKeyFields(payload);
            break;
        case '/schema':
            result = getTaginfoKeys(payload);
            break;
        case '/capabilities':
            result = getCapabilities();
            break;
        default:
            throw new Error('Not found');
            break;
*/
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
                assert.equal(schema.desc, 'General Building Point Feature');
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

            it('should handle no matches tdstoosm GET for MGCP', function() {
                var attrs = server.handleInputs({
                    fcode: 'ZZTOP',
                    translation: 'MGCP',
                    method: 'GET',
                    path: '/tdstoosm'
                }).attrs;
                assert(attrs.uuid.length > 0);
            });

            it('should handle osmtotds POST', function() {
                //http://localhost:8094/osmtotds
                var osm2trans = server.handleInputs({
                    command: 'translate',
                    input: '<osm version="0.6" upload="true" generator="JOSM"><node id="-1" lon="-105.21811763904256" lat="39.35643172777992" version="0"><tag k="building" v="yes"/><tag k="uuid" v="{bfd3f222-8e04-4ddc-b201-476099761302}"/></node></osm>',
                    translation: 'TDSv61',
                    method: 'POST',
                    path: '/osmtotds'
                });
                xml2js.parseString(osm2trans.output, function(err, result) {
                    if (err) console.error(err);
                    assert.equal(result.osm.node[0].tag[0].$.k, "Feature Code");
                    assert.equal(result.osm.node[0].tag[0].$.v, "AL013:Building");
                    assert.equal(result.osm.node[0].tag[1].$.k, "Unique Entity Identifier");
                    assert.equal(result.osm.node[0].tag[1].$.v, "bfd3f222-8e04-4ddc-b201-476099761302");
                });
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
                    command: 'translate',
                    input: '<osm version="0.6" upload="true" generator="JOSM"><node id="-9" lon="-104.907037158172" lat="38.8571566428667" version="0"><tag k="Horizontal Accuracy Category" v="Accurate"/><tag k="Built-up Area Density Category" v="Unknown"/><tag k="Commercial Copyright Notice" v="UNK"/><tag k="Feature Code" v="AL020:Built-Up Area Area Feature"/><tag k="Functional Use" v="Other"/><tag k="Condition of Facility" v="Unknown"/><tag k="Name" v="Manitou Springs"/><tag k="Named Feature Identifier" v="UNK"/><tag k="Name Identifier" v="UNK"/><tag k="Relative Importance" v="Unknown"/><tag k="Source Description" v="N_A"/><tag k="Source Date and Time" v="UNK"/><tag k="Source Type" v="Unknown"/><tag k="Associated Text" v="&lt;OSM&gt;{&quot;poi&quot;:&quot;yes&quot;}&lt;/OSM&gt;"/><tag k="MGCP Feature universally unique identifier" v="c6df0618-ce96-483c-8d6a-afa33541646c"/></node></osm>',
                    translation: 'MGCP',
                    method: 'POST',
                    path: '/tdstoosm'
                });
                var output = xml2js.parseString(trans2osm.output, function(err, result) {
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

            it('throws error if unrecognized command', function() {
                assert.throws(function error() {
                    server.handleInputs({
                    command: 'exacerbate',
                    input: '<osm version="0.6" upload="true" generator="JOSM"><node id="-1" lon="-105.21811763904256" lat="39.35643172777992" version="0"><tag k="building" v="yes"/><tag k="uuid" v="{bfd3f222-8e04-4ddc-b201-476099761302}"/></node></osm>',
                    translation: 'TDSv61',
                    method: 'POST',
                    path: '/osmtotds'
                    })
                }, Error, 'Unrecognized command');
            });

        });

        describe('capabilities', function () {
          it('should return 200', function (done) {
            http.get('http://localhost:8094/capabilities', function (res) {
              assert.equal(200, res.statusCode);
              done();
            });
          });
        });

        describe('schema', function () {
          it('should return 200', function (done) {
            http.get('http://localhost:8094/schema?geometry=point&translation=MGCP&searchstr=Buil&maxlevdst=20&limit=12', function (res) {
              assert.equal(200, res.statusCode);
              done();
            });
          });
        });

        describe('osmtotds', function () {
          it('should return 200', function (done) {
            http.get('http://localhost:8094/osmtotds?idval=AP030&geom=Line&translation=TDSv61&idelem=fcode', function (res) {
              assert.equal(200, res.statusCode);
              done();
            });
          });
        });

        describe('tdstoosm', function () {
          it('should return 200', function (done) {
            http.get('http://localhost:8094/tdstoosm?fcode=AL013&translation=TDSv61', function (res) {
              assert.equal(200, res.statusCode);
              done();
            });
          });
        });

        describe('taginfo/key/values', function () {
          it('should return 200', function (done) {
            http.get('http://localhost:8094/taginfo/key/values?fcode=AP030&filter=ways&key=SGCC&page=1&query=Clo&rp=25&sortname=count_ways&sortorder=desc&translation=TDSv61', function (res) {
              assert.equal(200, res.statusCode);
              done();
            });
          });
        });

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

        after(function (done) {
            server.cluster.disconnect(done);
        });
    });

}
