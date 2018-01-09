var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    describe('shear wall (AA011) OSM to ORG translations', function() {
        it('should handle OSM to TDSv40', function() {
            var trans2tds40 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'><way id='-39158' visible='true'><nd ref='-39156' /><nd ref='-39157' /><tag k='natural' v='shear_wall' /></way></osm>",
                method: 'POST',
                translation: 'TDSv40',
                path: '/translateTo'
            });  

            xml2js.parseString(trans2tds40, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[1].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[1].$.v, "AA011");
            });
        });
        // it('should handle OSM to TDSv61', function() {
        //     var trans2tds61 = server.handleInputs({
        //         osm: "<osm version='0.6' generator='JOSM'><way id='-39158' visible='true'><nd ref='-39156' /><nd ref='-39157' /><tag k='natural' v='shear_wall' /></way></osm>",
        //         method: 'POST',
        //         translation: 'TDSv61',
        //         path: '/translateTo'
        //     });

        //     xml2js.parseString(trans2tds61, function(err, result) {
        //         if (err) console.log(err);
        //         assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
        //         assert.equal(result.osm.way[0].tag[0].$.v, "AA011");
        //     });
        // });
        // it('should handle OSM to MGCP', function() {
        //     var trans2mgcp = server.handleInputs({
        //         osm: "<osm version='0.6' generator='JOSM'><way id='-39158' visible='true'><nd ref='-39156' /><nd ref='-39157' /><tag k='natural' v='shear_wall' /></way></osm>",
        //         method: 'POST',
        //         translation: 'TDSv61',
        //         path: '/translateTo'
        //     });

        //     xml2js.parseString(trans2mgcp, function(err, result) {
        //         if (err) console.log(err);
        //         assert.equal(result.osm.way[0].tag[0].$.k, "FCODE");
        //         assert.equal(result.osm.way[0].tag[0].$.v, "AA011");
        //     });
        // });
        // it('should handle OSM to GGDMv30', function() {
        //     var trans2ggdmv30 = server.handleInputs({
        //         osm: "<osm version='0.6' generator='JOSM'><way id='-39158' visible='true'><nd ref='-39156' /><nd ref='-39157' /><tag k='natural' v='shear_wall' /></way></osm>",
        //         method: 'POST',
        //         translation: 'TDSv61',
        //         path: '/translateTo'
        //     });

        //     xml2js.parseString(trans2ggdmv30, function(err, result) {
        //       if (err) console.log(err);
        //       assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
        //       assert.equal(result.osm.way[0].tag[0].$.v, "AA011")
        //     })
        // })
    });
});