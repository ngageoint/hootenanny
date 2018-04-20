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
    describe('handleInputs', function() {

        it('should translate Land Water Boundary (BA010) from tds61 -> osm -> tds61', function() {
            
            var data = '<osm version="0.6" generator="JOSM"><way id="-38983" visible="true"><nd ref="-38979" /><nd ref="-38982" /> <tag k="F_CODE" v="BB081" /><tag k="MCC" v="999" /><tag k="PWC" v="999" /></way></osm>'            
            var osm_xml = server.handleInputs({
                    osm: data,
                    method: 'POST',
                    translation: 'TDSv61',
                    path: '/translateFrom'
                })

                xml2js.parseString(osm_xml, function(err, result) {
                    if (err) console.error(err);
                    console.log(result.osm.way[0].tag)
                //     assert.equal(result.osm.way[0].tag[1].$.k, 'F_CODE')
                //     assert.equal(result.osm.way[0].tag[1].$.v, 'BA010')
                //     assert.equal(result.osm.way[0].tag[2].$.k, 'SLT')
                //     assert.equal(result.osm.way[0].tag[2].$.v, '6')
                })

                var tds_xml = server.handleInputs({
                    osm: osm_xml,
                    method: 'POST',
                    translation: 'TDSv61',
                    path: '/translateTo'
                })

                xml2js.parseString(tds_xml, function(err, result) {
                    if (err) console.error(err);
                    console.log(result.osm.way[0].tag)
                //     assert.equal(result.osm.way[0].tag[1].$.k, 'F_CODE')
                //     assert.equal(result.osm.way[0].tag[1].$.v, 'BA010')
                //     assert.equal(result.osm.way[0].tag[2].$.k, 'SLT')
                //     assert.equal(result.osm.way[0].tag[2].$.v, '6')
                })
            })
    })
})
