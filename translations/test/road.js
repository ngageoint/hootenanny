
// See docs/developer/ElementTranslationService.asciidoc

var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser
    parser = new DOMParser(),
    hashseedzero = true;

var server = require('../TranslationServer.js');

describe('Roads', function() {
    it('should handle OSM to MGCP POST of road line feature with width', function() {
        var osm2trans = server.handleInputs({
            osm: '<osm version="0.6" upload="true" generator="hootenanny">\
                  <way id="-8" version="0">\
                  <nd ref="-21"/><nd ref="-24"/><nd ref="-27"/>\
                  <tag k="highway" v="road"/><tag k="uuid" v="{8cd72087-a7a2-43a9-8dfb-7836f2ffea13}"/>\
                  <tag k="width" v="20"/><tag k="lanes" v="2"/>\
                  </way></osm>',
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        xml2js.parseString(osm2trans, function(err, result) {
            if (err) console.error(err);
            assert.equal(result.osm.$.schema, "MGCP");
            assert.equal(result.osm.way[0].tag[0].$.k, "FCODE");
            assert.equal(result.osm.way[0].tag[0].$.v, "AP030");
            assert.equal(result.osm.way[0].tag[1].$.k, "LTN");
            assert.equal(result.osm.way[0].tag[1].$.v, "2");
            assert.equal(result.osm.way[0].tag[2].$.k, "UID");
            assert.equal(result.osm.way[0].tag[2].$.v, "8CD72087-A7A2-43A9-8DFB-7836F2FFEA13");
            assert.equal(result.osm.way[0].tag[4].$.k, "WD1");
            assert.equal(result.osm.way[0].tag[4].$.v, "20");
        });
    });

    it('should handle OSM to TDSv71 POST of road line feature with width', function() {
        var osm2trans = server.handleInputs({
            osm: '<osm version="0.6" upload="true" generator="hootenanny">\
                  <way id="-8" version="0">\
                  <nd ref="-21"/><nd ref="-24"/><nd ref="-27"/>\
                  <tag k="highway" v="road"/><tag k="uuid" v="{8cd72087-a7a2-43a9-8dfb-7836f2ffea13}"/>\
                  <tag k="width" v="20"/><tag k="lanes" v="2"/>\
                  </way></osm>',
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });
        xml2js.parseString(osm2trans, function(err, result) {
            if (err) console.error(err);
            assert.equal(result.osm.$.schema, "TDSv71");
            assert.equal(result.osm.way[0].tag[0].$.k, "LTN");
            assert.equal(result.osm.way[0].tag[0].$.v, "2");
            assert.equal(result.osm.way[0].tag[1].$.k, "ZI016_WD1");
            assert.equal(result.osm.way[0].tag[1].$.v, "20");
            assert.equal(result.osm.way[0].tag[2].$.k, "RIN_ROI");
            assert.equal(result.osm.way[0].tag[2].$.v, "5");
            assert.equal(result.osm.way[0].tag[3].$.k, "F_CODE");
            assert.equal(result.osm.way[0].tag[3].$.v, "AP030");
            assert.equal(result.osm.way[0].tag[4].$.k, "OSMTAGS");
            assert.equal(result.osm.way[0].tag[4].$.v, '{"highway":"road"}');
            assert.equal(result.osm.way[0].tag[5].$.k, "RTY");
            assert.equal(result.osm.way[0].tag[5].$.v, "-999999");
            assert.equal(result.osm.way[0].tag[6].$.k, "UFI");
            assert.equal(result.osm.way[0].tag[6].$.v, "8cd72087-a7a2-43a9-8dfb-7836f2ffea13");
        });
    });

   it('should handle OSM to GGDMv30 POST of road line feature with width', function() {
        var osm2trans = server.handleInputs({
            osm: '<osm version="0.6" upload="true" generator="hootenanny">\
                  <way id="-8" version="0">\
                  <nd ref="-21"/><nd ref="-24"/><nd ref="-27"/>\
                  <tag k="highway" v="road"/><tag k="uuid" v="{8cd72087-a7a2-43a9-8dfb-7836f2ffea13}"/>\
                  <tag k="width" v="20"/><tag k="lanes" v="2"/>\
                  </way></osm>',
            method: 'POST',
            translation: 'GGDMv30',
            path: '/translateTo'
        });
        xml2js.parseString(osm2trans, function(err, result) {
            if (err) console.error(err);

            assert.equal(result.osm.$.schema, "GGDMv30");
            assert.equal(result.osm.way[0].tag[0].$.k, "LTN");
            assert.equal(result.osm.way[0].tag[0].$.v, "2");
            assert.equal(result.osm.way[0].tag[1].$.k, "ZI016_WD1");
            assert.equal(result.osm.way[0].tag[1].$.v, "20");
            assert.equal(result.osm.way[0].tag[2].$.k, "RIN_ROI");
            assert.equal(result.osm.way[0].tag[2].$.v, "5");
            assert.equal(result.osm.way[0].tag[3].$.k, "F_CODE");
            assert.equal(result.osm.way[0].tag[3].$.v, "AP030");
            assert.equal(result.osm.way[0].tag[4].$.k, "OSMTAGS");
            assert.equal(result.osm.way[0].tag[4].$.v, '{"highway":"road"}');
            assert.equal(result.osm.way[0].tag[5].$.k, "RTY");
            assert.equal(result.osm.way[0].tag[5].$.v, "-999999");
            assert.equal(result.osm.way[0].tag[6].$.k, "UFI");
            assert.equal(result.osm.way[0].tag[6].$.v, "8cd72087-a7a2-43a9-8dfb-7836f2ffea13");
        });
    });

});
