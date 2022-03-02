
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

describe('Untangle', function () {
    it('should untangle MGCP tags', function() {
        var trans2osm = server.handleInputs({
            osm: '<osm version="0.6" upload="true" generator="hootenanny">\
                  <way id="-1" version="0">\
                  <nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/>\
                  <tag k="FCODE" v="AL013"/><tag k="levels" v="3"/>\
                  </way></osm>',
            method: 'POST',
            translation: 'MGCP',
            path: '/translateFrom'
        });
        xml2js.parseString(trans2osm, function(err, result) {
            if (err) console.error(err);
            assert.equal(result.osm.way[0].tag[0].$.k, "levels");
            assert.equal(result.osm.way[0].tag[0].$.v, "3");
            assert.equal(result.osm.way[0].tag[1].$.k, "building");
            assert.equal(result.osm.way[0].tag[1].$.v, "yes");
        });
    });

    it('should untangle TDSv61 tags', function() {
        var trans2osm = server.handleInputs({
            osm: '<osm version="0.6" upload="true" generator="hootenanny">\
                  <way id="-1" version="0">\
                  <nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/>\
                  <tag k="AL013" v="building"/><tag k="levels" v="3"/>\
                  </way></osm>',
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateFrom'
        });
        xml2js.parseString(trans2osm, function(err, result) {
            if (err) console.error(err);
            assert.equal(result.osm.way[0].tag[0].$.k, "levels");
            assert.equal(result.osm.way[0].tag[0].$.v, "3");
            assert.equal(result.osm.way[0].tag[1].$.k, "building");
            assert.equal(result.osm.way[0].tag[1].$.v, "yes");
        });
    });

    it('should untangle TDSv40 tags', function() {
        var trans2osm = server.handleInputs({
            osm: '<osm version="0.6" upload="true" generator="hootenanny">\
                  <way id="-1" version="0">\
                  <nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/>\
                  <tag k="AL013" v="building"/><tag k="levels" v="3"/>\
                  </way></osm>',
            method: 'POST',
            translation: 'TDSv40',
            path: '/translateFrom'
        });
        xml2js.parseString(trans2osm, function(err, result) {
            if (err) console.error(err);
            assert.equal(result.osm.way[0].tag[0].$.k, "levels");
            assert.equal(result.osm.way[0].tag[0].$.v, "3");
            assert.equal(result.osm.way[0].tag[1].$.k, "building");
            assert.equal(result.osm.way[0].tag[1].$.v, "yes");
        });
    });


});
