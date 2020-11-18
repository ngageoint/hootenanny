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

    it('should translate ZI001_SDP and ZI001_SDV from ggdmv30 -> osm', function() {

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
                            <tag k="F_CODE" v="ZI039"/>\
                            <tag k="ZI001_SDP" v="DigitalGlobe"/>\
                            <tag k="ZI001_SDV" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'GGDMv30',
            path: '/translateFrom'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);
        var gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

        var tags = gj.features[0].properties;
        assert.equal(tags['source:metadata'], 'entity');
        assert.equal(tags['source'], 'DigitalGlobe');
        assert.equal(tags['source:datetime'], '2017-11-02 06:17:23');

    });


    it('should translate source and source:datetime to ZI001_SDP and ZI001_SDV osm -> ggdmv30', function() {

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
                            <tag k="area" v="yes"/>\
                            <tag k="source:metadata" v="entity"/>\
                            <tag k="source" v="DigitalGlobe"/>\
                            <tag k="source:datetime" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';
        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'GGDMv30',
            path: '/translateTo'
        });

        // console.log(tds_xml);
        
        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "GGDMv30");

        var tags = gj.features[0].properties;
        assert.equal(tags["F_CODE"], 'ZI039');
        assert.equal(tags["ZI001_SDP"], 'DigitalGlobe');
        assert.equal(tags["ZI001_SDV"], '2017-11-02 06:17:23');

    });

    it('should translate source:imagery and source:imagery:datetime to ZI001_SDP and ZI001_SDV osm -> ggdmv30', function() {

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
                            <tag k="area" v="yes"/>\
                            <tag k="source:metadata" v="entity"/>\
                            <tag k="source:imagery" v="DigitalGlobe"/>\
                            <tag k="source:imagery:datetime" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';
        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'GGDMv30',
            path: '/translateTo'
        });

        // console.log(tds_xml);
        
        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "GGDMv30");

        var tags = gj.features[0].properties;
        assert.equal(tags["F_CODE"], 'ZI039');
        assert.equal(tags["ZI001_SDP"], 'DigitalGlobe');
        assert.equal(tags["ZI001_SDV"], '2017-11-02 06:17:23');

    });

    it('should translate source over source:imagery to ZI001_SDP osm -> ggdmv30', function() {

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
                            <tag k="area" v="yes"/>\
                            <tag k="source:metadata" v="entity"/>\
                            <tag k="source" v="OpenStreetMap"/>\
                            <tag k="source:imagery" v="DigitalGlobe"/>\
                        </way>\
                    </osm>';
        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'GGDMv30',
            path: '/translateTo'
        });

        // console.log(tds_xml);
        
        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "GGDMv30");

        var tags = gj.features[0].properties;
        assert.equal(tags["F_CODE"], 'ZI039');
        assert.equal(tags["ZI001_SDP"], 'OpenStreetMap');

    });

    it('should translate source:datetime over source:imagery:datetime to ZI001_SDV osm -> ggdmv30', function() {

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
                            <tag k="area" v="yes"/>\
                            <tag k="source:metadata" v="entity"/>\
                            <tag k="source:datetime" v="2017-07-02"/>\
                            <tag k="source:imagery:datetime" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';
        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'GGDMv30',
            path: '/translateTo'
        });

        // console.log(tds_xml);
        
        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "GGDMv30");

        var tags = gj.features[0].properties;
        assert.equal(tags["F_CODE"], 'ZI039');
        assert.equal(tags["ZI001_SDV"], '2017-07-02');

    });

});
