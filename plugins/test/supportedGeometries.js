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

    describe('supportedGeometries', function() {
        it ('replies supported geometries for provided feature code', function() {
            var baseParams = {
                method: 'GET',
                path: '/supportedGeometries',
            }
            var tds61Building = Object.assign(baseParams, { translation: 'TDSv61', fcode: 'AL013' });
            assert.deepEqual(server.handleInputs(tds61Building), ['Point', 'Area']);            
            var tds40Wall = Object.assign(baseParams, { translation: 'TDSv40', fcode: 'AL260' })
            assert.deepEqual(server.handleInputs(tds40Wall), [ 'Line' ]);
            var mgcpFord = Object.assign(baseParams, { translation: 'MGCP', fcode: 'BH070' });
            assert.deepEqual(server.handleInputs(mgcpFord), [ 'Line', 'Point' ]);
            var ggdm30Tower = Object.assign(baseParams, { translation: 'GGDMv30' , fcode: 'AL241'})
            assert.deepEqual(server.handleInputs(ggdm30Tower), [ 'Point', 'Area' ]); 
        })
    });

});