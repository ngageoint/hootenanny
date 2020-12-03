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
    this.timeout(5000);


    it('should translate Bridge (AQ040) Area from tdsv61 -> osm -> tdsv61', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="F_CODE" v="AQ040"/>\
                        </way>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateFrom'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "bridge");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "yes");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "area");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "yes");

        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateTo'
        });

        // console.log(tds_xml);

        xml = parser.parseFromString(tds_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "F_CODE");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "AQ040");

    });



    it('should translate Aqueduct (BH010) Area from tdsv61 -> osm -> tdsv61', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="F_CODE" v="BH010"/>\
                        </way>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateFrom'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "bridge");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "aqueduct");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "area");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "yes");

        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateTo'
        });

        // console.log(tds_xml);

        xml = parser.parseFromString(tds_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "F_CODE");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "BH010");

    });


});
