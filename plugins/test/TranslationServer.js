var assert = require('assert'),
    http = require('http');
var server  = require('../TranslationServer.js');

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
    describe('server', function () {

        describe('searchSchema', function(){

            it('should search for default options', function(){
                assert.equal(JSON.stringify(server.searchSchema(defaults)[0]), JSON.stringify(defaultsResult));
            });

            it('should search for "Bui" point feature types in the MGCP schema', function(){
                assert.equal(JSON.stringify(server.searchSchema(MgcpPointBui).slice(0,2)), JSON.stringify(MgcpResult));
            });
        });

        describe('handleInputs', function(){

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
            it('should handle osmtotds GET', function(){
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

            function notFound() {
                server.handleInputs({
                    idval: 'AL015',
                    geom: 'Point',
                    translation: 'MGCP',
                    idelem: 'fcode',
                    path: '/foo'
                })
            }

            it('throws error if not found', function(){
                assert.throws(notFound, Error, 'Not found');
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
