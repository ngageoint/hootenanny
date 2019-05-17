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


    it('should translate Runway (F_CODE=GB055) with direction tags from tdsv61 -> osm -> tdsv61', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651" />\
                        <node id="-11" action="modify" visible="true" lat="0.68341620728" lon="18.45091527847" />\
                        <node id="-12" action="modify" visible="true" lat="0.68306209303" lon="18.45157116983" />\
                        <node id="-13" action="modify" visible="true" lat="0.68270797876" lon="18.45141400736" />\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="F_CODE" v="GB055"/>\
                            <tag k="GB052_RIDH" v="4"/>\
                            <tag k="GB052_RIDL" v="22"/>\
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
        var gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

        var tags = gj.features[0].properties;
        assert.equal(tags['aeroway'], 'runway');
        assert.equal(tags['aeroway:runway_designator_high'], '4');
        assert.equal(tags['aeroway:runway_designator_low'], '22');

        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateTo'
        });

        // console.log(tds_xml);

        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");

        var tags = gj.features[0].properties;
        assert.equal(tags["F_CODE"], 'GB055');
        assert.equal(tags["GB052_RIDH"], '4');
        assert.equal(tags["GB052_RIDL"], '22');

    });


    it('should translate Runway (F_CODE=GB055) from osm -> tdsv61 even while missing nodes from the OSM', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="aeroway" v="runway"/>\
                        </way>\
                    </osm>';

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
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "GB055");

    });


    it('should translate Stopway (F_CODE=GB045) from tdsv61 -> osm -> tdsv61 even while missing nodes from the OSM', function() {

        var tds_xml = '<osm version="0.6">\
                        <way id="-19">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="F_CODE" v="GB045"/>\
                        </way>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: tds_xml,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateFrom'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "aeroway");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "stopway");


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
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "GB045");

    });


});

