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


    it('should translate a mixture of valid and invalid tags from osm -> tdsv61', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="building" v="yes"/>\
                        </way>\
                        <way id="-20" action="modify" visible="true">\
                            <nd ref="-3" />\
                            <nd ref="-4" />\
                            <nd ref="-5" />\
                            <nd ref="-6" />\
                            <nd ref="-3" />\
                            <tag k="use" v="residential"/>\
                        </way>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateTo'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "F_CODE");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "AL013");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "No Valid Feature Code");

    });

    it('should translate another mixture of valid and invalid tags from osm -> tdsv61', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="use" v="residential"/>\
                        </way>\
                        <way id="-20" action="modify" visible="true">\
                            <nd ref="-3" />\
                            <nd ref="-4" />\
                            <nd ref="-5" />\
                            <nd ref="-6" />\
                            <nd ref="-3" />\
                            <tag k="building" v="yes"/>\
                        </way>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateTo'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "F_CODE");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "AL013");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "No Valid Feature Code");

    });

    it('should translate error from tdsv61 -> osm', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="error" v="No Valid Feature Code"/>\
                        </way>\
                        <way id="-20" action="modify" visible="true">\
                            <nd ref="-3" />\
                            <nd ref="-4" />\
                            <nd ref="-5" />\
                            <nd ref="-6" />\
                            <nd ref="-3" />\
                            <tag k="tag1" v="{\'use\':\'residential\'}"/>\
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
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "No Valid Feature Code");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "tag1");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "{\'use\':\'residential\'}");

    });

});

