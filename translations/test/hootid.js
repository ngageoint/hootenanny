var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {
    this.timeout(5000);

    hoot.Settings.set({"writer.include.id.tag":"true"});

    var cnvtOps = hoot.Settings.get("convert.ops");
    console.log('Convert: ' + cnvtOps);
    hoot.Settings.set({"writer.include.id.tag":"true"});
    hoot.Settings.set({"convert.ops":"AddElementIdVisitor;SchemaTranslationVisitor"});
    hoot.Settings.set({"ogr.debug.dumptags": "true"});

    it('should translate an OSM feature to GGDM and include a hoot:id tag', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="8675309" action="modify" visible="true">\
                            <nd ref="200" />\
                            <nd ref="400" />\
                            <nd ref="600" />\
                            <tag k="highway" v="road"/>\
                        </way>\
                    </osm>';

        var ggdm_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'GGDMv30',
            path: '/translateTo'
        });

        console.log(ggdm_xml);

        xml = parser.parseFromString(ggdm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "GGDMv30");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "OSMTAGS");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "GB055");
    });

    hoot.Settings.set({"writer.include.id.tag":"false"});
    hoot.Settings.set({"ogr.debug.dumptags": "false"});
    hoot.Settings.set({"convert.ops":cnvtOps});

});
