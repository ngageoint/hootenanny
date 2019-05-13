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

    it('should translate SDP and SDV from mgcp -> osm', function() {

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
                            <tag k="FCODE" v="AL015"/>\
                            <tag k="SDP" v="DigitalGlobe"/>\
                            <tag k="SDV" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateFrom'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);
        var gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

        var tags = gj.features[0].properties;
        assert.equal(tags['building'], 'yes');
        assert.equal(tags['source'], 'DigitalGlobe');
        assert.equal(tags['source:datetime'], '2017-11-02 06:17:23');

    });


    it('should translate source and source:datetime to SDP and SDV osm -> mgcp', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
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
                            <tag k="building" v="yes"/>\
                            <tag k="source" v="DigitalGlobe"/>\
                            <tag k="source:datetime" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';
        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(tds_xml);
        
        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");

        var tags = gj.features[0].properties;
        assert.equal(tags["FCODE"], 'AL015');
        assert.equal(tags["SDP"], 'DigitalGlobe');
        assert.equal(tags["SDV"], '2017-11-02 06:17:23');

    });

    it('should translate source:imagery and source:imagery:datetime to SDP and SDV osm -> mgcp', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
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
                            <tag k="building" v="yes"/>\
                            <tag k="source:imagery" v="DigitalGlobe"/>\
                            <tag k="source:imagery:datetime" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';
        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(tds_xml);
        
        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");

        var tags = gj.features[0].properties;
        assert.equal(tags["FCODE"], 'AL015');
        assert.equal(tags["SDP"], 'DigitalGlobe');
        assert.equal(tags["SDV"], '2017-11-02 06:17:23');

    });

    it('should translate source over source:imagery to SDP osm -> mgcp', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
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
                            <tag k="building" v="yes"/>\
                            <tag k="source" v="OpenStreetMap"/>\
                            <tag k="source:imagery" v="DigitalGlobe"/>\
                        </way>\
                    </osm>';
        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(tds_xml);
        
        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");

        var tags = gj.features[0].properties;
        assert.equal(tags["FCODE"], 'AL015');
        assert.equal(tags["SDP"], 'OpenStreetMap');

    });

    it('should translate source:datetime over source:imagery:datetime to SDV osm -> mgcp', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
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
                            <tag k="building" v="yes"/>\
                            <tag k="source:datetime" v="2017-07-02"/>\
                            <tag k="source:imagery:datetime" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';
        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(tds_xml);
        
        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");

        var tags = gj.features[0].properties;
        assert.equal(tags["FCODE"], 'AL015');
        assert.equal(tags["SDV"], '2017-07-02');

    });

});
