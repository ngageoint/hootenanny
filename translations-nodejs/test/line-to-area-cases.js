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

    describe('shear wall (AA011) OSM to OGR translations', function() {
        it('should handle OSM to TDSv40', function() {
            var trans2tds40 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'><way id='-39158' visible='true'><nd ref='-39156' /><nd ref='-39157' /><tag k='natural' v='shear_wall' /></way></osm>",
                method: 'POST',
                translation: 'TDSv40',
                path: '/translateTo'
            });

            xml2js.parseString(trans2tds40, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AA011");
            });
        });
        it('should handle OSM to TDSv61', function() {
            var trans2tds61 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'><way id='-39158' visible='true'><nd ref='-39156' /><nd ref='-39157' /><tag k='natural' v='shear_wall' /></way></osm>",
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            xml2js.parseString(trans2tds61, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AA011");
            });
        });
        it('should not find OSM to MGCP translation', function() {
            var trans2mgcp = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'><way id='-39158' visible='true'><nd ref='-39156' /><nd ref='-39157' /><tag k='natural' v='shear_wall' /></way></osm>",
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });

            xml2js.parseString(trans2mgcp, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "error");
                assert.equal(result.osm.way[0].tag[0].$.v, "Line geometry is not valid for AA011 in MGCP TRD4");
            });
        });
        it('should handle OSM to GGDMv30', function() {
            var trans2ggdmv30 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'><way id='-39158' visible='true'><nd ref='-39156' /><nd ref='-39157' /><tag k='natural' v='shear_wall' /></way></osm>",
                method: 'POST',
                translation: 'GGDMv30',
                path: '/translateTo'
            });

            xml2js.parseString(trans2ggdmv30, function(err, result) {
              if (err) console.log(err);
              assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
              assert.equal(result.osm.way[0].tag[0].$.v, "AA011")
            })
        })
    });

    describe('excavating machine (AF050) OSM to OGR translation', function() {
        it('should handle OSM to TDSv40', function() {
            var trans2tds40 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'> <way id='-39012' visible='true'><nd ref='-39010' /><nd ref='-39011' /><tag k='ele:max' v='324' /><tag k='man_made' v='excavating_machine' /><tag k='uuid' v='{9c1b7b24-c3c9-4ef1-aaf0-1d279f4f232d}' /></way></osm>",
                method: 'POST',
                translation: 'TDSv40',
                path: '/translateTo'
            });

            xml2js.parseString(trans2tds40, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AF050");
            });
        });

        it('should handle OSM to TDSv61', function() {
            var trans2tds61 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'> <way id='-39012' visible='true'><nd ref='-39010' /><nd ref='-39011' /><tag k='ele:max' v='324' /><tag k='man_made' v='excavating_machine' /><tag k='uuid' v='{9c1b7b24-c3c9-4ef1-aaf0-1d279f4f232d}' /></way></osm>",
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            xml2js.parseString(trans2tds61, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AF050");
            });
        });
        it('should handle OSM to MGCP', function() {
            var trans2mgcp = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'> <way id='-39012' visible='true'><nd ref='-39010' /><nd ref='-39011' /><tag k='ele:max' v='324' /><tag k='man_made' v='excavating_machine' /><tag k='uuid' v='{9c1b7b24-c3c9-4ef1-aaf0-1d279f4f232d}' /></way></osm>",
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });

            xml2js.parseString(trans2mgcp, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "error");
                assert.equal(result.osm.way[0].tag[0].$.v, "Line geometry is not valid for AF050 in MGCP TRD4");
            });
        });
        it('should handle OSM to GGDMv30', function() {
            var trans2ggdmv30 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'> <way id='-39012' visible='true'><nd ref='-39010' /><nd ref='-39011' /><tag k='ele:max' v='324' /><tag k='man_made' v='excavating_machine' /><tag k='uuid' v='{9c1b7b24-c3c9-4ef1-aaf0-1d279f4f232d}' /></way></osm>",
                method: 'POST',
                translation: 'GGDMv30',
                path: '/translateTo'
            });

            xml2js.parseString(trans2ggdmv30, function(err, result) {
              if (err) console.log(err);
              assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
              assert.equal(result.osm.way[0].tag[0].$.v, "AF050")
            })
        })
    })
    describe('watercourse_crossing (AQ111) OSM to OGR translation', function() {
        it('should handle OSM to TDSv40', function() {
            var trans2tds40 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'><way id='-39104' visible='true'><nd ref='-39102' /><nd ref='-39103' /><nd ref='-39105' /><tag k='man_made' v='watercourse_crossing' /><tag k='uuid' v='{10ffeed9-b866-412d-8d77-2378d2147a03}' /></way></osm>",
                method: 'POST',
                translation: 'TDSv40',
                path: '/translateTo'
            });

            xml2js.parseString(trans2tds40, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AQ111");
            });
        });

        it('should handle OSM to TDSv61', function() {
            var trans2tds61 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'><way id='-39104' visible='true'><nd ref='-39102' /><nd ref='-39103' /><nd ref='-39105' /><tag k='man_made' v='watercourse_crossing' /><tag k='uuid' v='{10ffeed9-b866-412d-8d77-2378d2147a03}' /></way></osm>",
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            xml2js.parseString(trans2tds61, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "error");
                assert.equal(result.osm.way[0].tag[0].$.v, 'Line geometry is not valid for AQ111 in TDSv61');
            });
        });
        it('should handle OSM to MGCP', function() {
            var trans2mgcp = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'><way id='-39104' visible='true'><nd ref='-39102' /><nd ref='-39103' /><nd ref='-39105' /><tag k='man_made' v='watercourse_crossing' /><tag k='uuid' v='{10ffeed9-b866-412d-8d77-2378d2147a03}' /></way></osm>",
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });

            xml2js.parseString(trans2mgcp, function(err, result) {
                if (err) console.log(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "error");
                assert.equal(result.osm.way[0].tag[0].$.v, "Line geometry is not valid for AQ111 in MGCP TRD4");
            });
        });
        it('should handle OSM to GGDMv30', function() {
            var trans2ggdmv30 = server.handleInputs({
                osm: "<osm version='0.6' generator='JOSM'><way id='-39104' visible='true'><nd ref='-39102' /><nd ref='-39103' /><nd ref='-39105' /><tag k='man_made' v='watercourse_crossing' /><tag k='uuid' v='{10ffeed9-b866-412d-8d77-2378d2147a03}' /></way></osm>",
                method: 'POST',
                translation: 'GGDMv30',
                path: '/translateTo'
            });

            xml2js.parseString(trans2ggdmv30, function(err, result) {
              if (err) console.log(err);
              assert.equal(result.osm.way[0].tag[0].$.k, "error");
              assert.equal(result.osm.way[0].tag[0].$.v, "Line geometry is not valid for AQ111 in TDSv61");
            })
        })
    })
    describe('conveyer:type=bucket: (AF021) OSM to OGR translation', function() {

        it('should handle OSM to TDSv40', function() {
            var trans2tds40 = server.handleInputs({
                osm: '<osm version="0.6" generator="hootenanny" srs="+epsg:4326" schema="OSM"><bounds minlat="0" minlon="0" maxlat="0" maxlon="0"/><way visible="true" id="-39098" timestamp="1970-01-01T00:00:00Z" version="1"><tag k="conveyer:type" v="bucket:"/></way></osm>',
                method: 'POST',
                translation: 'TDSv40',
                path: '/translateTo'
            });

            xml2js.parseString(trans2tds40, function(err, result) {
                if (err) console.log(err);
            });
        });
        it('should handle OSM to TDSv61', function() {
            var trans2tds40 = server.handleInputs({
                osm: '<osm version="0.6" generator="hootenanny" srs="+epsg:4326" schema="OSM"><bounds minlat="0" minlon="0" maxlat="0" maxlon="0"/><way visible="true" id="-39098" timestamp="1970-01-01T00:00:00Z" version="1"><tag k="conveyer:type" v="bucket:"/></way></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            xml2js.parseString(trans2tds40, function(err, result) {
                if (err) console.log(err);
            });
        });
        it('should handle OSM to TDSv61', function() {
            var trans2tds40 = server.handleInputs({
                osm: '<osm version="0.6" generator="hootenanny" srs="+epsg:4326" schema="OSM"><bounds minlat="0" minlon="0" maxlat="0" maxlon="0"/><way visible="true" id="-39098" timestamp="1970-01-01T00:00:00Z" version="1"><tag k="conveyer:type" v="bucket:"/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });

            xml2js.parseString(trans2tds40, function(err, result) {
                if (err) console.log(err);
            });
        });
    })
});

